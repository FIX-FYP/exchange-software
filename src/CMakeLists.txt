set(HEADER_LIST_EXCHANGE_LIB
        ../include/exchange/OrderBook.hpp
#        ../include/exchange/Order.hpp
#        ../include/exchange/Order.hpp
        ../include/exchange/Order.hpp ../include/exchange/PriceLevel.hpp ../include/exchange/MatchEngine.hpp)

add_library(exchange
        exchange/OrderBook.cpp
        ${HEADER_LIST_EXCHANGE_LIB} exchange/Order.cpp exchange/PriceLevel.cpp)

target_link_libraries(exchange PRIVATE fmt::fmt)
target_include_directories(exchange PUBLIC ${PROJECT_SOURCE_DIR}/include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST_EXCHANGE_LIB})
