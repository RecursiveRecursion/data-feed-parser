#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <string>
#include <unordered_map>
#include <cstdint>
#include <utility>

#include "OrderBook.h"


class MarketData
{
private:
	std::unordered_map<std::string, OrderBook> symbol_orderBook;
	std::unordered_map<uint64_t, std::string> orderId_symbol;

public:
	std::unordered_map<std::string, OrderBook>& getSymbol_orderBook();
	std::unordered_map<uint64_t, std::string>& getOrderId_symbol();
	void addOrderBook(const std::string& symbol);
	OrderBook& getOrderBook(const std::string& symbol);
	OrderBook& getOrderBook(uint64_t orderId);
	void addSymbol(const uint64_t orderId, const std::string& symbol);
};

#endif // !MARKET_DATA_H
