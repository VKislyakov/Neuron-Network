#pragma once

#ifndef DATASET_H
#define DATASET_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class DataSet
{
public:
	DataSet();

	virtual ~DataSet();

	

	/*
	������� ��� �����: � ��������� ��-�� � �������� �� ����
	����� ������ �������: 1) ����-�� ��������� �������
						  2) ���� � ������ ����� � ������� ����� ������� ������
						  3) ���� � ����� � ������� � �����, ���� ������� ����� �� ������ "END"

	*/
	void createDataFile();



private:


	vector<vector<double>> trainingData; // ��������� ������

	vector<vector<double>> answerForData; // ���������� ������ �� ������

};

#endif // DATASET_H