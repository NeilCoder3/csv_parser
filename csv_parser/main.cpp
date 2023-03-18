#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include "print_var_name_str.h"
#include "CSVParser.hpp"

int main()
{
	char aa[] = "fs";
	std::string s_aa(aa);
	auto t_start = std::chrono::steady_clock::now();
	/*TEST*/
	std::string f_name = "Test0";
	std::vector<int> v1{ 10,20,30 };
	std::vector<int> v2({ 100,200,300 });
	CSVParser<int> cc({ v1,v2 }, { "v1","v2" }, f_name);
	cc.writeTable();


	/*-----*/

	// TO DO...
	// Add date + time to file name
	// Add iteration over files in a folder

	auto t_end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = t_end - t_start;
	std::cout << std::setprecision(3) << "elapsed time: " << elapsed_seconds.count() << "s\n";
	return 0;
}