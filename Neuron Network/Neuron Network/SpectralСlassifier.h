#pragma once
#ifndef SPECTRAL_CLASSIFIER_H
#define SPECTRAL_CLASSIFIER_H

#include "Net.h"
#include "DataSet.h"
#include "moduleSVD.h"

class Spectral—lassifier
{
	ParseData spectralData;
	Net classifier;

public:
	

	Spectral—lassifier();
	virtual ~Spectral—lassifier();
};

#endif