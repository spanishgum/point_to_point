#include "lemon_graph.h"

#include <ctime>
#include <functional>

void timeit(void(Lemon::*)(void), Lemon&);
void timeit(void(Data::*)(void), Data&);

int main() {

	std::cout << "\nTesting KN57\n-------------------\n\n";
	DataFile KN57("KN57/dist.txt", 57);
	Data D1(KN57);
	// for (int i = 0; i < D1.getDim(); ++i)
		// D1.getData(i);
	// D1.createGraph();
	timeit(&Data::createGraph, D1);
	D1.outputGraph("KN57/test_out.txt");
	D1.importGraph("KN57/test_out.txt");
	// D1.testImport();
	
	
	std::cout << "\nTesting HA30\n-------------------\n\n";
	DataFile HA30("HA30/dist.txt", 30);
	Data D2(HA30);
	// for (int i = 0; i < D2.getDim(); ++i)
		// D2.getData(i);
	D2.createGraph();
	D2.outputGraph("HA30/test_out.txt");
	D2.importGraph("HA30/test_out.txt");
	// D2.testImport();
	
	
	lemon::ListDigraph LDG;
	
	lemon::ListGraph LG;
	Lemon L(D2.getGraph(), &LG, &LDG);
	L.test();
	
	timeit((&Lemon::weightedMatching), L);
	// L.kruskalsMinSpanningTree();
	L.dijkstrasShortestPath();
	
	
	return 0;
}

void timeit(void(Lemon::*fptr)(void), Lemon& L) {
	clock_t beg = clock();
	(L.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}

void timeit(void(Data::*fptr)(void), Data& D) {
	clock_t beg = clock();
	(D.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}
