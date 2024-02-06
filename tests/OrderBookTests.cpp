//
// Created by prema on 3/2/2024.
//

#include <catch2/catch_test_macros.hpp>

#include <exchange/OrderBook.hpp>

#include <fmt/format.h>

#include "ExchangeOrderMatcher.hpp"

//void inline testOrderEquality(exchange::order_list::const_iterator itOrder, const exchange::Order& expected) {
//    REQUIRE((*itOrder)->getOrderId() == expected.getOrderId());
//    REQUIRE((*itOrder)->getQuantity() == expected.getQuantity());
//    REQUIRE((*itOrder)->getPrice() == expected.getPrice());
//    REQUIRE((*itOrder)->getTimestamp() == expected.getTimestamp());
//    REQUIRE((*itOrder)->getSide() == expected.getSide());
//    REQUIRE((*itOrder)->getSenderId() == expected.getSenderId());
//    REQUIRE((*itOrder)->getStatus() == expected.getStatus());
//    REQUIRE((*itOrder)->getSymbol() == expected.getSymbol());
//    REQUIRE((*itOrder)->getType() == expected.getType());
//}

ExchangeOrderMatcher EqualsExchangeOrder(const exchange::Order& order) {
    return ExchangeOrderMatcher{order};
}

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
        bo3->setPrice(101.05);
        bo3->setOrderId(4);

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
        so3->setPrice(99.95);
        so3->setOrderId(4);

        ob.insertOrder(so3);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so2->getOrderId());
    }

    SECTION("Inserted buy order should be ordered by time for same price") {
        ob.insertOrder(bo1);

        auto bo2 = std::make_shared<exchange::Order>(*bo1);
        bo2->setOrderId(3);
        bo2->setTimestamp(3);

        ob.insertOrder(bo2);
        auto topBid = ob.peekTopBid();
        auto topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo1->getOrderId());


        auto bo3 = std::make_shared<exchange::Order>(*bo2);
        bo3->setTimestamp(1);
        bo3->setOrderId(4);

        ob.insertOrder(bo3);
        topBid = ob.peekTopBid();
        topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo3->getOrderId());
    }

    SECTION("Inserted sell order should be ordered by time for same price") {
        ob.insertOrder(so1);

        auto so2 = std::make_shared<exchange::Order>(*so1);
        so2->setOrderId(3);
        so2->setTimestamp(3);

        ob.insertOrder(so2);
        auto topOffer = ob.peekTopOffer();
        auto topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so1->getOrderId());

        auto so3 = std::make_shared<exchange::Order>(*so2);
        so3->setTimestamp(1);
        so3->setOrderId(4);

        ob.insertOrder(so3);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so3->getOrderId());
    }
}

