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

class DataSet;
vector<string> getDirectoryAttachments(string dPath);
//---------------------------------------------------------

class Bloc {

public:
	friend DataSet;

	Bloc();

	virtual ~Bloc();

	Bloc(string path); /*	Forms Bloc on the path from one file, if it exists.
							Otherwise from many files along this path, adding "01", "02" and so on respectively.
						*/
	int save(string sPath); /* Saves the block in the specified folder, for example 'diod', and the class folder defines itself.
								Attention !!! If the class folder is missing, the file will not be saved !!!
							*/
private:
	

	vector<vector<double>> data;

	string answer;
};

//---------------------------------------------------------
class DataSet {
public:
	DataSet();
	virtual ~DataSet();
	DataSet(string dPath);
	int save(string savePath);	
	map<vector<double>, vector<Bloc>> getData();
	//map<vector<double>, vector<Bloc>> get_mapData();

private:

	map<vector<double>, vector<Bloc>> mapData;
	map<string, vector<double>> mapAnswer;
};



#endif // DATASET_H