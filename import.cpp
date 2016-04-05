#include "import.h"

Data::Data(struct DataFile df) {
	this->file = df.path;
	this->matrix_dim = df.dim;
	this->allocMatrix();
	this->readData();
}

Data::~Data() {
	this->deallocMatrix();
}

void Data::allocMatrix() {
	this->matrix = new float *[this->matrix_dim];
	for (unsigned int i = 0; i < this->matrix_dim; ++i)
		this->matrix[i] = new float [this->matrix_dim];
}

void Data::deallocMatrix() {
	for (unsigned int i = 0; i < this->matrix_dim; ++i)
		delete [] this->matrix[i];
	delete [] this->matrix;
}

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

void Data::getData() {
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		for (unsigned int j = 0; j < this->matrix_dim; ++j) {
			std:: cout << this->matrix[i][j] << " ";
		}
		std:: cout << std::endl;
	}
}

void Data::getData(unsigned int city) {
	if (city >= this->matrix_dim)
		throw "Invalid city ID.\n";
	for (unsigned int i = 0; i < this->matrix_dim; ++i)
		std:: cout << this->matrix[city][i] << " ";
	std::cout << std::endl;
}

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

void Data::addEdgeToVertex(int v) {
	Vertex &vertex = adjacency[v];
	unsigned int new_degree = vertex.edges.size() + 1;
	if (new_degree > this->matrix_dim) return;
	// std::cout << "adding edges to " << v << ":\n";
	for (auto e : findClosestTo(v, new_degree)) {
		vertex.addEdge(e);
		// std::cout << e.second << " ";
	}
	// std::cout << std::endl;
	// std::cin.get();
}

bool Data::checkGraphIsConnected() {
	this->DFSGraphUtil(0);
	bool res = 1;
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		if (this->visited[i] == 0) { // add edge to make connected
			this->addEdgeToVertex(i);
		}
		else {
			// std::cout << i << " is connected:\n";
			// for (auto e : adjacency[i].edges) {
				// std::cout << e.second << " ";
			// }
			// std::cout << std::endl;
		}
		res &= visited[i];
	}
	return res;
}

// A recursive function to print DFS starting from v
void Data::DFSGraphUtil(int v) {
	// Mark the current node as visited and print it
	this->visited[v] = true;
	// std::cout << "=================" << v << "\n";
	// std::cin.get();
	std::vector< std::pair<float, int> > &v_edges = this->adjacency[v].edges;
	// Recur for all the vertices adjacent to this vertex
	for (unsigned int e = 0; e < v_edges.size(); ++e) {
		if (!this->visited[v_edges[e].second])
			this->DFSGraphUtil(v_edges[e].second);
	}
}


void Data::createGraph() {
	srand(0);
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		Vertex V(i);
		unsigned int num_edges = MIN_DEGREE + (rand() % (MAX_DEGREE - MIN_DEGREE));
		for (auto e : findClosestTo(i, num_edges))
			V.addEdge(e);
		this->adjacency.push_back(V);
	}
	this->visited = new bool[this->matrix_dim];
	for (unsigned int i = 0; i < this->matrix_dim; ++i) 
		this->visited[i] = 0;
	while (!checkGraphIsConnected()) {
		std::cerr << "Attempting to connect graph by adding more edges.\n";
		// for (unsigned int i = 0; i < this->matrix_dim; ++i)
			// std::cout << this->visited[i];
		// std::cout << std::endl;
		// std::cin.get();
	}
	delete [] this->visited;
}

void Data::outputGraph() {
	for (auto v : this->adjacency) {
		std::cout << v.id << " : ";
		for (auto e : v.edges)
			std::cout << e.second << " ";
		std::cout << std::endl;
	}
}

std::vector<Vertex> Data::getGraph() {
	return this->adjacency;
}