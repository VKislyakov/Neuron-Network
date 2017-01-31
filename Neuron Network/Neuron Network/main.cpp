#include "Net.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

int main()
{
	vector<int> settings_net = { 2,1,6,1 }; // настройки сети
	vector<vector<double>> x = { { 0.045 },{ 0.106 },{ 0.14 },{ 0.2 },{ 0.24 },{ 0.3 },{ 0.35 },{ 0.39 },{ 0.43 },{ 0.495 },{ 0.54 },{ 0.61 } };
	vector<vector<double>> d;
	for (auto arg : x) {
		vector<double> a = { sin(10 * arg[0]) / 2 };
		d.push_back(a);
	}
	bool start_control;
	cout << "0 - start with teacing"<< endl <<" 1 - start without teacing"<<endl;
	cin >> start_control;

	if (start_control) { 
		Net General_Net(true);
		ofstream out("rez.txt");
		for (double i = 0.01; i <0.63; i = i + 0.01) {
			x[0][0] = i;
			d[0] = General_Net.startNet(x[0]);
			out << d[0][0] << endl;
		}
		out.close();

	}
	else{
		Net generalNet(settings_net);
		generalNet.teaching(x, d);
	}

	return 0;
}