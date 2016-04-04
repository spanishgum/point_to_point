#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct DataFile {
	std::string path;
	int dim;
	inline DataFile(std::string s, int d) {
		this->path = s;
		this->dim = d;
	}
};


class Data {
	private:
		std::string file;
		float **matrix;
		int matrix_dim;
	public:
		Data(struct DataFile);
		~Data();
		void allocMatrix();
		void deallocMatrix();
		void readData();
		void getData();
		void getData(int i);
		inline int getDim() const { return this->matrix_dim; }

};	



#endif
