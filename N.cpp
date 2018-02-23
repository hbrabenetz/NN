
#include "N.h"

void eins_durch_ehoch(double * p_val) {

	// output = 1 / (1+e^(-Înput))
	// gibt 0 bis +1 

	*p_val = 1.0 / (1 + pow(2.718, -1.0 * *p_val));

	return; //  0.0; //  *d_val;
}


void N::norm(double& p_v_orig) { //, double& A_max, double& A_min, double& new_A_max, double& new_A_min) {
	p_v_orig = (p_v_orig - A_min) * (new_A_max - new_A_min) / (A_max - A_min) + new_A_min;
}


double N::denorm(double& p_v_norm) { //, double& A_max, double& A_min, double& new_A_max, double& new_A_min) {
	return (p_v_norm - new_A_min) * (A_max - A_min) / (new_A_max - new_A_min) + A_min;
}


N::N(std::initializer_list<int>& topol, double LearnRate, activationMethodchoosen act_method_received, std::tuple<double, double, double, double> normParam):
	top{ topol }, LearnRate{ LearnRate }, act_method{ act_method_received }, normalizationParam{ normParam }
{

	A_max = get<0>(normalizationParam);
	A_min = get<1>(normalizationParam);
	new_A_max = get<2>(normalizationParam);
	new_A_min = get<3>(normalizationParam);

	if (act_method == activationMethodchoosen::eins_durch_ehoch)
		p_activationfunction = eins_durch_ehoch;

	Nnod = 0;
	using std::cout;
	cout << "topologie ";
	for (auto e : top) {
		cout << e << " ";

		Nnod += e;

	}
	cout << endl;

	Nlay = static_cast<int> (top.size()); // (int)
	cout << "Nlayer = " << Nlay << endl;

	if (Nlay < 3) {
		cout << "A neuronal network must have at least three layer including input and output layer. Programm will be terminated." << endl;
		exit(0);
	}

	nod = new double*[Nlay];
	err = new double*[Nlay];
	for (int nlay = 0; nlay < Nlay; ++nlay) {
		nod[nlay] = new double[top[nlay] + 1]; // +1 is for D which is always 1.0
		err[nlay] = new double[top[nlay]];
	}

	Nwij = 0;
	wij = new double **[Nlay - 1];
	for (int nlay = 0; nlay < Nlay - 1; ++nlay) // last layer needs no wij's
		wij[nlay] = new double*[top[nlay] + 1]; // +1 is for D which is always 1.0

	for (int nlay = 0; nlay < Nlay - 1; ++nlay)
		for (int i = 0; i < top[nlay] + 1; ++i) {
				wij[nlay][i] = new double[top[nlay + 1]];
				Nwij += top[nlay + 1];
			}

	cout << "Nnod = " << Nnod << endl;
	cout << "Nwij = " << Nwij << endl;

	auto random_d = std::bind(std::uniform_real_distribution<double>(-1.0, 1.0), std::default_random_engine{});

	for (int nlay = 0; nlay < Nlay - 1; ++nlay)
		for (int i = 0; i < top[nlay] + 1; ++i)
			for (int j = 0; j < top[nlay + 1]; ++j)
				wij[nlay][i][j] = random_d();

	/** the fictive d nodes have to be set to 1.0 */
	for (int nlay = 0; nlay < Nlay; ++nlay)
		nod[nlay][top[nlay]] = 1.0; // +1 is for D which is always 1.0

	/** Set the input nodes to 0 so it does not crash if it will be forgotten by the user */
	for (int i = 0; i < top[0]; ++i)
		nod[0][i] = 0.0;

	/* Vektor for true Values */
	tru = new double[top[Nlay - 1]];
	for (int i = 0; i < top[Nlay - 1]; ++i) // lets initialize them just to avoid breakdowns
		tru[i] = 0.0;

	den = new double[top[Nlay - 1]];
	for (int i = 0; i < top[Nlay - 1]; ++i)
		den[i] = 0.0;

	cout << "Neuronal Network is up and ready" << endl;

}


