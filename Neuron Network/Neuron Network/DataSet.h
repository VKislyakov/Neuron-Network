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
	Создате два файла: с обучающим мн-ом и ответами на него
	После вызова ожидает: 1) Колл-во вариантов ответов
						  2) Путь с именем файла в который будет вестись запись
						  3) Путь к файлу с данными и ответ, пока вметсто файла не введут "END"

	*/
	void createDataFile();



private:


	vector<vector<double>> trainingData; // обучающие данные

	vector<vector<double>> answerForData; // правильные ответы на данные

};

#endif // DATASET_H