#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace DataFiles {
	struct KN57 {
		std::string path = "KN57/dist.txt";
		int dim = 57;
	};
	struct HA30 {
		std::string path = "HA30/dist.txt";
		int dim = 30;
	};
	struct USCAP {
		std::string path = "HA30/ll.txt";
		// int dim = 50; // no matrix for this dataset
	};
};


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
