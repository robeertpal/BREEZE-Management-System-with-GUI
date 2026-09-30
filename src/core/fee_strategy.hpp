#pragma once

template<typename T>
class FeeStrategy {
public:
    virtual T calculateFee(T amount) const = 0;
    virtual ~FeeStrategy() = default;
};

template<typename T>
class StandardFee : public FeeStrategy<T> {
public:
    T calculateFee(T amount) const override {
        return amount * static_cast<T>(0.10); // 10%
    }
};

template<typename T>
class MGFee : public FeeStrategy<T> {
public:
    T calculateFee(T amount) const override {
        return amount * static_cast<T>(0.05); // 5%
    }
};