#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include "print_var_name_str.h"
#include "CSVParser.hpp"
#include <filesystem>
#include <algorithm>
#include <iomanip>

int main()
{
	// Write a line to display the name of the current folder
	std::cout << "\n\n\nThe current working folder is: " << std::filesystem::current_path().string() << std::endl;

	// Time when the program starts executing
	auto t_start = std::chrono::steady_clock::now();

	// Settings of the input & output folder
	std::string input_folder = "Input\\Test";
	std::string output_folder = "Output\\Test";
	std::filesystem::path input_path{ input_folder };
	std::filesystem::path output_path{ output_folder };

	/*********************************************************************************
	*Test the function of reading csv file
	**********************************************************************************/
	int file_num = 0;
	std::cout << "Iterating the input folder: " << input_path.string() << std::endl;

	// Iteration over the input folder
	for (const auto& ith_file : std::filesystem::directory_iterator{ input_path })
	{
		// Display file info
		std::cout << "File " << std::setw(4) << std::setfill('0') << ++file_num << ": "
			<< ith_file.path().filename().string() << std::endl;

		// Read csv file
		CSVParser<double> file_in;
		auto T = file_in.readTable(ith_file.path().string());

		// Do something with the dataset, e.g.
		std::cout << "Dispaly an entry\n";
		std::cout << "T.v1(0) = " << T.at("v1").at(0) << std::endl;
	}


	/*********************************************************************************
	*Test the function of writing csv file
	**********************************************************************************/

	// Suppose these are the variables you want to save into csv files
	std::vector<int> v1{ 10,20,30 };
	std::vector<int> v2({ 100,200,300 });
	std::string out_full_path_name = output_folder + "\\Sample_output.csv";

	// Write the data above into a csv file
	CSVParser<int> file_out({ v1,v2 }, { "v1","v2" });
	file_out.writeTable(out_full_path_name);

	// Time when the program ends
	auto t_end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = t_end - t_start;
	std::cout << std::setprecision(3) << "\n\n\nelapsed time: " << elapsed_seconds.count() << "s\n";

	return 0;
}