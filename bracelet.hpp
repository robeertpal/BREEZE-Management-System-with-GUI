#pragma once
#include <iostream>
#include "fee_strategy.hpp" // adaugă aici!

template <typename T>
class Bracelet {
private:
    T balance;
    const FeeStrategy<T>* feeStrategy; // pointer la strategia de fee

public:
    Bracelet(T balance_ = T{}, const FeeStrategy<T>* fee_ = nullptr)
        : balance(balance_), feeStrategy(fee_) {}

    void setFeeStrategy(const FeeStrategy<T>* strategy) {
        feeStrategy = strategy;
    }

    void show() const {
        std::cout << "Sold actual: " << balance << " lei\n";
    }

    void topUp(T amount) {
        T fee = feeStrategy ? feeStrategy->calculateFee(amount) : T(0);
        balance += (amount - fee);
        std::cout << "Incarcare: +" << amount << " lei, comision: " << fee
                  << " lei. Sold nou: " << balance << " lei.\n";
    }

    void pay(T amount) {
        T fee = feeStrategy ? feeStrategy->calculateFee(amount) : T(0);
        T total = amount + fee;
        if (balance < total) {
            throw std::runtime_error("Fonduri insuficiente!");
        }
        balance -= total;
        std::cout << "Plată: -" << amount << " lei, comision: " << fee
                  << " lei. Sold ramas: " << balance << " lei\n";
    }

    T getBalance() const { return balance; }

    // Friend/functie liberă template pentru comparație solduri
    template <typename U>
    friend bool braceletBalanceEqual(const Bracelet<U>& b1, const Bracelet<U>& b2);
};
