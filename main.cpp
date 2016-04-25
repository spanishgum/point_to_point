#include "lemon_graph.h"

#include <ctime>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <cstring>

#define WEIGHTEDMATCHING 0
#define KRUSKAL 1
#define DIJKSTRA 2
#define KRUSKDIJK 3
#define KRUSKDIJKSEQ 4
#define DIJKSTRASEQ 5


void printResult(std::string, int, float);
template< typename T>
void timeit(void(Lemon::*fptr)(void), Lemon& L, int i);
template< typename T>
void timeit2(void(Lemon::*spanTree)(void), void(Lemon::*shortPath)(void), Lemon& L, int i);
void timeit(void(Data::*)(void), Data&);

void testSynthetic(std::string testSyntheticFile, std::string testSyntheticGraph, int n);
void testHA30();
void testKN57();

std::mutex mu;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::seconds seconds;

static void usage() {
	std::cerr << "Usage: ./main [-t0] [-t1] [-d datfile -g graphfile -n N]\n";	
	exit(0);
}

int main(int argc, char *argv[]) {

	std::string d_file;
	std::string g_file;
	int n;
	bool d_flag = 0, g_flag = 0, n_flag = 0;
	
	if (argc < 1) {
		usage();
	}
	else if (argc == 2) {
		if (strcmp(argv[1], "-t0") == 0) {
			testKN57();
			return 0;
		}
		else if (strcmp(argv[1], "-t1") == 0) {
			testHA30();
			return 0;
		}
		else {
			usage();
		}
	}
	else {
		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "-d") == 0) {
				if (++i < argc)
				{
					d_flag = 1;
					d_file = argv[i];
				} 
				else usage();
			}
			else if (strcmp(argv[i], "-g") == 0) {
				if (++i < argc)
				{
					g_flag = 1;
					g_file = argv[i];
				} 
				else usage();
			}
			else if (strcmp(argv[i], "-n") == 0) {
				if (++i < argc)
				{
					n_flag = 1;
					n = atoi(argv[i]);
				} 
				else usage();
			}
			else {
				usage();
			}
		}
	}
	if (d_flag && g_flag && n_flag) {
		testSynthetic(d_file, g_file, n);
	}
	else {
		usage();
	}
	
	return 0;
}

void testSynthetic(std::string testSyntheticFile, std::string testSyntheticGraph, int n) {
	std::cout << "\nTesting sythetic data file '" << testSyntheticFile
		<< "' with 'n = " << n << "'\n------------------------------------------------\n\n";
	
	DataFile N10(testSyntheticFile, n);
	Data D0(N10);
	
	D0.createGraph();
	D0.outputGraph(testSyntheticGraph);
	D0.importGraph(testSyntheticGraph);
	
	lemon::ListGraph D0_LG, D0_LG2, D0_LG3;
	Lemon D0_L(D0.getGraph(), &D0_LG);
	Lemon D0_L2(D0.getGraph(), &D0_LG2);
	Lemon D0_L3(D0.getGraph(), &D0_LG3);
        
        if (n < 500) {
            timeit<milliseconds>(&Lemon::initDistributionCenterSeq, D0_L, DIJKSTRASEQ);
            timeit<milliseconds>(&Lemon::initDistributionCenter, D0_L, DIJKSTRA);
            timeit2<milliseconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, D0_L2, KRUSKDIJKSEQ);
            timeit2<milliseconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, D0_L3, KRUSKDIJK);
            timeit<milliseconds>(&Lemon::kruskalsTrim, D0_L, KRUSKAL);
        } else {
            timeit<seconds>(&Lemon::initDistributionCenterSeq, D0_L, DIJKSTRASEQ);
            timeit<seconds>(&Lemon::initDistributionCenter, D0_L, DIJKSTRA);
            timeit2<seconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, D0_L2, KRUSKDIJKSEQ);
            timeit2<seconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, D0_L3, KRUSKDIJK);
            timeit<seconds>(&Lemon::kruskalsTrim, D0_L, KRUSKAL);
        }
}

void testKN57() {
	std::cout << "\nTesting KN57\n------------------------------------------------\n\n";
	
	DataFile KN57("KN57/dist.txt", 57);
	Data D1(KN57);
	
	D1.createGraph();
	D1.outputGraph("KN57/test_out.txt");
	D1.importGraph("KN57/test_out.txt");
	
	lemon::ListGraph LG, LG2, LG3;
	Lemon L(D1.getGraph(), &LG);
	Lemon L2(D1.getGraph(), &LG2);
	Lemon L3(D1.getGraph(), &LG3);
	
	timeit<milliseconds>(&Lemon::initDistributionCenterSeq, L, DIJKSTRASEQ);
	timeit<milliseconds>(&Lemon::initDistributionCenter, L, DIJKSTRA);
	timeit2<milliseconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, L2, KRUSKDIJKSEQ);
	timeit2<milliseconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, L3, KRUSKDIJK);
	timeit<milliseconds>(&Lemon::kruskalsTrim, L, KRUSKAL);
}

