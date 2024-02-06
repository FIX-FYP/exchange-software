//
// Created by prema on 6/2/2024.
//

#ifndef MATCH_ENGINE_EXCHANGEORDERMATCHER_HPP
#define MATCH_ENGINE_EXCHANGEORDERMATCHER_HPP
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>

#include <exchange/Order.hpp>

struct ExchangeOrderMatcher : Catch::Matchers::MatcherGenericBase {
public:
    explicit ExchangeOrderMatcher(const exchange::Order& order) :
        _order{order} {}

    bool match(const exchange::Order& other) const {
        return _order.getOrderId() == other.getOrderId()
            && _order.getQuantity() == other.getQuantity()
            && _order.getPrice() == other.getPrice()
            && _order.getTimestamp() == other.getTimestamp()
            && _order.getSide() == other.getSide()
            && _order.getSenderId() == other.getSenderId()
            && _order.getStatus() == other.getStatus()
            && _order.getSymbol() == other.getSymbol()
            && _order.getType() == other.getType();
    }

    std::string describe() const override {
        return "Equals: " + _order.str();
    }

private:
    const exchange::Order& _order;
};

#endif //MATCH_ENGINE_EXCHANGEORDERMATCHER_HPP
