#include <fstream>

#include "cdatastate.h"
#include "extfunctions.h"

using namespace std;

// установка значения количества ТВС в активной зоне
int cDataState::NumberFAs = 163;

// оператор обращения к состоянию конкретной ТВС 
const cState& cDataState::operator[](int FA_Number) const
{
	// если значение ТВС <= 0, то выкидываем исключение
    if (FA_Number <= 0) throw invalid_argument("FA Number is <= 0 in operator[] runtime!");
    // если значение ТВС > общего количества ТВС, то выкидываем исключение
	if (FA_Number > NumberFAs) throw invalid_argument("FA Number is > than number of FAs(" + ExtFunctions::to_string(NumberFAs) + ") in the core in operator[] runtime!");
    // если такой ТВС нет, то возвращается пустое состояние 
	if (Data.count(FA_Number) == 0) return EmptyState;
	// иначе возвращаем состояние
    return Data.at(FA_Number);
}

// добавление нового состояния ТВС
cState& cDataState::add(int FA_Number)
{
	// если значение ТВС <= 0, то выкидываем исключение
	if (FA_Number <= 0) throw invalid_argument("FA Number is <= 0 in cDataState::add runtime!");
	// если значение ТВС > общего количества ТВС, то выкидываем исключение
    if (FA_Number > NumberFAs) throw invalid_argument("FA Number is > than number of FAs(" + ExtFunctions::to_string(NumberFAs) + ") in the core in cDataState::add runtime!");
	// иначе создаем состояние для ТВС и возвращаем его или возвращаем уже имеющееся
	return Data[FA_Number];
}

