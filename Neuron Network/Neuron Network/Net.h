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

	Net(vector<int> conf); // Generates net with random scales.

	Net(vector<vector<vector<double>>> wieght); // Generates net based on a three-dimensional matrix of weights.

//!!!! исправить на получение имени или пути к файлу
	Net(bool g); // Generates a network from a file.

	vector<double> startNet(vector<double> x); // Runs net for vector x.

	int teaching(vector<vector<double>> x, vector<vector<double>> d, double e = 0.01); // Trains net to a given accuracy, only by training set.

	void setLayer(vector<Layer> lay); // Specifies new weights for all layers.

	virtual ~Net();

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


