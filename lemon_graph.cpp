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





Lemon::Lemon(std::vector<Vertex> adjacency_list, ListGraph *g, ListDigraph *dg) 
	: graph(g), weights(*g), n2idx(*g), e2n(*g),
        digraph(dg), weightsDi(*dg), arcMap(*dg), n2idxDi(*dg)  {

	for (auto v : adjacency_list) {
		ListGraph::Node n = this->graph->addNode();
		this->n2idx[n] = v.id;
		this->idx2n.insert(std::make_pair(v.id, n));

                ListDigraph::Node dN = this->digraph->addNode();
                this->n2idxDi[dN] = v.id;
                this->idx2nDi.insert(std::make_pair(v.id, dN));
	}
	for (auto v : adjacency_list) {
		for (auto e : v.edges) {
			int e_i = v.id, e_j = e.second;
			float w = e.first;
			ListGraph::Edge E = this->graph->addEdge(idx2n[e_i], idx2n[e_j]);
			this->weights[E] = w;
			this->e2n[E] = std::make_pair(e_i, e_j);	

                        ListDigraph::Arc a = this->digraph->addArc(idx2nDi[e_i], idx2nDi[e_j]);
                        this->weightsDi[a] = w;
		}
	}
}

Lemon::~Lemon() {

}

void Lemon::test() {
        /*
	for (ListGraph::NodeIt n(*this->graph); n != INVALID; ++n)
	    std::cout << n2idx[n] << " ";
	std::cout << "\n";
	for (ListGraph::EdgeIt e(*this->graph); e != INVALID; ++e)
        		std::cout << weights[e] << " ";
        std::cout << "\n";
	for (auto n : this->n2idx)
	    std::cout << n << " ";
	std::cout << "\n";
	for (auto n : this->idx2n)
	    std::cout << n << " ";
	std::cout << "\n";

        for (ListDigraph::ArcIt a(*this->digraph); a != INVALID; ++a) {
            std::cout << "(" << this->digraph->id(this->digraph->source(a)) 
                      << "," << this->digraph->id(this->digraph->target(a))
                      << ") -  " << this->weightsDi[a] << std::endl;;
        }
        */
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

    std::cout << "Dijkstra Shortest Path" << std::endl;
    
    // Facility node
    std::srand(std::time(NULL));
    std::map<int, ListDigraph::Node>::iterator startPair = idx2nDi.begin();
    std::advance(startPair, std::rand() % idx2nDi.size());
    ListDigraph::Node s = startPair->second;

    std::cout << "Facility Node: " << this->digraph->id(s) << std::endl;
   
    
    for (auto t : this->idx2nDi) {
        auto d = Dijkstra<ListDigraph, ListDigraph::ArcMap<float> >(*(this->digraph), this->weightsDi);
        d.run(s);

        std::cout << "The distance of node t from node s: "
                  << d.dist(t.second) << std::endl;

        std::cout << "Shortest path from " << this->digraph->id(s) << " to "
                  << this->digraph->id(t.second)
                  << " goes through the following nodes: ";
        
        for (ListDigraph::Node v = t.second; v != s; v = d.predNode(v)) {
            std::cout << this->digraph->id(v) << "->";
        }
        std::cout << this->digraph->id(s) << std::endl;
    }
    
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
