#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <ios>
#include <sstream>

#include "MarketData.h"
#include "OrderBook.h"
#include "EnumCommand.h"
#include "EnumSide.h"


class Parser
{
private:
	std::string symbolFilter;
	bool isOutputFiltered = false;
	MarketData& marketData;
	std::unordered_map<std::string, Command> str_command{
		{ "ORDER ADD", Command::ORDER_ADD },
		{ "ORDER MODIFY", Command::ORDER_MODIFY },
		{ "ORDER CANCEL", Command::ORDER_CANCEL },
		{ "SUBSCRIBE BBO", Command::SUBSCRIBE_BBO },
		{ "UNSUBSCRIBE_BBO", Command::UNSUBSCRIBE_BBO },
		{ "SUBSCRIBE VWAP", Command::SUBSCRIBE_VWAP },
		{ "UNSUBSCRIBE VWAP", Command::UNSUBSCRIBE_VWAP },
		{ "PRINT", Command::PRINT },
		{ "PRINT_FULL", Command::PRINT_FULL }
	};

public:
	Parser(MarketData& marketData, const std::string& symbolFilter="");
	bool getIsOutputFiltered() const;
	void parseCommand(const std::string& line);
	bool parseArguments(const int countArguments, const std::string& line, std::vector<std::string>& arguments) const;
	bool convertString_Id(const std::string& stringId, uint64_t& id);
	bool convertString_Side(const std::string& stringSide, Side& side);
	bool convertString_Quantity(const std::string& stringQuantity, uint64_t& quantity);
	bool convertString_Price(const std::string& stringPrice, double& price);
	void addOrder(const std::string& line);
	void modifyOrder(const std::string& line);
	void cancelOrder(const std::string& line);
	void print(const std::string& line) const;
	bool findOrderBook(const std::string& symbol, 
		const std::unordered_map<std::string, OrderBook>& symbol_orderBook) const;
	bool findOrderBook(uint64_t orderId,
		const std::unordered_map<uint64_t, std::string>& orderId_symbol) const;
	void printError(const std::string& error, const std::string& line) const;
};

#endif // !PARSER_H
