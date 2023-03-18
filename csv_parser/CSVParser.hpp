#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <utility> // std::pair
#include <map> // std::map
#include <string>
#include <initializer_list>
#include "print_var_name_str.h"
#include <cassert>

template<typename T>
class CSVParser
{
private:
	std::string _filename;
	std::vector<std::string> _col_names;
	std::vector<std::vector<T>> _data_vecs;
	std::map < std::string, std::vector<T>> _dataset;
	size_t _m_rows;
	size_t _n_cols;

	/*******************************************************************
	* Private functions
	********************************************************************/
	void map_colname_with_data()
	{
		size_t N = _col_names.size();
		for (size_t i = 0; i < N; i++)
		{
			_dataset[_col_names.at(i)] = _data_vecs.at(i);
		}
	}


public:
	/*******************************************************************
	* Constructors & Destructors
	********************************************************************/
	CSVParser() = delete;

	// the constructor being used
	CSVParser(std::initializer_list<std::vector<T>> data_value, std::initializer_list<std::string> data_name, std::string f = "test.csv")
	{
		assert(data_value.size() == data_name.size());

		// file name
		_filename = f;

		// column names
		for (const auto& i : data_name)
		{
			_col_names.push_back(i);
		}

		// dataset
		for (const auto& i : data_value)
		{
			_data_vecs.push_back(i);
		}

		// associate the column names with the dataset
		map_colname_with_data();

		// size of the table stored in the csv file
		_m_rows = _data_vecs.at(0).size();
		_n_cols = _data_vecs.size();
	}

	~CSVParser() {};

	/*********************************************************************
	* Setters & Getters
	**********************************************************************/


	/*********************************************************************
	* Major functions
	**********************************************************************/

	// write data into csv file where the first row stores the column names
	void writeTable() const
	{
		// Create an output filestream object
		std::ofstream myFile(_filename);
		if (!myFile.is_open())
		{
			myFile.clear();
			std::cerr << "Could not open " << _filename << std::endl;
		}

		// Send column names to the stream
		for (auto ith_col = _col_names.begin(); ith_col != _col_names.end(); ith_col++)
		{
			myFile << *ith_col;
			if (ith_col != _col_names.end())
			{
				myFile << ","; // no commat at the end of the line
			}
		}
		myFile << "\n";

		//Send data to the stream
		for (size_t r = 0; r < _m_rows; r++)
		{
			for (size_t c = 0; c < _n_cols; c++)
			{
				myFile << _data_vecs[c][r];
				if (c != _n_cols - 1)
				{
					myFile << ",";
				}
			}
			myFile << "\n";
		}

		// needed for some implementations
		myFile.clear();

		// disconnect file
		myFile.close();
	}


};

