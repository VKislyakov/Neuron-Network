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
	Net(vector<int> conf);

	Net(vector<vector<vector<double>>> wieght);

	Net(bool g); // обеспецивает задание архитектуры и весов из файла,

	vector<double> startNet(vector<double> x);

	int teaching(vector<vector<double>> x, vector<vector<double>> d, double e = 0.01);

	void setLayer(vector<Layer> lay);

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


