#include "lemon_graph.h"


/*
GOALS:
-graph connectivity
-max weighed matching
minimum cost flow analysis
minimum Steiner trees
sub graph matching
min and max cuts
parallel algorithms

*/





Lemon::Lemon(std::vector<Vertex> adjacency_list, ListGraph *g) 
	: graph(g), weights(*g), n2idx(*g), e2n(*g) {
	for (auto v : adjacency_list) {
		ListGraph::Node n = this->graph->addNode();
		this->n2idx[n] = v.id;
		this->idx2n.insert(std::make_pair(v.id, n));
	}
	for (auto v : adjacency_list) {
		for (auto e : v.edges) {
			int e_i = v.id, e_j = e.second;
			float w = e.first;
			ListGraph::Edge E = this->graph->addEdge(idx2n[e_i], idx2n[e_j]);
			this->weights[E] = w;
			this->e2n[E] = std::make_pair(e_i, e_j);	
		}
	}
}

Lemon::~Lemon() {

}

void Lemon::test() {
	for (ListGraph::NodeIt n(*this->graph); n != INVALID; ++n)
		std::cout << n2idx[n] << " ";
	std::cout << "\n";
	for (ListGraph::EdgeIt e(*this->graph); e != INVALID; ++e)
		std::cout << weights[e] << " ";
	std::cout << "\n";
	// for (auto n : this->n2idx)
		// std::cout << n << " ";
	// std::cout << "\n";
	// for (auto n : this->idx2n)
		// std::cout << n << " ";
	// std::cout << "\n";
}

void Lemon::weightedMatching() {
	MaxWeightedMatching< ListGraph, ListGraph::EdgeMap<float> > 
		M(*this->graph, this->weights);
	M.run();
	std::vector< std::pair<int, int> > results;
	for (ListGraph::NodeIt n(*this->graph); n != INVALID; ++n) {
		if (M.mate(n) == INVALID) continue;
		results.push_back(std::make_pair(n2idx[n], n2idx[M.mate(n)]));
	}
	std::cout << "Max weighted matching edge set:\n";
	for (auto r : results)
		std::cout << r.first << ":" << r.second << "\n";
}

void Lemon::kruskalsMinSpanningTree() {
	ListGraph::EdgeMap<bool> treeMap(*this->graph);
	std::cout << "\nWeight of the minimum spanning tree: " 
		<< kruskal(*this->graph, this->weights, treeMap) << "\n";
	int k = 0;
	for (ListGraph::EdgeIt e(*this->graph); e != INVALID; ++e) {
		if (treeMap[e]) {
			// std::cout << this->graph->id(e) << "\n";
			std::cout << e2n[e].first << ":" << e2n[e].second << "\n";
			++k;
		}
	}
	std::cout << "\n";	
}

void Lemon::dijkstrasShortestPath() {
	
}



// ListGraph *graph;
// ListGraph::EdgeMap<float> weights;
// ListGraph::NodeMap<int> n2idx;
// std::map<int, ListGraph::Node> idx2n;



// std::cout << v.id << " : ";
// for (auto e : v.edges) {
	// std::cout << e.first << "," << e.second << " ";
// }
// std::cout << std::endl;