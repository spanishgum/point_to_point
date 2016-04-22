#ifndef LEMON_GRAPH_H
#define LEMON_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sys/stat.h>
#include <algorithm>
#include <map>


#include "import.h"
#include "lemon/core.h"
#include "lemon/list_graph.h"
#include "lemon/matching.h"
#include "lemon/kruskal.h"
#include "lemon/dijkstra.h"

using namespace lemon;

class Lemon {
	private:
		ListGraph *graph;
		ListGraph::EdgeMap<float> weights;
		ListGraph::NodeMap<int> n2idx;
		std::map<int, ListGraph::Node> idx2n;
		ListGraph::EdgeMap< std::pair<int, int> > e2n;

                ListDigraph *digraph;
                ListDigraph::ArcMap<float> weightsDi;
                ListDigraph::ArcMap<int> arcMap;
                ListDigraph::NodeMap<int> n2idxDi;
                std::map<int, ListDigraph::Node> idx2nDi;
	        
	public:
		Lemon(std::vector<Vertex>, ListGraph *, ListDigraph *);
		~Lemon();
		void test();
		int getN2idx(ListGraph::Node);
		void weightedMatching();
		static void weightedMatching(Lemon&);
		void kruskalsMinSpanningTree();
		void dijkstrasShortestPath();

};







#endif
