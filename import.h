#ifndef IMPORT_H
#define IMPORT_H

#include <iostream>
#include <fstream>

#define 



class Data {
	private:
		std::string file;
	public:
		Data();
		~Data();
		void getData(std::string file);
		void readData(std::string file);

}



#endif
