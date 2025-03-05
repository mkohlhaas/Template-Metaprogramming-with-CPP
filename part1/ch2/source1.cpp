#include "wrapper.h"
#include <print>

namespace ext
{
    // explicit instantiation
    template struct wrapper<int>;

    void
    f()
    {
        ext::wrapper<int> a{42};
        std::println("{}", a.data);
    }
} // namespace ext
