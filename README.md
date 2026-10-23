# E-Commerce System: Design Patterns and Exception Handling (C++)

A console e-commerce program that applies the **Singleton** pattern, the **Strategy** pattern for payments (`CashPayment`, `CreditCardPayment`, `GCashPayment`), custom exception classes, and the four pillars of OOP. It deliberately avoids STL containers.

## Build and run

```bash
g++ -std=c++17 main.cpp -o ecommerce
./ecommerce
```

## What I practiced

- Strategy pattern for interchangeable payment methods
- A custom exception hierarchy (`ECommerceException` and its subclasses)
- File I/O and input validation

> Coursework (April 2025).
