//
// Created by prema on 31/1/2024.
//

#ifndef MATCH_ENGINE_ORDER_HPP
#define MATCH_ENGINE_ORDER_HPP

#include <string>
#include <cstdint>
#include <utility>
#include <memory>

namespace exchange {
    using order_id = uint64_t;
    using sender_id = std::string;
    using product_symbol = std::string;
    using order_quantity = uint32_t;
    using order_timestamp = uint64_t;
    using order_price = double;

    enum class OrderSide {
        BUY,
        SELL
    };

    enum class OrderType {
        MARKET,
        LIMIT
    };

    enum class OrderStatus {
        NEW, PENDING_NEW,
        CANCEL, PENDING_CANCEL,
        REPLACE, PENDING_REPLACE,
        PARTIAL_FILL,
        FILL
    };


    class Order {
    public:
        static std::string orderTypeToStr(OrderType t);
        static std::string orderStatusToStr(OrderStatus s);
        static std::string orderSideToStr(OrderSide s);

    public:
        Order(order_id orderId, sender_id senderId, OrderType type, OrderStatus status, OrderSide side,
              product_symbol symbol, order_price price, order_quantity quantity, order_timestamp timestamp)
                : _orderId(orderId), _senderId(std::move(senderId)), _type(type), _status(status), _side(side), _symbol(std::move(symbol)),
                  _price(price), _quantity(quantity), _timestamp(timestamp) {}

    public:
        void print() const;
        void swap(Order& other);

    public:
        [[nodiscard]] order_id getOrderId() const {
            return _orderId;
        }

        [[nodiscard]] sender_id getSenderId() const {
            return _senderId;
        }

        [[nodiscard]] OrderType getType() const {
            return _type;
        }

        [[nodiscard]] OrderStatus getStatus() const {
            return _status;
        }

        [[nodiscard]] OrderSide getSide() const {
            return _side;
        }

        [[nodiscard]] product_symbol getSymbol() const {
            return _symbol;
        }

        [[nodiscard]] order_price getPrice() const {
            return _price;
        }

        [[nodiscard]] order_quantity getQuantity() const {
            return _quantity;
        }

        order_quantity& getQuantity() {
            return _quantity;
        }

        [[nodiscard]] order_timestamp getTimestamp() const {
            return _timestamp;
        }

    private:
        order_id _orderId;
        sender_id _senderId;
        OrderType _type;
        OrderStatus _status;
        OrderSide _side;
        product_symbol _symbol;
        order_price _price;
        order_quantity _quantity;
        order_timestamp _timestamp;
    };

    using order_ptr = std::shared_ptr<Order>;
}
#endif //MATCH_ENGINE_ORDER_HPP
