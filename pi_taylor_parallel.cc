#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using my_float = long double;

void
pi_taylor_chunk(std::vector<my_float> &output,
        size_t thread_id, size_t start_step, size_t stop_step) {
	int sign = (start_step % 2 == 0 ) ? 1 : -1;
	my_float sum = 0;
	for(size_t i = start_step; i < stop_step; i++) {
		my_float float_denom = static_cast<my_float>(2*i + 1);
		sum = sum + sign/float_denom;
		sign = -sign;
	}	
	output[thread_id] = sum;
}

std::pair<size_t, size_t>
usage(int argc, const char *argv[]) {
    // read the number of steps from the command line
    if (argc != 3) {
        std::cerr << "Invalid syntax: pi_taylor <steps> <threads>" << std::endl;
        exit(1);
    }

    size_t steps = std::stoll(argv[1]);
    size_t threads = std::stoll(argv[2]);

    if (steps < threads ){
        std::cerr << "The number of steps should be larger than the number of threads" << std::endl;
        exit(1);

    }
    return std::make_pair(steps, threads);
}

int main(int argc, const char *argv[]) {


    auto ret_pair = usage(argc, argv);
    auto steps = ret_pair.first;
    auto threads = ret_pair.second;
    
    std::vector<std::thread> thread_vector;
    std::vector<my_float> results_vector(threads, 0.0);
    size_t n_start = 0;
    size_t n_diff = 0;
    
    for(size_t i = 0; i < threads; i++){
		if(i < steps % threads){ //start - stop = steps/THREADS + 1
			//n_start = i * (steps / threads+1);
			n_diff = steps / threads + 1;
		}else{ // start - stop = steps/threads 
			//n_start = i * steps / threads - 1;
			n_diff = steps / threads;
		}
		
		thread_vector.push_back(std::thread(pi_taylor_chunk, 
								std::ref(results_vector),
								i,n_start, n_start + n_diff));
		n_start = n_start + n_diff;
	}
	
	my_float pi = 0;

	for(size_t i = 0; i < threads; i++){
		thread_vector[i].join();
		pi = pi + results_vector[i];
	}
	pi = pi*4;
	


    std::cout << "For " << steps << ", pi value: "
        << std::setprecision(std::numeric_limits<my_float>::digits10 + 1)
        << pi << std::endl;
}

