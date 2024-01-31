//
// Created by prema on 31/1/2024.
//

#include "exchange/Order.hpp"
#include <fmt/format.h>

namespace exchange {
    std::string Order::orderTypeToStr(OrderType t) {
        switch (t) {
            case OrderType::MARKET:
                return "MARKET";
            case OrderType::LIMIT:
                return "LIMIT";
            default:
                throw;
        }
    }

    std::string Order::orderStatusToStr(OrderStatus s) {
        switch (s) {
            case OrderStatus::NEW:
                return "NEW";
            case OrderStatus::PENDING_NEW:
                return "PENDING_NEW";
            case OrderStatus::CANCEL:
                return "CANCEL";
            case OrderStatus::PENDING_CANCEL:
                return "PENDING_CANCEL";
            case OrderStatus::REPLACE:
                return "REPLACE";
            case OrderStatus::PENDING_REPLACE:
                return "PENDING_REPLACE";
            case OrderStatus::PARTIAL_FILL:
                return "PARTIAL_FILL";
            case OrderStatus::FILL:
                return "FILL";
            default:
                throw;
        }
    }

    std::string Order::orderSideToStr(OrderSide s) {
        if (s == OrderSide::BUY) return "BUY";
        else return "SELL";
    }

    void Order::print() const {
        fmt::print(
            "ORDER ID: {} | SENDER ID: {} | ORDER TYPE: {} | ORDER STATUS: {} | ORDER SIDE: {} | ORDER SYMBOL:  {} | ORDER PRICE: {} | ORDER QUANTITY: {} | ORDER TIMESTAMP: {}",
            _orderId,
            _senderId,
            orderTypeToStr(_type),
            orderStatusToStr(_status),
            orderSideToStr(_side),
            _symbol,
            _price,
            _quantity,
            _timestamp);
    }



}
