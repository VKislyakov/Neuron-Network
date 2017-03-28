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

	Bloc(string path); /*	��������� Bloc �� ���� �� ������ �����, ���� �� ����������,
					  ����� �� ��-�� ������ �� ������� ���� �������� " 01"," 02" � �� �������������
						*/
	void save(string sPath,int numberElem); /* ��������� ���� � �������� �����, �������� 'diod', � ����� ������ ���������� ����
											�������� !!!  ���� ����� ������ ����������� ���� �� ����� �������� !!!
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
	vector<Bloc> blocSet; // ��������� ������
	map<string, vector<double>> answerForData; // ���������� ������ �� ������

};



#endif // DATASET_H