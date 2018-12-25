#include <stdexcept>

#include "cstate.h"
#include "extfunctions.h"

using namespace std;

// установка начальных значений текущего и дополнительного значений числа слоев в ТВС
int cState::NumLayers = 1;
int cState::prevNumLayers = 1;

// получение значения Kv по номеру слоя
const double& cState::GetKv(int LayerNumber) const
{
	// если номер слоя меньше или равен 0, то выкидывается исключение
    if (LayerNumber <= 0) throw invalid_argument("Layer Number is <= 0 in method GetKv!");
    // если номер слоя больше максимального, то выкидывается исключение
	if (LayerNumber > NumLayers) throw invalid_argument("Layer Number is > than maximum layer number " + ExtFunctions::to_string(NumLayers) +" in method GetKv!");
	// если номер слоя больше размера вектора с данными по Kv для данной ТВС, но не больше максимального значения, возвращается NaN
	if (Kv.size() < LayerNumber) return quiet_NaN;
	// если значение присутствует - отправляется оно
    return Kv[LayerNumber-1];
}

void cState::SetKv(int LayerNumber, const double &newKv)
{
	// если номер слоя меньше или равен 0, то выкидывается исключение
    if (LayerNumber <= 0) throw invalid_argument("Layer Number is <= 0 in method SetKv!");
	// если номер слоя больше максимального, то выкидывается исключение
    if (LayerNumber > NumLayers) throw invalid_argument("Layer Number is > than maximum layer number " + ExtFunctions::to_string(NumLayers) +" in method SetKv!");
	// если если номер слоя больше размера вектора с данными по Kv для данной ТВС, но не больше максимального значения, то производится увеличение размера вектора с заполнением новых ячеек с помощью NaN
    if (Kv.size() < NumLayers) Kv.resize(NumLayers,numeric_limits<double>::quiet_NaN());
    // ячейка заполяется новым значением
	Kv[LayerNumber-1] = newKv;
}
