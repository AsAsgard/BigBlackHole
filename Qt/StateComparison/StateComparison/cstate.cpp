#include <stdexcept>

#include "cstate.h"
#include "extfunctions.h"

using namespace std;

// ��������� ��������� �������� �������� � ��������������� �������� ����� ����� � ���
int cState::NumLayers = 1;
int cState::prevNumLayers = 1;

// ��������� �������� Kv �� ������ ����
const double& cState::GetKv(int LayerNumber) const
{
	// ���� ����� ���� ������ ��� ����� 0, �� ������������ ����������
    if (LayerNumber <= 0) throw invalid_argument("Layer Number is <= 0 in method GetKv!");
    // ���� ����� ���� ������ �������������, �� ������������ ����������
	if (LayerNumber > NumLayers) throw invalid_argument("Layer Number is > than maximum layer number " + ExtFunctions::to_string(NumLayers) +" in method GetKv!");
	// ���� ����� ���� ������ ������� ������� � ������� �� Kv ��� ������ ���, �� �� ������ ������������� ��������, ������������ NaN
	if (Kv.size() < LayerNumber) return quiet_NaN;
	// ���� �������� ������������ - ������������ ���
    return Kv[LayerNumber-1];
}

void cState::SetKv(int LayerNumber, const double &newKv)
{
	// ���� ����� ���� ������ ��� ����� 0, �� ������������ ����������
    if (LayerNumber <= 0) throw invalid_argument("Layer Number is <= 0 in method SetKv!");
	// ���� ����� ���� ������ �������������, �� ������������ ����������
    if (LayerNumber > NumLayers) throw invalid_argument("Layer Number is > than maximum layer number " + ExtFunctions::to_string(NumLayers) +" in method SetKv!");
	// ���� ���� ����� ���� ������ ������� ������� � ������� �� Kv ��� ������ ���, �� �� ������ ������������� ��������, �� ������������ ���������� ������� ������� � ����������� ����� ����� � ������� NaN
    if (Kv.size() < NumLayers) Kv.resize(NumLayers,numeric_limits<double>::quiet_NaN());
    // ������ ���������� ����� ���������
	Kv[LayerNumber-1] = newKv;
}
