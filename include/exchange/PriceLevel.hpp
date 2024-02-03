//
// Created by prema on 1/2/2024.
//

#ifndef MATCH_ENGINE_PRICELEVEL_HPP
#define MATCH_ENGINE_PRICELEVEL_HPP

#include <list>
#include "exchange/Order.hpp"

namespace exchange {
    using order_list = std::list<order_ptr>;
    using level_volume = uint64_t;
    class PriceLevel {
    public:
        explicit PriceLevel(order_price);

    public:
        order_ptr extractTop();
        void insertOrder(order_ptr);
        void updateOrder(order_ptr& currOrder, Order& newOrder);
        void removeOrder(const Order&);
        [[nodiscard]] order_list::const_iterator peekTop() const;
        void print() const;

    public:
        [[nodiscard]] level_volume getVolume() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] size_t getSize() const noexcept;
        [[nodiscard]] order_price getPrice() const noexcept;

    private:
        order_price _price;
        level_volume _volume;
        order_list _orders;
    };
}

#endif //MATCH_ENGINE_PRICELEVEL_HPP
