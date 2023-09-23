 file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ExchangeSoftware_SOURCE_DIR}/include/lob/*.hpp")

add_library(
        orderbook
        OrderBook.cpp
        ${HEADER_LIST})

 target_include_directories(orderbook PUBLIC ../include)

 target_compile_features(orderbook PUBLIC cxx_std_17)

 source_group(
         TREE "${PROJECT_SOURCE_DIR}/include"
         PREFIX "Header Files"
         FILES ${HEADER_LIST})
