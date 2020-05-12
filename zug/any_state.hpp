//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/meta.hpp>
#include <zug/state_traits.hpp>

#include <stdexcept>
#include <string>

#if ZUG_TRACE_ANY_STATE_ALLOC
#include <iostream>
#endif

namespace zug {

//! @defgroup any_state
//! @{

/*!
 * Polymorphically holds any value implementing the `state_traits`.
 * This type is used for the implementation of `transducer`.
 */
class any_state
{
public:
    any_state() noexcept
        : data_(reinterpret_cast<char*>(&null_holder_storage<>::value))
        , size_(0)
    {}

    ~any_state() noexcept
    {
        if (size_) {
            content()->~holder_base();
            delete[] data_;
        }
    }

    any_state(any_state&& other)
        : data_(other.data_)
        , size_{}
    {
        using std::swap;
        swap(size_, other.size_);
    }

    any_state(const any_state& other)
        : data_(other.size_ ? new char[other.size_] : nullptr)
        , size_(other.size_)
    {
#if ZUG_TRACE_ANY_STATE_ALLOC
        std::cout << "alloc-c" << std::endl;
#endif
        other.content()->clone(data_);
    }

    template <typename ValueType>
    any_state(
        ValueType&& value,
        std::enable_if_t<
            !std::is_base_of<any_state, std::decay_t<ValueType>>::value &&
            !std::is_base_of<meta::bottom, std::decay_t<ValueType>>::value>* =
            0)
        : data_(new char[sizeof(holder<std::decay_t<ValueType>>)])
        , size_(sizeof(holder<std::decay_t<ValueType>>))
    {
        new (data_)
            holder<std::decay_t<ValueType>>(std::forward<ValueType>(value));
#if ZUG_TRACE_ANY_STATE_ALLOC
        std::cout << "alloc-n " << typeid(std::decay_t<ValueType>).name()
                  << std::endl;
#endif
    }

    any_state& operator=(any_state&& other)
    {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        return *this;
    }

    any_state& operator=(const any_state& rhs)
    {
        if (&rhs != this) {
            realloc_(rhs.content()->size());
            rhs.content()->clone(data_);
        }
        return *this;
    }

    template <typename ValueType>
    auto operator=(ValueType&& rhs) -> std::enable_if_t<
        !std::is_base_of<any_state, std::decay_t<ValueType>>::value,
        any_state&>
    {
        realloc_(sizeof(holder<std::decay_t<ValueType>>));
        new (data_)
            holder<std::decay_t<ValueType>>(std::forward<ValueType>(rhs));
        return *this;
    }

    template <typename T>
    std::decay_t<T>& as() &
    {
        return as_impl(meta::pack<std::decay_t<T>>{});
    }

    template <typename T>
    std::decay_t<T>&& as() &&
    {
        return std::move(as_impl(meta::pack<std::decay_t<T>>{}));
    }

    template <typename T>
    const std::decay_t<T>& as() const&
    {
        return const_cast<any_state*>(this)->as_impl(
            meta::pack<std::decay_t<T>>{});
    }

    template <typename T>
    void check() const
    {
        if (!has<T>()) {
            throw std::runtime_error(std::string("Have ") + type().name() +
                                     ", but expect " +
                                     typeid(std::decay_t<T>).name());
        }
    }

    template <typename T>
    bool has() const
    {
        return has_impl(meta::pack<std::decay_t<T>>{});
    }

    const std::type_info& type() const noexcept { return content()->type(); }

private:
    void realloc_(std::size_t size)
    {
        if (size_ > 0)
            content()->~holder_base();
        if (size_ < size) {
            if (size_ > 0)
                delete[] data_;
            data_ = new char[size];
            size_ = size;
#if ZUG_TRACE_ANY_STATE_ALLOC
            std::cout << "alloc-r" << std::endl;
#endif
        }
    }

    template <typename T>
    T& as_impl(meta::pack<T>)
    {
#if ZUG_SAFE_ANY_STATE
        check<T>();
#endif
        return reinterpret_cast<holder<T>*>(data_)->held;
    }

