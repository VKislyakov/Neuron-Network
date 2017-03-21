#include "Net.h"

Net::~Net() {
}

Net::Net(vector<int> conf) :config(conf) {
	for (int i = 1; i <= config[0]; i++) {
		Layer a(config[i], config[i + 1]);
		layers.push_back(a);
	}
};

Net::Net(vector<vector<vector<double>>> wieght)
{
	for (auto elemWieght : wieght) {
		Layer a(elemWieght);
		layers.push_back(a);
	}
}

Net::Net(bool g)
{
	ifstream in("saveW.txt");
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

vector<double> Net::startNet(vector<double> x) {
	for (int i = 0; i < config[0]; i++) {
		x = layers[i].actF(x);
	}
	return x;
}

int Net::teaching(vector<vector<double>> x, vector<vector<double>> d, double e) {

	int number_epoch = 0;
	int	random_key = 0;
	vector<bool> epochControlVector(x.size(), false);
	double epochControl = true;
	ofstream outErrouTeach("errou_teach.txt");
	double errou_teach = 10; // задаем переменную для хранения ошибки на обучающем мн-ве, должна быть больше переменной "е"

	while (errou_teach > e && number_epoch < 10000) {
		for (auto elemEpContVect : epochControlVector)
			epochControl = epochControl && elemEpContVect;
		if (epochControl) {
			for (auto &elemEpContVect : epochControlVector)
				elemEpContVect = false;
			number_epoch++;
			errou_teach = 0;
			for (decltype(x.size()) j = 0; j < x.size(); j++)
			{
				errou_teach = errou_teach + functionError(startNet(x[j]), d[j]);
			}
			if (number_epoch % 20 == 0 || number_epoch < 30 || errou_teach < e + e) {
				cout << number_epoch << " == " << errou_teach << endl;
				outErrouTeach << number_epoch << " " << errou_teach << endl;
				if (number_epoch % 200 == 0) {
					save();
				}
			}

		}
		epochControl = true;
		random_key = rand() % x.size();
		teach(x[random_key], d[random_key]);
		epochControlVector[random_key] = true;
	}
	outErrouTeach.close();
	save();
	return (0);
}

void Net::setLayer(vector<Layer> lay) {
	layers = lay;
}

// не коректная функция, производна активационной функции не вызывается, считается по формуле!!! нужно вызывать!!!
vector<vector<double>> Net::deltaM(vector<double> d) {

	vector<double> deltalay;
	vector<vector<double>> delta(config[0]);
	vector<double> activf = layers[config[0] - 1].actF();
	for (int i = 0; i < config[config.size() - 1]; i++) {
		deltalay.push_back((activf[i] - d[i])*(1 - activf[i] * activf[i]));
	}
	delta[config[0] - 1] = deltalay;

	vector<vector<double>> matrix_w;
	for (int lay = config[0] - 2; lay >= 0; lay--) {
		deltalay.clear();
		activf.clear();
		matrix_w.clear();
		matrix_w = layers[lay + 1].getMatrixW();
		vector<double> activf = layers[lay].actF();
		double sum_del_w;
		for (int j = 0; j < config[lay + 2]; j++) {
			sum_del_w = 0;
			for (decltype(matrix_w.size()) k = 0; k < matrix_w.size(); k++) {
				sum_del_w = sum_del_w + delta[lay + 1][k] * matrix_w[k][j];
			}
			deltalay.push_back(sum_del_w*(1 - activf[j] * activf[j]));
		}
		delta[lay] = deltalay;
	}
	return delta;
}

void Net::correct(vector<vector<double>> delta, double alfa, vector<double> x) {
	layers[0].correct(delta[0], x, alfa);
	for (int i = 1; i < config[0]; i++) {
		layers[i].correct(delta[i], layers[i - 1].actF(), alfa);
	}
}

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

double Net::teach(vector<double> x, vector<double> d) {
	vector<double> y = startNet(x);
	vector<vector<double>> delta = deltaM(d);
	double alfa = goldenSection(delta, x, d);
	correct(delta, alfa, x);
	return (functionError(y, d));
}

vector<vector<vector<double>>> Net::save() {
	vector<vector<vector<double>>> W;
	ofstream outs("saveW.txt");
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

double Net::functionError(vector<double> y, vector<double> d) {
	double errorf = 0;
	for (decltype(y.size()) i = 0; i < y.size(); i++) {
		errorf = errorf + (y[i] - d[i]) * (y[i] - d[i]);
	}
	return errorf / 2;
}


