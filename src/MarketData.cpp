#include "MarketData.h"

std::unordered_map<std::string, OrderBook>& MarketData::getSymbol_orderBook()
{
	return symbol_orderBook;
}


std::unordered_map<uint64_t, std::string>& MarketData::getOrderId_symbol()
{
	return orderId_symbol;
}

void MarketData::addOrderBook(const std::string& symbol)
{
	symbol_orderBook.insert(std::make_pair(symbol, OrderBook(symbol)));
}

OrderBook& MarketData::getOrderBook(const std::string& symbol)
{
	return symbol_orderBook.find(symbol)->second;
}

OrderBook& MarketData::getOrderBook(uint64_t orderId)
{
	auto i = orderId_symbol.find(orderId);
	const std::string& symbol = i->second;
	return getOrderBook(symbol);
}

void MarketData::addSymbol(const uint64_t orderId, const std::string& symbol)
{
	orderId_symbol.insert(std::make_pair(orderId, symbol));
}
