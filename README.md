# matching-engine

A C++20-based matching engine project that implements core functionalities for order matching and includes components for processing pairs and publishing market data.

``` 
 matching-engine/
├── engine/
│   ├── main.cpp
│   └── CMakeLists.txt
├── core/
│   ├── include/
│   │   ├── limit.h
│   │   ├── order.h
│   │   ├── match.h
│   │   ├── order_book.h
│   │   └── matching_service.h
│   ├── limit.cpp
│   ├── order.cpp
│   ├── match.cpp
│   ├── order_book.cpp
│   ├── matching_service.cpp
│   └── CMakeLists.txt
├── tests/
│   ├── core/
│   │   ├── test_limit.cpp
│   │   ├── test_order.cpp
│   └── engine/
│       ├── test_pair_processor.cpp
│       ├── test_market_publisher.cpp
│   └── CMakeLists.txt
└── CMakeLists.txt

```

# Key Components
## Core
Contains the main logic for order matching (limit, order, match, etc.).

## Engine
Includes the entry point for processing pairs (pair_processor) and publishing market data (market_publisher).
## Tests
Unit tests for validating the functionality of core and engine modules using Google Test.


# How to try

### Executing producer
```shell
kafka-console-producer --broker-list localhost:9092 --topic BTC_USDT
```

### Place limit order
```shell
type=place_order,id=12345,userId=67890,orderType=0,bid=true,size=1.5,price=10000.0,pair=BTC-USD
type=place_order,id=12323,userId=67894,orderType=0,bid=true,size=1.2,price=10001.1,pair=BTC-USD
type=place_order,id=12346,userId=67891,orderType=0,bid=false,size=1,price=9999.9,pair=BTC-USD
type=place_order,id=12348,userId=67899,orderType=0,bid=false,size=1,price=9999.8,pair=BTC-USD
```

### Place market order

```shell
type=place_order,id=523434,userId=63453,orderType=1,bid=false,size=0.5,price=10000.0,pair=BTC-USD
type=place_order,id=63435,userId=74532,orderType=1,bid=true,size=0.3,price=9999.9,pair=BTC-USD
```

### Cancel order
```shell
type=cancel_order,orderId=12345

```
