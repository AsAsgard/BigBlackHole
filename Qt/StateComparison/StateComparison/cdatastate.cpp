#include <fstream>

#include "cdatastate.h"
#include "extfunctions.h"

using namespace std;

// ��������� �������� ���������� ��� � �������� ����
int cDataState::NumberFAs = 163;

// �������� ��������� � ��������� ���������� ��� 
const cState& cDataState::operator[](int FA_Number) const
{
	// ���� �������� ��� <= 0, �� ���������� ����������
    if (FA_Number <= 0) throw invalid_argument("FA Number is <= 0 in operator[] runtime!");
    // ���� �������� ��� > ������ ���������� ���, �� ���������� ����������
	if (FA_Number > NumberFAs) throw invalid_argument("FA Number is > than number of FAs(" + ExtFunctions::to_string(NumberFAs) + ") in the core in operator[] runtime!");
    // ���� ����� ��� ���, �� ������������ ������ ��������� 
	if (Data.count(FA_Number) == 0) return EmptyState;
	// ����� ���������� ���������
    return Data.at(FA_Number);
}

// ���������� ������ ��������� ���
cState& cDataState::add(int FA_Number)
{
	// ���� �������� ��� <= 0, �� ���������� ����������
	if (FA_Number <= 0) throw invalid_argument("FA Number is <= 0 in cDataState::add runtime!");
	// ���� �������� ��� > ������ ���������� ���, �� ���������� ����������
    if (FA_Number > NumberFAs) throw invalid_argument("FA Number is > than number of FAs(" + ExtFunctions::to_string(NumberFAs) + ") in the core in cDataState::add runtime!");
	// ����� ������� ��������� ��� ��� � ���������� ��� ��� ���������� ��� ���������
	return Data[FA_Number];
}

// ���������� ������ �� �����
void cDataState::ReadDataFromFile(const wstring &FileName)
{
	// ��������� ����, ���� ����� ��� - ���������� ����������
    ifstream InputFile(FileName);
    if(!InputFile.is_open()) throw ios_base::failure("Not possible to open the file! Try to check the correctness of the path.");
    
	// ������������ ����������� ������
	enum Decl {
		None = 0,
		DeclaratedKqBurn = 1,
		DeclaratedKv = 2
	};

	// ������� ������ � ������
	int FAPos = 0, LayerPos = 0, KvPos = 0, KqPos = 0, BurnPos = 0;
	// ������ ��� ���������� ���������� ������
	bool ReadFail = false;
	// ��� ������ ���������
	Decl Declaration = None;
	stringstream ss("");
	string str = "";
	while (!InputFile.eof()) {
		// ���� �� ���� ������ ���������� - ��������� ��������� ������
		if (!ReadFail) getline(InputFile, str);
		// ���� ���� ������ ���������� - ������� ����� � ��������� ������, ������� �� ������� �������
		if (ReadFail) { ss.clear(); getline(ss, str); ReadFail = false;}
		// ���������� ������ ������
		if (str == "") continue;
		// ������� �����, ���� ����� ���������� ������ ��� ������������ ���������� ������
		ss.clear(); ss.str("");
		// ���� ������� ������ �� ���������, �� ���� ������ ������� - ���������
		if (Declaration == None) 
		{
			// ����� ����� ���, � ����� Kq ��� Burn ��� ������ ��������� ����
			if ((str.find("Kq") != string::npos || str.find("Burn") != string::npos ) && str.find("FA") != string::npos && str.find("Layer") == string::npos)
			{
				// ������ ����� ��������� Kq � Burn
				Declaration = DeclaratedKqBurn;
				ss << str;
				// ���������� ������� �������� � Kq, Burn � FA � ������
				FAPos = 0; KqPos = 0; BurnPos = 0;
				int i = 0;
				while(!ss.eof()) {
					str = "";
					ss >> str;
					// ���� ���� ������ - ��� �� ��������� (��������, ������������ #)
					if (str.length() <= 1) continue;
					++i;
					if (str.find("FA") != string::npos) FAPos = i;
					if (str == "Kq") KqPos = i;
					if (str == "Burn") BurnPos = i;
				}
				// ���� �� ����� ������ Kq � Burn - ���������� ������
				if (KqPos == 0 && BurnPos == 0) Declaration = None; 
				continue;
			// ����� ����� ���, � ����� Kv � ����� ��������� ����
			} else if (str.find("Kv") != string::npos && str.find("Layer") != string::npos && str.find("FA") != string::npos) {
				// ������ ����� ��������� Kv
				Declaration = DeclaratedKv;
				ss << str;
				// ���������� ������� �������� � Kv, Layer � FA � ������
				FAPos = 0; LayerPos = 0; KvPos = 0;
				int i = 0;
				while(!ss.eof()) {
					str = "";
					ss >> str;
					// ���� ���� ������ - ��� �� ��������� (��������, ������������ #)
					if (str.length() <= 1) continue;
					++i;
					if (str.find("FA") != string::npos) FAPos = i;
					if (str.find("Layer") != string::npos) LayerPos = i;
					if (str == "Kv") KvPos = i;
				}
				// ���� �� ����� Kv - ���������� ������
				if (KvPos == 0) Declaration = None;
				continue;
			} else {
				continue;
			}
		}
		// ���� ��������� Kq �/��� Burn
		if (Declaration == DeclaratedKqBurn)
		{
			ss << str;
			int FANum = 0;
			double KqValue = -1, BurnValue = -1;
			int i = 0;
			double value;
			// �� ����� ������ ��������� ������ - ���� ������� ������, �� ����� �� ���� ��������
			while (!ss.eof()) {
				// ���������� �������� � ������������ ���������
				if ( isspace( ss.peek() ) || ss.peek() == EOF ) { ss.get(); continue;}
				// ���� ��� �������� �� ����������� - ����� ������, ����� ��������� ���� ���������� Kv - ����� � ������ ����������
				if (!(ss >> value).fail())
				{
					++i;
					if (i == FAPos) FANum = static_cast<int>(value);
					if (i == KqPos) KqValue = value;
					if (i == BurnPos) BurnValue = value;
				} else {
					Declaration = None;
					ReadFail = true;
					break;
				}
			}
			// ���� ��� �� ��� - ��������� � ��������� �������� �����
			if (Declaration == None || FANum > NumberFAs || (KqPos != 0 && KqValue < 0) || (BurnPos != 0 && BurnValue < 0)) continue;
			// ������ Kq � Burn
			if (KqPos != 0) this->add(FANum).SetKq(KqValue);
			if (BurnPos != 0) this->add(FANum).SetBurn(BurnValue);
		}
		// ���� ��������� Kv
		if (Declaration == DeclaratedKv)
		{
			ss << str;
			int FANum = 0, LayerNum = 0;
			double KvValue = -1;
			int i = 0;
			double value;
			// �� ����� ������ ��������� ������ - ���� ������� ������, �� ����� �� ���� ��������
			while (!ss.eof()) {
				// ���������� �������� � ������������ ���������
				if ( isspace( ss.peek() ) || ss.peek() == EOF ) { ss.get(); continue;}
				// ���� ��� �������� �� ����������� - ����� ������, ����� ��������� ���� ���������� Kv - ����� � ������ ����������
				if (!(ss >> value).fail())
				{
					++i;
					if (i == FAPos) FANum = static_cast<int>(value);
					if (i == LayerPos) LayerNum = static_cast<int>(value);
					if (i == KvPos) KvValue = value;
				} else {
					Declaration = None;
					ReadFail = true;
					break;
				}
			}
			// ���� ��� �� ��� - ��������� � ��������� �������� �����
			if (Declaration == None || FANum > NumberFAs || KvValue < 0) continue;
			// ���� ������� ����� ����� ������, ��� ��������� - ����������� ����� �����
			if (LayerNum > cDataState::GetNumLayers()) cDataState::SetNumLayers(LayerNum);
			// ������ Kv
			this->add(FANum).SetKv(LayerNum, KvValue);
		}
	}
	// ��������� ����������� ����
    InputFile.close();
	// ���� ������ ��� - ���������� ����������
	if (Data.empty()) throw invalid_argument("Error during reading file! Bad format! No data was read.");
}

