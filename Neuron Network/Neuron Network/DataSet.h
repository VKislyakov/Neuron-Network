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

//---------------------------------------------------------

class Bloc {

public:
	friend DataSet;

	Bloc();

	virtual ~Bloc();

	Bloc(string path); /*	формирует Bloc по пути из одного файла, если он существует,
					  иначе из мн-ва файлов по данному пути добавл€€ " 01"," 02" и тд соответсвенно
						*/
	void save(string sPath,int numberElem); /* сохран€ет блок в указаной папке, например 'diod', а папку класса определ€ет сама
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

	virtual ~DataSet();

	void createDataFile();



private:
	vector<Bloc> blocSet; // обучающие данные
	map<string, vector<double>> answerForData; // правильные ответы на данные

};



#endif // DATASET_H