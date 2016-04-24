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
#include <limits>
#include <future>


#include "import.h"
#include "lemon/core.h"
#include "lemon/list_graph.h"
#include "lemon/matching.h"
#include "lemon/kruskal.h"
#include "lemon/dijkstra.h"
#include "lemon/min_cost_arborescence.h"

using namespace lemon;

class Lemon {
	private:
		ListGraph *graph;                                   // Undirected graph
		ListGraph::EdgeMap<float> weights;                  // Edge value
		ListGraph::NodeMap<int> n2idx;                      // Mapping of Node to Node id
		std::map<int, ListGraph::Node> idx2n;               // Mapping of Node id to Node
		ListGraph::EdgeMap< std::pair<int, int> > e2n;      // Source-Target edge structure
        std::pair<int, ListGraph::Node> disCenter;          // Distribution Center

        float dijkstrasTotalMinDistance(ListGraph::Node &); 
	        
	public:
		Lemon(std::vector<Vertex>, ListGraph *);
		~Lemon();

        void initDistributionCenter();
        void initDistributionCenterSeq();

        /*  Accessors   */
		int getN2idx(ListGraph::Node);

        /*  Algorithms  */
        void minCost();                                     // Min Cost Arborescence
        void kruskalsTrim(); 
		void kruskalsMinSpanningTree();
		void weightedMatching();
		void dijkstrasShortestPath();

		static void weightedMatching(Lemon&);

		const std::vector<std::string> funcName = { "Weighted Matching",
                                                    "Kruskal", 
                                                    "Dijkstra", 
                                                    "Kruskal's & Dijkstra's Combo",
                                                    "Kruskal's & Dijkstra's Combo Seq",
                                                    "Dijkstra Seq" };


};

#endif
