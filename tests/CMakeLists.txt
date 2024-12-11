# Core tests
add_executable(tests ${SOURCES}
    main.cpp
       core/test_limit.cpp
       core/test_order_book.cpp
    #    core/test_order.cpp
    #    core/test_match.cpp
    #    core/test_order_book.cpp
)
target_link_libraries(tests PRIVATE core gtest_main)

## Consumer tests
#add_executable(test_pair_processor
#        consumer/test_pair_processor.cpp
#)
#target_link_libraries(test_pair_processor PRIVATE core gtest gtest_main)

#add_executable(test_market_publisher
#        consumer/test_market_publisher.cpp
#)
#target_link_libraries(test_market_publisher PRIVATE core gtest gtest_main)

enable_testing()
# Register tests
add_test(NAME CoreTests COMMAND tests)
#add_test(NAME PairProcessorTests COMMAND test_pair_processor)
#add_test(NAME MarketPublisherTests COMMAND test_market_publisher)
