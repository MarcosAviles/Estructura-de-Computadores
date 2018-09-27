// size.cc

#include <algorithm>    // nth_element
#include <chrono>       // high_resolution_clock
#include <iomanip>      // setw
#include <iostream>     // cout
#include <vector>       // vector

int main()
{
	const unsigned STEPS = 100000000;
	const unsigned GAP = 12;
	const unsigned REP = 50;

	std::cout << "#" 
	          << std::setw(GAP - 1) << "size"
	          << std::setw(GAP    ) << "time"
	          << std::endl;

	for (unsigned size = 1 << 10; size <= 1 << 26; size <<= 1)
	{
		using namespace std::chrono;

		unsigned size_1 = size - 1;

		std::vector<unsigned char> bits(size);

		std::vector<double> sec(REP);
		for (auto &s: sec)
		{
			auto start = high_resolution_clock::now();
			for (unsigned i=0; i<STEPS; i+=64){
				bits.at(i & size_1)++;
			}
			auto stop = high_resolution_clock::now();
			s = duration_cast<nanoseconds>(stop - start).count() / double(STEPS);
		}

		nth_element(sec.begin(), sec.begin() + sec.size() / 2, sec.end());
		std::cout << std::setw(GAP) << size
		          << std::setw(GAP) << std::fixed << std::setprecision(3)
		          << sec[sec.size() / 2]
		          << std::endl;
	}
}
