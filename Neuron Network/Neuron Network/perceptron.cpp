#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

class Neuron
{
public:
	Neuron(int number_weight) {
		for (int i = 0; i <= number_weight; i++) // 
			weights.push_back(0.0001 * (rand() % 4001 - 2000));  // 
		sum = 0;
		F = 0;
	}
	
	Neuron(vector<double> verton_weight) {
		weights = verton_weight;
		sum = 0;
		F = 0;
	}

	int getKolWeight() {
		return(int(weights.size()));
	}

	vector<double> setAllWeights() { // 
		return weights;
	}

	double setElemWeight(int number) {
		return(weights[number]);
	}

	double Sum(vector<double> x) {
		sum = weights[weights.size() - 1];
		for (int i = 0; i < weights.size() - 1; i++)
			sum = sum + weights[i] * x[i];
		return sum;
	}

	double Sum() {
		return sum;
	}

	double ActF(vector<double> x) {
		Sum(x);
		F = (exp(sum) - exp(-sum)) / (exp(sum) + exp(-sum));
		return F;
	}

	double ActF() {
		return F;
	}

	void correctWeights(double deltaW, vector<double> y, double alfa) {
		for (int i = 0; i < weights.size() - 1; i++)
			weights[i] = weights[i] - deltaW * y[i] * alfa;
		weights[weights.size() - 1] = weights[weights.size() - 1] - deltaW * alfa;
	}
	
	void setNewVectorWeights(vector<double> W) {
		weights = W;
	}

private:
	vector<double> weights;
	double sum;
	double F;
};

class Layer {
public:
	Layer(int early_kol_neu, int kol_neu){
		for (int i = 0; i < kol_neu; i++) {
			Neuron a(early_kol_neu);
			neurons.push_back(a);
			activ_f.push_back(0);
			sum_s.push_back(0);
		}
	}

	Layer(vector<vector<double>> W){
		for (int i = 0; i < W.size(); i++) {
			Neuron a(W[i]);
			neurons.push_back(a);
			activ_f.push_back(0);
			sum_s.push_back(0);
		}
	}

	vector<double> actF(vector<double> X){
		for (int i = 0; i < neurons.size(); i++) {
			activ_f[i] = neurons[i].ActF(X);
			sum_s[i] = neurons[i].Sum();
		}
		return(activ_f);
	}

	vector<double> actF(){
		return(activ_f);
	}

	int correct(vector<double> deltw, vector<double> y, double alfa) {
		for (int i = 0; i<neurons.size(); i++) {
			neurons[i].correctWeights(deltw[i], y, alfa);
		}
		return 0;
	}

	vector<double> getVectorW(int i) { //
		return neurons[i].setAllWeights();
	}

	vector<vector<double>> getMatrixW() { //
		vector<vector<double>> matrix;
		for (int i = 0; i<neurons.size(); i++) {
			matrix.push_back(getVectorW(i));
		}
		return matrix;
	}
private:
	vector<Neuron> neurons;
	vector<double> activ_f;
	vector<double> sum_s;
};

class Net
{
public:
	Net(vector<int> conf) {
		config = conf;
		for (int i = 1; i <= config[0]; i++) {
			Layer a(config[i], config[i + 1]);
			layers.push_back(a);
		}
	};
	
	Net(vector<vector<vector<double>>> wieght)
	{
		for (int i = 1; i <= wieght.size(); i++) {
			Layer a(wieght[i]);
			layers.push_back(a);
		}
	}
	
	Net(bool g)
	{
		vector<vector<vector<double>>> wieght;
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
		vector<double> neu;
		vector<vector<double>> lay;
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

		for (int i = 0; i < wieght.size(); i++) {
			Layer a(wieght[i]);
			layers.push_back(a);
		}
	}
	
	double functionError(vector<double> y, vector<double> d) {
		double errorf = 0;
		for (int i = 0; i < y.size(); i++)
		{
			errorf = errorf + (y[i] - d[i]) * (y[i] - d[i]);
		}
		return errorf / 2;
	}
	
	vector<double> startNet(vector<double> x) { 
		for (int i = 0; i<config[0]; i++) {
			x = layers[i].actF(x);
		}

		return x;
	}
	
	double teach(vector<double> x, vector<double> d) {
		vector<double> y = startNet(x);
		vector<vector<double>> delta = deltaM(d);
		double alfa = goldenSection(delta, x, d);
		correct(delta, alfa, x);
		return (functionError(y, d));
	}
	
	vector<vector<vector<double>>> save() {
		vector<vector<vector<double>>> W;
		ofstream outs("saveW.txt");
		for (int i = 0; i < config[0]; i++)
		{
			vector<vector<double>> a = layers[i].getMatrixW();
			W.push_back(a);
		}
		for (int i = 0; i < config.size(); i++) {
			outs << config[i] << " ";
		}
		outs << endl;
		for (int i = 0; i < W.size(); i++) {
			for (int j = 0; j < W[i].size(); j++) {
				for (int k = 0; k < W[i][j].size(); k++)
					outs << W[i][j][k] << " ";
				outs << endl;
			}
		}
		outs.close();
		return(W);
	}
	
	int teaching(vector<double> x, vector<double> d){
		return (0);
	}
private:
	int kol_sloev;
	vector<int> config;
	vector<Layer> layers;

