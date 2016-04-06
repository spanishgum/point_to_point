#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>
#include <algorithm>

#define MIN_DEGREE 3
#define MAX_DEGREE 8

struct Vertex {
	int id;
	std::vector< std::pair<float, int> > edges;
	inline Vertex(int i) {
		this->id = i;
	}
	inline void addEdge(std::pair<float, int> weight_id) {
		if (std::find(this->edges.begin(), this->edges.end(), weight_id)
				== this->edges.end())
			this->edges.push_back(weight_id);
	}
	inline bool hasEdge(int id) {
		for (unsigned int e = 0; e < edges.size(); ++e)
			if (edges[e].second == id)
				return true;
		return false;
	}
};


class Graph {
	private:
		std::vector<Vertex> adjacency;
		
	public:
		Graph(std::vector<Vertex>);
		~Graph();
		void showAdjacency();
};



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


class Data {
	private:
		std::string file;
		float **matrix;
		unsigned int matrix_dim;
		
		std::vector<Vertex> adjacency;
		
		std::vector<Vertex> imported_adjacency;
		
		void allocMatrix();
		void deallocMatrix();
		void readData();
		
		std::vector< std::pair<float, int> > findClosestTo(unsigned int, unsigned int);
		
		bool *visited;
		
		void addEdgeToVertex(int);
		bool checkGraphIsConnected();
		void DFSGraphUtil(int);
		
	public:
		Data(struct DataFile);
		~Data();
		inline int getDim() const { return this->matrix_dim; }
		void getData();
		void getData(unsigned int i);

		void createGraph();
		void outputGraph(std::string);
		void importGraph(std::string);
		std::vector<Vertex> getGraph();
		void testImport();
};	



#endif
