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
### Contains the main logic for order matching (limit, order, match, etc.).

## Engine
### Includes the entry point for processing pairs (pair_processor) and publishing market data (market_publisher).
## Tests
### Unit tests for validating the functionality of core and engine modules using Google Test.
