#include "wrapper.h"
#include <print>

namespace ext
{
    void
    g()
    {
        wrapper<int> a{100};

        std::println("{}", a.data);
    }
} // namespace ext
