// line.cc

#include <algorithm>    // nth_element
#include <chrono>       // high_resolution_clock
#include <iomanip>      // setw
#include <iostream>     // cout
#include <vector>       // vector

int main()
{
	const unsigned LINE = 1 << 10;  // 1024B
	const unsigned SIZE = 1 << 21;  // 2MB
	const unsigned GAP = 12;
	const unsigned REP = 100;

	static_assert(sizeof(unsigned char) == 1, "sizeof(unsigned char) != 1");

	std::vector<unsigned char> bits(SIZE);

	std::cout << "#" 
	          << std::setw(GAP - 1) << "line"
	          << std::setw(GAP    ) << "time"
	          << std::endl;

	for (unsigned line = 1; line <= LINE; line <<= 1)
	{
		using namespace std::chrono;

		std::vector<double> sec(REP);
		for (auto &s: sec)
		{
			auto start = high_resolution_clock::now();
			for(unsigned i=0; i<line; i++){
				for(unsigned j=0; j<SIZE; j+=line){
					bits.at(j)++;
				}
			}
			auto stop = high_resolution_clock::now();
			//s = duration_cast<nanoseconds>(stop - start).count() * line / double(bits.size());
			s = duration_cast<nanoseconds>(stop - start).count() / double(bits.size());
		}

		nth_element(sec.begin(), sec.begin() + sec.size() / 2, sec.end());
		std::cout << std::setw(GAP) << line
		          << std::setw(GAP) << std::fixed << std::setprecision(3)
		          << std::setw(GAP) << sec[sec.size() / 2]
		          << std::endl;
	}
}
