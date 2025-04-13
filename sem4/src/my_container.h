#include <iostream>


template <class T>
class Container
{
public:
    Container() = default;
    Container(const Container&) = default;
    virtual ~Container() = default;

    // virtual Container& operator=(const Container&) = 0;

    virtual T& operator[](std::size_t) = 0;

    virtual const T& operator[](std::size_t) const = 0;

    [[nodiscard]] virtual std::size_t size() const = 0;

    [[nodiscard]] virtual std::size_t max_size() const = 0;

    [[nodiscard]] virtual bool empty() const = 0;
};