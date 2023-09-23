//
// Created by prema on 23/9/2023.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <lob/OrderBook.hpp>

TEST_CASE("Quick check", "[main]") {
    OrderBook lob{};
    REQUIRE( 1 == 1 );
}
