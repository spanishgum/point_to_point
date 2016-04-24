#include "import.h"

/*
 * #Data(struct DataFile)
 * Constructor for a Data object
 *
 * @Parameters:		struct DataFile that holds input information about a text file
 * @Functionality:	Creates a Data Object from the DataFile struct
 * @Return:		Data object
 */
Data::Data(struct DataFile df) {
	this->file = df.path;
	this->matrix_dim = df.dim;
	this->allocMatrix();
	this->readData();
}

/*
 * #~Data() 
 * Destructor for a Data object
 *
 * @Parameters:
 * @Functionality:	Calls deallocMatrix() to destroy the matrix
 * @Return: 
 */
Data::~Data() {
	this->deallocMatrix();
}

/*
 * #allocMatrix() 
 *
 * @Parameters:
 * @Functionality:	Allocates space for the matrix in the Data object
 * @Return: 
 */
void Data::allocMatrix() {
	this->matrix = new float *[this->matrix_dim];
	for (unsigned int i = 0; i < this->matrix_dim; ++i)
		this->matrix[i] = new float [this->matrix_dim];
}

/*
 * #deallocMatrix() 
 *
 * @Parameters:
 * @Functionality:	Deallocates space for the matrix in the Data object
 * @Return: 
 */
void Data::deallocMatrix() {
	for (unsigned int i = 0; i < this->matrix_dim; ++i)
		delete [] this->matrix[i];
	delete [] this->matrix;
}

/*
 * #readData() 
 *
 * @Parameters:		
 * @Functionality:	Reads data in from the file and inputs the values read in 
 * 			into the matrix
 * @Return:		
 */
void Data::readData() {
	std::ifstream ifs(this->file);
	if (!ifs) {
		throw "Invalid data file name";		
	}
	std::string line;
	int line_num = 0;
	while (std::getline(ifs, line)) {
		std::stringstream ss(line);
		float val;
		for (unsigned int i = 0; i < this->matrix_dim; ++i) {
			ss >> val;
			this->matrix[line_num][i] = val;
		}
		++line_num;
	}
}

/*
 * #getData() 
 *
 * @Parameters:		
 * @Functionality:	Prints out all of the numbers in the matrix
 * @Return:		
 */
void Data::getData() {
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		for (unsigned int j = 0; j < this->matrix_dim; ++j) {
			std:: cout << this->matrix[i][j] << " ";
		}
		std:: cout << std::endl;
	}
}

/*
 * #getData(unsigned int) 
 *
 * @Parameters:		unsigned int identifying the index where a cities information
 * 			is stored in the matrix
 * @Functionality:	Prints out the specific cities information in the matrix
 * @Return:		
 */
void Data::getData(unsigned int city) {
	if (city >= this->matrix_dim)
		throw "Invalid city ID.\n";
	for (unsigned int i = 0; i < this->matrix_dim; ++i)
		std:: cout << this->matrix[city][i] << " ";
	std::cout << std::endl;
}

/*
 * #findClosestTo(unsigned int, unsigned int) 
 *
 * @Parameters:		unsigned int identifying the index of a city in the matrix
 * 			and unsigned int count to limit the number of neighbors a city can have
 * @Functionality:	Creates a vector of neighbors closest to city i up to the count passed in
 * @Return:		vector of pairs containing the weights of the city and the index in the matrix
 * 			of that city
 */
std::vector< std::pair<float, int> > Data::findClosestTo(unsigned int i, unsigned int count) {
	if (i >= this->matrix_dim || count > this->matrix_dim)
		throw "findClosestTo: Bad args.\n";
	float *city = this->matrix[i];
	std::vector< std::pair<float, int> > neighbors;
	for (unsigned int j = 0; j < this->matrix_dim; ++j) {
		if (i == j) continue;
		else if (neighbors.size() < count) {
			neighbors.push_back(std::make_pair(city[j], j));
		}
		else {
			std::sort(neighbors.begin(), neighbors.end());
			if (city[j] < neighbors.back().first) {
				neighbors.pop_back();
				neighbors.push_back(std::make_pair(city[j], j));
			}
		}
	}
	return neighbors;
}

/*
 * #addEdgeToVertex() 
 *
 * @Parameters:		int that indexes where the vertex is in the adjacency list
 * @Functionality:	adds and edge to the edge vector of the Vertex structure
 * @Return:		
 */
void Data::addEdgeToVertex(int v) {
	Vertex &vertex = adjacency[v];
	unsigned int new_degree = vertex.edges.size() + 1;
	if (new_degree > this->matrix_dim) return;
	for (auto e : findClosestTo(v, new_degree)) {
		vertex.addEdge(e);
		this->adjacency[e.second].addEdge(std::make_pair(e.first, vertex.id));
	}
}

