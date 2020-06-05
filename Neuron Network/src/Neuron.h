#pragma once
#ifndef NEURON_H
#define NEURON_H
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

class Neuron
{
public:
	Neuron() = default;

	Neuron(int number_weight);

	Neuron(vector<double> verton_weight);

	virtual ~Neuron() = default;

	vector<double> getAllWeights();

	double getElemWeight(int number);

	double Sum(vector<double> x);

	double Sum();

	double ActF(vector<double> x);

	double ActF();

	double derF();

	void correctWeights(double deltaW, vector<double> y, double alfa);

	void setNewVectorWeights(vector<double> W);
private:
    vector<double> weights;
    double sum = 0;
    double F = 0;
};

#endif // !NEURON_H