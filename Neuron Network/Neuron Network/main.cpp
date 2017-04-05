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
#include <locale.h>
#include <boost/filesystem.hpp>
#include <Eigen/SVD>
using namespace boost::filesystem;
using namespace std;
using namespace Eigen;


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

	
	string Path1, Path2, Path3;

	getline(cin, Path1);
	getline(cin, Path2);
	getline(cin, Path3);
	
	ParseData a1(Path1);
	ParseData a2(Path1);
	ParseData a3(Path1);

	vector<Data> b1 = a1.getDataTEST();
	vector<Data> b2 = a2.getDataTEST();
	vector<Data> b3 = a3.getDataTEST();

	moduleSVD c1(b1[0].data);
	moduleSVD c2(b2[0].data);
	moduleSVD c3(b3[0].data);

	vector<vector<double>> v1,test1;
	vector<vector<double>> v2, test2;
	vector<vector<double>> v3, test3;

	v1 = c1.getTeachData(20);
	test1 = c1.getNewhData(20, b1[1].data);

	v2 = c2.getTeachData(20);
	test2 = c2.getNewhData(20, b2[1].data);

	v3 = c3.getTeachData(20);
	test3 = c3.getNewhData(20, b3[1].data);


	
	//cout << c.svd.singularValues();
	
	
	
	return 0;
}
