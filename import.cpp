#include "import.h"

Data::Data() {
	this->file = "";
	this->matrixInUse = false;
}

Data::~Data() {

}

void Data::allocMatrix(int x, int y) {
	if (this->matrixInUse) {
		throw "Data matrix already in use";
	}
	else {
		this->matrix_x = x;
		this->matrix_y = y;
		this->matrix = new float *[y];
		for (int i = 0; i < y; ++i)
			this->matrix[i] = new float[x];
		this->matrixInUse = true;
	}
}

void Data::deallocMatrix() {
	for (int i = 0; i < this->matrix_y; ++i)
		delete this->matrix[i];
	delete this->matrix;
	this->matrixInUse = false;
}

void Data::readData(std::string file) {
	std::ifstream ifs(file);
	if (!ifs) {
		throw "Invalid data file name";		
	}
	std::string line;
	while (std::getline(ifs, line)) {
		std::cout << line;		
	}
}

void Data::getData(std::string file) {
	
}
