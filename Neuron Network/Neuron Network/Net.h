#pragma once
#ifndef NET_H
#define NET_H
#include "Layer.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

class Net
{
public:

	Net(vector<int> conf); // генерирует сеть с рандомными весами

	Net(vector<vector<vector<double>>> wieght); // генерирует сень на основе трехмерной матрицы весов

//!!!! исправить на получение имени или пути к файлу
	Net(bool g); // генерирует сеть из файла,

	vector<double> startNet(vector<double> x); // запускает сеть для вектора х

	int teaching(vector<vector<double>> x, vector<vector<double>> d, double e = 0.01); // обучает сеть до заданной точности, только по обучающему множеству

	void setLayer(vector<Layer> lay); // задает новые веса для всех слоев

private:
	int kol_sloev;
	vector<int> config;
	vector<Layer> layers;

	vector<vector<double>> deltaM(vector<double> d);

	void correct(vector<vector<double>> delta, double alfa, vector<double> x);

	double goldenSection(vector<vector<double>> delta, vector<double> x, vector<double> d); 

	double teach(vector<double> x, vector<double> d);

	vector<vector<vector<double>>> save();

	double functionError(vector<double> y, vector<double> d);

};

#endif // !NET_H


