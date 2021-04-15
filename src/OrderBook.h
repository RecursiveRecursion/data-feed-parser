#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <functional>
#include <utility>
#include <iostream>

#include "Order.h"
#include "EnumSide.h"


class OrderBook
{
private:
	std::string symbol;
	std::unordered_map<uint64_t, Order> orderId_order;
	std::map<double, std::set<uint64_t>, std::greater<double>> bid; // <price, set of orderId>
	std::map<double, std::set<uint64_t>> ask; // <price, set of orderId>

public:
	OrderBook(const std::string& symbol);
	const std::string& getSymbol() const;
	const std::map<double, std::set<uint64_t>, std::greater<double>>& getBid() const;
	const std::map<double, std::set<uint64_t>>& getAsk() const;

	bool addOrder(uint64_t orderId, Side side, uint64_t quantity, double price);
	void addBid(uint64_t orderId, double price);
	void addAsk(uint64_t orderId, double price);
	void removeBid(uint64_t orderId, double price);
	void removeAsk(uint64_t orderId, double price);
	bool modifyOrder(uint64_t orderId, uint64_t quantity, double price);
	void cancelOrder(uint64_t orderId);
	const Order& getOrder(uint64_t orderId) const;
};

#endif // !ORDER_BOOK_H
