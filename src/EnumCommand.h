#ifndef ENUM_COMMAND_H
#define ENUMD_COMMAND_H


enum class Command
{
	ORDER_ADD,
	ORDER_MODIFY,
	ORDER_CANCEL,
	SUBSCRIBE_BBO,
	UNSUBSCRIBE_BBO,
	SUBSCRIBE_VWAP,
	UNSUBSCRIBE_VWAP,
	PRINT,
	PRINT_FULL
};

#endif // !ENUM_COMMAND_H
