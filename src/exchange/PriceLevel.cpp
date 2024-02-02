//
// Created by prema on 2/2/2024.
//

#include "exchange/PriceLevel.hpp"
#include <fmt/format.h>

namespace exchange {
    PriceLevel::PriceLevel(order_price price) : _price{price}, _volume{0}, _orders{} {}

    void PriceLevel::insertOrder(order_ptr order) {
        _volume += order->getQuantity();
        if (_orders.empty()) {
            _orders.push_back(std::move(order));
            return;
        } else if ((*_orders.begin())->getTimestamp() <= order->getTimestamp()) {
            _orders.insert(std::next(_orders.begin()), std::move(order));
            return;
        }


        for (auto it = std::prev(_orders.end()); it != _orders.begin(); it--) {
            if ((*it)->getTimestamp() <= order->getTimestamp()) {
                _orders.insert(std::next(it), std::move(order));
                return;
            }
        }

        if ((*_orders.begin())->getTimestamp() > order->getTimestamp())
            _orders.insert(_orders.begin(), std::move(order));
        else throw;
    }

    void PriceLevel::updateOrder(order_ptr& currOrder, Order& newOrder) {
        _volume -= currOrder->getQuantity();
        _volume += newOrder.getQuantity();
        currOrder->swap(newOrder);
    }

    void PriceLevel::removeOrder(const Order& order) {
        for (auto it = _orders.begin(); it != _orders.end(); it++) {
            if ((*it)->getOrderId() == order.getOrderId()) {
                _volume -= order.getQuantity();
                _orders.erase(it);
                return;
            }
        }
    }

    void PriceLevel::print() const {
        fmt::print("PRICE: {} TOTAL VOLUME: {}\n", _price, _volume);

        if (_orders.empty()) {
            fmt::print("Empty Price Level!\n");
            return;
        }

        for (const auto& order : _orders) {
            order->print();
        }
    }

    level_volume PriceLevel::getVolume() const noexcept {
        return _volume;
    }

    bool PriceLevel::empty() const noexcept {
        return _orders.empty();
    }

    size_t PriceLevel::getSize() const noexcept {
        return _orders.size();
    }

    order_price PriceLevel::getPrice() const noexcept {
        return _price;
    }
}