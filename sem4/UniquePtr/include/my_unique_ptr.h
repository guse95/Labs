#pragma once
#include <memory>

namespace my_smart_ptr {
    template <class T, class Deleter = std::default_delete<T>>
    class UniquePtr {
    protected:
        T* ptr = nullptr;
        Deleter deleter;
    public:
        constexpr UniquePtr() noexcept : ptr(nullptr) {}
        constexpr explicit UniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}
        explicit UniquePtr(T* ptr_) : ptr(ptr_) {}
        UniquePtr(T* ptr_, Deleter deleter_) : ptr(ptr_), deleter(deleter_) {}

        UniquePtr(UniquePtr& other) = delete;

        UniquePtr(UniquePtr&& other) noexcept : ptr(std::move(other.ptr)), deleter(other.deleter) {
            other.ptr = nullptr;
        }
        UniquePtr(UniquePtr&& other, Deleter deleter_) noexcept : ptr(other.ptr), deleter(deleter_) {
            other.ptr = nullptr;
        }
        ~UniquePtr() {
            if (ptr) {
                deleter(ptr);
            }
        }

        UniquePtr& operator=(UniquePtr& other) = delete;

        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (ptr) {
                deleter(ptr);
            }
            ptr = other.ptr;
            other.ptr = nullptr;
            return *this;
        }

        T* get() const noexcept {
            return ptr;
        }

        T& operator*() const noexcept {
            return *ptr;
        }

        T* operator->() const noexcept {
            return ptr;
        }

        explicit operator bool() const noexcept {
            return ptr != nullptr;
        }

        void swap(UniquePtr& other) noexcept {
            std::swap(ptr, other.ptr);
        }

        T* release() noexcept {
            T* tmp = ptr;
            ptr = nullptr;
            return tmp;
        }

        void reset(T* ptr_ = nullptr) noexcept {
            deleter(ptr);
            ptr = ptr_;
        }
    };

    template <class T, class Deleter>
    class UniquePtr<T[], Deleter> {
    protected:
        T* ptr = nullptr;
        Deleter deleter;
    public:
        constexpr UniquePtr() noexcept : ptr(nullptr) {}
        constexpr explicit UniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}
        explicit UniquePtr(T* ptr_) : ptr(ptr_) {}
        UniquePtr(T* ptr_, Deleter deleter_) : ptr(ptr_), deleter(deleter_) {}

        UniquePtr(UniquePtr& other) = delete;

        UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr), deleter(other.deleter) {
            other.ptr = nullptr;
        }
        UniquePtr(UniquePtr&& other, Deleter deleter_) noexcept : ptr(other.ptr), deleter(deleter_) {
            other.ptr = nullptr;
        }

        ~UniquePtr() {
            if (ptr) {
                deleter(ptr);
            }
        }

        UniquePtr& operator=(UniquePtr& other) = delete;

        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (ptr) {
                deleter(ptr);
            }
            ptr = other.ptr;
            other.ptr = nullptr;
            return *this;
        }

        T* get() const noexcept {
            return ptr;
        }

        T& operator[](std::size_t i) const noexcept {
            return ptr[i];
        }

        T& operator*() const noexcept {
            return *ptr;
        }

        T* operator->() const noexcept {
            return ptr;
        }

        explicit operator bool() const noexcept {
            return ptr != nullptr;
        }

        void swap(UniquePtr& other) noexcept {
            std::swap(ptr, other.ptr);
        }

        T* release() noexcept {
            T* tmp = ptr;
            ptr = nullptr;
            return tmp;
        }

        void reset(T* ptr_ = nullptr) noexcept {
            deleter(ptr);
            ptr = ptr_;
        }
    };
}