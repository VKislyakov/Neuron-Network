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

	Bloc(string path); /*	��������� Bloc �� ���� �� ������ �����, ���� �� ����������,
					  ����� �� ��-�� ������ �� ������� ���� �������� " 01"," 02" � �� �������������
						*/
	int save(string sPath); /* ��������� ���� � �������� �����, �������� 'diod', � ����� ������ ���������� ����
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
	DataSet(string dPath);
	int save(string savePath);
	virtual ~DataSet();
	//map<vector<double>, vector<Bloc>> get_mapData();

private:
	//vector<Bloc> blocSet; // ��������� ������
	map<vector<double>, vector<Bloc>> mapData;
	//vector<vector<double>> nAnswer; // ���������� ������ �� ������
	//vector<string> classesData; 
	map<string, vector<double>> mapAnswer;
	//vector<int> numberClassItems;
};



#endif // DATASET_H