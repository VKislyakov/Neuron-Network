#include "Net.h"
#include "DataSet.h"
#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <locale.h>
#include <boost/filesystem.hpp>
#include <Eigen/SVD>
using namespace boost::filesystem;
using namespace std;


int main(int argc, char* argv[]) {

	/*
	vector<int> settings_net = { 2,1,6,1 }; // настройки сети
	vector<vector<double>> x = { { 0.045 },{ 0.106 },{ 0.14 },{ 0.2 },{ 0.24 },{ 0.3 },{ 0.35 },{ 0.39 },{ 0.43 },{ 0.495 },{ 0.54 },{ 0.61 } };
	vector<vector<double>> d;
	for (auto arg : x) {
		vector<double> a = { sin(10 * arg[0]) / 2 };
		d.push_back(a);
	}
	bool start_control;
	cout << "0 - start with teacing222"<< endl <<" 1 - start without teacing"<<endl;
	cin >> start_control;

	if (start_control) {
		Net generalNet(true);
		ofstream out("rez.txt");
		for (double i = 0.01; i <0.63; i = i + 0.01) {
			x[0][0] = i;
			d[0] = generalNet.startNet(x[0]);
			out << d[0][0] << endl;
		}
		out.close();
	}
	else{
		Net generalNet(settings_net);
		generalNet.teaching(x, d);
	}
	*/

	string fPath;
	getline(cin, fPath);
	//string fPath = "C:\\VKR\\save\\test.txt";
	//vector<string> v = getDirectoryAttachments(fPath);
	//vector<double> d(v.size(),0);
	DataSet a(fPath);
	




	return 0;
}
