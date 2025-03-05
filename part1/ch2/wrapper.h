#pragma once

// see source1.cpp
// and source2.cpp

namespace ext
{
    template <typename T>
    struct wrapper
    {
        T data;
    };

    // Explicit Instantiation Declaration
    // Same as explicit instantiation plus extern keyword.
    // Only one source file will have an instantiation!
    // Tells the compiler not to generate definitions for this instantiation when a source file (translation unit)
    // including this header is compiled.
    extern template struct wrapper<int>;

    void f();
    void g();
} // namespace ext
