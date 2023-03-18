#include <iostream>
#include <string>
#include "print_var_name_str.h"
#include <map>
#include "CSVParser.hpp"

int main()
{
	/*TEST*/
	std::string f_name = "test3.csv";
	std::vector<int> v1{ 10,20,30 };
	std::vector<int> v2({ 100,200,300 });
	CSVParser<int> cc({ v1,v2 }, { "v1","v2" }, f_name);
	cc.writeTable();


	/*-----*/

	// TO DO...
	// Add timing
	// Add iteration over files in a folder

	return 0;
}