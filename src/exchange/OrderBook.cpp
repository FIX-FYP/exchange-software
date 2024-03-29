//
// Created by prema on 6/9/2023.
//

#include "exchange/OrderBook.hpp"
#include <fmt/format.h>

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

    void OrderBook::removeOrder(const Order& order) {
        if (order.getSide() == OrderSide::BUY)
            return removeBuyOrder(order);
        else return removeSellOrder(order);
    }

    void OrderBook::print() const {
        fmt::print("--------------------\nBIDS:\n");
        for (const auto& [price, level] : _bids) {
            level.print();
        }

        fmt::print("--------------------\nOFFERS:\n");
        for (const auto& [price, level] : _offers) {
            level.print();
        }
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
        auto itCurrPLevel = _bids.find(price);
        if (itCurrPLevel == _bids.end())
            throw;

        auto& currPLevel = itCurrPLevel->second;
        if (currOrder->getPrice() == newOrder.getPrice())
            currPLevel.updateOrder(currOrder, newOrder);
        else {
            currPLevel.removeOrder(*currOrder);
            if (currPLevel.empty())
                _bids.erase(itCurrPLevel);

            currOrder->swap(newOrder);
            order_price newPrice = currOrder->getPrice();
            auto itNewPLevel = _bids.find(newPrice);
            if (itNewPLevel == _bids.end()) {
                auto [newIt, inserted] = _bids.emplace(newPrice, PriceLevel{newPrice});
                itNewPLevel = newIt;
            }

            itNewPLevel->second.insertOrder(std::move(currOrder));
        }
    }

    void OrderBook::updateSellOrder(order_ptr currOrder, Order& newOrder) {
        auto price = currOrder->getPrice();
        auto itCurrPLevel = _offers.find(price);
        if (itCurrPLevel == _offers.end())
            throw;

        auto& currPLevel = itCurrPLevel->second;
        if (currOrder->getPrice() == newOrder.getPrice())
            currPLevel.updateOrder(currOrder, newOrder);
        else {
            currPLevel.removeOrder(*currOrder);
            if (currPLevel.empty())
                _offers.erase(itCurrPLevel);

            currOrder->swap(newOrder);
            order_price newPrice = currOrder->getPrice();
            auto itNewPLevel = _offers.find(newPrice);
            if (itNewPLevel == _offers.end()) {
                auto [newIt, inserted] = _offers.emplace(newPrice, PriceLevel{newPrice});
                itNewPLevel = newIt;
            }

            itNewPLevel->second.insertOrder(std::move(currOrder));
        }
    }

    void OrderBook::removeBuyOrder(const Order& order) {
        auto price = order.getPrice();
        auto it = _bids.find(price);
        if (it == _bids.end())
            throw;

        auto& pLevel = it->second;
        pLevel.removeOrder(order);
        if (pLevel.empty())
            _bids.erase(it);
    }

    void OrderBook::removeSellOrder(const Order& order) {
        auto price = order.getPrice();
        auto it = _offers.find(price);
        if (it == _offers.end())
            throw;

        auto& pLevel = it->second;
        pLevel.removeOrder(order);
        if (pLevel.empty())
            _offers.erase(it);
    }

    std::optional<order_list::const_iterator> OrderBook::peekTopBid() const {
        if (_bids.empty())
            return std::nullopt;

        return _bids.begin()->second.peekTop();
    }

    std::optional<order_list::const_iterator> OrderBook::peekTopOffer() const {
        if (_offers.empty())
            return std::nullopt;

        return _offers.begin()->second.peekTop();
    }
}
