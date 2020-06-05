#include "Neuron.h"

Neuron::Neuron(int number_weight) {
	srand(time(0));
	for (int i = 0; i <= number_weight; i++) // 
		weights.push_back(0.0001 * (rand() % 6001 - 3000));  // 
	sum = 0;
	F = 0;
}
//---------------------------------------------------------
Neuron::Neuron(vector<double> verton_weight) {
	weights = verton_weight;
	sum = 0;
	F = 0;
}
//---------------------------------------------------------
Neuron::~Neuron() {
}

Neuron::Neuron() {
}
//---------------------------------------------------------
vector<double> Neuron::getAllWeights() { // 
	return weights;
}
//---------------------------------------------------------
double Neuron::getElemWeight(int number) {
	return(weights[number]);
}
//---------------------------------------------------------
double Neuron::Sum(vector<double> x) {
	sum = weights[weights.size() - 1];
	for (decltype(weights.size()) i = 0; i < weights.size() - 1; i++)
		sum = sum + weights[i] * x[i];
	return sum;
}
//---------------------------------------------------------
double Neuron::Sum() {
	return sum;
}
//---------------------------------------------------------

/*
double Neuron::ActF(vector<double> x) {
	Sum(x);
	F = (exp(sum) - exp(-sum)) / (exp(sum) + exp(-sum));
	return F;
}
*/
double Neuron::ActF(vector<double> x) {
	Sum(x);
	F = 1 / (1 + exp(-sum));
	return F;
}
//---------------------------------------------------------
double Neuron::ActF() {
	return F;
}
//---------------------------------------------------------
double Neuron::derF() {
	return (1 - F)*F;
	//return 1-F*F;
}
//---------------------------------------------------------
void Neuron::correctWeights(double deltaW, vector<double> y, double alfa) {
	for (decltype(weights.size()) i = 0; i < weights.size() - 1; i++)
		weights[i] = weights[i] - deltaW * y[i] * alfa;
	weights[weights.size() - 1] = weights[weights.size() - 1] - deltaW * alfa;
}
//---------------------------------------------------------
void Neuron::setNewVectorWeights(vector<double> W) {
	weights = W;
}
//---------------------------------------------------------
