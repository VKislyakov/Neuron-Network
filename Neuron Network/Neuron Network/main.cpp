#include "Net.h"
#include "DataSet.h"
#include "moduleSVD.h"
#include <vector>
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
//---------------------------------------------------------
double divider(double a) {
	if (abs(a) > 10)
		return(100);
	if (abs(a) > 0.2)
		return(10);
	return(1);
}

int divisionComponents(vector<vector<double>> &x, double buffDiv1, double buffDiv2) {
	if (buffDiv1 == 1)
		return(1);
	for (size_t i = 0; i < x.size(); i++)
		x[i][0] = x[i][0] / buffDiv1;
	if (buffDiv2 > 1)
		for (size_t i = 0; i < x.size(); i++)
			x[i][1] = x[i][1] / buffDiv2;
	return (0);
}

void teachNet(vector<Data> b1) {
	cout << "Teach Net" << endl;
	string Path1, Path2;

	cout << "Path to save: " << endl;
	getline(cin, Path2);

	int kolNeu, kolComp;
	cout << "Number of neurons = ";
	cin >> kolNeu;

	cout << "Number of components = ";
	cin >> kolComp;
	cout << endl << "	start SVD" << endl;
	moduleSVD c1(b1[0].data);
	//-----------------------------------------------------
	vector<vector<double>> x, testX, d, testD;

	x = c1.getTeachData(kolComp);
	d = b1[0].answer;
	testX = c1.getNewhData(kolComp, b1[1].data);
	testD = b1[1].answer;

	divisionComponents(testX, divider(x[0][0]), divider(x[0][1]));
	divisionComponents(x, divider(x[0][0]), divider(x[0][1]));
	
	vector<int> settings_net = { 2,kolComp,kolNeu,14 }; // настройки сети
	Net generalNet(settings_net);
	generalNet.savePath = Path2;
	generalNet.teaching(x, d, testX, testD, 0.08);

}

void workNet(vector<Data> b1) {
	cout << "Work Net!" << endl;
	string Path1, Path2, Path3;
	cout << endl << "	start SVD" << endl;
	moduleSVD c1(b1[0].data);
	//-----------------------------
	cout << endl << "Path to save: " << endl;
	getline(cin, Path2);
	cout << "Path to weight: " << endl;
	getline(cin, Path3);
	int kolComp,kolSave;
	cout << "Number of components = ";
	cin >> kolComp;
	cout << "Number of components = ";
	cin >> kolSave;
	vector<int> answerNet;
	for(int numberSave = 1;numberSave<=kolSave;numberSave++){
			
		Net generalNet(Path3 + to_string(numberSave) + ".txt");

		vector<vector<double>> xControl, dControl, x;
		xControl = c1.getNewhData(kolComp, b1[2].data);
		dControl = b1[2].answer;
		x = c1.getTeachData(kolComp);

		divisionComponents(xControl, divider(x[0][0]), divider(x[0][1]));

		std::ofstream out(Path2 + to_string(numberSave) + ".txt");
		int trueAnswer = 0;

		for (size_t i = 0; i < xControl.size(); i++) {
			vector<double> y = generalNet.startNet(xControl[i]);
			int answerY = 0, answerD = 0;
			double maxY = 0, maxD = 0;
			for (decltype(y.size()) j = 0; j < y.size(); j++) {
				if (y[j] > maxY) {
					maxY = y[j];
					answerY = j;
				}
				if (dControl[i][j] > maxD) {
					maxD = dControl[i][j];
					answerD = j;
				}
			}
			out << answerD << " == " << answerY;
			if (answerD == answerY) {
				out << " YES" << endl;
				trueAnswer++;
			}
			else
			{
				out << " NO" << endl;
			}
		}
		answerNet.push_back(trueAnswer);
		out << trueAnswer << "/" << xControl.size();
		out.close();
	}
	std::ofstream out(Path2 + "All.txt");
	std::copy(answerNet.begin(), answerNet.end(), std::ostream_iterator<int>(out, "\n"));
}

void lookAnswerData(vector<Data> b1) {
	cout << "Work Net!" << endl;
	string Path1, Path2, Path3;

	cout << "Path to save: " << endl;
	getline(cin, Path2);
	cout << "Path to weight: " << endl;
	getline(cin, Path3);

	int kolComp;
	cout << "Number of components = ";
	cin >> kolComp;

	Net generalNet(Path2);
	generalNet.savePath = Path2;
	cout << endl << "	start SVD" << endl;
	moduleSVD c1(b1[0].data);

	vector<vector<double>> xControl, dControl;
	xControl = c1.getNewhData(kolComp, b1[2].data);
	dControl = b1[2].answer;
	/*
	for (size_t i = 0; i < xControl.size(); i++) {
		xControl[i][0] = xControl[i][0] / 10;
	}
	*/
	std::ofstream out(Path2 + "\\controlShowData.txt");
	for (size_t i = 0; i < xControl.size(); i++) {
		out << endl;
		vector<double> y = generalNet.startNet(xControl[i]);
		for (decltype(y.size()) j = 0; j < y.size(); j++) {
			out << dControl[i][j] << "       ";
		}
		out << endl;
		for (decltype(y.size()) j = 0; j < y.size(); j++) {
			out << y[j] << " ";
		}
		out << endl;
	}
}
//---------------------------------------------------------

//---------------------------------------------------------
int main(int argc, char* argv[]) {
	
	string start;
	cout << "Path data" << endl;
	getline(cin, start);
	ParseData data(start);
	//ParseData diod(start + "\diod");
	//ParseData visible(start + "\visible");
		cout << "\nteach\nwork\nlook\n";
		getline(cin, start);
		cout << endl;
		if (start == "teach") {
			getline(cin, start);
			cout << endl;
			if (start == "NewCross") {
				vector<CrossValid> crossV = CrossValidation( data.getClassDistribution());
				getline(cin, start);
				cout << endl;
				saveCrossValid(start, crossV);
				vector<Data> b1 = data.getDataCrossValid(crossV);
				teachNet(b1);
			}



		}
		/*if (start == "work")
			workNet();

		if ((start == "look"))
			lookAnswerData();
	*/

	
	
	return 0;
}
