#include <iostream>
#include <string>
#include <fstream>

#include "MarketData.h"
#include "Parser.h"


int main(int argc, char* argv[])
{
	if (argc == 1 || argc > 3) {
		std::cerr << "Usage: " << argv[0] << " <file> [<symbol>]" << std::endl;
		return 1;
	}

	std::string file = argv[1];
	std::ifstream commandsFile(file);
	if (!commandsFile.is_open()) {
		std::cerr << "Unable to open file" << std::endl;
		return 2;
	}

	std::string symbol;
	if (argc == 3) {
		symbol = argv[2];
	}

	MarketData marketData;
	Parser parser(marketData, symbol);
	std::string line;
	while (std::getline(commandsFile, line)) {
		parser.parseCommand(line);
	}
	commandsFile.close();

	return 0;
}
