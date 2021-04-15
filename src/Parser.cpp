#include "Parser.h"


Parser::Parser(MarketData& marketData, const std::string& symbolFilter)
	: symbolFilter { symbolFilter }, marketData{ marketData }
{
	if (!symbolFilter.empty()) {
		isOutputFiltered = true;
	}
}

bool Parser::getIsOutputFiltered() const
{
	return isOutputFiltered;
}

void Parser::parseCommand(const std::string& line)
{
	size_t index = line.find(',');
	if (index != std::string::npos) {
		std::string cmd = line.substr(0, index);
		auto i = str_command.find(cmd);
		if (i == str_command.end()) {
			printError("Incorrect command in line: ", line);
		}
		else {
			Command command = i->second;

			switch (command) {
			case Command::ORDER_ADD:
				addOrder(line);
				break;
			case Command::ORDER_MODIFY:
				modifyOrder(line);
				break;
			case Command::ORDER_CANCEL:
				cancelOrder(line);
				break;
			case Command::SUBSCRIBE_BBO:
				printError("This command is not implemented yet: ", line);
				break;
			case Command::UNSUBSCRIBE_BBO:
				printError("This command is not implemented yet: ", line);
				break;
			case Command::SUBSCRIBE_VWAP:
				printError("This command is not implemented yet: ", line);
				break;
			case Command::UNSUBSCRIBE_VWAP:
				printError("This command is not implemented yet: ", line);
				break;
			case Command::PRINT:
				print(line);
				break;
			case Command::PRINT_FULL:
				printError("This command is not implemented yet: ", line);
				break;
			}
		}
	}
	else {
		printError("Incorrect command syntax in line : ", line);
	}
}

bool Parser::parseArguments(const int countArguments, const std::string& line, std::vector<std::string>& arguments) const
{
	size_t previous = 0;
	size_t current = line.find(',', previous);
	std::string argument;
	size_t parsedArguments = 0;
	bool isCommandSkipped = false;

	while (current != std::string::npos && parsedArguments <= countArguments) {
		if (!isCommandSkipped) {
			isCommandSkipped = true;
			previous = current + 1;
			current = line.find(',', previous);
		}
		else {
			if (current - previous != 0) {
				argument = line.substr(previous, current - previous);
				arguments.push_back(argument);
				previous = current + 1;
				current = line.find(',', previous);
				++parsedArguments;
			}
			else {
				printError("Empty argument in line: ", line);
				return false;
			}
		}
	}
	if (previous != std::string::npos) {
		argument = line.substr(previous, current - previous);
		arguments.push_back(argument);
		++parsedArguments;
	}

	if (current == std::string::npos) {
		if (parsedArguments < countArguments) {
			printError("Not enough arguments in line: ", line);
			return false;
		}
		else if (parsedArguments > countArguments) {
			printError("Too many arguments in line: ", line);
			return false;
		}
		else {
			return true;
		}
	}

	return false;
}

bool Parser::convertString_Id(const std::string& stringId, uint64_t& id)
{
	try {
		id = std::stoull(stringId);
		return true;
	}
	catch (std::exception& e) {
		printError(e.what(), stringId);
	}

	return false;
}

bool Parser::convertString_Side(const std::string& stringSide, Side& side)
{
	bool isCorrectSide = false;
	if (stringSide == "Buy") {
		side = Side::BUY;
		isCorrectSide = true;
	}
	else if (stringSide == "Sell") {
		side = Side::SELL;
		isCorrectSide = true;
	}

	return isCorrectSide;
}

bool Parser::convertString_Quantity(const std::string& stringQuantity, uint64_t& quantity)
{
	try {
		quantity = std::stoull(stringQuantity);
		return true;
	}
	catch (std::exception& e) {
		printError(e.what(), stringQuantity);
	}

	return false;
}

bool Parser::convertString_Price(const std::string& stringPrice, double& price)
{
	try {
		price = std::stod(stringPrice);
		return true;
	}
	catch (std::exception& e) {
		printError(e.what(), stringPrice);
	}

	return false;
}

void Parser::addOrder(const std::string& line)
{
	std::vector<std::string> parsedArguments;
	if (!parseArguments(5, line, parsedArguments)) {
		return;
	}

	uint64_t orderId;
	if (!convertString_Id(parsedArguments[0], orderId)) {
		printError("Incorrect order id in line: ", line);
		return;
	}

	std::string symbol = parsedArguments[1];

	Side side;
	if (!convertString_Side(parsedArguments[2], side)) {
		printError("Incorrect order side in line: ", line);
		return;
	}

	uint64_t quantity;
	if (!convertString_Quantity(parsedArguments[3], quantity)) {
		printError("Incorrect order quantity in line: ", line);
		return;
	}

	double price;
	if (!convertString_Price(parsedArguments[4], price)) {
		printError("Incorrect order price in line: ", line);
		return;
	}

	auto& orderBooks = marketData.getSymbol_orderBook();
	if (!findOrderBook(symbol, orderBooks)) {
		marketData.addOrderBook(symbol);
	}
	auto& orderBook = marketData.getOrderBook(symbol);
	if (orderBook.addOrder(orderId, side, quantity, price)) {
		marketData.addSymbol(orderId, symbol);
	}
	else {
		printError("Incorrect order id in line: ", line);
	}
}

