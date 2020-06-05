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

	explicit Neuron(int number_weight);

	explicit Neuron(const vector<double>& weights);

	virtual ~Neuron() = default;

	vector<double> getAllWeights() const;

	double getElemWeight(int number) const;

	double Sum(const vector<double>&  x);

	double Sum() const;

	double ActF(const vector<double>&  x);

	double ActF() const;

	double derF() const;

	void correctWeights(double deltaW, const vector<double>&  y, double alfa);

	void setNewVectorWeights(const vector<double>&  W);
private:
    vector<double> weights;
    double sum = 0;
    double F = 0;
};

#endif // !NEURON_H