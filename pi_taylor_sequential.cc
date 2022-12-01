#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>

// Allow to change the floating point type
using my_float = long double;

my_float pi_taylor(size_t steps) {
	int sign = 1;
	my_float sum = 0;
	
	for(size_t i = 0; i < steps; i++){
		my_float float_denom = static_cast<my_float>(2*i + 1);
		sum = sum + sign/float_denom;
		sign = -sign;
	}
	
    return 4*sum;
}

int main(int argc, const char *argv[]) {

    // read the number of steps from the command line
    if (argc != 2) {
        std::cerr << "Invalid syntax: pi_taylor <steps>" << std::endl;
        exit(1);

    }

    size_t steps = std::stoll(argv[1]);
    auto start = std::chrono::steady_clock::now();
    // Start pi calculation
    auto pi = pi_taylor(steps);
	
	auto stop = std::chrono::steady_clock::now();
	auto time_diff = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
	std::cout << "Time in us: " << time_diff.count() << std::endl;


    std::cout << "For " << steps << ", pi value: "
        << std::setprecision(std::numeric_limits<my_float>::digits10 + 1)
        << pi << std::endl;
}
