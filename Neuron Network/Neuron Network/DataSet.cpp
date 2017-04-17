#include "DataSet.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
ParseData::ParseData() {

}
//---------------------------------------------------------
ParseData::~ParseData() {

}
//---------------------------------------------------------
Bloc::Bloc() {

}
//---------------------------------------------------------
Bloc::~Bloc() {

}
//---------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

//---------------------------------------------------------
// Public methods.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------------------------------------

/*
	Auxiliary function.
	Returns everything in the folder (names).
*/
vector<string> ParseData::getDirectoryAttachments(string dPath) {
	path p(dPath);
	vector<string> v;
	for (auto&& x : directory_iterator(p))
		v.push_back(x.path().filename().string());
	sort(v.begin(), v.end());
	return v;
}
//------------------------------------------------------
vector<Data> ParseData::getDataTEST() {
	vector<Data> getBack;
	Data teach, test, control;
	size_t sizeblocSet = 0, sizeSum = 0;
	
	for (auto sizeData : numberDataItems) {
		// Distribution of data between sets.
		
		size_t  sizeTeach, sizeTest, sizeControl;
		sizeTeach = sizeData*0.6;
		sizeTest = (sizeData - sizeTeach)*0.5;
		sizeControl = sizeData - sizeTeach - sizeTest;
		for (/**/; sizeblocSet < sizeSum + sizeData; sizeblocSet++) {
			if (sizeblocSet < sizeSum+ sizeTeach) {
				for (auto v : blocSet[sizeblocSet].data) {
					teach.data.push_back(v);
					teach.answer.push_back(dataAnswer[sizeblocSet]);
				}
			}
			else if(sizeblocSet < sizeSum + sizeTeach+ sizeTest){
				for (auto v : blocSet[sizeblocSet].data) {
					test.data.push_back(v);
					test.answer.push_back(dataAnswer[sizeblocSet]);
				}
			}
			else {
				for (auto v : blocSet[sizeblocSet].data) {
					control.data.push_back(v);
					control.answer.push_back(dataAnswer[sizeblocSet]);
				}
			}
		}
		sizeSum += sizeData;
	}
	getBack.push_back(teach);
	getBack.push_back(test);
	getBack.push_back(control);
	return getBack;
}
//---------------------------------------------------------
vector<Data> ParseData::getDataCrossValid(vector<CrossValid> crossV) {
	Data teach, test, control;
	size_t sizeSum = 0;
	for (size_t numberClass = 0; numberClass < crossV.size(); numberClass++) {

		for (auto i: crossV[numberClass].teach)
			for (auto v : blocSet[sizeSum + i].data) {
				teach.data.push_back(v);
				teach.answer.push_back(dataAnswer[sizeSum + i]);
			}
		for (auto i : crossV[numberClass].test)
			for (auto v : blocSet[sizeSum + i].data) {
				test.data.push_back(v);
				test.answer.push_back(dataAnswer[sizeSum + i]);
			}
		for (auto i : crossV[numberClass].control)
			for (auto v : blocSet[sizeSum + i].data) {
				control.data.push_back(v);
				control.answer.push_back(dataAnswer[sizeSum + i]);
			}
		sizeSum += numberDataItems[numberClass];
	}
	vector<Data> buffReturn;
	buffReturn.push_back(teach);
	buffReturn.push_back(test);
	buffReturn.push_back(control);
	return buffReturn;
}
//---------------------------------------------------------
vector<int> ParseData::getClassDistribution() {
	return(numberDataItems);
}
//---------------------------------------------------------
Bloc::Bloc(string fPath) {

	path p(fPath);
	// If read from the file at once.
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
	// If there are several files.
	else {
		int numberOfScan = 1;	// Counting the number of files for one instance, the path to the file is built on it.
		path p(fPath + " 0" + to_string(numberOfScan));

		//	Passage on files while there are supposed files.
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

			// Reading a file.
			while (getline(in, Elem)) {
				dataElem.push_back(stod(Elem.substr((Elem.find(" ") + 1))));
			}

			data.push_back(dataElem);

			++numberOfScan;
			if (numberOfScan < 10)
				p = path(fPath + " 0" + to_string(numberOfScan));
			else
				p = path(fPath + " " + to_string(numberOfScan));
			dataElem.clear();
			in.close();
		}
	}
}
//---------------------------------------------------------
ParseData::ParseData(string dPath) {
	path directWithClasses(dPath);

	if (is_directory(directWithClasses))
	{
		cout << directWithClasses << " is a directory containing Classes :" << endl;
		vector<string> classesData = getDirectoryAttachments(dPath);

		size_t numberC = 0; //	Number of the current class to form a dictionary with answers
		for (auto&& className : classesData) {

			// Forming a dict with answers.
			vector<double> d(classesData.size(), 0.05);
			d[numberC] = 0.95;
			++numberC;
			mapAnswer.insert(pair<vector<double>, string>(d, className));	

			//	Path to a folder with files for a particular class.
			string classPath = dPath + "\\" + className; 
			//
			vector<string> classElem = getDirectoryAttachments(classPath);
			int kolElem = stoi(classElem[classElem.size() - 1].substr(0, 2));
			numberDataItems.push_back(kolElem);
			//numberClassItems.push_back(kolElem);
			for (int i = 1; i <= kolElem; i++) {
				if (i < 10) {
					Bloc a(classPath + "\\0" + to_string(i));
					blocSet.push_back(a);
					dataAnswer.push_back(d);
				}
				else {
					Bloc a(classPath + "\\" + to_string(i));
					blocSet.push_back(a);
					dataAnswer.push_back(d);
				}
			}
			d.clear();
		}

		cout << endl << "	Create DATA END	" << endl;
	}
	else {
		cout << endl << "	Path: " + dPath + " is a not directory" << endl;
	}
}
//---------------------------------------------------------
//Cross validatiom methots.
//---------------------------------------------------------
vector<CrossValid> CrossValidation(vector<int> classDistribution) {
	srand(time(0));
	vector<CrossValid> buffReturn;
	for (int sizeData : classDistribution) {
		CrossValid buffPush;
		vector<int> vForRandCreate;
		for (int i = 0; i < sizeData; i++)
			vForRandCreate.push_back(i);
		size_t  sizeTeach, sizeTest, sizeControl;
		sizeTeach = sizeData*0.6;
		sizeTest = (sizeData - sizeTeach)*0.5;
		sizeControl = sizeData - sizeTeach - sizeTest;
		//
		while (vForRandCreate.size() > 0) {
			srand(time(0));
			int randomKey = 0;
			if (vForRandCreate.size() > 1)
				randomKey = rand() % (vForRandCreate.size() - 1);

			if (vForRandCreate.size() > (sizeTest + sizeControl)) {
				buffPush.teach.push_back(vForRandCreate[randomKey]);
				vForRandCreate.erase(vForRandCreate.begin() + randomKey);
			}
			else if (vForRandCreate.size() > sizeControl) {
				buffPush.test.push_back(vForRandCreate[randomKey]);
				vForRandCreate.erase(vForRandCreate.begin() + randomKey);
			}
			else {
				buffPush.control.push_back(vForRandCreate[randomKey]);
				vForRandCreate.erase(vForRandCreate.begin() + randomKey);

			}
		}
		buffReturn.push_back(buffPush);
	}
	return buffReturn;
}
//---------------------------------------------------------
void saveCrossValid(string Path, vector<CrossValid> v) {
	std::ofstream out(Path);
	out << v.size() << endl;
	for (size_t i = 0; i < v.size(); i++) {
		out << v[i].teach.size() << " " << v[i].test.size() << " " << v[i].control.size() << endl;
		std::copy(v[i].teach.begin(), v[i].teach.end(), std::ostream_iterator<int>(out, " "));
		out << endl;
		std::copy(v[i].test.begin(), v[i].test.end(), std::ostream_iterator<int>(out, " "));
		out << endl;
		std::copy(v[i].control.begin(), v[i].control.end(), std::ostream_iterator<int>(out, " "));
		out << endl;
	}
}
//---------------------------------------------------------
vector<CrossValid> readCrossValid(string Path) {
	std::ifstream in(Path);
	vector<CrossValid> buffReturn;
	int kol;
	in >> kol;
	for (int i = 0; i < kol; i++) {
		CrossValid buffPush;
		int kolTeach, kolTest, kolControl;
		in >> kolTeach >> kolTest >> kolControl;
		for (int j = 0; j < kolTeach; j++) {
			int b;
			in >> b;
			buffPush.teach.push_back(b);
		}
		for (int j = 0; j < kolTest; j++) {
			int b;
			in >> b;
			buffPush.test.push_back(b);
		}
		for (int j = 0; j < kolControl; j++) {
			int b;
			in >> b;
			buffPush.control.push_back(b);
		}
		buffReturn.push_back(buffPush);
	}
	return buffReturn;
}
//---------------------------------------------------------
// Private methods.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------------------------------------

//---------------------------------------------------------