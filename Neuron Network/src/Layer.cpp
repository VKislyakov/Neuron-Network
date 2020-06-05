#include "Layer.h"


Layer::Layer(int early_kol_neu, int kol_neu) {
	for (int i = 0; i < kol_neu; i++) {
		Neuron a(early_kol_neu);
		neurons.push_back(a);
		activ_f.push_back(0);
		sum_s.push_back(0);
	}
}

Layer::Layer(const vector<vector<double>>& W) {
	for (size_t i = 0; i < W.size(); i++) {
		Neuron a(W[i]);
		neurons.push_back(a);
		activ_f.push_back(0);
		sum_s.push_back(0);
	}
}

vector<double> Layer::actF(const vector<double>& X) {
	for (decltype(neurons.size()) i = 0; i < neurons.size(); i++) {
		activ_f[i] = neurons[i].ActF(X);
		sum_s[i] = neurons[i].Sum();
	}
	return(activ_f);
}

vector<double> Layer::actF() const{
	return(activ_f);
}

vector<double> Layer::derF() const {
	vector<double> getBack;
	for (const auto& neuron : neurons)
		getBack.push_back(neuron.derF());
	return(getBack);
}


int Layer::correct(const vector<double>& deltw, const vector<double>& y, double alfa) {
	for (decltype(neurons.size()) i = 0, sz = neurons.size(); i < sz; i++) {
		neurons[i].correctWeights(deltw[i], y, alfa);
	}
	return 0;
}

vector<double> Layer::getVectorW(int i) const { //
	return neurons[i].getAllWeights();
}

vector<vector<double>> Layer::getMatrixW() const { //
	vector<vector<double>> matrix;
	for (decltype(neurons.size()) i = 0; i<neurons.size(); i++) {
		matrix.push_back(getVectorW(i));
	}
	return matrix;
}
