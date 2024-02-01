//
// Created by prema on 6/9/2023.
//

#include "exchange/OrderBook.hpp"

namespace exchange {
    void OrderBook::insertOrder(order_ptr order) {
        if (order->getSide() == OrderSide::BUY)
            return insertBuyOrder(std::move(order));
        else return insertSellOrder(std::move(order));
    }

    void OrderBook::updateOrder(order_ptr currOrder, Order &newOrder) {
        if (currOrder->getSide() == OrderSide::BUY)
            return updateBuyOrder(std::move(currOrder), newOrder);
        else return updateSellOrder(std::move(currOrder), newOrder);
    }

    void OrderBook::insertBuyOrder(order_ptr order) {
        auto price = order->getPrice();
        auto it = _bids.find(price);
        if (it == _bids.end()) {
            auto [newIt, inserted] = _bids.emplace(price, PriceLevel{price});
            it = newIt;
        }

        it->second.insertOrder(std::move(order));
    }

    void OrderBook::insertSellOrder(order_ptr order) {
        auto price = order->getPrice();
        auto it = _offers.find(price);
        if (it == _offers.end()) {
            auto [newIt, inserted] = _offers.emplace(price, PriceLevel{price});
            it = newIt;
        }

        it->second.insertOrder(std::move(order));
    }

    void OrderBook::updateBuyOrder(order_ptr currOrder, Order& newOrder) {
        auto price = currOrder->getPrice();
        auto it = _bids.find(price);
        if (it == _bids.end())
            throw;

        auto& pLevel = it->second;
        pLevel.updateOrder(std::move(currOrder), newOrder);
    }

    void OrderBook::updateSellOrder(order_ptr currOrder, Order& newOrder) {
        auto price = currOrder->getPrice();
        auto it = _offers.find(price);
        if (it == _offers.end())
            throw;

        auto& pLevel = it->second;
        pLevel.updateOrder(std::move(currOrder), newOrder);
    }


}
