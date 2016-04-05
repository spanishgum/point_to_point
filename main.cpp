#include "import.h"
#include "lemon/list_graph.h"

int main() {

	std::cout << "\nTesting KN57\n-------------------\n\n";
	DataFile KN57("KN57/dist.txt", 57);
	Data D1(KN57);
	// for (int i = 0; i < D1.getDim(); ++i)
		// D1.getData(i);
	// D1.createGraph();
	// D1.outputGraph();
	
	std::cout << "\nTesting HA30\n-------------------\n\n";
	DataFile HA30("HA30/dist.txt", 30);
	Data D2(HA30);
	// for (int i = 0; i < D2.getDim(); ++i)
		// D2.getData(i);
	D2.createGraph();
	// D2.outputGraph();
	
	lemon::ListDigraph LDG();
	
	return 0;
}