// считывание данных из файла
void cDataState::ReadDataFromFile(const wstring &FileName)
{
	// открываем файл, если файла нет - выкидываем исключение
    ifstream InputFile(FileName);
    if(!InputFile.is_open()) throw ios_base::failure("Not possible to open the file! Try to check the correctness of the path.");
    
	// перечисление считываемых данных
	enum Decl {
		None = 0,
		DeclaratedKqBurn = 1,
		DeclaratedKv = 2
	};

	// позиции данных в строке
	int FAPos = 0, LayerPos = 0, KvPos = 0, KqPos = 0, BurnPos = 0;
	// ошибка при считывании конкретной строки
	bool ReadFail = false;
	// что сейчас считываем
	Decl Declaration = None;
	stringstream ss("");
	string str = "";
	while (!InputFile.eof()) {
		// если не было ошибки считывания - считываем следующую строку
		if (!ReadFail) getline(InputFile, str);
		// если была ошибка считывания - очищаем поток и считываем строку, которую не удалось считать
		if (ReadFail) { ss.clear(); getline(ss, str); ReadFail = false;}
		// пропускаем пустую строку
		if (str == "") continue;
		// очищаем поток, куда будем записывать строку для последующего считывания данных
		ss.clear(); ss.str("");
		// если никакие данные не считываем, то ищем нужные столбцы - заголовки
		if (Declaration == None) 
		{
			// нашли номер ТВС, а также Kq или Burn без номера высотного слоя
			if ((str.find("Kq") != string::npos || str.find("Burn") != string::npos ) && str.find("FA") != string::npos && str.find("Layer") == string::npos)
			{
				// теперь будем считывать Kq и Burn
				Declaration = DeclaratedKqBurn;
				ss << str;
				// определяем позиции столбцов с Kq, Burn и FA в строке
				FAPos = 0; KqPos = 0; BurnPos = 0;
				int i = 0;
				while(!ss.eof()) {
					str = "";
					ss >> str;
					// если один символ - его не учитываем (например, предваряющая #)
					if (str.length() <= 1) continue;
					++i;
					if (str.find("FA") != string::npos) FAPos = i;
					if (str == "Kq") KqPos = i;
					if (str == "Burn") BurnPos = i;
				}
				// если не нашли именно Kq и Burn - продолжаем искать
				if (KqPos == 0 && BurnPos == 0) Declaration = None; 
				continue;
			// нашли номер ТВС, а также Kv и номер высотного слоя
			} else if (str.find("Kv") != string::npos && str.find("Layer") != string::npos && str.find("FA") != string::npos) {
				// теперь будем считывать Kv
				Declaration = DeclaratedKv;
				ss << str;
				// определяем позиции столбцов с Kv, Layer и FA в строке
				FAPos = 0; LayerPos = 0; KvPos = 0;
				int i = 0;
				while(!ss.eof()) {
					str = "";
					ss >> str;
					// если один символ - его не учитываем (например, предваряющая #)
					if (str.length() <= 1) continue;
					++i;
					if (str.find("FA") != string::npos) FAPos = i;
					if (str.find("Layer") != string::npos) LayerPos = i;
					if (str == "Kv") KvPos = i;
				}
				// если не нашли Kv - продолжаем искать
				if (KvPos == 0) Declaration = None;
				continue;
			} else {
				continue;
			}
		}
		// если считываем Kq и/или Burn
		if (Declaration == DeclaratedKqBurn)
		{
			ss << str;
			int FANum = 0;
			double KqValue = -1, BurnValue = -1;
			int i = 0;
			double value;
			// до конца строки считываем данные - если столбец нужный, то берем из него значение
			while (!ss.eof()) {
				// устранение проблемы с завершающими пробелами
				if ( isspace( ss.peek() ) || ss.peek() == EOF ) { ss.get(); continue;}
				// если нет проблемы со считыванием - берем данные, иначе завершаем цикл считывания Kv - снова в поиске заголовков
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
			// если что не так - переходим к следующей итерации цикла
			if (Declaration == None || FANum > NumberFAs || (KqPos != 0 && KqValue < 0) || (BurnPos != 0 && BurnValue < 0)) continue;
			// задаем Kq и Burn
			if (KqPos != 0) this->add(FANum).SetKq(KqValue);
			if (BurnPos != 0) this->add(FANum).SetBurn(BurnValue);
		}
		// если считываем Kv
		if (Declaration == DeclaratedKv)
		{
			ss << str;
			int FANum = 0, LayerNum = 0;
			double KvValue = -1;
			int i = 0;
			double value;
			// до конца строки считываем данные - если столбец нужный, то берем из него значение
			while (!ss.eof()) {
				// устранение проблемы с завершающими пробелами
				if ( isspace( ss.peek() ) || ss.peek() == EOF ) { ss.get(); continue;}
				// если нет проблемы со считыванием - берем данные, иначе завершаем цикл считывания Kv - снова в поиске заголовков
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
			// если что не так - переходим к следующей итерации цикла
			if (Declaration == None || FANum > NumberFAs || KvValue < 0) continue;
			// если текущее число слоев меньше, чем считанное - увеличиваем число слоев
			if (LayerNum > cDataState::GetNumLayers()) cDataState::SetNumLayers(LayerNum);
			// задаем Kv
			this->add(FANum).SetKv(LayerNum, KvValue);
		}
	}
	// закрываем считываемый файл
    InputFile.close();
	// если данных нет - выкидываем исключение
	if (Data.empty()) throw invalid_argument("Error during reading file! Bad format! No data was read.");
}

// сравнение состояний
tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2)
{
	// переменная с результатом
	cDataState ResultState;
	// максимальное значение Kv
	double maxKv = 0;
	// сравнение для всех ТВС
	for(int i = 1; i <= cDataState::GetNumberFAs(); i++) 
	{
		// сравниваем Kq
		/*
			Если хоть в одном из состояний значение Kq для данной ТВС = NaN,
			то результат сравнения тоже NaN, 
			иначе результат = Первое состояние - Второе состояние
		*/
		double Kq1 = 0, Kq2 = 0;
		Kq1 = State1[i].GetKq();
		Kq2 = State2[i].GetKq();
		if (qIsNaN(Kq1) || qIsNaN(Kq2)) {
			ResultState.add(i).SetKq(numeric_limits<double>::quiet_NaN());
		} else {
			ResultState.add(i).SetKq(Kq1-Kq2);
		}

		// сравниваем Burn
		/*
			Если хоть в одном из состояний значение Burn для данной ТВС = NaN,
			то результат сравнения тоже NaN, 
			иначе результат = Первое состояние - Второе состояние
		*/
		double Burn1 = 0, Burn2 = 0;
		Burn1 = State1[i].GetBurn();
		Burn2 = State2[i].GetBurn();
		if (qIsNaN(Burn1) || qIsNaN(Burn2)) {
			ResultState.add(i).SetBurn(numeric_limits<double>::quiet_NaN());
		} else {
			ResultState.add(i).SetBurn(Burn1-Burn2);
		}

		// сравниваем Kv
		/*
			Если хоть в одном из состояний значение Kv для данной ТВС и слоя = NaN,
			то результат сравнения тоже NaN, 
			иначе результат = Первое состояние - Второе состояние
			Параллельно находим максимальное значение Kv среди этих двух состояний
		*/
		// пробегаем по всем слоям в данной ТВС
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
	// возвращаем кортеж из результирующего состояния и максимального значения Kv
	return make_tuple(ResultState, maxKv);
}