#include "import.h"

Data::Data(struct DataFile df) {
	this->file = df.path;
	this->matrix_dim = df.dim;
}

Data::~Data() {

}

void Data::allocMatrix() {
	this->matrix = new float *[this->matrix_dim];
	for (int i = 0; i < this->matrix_dim; ++i)
		this->matrix[i] = new float [this->matrix_dim];
}

void Data::deallocMatrix() {
	for (int i = 0; i < this->matrix_dim; ++i)
		delete this->matrix[i];
	delete this->matrix;
}

void Data::readData(std::string file) {
	std::ifstream ifs(file);
	if (!ifs) {
		throw "Invalid data file name";		
	}
	std::string line;
	while (std::getline(ifs, line)) {
		std::stringstream ss(line);
		float x, y;
		ss >> x >> y;
		std::cout << x << "\t" << y << "\n";
	}
}

void Data::getData(std::string file) {
	
}
