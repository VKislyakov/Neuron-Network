#include "Neuron.h"

Neuron::Neuron(int number_weight) {
	srand(time(0));
	for (int i = 0; i <= number_weight; i++)
		weights.push_back(0.0001 * (rand() % 6001 - 3000));  //magic number??
}

Neuron::Neuron(const vector<double>& weights)
: weights(weights)
  {}

vector<double> Neuron::getAllWeights() const{
	return weights;
}

double Neuron::getElemWeight(int number) const {
	return weights[number];
}

double Neuron::Sum(const vector<double>& x) {
	sum = weights.back();
	for (auto i = 0; i < weights.size() - 1; i++)
		sum = sum + weights[i] * x[i];
	return sum;
}

double Neuron::Sum() const {
	return sum;
}

/*
double Neuron::ActF(vector<double> x) {
	Sum(x);
	F = (exp(sum) - exp(-sum)) / (exp(sum) + exp(-sum));
	return F;
}
*/
double Neuron::ActF(const vector<double>& x) {
	Sum(x);
	F = 1 / (1 + exp(-sum));
	return F;
}

double Neuron::ActF() const {
	return F;
}

double Neuron::derF() const {
	return (1 - F)*F;
	//return 1-F*F;
}

void Neuron::correctWeights(double deltaW, const vector<double>& y, double alfa) {
	for (auto i = 0; i < weights.size() - 1; i++)
		weights[i] = weights[i] - deltaW * y[i] * alfa;
	weights[weights.size() - 1] = weights[weights.size() - 1] - deltaW * alfa;
}

void Neuron::setNewVectorWeights(const vector<double>& W) {
	weights = W;
}
