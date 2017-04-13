#include "Net.h"

Net::~Net() {
}

Net::Net() {
}
//---------------------------------------------------------

//---------------------------------------------------------
// Public methods.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------------------------------------

Net::Net(vector<int> conf) :config(conf) {
	for (int i = 1; i <= config[0]; i++) {
		Layer a(config[i], config[i + 1]);
		layers.push_back(a);
	}
};
//---------------------------------------------------------
Net::Net(vector<vector<vector<double>>> wieght)
{
	for (auto elemWieght : wieght) {
		Layer a(elemWieght);
		layers.push_back(a);
	}
}
//---------------------------------------------------------
Net::Net(string Path)
{
	ifstream in(Path);
	vector<int> conf;
	int read;
	in >> read;
	conf.push_back(read);
	for (int i = 0; i <= conf[0]; i++)
	{
		in >> read;
		conf.push_back(read);
	}
	config = conf;
	vector<vector<vector<double>>> wieght;
	vector<vector<double>> lay;
	vector<double> neu;

	double w;
	for (int i = 0; i < conf[0]; i++) {
		lay.clear();
		for (int j = 0; j < (conf[i + 2]); j++) {
			neu.clear();
			for (int k = 0; k <= (conf[i + 1]); k++)
			{
				in >> w;
				neu.push_back(w);
			}
			lay.push_back(neu);
		}
		wieght.push_back(lay);
	}
	for (auto elementWieght : wieght) {
		Layer a(elementWieght);
		layers.push_back(a);
	}
}
//---------------------------------------------------------
vector<double> Net::startNet(vector<double> x) {
	for (int i = 0; i < config[0]; i++) {
		x = layers[i].actF(x);
	}
	return x;
}
//---------------------------------------------------------
int Net::teaching(vector<vector<double>> x, vector<vector<double>> d, vector<vector<double>> testX, vector<vector<double>> testD, double e) {
	cout << endl << "Start teach" << endl;
	int numberEpoch = 0;
	int	random_key = 0;
	int iter = 0;
	int errorControlEndNumber = 0;
	bool epochControl = true, diffControl = true, minControl = true;
	ofstream outErrouTeach(savePath + "\\error_teach.txt");
	double errorTeach = 10, errorMax = 0, errorMin = 5000, errorTest = 0, errorMinTest = 100;
	vector<int> numberTeachItem(x.size(),0);
	list<double> diffTeachTestErrou;
	// Begin teach.
	while (errorTeach > e && numberEpoch < 1000 && (diffControl || minControl)) {
		for (auto v : numberTeachItem)
			epochControl = epochControl && v;
		if (epochControl) {
			iter = 0;
			vector<int> a(x.size(), 0);
			numberTeachItem = a;
			numberEpoch++;
			save(numberEpoch);
			errorTeach = 0;
			errorTest = 0;
			errorMax = 0;
			errorMin = 5000;
			// Calculating errors.
			for (decltype(testX.size()) j = 0; j < testX.size(); j++)
				errorTest = errorTest + functionError(startNet(testX[j]), testD[j]);	
			errorTest = errorTest / testX.size();
			if(errorMinTest > errorTest)
				errorMinTest = errorTest;
			for (decltype(x.size()) j = 0; j < x.size(); j++)
			{
				double buffErrou = functionError(startNet(x[j]), d[j]);
				errorTeach = errorTeach + buffErrou;
				if (buffErrou > errorMax)
					errorMax = buffErrou;
				if (buffErrou < errorMin)
					errorMin = buffErrou;
			}
			errorTeach = errorTeach / x.size();
			// Error records by epoch number in console.
			cout << endl << numberEpoch << " == " << errorTeach << endl;
			cout << " TEST == " << errorTest << endl;
			cout << " MinErrou = " << errorMin << " MaxErrou = " << errorMax << endl;
			// Error records by epoch number in file.
			outErrouTeach << numberEpoch << " " << errorTeach << " " << errorTest << endl;
			// Monitoring of the change in the minimum error on the test set.
			if (diffControl) {
				if (numberEpoch == 1)
					for (size_t i = 0; i < 5; i++)
						diffTeachTestErrou.push_back(abs(errorTest - errorTeach));
				else {
					diffTeachTestErrou.push_back(errorTest - errorTeach);
					diffTeachTestErrou.pop_front();
				}
				double sumDiff = 0;
				for (auto item : diffTeachTestErrou) {
					sumDiff += item;
				}
				if (sumDiff / 5 > 0.06)
					diffControl = false;
			}
			if (!(diffControl) && minControl && ((errorMinTest + 0.01) >= errorTest)) {
				minControl = false;
				continue;
			}
		}
		// New iteration.
		epochControl = true;
		iter++;
		do
		{
			random_key = rand() % x.size();
			numberTeachItem[random_key]++;
		} while (numberTeachItem[random_key]>3);

		cout << "\r\t\t\t\t\t\t\t\r=== Iter " << iter << "\t"<< numberTeachItem[random_key];
		double buffErrou = teach(x[random_key], d[random_key]);
		cout << "\tENDteach ===";
		if (buffErrou < errorMin + (errorMax - errorMin)*0.70)
			numberTeachItem[random_key] = 3;
	}
	outErrouTeach.close();
	save(0);
	return (0);
}
//---------------------------------------------------------
void Net::setLayer(vector<Layer> lay) {
	layers = lay;
}
//---------------------------------------------------------
double Net::functionError(vector<double> y, vector<double> d) {
	double errorf = 0;
	for (decltype(y.size()) i = 0; i < y.size(); i++) {
		errorf = errorf + (y[i] - d[i]) * (y[i] - d[i]);
	}
	return errorf / 2;
}

