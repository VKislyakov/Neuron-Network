#pragma once
#ifndef NEURON_H
#define NEURON_H
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

class Neuron
{
private:
	vector<double> weights;
	double sum;
	double F;
public:
	Neuron();

	Neuron(int number_weight);

	Neuron(vector<double> verton_weight);

	virtual ~Neuron();

	vector<double> getAllWeights();

	double getElemWeight(int number);

	double Sum(vector<double> x);

	double Sum();

	double ActF(vector<double> x);

	double ActF();

	double derF();

	void correctWeights(double deltaW, vector<double> y, double alfa);

	void setNewVectorWeights(vector<double> W);


};

#endif // !NEURON_H