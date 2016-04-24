#include "lemon_graph.h"

/**
 *  Configures Lemon Object
 *
 *  @param adjacency_list       List of grpah vertices
 *  @param g                    Empty LisGraph Object
 */
Lemon::Lemon(std::vector<Vertex> adjacency_list, ListGraph *g) 
	: graph(g), weights(*g), n2idx(*g), e2n(*g), numThreads(50) {

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

    if (this->idx2n.size() < numThreads)
        this->numThreads = this->idx2n.size();
}

/**
 *  ~Destructor
 *
 *  @param none
 *  @return none
 */
Lemon::~Lemon() {

}

/**
 *  Runs Dijkstra's algorithm in parallel on each 
 *  node to every other node and selects the node,
 *  that provides the total minimum distance, as
 *  the distribution center.
 *
 *  @param none
 *  @return none
 */
void Lemon::initDistributionCenter() {
    float cur, min = std::numeric_limits<float>::max();
    unsigned int NUM_NODES = this->idx2n.size();
    
    for (unsigned int t = 0; t < std::ceil(NUM_NODES / this->numThreads); ++t) { 
        std::vector<std::future<float>> distances;
        for (unsigned int i = t*this->numThreads; i < (t+1)*this->numThreads && i < this->idx2n.size(); ++i) {
            distances.push_back(std::async(std::launch::async,
                                            &Lemon::dijkstrasTotalMinDistance,
                                            this,
                                            std::ref(this->idx2n[i])));
        }

        for (unsigned int i = t*this->numThreads, j = 0; i < (t+1)*this->numThreads && i < this->idx2n.size(); ++i, ++j) {
            // Thread waits for computation to end on call to get()
            if ((cur = distances[j].get()) <= min) {
                this->disCenter.first = i; 
                this->disCenter.second = this->idx2n[i];
                min = cur;
            }
        }
        distances.clear();
    }
    
    std::cout << "Found DistributionCenter: " << disCenter.first 
              << " Total Distance: " << min <<  std::endl;
}

/**
 *  Runs Dijkstra's algorithm, sequentially,  on each 
 *  node to every other node and selects the node
 *  that provides the total minimum distance, as
 *  the distribution center.
 *
 *  @param none
 *  @return none
 */
void Lemon::initDistributionCenterSeq() {
    float cur, min = std::numeric_limits<float>::max();

    for (auto n : this->idx2n) {
        if ((cur = dijkstrasTotalMinDistance(n.second)) <= min) {
            this->disCenter.first = n.first;
            this->disCenter.second = n.second;
            min = cur;
        }
    }
    std::cout << "Found DistributionCenter: " << disCenter.first 
              << " Total Distane: " << min << std::endl;
}

/**
 *  Performs Edmond's blossom shrinking algorithm to calculate
 *  maximum weighted matching in an undirected graph.
 *
 *  @param none
 *  @return none
 */
void Lemon::weightedMatching() {
	MaxWeightedMatching< ListGraph, ListGraph::EdgeMap<float> > 
		M(*(this->graph), this->weights);
	M.run();
	std::vector< std::pair<int, int> > results;
	for (ListGraph::NodeIt n(*(this->graph)); n != INVALID; ++n) {
		if (M.mate(n) == INVALID) continue;
		results.push_back(std::make_pair(this->n2idx[n], this->n2idx[M.mate(n)]));
	}
	std::cout << "Max weighted matching edge set:\n";
	for (auto r : results)
		std::cout << r.first << ":" << r.second << "\n";
}

/**
 *  Performs Edmond's blossom shrinking algorithm to calculate
 *  maximum weighted matching in an undirected graph.
 *
 *  @param L                Lemon Object 
 *  @return none
 */