//---------------------------------------------------------
// Private methods.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------------------------------------

vector<vector<double>> Net::deltaM(vector<double> d) {

	vector<double> deltalay;
	vector<vector<double>> delta(config[0]);
	vector<double> activf = layers[config[0] - 1].actF();
	vector<double> derivf = layers[config[0] - 1].derF();
	for (int i = 0; i < config[config.size() - 1]; i++) {
		deltalay.push_back((activf[i] - d[i])*derivf[i]);
		//deltalay.push_back((activf[i] - d[i])*(activf[i] *(1- activf[i])));
	}
	delta[config[0] - 1] = deltalay;

	vector<vector<double>> matrix_w;
	for (int lay = config[0] - 2; lay >= 0; lay--) {
		deltalay.clear();
		activf.clear();
		matrix_w.clear();
		matrix_w = layers[lay + 1].getMatrixW();
		vector<double> activf = layers[lay].actF();
		vector<double> derivf = layers[lay].derF();
		double sum_del_w;
		for (int j = 0; j < config[lay + 2]; j++) {
			sum_del_w = 0;
			for (decltype(matrix_w.size()) k = 0; k < matrix_w.size(); k++) {
				sum_del_w = sum_del_w + delta[lay + 1][k] * matrix_w[k][j];
			}
			deltalay.push_back(sum_del_w*derivf[j]);
			//deltalay.push_back(sum_del_w*(activf[j] * (1 - activf[j])));
		}
		delta[lay] = deltalay;
	}
	return delta;
}
//---------------------------------------------------------
void Net::correct(vector<vector<double>> delta, double alfa, vector<double> x) {
	layers[0].correct(delta[0], x, alfa);
	for (int i = 1; i < config[0]; i++) {
		layers[i].correct(delta[i], layers[i - 1].actF(), alfa);
	}
}
//---------------------------------------------------------
double Net::goldenSection(vector<vector<double>> delta, vector<double> x, vector<double> d) {
	Net minimiNet(config);
	minimiNet.setLayer(layers);
	double fi = (1 + sqrt(5)) / 2;
	double a = 0;
	double b = 1;
	double x1, x2, f1, f2;
	vector<double> y;
	x1 = b - (b - a) / fi;
	x2 = a + (b - a) / fi;
	while (abs(b - a) > 0.01) {

		minimiNet.setLayer(layers);//	!
		minimiNet.correct(delta, x1, x);
		y = minimiNet.startNet(x);
		f1 = minimiNet.functionError(y, d);

		minimiNet.setLayer(layers);//	!
		minimiNet.correct(delta, x2, x);
		y = minimiNet.startNet(x);
		f2 = minimiNet.functionError(y, d);

		if (f1 > f2) {
			a = x1;
			x1 = x2;
			x2 = a + (b - a) / fi;
		}
		else {
			b = x2;
			x2 = x1;
			x1 = b - (b - a) / fi;
		}
	}
	return (a + b) / 2;
}
//---------------------------------------------------------
double Net::teach(vector<double> x, vector<double> d) {
	vector<double> y = startNet(x);
	vector<vector<double>> delta = deltaM(d);
	double alfa = goldenSection(delta, x, d);
	correct(delta, alfa, x);
	return (functionError(y, d));
}
//---------------------------------------------------------
vector<vector<vector<double>>> Net::save(int i) {
	vector<vector<vector<double>>> W;
	ofstream outs(savePath+"\\saveWeight" + to_string(i) + ".txt");
	for (int i = 0; i < config[0]; i++)
	{
		vector<vector<double>> a = layers[i].getMatrixW();
		W.push_back(a);
	}
	for (decltype(config.size()) i = 0; i < config.size(); i++) {
		outs << config[i] << " ";
	}
	outs << endl;
	for (decltype(W.size()) i = 0; i < W.size(); i++) {
		for (decltype(W[0].size()) j = 0; j < W[i].size(); j++) {
			for (decltype(W[0][0].size()) k = 0; k < W[i][j].size(); k++)
				outs << W[i][j][k] << " ";
			outs << endl;
		}
	}
	outs.close();
	return(W);
}
//---------------------------------------------------------


