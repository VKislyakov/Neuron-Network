#pragma once

#ifndef LAYER_H
#define LAYER_H

#include "Neuron.h"
#include <vector>
#include <cmath>

using namespace std;
class Layer {

public:
	Layer() = default;

	Layer(int early_kol_neu, int kol_neu);

 	explicit Layer(const vector<vector<double>>& W);

	virtual ~Layer() = default;

	vector<double> actF(const vector<double>& X);

	vector<double> actF() const;

	vector<double> derF() const;

	int correct(const vector<double>& deltw, const vector<double>& y, double alfa);

	vector<double> getVectorW(int i) const;

	vector<vector<double>> getMatrixW() const;

private:
	vector<Neuron> neurons;
	vector<double> activ_f;
	vector<double> sum_s;
};

#endif