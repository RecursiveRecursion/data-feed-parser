#include "Order.h"

Order::Order(uint64_t orderId, Side side, uint64_t quantity, double price)
	: orderId{ orderId }, side{ side }, quantity{ quantity }, price{ price }
{

}

uint64_t Order::getOrderId() const { return orderId; }
void Order::setOrderId(uint64_t orderId) { this->orderId = orderId; }

Side Order::getSide() const { return side; }
void Order::setSide(Side side) { this->side = side; }

uint64_t Order::getQuantity() const { return quantity; }
void Order::setQuantity(uint64_t quantity) { this->quantity = quantity; }

double Order::getPrice() const { return price; }
void Order::setPrice(double price) { this->price = price; }
