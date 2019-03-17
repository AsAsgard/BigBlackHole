#ifndef CSTATE_H
#define CSTATE_H

/*
 * class cState
 *
 * Version 1.3
 *
 * Writed by Brylkin Dmitry. 28.11.2018
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <vector>
#include <string>
#include <limits>
#include <tuple>

class cDataState;

/*
 * Класс cState
 *
 * Используется для хранения данных состояния конкретной ТВС в активной зоне
 *
 * Хранимые данные: Kq, Burn, Kv (по слоям)
 * Данные хранятся для всех слоев, реализованы методы работы с данными
 */
class cState
{
    // друзья
    friend class cDataState;
	friend std::tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2);
private:
    // ПОЛЯ
	// данные
    double Kq;
    double Burn;
    std::vector<double> Kv;
	// NaN
	double quiet_NaN;
    // количество слоев в ТВС
    static int NumLayers;
	// число слоев в ТВС, которое запомнили
	static int prevNumLayers;

    // МЕТОДЫ
	// изменение данных
    void SetKq(const double &newKq) {Kq = newKq;}
    void SetBurn(const double &newBurn) {Burn = newBurn;}
    void SetKv(const std::vector<double> &newKv) {Kv = newKv;}
    void SetKv(int LayerNumber, const double &newKv);
public:
    // конструкторы
    cState() 
        : Kq(std::numeric_limits<double>::quiet_NaN())
        , Burn(std::numeric_limits<double>::quiet_NaN())
        , quiet_NaN(std::numeric_limits<double>::quiet_NaN()) {}

    cState(const double &newKq,
           const double &newBurn,
           const std::vector<double> &newKv) 
		   : Kq(newKq)
		   , Burn(newBurn)
		   , Kv(newKv)
		   , quiet_NaN(std::numeric_limits<double>::quiet_NaN()) {}
    
	explicit cState(const std::vector<double> &newKv) 
        : Kq(std::numeric_limits<double>::quiet_NaN())
        , Burn(std::numeric_limits<double>::quiet_NaN())
        , Kv(newKv)
        , quiet_NaN(std::numeric_limits<double>::quiet_NaN()) {}
    
	cState(const cState &newState) 
        : Kq(newState.Kq)
        , Burn(newState.Burn)
        , Kv(newState.Kv)
        , quiet_NaN(std::numeric_limits<double>::quiet_NaN()) {}
    // получение значений
    const double& GetKq() const { return Kq;}
    const double& GetBurn() const { return Burn;}
    const std::vector<double>& GetKv() const { return Kv;}
    const double& GetKv(int LayerNumber) const;
};

#endif // CSTATE_H
