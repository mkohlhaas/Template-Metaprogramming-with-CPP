#pragma once

namespace ext
{
    template <typename T>
    struct wrapper
    {
        T data;
    };

    extern template struct wrapper<int>;

    void f();
    void g();
} // namespace ext
