#include "import.h"

int main() {

	DataFile KN57("KN57/dist.txt", 57);
	DataFile HA30("HA30/dist.txt", 57);

	Data D(KN57);
	D.readData();
	// D.getData();

	return 0;
}