	vector<vector<double>> deltaM(vector<double> d) {
		//	���������� ������ �� ��������� ����
		vector<double> deltalay;
		vector<vector<double>> delta(config[0]);
		vector<double> activf = layers[config[0] - 1].actF();
		for (int i = 0; i<config[config.size() - 1]; i++) {
			deltalay.push_back((activf[i] - d[i])*(1 - activf[i] * activf[i]));
		}
		delta[config[0] - 1] = deltalay;
		//	��������� �������� ������ ���������� ����
		//	���������� ������ �� ��������� �����
		vector<vector<double>> matrix_w;
		for (int lay = config[0] - 2; lay >= 0; lay--) {// lay ����� ����
			deltalay.clear();
			activf.clear();
			matrix_w.clear();
			matrix_w = layers[lay + 1].getMatrixW();// ���� �������� ���� lay+1
			vector<double> activf = layers[lay].actF();// �������� �������� ���� lay
			double sum_del_w;
			for (int j = 0; j<config[lay + 2]; j++) {
				sum_del_w = 0;
				for (int k = 0; k < matrix_w.size(); k++) {
					sum_del_w = sum_del_w + delta[lay + 1][k] * matrix_w[k][j];
				}
				deltalay.push_back(sum_del_w*(1 - activf[j] * activf[j]));
			}
			delta[lay] = deltalay;
		}
		//	��������� �������� ������
		return delta;
	}
	
	void correct(vector<vector<double>> delta, double alfa, vector<double> x) {
		layers[0].correct(delta[0], x, alfa);
		for (int i = 1; i<config[0]; i++) {
			layers[i].correct(delta[i], layers[i - 1].actF(), alfa);
		}
	}
	
	void setLayer(vector<Layer> lay) {
		layers = lay;
	}
	
	double goldenSection(vector<vector<double>> delta, vector<double> x, vector<double> d) {
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
	
};

int main()
{
	vector<int> settings_net = { 2,1,6,1 }; // настройки сети
	vector<bool> epoch;// 
	double epoch_control = true;
	//	
	vector<vector<double>> test = { { 0.022 },{ 0.067 },{ 0.125 },{ 0.16 },{ 0.22 },{ 0.27 },{ 0.32 },{ 0.37 },{ 0.40 },{ 0.469 },{ 0.518 },{ 0.57 } };
	vector<vector<double>> test_d;
	for (int i = 0; i < test.size(); i++) {
		vector<double> a = { sin(10 * test[i][0]) / 2 };
		test_d.push_back(a);
	}
	vector<vector<double>> x = { { 0.045 },{ 0.106 },{ 0.14 },{ 0.2 },{ 0.24 },{ 0.3 },{ 0.35 },{ 0.39 },{ 0.43 },{ 0.495 },{ 0.54 },{ 0.61 } };
	vector<vector<double>> d;
	for (int i = 0; i < x.size(); i++) {
		vector<double> a = { sin(10 * x[i][0]) / 2 };
		d.push_back(a);
		epoch.push_back(false);
	}
	// 
	int start_control;
	cout << "1 - start with teacing"<< endl <<" 0 - start without teacing"<<endl;
	cin >> start_control;

	if (start_control == 0) { //
		Net General_Net(true);
		ofstream out("rez.txt");
		for (double i = 0.01; i <0.63; i = i + 0.01) {
			x[0][0] = i;
			d[0] = General_Net.startNet(x[0]);
			out << d[0][0] << endl;
		}
		out.close();

	}
	else {
		int number_epo�h = 0; //
		int	random_key = 0; // 
		Net general_Net(settings_net);

		ofstream out_errou_teach("errou_teach.txt");
		ofstream out_errou_test("errou_test.txt");

		double errou_teach = 10, errou_test = 10;
		double e = 0.0001;
		while (errou_teach > e && number_epo�h<100000)
		{
			for (int j = 0; j < x.size(); j++) // 
				epoch_control = epoch_control && epoch[j];
			if (epoch_control) {	// 
				for (int j = 0; j < x.size(); j++) // 
					epoch[j] = false;
				number_epo�h++;
				errou_teach = 0;
				for (int j = 0; j < x.size(); j++) // 
				{
					errou_teach = errou_teach + general_Net.functionError(general_Net.startNet(x[j]), d[j]);
				}
				if (number_epo�h % 20 == 0 || number_epo�h < 30 || errou_teach < e + e) { //
					cout << number_epo�h << " == " << errou_teach << endl;
					out_errou_teach << number_epo�h << " " << errou_teach << endl;
					errou_test = 0;
					for (int j = 0; j < test.size(); j++)
					{
						errou_test = errou_test + general_Net.functionError(general_Net.startNet(test[j]), test_d[j]);
					}
					cout << number_epo�h << " test== " << errou_test << endl;
					out_errou_test << number_epo�h << " " << errou_test << endl;

				}
				if (number_epo�h % 200 == 0 || errou_teach <= e + e) { // 
					general_Net.save();
				}
			}

			epoch_control = true;
			random_key = rand() % x.size(); // 
			general_Net.teach(x[random_key], d[random_key]);
			epoch[random_key] = true;

		}
		out_errou_teach.close();

		out_errou_teach.open("rez.txt");
		for (double i = 0.01; i < 0.63; i = i + 0.01) {
			x[0][0] = i;
			d[0] = general_Net.startNet(x[0]);
			out_errou_teach << d[0][0] << endl;
		}
	}


	return 0;
}
