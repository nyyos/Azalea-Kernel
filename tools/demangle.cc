#include <cxxabi.h>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "usage: demangle [symbol]" << std::endl;
		return 1;
	}

	std::string sym = argv[1];
	int status;
	if (sym.at(0) != '_' || sym.at(1) != 'Z') {
		std::cout << sym;
	} else {
		std::cout << abi::__cxa_demangle(sym.c_str(), nullptr, nullptr,
						 &status);
	}

	return 0;
}
