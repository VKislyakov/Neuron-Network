#pragma once

#ifndef LAYER_H
#define LAYER_H
#include "Neuron.h"
#include <vector>
#include <cmath>

using namespace std;
class Layer {

public:
	Layer(int early_kol_neu, int kol_neu);

	Layer(vector<vector<double>> W);

	vector<double> actF(vector<double> X);

	vector<double> actF();

	int correct(vector<double> deltw, vector<double> y, double alfa);

	vector<double> getVectorW(int i);

	vector<vector<double>> getMatrixW();
private:
	vector<Neuron> neurons;
	vector<double> activ_f;
	vector<double> sum_s;
};

#endif