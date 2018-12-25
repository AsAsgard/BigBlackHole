#ifndef CDATASTATE_H
#define CDATASTATE_H

#include <map>

#include "cstate.h"

// данные конкретного состояния в активной зоне
class cDataState
{
	// друзья
	friend std::tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2);
private:
	// ПОЛЯ
	// данные для конкретной ТВС
    std::map<int, cState> Data;
	// пустое состояние
	cState EmptyState; 
	// количество ТВС
    static int NumberFAs;

	// МЕТОДЫ
	// добавление элемента в поле Data
	cState& add(int FA_Number);
    // установка нового значения числа высотных слоев в АЗ
	static void SetNumLayers(int newNumLayers) { cState::NumLayers = newNumLayers;}
public:
	// МЕТОДЫ
    // конструкторы
    cDataState() {}
	cDataState(const cDataState &rState) : Data(rState.Data) {}
	cDataState(cDataState &&rrState) {if (this != &rrState) Data = rrState.Data;}
	// операторы =
	cDataState& operator=(const cDataState &rState) {Data = rState.Data; return (*this);}
	cDataState& operator=(cDataState &&rrState) {if (this != &rrState) Data = rrState.Data; return (*this);}
    // считывание данных из файла
    void ReadDataFromFile(const std::wstring &FileName);
    // оператор []
    const cState& operator[](int FA_Number) const;
    // получение числа ТВС
    static int GetNumberFAs() {return NumberFAs;}
    // получение числа высотных слоев
    static int GetNumLayers() {return cState::NumLayers;}
	// удалить число слоев
	static void reduceNumLayers() { SetNumLayers(1);}
	// запомнить текущее число слоев
	static void rememberNumLayers() { cState::prevNumLayers = cState::NumLayers;}
	// установить число слоев, которое ранее запоминали
	static void setRememberedNumLayers() { SetNumLayers(cState::prevNumLayers);}
};

// сравнить состояния
std::tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2);

#endif // CDATASTATE_H
