//
// Created by prema on 3/2/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "exchange/OrderBook.hpp"

TEST_CASE("Orders should be inserted correctly", "[orderbook]") {
    exchange::OrderBook ob;
    auto bo1 = std::make_shared<exchange::Order>(
            1, "John",
            exchange::OrderType::LIMIT,
            exchange::OrderStatus::NEW,
            exchange::OrderSide::BUY,
            "ABC", 100.0, 50, 2);

    auto so1 = std::make_shared<exchange::Order>(
            1, "John",
            exchange::OrderType::LIMIT,
            exchange::OrderStatus::NEW,
            exchange::OrderSide::SELL,
            "ABC", 100.0, 50, 2);

    SECTION("Empty bids and offers should return empty") {
        auto topBid = ob.peekTopBid();
        REQUIRE(!topBid.has_value());

        auto topOffer = ob.peekTopOffer();
        REQUIRE(!topOffer.has_value());
    }

    SECTION("Only empty bids should return empty") {
        ob.insertOrder(so1);

        auto topBid = ob.peekTopBid();
        REQUIRE(!topBid.has_value());
    }

    SECTION("Only empty offers should return empty") {
        ob.insertOrder(bo1);

        auto topOffer = ob.peekTopOffer();
        REQUIRE(!topOffer.has_value());
    }

    SECTION("Inserted buy orders should be ordered by price first") {
        ob.insertOrder(bo1);
        auto topBid = ob.peekTopBid();
        auto topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo1->getOrderId());

        auto bo2 = std::make_shared<exchange::Order>(*bo1);
        bo2->setPrice(101.1);
        bo2->setOrderId(3);

        ob.insertOrder(bo2);
        topBid = ob.peekTopBid();
        topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo2->getOrderId());


        auto bo3 = std::make_shared<exchange::Order>(*bo2);
        bo2->setPrice(101.05);
        bo2->setOrderId(4);

        ob.insertOrder(bo3);
        topBid = ob.peekTopBid();
        topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo2->getOrderId());
    }

    SECTION("Inserted sell orders should be ordered by price first") {
        ob.insertOrder(so1);
        auto topOffer = ob.peekTopOffer();
        auto topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so1->getOrderId());

        auto so2 = std::make_shared<exchange::Order>(*so1);
        so2->setPrice(99.9);
        so2->setOrderId(3);

        ob.insertOrder(so2);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so2->getOrderId());


        auto so3 = std::make_shared<exchange::Order>(*so2);
        so2->setPrice(99.95);
        so2->setOrderId(4);

        ob.insertOrder(so3);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so2->getOrderId());
    }

}