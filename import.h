#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <list>
#include <sys/stat.h>
#include <algorithm>

#define MIN_DEGREE 2
#define MAX_DEGREE 8

struct DataFile {
	std::string path;
	int dim;
	inline DataFile(std::string s, int d) {
		struct stat buf;
		if (stat(s.c_str(), &buf)) {
			throw "Invalid file path.";
		}
		this->path = s;
		this->dim = d;
	}
};

struct Vertex {
	int id;
	std::list<int> edges;
	inline Vertex(int i) {
		this->id = i;
	}
	inline void addEdge(int i) {
		if (std::find(this->edges.begin(), this->edges.end(), i)
				== this->edges.end())
			this->edges.push_back(i);
	}
};

class Data {
	private:
		std::string file;
		float **matrix;
		unsigned int matrix_dim;
		
		std::list<Vertex> adjacency;
		
		void allocMatrix();
		void deallocMatrix();
		void readData();
		
		std::list< std::pair<float, int> > findClosestTo(unsigned int, unsigned int);
		
	public:
		Data(struct DataFile);
		~Data();
		inline int getDim() const { return this->matrix_dim; }
		void getData();
		void getData(unsigned int i);

		void createGraph();
		void outputGraph();
};	



#endif