void Lemon::weightedMatching(Lemon &L) {
	MaxWeightedMatching< ListGraph, ListGraph::EdgeMap<float> > 
		M(*(L.graph), L.weights);
	M.run();
	std::vector< std::pair<int, int> > results;
	for (ListGraph::NodeIt n(*(L.graph)); n != INVALID; ++n) {
		if (M.mate(n) == INVALID) continue;
		results.push_back(std::make_pair(L.getN2idx(n), L.getN2idx(M.mate(n))));
	}
	std::cout << "Max weighted matching edge set:\n";
	for (auto r : results)
		std::cout << r.first << ":" << r.second << "\n";
}

int Lemon::getN2idx(ListGraph::Node n){
	return n2idx[n];
}

/**
 *  Performs Min Cost Arborescence's algorithm to determine
 *  the minimum cost subgraph, spanning all nodes reachable
 *  from the distribution center.
 *
 *  @param none
 *  @return none
 */
void Lemon::minCost() {
    MinCostArborescence< ListGraph, ListGraph::EdgeMap<float >>
        M(*(this->graph), this->weights);

    M.init();
    M.addSource(disCenter.second);
    M.start();
    std::cout << "Running Min Cost Arborescence: "
              << M.arborescenceCost() << std::endl;
}

/**
 *  Performs Kruskals algorithm on this->graph, and
 *  transforms it to a minimum cost spanning tree.
 *
 *  @param none
 *  @return none
 */
void Lemon::kruskalsTrim() {
    ListGraph::EdgeMap<bool> treeMap(*this->graph);

	std::cout << "\nWeight of the minimum spanning tree: " 
		<< kruskal(*this->graph, this->weights, treeMap) << "\n";

	for (ListGraph::EdgeIt e(*this->graph); e != INVALID; ++e) {
		if (treeMap[e]) {
           this->graph->erase(e); 
           this->weights[e] = std::numeric_limits<float>::max();
        }
	}
}

/**
 *  Finds a minimum cost spanning tree.
 *
 *  @param none
 *  @return none
 */
void Lemon::kruskalsMinSpanningTree() {
	ListGraph::EdgeMap<bool> treeMap(*this->graph);
	std::cout << "\nWeight of the minimum spanning tree: " 
		<< kruskal(*this->graph, this->weights, treeMap) << "\n";
	int k = 0;
	for (ListGraph::EdgeIt e(*this->graph); e != INVALID; ++e) {
		if (treeMap[e]) {
			// std::cout << this->graph->id(e) << "\n";
			//std::cout << e2n[e].first << ":" << e2n[e].second << "\n";
			++k;
		} 
	}
	std::cout << "\n";	
}

/**
 *  Determines the minimum total distance from the
 *  source node to every other node in the graph.
 *
 *  @param s            Potential facility node 
 *  @return             Total minimum distance to every node
 */
float Lemon::dijkstrasTotalMinDistance(ListGraph::Node &s) {
    float distance = 0;
    for (auto t : this->idx2n) {
        auto d = Dijkstra<ListGraph, ListGraph::EdgeMap<float> >(*(this->graph), this->weights);
        d.run(s);
        distance += d.dist(t.second);
    }
    //std::cout << this->n2idx[s] << " total min distance: " << distance << std::endl;
    return distance;
}

/**
 *  Performs Dijkstra's algorithm to find theshortest paths
 *  from the facility node, when all arc lengths are non-negative.
 *
 *  @param none
 *  @return none
 */
void Lemon::dijkstrasShortestPath() {

    std::cout << "Dijkstra Shortest Path" << std::endl;
        
    for (auto t : this->idx2n) {
        auto d = Dijkstra<ListGraph, ListGraph::EdgeMap<float> >(*(this->graph), this->weights);
        d.run(this->disCenter.second);

        std::cout << "The distance of node t from node s: "
                  << d.dist(t.second) << std::endl;

        std::cout << "Shortest path from " << this->graph->id(disCenter.second) << " to "
                  << this->graph->id(t.second)
                  << " goes through the following nodes: ";
        
        for (ListGraph::Node v = t.second; v != disCenter.second; v = d.predNode(v)) {
            std::cout << this->graph->id(v) << "->";
        }
        std::cout << this->graph->id(disCenter.second) << std::endl;
    }
    
}
