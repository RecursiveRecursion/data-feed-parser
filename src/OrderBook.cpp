#include "OrderBook.h"

OrderBook::OrderBook(const std::string& symbol)
	: symbol{symbol}
{

}

const std::string& OrderBook::getSymbol() const
{
	return symbol;
}

const std::map<double, std::set<uint64_t>, std::greater<double>>& OrderBook::getBid() const
{
	return bid;
}

const std::map<double, std::set<uint64_t>>& OrderBook::getAsk() const
{
	return ask;
}

bool OrderBook::addOrder(uint64_t orderId, Side side, uint64_t quantity, double price)
{
	bool isOrderAdded = false;

	auto i = orderId_order.find(orderId);
	if (i == orderId_order.end()) {
		orderId_order.insert(std::make_pair(orderId, Order{ orderId, side, quantity, price }));
		if (side == Side::BUY) {
			addBid(orderId, price);
			isOrderAdded = true;
		}
		else {
			addAsk(orderId, price);
			isOrderAdded = true;
		}
	}
	else {
		std::cerr << "Order won't be added. Id " << orderId << " is already in this order book!" << std::endl;
	}

	return isOrderAdded;
}

void OrderBook::addBid(uint64_t orderId, double price)
{
	auto i = bid.find(price);
	if (i == bid.end()) {
		i = bid.insert(std::make_pair(price, std::set<uint64_t>())).first;
	}
	i->second.insert(orderId);
}

void OrderBook::addAsk(uint64_t orderId, double price)
{
	auto i = ask.find(price);
	if (i == ask.end()) {
		i = ask.insert(std::make_pair(price, std::set<uint64_t>())).first;
	}
	i->second.insert(orderId);
}

bool OrderBook::modifyOrder(uint64_t orderId, uint64_t quantity, double price)
{
	bool isOrderModified = false;

	auto i = orderId_order.find(orderId);
	if (i != orderId_order.end()) {
		auto& order = i->second;
		if (order.getSide() == Side::BUY) {
			removeBid(orderId, order.getPrice());
		}
		else if (order.getSide() == Side::SELL) {
			removeAsk(orderId, order.getPrice());
		}
		order.setQuantity(quantity);
		order.setPrice(price);
		if (order.getSide() == Side::BUY) {
			addBid(orderId, price);
		}
		else if (order.getSide() == Side::SELL) {
			addAsk(orderId, price);
		}
		isOrderModified = true;
	}
	else {
		std::cerr << "Order won't be modified. Id " << orderId << " doesn't exist!" << std::endl;
	}

	return isOrderModified;
}

void OrderBook::cancelOrder(uint64_t orderId)
{
	auto i = orderId_order.find(orderId);
	if (i != orderId_order.end()) {
		auto& order = i->second;
		if (order.getSide() == Side::BUY) {
			removeBid(orderId, order.getPrice());
		}
		else if (order.getSide() == Side::SELL) {
			removeAsk(orderId, order.getPrice());
		}
		orderId_order.erase(i);
	}
	else {
		std::cerr << "Order won't be canceled. Id " << orderId << " doesn't exist!" << std::endl;
	}
}

void OrderBook::removeBid(uint64_t orderId, double price)
{
	auto i = bid.find(price);
	if (i != bid.end()) {
		auto& orderIds = i->second;
		orderIds.erase(orderId);
		if (orderIds.empty()) {
			bid.erase(i);
		}
	}
	else {
		std::cerr << "Bid won't be removed. Bid with price " << price << " doesn't exist!" << std::endl;
	}
}

void OrderBook::removeAsk(uint64_t orderId, double price)
{
	auto i = ask.find(price);
	if (i != ask.end()) {
		auto& orderIds = i->second;
		orderIds.erase(orderId);
		if (orderIds.empty()) {
			ask.erase(i);
		}
	}
	else {
		std::cerr << "Ask won't be removed. Ask with price " << price << " doesn't exist!" << std::endl;
	}
}

const Order& OrderBook::getOrder(uint64_t orderId) const
{
	return orderId_order.find(orderId)->second;
}
