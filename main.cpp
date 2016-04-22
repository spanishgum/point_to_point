#include "lemon_graph.h"

#include <ctime>
#include <functional>
#include <thread>
#include <mutex>

void timeit(void(Lemon::*fptr)(void), Lemon& L){
	clock_t beg = clock();
	(L.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;

	/*Can either pass the algName as a parameter or we can 
	 * create a map of Lemon::*fptr to AlgName*/
	//printResult(algName, result);
	std::cout << "\n" << result << "s\n";
}
void timeit(void(Data::*)(void), Data&);
void printResult(std::string, float);


std::mutex mu;

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
	
	clock_t beg = clock();	
	std::thread weightedMatchingThread([&] {L.weightedMatching();});
	std::thread kruskalsThread([&] {timeit(&Lemon::kruskalsMinSpanningTree, L);});
	//timeit((&Lemon::weightedMatching), L);
	// L.kruskalsMinSpanningTree();
	L.dijkstrasShortestPath();
	
	weightedMatchingThread.join();
	kruskalsThread.join();
	float result = (float)(clock() - beg) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
	return 0;
}

/*void timeit(void(Lemon::*fptr)(void), Lemon& L) {
	clock_t beg = clock();
	(L.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;

	//Can either pass the algName as a parameter or we can 
	 // create a map of Lemon::*fptr to AlgName
	//printResult(algName, result);
	std::cout << "\n" << result << "s\n";
}*/

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
	
	std::cout << "\n" << "Time it took " << algName << " to complete: " << result << " seconds\n";
	//could also store results here if we wanted to use them for comparison later
	
	mu.unlock();
}

