//
// Created by wangrl2016 on 2023/3/27.
//

#pragma once

namespace base {
    // Noncopyable is the base class for objects that do not want to
    // be copied. It hides its copy-constructor and its assigment-operator.
    class Noncopyable {
    public:
        Noncopyable() = default;

        Noncopyable(Noncopyable&&) = default;

        Noncopyable& operator=(Noncopyable&&) = default;

        Noncopyable(const Noncopyable&) = delete;

        Noncopyable& operator=(const Noncopyable&) = delete;
    };
}
