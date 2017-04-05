#pragma once

#ifndef DATASET_H
#define DATASET_H
#include <map>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace std;


vector<string> getDirectoryAttachments(string dPath);
//---------------------------------------------------------
struct Data
{
	vector<vector<double>> data;
	vector<vector<double>> answer;
};
//---------------------------------------------------------

//---------------------------------------------------------
class Bloc {

public:
	friend class ParseData;
	Bloc();

	virtual ~Bloc();

	Bloc(string path); /*	Forms Bloc on the path from one file, if it exists.
							Otherwise from many files along this path, adding "01", "02" and so on respectively.
						*/
	vector<vector<double>> data;
private:
	
	
};

//---------------------------------------------------------
class ParseData {
public:
	ParseData();
	virtual ~ParseData();
	ParseData(string dPath);
	vector<Data> getDataTEST();
	
private:
	vector<vector<double>> dataAnswer;
	vector<Bloc> blocSet;
	vector<int> numberDataItems;
	map<vector<double>, string> mapAnswer;
};

//---------------------------------------------------------

#endif // DATASET_H