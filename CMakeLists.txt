cmake_minimum_required(VERSION 3.19)
project(DataFeedParser)

set(CMAKE_CXX_STANDARD 20)

add_executable(md_replay
        src/main.cpp
        src/EnumSide.h
        src/EnumCommand.h
        src/Order.cpp
        src/Order.h
        src/OrderBook.cpp
        src/OrderBook.h
        src/MarketData.cpp
        src/MarketData.h
        src/Parser.cpp
        src/Parser.h
        )