    any_state& as_impl(meta::pack<any_state>) { return *this; }

    template <typename T>
    bool has_impl(meta::pack<T>) const
    {
        return content()->type() == typeid(T);
    }

    bool has_impl(meta::pack<any_state>) const { return true; }

    friend struct state_traits<any_state>;

    struct holder_base
    {
        virtual ~holder_base()                              = default;
        virtual const std::type_info& type() const noexcept = 0;
        virtual void clone(char* data) const                = 0;
        virtual void move(char* data) const                 = 0;
        virtual any_state complete() const                  = 0;
        virtual bool is_reduced() const                     = 0;
        virtual any_state unwrap() const                    = 0;
        virtual any_state unwrap_all() const                = 0;
        virtual any_state rewrap(any_state) const           = 0;
        virtual any_state data() const                      = 0;
        virtual std::size_t size() const                    = 0;
    };

    template <typename T>
    struct holder : holder_base
    {
        T held;

        template <typename TT>
        holder(TT&& x)
            : held(std::forward<TT>(x))
        {}
        const std::type_info& type() const noexcept override
        {
            return typeid(T);
        }
        void clone(char* data) const override { new (data) holder<T>(held); }
        void move(char* data) const override
        {
            new (data) holder<T>(std::move(held));
        }
        any_state complete() const override { return state_complete(held); }
        bool is_reduced() const override { return state_is_reduced(held); }
        any_state unwrap() const override { return state_unwrap(held); }
        any_state unwrap_all() const override { return state_unwrap_all(held); }
        any_state rewrap(any_state x) const override
        {
            return state_rewrap(held, std::move(x));
        }
        any_state data() const override
        {
            return state_data(held, [] { return any_state{}; });
        }
        std::size_t size() const override { return sizeof(T); }
    };

    struct null_holder : holder_base
    {
        virtual ~null_holder() = default;

        const std::type_info& type() const noexcept override
        {
            return typeid(void);
        }

        void clone(char* data) const override { new (data) null_holder; }
        void move(char* data) const override { new (data) null_holder; }
        any_state complete() const override { return {}; }
        bool is_reduced() const override { return false; }
        any_state unwrap() const override { return {}; }
        any_state unwrap_all() const override { return {}; }
        any_state rewrap(any_state x) const override { return x; }
        any_state data() const override { return {}; }
        std::size_t size() const override { return 0; }
    };

    template <typename Dummy = void>
    struct null_holder_storage
    {
        static null_holder value;
    };

    holder_base* content() const
    {
        return reinterpret_cast<holder_base*>(data_);
    }

    char* data_;
    std::size_t size_;
};

template <typename Dummy>
any_state::null_holder any_state::null_holder_storage<Dummy>::value{};

template <>
struct state_traits<any_state>
{
    template <typename T>
    static decltype(auto) complete(T&& t)
    {
        return std::forward<T>(t).content()->complete();
    }

    template <typename T>
    static decltype(auto) is_reduced(T&& t)
    {
        return std::forward<T>(t).content()->is_reduced();
    }

    template <typename T>
    static decltype(auto) unwrap(T&& t)
    {
        return std::forward<T>(t).content()->unwrap();
    }

    template <typename T>
    static decltype(auto) unwrap_all(T&& t)
    {
        return std::forward<T>(t).content()->unwrap_all();
    }

    template <typename T, typename U>
    static decltype(auto) rewrap(T&& t, U&& x)
    {
        return std::forward<T>(t).content()->rewrap(std::forward<U>(x));
    }

    template <typename T, typename D>
    static auto data(T&& t, D&& d)
        -> std::decay_t<decltype(std::forward<D>(d)())>
    {
        using data_t = std::decay_t<decltype(std::forward<D>(d)())>;
        auto data    = t.content()->data();
        return data.template has<data_t>() ? data.template as<data_t>()
                                           : std::forward<D>(d)();
    }
};

//! @}

} // namespace zug
