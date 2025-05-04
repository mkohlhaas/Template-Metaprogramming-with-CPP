#include "wrapper.h"
#include <print>

// Explicit instantiation definition

namespace ext
{
    // Explicit instantiation definition is introduced with the `template` keyword
    // but not followed by any parameter list.
    template struct wrapper<int>;

    // When you do explicit template declarations, keep in mind that a class member function
    // that is defined within the body of the class is always considered inline and therefore it
    // will always be instantiated. Therefore, you can only use the extern keyword for member
    // functions that are defined outside of the class body.

    void
    f()
    {
        ext::wrapper<int> a{42};
        std::println("{}", a.data);
    }
} // namespace ext

using namespace ext; //               // won't help finding ext for template instantiation
// template struct wrapper<double>;   // error: Explicit instantiation of 'ext::wrapper' must occur in namespace 'ext'

template struct ext::wrapper<double>; // we need explicit namespace specifier
