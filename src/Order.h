#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

#include "EnumSide.h"


class Order
{
private:
	uint64_t orderId;
	Side side;
	uint64_t quantity;
	double price;

public:
	Order(uint64_t orderId, Side side, uint64_t quantity, double price);
	uint64_t getOrderId() const;
	void setOrderId(uint64_t orderId);
	Side getSide() const;
	void setSide(Side side);
	uint64_t getQuantity() const;
	void setQuantity(uint64_t quantity);
	double getPrice() const;
	void setPrice(double price);
};

#endif // !ORDER_H
