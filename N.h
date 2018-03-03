#pragma once

#include <iostream>
#include <vector>
#include <initializer_list>
#include <cmath>
#include <random>
#include <functional>
#include <chrono>
#include <tuple>
#include <memory> // for unique_pointer
#include <utility> // for std::move of unique pointer

using namespace std;

using topologie = std::initializer_list<int>;

enum class activationMethodchoosen { tanh_sigmoid, eins_durch_ehoch, ReLU, no_formula }; // Attention, it always needs an activation method, 

inline void eins_durch_ehoch(double * p_val);
inline double derivative_eins_durch_ehoch(double * p_val);

inline void ReLU(double * p_val);
inline double derivative_ReLU(double * p_val);

using normalization = std::tuple<double, double, double, double>;
using randomInit = std::tuple<double, double>;

using learnRate = double;
using learn = bool;

class N {
public:

	N(std::initializer_list<int>& t, double LearnRate = 0.9, activationMethodchoosen act_method_received = activationMethodchoosen::eins_durch_ehoch, normalization nP = { 1.0, 0.0, 1.0, 0.0 }, randomInit in = { -1.0, 1.0 });
	double * input;
	double * trueVal; 
	void calc(bool learn);
	double * output;

private:
	std::vector<int> top;
	double LearnRate;
	activationMethodchoosen act_method;

	double A_max, A_min, new_A_max, new_A_min;
	void   norm  (double& p_v_orig); // Normalization function
	double denorm(double& p_v_norm); // Denormalization function

	void(*p_activationfunction)(double * val);
	double(*p_slope)(double * val);

	double ** nod;
	double ** err;
	double *** wij;
	double * den; // denormalized result of calc, only returned on demand
	int Nlay;
	int Nnod;
	int Nwij;
	double* getCalcRes();

public:
	~N();

};

