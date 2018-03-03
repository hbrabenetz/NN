
#include "N.h" 

int main() {

	// create the Network with a unique smart pointer
	std::unique_ptr<N> p = std::make_unique<N>(topologie{ 2,3,1 }, learnRate{ 0.09 }, 
						   activationMethodchoosen::ReLU, normalization{ 1.0, 0.0, 1.0, 0.0 }, randomInit{ 0.0, 1.0 }); 

	std::chrono::high_resolution_clock::time_point timex;
	std::chrono::nanoseconds elapsed;
	// 1 sec == 1 000 000 000 Nanosekunden

	timex = std::chrono::high_resolution_clock::now();
	if (1) // proper sequence for teaching
		for (int it = 0; it < 1000; ++it) {
			p->input[0] = 0.0;
			p->input[1] = 0.0;
			p->trueVal[0] = 0.0;
			p->calc(learn{ true });
			p->input[0] = 0.0;
			p->input[1] = 1.0;
			p->trueVal[0] = 1.0;
			p->calc(learn{ true });
			p->input[0] = 1.0;
			p->input[1] = 0.0;
			p->trueVal[0] = 1.0;
			p->calc(learn{ true });
			p->input[0] = 1.0;
			p->input[1] = 1.0;
			p->trueVal[0] = 0.0;
			p->calc(learn{ true });
		} // for (int it = 0; it < 1000; ++it) {
	elapsed = std::chrono::high_resolution_clock::now() - timex;

	if (1) { // check if the network really learned to solve its task
		cout << "Ergebnis:" << endl;
		p->input[0] = 0.0;
		p->input[1] = 0.0;
		p->calc(learn{ false });
		cout << p->input[0] << p->input[1] << " -> " << p->output[0] << endl; 
		p->input[0] = 0.0;
		p->input[1] = 1.0;
		p->calc(learn{ false });
		cout << p->input[0] << p->input[1] << " -> " << p->output[0] << endl; 
		p->input[0] = 1.0;
		p->input[1] = 0.0;
		p->calc(learn{ false });
		cout << p->input[0] << p->input[1] << " -> "  << p->output[0] << endl; 
		p->input[0] = 1.0;
		p->input[1] = 1.0;
		p->calc(learn{ false });
		cout << p->input[0] << p->input[1] << " -> " << p->output[0] << endl; ;
	} // if (1) { // check if the network really learned to solve its task

	std::cout << "Elapsed time in seconds = " << elapsed.count() / 1000000000.0 << std::endl;

	getchar();
	return 0;
}