double* N::getCalcRes() {

	for (int i = 0; i < top[Nlay - 1]; ++i)
		den[i] = denorm(nod[Nlay - 1][i]);

	return den;
}


void N::calc() {

	//
	// Forward calculation
	//

	// 
	// here I normalize the input layer
	//

	for (int n = 0; n < top[0]; ++n)
		norm(nod[0][n]);

	/** Starts with layer 1 since layer 0 needs input but no calculation */
	for (int nlay = 1; nlay < Nlay; ++nlay)
		for (int n = 0; n < top[nlay]; ++n) {

			nod[nlay][n] = 0.0;

			/** we do <= because we want to include the fictive d node which is always 1.0 */
			for (int nprev = 0; nprev <= top[nlay - 1]; ++nprev)
				nod[nlay][n] += nod[nlay - 1][nprev] *
				wij[nlay - 1][nprev][n];

			(*p_activationfunction)((double *)(&(nod[nlay][n])));

		}

	// hier könnte man die funktion verlassen falls man nicht lernen will zb
	// durch einen boolschen parameter learn as true or false

	//
	// Backpropagation
	//

	// A good description of neural networks can be found here
	// http://www3.cs.stonybrook.edu/~cse634/ch6NN.pdf
	
	//
	// Backpropagation Algorithm
	//
	// 0    0    0       Errk = Ok * (1 - Ok) * (Tk - Ok) ....Error for the Output Nodes k
	//   \  |  /         We go back now Layer by Layer and calculate for each node its error
	//      0            Erri = Oi * (1 - Oi) * SUM Errk*wik    Errj = Erri for the next step backwards
	//    \ | /
	//      0            Erri = Oi * (1 - Oi) * SUM Errj*wij
	//
	//                   then we calculate all new wij and d's which can be done even forward:
	//                   wij = wij + learnRate * Errj * Oi
	//                   Dj  = Dj  + learnRate * Errj * 1.0 (better seen as a fitive additional Node with const value 1.0
	//                -> Dij = Dij + learnRate * Errj * 1.0 where D is another Node on top of the i Nodes with const value 1.0 and so can have a normal wij             
	//					
	//                   always assuming 
	//                   we calculate Ij = SUM wij * Oi + Dj and
	//                   we squash with Oj = 1/(1+Exp(-Ij))                   
	//

	// Output layer errors, Errk = Ok * (1 - Ok) * (Tk - Ok) ....Error for the Output Nodes 
	for (int n = 0; n < top[Nlay - 1]; ++n)
		err[Nlay - 1][n] =  nod[Nlay - 1][n] *
							(1 - nod[Nlay - 1][n]) *
							(tru[n] - nod[Nlay - 1][n]);

	// Hidden layer errors, Erri = Oi * (1 - Oi) * SUM Errk*wik 
	for (int nlay = Nlay - 2; nlay > 0; --nlay)
		for (int n = 0; n < top[nlay]; ++n) {

			err[nlay][n] = 0.0;

			for (int nnext = 0; nnext < top[nlay + 1]; ++nnext)
				err[nlay][n] +=
				err[nlay + 1][nnext] *
				wij[nlay][n][nnext];

			err[nlay][n] *= nod[nlay][n] * (1 - nod[nlay][n]);
		}

	// wij's und d's ändern, 
	// wij = wij + learnRate * Errj * Oi
	// Dj  = Dj  + learnRate * Errj * 1.0, -> Dij = Dij + learnRate * Errj * 1.0 
	for (int nlay = 1; nlay < Nlay; ++nlay)
		for (int n = 0; n < top[nlay]; ++n) {

			/* we do <= because we want to include the fictive d node which is always 1.0 */
			for (int nprev = 0; nprev <= top[nlay - 1]; ++nprev)
				wij[nlay - 1][nprev][n] += LearnRate *
				nod[nlay - 1][nprev] *
				err[nlay][n];

		}

	return; // 0; // denorm(nod[Nlay - 1][0]); // könnte auch tuple returnen usw
}


N::~N() {

	// to be done but not necessary in our specific use

}