void testHA30() {
	std::cout << "\nTesting HA30\n------------------------------------------------\n\n";
	
	DataFile HA30("HA30/dist.txt", 30);
	Data D2(HA30);
	
	D2.createGraph();
	D2.outputGraph("HA30/test_out.txt");
	D2.importGraph("HA30/test_out.txt");
	
	lemon::ListGraph LG4, LG5, LG6;
	Lemon L4(D2.getGraph(), &LG4);
	Lemon L5(D2.getGraph(), &LG5);
	Lemon L6(D2.getGraph(), &LG6);
	
	timeit<milliseconds>(&Lemon::initDistributionCenterSeq, L4, DIJKSTRASEQ);
	timeit<milliseconds>(&Lemon::initDistributionCenter, L4, DIJKSTRA);
	timeit2<milliseconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenterSeq, L5, KRUSKDIJKSEQ);
	timeit2<milliseconds>(&Lemon::kruskalsTrim, &Lemon::initDistributionCenter, L6, KRUSKDIJK);
	timeit<milliseconds>(&Lemon::kruskalsTrim, L4, KRUSKAL);
}

/*
 * #timeit(void(Lemon::*fptr)(void), Lemon&, int)
 *
 * @Parameters:		Pointer to a lemon function, the Lemon object, and an int
 * 			index into the funcName vector
 * @Fucntionality:	Times the functions passed to be used for a comparison of running time
 * @Return:		
 *
 */
template< typename T>
void timeit(void(Lemon::*fptr)(void), Lemon& L, int i) {
	//clock_t beg = clock();
        
        T beg = std::chrono::duration_cast<T>
                    (std::chrono::system_clock::now().time_since_epoch()); 

	(L.*fptr)();

	T end = std::chrono::duration_cast<T>
                    (std::chrono::system_clock::now().time_since_epoch());
        	
	std::cout << "Time it took " << L.funcName[i] << " to complete: " << end.count() - beg.count() << "\n\n";


	//int minute = result / 60;
	//float seconds = result - (minute * 60);

	//Can either pass the algName as a parameter or we can 
	//printResult(L.funcName[i], minute, seconds);
	
}

/*
 * #timeit2(void(Lemon::*fptr)(void), void(Lemon::*fptr)(void), Lemon&, int)
 *
 * @Parameters:		Pointer to a lemon function, Pointer to a lemon function, the Lemon object, and an int
 * 			index into the funcName vector
 * @Fucntionality:	Times the functions passed to be used for a comparison of running time
 * @Return:		
 *
 */
template <typename T>
void timeit2(void(Lemon::*spanTree)(void), void(Lemon::*shortPath)(void), Lemon& L, int i){
	//clock_t beg = clock();
        T beg = std::chrono::duration_cast<T>
                    (std::chrono::system_clock::now().time_since_epoch()); 

	(L.*spanTree)();
	(L.*shortPath)();

        T end = std::chrono::duration_cast<T>
                    (std::chrono::system_clock::now().time_since_epoch());
        	
	std::cout << "Time it took " << L.funcName[i] << " to complete: " << end.count() - beg.count() << "\n\n";

	//float result = ((float)(clock() - beg)) / CLOCKS_PER_SEC;

	//int minute = result / 60;
	//float seconds = result - (minute * 60);

	//Can either pass the algName as a parameter or we can 
	//printResult(L.funcName[i], minute, seconds);
}

/*
 * #timeit(void(Data::*fptr)(void), Data&)
 *
 * @Parameters:		Pointer to a Data function, the Data object
 * @Fucntionality:	Times the functions passed to be used for a comparison of running time
 * @Return:		
 *
 */
void timeit(void(Data::*fptr)(void), Data& D) {
	//clock_t beg = clock();
        seconds beg = std::chrono::duration_cast<seconds>
                    (std::chrono::system_clock::now().time_since_epoch()); 

        (D.*fptr)();

        seconds end = std::chrono::duration_cast<seconds>
                    (std::chrono::system_clock::now().time_since_epoch());
        	
	std::cout << "Time it took to complete: " << end.count() - beg.count() << "\n\n";


	//clock_t res = clock() - beg;
	//float result = ((float)res) / CLOCKS_PER_SEC;
	//std::cout << "\n" << result << "s\n";
}

/*This function allows us to print the times for each function on seperate threads
 * without the chance of them trying to print out at the same time*/


/*
 * #printResult(string, float)
 *
 * @Parameters:		the string is the name of an algorithm being run and the in is the
 * 			time it took to run the algorithm
 * @Fucntionality:	Prints the time it took to run an algorithm
 * @Return:		
 *
 * @Note: 		The mutex is not needed with the currently implementation since the algorithms
 * 			are run sequentially and not in parallel
 */
void printResult(std::string algName, int min, float sec){
	mu.lock();
	
	std::cout << "Time it took " << algName << " to complete: " << min << ":" << sec << "\n\n";
	//could also store results here if we wanted to use them for comparison later
	
	mu.unlock();
}

