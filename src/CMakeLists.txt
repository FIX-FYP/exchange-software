 file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${OrderBookMatchingEngine_SOURCE_DIR}/include/lob/*.hpp")

add_library(
        order_book
        OrderBook.cpp
        ${HEADER_LIST})

 target_include_directories(order_book PUBLIC ../include)

 target_compile_features(order_book PUBLIC cxx_std_17)

 source_group(
         TREE "${PROJECT_SOURCE_DIR}/include"
         PREFIX "Header Files"
         FILES ${HEADER_LIST})
