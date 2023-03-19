/**
 * @file CSVParser.hpp
 * @author neilblue2011@hotmail.com
 * @brief This is a class designed to read csv files and to write the dataset into csv files
 * @version 0.1
 * @date 2023-03-19 (YYYY-MM-DD)
 *
 * @copyright Copyright (c) 2023
 *
 */


#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <utility> // std::pair
#include <map> // std::map
#include <string>
#include <initializer_list>
#include "print_var_name_str.h" // it's not used at the moment
#include <cassert>
#include <chrono>
#include <ctime>
#include <sstream>
#include <filesystem>

template<typename T>
class CSVParser
{
private:
	std::string _full_file_name;
	std::vector<std::string> _col_names;
	std::vector<std::vector<T>> _data_vecs;
	std::map < std::string, std::vector<T>> _table_dataset;
	size_t _m_rows{ 0 };
	size_t _n_cols{ 0 };

	/*******************************************************************
	* Private functions
	********************************************************************/

	// set _table_dataset using the other private variables
	void map_colname_with_data()
	{
		size_t N = _col_names.size();
		for (size_t i = 0; i < N; i++)
		{
			_table_dataset[_col_names.at(i)] = _data_vecs.at(i);
		}
	}

	// obtain the current time (Year-Month-Date-Hour-Minute-Second) and convert it to strings
	std::string generate_datetime_string()
	{
		time_t now = time(0);
		tm datetime;
		localtime_s(&datetime, &now);

		int year = 1900 + datetime.tm_year;
		int month = 1 + datetime.tm_mon;
		int day = datetime.tm_mday;
		int hour = datetime.tm_hour;
		int minute = datetime.tm_min;
		int second = datetime.tm_sec;

		char year_s[5], month_s[3], day_s[3], hour_s[3], minute_s[3], second_s[3];
		sprintf_s(year_s, 5, "%04d", year);
		sprintf_s(month_s, 3, "%02d", month);
		sprintf_s(day_s, 3, "%02d", day);
		sprintf_s(hour_s, 3, "%02d", hour);
		sprintf_s(minute_s, 3, "%02d", minute);
		sprintf_s(second_s, 3, "%02d", second);

		std::string data_time = "_" + std::string(year_s) + "_" + std::string(month_s) + "_" + std::string(day_s) + "_" + std::string(hour_s) + "_" + std::string(minute_s) + "_" + std::string(second_s);

		return data_time;
	}

	// clear the private variables and set to default
	void clear()
	{
		_full_file_name = "";
		_col_names.clear();
		_data_vecs.clear();
		_table_dataset.clear();
		_m_rows = 0;
		_n_cols = 0;
	}

	// create the output folder it doesn't exist
	void initOutFolder()
	{
		std::filesystem::path p{ _full_file_name };
		if (!std::filesystem::exists(p.parent_path()))
		{
			std::filesystem::create_directories(p.parent_path());
		}
	}
public:
	/*******************************************************************
	* Constructors & Destructors
	********************************************************************/
	CSVParser() = default;
	virtual ~CSVParser() = default;

	// a constructor used before calling writeTable()
	CSVParser(std::initializer_list<std::vector<T>> data_value, std::initializer_list<std::string> data_name)
	{
		assert(data_value.size() == data_name.size());

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

	/*********************************************************************
	* Major functions
	**********************************************************************/

	// write data into a csv file where the first row stores the column names
	void writeTable(std::string f = "test_write.csv", bool auto_add_datetime_flag = true)
	{
		// file name + date + time
		std::string ymdhms{};
		if (auto_add_datetime_flag)
		{
			ymdhms = generate_datetime_string();
		}
		f.erase(f.end() - 4, f.end()); // remove the extension ".csv"
		_full_file_name = f + ymdhms + ".csv";

		// initialize the output folder
		initOutFolder();

		// create an output filestream object
		std::ofstream fout(_full_file_name);
		if (!fout.is_open())
		{
			fout.clear();
			std::cerr << "Could not open " << _full_file_name << std::endl;
			std::exit(EXIT_FAILURE);
		}

		// send column names to the stream
		for (auto ith_col = _col_names.begin(); ith_col != _col_names.end(); ith_col++)
		{
			fout << *ith_col;
			fout << ",";
		}
		// no comma at the end of the line
		fout.seekp(-1, std::ios::cur);
		fout << "\n";

		// send data to the stream
		for (size_t r = 0; r < _m_rows; r++)
		{
			for (size_t c = 0; c < _n_cols; c++)
			{
				fout << _data_vecs[c][r];
				if (c != _n_cols - 1)
				{
					fout << ",";
				}
			}
			fout << "\n";
		}

		// needed for some implementations
		fout.clear();

		// disconnect file
		fout.close();
	}

	// read data from a csv file where the first row stores the column names; store the result into _table_dataset
	std::map < std::string, std::vector<T>> readTable(const std::string& f)
	{
		// set private variables to their default values
		clear();

		// set file name to the private variable
		_full_file_name = f;

		// create an input filestream object
		std::ifstream fin(_full_file_name);
		if (!fin.is_open())
		{
			fin.clear();
			std::cerr << "Could not open " << _full_file_name << std::endl;
			std::exit(EXIT_FAILURE);
		}

		// helper variables
		std::string ith_line, col_name;
		T val{};

		/* Read the column names ---------------------------------------*/
		// read the first line and save it into column names
		std::getline(fin, ith_line);

		// create a string stream to further parse the 1st line
		std::stringstream ss(ith_line);

		// extract each column name
		while (std::getline(ss, col_name, ','))
		{
			_col_names.push_back(col_name);
		}

		// get the number of columns
		_n_cols = _col_names.size();

		// allocate space for the data
		_data_vecs.resize(_n_cols);

		/* Read data-----------------------------------------------------*/
		// iterate over each line
		while (std::getline(fin, ith_line))
		{
			// associate a stingstream to the strings of the current line
			ss = std::stringstream(ith_line);

			// keep track of the current column index
			int ith_col = 0;

			// extract each data
			while (ss >> val)
			{
				// add the current number to the end of the "ith_col" column
				_data_vecs.at(ith_col).push_back(val);

				// check the next token
				if (ss.peek() == ',')
				{
					// if the token is comma, discard it from the stream
					ss.ignore();
				}

				// increament the column index
				ith_col++;
			}
		}

		// get the number of rows
		_m_rows = _data_vecs.at(0).size();

		// equal number of names and data
		assert(_data_vecs.size() == _col_names.size());

		// needed for some implementations
		fin.clear();

		// disconnect file
		fin.close();

		// construct the output
		map_colname_with_data();

		return _table_dataset;
	}

	/*********************************************************************
	* Setters & Getters
	**********************************************************************/
	std::map < std::string, std::vector<T>> getTable()
	{
		return _table_dataset;
	}

	size_t getNumRows() { return _m_rows; }

	size_t getNumCols() { return _n_cols; }
};
