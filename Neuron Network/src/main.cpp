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

void teachOneNet(Net &generalNet, moduleSVD &svd, vector<Data> &data, double e , vector<vector<double>> &xControl) {
	vector<vector<double>> x, testX, d, testD;

	x = svd.getTeachData(generalNet.getNumberInputData());
	d = data[0].answer;
	testX = svd.getNewhData(generalNet.getNumberInputData(), data[1].data);
	testD = data[1].answer;

	divisionComponents(testX, divider(x[0][0]));
	divisionComponents(xControl, divider(x[0][0]));
	divisionComponents(x, divider(x[0][0]));

	generalNet.teaching(x, d, testX, testD, e);
}

int main(int argc, char* argv[]) {
	
	string start;
	cout << "\n1)Create Cross\n2)Start without teach\n3)Start with teach";
	getline(cin, start);
	cout << endl;
	if ((start == "1")) {
		ParseData data("C:\\VKR\\3sources\\ir");
		for (int i = 1; i <= 10; i++) {
			vector<CrossValid> crossV = CrossValidation(data.getClassDistribution());
			saveCrossValid("C:\\VKR\\3sources\\cross2\\"+to_string(i)+"\\crossValid.txt", crossV);
		}
	}
	else if ((start == "2")) {
		cout << "Number:\n";
		getline(cin, start);
		SpectralClassifier CR(false, start, "C:\\VKR\\3sources");
		CR.teachSR();
	}
	else if (start == "3") {
		cout << "path cross:\n";
		getline(cin, start);
		SpectralClassifier CR(true, start , "C:\\VKR\\3sources");
		CR.teachSR();
		for (int i = 5;i<35;i++)
			CR.teachSR_multi(i);
	}
	else if (start == "4") {
		string pathData, pathCross, pathSave;
		cout << "Path data:\n";
		getline(cin, pathData);
		cout << "Path save:\n";
		getline(cin, pathSave);
		cout << "Path cross:\n";
		getline(cin, pathCross);
		cout << "eps:\n";
		double eps;
		cin >> eps;
		vector<CrossValid> generalCross = readCrossValid(pathCross+ "\\crossValid.txt");
		ParseData dataPars(pathData);
		vector<Data> data = dataPars.getDataCrossValid(generalCross);
		moduleSVD svd(data[0].data);
		std::ofstream out(pathSave + "\\cross8_80.txt");

		for (int i = 8; i < 80; i+=2) {
			vector<int> settings_net = { 2,i,int(sqrt(i*14)),14 };
			Net newNet(settings_net);
			newNet.savePath = pathSave;
			vector<vector<double>> xControl = svd.getNewhData(newNet.getNumberInputData(), data[2].data);
			teachOneNet(newNet, svd, data, eps, xControl);
			out << i << " " << int(sqrt(i * 14))
				<<":  " << newNet.percentTrueAnswer(xControl, data[2].answer) << endl;

		}
	}
	else if (start == "5") {
		//-----------------------------------
		string pathData, pathCross, pathSave;
		cout << "Path data:\n";
		getline(cin, pathData);
		cout << "Path save:\n";
		getline(cin, pathSave);
		cout << "Path cross:\n";
		getline(cin, pathCross);
		cout << "eps:\n";
		double eps;
		cin >> eps;
		int beginN, endN, step;
		cout << "Begin and end neuron number, and step:\n";
		cin >> beginN >> endN >> step;
		int compon;
		cout << "Number component:\n";
		cin >> compon;
		//--------------------------------------
		vector<CrossValid> generalCross = readCrossValid(pathCross + "\\crossValid.txt");
		ParseData dataPars(pathData);
		vector<Data> data = dataPars.getDataCrossValid(generalCross);
		moduleSVD svd(data[0].data);
		std::ofstream out(pathSave + "\\crossNeuron" +to_string(beginN) + "_" + to_string(endN) +".txt");
		//--------------------------------------
		for (int i = beginN; i < endN; i += step) {
			vector<int> settings_net = { 2, compon, i, 14 };
			Net newNet(settings_net);
			newNet.savePath = pathSave;
			vector<vector<double>> xControl = svd.getNewhData(newNet.getNumberInputData(), data[2].data);
			teachOneNet(newNet, svd, data, eps, xControl);
			out << i << ":  " << newNet.percentTrueAnswer(xControl, data[2].answer) << endl;
		}
		//--------------------------------------
	}
	return 0;
}
