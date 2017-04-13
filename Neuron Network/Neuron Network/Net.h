#pragma once
#ifndef NET_H
#define NET_H
#include "Layer.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

class Net
{
public:
	// Path to save weights.
	string savePath;

	// Generates net with random scales.
	Net(vector<int> conf); 

	// Generates net based on a three-dimensional matrix of weights.
	Net(vector<vector<vector<double>>> wieght); 
	
	// Generates a network from a file.
	Net(string Path); 
	
	// Runs net for vector x.
	vector<double> startNet(vector<double> x); 

	// Trains net to a given accuracy, only by training set.
	int teaching(vector<vector<double>> x, vector<vector<double>> d, vector<vector<double>> testX, vector<vector<double>> testD, double e = 0.01);
	
	// Specifies new weights for all layers.
	void setLayer(vector<Layer> lay); 

	// Counts the error function.
	double functionError(vector<double> y, vector<double> d);

	virtual ~Net();

	Net();

private:
	int kol_sloev;
	vector<int> config;
	vector<Layer> layers;
	

	vector<vector<double>> deltaM(vector<double> d);

	void correct(vector<vector<double>> delta, double alfa, vector<double> x);

	double goldenSection(vector<vector<double>> delta, vector<double> x, vector<double> d); 

	double teach(vector<double> x, vector<double> d);

	vector<vector<vector<double>>> save(int i);


};

#endif // !NET_H


