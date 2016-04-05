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

std::list< std::pair<float, int> > Data::findClosestTo(unsigned int i, unsigned int count) {
	if (i >= this->matrix_dim || count >= this->matrix_dim)
		throw "findClosestTo: Bad args.\n";
	float *city = this->matrix[i];
	std::list< std::pair<float, int> > neighbors;
	for (unsigned int j = 0; j < this->matrix_dim; ++j) {
		if (i == j) continue;
		else if (neighbors.size() < count) {
			neighbors.push_back(std::make_pair(city[j], j));
		}
		else {
			neighbors.sort();
			if (city[j] < neighbors.back().first) {
				neighbors.pop_back();
				neighbors.push_back(std::make_pair(city[j], j));
			}
		}
	}
	return neighbors;
}

void Data::createGraph() {
	srand(0);
	for (unsigned int i = 0; i < this->matrix_dim; ++i) {
		Vertex V(i);
		unsigned int num_edges = MIN_DEGREE + (rand() % (MAX_DEGREE - MIN_DEGREE));
		for (auto e : findClosestTo(i, num_edges))
			V.addEdge(e.second);
		this->adjacency.push_back(V);
	}
}

void Data::outputGraph() {
	for (auto v : this->adjacency) {
		std::cout << v.id << " : ";
		for (auto e : v.edges)
			std::cout << e << " ";
		std::cout << std::endl;
	}
}