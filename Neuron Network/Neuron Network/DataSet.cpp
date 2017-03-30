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
	Auxiliary function.
	Returns everything in the folder (names).
*/
vector<string> getDirectoryAttachments(string dPath) {
	path p(dPath);
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

	// ���� ���������� ����� ��  �����
	if (exists(p)) {
		if (is_regular_file(p)) {
			std::ifstream in(fPath);
			cout << "	" + fPath << endl;
			string bufData;
			while (getline(in, bufData))
			{
				vector<double> dataElem;
				size_t posR, posL;
				posL = 0;
				while (posL != bufData.find_last_of(" ")) {
					posR = bufData.find(" ", posL + 1);
					if (posL > 0)
						dataElem.push_back(stod(bufData.substr(posL + 1, posR - posL - 1)));
					else
						dataElem.push_back(stod(bufData.substr(posL, posR - posL)));
					posL = posR;
				}
				data.push_back(dataElem);
				dataElem.clear();
			}
		}
	}
	// ���� ������ ���������
	else {
		int numberOfScan = 1;	// ������ ���-�� ������ ��� ������ ����������, �� ��� ������� ���� � �����
		path p(fPath + " 0" + to_string(numberOfScan));

		//	������ �� ������, ���� ���������� �������������� �����
		while (exists(p)) {
			vector<double> dataElem;
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

			// ���������� �����
			while (getline(in, Elem)) {
				dataElem.push_back(stod(Elem.substr((Elem.find(" ") + 1))));
			}//--------------- ����� ���������� �����

			data.push_back(dataElem);


			++numberOfScan;
			if (numberOfScan < 10)
				p = path(fPath + " 0" + to_string(numberOfScan));
			else
				p = path(fPath + " " + to_string(numberOfScan));
			dataElem.clear();
			in.close();
		} //---------------- ����� ������� �� ������
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
		std::copy(dataElem.begin(), dataElem.end(), std::ostream_iterator<double>(out, " "));
		out << endl;
	}
	return 0;
}

//---------------------------------------------------------

DataSet::DataSet(string dPath) {
	path directWithClasses(dPath);

	if (is_directory(directWithClasses))
	{
		cout << directWithClasses << " is a directory containing Classes :" << endl;
		vector<string> classesData = getDirectoryAttachments(dPath);

		size_t numberC = 0; //	����� �������� ������ ��� ������������ ������� � ��������
		for (auto&& className : classesData) {
			// Buffer vector
			vector<Bloc> blocSet;
			string classPath = dPath + "\\" + className; //	���� � ����� � ������� ��� ����������� ������
			vector<string> classElem = getDirectoryAttachments(classPath);

			// Not decorated in blocks files
			if (classElem[0].size() > 2) { 
				int kolElem = stoi(classElem[classElem.size() - 1].substr(0, 2));
				//numberClassItems.push_back(kolElem);
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
			// Decorated in blocks files
			else { 
				//numberClassItems.push_back(stoi(classElem[classElem.size() - 1].substr(0, 2)));
				for (auto elem:classElem) {
					Bloc a(classPath + "\\" + elem);
					blocSet.push_back(a);
				}
			}
			// Forming a vocabulary with answers.
			vector<double> d(classesData.size(), 0);
			d[numberC] = 1;
			mapAnswer.insert(pair<string, vector<double>>(className, d));
			mapData.insert(pair<vector<double>, vector<Bloc>>(d, blocSet));
			++numberC;
			blocSet.clear();
			d.clear();
			// End of dictionary formation.			
		}

		cout << endl << "	Create DATA END	" << endl;
	}
	else {
		cout << endl << "	Path: " + dPath + " is a not directory" << endl;
	}
}


//---------------------------------------------------------

int DataSet::save(string savePath) {
	for(auto blocSet = mapData.begin(); blocSet != mapData.end(); blocSet++)
		for (auto x : blocSet->second)
			x.save(savePath);
	cout << "	Save END	" << endl;
	return 0;
}

//---------------------------------------------------------

map<vector<double>, vector<Bloc>> DataSet::getData() {
	return mapData;
}

//---------------------------------------------------------