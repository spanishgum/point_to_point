#include "lemon_graph.h"

#include <ctime>
#include <functional>
#include <thread>
#include <mutex>

#define WEIGHTEDMATCHING 0
#define KRUSKAL 1
#define DIJKSTRA 2
#define KRUSKDIJK 3
#define KRUSKDIJKSEQ 4
#define DIJKSTRASEQ 5


void printResult(std::string, float);
void timeit(void(Lemon::*fptr)(void), Lemon& L, int i);
void timeit2(void(Lemon::*spanTree)(void), void(Lemon::*shortPath)(void), Lemon& L, int i);
void timeit(void(Data::*)(void), Data&);

std::mutex mu;

int main() {

	std::cout << "\nTesting KN57\n-------------------\n\n";
	DataFile KN57("KN57/dist.txt", 57);
	Data D1(KN57);
    	D1.createGraph();
	D1.outputGraph("KN57/test_out.txt");
	D1.importGraph("KN57/test_out.txt");

        lemon::ListGraph LG, LG2, LG3;
	Lemon L(D1.getGraph(), &LG);
        Lemon L2(D1.getGraph(), &LG2);
        Lemon L3(D1.getGraph(), &LG3);

        timeit(&Lemon::initDistributionCenterSeq, L, DIJKSTRASEQ);
	timeit(&Lemon::initDistributionCenter, L, DIJKSTRA);
    
        timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, L2, KRUSKDIJKSEQ);
	timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, L3, KRUSKDIJK);
        
	std::cout << "\nTesting HA30\n-------------------\n\n";
	DataFile HA30("HA30/dist.txt", 30);
	Data D2(HA30);
    	D2.createGraph();
	D2.outputGraph("HA30/test_out.txt");
	D2.importGraph("HA30/test_out.txt");
        lemon::ListGraph LG4, LG5, LG6;
	Lemon L4(D2.getGraph(), &LG4);
        Lemon L5(D2.getGraph(), &LG5);
        Lemon L6(D2.getGraph(), &LG6);

        timeit(&Lemon::initDistributionCenterSeq, L4, DIJKSTRASEQ);
	timeit(&Lemon::initDistributionCenter, L4, DIJKSTRA);
    
        timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, L5, KRUSKDIJKSEQ);
	timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, L6, KRUSKDIJK);

        /*
        lemon::ListGraph LG2;
        Lemon L2(D1.getGraph(), &LG2);
        std::thread pureDijkstra ([&] {timeit(&Lemon::initDistributionCenter, L2, DIJKSTRA);});
	std::thread KruskalDijkstra ([&] {timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, L2, KRUSKDIJK);});

	pureDijkstra.join();
	KruskalDijkstra.join();
        */
	
	
        /*
        std::thread pureDijkstraSeq ([&] {timeit(&Lemon::initDistributionCenterSeq, L, DIJKSTRASEQ);});
	std::thread pureDijkstra ([&] {timeit(&Lemon::initDistributionCenter, L, DIJKSTRA);});
    
        std::thread KruskalDijkstraSeq ([&] {timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, L, KRUSKDIJKSEQ);});
	std::thread KruskalDijkstra ([&] {timeit2(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, L, KRUSKDIJK);});

        pureDijkstraSeq.join();
	pureDijkstra.join();
        KruskalDijkstraSeq.join();
	KruskalDijkstra.join();
        */
        /*	
	clock_t beg = clock();	
	std::thread weightedMatchingThread([&] {timeit(&Lemon::weightedMatching, L, WEIGHTEDMATCHING);});
	std::thread kruskalsThread([&] {timeit(&Lemon::kruskalsMinSpanningTree, L, KRUSKAL);});
	std::thread dijkstrasThread([&] {timeit(&Lemon::dijkstrasShortestPath, L, DIJKSTRA);});
	//timeit((&Lemon::weightedMatching), L);
	// L.kruskalsMinSpanningTree();
	//L.dijkstrasShortestPath();
	
	weightedMatchingThread.join();
	kruskalsThread.join();
	dijkstrasThread.join();
	float result = (float)(clock() - beg) / CLOCKS_PER_SEC;
	std::cout << "\nTime it took for all of the algs to run in parallel: " << result << " seconds\n";

        */
	return 0;
}

void timeit(void(Lemon::*fptr)(void), Lemon& L, int i) {
	clock_t beg = clock();
	(L.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;

	//Can either pass the algName as a parameter or we can 
	// create a map of Lemon::*fptr to AlgName
	printResult(L.funcName[i], result);
	//std::cout << "\n" << result << "s\n";
}

void timeit2(void(Lemon::*spanTree)(void), void(Lemon::*shortPath)(void), Lemon& L, int i){
	clock_t beg = clock();

	(L.*spanTree)();
	(L.*shortPath)();
	
	float result = ((float)(clock() - beg)) / CLOCKS_PER_SEC;

	printResult(L.funcName[i], result);
}

void timeit(void(Data::*fptr)(void), Data& D) {
	clock_t beg = clock();
	(D.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}

/*This function allows us to print the times for each function on seperate threads
 * without the chance of them trying to print out at the same time*/
void printResult(std::string algName, float result){
	mu.lock();
	
	std::cout << "Time it took " << algName << " to complete: " << result << " seconds\n\n";
	//could also store results here if we wanted to use them for comparison later
	
	mu.unlock();
}

