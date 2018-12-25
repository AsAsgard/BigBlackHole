#ifndef CSTATE_H
#define CSTATE_H

#include <vector>
#include <string>
#include <limits>
#include <tuple>

// ������ ��� ���������� ���
class cState
{
    // ������
    friend class cDataState;
	friend std::tuple<cDataState, double> CompareStates(const cDataState &State1, const cDataState &State2);
private:
    // ����
	// ������
    double Kq;
    double Burn;
    std::vector<double> Kv;
	// NaN
	double quiet_NaN;
    // ���������� ����� � ���
    static int NumLayers;
	// ����� ����� � ���, ������� ���������
	static int prevNumLayers;

    // ������
	// ��������� ������
    void SetKq(const double &newKq) {Kq = newKq;}
    void SetBurn(const double &newBurn) {Burn = newBurn;}
    void SetKv(const std::vector<double> &newKv) {Kv = newKv;}
    void SetKv(int LayerNumber, const double &newKv);
public:
    // ������������
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
    // ��������� ��������
    const double& GetKq() const { return Kq;}
    const double& GetBurn() const { return Burn;}
    const std::vector<double>& GetKv() const { return Kv;}
    const double& GetKv(int LayerNumber) const;
};

#endif // CSTATE_H
