/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_INTERVAL_HPP
#define INONEWEEKEND_INCLUDE_INTERVAL_HPP

#include <concepts>
#include "util.hpp"

template <std::floating_point T = double>
class Interval
{
public:
    // Default interval is empty
    constexpr Interval() : Interval(infinity<T>, -infinity<T>) {}
    constexpr Interval(T min, T max) : m_min(min), m_max(max) {}

    constexpr T min() const { return m_min; }
    constexpr T max() const { return m_max; }

    constexpr T size() const { return m_max - m_min; }

    constexpr bool isEmpty() const
    {
        return m_min > m_max;
    }

    constexpr bool contains(const T value) const
    {
        return (value >= m_min) && (value <= m_max);
    }

    constexpr bool surrounds(const T value) const
    {
        return (value > m_min) && (value < m_max);
    }

    static constexpr Interval<T> empty()
    {
        return s_empty;
    }

    static constexpr Interval<T> universe()
    {
        return s_universe;
    }

private:
    T m_min;
    T m_max;

    static constexpr Interval<T> s_empty = Interval<T>(infinity<T>, -infinity<T>);
    static constexpr Interval<T> s_universe = Interval<T>(-infinity<T>, infinity<T>);
};

#endif /* INONEWEEKEND_INCLUDE_INTERVAL_HPP */
