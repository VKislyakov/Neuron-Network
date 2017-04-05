#pragma once
#ifndef SVD_H
#define SVD_H

#include <vector>
#include <Eigen/SVD>
using namespace Eigen;
using namespace std;
class moduleSVD
{
public:
	moduleSVD();
	virtual ~moduleSVD();
	moduleSVD(vector<vector<double>> teachData);
	vector<vector<double>> getTeachData(int components);

	/*  
		Go to the space of the main components for the new data.
		For matrix X_test:
		T_test = X_test * P
	*/
	vector<vector<double>> getNewhData(int components, vector<vector<double>> data);

private:
	vector<vector<double>> matxrixToVectVect(MatrixXd m);
	MatrixXd vectVectToMatrix(vector<vector<double>> vV);

	MatrixXd teachMatrix; // X = U*S*V' = T*P' .
	MatrixXd T; // T = U*S .
	MatrixXd P; // P = V .
};

#endif