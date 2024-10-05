#include <cxxabi.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cerr << "usage: demangle [file] [build output]"
			  << std::endl;
		return 1;
	}
	std::string file = argv[1];
	std::ifstream infile(file);
	std::string line;
	std::ostringstream obuf;

	while (std::getline(infile, line)) {
		int status = 0;
		std::size_t start_idx = 0;
		int spaces = 0;
		for (char c : line) {
			start_idx++;
			if (c != ' ')
				continue;
			if (++spaces == 2) {
				break;
			}
		}
		auto symbol = line.substr(start_idx);
		obuf << line.substr(0, start_idx);
		if (symbol.at(0) != '_' || symbol.at(1) != 'Z') {
			obuf << symbol;
		} else {
			obuf << abi::__cxa_demangle(symbol.c_str(), nullptr,
						    nullptr, &status);
		}
		obuf << '\n';
	}

	infile.close();
	std::ofstream outfile(file);
	auto res = obuf.str();
	outfile << res.substr(0, res.length() - 1) << std::endl;
	outfile.close();

	std::string buildout = argv[2];
	std::ofstream buildoutf(buildout);
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	buildoutf << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << std::endl;

	return 0;
}
