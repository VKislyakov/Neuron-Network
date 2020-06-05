#include "moduleSVD.h"



moduleSVD::moduleSVD()
{
}


moduleSVD::~moduleSVD()
{
}

//---------------------------------------------------------
// Public methods.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------------------------------------

//---------------------------------------------------------
moduleSVD::moduleSVD(vector<vector<double>> teachData) {
	teachMatrix = vectVectToMatrix(teachData);
	JacobiSVD<MatrixXd> svd(teachMatrix, ComputeThinU | ComputeThinV);
	MatrixXd singular(svd.matrixU().cols(), svd.matrixU().cols());
	for (int i = 0; i < svd.matrixU().cols(); i++)
		for (int j = 0; j < svd.matrixU().cols(); j++)
			if (i == j)
				singular(i, j) = svd.singularValues()(i);
			else
				singular(i, j) = 0;
	T = svd.matrixU() * singular;
	P = svd.matrixV();
}

//---------------------------------------------------------
vector<vector<double>> moduleSVD::getTeachData(int components) {
	return matxrixToVectVect(T.block(0,0,T.rows(),components));
}
//---------------------------------------------------------

vector<vector<double>> moduleSVD::getNewhData(int components, vector<vector<double>> data) {
	
	return matxrixToVectVect((vectVectToMatrix(data)*P).block(0,0,data.size(),components));
}

//---------------------------------------------------------
// Private methods.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------------------------------------

//---------------------------------------------------------
vector<vector<double>> moduleSVD::matxrixToVectVect(MatrixXd m) {
	vector<vector<double>> getBack;
	vector<double> buf;
	for (size_t i = 0; i < m.rows(); i++) {
		for (size_t j = 0; j < m.cols(); j++) {
			buf.push_back(m(i, j));
		}
		getBack.push_back(buf);
		buf.clear();
	}
	return (getBack);
}

//---------------------------------------------------------
MatrixXd moduleSVD::vectVectToMatrix(vector<vector<double>> vV) {
	MatrixXd m(vV.size(), vV[0].size());
	for (size_t j = 0; j < vV[0].size(); j++)
		for (size_t i = 0; i < vV.size(); i++)
			m(i, j) = vV[i][j];
	return m;
}

//---------------------------------------------------------