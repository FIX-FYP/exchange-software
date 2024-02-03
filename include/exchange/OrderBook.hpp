//
// Created by prema on 6/9/2023.
//

#ifndef MATCH_ENGINE_ORDERBOOK_HPP
#define MATCH_ENGINE_ORDERBOOK_HPP

#include <optional>
#include <map>
#include "exchange/Order.hpp"
#include "exchange/PriceLevel.hpp"

namespace exchange {
    class OrderBook {
    public:
        OrderBook()=default;
        void insertOrder(order_ptr);
        void updateOrder(order_ptr currOrder, Order& newOrder);
        void removeOrder(const Order&);
        void print() const;

    public:
        std::optional<Order> extractBestBid(const Order&);
        std::optional<Order> extractBestOffer(const Order&);

    public:
        [[nodiscard]] std::optional<order_list::const_iterator> peekTopBid() const;
        [[nodiscard]] std::optional<order_list::const_iterator> peekTopOffer() const;

    private:
        void insertBuyOrder(order_ptr);
        void insertSellOrder(order_ptr);

        void updateBuyOrder(order_ptr, Order&);
        void updateSellOrder(order_ptr, Order&);

        void removeBuyOrder(const Order&);
        void removeSellOrder(const Order&);

    private:
        std::map<order_price, PriceLevel, std::greater<>> _bids;
        std::map<order_price, PriceLevel, std::less<>> _offers;
    };

}


#endif //MATCH_ENGINE_ORDERBOOK_HPP
