
#include "N.h" 

int main() {

	// create the Network
	N n{ topologie{ 2,3,1 }, learnRate{ 0.9 }, activationMethodchoosen::eins_durch_ehoch, normalization{ 1.0, 0.0, 1.0, 0.0 } };

	std::chrono::high_resolution_clock::time_point timex;
	std::chrono::nanoseconds elapsed;
	// 1 sec == 1 000 000 000 Nanosekunden

	timex = std::chrono::high_resolution_clock::now();
	if (1) // proper sequence for teaching
		for (int it = 0; it < 1000; ++it) {
			n.input[0] = 0.0;
			n.input[1] = 0.0;
			n.trueVal[0] = 0.0;
			n.calc(learn{ true });
			n.input[0] = 0.0;
			n.input[1] = 1.0;
			n.trueVal[0] = 1.0;
			n.calc(learn{ true });
			n.input[0] = 1.0;
			n.input[1] = 0.0;
			n.trueVal[0] = 1.0;
			n.calc(learn{ true });
			n.input[0] = 1.0;
			n.input[1] = 1.0;
			n.trueVal[0] = 0.0;
			n.calc(learn{ true });
		} // for (int it = 0; it < 1000; ++it) {
	elapsed = std::chrono::high_resolution_clock::now() - timex;


	if (1) { // check if the network really learned to solve its task
		cout << "Ergebnis:" << endl;
		n.input[0] = 0.0;
		n.input[1] = 0.0;
		n.calc(learn{ false });
		cout << n.input[0] << n.input[1] << " -> " << n.output[0] << endl; 
		n.input[0] = 0.0;
		n.input[1] = 1.0;
		n.calc(learn{ false });
		cout << n.input[0] << n.input[1] << " -> " << n.output[0] << endl; 
		n.input[0] = 1.0;
		n.input[1] = 0.0;
		n.calc(learn{ false });
		cout << n.input[0] << n.input[1] << " -> "  << n.output[0] << endl; 
		n.input[0] = 1.0;
		n.input[1] = 1.0;
		n.calc(learn{ false });
		cout << n.input[0] << n.input[1] << " -> " << n.output[0] << endl; ;
	} // if (1) { // check if the network really learned to solve its task

	std::cout << "Elapsed time in seconds = " << elapsed.count() / 1000000000.0 << std::endl;

	getchar();
	return 0;
}
