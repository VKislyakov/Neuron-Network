#pragma once
#ifndef SPECTRAL_CLASSIFIER_H
#define SPECTRAL_CLASSIFIER_H

#include "Net.h"
#include "DataSet.h"
#include "moduleSVD.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;
class SpectralClassifier
{
public:
	Net classifier;
	Net irNet;
	Net diodNet;
	Net visibleNet;
	vector<Data> dataCollegium;
	string PathS;
	

	SpectralClassifier();

	virtual ~SpectralClassifier();

	SpectralClassifier(bool teachOrRead,string Path, string dataPath);
	void teachSR(double e = 0.01);
	void SpectralClassifier::teachOneClassifier(Net &generalNet, moduleSVD &svd, vector<Data> &data, double e);
	void teachSR_multi(int neuron, double e = 0.01);

	
};

#endif // !SPECTRAL_CLASSIFIER_H