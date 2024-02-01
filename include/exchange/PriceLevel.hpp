//
// Created by prema on 1/2/2024.
//

#ifndef MATCH_ENGINE_PRICELEVEL_HPP
#define MATCH_ENGINE_PRICELEVEL_HPP

#include <list>
#include "exchange/Order.hpp"

namespace exchange {
    using order_list = std::list<std::shared_ptr<Order>>;
    using level_volume = uint64_t;
    class PriceLevel {
    public:
        explicit PriceLevel(order_price);

    public:
        std::shared_ptr<Order> extractTop();
        [[nodiscard]] order_list::iterator peekTop() const;
        void insertOrder(std::shared_ptr<Order>);
        void updateOrder(std::shared_ptr<Order> currOrder, const Order& newOrder);
        void removeOrder(std::shared_ptr<Order>);
        void print() const;

    public:
        [[nodiscard]] level_volume getVolume() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] size_t getSize() const noexcept;
        [[nodiscard]] order_price getPrice() const noexcept;

    private:
        order_list _orders;
        level_volume _volume;
        order_price _price;
    };
}

#endif //MATCH_ENGINE_PRICELEVEL_HPP
