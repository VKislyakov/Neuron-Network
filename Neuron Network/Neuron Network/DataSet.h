#pragma once

#ifndef DATASET_H
#define DATASET_H
#include <map>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include <ctime>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace std;



//---------------------------------------------------------
struct Data
{
	vector<vector<double>> data;
	vector<vector<double>> answer;
};
//---------------------------------------------------------
	struct CrossValid{
		vector<int> teach;
		vector<int> test;
		vector<int> control;
	};
	vector<CrossValid> CrossValidation(vector<int> classDistribution);
	vector<CrossValid> readCrossValid(string Path);
	void saveCrossValid(string Path, vector<CrossValid> v);
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
	vector<int> getClassDistribution();
	vector<Data> ParseData::getDataCrossValid(vector<CrossValid> crossV);
private:
	static vector<string> getDirectoryAttachments(string dPath);
	vector<vector<double>> dataAnswer;
	vector<Bloc> blocSet;
	vector<int> numberDataItems;
	map<vector<double>, string> mapAnswer;
};

//---------------------------------------------------------

#endif // DATASET_H