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
#include <chrono>
#include <ctime>

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

public:
	/*******************************************************************
	* Constructors & Destructors
	********************************************************************/
	CSVParser() = delete;

	// the constructor being used
	CSVParser(std::initializer_list<std::vector<T>> data_value, std::initializer_list<std::string> data_name, std::string f = "test", bool auto_add_datetime_flag = true)
	{
		assert(data_value.size() == data_name.size());

		// file name + date + time
		std::string ymdhms("");
		if (auto_add_datetime_flag)
		{
			ymdhms = generate_datetime_string();
		}
		_filename = f + ymdhms + ".csv";

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