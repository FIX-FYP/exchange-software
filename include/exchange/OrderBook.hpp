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
        void print() const;
        void insertOrder(order_ptr);
        void updateOrder(order_ptr currOrder, Order& newOrder);
        void removeOrder(order_ptr);

    public:
        std::optional<Order> extractBestBid(const Order&);
        std::optional<Order> extractBestOffer(const Order&);

    public:
//        std::optional<Order> extractBestBid(const Order&);
//        std::optional<Order> extractBestOffer(const Order&);

    private:
        void insertBuyOrder(order_ptr);
        void insertSellOrder(order_ptr);

        void updateBuyOrder(order_ptr, Order&);
        void updateSellOrder(order_ptr, Order&);

        void removeBuyOrder(order_ptr);
        void removeSellOrder(order_ptr);

    private:
        std::map<order_price, PriceLevel, std::greater<>> _bids;
        std::map<order_price, PriceLevel, std::less<>> _offers;
    };

}


#endif //MATCH_ENGINE_ORDERBOOK_HPP
