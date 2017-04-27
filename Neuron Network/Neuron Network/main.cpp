#include "Net.h"
#include "DataSet.h"
#include "moduleSVD.h"
#include "SpectralClassifier.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <Eigen/SVD>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
using namespace std;
using namespace Eigen;
/*
void teachOneClassifier(Net &generalNet, moduleSVD &svd, vector<Data> &data, double e) {
	vector<vector<double>> x, testX, d, testD;

	x = svd.getTeachData(generalNet.getNumberInputData());
	d = data[0].answer;
	testX = svd.getNewhData(generalNet.getNumberInputData(), data[1].data);
	testD = data[1].answer;

	divisionComponents(testX, divider(x[0][0]));
	divisionComponents(x, divider(x[0][0]));

	generalNet.teaching(x, d, testX, testD, e);
}
*/
int main(int argc, char* argv[]) {
	
	string start;
	cout << "\n4)Create Cross\n5)Start without teach\n6)Start with teach";
	getline(cin, start);
	cout << endl;
	if ((start == "4")) {
		ParseData data("C:\\VKR\\3sources\\ir");
		for (int i = 1; i <= 7; i++) {
			vector<CrossValid> crossV = CrossValidation(data.getClassDistribution());
			saveCrossValid("C:\\VKR\\3sources\\cross1divAll\\"+to_string(i)+"\\crossValid.txt", crossV);
		}
	}
	else if ((start == "5")) {
		cout << "Number:\n";
		getline(cin, start);
		SpectralClassifier CR(false, start, "C:\\VKR\\3sources");
		CR.teachSR();
	}
	else if (start == "6") {
		cout << "Number:\n";
		getline(cin, start);
		SpectralClassifier CR(true, start , "C:\\VKR\\3sources");
		CR.teachSR();
	}
	
	return 0;
}
