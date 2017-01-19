#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

class neuron
{
public:
	neuron(int k) {
		for (int i = 0; i <= k; i++) // на один больше чем на предыдушем слое нейнонов
			weight.push_back(0.0001 * (rand() % 4001 - 2000));  // от -0.2 до 0.2
		sum = 0;
		F = 0;
	}
	neuron(vector<double> w) {
		weight = w;
		sum = 0;
		F = 0;
	}
	int set_KolWeig() {
		return(int(weight.size()));
	}
	vector<double> set_AllWeights() { // отдать вектор весов
		return weight;
	}
	double set_ElemWeight(int i) {
		return(weight[i]);
	}
	double Sum(vector<double> x) {
		sum = weight[weight.size() - 1];
		for (int i = 0; i < weight.size() - 1; i++)
			sum = sum + weight[i] * x[i];
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
	void correct_weights(double deltaW, vector<double> y, double alfa) {
		for (int i = 0; i < weight.size() - 1; i++)
			weight[i] = weight[i] - deltaW * y[i] * alfa;
		weight[weight.size() - 1] = weight[weight.size() - 1] - deltaW * alfa;
	}
	void get_NewVectorWeights(vector<double> W) {
		weight = W;
	}

private:
	vector<double> weight;
	double sum;
	double F;
};

class layer
{
public:
	layer(int early_kol_neu, int kol_neu)
	{
		for (int i = 0; i < kol_neu; i++) {
			neuron a(early_kol_neu);
			neurons.push_back(a);
			activ_f.push_back(0);
			sum_s.push_back(0);
		}
	}
	layer(vector<vector<double>> W)
	{
		for (int i = 0; i < W.size(); i++) {
			neuron a(W[i]);
			neurons.push_back(a);
			activ_f.push_back(0);
			sum_s.push_back(0);
		}
	}
	vector<double> actF(vector<double> X)
	{

		for (int i = 0; i < neurons.size(); i++) {
			activ_f[i] = neurons[i].ActF(X);
			sum_s[i] = neurons[i].Sum();
		}
		return(activ_f);
	}
	vector<double> actF()
	{
		return(activ_f);
	}
	int correct(vector<double> deltw, vector<double> y, double alfa) {
		for (int i = 0; i<neurons.size(); i++) {
			neurons[i].correct_weights(deltw[i], y, alfa);
		}
		return 0;
	}
	vector<double> set_VectW(int i) { // отдает веса конкретного нейрона
		return neurons[i].set_AllWeights();
	}
	vector<vector<double>> set_MatrixW() { // отдает матрицу весов всех нейронов слоя
		vector<vector<double>> matrix;
		for (int i = 0; i<neurons.size(); i++) {
			matrix.push_back(set_VectW(i));
		}
		return matrix;
	}
private:
	vector<neuron> neurons;
	vector<double> activ_f;
	vector<double> sum_s;
};

class net
{
public:
	net(vector<int> conf) {
		config = conf;
		for (int i = 1; i <= config[0]; i++) {
			layer a(config[i], config[i + 1]);
			layers.push_back(a);
		}
	};
	net(vector<vector<vector<double>>> wieght)
	{
		for (int i = 1; i <= wieght.size(); i++) {
			layer a(wieght[i]);
			layers.push_back(a);
		}
	}
	net(bool g)
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
			layer a(wieght[i]);
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
	vector<double> startnet(vector<double> x) { // запуск
		for (int i = 0; i<config[0]; i++) {
			x = layers[i].actF(x);
		}

		return x;
	}
	//	матрица дельт для всех нейронов
	vector<vector<double>> deltaM(vector<double> d) {
		//	определяем дельты на последнем слое
		vector<double> deltalay;
		vector<vector<double>> delta(config[0]);
		vector<double> activf = layers[config[0] - 1].actF();
		for (int i = 0; i<config[config.size() - 1]; i++) {
			deltalay.push_back((activf[i] - d[i])*(1 - activf[i] * activf[i]));
		}
		delta[config[0] - 1] = deltalay;
		//	закончили находить дельты последнего слоя
		//	определяем дельты на остальных слоях
		vector<vector<double>> matrix_w;
		for (int lay = config[0] - 2; lay >= 0; lay--) {// lay номер слоя
			deltalay.clear();
			activf.clear();
			matrix_w.clear();
			matrix_w = layers[lay + 1].set_MatrixW();// веса нейронов слоя lay+1
			vector<double> activf = layers[lay].actF();// выходные значения слоя lay
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
		//	закончили находить дельты
		return delta;
	}
	void correct(vector<vector<double>> delta, double alfa, vector<double> x) {
		layers[0].correct(delta[0], x, alfa);
		for (int i = 1; i<config[0]; i++) {
			layers[i].correct(delta[i], layers[i - 1].actF(), alfa);
		}
	}
	void get_lay(vector<layer> lay) {

		layers = lay;
	}
	double golden_section(vector<vector<double>> delta, vector<double> x, vector<double> d) {
		net minimi_net(config);
		minimi_net.get_lay(layers);
		double fi = (1 + sqrt(5)) / 2;
		double a = 0;
		double b = 1;
		double x1, x2, f1, f2;
		vector<double> y;
		x1 = b - (b - a) / fi;
		x2 = a + (b - a) / fi;
		while (abs(b - a) > 0.01) {
			minimi_net.get_lay(layers);//	!
			minimi_net.correct(delta, x1, x);
			y = minimi_net.startnet(x);
			f1 = minimi_net.functionError(y, d);
			minimi_net.get_lay(layers);//	!
			minimi_net.correct(delta, x2, x);
			y = minimi_net.startnet(x);
			f2 = minimi_net.functionError(y, d);
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
	double teach(vector<double> x, vector<double> d) {
		vector<double> y = startnet(x);

		vector<vector<double>> delta = deltaM(d);

		double alfa = golden_section(delta, x, d);
		correct(delta, alfa, x);
		return (functionError(y, d));
		////////!!!
	}
	vector<vector<vector<double>>> save() {
		vector<vector<vector<double>>> W;
		ofstream outs("saveW.txt");
		for (int i = 0; i < config[0]; i++)
		{
			vector<vector<double>> a = layers[i].set_MatrixW();
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

private:
	int kol_sloev;
	vector<int> config;
	vector<layer> layers;
};

int main()
{
	vector<int> mm = { 2,1,6,1 }; // параметры сети
	vector<bool> eroug;// вектор фиксирующий конец эпохи, размерность формируется в процессе создания обучающего мн-ва
	double erou = true;
	//	Формирование обучающего и тестогвого множества
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
		eroug.push_back(false);
	}
	// начало
	int c;
	cout << "vvedite '0' chtobi zapystit NE obychaia ili '1' chtobi obychit";
	cin >> c;

	if (c == 0) { // запуск уже обученной сети
		net nn(true);
		ofstream out("rez.txt");
		for (double i = 0.01; i <0.63; i = i + 0.01) {
			x[0][0] = i;
			d[0] = nn.startnet(x[0]);
			out << d[0][0] << endl;
		}
		out.close();

	}
	else {
		int i = 0; // кол-во эпох
		int	k = 0; // используестся при выборе рандомного эл-та обуч. мн-ва
		net nn(mm);

		ofstream out("errou_teach.txt");
		ofstream out2("errou_test.txt");

		double er = 10, er_test = 10;
		double e = 0.0001;
		while (er > e && i<100000)
		{
			for (int j = 0; j < x.size(); j++) // сбор данных об эпохе
				erou = erou && eroug[j];
			if (erou) {	// проверка конца эпохи 
				for (int j = 0; j < x.size(); j++) // откатывает данные об эпохе
					eroug[j] = false;
				i++;
				er = 0;
				for (int j = 0; j < x.size(); j++) // вычисление ошибки
				{
					er = er + nn.functionError(nn.startnet(x[j]), d[j]);
				}
				if (i % 20 == 0 || i < 30 || er < e + e) { //вывод ошибки/условия
					cout << i << " == " << er << endl;
					out << i << " " << er << endl;
					er_test = 0;
					for (int j = 0; j < test.size(); j++)
					{
						er_test = er_test + nn.functionError(nn.startnet(test[j]), test_d[j]);
					}
					cout << i << " test== " << er_test << endl;
					out2 << i << " " << er_test << endl;

				}
				if (i % 200 == 0 || er <= e + e) { // сохранение весов/условия
					nn.save();
				}
			}

			erou = true;
			k = rand() % x.size(); // выбор рандомного эл-та обуч. мн-ва
			nn.teach(x[k], d[k]);
			eroug[k] = true;

		}
		out.close();
		out.open("rez.txt");
		for (double i = 0.01; i < 0.63; i = i + 0.01) {
			x[0][0] = i;
			d[0] = nn.startnet(x[0]);
			out << d[0][0] << endl;
		}
	}


	return 0;
}
