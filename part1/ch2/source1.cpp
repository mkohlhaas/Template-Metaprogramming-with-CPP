#include "wrapper.h"
#include <print>

namespace ext
{
    template struct wrapper<int>;

    void
    f()
    {
        ext::wrapper<int> a{42};

        std::println("{}", a.data);
    }
} // namespace ext
