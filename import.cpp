#include "import.h"

Data::Data(struct DataFile df) {
	this->file = df.path;
	this->matrix_dim = df.dim;
	this->allocMatrix();
}

Data::~Data() {
	this->deallocMatrix();
}

void Data::allocMatrix() {
	this->matrix = new float *[this->matrix_dim];
	for (int i = 0; i < this->matrix_dim; ++i)
		this->matrix[i] = new float [this->matrix_dim];
}

void Data::deallocMatrix() {
	for (int i = 0; i < this->matrix_dim; ++i)
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
		for (int i = 0; i < this->matrix_dim; ++i) {
			ss >> val;
			this->matrix[line_num][i] = val;
		}
		++line_num;
	}
}

void Data::getData() {
	for (int i = 0; i < this->matrix_dim; ++i) {
		for (int j = 0; j < this->matrix_dim; ++j) {
			std:: cout << this->matrix[i][j] << " ";
		}
		std:: cout << std::endl;
	}
}


void Data::getData(int city) {
	if (city >= this->matrix_dim)
		throw "Invalid city ID.\n";
	for (int i = 0; i < this->matrix_dim; ++i)
		std:: cout << this->matrix[city][i] << " ";
	std::cout << std::endl;
}