/*
 * checkGraphIsConnected() 
 *
 * @Parameters:		
 * @Functionality:	Does a DFS through the nodes to ensure that every not is connected,
 * 			if it is not then it will add edges to the node until it is connected
 * @Return:		bool: false if it is not connected and true if it is connected
 */
bool Data::checkGraphIsConnected() {
	this->DFSGraphUtil(0);
	bool res = 1;
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		if (this->visited[i] == 0) { // add edge to make connected
			this->addEdgeToVertex(i);
		}
		res &= visited[i];
	}
	return res;
}

/*
 * DFSGraphUtil(int) 
 *
 * @Parameters:		int that is the index of a vertex
 * @Functionality:	Does a DFS through the nodes
 * @Return:		
 */
void Data::DFSGraphUtil(int v) {
	// Mark the current node as visited and print it
	this->visited[v] = true;
	std::vector< std::pair<float, int> > &v_edges = this->adjacency[v].edges;
	// Recur for all the vertices adjacent to this vertex
	for (unsigned int e = 0; e < v_edges.size(); ++e) {
		if (!this->visited[v_edges[e].second])
			this->DFSGraphUtil(v_edges[e].second);
	}
}

/*
 * createGraph() 
 *
 * @Parameters:		
 * @Functionality:	Creates a graph based on the adjacency list. The graph is bidirectional,
 * 			or undirected
 * @Return:		
 */
void Data::createGraph() {
	srand(0);
	// populate the adjaency list with random count of nearest neighbors
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		Vertex V(i);
		unsigned int num_edges = MIN_DEGREE + (rand() % (MAX_DEGREE - MIN_DEGREE));
		for (auto e : findClosestTo(i, num_edges)) {
			V.addEdge(e);
		}
		this->adjacency.push_back(V);
	}
	// go back through list and add reverse neighbors (if (e_i -> e_j) then (e_j -> e_i))
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		for (unsigned int e = 0; e < this->adjacency[i].edges.size(); ++e) {
			this->adjacency[this->adjacency[i].edges[e].second].addEdge(std::make_pair(this->adjacency[i].edges[e].first, i));
		}
	}
	this->visited = new bool[this->matrix_dim];
	for (unsigned int i = 0; i < this->matrix_dim; ++i) 
		this->visited[i] = 0;
	while (!checkGraphIsConnected()) {
		std::cerr << "Attempting to connect graph by adding more edges.\n";
	}
	delete [] this->visited;
}

/*
 * outputGraph(string) 
 *
 * @Parameters:		string with the name of the outfile
 * @Functionality:	Outputs the graph to a file
 * @Return:		
 */
void Data::outputGraph(std::string ofile) {
	std::ofstream ofs(ofile);
	for (auto v : this->adjacency) {
		ofs << v.id << " : ";
		for (auto e : v.edges)
			ofs << e.first << "," << e.second << " ";
		ofs << std::endl;
	}
	ofs.close();
}

/*
 * importGraph(string) 
 *
 * @Parameters:		string that is the name of the file containing the graph
 * @Functionality:	Creates a graph based on the input file data
 * @Return:		
 */
void Data::importGraph(std::string ifile) {
	std::ifstream ifs(ifile);
	if (!ifs) {
		std::cerr << "Bad input file name.\n";
		return;
	}
	std::string line;
	int v_id, e_id, e_weight;
	char delim;
	while (std::getline(ifs, line)) {
		std::stringstream ss(line);
		ss >> v_id >> delim;
		Vertex V(v_id);
		while (ss >> e_weight >> delim >> e_id) {
			V.addEdge(std::make_pair(e_weight, e_id));
		}
		this->imported_adjacency.push_back(V);
	}
	ifs.close();
}

/*
 * testImport() 
 *
 * @Parameters:		
 * @Functionality:	Tests the imported graph to ensure it is correct
 * @Return:		
 */
void Data::testImport() {
	for (auto v : this->imported_adjacency) {
		std::cout << v.id << " : ";
		for (auto e : v.edges)
			std::cout << e.first << "," << e.second << " ";
		std::cout << std::endl;
	}
}

/*
 * getGraph() 
 *
 * @Parameters:		
 * @Functionality:	Gets the adjacency list of the imported graph
 * @Return:		returns the vector of vertices
 */
std::vector<Vertex> Data::getGraph() {
	return this->imported_adjacency;
}