void Parser::modifyOrder(const std::string& line)
{
	std::vector<std::string> parsedArguments;
	if (!parseArguments(3, line, parsedArguments)) {
		return;
	}

	uint64_t orderId;
	if (!convertString_Id(parsedArguments[0], orderId)) {
		printError("Incorrect order id in line: ", line);
		return;
	}


	uint64_t quantity;
	if (!convertString_Quantity(parsedArguments[1], quantity)) {
		printError("Incorrect order quantity in line: ", line);
		return;
	}

	double price;
	if (!convertString_Price(parsedArguments[2], price)) {
		printError("Incorrect order price in line: ", line);
		return;
	}

	auto& orderId_Symbol = marketData.getOrderId_symbol();
	if (findOrderBook(orderId, orderId_Symbol)) {
		auto& orderBook = marketData.getOrderBook(orderId);
		orderBook.modifyOrder(orderId, quantity, price);
	}
	else
	{
		printError("Incorrect order id in line: ", line);
	}
}

void Parser::cancelOrder(const std::string& line)
{
	std::vector<std::string> parsedArguments;
	if (!parseArguments(1, line, parsedArguments)) {
		return;
	}

	uint64_t orderId;
	if (!convertString_Id(parsedArguments[0], orderId)) {
		printError("Incorrect order id in line: ", line);
		return;
	}

	auto& orderId_Symbol = marketData.getOrderId_symbol();
	if (findOrderBook(orderId, orderId_Symbol)) {

		auto& orderBook = marketData.getOrderBook(orderId);
		orderBook.cancelOrder(orderId);
		orderId_Symbol.erase(orderId);
	}
	else
	{
		printError("Incorrect order id in line: ", line);
	}
}

void Parser::print(const std::string& line) const
{
	std::string symbol;
	if (!isOutputFiltered) {
		std::vector<std::string> parsedArguments;
		if (!parseArguments(1, line, parsedArguments)) {
			return;
		}
		symbol = parsedArguments[0];
	}
	else {
		symbol = symbolFilter;
	}

	const auto& orderBooks = marketData.getSymbol_orderBook();
	if (!findOrderBook(symbol, orderBooks)) {
		std::cerr << "Order symbol " << symbol << " doesn't exist in market data!" << std::endl;
		return;
	}

	const auto& orderBook = marketData.getOrderBook(symbol);
	const auto& bid = orderBook.getBid();
	const auto& ask = orderBook.getAsk();
	auto bidIterator = bid.begin();
	auto askIterator = ask.begin();
	std::cout << "Bid" << std::setw(32) << "Ask" << std::endl;
	while (bidIterator != bid.end() || askIterator != ask.end()) {
		std::ostringstream bidColumn;
		std::ostringstream askColumn;
		if (bidIterator != bid.end()) {
			double bidPrice = bidIterator->first;
			uint64_t bidTotalQuantity = 0;
			for (auto orderId : bidIterator->second) {
				bidTotalQuantity += orderBook.getOrder(orderId).getQuantity();
			}
			bidColumn << bidTotalQuantity << '@' << bidPrice;
			++bidIterator;
		}
		std::cout << std::left << std::setw(17) << bidColumn.str();
		std::cout << '|';
		if (askIterator != ask.end()) {
			double askPrice = askIterator->first;
			uint64_t askTotalQuantity = 0;
			for (auto orderId : askIterator->second) {
				askTotalQuantity += orderBook.getOrder(orderId).getQuantity();
			}
			askColumn << askTotalQuantity << '@' << askPrice;
			++askIterator;
		}
		std::cout << std::right << std::setw(17) << askColumn.str() << std::endl;
	}
}

bool Parser::findOrderBook(const std::string& symbol, 
	const std::unordered_map<std::string, OrderBook>& symbol_orderBook) const
{
	auto i = symbol_orderBook.find(symbol);
	if (i != symbol_orderBook.end()) {
		return true;
	}

	return false;
}

bool Parser::findOrderBook(uint64_t orderId,
	const std::unordered_map<uint64_t, std::string>& orderId_symbol) const
{
	auto i = orderId_symbol.find(orderId);
	if (i != orderId_symbol.end()) {
		return true;
	}

	return false;
}

void Parser::printError(const std::string& error, const std::string& line) const
{
	std::cerr << error << line << std::endl;
}
