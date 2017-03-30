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

	Bloc(string path); /*	формирует Bloc по пути из одного файла, если он существует,
					  иначе из мн-ва файлов по данному пути добавл€€ " 01"," 02" и тд соответсвенно
						*/
	int save(string sPath); /* сохран€ет блок в указаной папке, например 'diod', а папку класса определ€ет сама
								¬нимание !!!  если папка класса отсутствует файл не будет сохнанен !!!
											*/
private:
	

	vector<vector<string>> data;

	string answer;
};

//---------------------------------------------------------
class DataSet {
public:
	DataSet();
	DataSet(string dPath);
	int save(string savePath);
	virtual ~DataSet();
	//map<vector<double>, vector<Bloc>> get_mapData();

private:
	//vector<Bloc> blocSet; // обучающие данные
	map<vector<double>, vector<Bloc>> mapData;
	//vector<vector<double>> nAnswer; // правильные ответы на данные
	//vector<string> classesData; 
	map<string, vector<double>> mapAnswer;
	//vector<int> numberClassItems;
};



#endif // DATASET_H