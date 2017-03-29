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
/*	
	Вспомагалельная функция
	Возвращает все что есть в папке(имена)
											*/
vector<string> getDirectoryAttachments(string dPath) {
	path p(dPath);
	cout << p << " is a directory containing:\n";
	vector<string> v;
	for (auto&& x : directory_iterator(p))
		v.push_back(x.path().filename().string());
	sort(v.begin(), v.end());
	return v;
}
//------------------------------------------------------
Bloc::Bloc(string fPath) {

	size_t posR, posL;
	posR = fPath.rfind("\\");
	posL = fPath.rfind("\\", posR - 1);
	answer = fPath.substr(posL + 1, posR - posL - 1);

	path p(fPath);
	if (exists(p)) {
		if (is_regular_file(p)) {
			std::ifstream in(fPath);
			cout << "	" + fPath << endl;
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
		int numberOfScan = 1;	// отсчет кол-ва файлов для одного экземпляра, по ней строися путь к файлу
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
			}//--------------- конец считывания файла

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

int Bloc::save(string savePath) {
	std::ofstream out;
	vector<string> savedItems = getDirectoryAttachments(savePath + "\\" + answer);
	int numberItem = savedItems.size()+1;
	if (numberItem < 10)
		out.open(savePath + "\\" + answer + "\\0" + to_string(numberItem));
	else
		out.open(savePath + "\\" + answer + "\\" + to_string(numberItem));
	for (auto dataElem : data) {
		std::copy(dataElem.begin(), dataElem.end(), std::ostream_iterator<string>(out, " "));
		out << endl;
	}
	return 0;
}

//---------------------------------------------------------

DataSet::DataSet(string dPath) {
	path directWithClasses(dPath);

	if (is_directory(directWithClasses))
	{
		cout << directWithClasses << " is a directory containing:\n";
		vector<string> classesData = getDirectoryAttachments(dPath);

		cout << "Classes :" << endl;
		for (auto&& className : classesData) {
			string classPath = dPath + "\\" + className; //	путь к папке с файлами для конкретного класса
			vector<string> classElem = getDirectoryAttachments(classPath);
			if (classElem[0].size() > 2) { // ещё не оформленые в блоки файлы
				int kolElem = stoi(classElem[classElem.size() - 1].substr(0, 2));
				for (int i = 1; i <= kolElem; i++) {
					if (i < 10) {
						Bloc a(classPath + "\\0" + to_string(i));
						blocSet.push_back(a);
					}
					else {
						Bloc a(classPath + "\\" + to_string(i));
						blocSet.push_back(a);
					}
				}
			}
			else { // уже оформленные в блоки файлы
				for (auto elem:classElem) {
					Bloc a(classPath + "\\" + elem);
					blocSet.push_back(a);
				}
			}
		}
		cout << endl << "	Create DATA END	" << endl;
	}
	else {
		cout << endl << "	Path: " + dPath + " is a not directory" << endl;
	}
}


//---------------------------------------------------------

int DataSet::save(string savePath) {
	for (auto x : blocSet)
		x.save(savePath);
	cout << "	Save END	" << endl;
	return 0;
}

//---------------------------------------------------------