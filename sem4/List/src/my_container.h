#pragma once
#include <iostream>


template <class T>
class Container
{
public:
    Container() = default;
    Container(const Container&) = default;
    virtual ~Container() = default;

    [[nodiscard]] virtual std::size_t size() const = 0;

    [[nodiscard]] virtual std::size_t max_size() const = 0;

    [[nodiscard]] virtual bool empty() const = 0;
};