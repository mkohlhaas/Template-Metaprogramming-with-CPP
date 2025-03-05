#include "wrapper.h"
#include <print>

namespace ext
{
    void
    g()
    {
        // wrapper<int> has been instantiated in source1.cpp
        wrapper<int> a{100};
        std::println("{}", a.data);
    }
} // namespace ext
