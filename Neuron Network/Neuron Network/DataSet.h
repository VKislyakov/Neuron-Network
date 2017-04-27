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


double divider(double a);
void divisionComponents(vector<vector<double>> &x, double buffDiv1);
//---------------------------------------------------------
struct Data
{
	vector<vector<double>> data;
	vector<vector<double>> answer;
	 void inline clear() { // проверить правильность
		this->data.clear();
		this->answer.clear();
	}
};
//---------------------------------------------------------
	struct CrossValid{
		vector<int> teach;
		vector<int> test;
		vector<int> control;
	};
	vector<CrossValid> CrossValidation(vector<int> classDistribution);
	// Ex: C\\VKR\\"fileName".txt
	vector<CrossValid> readCrossValid(string Path);
	// Ex: C\\VKR\\"fileName".txt
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