#include "import.h"

int main() {

	DataFile KN57("KN57/dist.txt", 57);
	DataFile HA30("HA30/dist.txt", 57);

	std::cout << "Dataset 1 : KN57\n";
	Data D1(KN57);
	D1.readData();
	// D1.getData();
	
	std::cout << "\n\nDataset 2 : HA30\n";
	Data D2(KN57);
	D2.readData();
	for (int i = 0; i < D2.getDim(); ++i)
		D2.getData(i);

	
	return 0;
}
