#include "DataSet.h"


DataSet::DataSet() {

}
//---------------------------------------------------------
DataSet::~DataSet() {

}
//---------------------------------------------------------
Bloc::Bloc() {

}
//---------------------------------------------------------
Bloc::~Bloc() {

}
//---------------------------------------------------------

Bloc::Bloc(string fPath) {

	size_t posR, posL;
	posR = fPath.rfind("\\");
	posL = fPath.rfind("\\", posR - 1);
	answer = fPath.substr(posL + 1, posR - posL - 1);

	path p(fPath);
	if (exists(p)) {
		if (is_regular_file(p)) {
			std::ifstream in(fPath);
			string bufData;
			while (getline(in, bufData))
			{
				vector<string> dataElem;
				size_t posR, posL;
				posL = 0;
				while (posL != bufData.find_last_of(" ")) {
					posR = bufData.find(" ", posL + 1);
					if (posL > 0)
						dataElem.push_back(bufData.substr(posL + 1, posR - posL - 1));
					else
						dataElem.push_back(bufData.substr(posL, posR - posL));
					posL = posR;
				}
				data.push_back(dataElem);
				dataElem.clear();
			}
		}
	}
	else {
		int numberOfScan = 1;	// отсчет кол-ва файлов дл€ одного экземпл€ра, по ней строис€ путь к файлу
		path p(fPath + " 0" + to_string(numberOfScan));

		//	проход по файлам, пока существуют предполагаемые файлы
		while (exists(p)) {
			vector<string> dataElem;
			std::ifstream in;
			if (numberOfScan < 10) {
				in.open(fPath + " 0" + to_string(numberOfScan));
				cout << "Read file: " + fPath + " 0" + to_string(numberOfScan) << endl;
			}
			else {
				in.open(fPath + " " + to_string(numberOfScan));
				cout << "Read file: " + fPath + " " + to_string(numberOfScan) << endl;
			}
			string Elem;

			// считывание файла
			while (getline(in, Elem)) {
				dataElem.push_back(Elem.substr((Elem.find(" ") + 1)));
			}//--------------- конец считывани€ файла

			data.push_back(dataElem);


			++numberOfScan;
			if (numberOfScan < 10)
				p = path(fPath + " 0" + to_string(numberOfScan));
			else
				p = path(fPath + " " + to_string(numberOfScan));
			dataElem.clear();
			in.close();
		} //---------------- конец прохода по файлам
	}
}

//---------------------------------------------------------

void Bloc::save(string sPath, int numberElem) {
	std::ofstream out;
	if (numberElem < 10)
		out.open(sPath + "\\"+ answer + "\\ 0" + to_string(numberElem));
	else 
		out.open(sPath + "\\" + answer + "\\ " + to_string(numberElem));
	for (auto dataElem : data) {
		std::copy(dataElem.begin(), dataElem.end(), std::ostream_iterator<string>(out, " "));
		out << endl;
	}
}

//---------------------------------------------------------

void DataSet::createDataFile() {

}

//---------------------------------------------------------