// ��������� ���������
tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2)
{
	// ���������� � �����������
	cDataState ResultState;
	// ������������ �������� Kv
	double maxKv = 0;
	// ��������� ��� ���� ���
	for(int i = 1; i <= cDataState::GetNumberFAs(); i++) 
	{
		// ���������� Kq
		/*
			���� ���� � ����� �� ��������� �������� Kq ��� ������ ��� = NaN,
			�� ��������� ��������� ���� NaN, 
			����� ��������� = ������ ��������� - ������ ���������
		*/
		double Kq1 = 0, Kq2 = 0;
		Kq1 = State1[i].GetKq();
		Kq2 = State2[i].GetKq();
		if (qIsNaN(Kq1) || qIsNaN(Kq2)) {
			ResultState.add(i).SetKq(numeric_limits<double>::quiet_NaN());
		} else {
			ResultState.add(i).SetKq(Kq1-Kq2);
		}

		// ���������� Burn
		/*
			���� ���� � ����� �� ��������� �������� Burn ��� ������ ��� = NaN,
			�� ��������� ��������� ���� NaN, 
			����� ��������� = ������ ��������� - ������ ���������
		*/
		double Burn1 = 0, Burn2 = 0;
		Burn1 = State1[i].GetBurn();
		Burn2 = State2[i].GetBurn();
		if (qIsNaN(Burn1) || qIsNaN(Burn2)) {
			ResultState.add(i).SetBurn(numeric_limits<double>::quiet_NaN());
		} else {
			ResultState.add(i).SetBurn(Burn1-Burn2);
		}

		// ���������� Kv
		/*
			���� ���� � ����� �� ��������� �������� Kv ��� ������ ��� � ���� = NaN,
			�� ��������� ��������� ���� NaN, 
			����� ��������� = ������ ��������� - ������ ���������
			����������� ������� ������������ �������� Kv ����� ���� ���� ���������
		*/
		// ��������� �� ���� ����� � ������ ���
		for (int j = 1; j <= cDataState::GetNumLayers(); j++)
		{
			double Kv1 = 0, Kv2 = 0;
			Kv1 = State1[i].GetKv(j);
			Kv2 = State2[i].GetKv(j);
			if (!qIsNaN(Kv1) && Kv1 > maxKv) maxKv = Kv1; 
			if (!qIsNaN(Kv2) && Kv2 > maxKv) maxKv = Kv2;

			if (qIsNaN(Kv1) || qIsNaN(Kv2)) {
				ResultState.add(i).SetKv(j,numeric_limits<double>::quiet_NaN());
			} else {
				ResultState.add(i).SetKv(j,Kv1-Kv2);
			}
		}
	}
	// ���������� ������ �� ��������������� ��������� � ������������� �������� Kv
	return make_tuple(ResultState, maxKv);
}