TEST_CASE("Orders should be updated correctly", "[orderbook]") {
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

    SECTION("Updating buy order quantity should persist price") {
        ob.insertOrder(bo1);

        auto topBid = ob.peekTopBid();
        auto topBidVal = topBid.value();
        const auto price = bo1->getPrice();
        REQUIRE((*topBidVal)->getOrderId() == bo1->getOrderId());
        REQUIRE((*topBidVal)->getQuantity() == bo1->getQuantity());
        REQUIRE((*topBidVal)->getPrice() == price);

        exchange::Order o1Copy = *bo1;
        const exchange::order_quantity newQ = 10000;
        o1Copy.setQuantity(newQ);
        ob.updateOrder(bo1, o1Copy);
        REQUIRE((*topBidVal)->getOrderId() == bo1->getOrderId());
        REQUIRE((*topBidVal)->getQuantity() == newQ);
        REQUIRE((*topBidVal)->getPrice() == price);
    }

    SECTION("Updating sell order quantity should persist price ordering") {
        const auto price = so1->getPrice();
        const auto origQ = so1->getQuantity();
        ob.insertOrder(so1);

        auto topOffer = ob.peekTopOffer();
        auto topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so1->getOrderId());
        REQUIRE((*topOfferVal)->getQuantity() == origQ);
        REQUIRE((*topOfferVal)->getPrice() == price);

        exchange::Order o1Copy = *so1;
        const exchange::order_quantity newQ = 10001;
        o1Copy.setQuantity(newQ);

        ob.updateOrder(so1, o1Copy);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        REQUIRE((*topOfferVal)->getOrderId() == so1->getOrderId());
        REQUIRE((*topOfferVal)->getQuantity() == newQ);
        REQUIRE((*topOfferVal)->getPrice() == price);
    }

    SECTION("Updating buy order quantity should persist time ordering") {
        const auto price = bo1->getPrice();
        const auto origQ = bo1->getQuantity();
        ob.insertOrder(bo1);

        auto bo2 = std::make_shared<exchange::Order>(*bo1);
        bo2->setOrderId(3);
        bo2->setTimestamp(3);
        ob.insertOrder(bo2);

        auto topBid = ob.peekTopBid();
        auto topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo1->getOrderId());
        REQUIRE((*topBidVal)->getQuantity() == origQ);
        REQUIRE((*topBidVal)->getPrice() == price);

        exchange::Order o1Copy = *bo1;
        const exchange::order_quantity newQ = 10000;
        o1Copy.setQuantity(newQ);

        ob.updateOrder(bo1, o1Copy);
        topBid = ob.peekTopBid();
        topBidVal = topBid.value();
        REQUIRE((*topBidVal)->getOrderId() == bo1->getOrderId());
        REQUIRE((*topBidVal)->getQuantity() == newQ);
        REQUIRE((*topBidVal)->getPrice() == price);

        auto bo3 = std::make_shared<exchange::Order>(*bo1);
        bo3->setOrderId(4);
        bo3->setTimestamp(1);
        ob.insertOrder(bo3);

        o1Copy = *bo1;
        const exchange::order_quantity lastQ = 5;
        o1Copy.setQuantity(lastQ);

        ob.updateOrder(bo1, o1Copy);
        topBid = ob.peekTopBid();
        topBidVal = topBid.value();
        auto bo1It = std::next(topBidVal);
        REQUIRE((*bo1It)->getOrderId() == bo1->getOrderId());
        REQUIRE((*bo1It)->getQuantity() == lastQ);
        REQUIRE((*bo1It)->getPrice() == price);
    }

    SECTION("Updating sell order quantity should persist time ordering") {
        auto so1Copy = exchange::Order{*so1};
        ob.insertOrder(so1);

        auto so2 = std::make_shared<exchange::Order>(*so1);
        so2->setOrderId(3);
        so2->setTimestamp(3);
        ob.insertOrder(so2);

        auto topOffer = ob.peekTopOffer();
        auto topOfferVal = topOffer.value();
        REQUIRE_THAT(**topOfferVal, EqualsExchangeOrder(so1Copy));

        exchange::Order o1Copy = *so1;
        const exchange::order_quantity newQ = 10002;
        o1Copy.setQuantity(newQ);
        exchange::Order expectedOrder = o1Copy;

        ob.updateOrder(so1, o1Copy);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        REQUIRE_THAT(**topOfferVal, EqualsExchangeOrder(expectedOrder));

        auto so3 = std::make_shared<exchange::Order>(*so1);
        so3->setOrderId(4);
        so3->setTimestamp(1);
        ob.insertOrder(so3);

        o1Copy = *so1;
        const exchange::order_quantity lastQ = 5;
        o1Copy.setQuantity(lastQ);
        expectedOrder = o1Copy;

        ob.updateOrder(so1, o1Copy);
        topOffer = ob.peekTopOffer();
        topOfferVal = topOffer.value();
        auto so1It = std::next(topOfferVal);
        REQUIRE_THAT(**so1It, EqualsExchangeOrder(expectedOrder));
    }

    SECTION("Updating buy order price should update price levels") {
        exchange::Order bo1Copy = *bo1;
        ob.insertOrder(bo1);

        auto topBid = ob.peekTopBid();
        REQUIRE(topBid.has_value());
        auto topBidValue = topBid.value();
        fmt::print("top\n");
        (**topBidValue).print();
        fmt::print("copy\n");
        bo1Copy.print();

        REQUIRE_THAT(**topBidValue, EqualsExchangeOrder(bo1Copy));

        auto newPriceBo1 = exchange::Order{bo1Copy};;
        newPriceBo1.setPrice(bo1Copy.getPrice() - 0.01);
        auto expectedOrder = exchange::Order{newPriceBo1};

        ob.updateOrder(bo1, newPriceBo1);
        topBid = ob.peekTopBid();
        topBidValue = topBid.value();
        REQUIRE_THAT(**topBidValue, EqualsExchangeOrder(expectedOrder));
    }
}