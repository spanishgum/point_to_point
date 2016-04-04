#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>
#include <string>

class Data {
	private:
		std::string file;
		float **matrix;
		int matrix_x, matrix_y;
		bool matrixInUse;
	public:
		Data();
		~Data();
		void allocMatrix(int x, int y);
		void deallocMatrix();
		void getData(std::string file);
		void readData(std::string file);

};	



#endif
