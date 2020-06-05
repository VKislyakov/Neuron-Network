#include "SpectralClassifier.h"



SpectralClassifier::SpectralClassifier()
{
}

SpectralClassifier::~SpectralClassifier()
{
}
//----------------------------------------------------------

//----------------------------------------------------------
SpectralClassifier::SpectralClassifier(bool teachOrRead,string Path, string dataPath):PathS(Path){
	// Инициализация сетей.
	if(!teachOrRead){ 
		{Net newNet(Path + "\\ir\\saveWeight0.txt");
		irNet = newNet; }
		{Net newNet(Path + "\\diod\\saveWeight0.txt");
		diodNet = newNet; }
		{Net newNet(Path + "\\visible\\saveWeight0.txt");
		visibleNet = newNet; }
	}
	else {
		int a, b;/*
		cout << "\nNumber input data & number neuron:\n";
		cin >> a >> b;
		vector<int> settings_net = { 2,a,b,14 };
		*/
		{cout << "\nNumber input data & number neuron for ir Net:\n";
		cin >> a >> b;
		vector<int> settings_net = { 2,a,b,14 };
		Net newNet(settings_net);
		irNet = newNet; 
		irNet.savePath = Path + "\\ir";}
		//------------------
		{cout << "\nNumber input data & number neuron for diod Net:\n";
		cin >> a >> b;
		vector<int> settings_net = { 2,a,b,14 };
		Net newNet(settings_net);
		diodNet = newNet; 
		diodNet.savePath = Path + "\\diod"; }

		{cout << "\nNumber input data & number neuron for visible net:\n";
		cin >> a >> b;
		vector<int> settings_net = { 2,a,b,14 };
		Net newNet(settings_net);
		visibleNet = newNet; 
		visibleNet.savePath = Path + "\\visible"; }
	}
	// Создание главной сети.
	{vector<int> conf = { 1,42,14 };
	Net newNet(conf);
	classifier = newNet;
	classifier.savePath = Path + "\\Classifier";
	}

	// Обработка данных.
	ParseData dataParsIr(dataPath + "\\ir");
	ParseData dataParsDiod(dataPath + "\\diod");
	ParseData dataParsVisible(dataPath + "\\visible");

	vector<CrossValid> generalCross = readCrossValid(Path+"\\crossValid.txt");
	vector<Data> dataIr = dataParsIr.getDataCrossValid(generalCross);
	vector<Data> dataDiod = dataParsDiod.getDataCrossValid(generalCross);
	vector<Data> dataVisible = dataParsVisible.getDataCrossValid(generalCross);
	cout << "start SVD" << endl;
	moduleSVD svdIr(dataIr[0].data);
	moduleSVD svdDiod(dataDiod[0].data);
	moduleSVD svdVisible(dataVisible[0].data);
	cout << "end SVD" << endl;
	Data buffPush;

	//---------------------------------------------------------------
	if (teachOrRead) {
		teachOneClassifier(visibleNet, svdVisible, dataVisible, 0.06);
		teachOneClassifier(irNet, svdIr, dataIr, 0.07);
		teachOneClassifier(diodNet, svdDiod, dataDiod, 0.195);
	}
	//----------------------------------------------------------------
	vector<vector<double>> xIr,xDiod,xVisible;
	// Ir1
	
	xIr = svdIr.getTeachData(irNet.getNumberInputData());
	double xIrDiv = divider(xIr[0][0]);
	divisionComponents(xIr, xIrDiv);
	buffPush.data = irNet.workResult(xIr);

	// Diod1
	xDiod = svdDiod.getTeachData(diodNet.getNumberInputData());
	double xDiodDiv = divider(xDiod[0][0]);
	divisionComponents(xDiod, xDiodDiv);
	int i = 0;
	for (auto x : diodNet.workResult(xDiod)) {
		for(auto xItem: x)
			buffPush.data[i].push_back(xItem);
		i++;
	}

	// Visible1
	i = 0;
	xVisible = svdVisible.getTeachData(visibleNet.getNumberInputData());
	double xVisibleDiv = divider(xVisible[0][0]);
	divisionComponents(xVisible,xVisibleDiv);
	for (auto x : visibleNet.workResult(xVisible)) {
		for (auto xItem : x)
			buffPush.data[i].push_back(xItem);
		i++;
	}
	buffPush.answer = dataVisible[0].answer;
	dataCollegium.push_back(buffPush);
	cout << "clr" << endl;
	buffPush.clear();
	
	//---------------------------------------------------------------------------
	// Ir2
	xIr = svdIr.getNewhData(irNet.getNumberInputData(), dataIr[1].data);
	divisionComponents(xIr, xIrDiv);
	buffPush.data = irNet.workResult(xIr);

	// Diod2
	xDiod = svdDiod.getNewhData(diodNet.getNumberInputData(), dataDiod[1].data);
	divisionComponents(xDiod, xDiodDiv);
	i = 0;
	for (auto x : diodNet.workResult(xDiod)) {
		for (auto xItem : x)
			buffPush.data[i].push_back(xItem);
		i++;
	}

	// Visible2
	xVisible = svdVisible.getNewhData(visibleNet.getNumberInputData(), dataVisible[1].data);
	divisionComponents(xVisible, xVisibleDiv);
	i = 0;
	for (auto x : visibleNet.workResult(xVisible)) {
		for (auto xItem : x)
			buffPush.data[i].push_back(xItem);
		i++;
	}
	buffPush.answer = dataVisible[1].answer;
	dataCollegium.push_back(buffPush);
	buffPush.clear();
	//-----------------------------------------------------------------------
	// Ir3
	std::ofstream out(Path + "\\percentTrue_3_Net.txt");

	xIr = svdIr.getNewhData(irNet.getNumberInputData(), dataIr[2].data);
	divisionComponents(xIr, xIrDiv);
	buffPush.data = irNet.workResult(xIr);
	out << "Ir: " << irNet.percentTrueAnswer(xIr, dataVisible[2].answer) << endl;
	
	// Diod3
	xDiod = svdDiod.getNewhData(diodNet.getNumberInputData(), dataDiod[2].data);
	divisionComponents(xDiod, xDiodDiv);
	i = 0;
	for (auto x : diodNet.workResult(xDiod)) {
		for (auto xItem : x)
			buffPush.data[i].push_back(xItem);
		i++;
	}
	out << "Diod: " << diodNet.percentTrueAnswer(xDiod, dataVisible[2].answer) << endl;
	
	// Visible3
	i = 0;
	xVisible = svdVisible.getNewhData(visibleNet.getNumberInputData(), dataVisible[2].data);
	divisionComponents(xVisible, xVisibleDiv);
	for (auto x : visibleNet.workResult(xVisible)) {
		for (auto xItem : x)
			buffPush.data[i].push_back(xItem);
		i++;
	}
	buffPush.answer = dataVisible[2].answer;
	out << "Visible: " << visibleNet.percentTrueAnswer(xVisible, dataVisible[2].answer) << endl;
	dataCollegium.push_back(buffPush);
	//---------------------
	buffPush.clear();
	//---------------------
}
//----------------------------------------------------------
void SpectralClassifier::teachSR(double e) {
	classifier.teaching(dataCollegium[0].data, dataCollegium[0].answer, dataCollegium[1].data, dataCollegium[1].answer, e);
	std::ofstream out(PathS + "\\percentTrueCR.txt");
	out << classifier.percentTrueAnswer(dataCollegium[2].data,dataCollegium[2].answer);
}
//----------------------------------------------------------
void SpectralClassifier::teachSR_multi(int neuron,double e) {
	vector<int> conf = { 2,42,neuron,14 };
	Net newNet(conf);
	classifier = newNet;
	classifier.savePath = PathS + "\\" + to_string(neuron);
	classifier.teaching(dataCollegium[0].data, dataCollegium[0].answer, dataCollegium[1].data, dataCollegium[1].answer, e);
	std::ofstream out(PathS + "\\" + to_string(neuron) + "\\percentTrueCR.txt");
	out << classifier.percentTrueAnswer(dataCollegium[2].data, dataCollegium[2].answer);
}

//----------------------------------------------------------
void SpectralClassifier::teachOneClassifier(Net &generalNet, moduleSVD &svd, vector<Data> &data, double e) {
	vector<vector<double>> x, testX, d, testD;

	x = svd.getTeachData(generalNet.getNumberInputData());
	d = data[0].answer;
	testX = svd.getNewhData(generalNet.getNumberInputData(), data[1].data);
	testD = data[1].answer;

	divisionComponents(testX, divider(x[0][0]));
	divisionComponents(x, divider(x[0][0]));

	generalNet.teaching(x, d, testX, testD, e);
}