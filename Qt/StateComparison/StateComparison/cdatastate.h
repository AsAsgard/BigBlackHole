#ifndef CDATASTATE_H
#define CDATASTATE_H

#include <map>

#include "cstate.h"

// ������ ����������� ��������� � �������� ����
class cDataState
{
	// ������
	friend std::tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2);
private:
	// ����
	// ������ ��� ���������� ���
    std::map<int, cState> Data;
	// ������ ���������
	cState EmptyState; 
	// ���������� ���
    static int NumberFAs;

	// ������
	// ���������� �������� � ���� Data
	cState& add(int FA_Number);
    // ��������� ������ �������� ����� �������� ����� � ��
	static void SetNumLayers(int newNumLayers) { cState::NumLayers = newNumLayers;}
public:
	// ������
    // ������������
    cDataState() {}
	cDataState(const cDataState &rState) : Data(rState.Data) {}
	cDataState(cDataState &&rrState) {if (this != &rrState) Data = rrState.Data;}
	// ��������� =
	cDataState& operator=(const cDataState &rState) {Data = rState.Data; return (*this);}
	cDataState& operator=(cDataState &&rrState) {if (this != &rrState) Data = rrState.Data; return (*this);}
    // ���������� ������ �� �����
    void ReadDataFromFile(const std::wstring &FileName);
    // �������� []
    const cState& operator[](int FA_Number) const;
    // ��������� ����� ���
    static int GetNumberFAs() {return NumberFAs;}
    // ��������� ����� �������� �����
    static int GetNumLayers() {return cState::NumLayers;}
	// ������� ����� �����
	static void reduceNumLayers() { SetNumLayers(1);}
	// ��������� ������� ����� �����
	static void rememberNumLayers() { cState::prevNumLayers = cState::NumLayers;}
	// ���������� ����� �����, ������� ����� ����������
	static void setRememberedNumLayers() { SetNumLayers(cState::prevNumLayers);}
};

// �������� ���������
std::tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2);

#endif // CDATASTATE_H
