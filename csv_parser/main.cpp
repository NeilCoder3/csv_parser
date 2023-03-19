#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include "print_var_name_str.h"
#include "CSVParser.hpp"

int main()
{
	auto t_start = std::chrono::steady_clock::now();

	std::string input_folder = "Input";
	std::string output_folder = "Output";
	std::vector<int> v1{ 10,20,30 };
	std::vector<int> v2({ 100,200,300 });
	std::vector<int> v3{ 1000,2000,3000 };
	CSVParser<int> cc1({ v1,v2 }, { "v1","v2" });
	CSVParser<int> cc2({ v1,v3 }, { "v1","v3" });
	cc1.writeTable();
	cc2.writeTable("myTest", false);
	auto data3 = cc2.readTable("myTest");
	std::cout << data3.at("v1").at(0) << std::endl;


	/*-----*/

	// TO DO...
	// Add date + time to file name
	// Add iteration over files in a folder

	auto t_end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = t_end - t_start;
	std::cout << std::setprecision(3) << "elapsed time: " << elapsed_seconds.count() << "s\n";
	return 0;
}