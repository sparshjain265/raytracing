/**
 * MIT License
 *
 * Copyright (c) 2026 Sparsh Jain
 *
 */

#ifndef INONEWEEKEND_INCLUDE_HITTABLE_LIST_HPP
#define INONEWEEKEND_INCLUDE_HITTABLE_LIST_HPP

#include <vector>
#include <concepts>
#include <memory>

#include "hittable.hpp"
#include "interval.hpp"

template <std::floating_point T = double>
class HittableList : public Hittable<T>
{
public:
    HittableList() : m_objects() {}
    HittableList(const std::vector<std::shared_ptr<Hittable<T>>> &objects)
        : m_objects(objects) {}
    HittableList(std::shared_ptr<Hittable<T>> object)
    {
        add(object);
    }

    void add(std::shared_ptr<Hittable<T>> object)
    {
        m_objects.push_back(object);
    }

    void clear()
    {
        m_objects.clear();
    }

    virtual bool hit(
        const Ray<T> &r,
        Interval<T> rayT,
        HitRecord<T> &record) const override
    {
        HitRecord<T> tempRecord;
        bool hitAnything = false;
        T closestSoFar = rayT.max();

        for (const auto &object : m_objects)
        {
            if (object->hit(r, Interval<T>(rayT.min(), closestSoFar), tempRecord))
            {
                hitAnything = true;
                closestSoFar = tempRecord.t();
                record = tempRecord;
            }
        }

        return hitAnything;
    }

private:
    std::vector<std::shared_ptr<Hittable<T>>> m_objects;
};

#endif /* INONEWEEKEND_INCLUDE_HITTABLE_LIST_HPP */
