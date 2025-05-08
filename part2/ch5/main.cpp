#include <array>
#include <cassert>
#include <cstring>
#include <cxxabi.h>
#include <iostream>
#include <ostream>
#include <print>
#include <string>
#include <type_traits>
#include <vector>

// Type Traits and Conditional Compilation

#pragma GCC diagnostic ignored "-Wunused"
#pragma GCC diagnostic ignored "-Wunused-parameter"

std::string
demangle(const char *mangled_name)
{
    std::string result;
    std::size_t len    = 0;
    int         status = 0;
    char       *ptr    = __cxxabiv1::__cxa_demangle(mangled_name, nullptr, &len, &status);

    if (status == 0)
    {
        result = ptr;
    }
    else
    {
        result = "demangle error";
    }

    ::free(ptr);

    return result;
}

// Understanding and defining type traits

namespace n501
{
    template <typename T>
    struct is_floating_point
    {
        static const bool value = false;
    };

    template <>
    struct is_floating_point<float>
    {
        static const bool value = true;
    };

    template <>
    struct is_floating_point<double>
    {
        static const bool value = true;
    };

    template <>
    struct is_floating_point<long double>
    {
        static const bool value = true;
    };

    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

    template <typename T>
    void
    process_real_number(T const value)
    {
        // only allow this function if T is a floating point type
        static_assert(is_floating_point_v<T>);

        std::println("processing a real number: {}", value);
    }
} // namespace n501

namespace n502
{
    using namespace std;

    // widget

    struct widget
    {
        int    id;
        string name;

        ostream &
        write(ostream &os) const
        {
            os << id << ',' << name << '\n';
            return os;
        }
    };

    // gadget

    struct gadget
    {
        int    id;
        string name;

        friend ostream &operator<<(ostream &os, gadget const &o);
    };

    ostream &
    operator<<(ostream &os, gadget const &o)
    {
        os << o.id << ',' << o.name << '\n';
        return os;
    }

    // uses_write

    template <typename T>
    struct uses_write
    {
        static constexpr bool value = false;
    };

    template <>
    struct uses_write<widget>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool uses_write_v = uses_write<T>::value;

    // serializer

    template <bool>
    struct serializer
    {
        template <typename T>
        static void
        serialize(ostream &os, T const &value)
        {
            os << value;
        }
    };

    template <>
    struct serializer<true>
    {
        template <typename T>
        static void
        serialize(ostream &os, T const &value)
        {
            value.write(os);
        }
    };

    // API

    template <typename T>
    void
    serialize(ostream &os, T const &value)
    {
        serializer<uses_write_v<T>>::serialize(os, value);
    }
} // namespace n502

// Exploring SFINAE and its purpose

namespace n503
{
    namespace
    {
        // begin

        template <typename T>
        auto
        begin(T &c)
        {
            return c.begin();
        }

        template <typename T, size_t N>
        T *
        begin(T (&arr)[N])
        {
            return arr;
        }
    } // namespace

    namespace
    {
        // increment

        template <typename T>
        void
        increment(T &val)
        {
            // Substituting for T any type for which the post-fix operator++ is not implemented is a failure.
            // This failure is an error and will not be ignored by the compiler!
            val++;
        }
    } // namespace

    namespace
    {
        // handle

        // e.g. char (*)[N % 2 == 0] either creates char (*)[true] = char(*)[1] or char (*)[false] = char(*)[0].
        // The latter - array of size 0 - is a SFINAE error.
        // See https://en.cppreference.com/w/cpp/language/sfinae (lists type errors that are SFINAE errors)
        //
        // `= nullptr` is a default value

        // char a[0];

        template <typename T, size_t N>
        void
        handle(T (&arr)[N], char (*)[N % 2 == 0] = nullptr) // accepts array with even number of elements
        {
            std::println("handle even array: {} elements", N);
        }

        template <typename T, size_t N>
        void
        handle(T (&arr)[N], char (*)[N % 2 == 1] = nullptr) // accepts array with odd number of elements
        {
            std::println("handle odd array: {} elements", N);
        }
    } // namespace
} // namespace n503

namespace n504
{
    namespace
    {
        // foo

        template <typename T>
        struct foo
        {
            using foo_type = T;
        };

        struct int_foo : foo<int>
        {
            // ...
        };
    } // namespace

    namespace
    {
        // bar

        template <typename T>
        struct bar
        {
            using bar_type = T;
        };

        struct int_bar : bar<int>
        {
            // ...
        };
    } // namespace

    namespace
    {
        // handle

        template <typename T>
        decltype(typename T::foo_type(), void()) // only accept types with an inner foo_type, return void
        handle(T const &)
        {
            std::println("handle a foo");
        }

        template <typename T>
        decltype(typename T::bar_type(), void()) // only accept types with an inner bar_type, return void
        handle(T const &)
        {
            std::println("handle a bar");
        }
    } // namespace
} // namespace n504

// Enabling SFINAE with the enable_if type trait
//
// See attached document "Below report - Type Traits SFINAE and Compile-Time Reflection in C++14.pdf"
// for explanation of `enable_if`.

namespace n505
{
    template <bool B, typename T = void>
    struct enable_if
    {
    };

    // only for true we have a `type
    template <typename T>
    struct enable_if<true, T>
    {
        using type = T;
    };

    template <bool B, typename T = void>
    using enable_if_t = enable_if<B, T>::type;

    // C++ allows unnamed function parameters (same for template parameters), e.g. void func(int * = nullptr);

    template <typename T, enable_if_t<n502::uses_write_v<T>> * = nullptr>
    void
    serialize(std::ostream &os, T const &value)
    {
        value.write(os);
    }

    template <typename T, enable_if_t<not n502::uses_write_v<T>> * = nullptr>
    void
    serialize(std::ostream &os, T const &value)
    {
        os << value;
    }
} // namespace n505

namespace n506
{
    // specify the return type of a function

    template <typename T>
    typename std::enable_if_t<n502::uses_write_v<T>> // returns void (the default in enable_if)
    serialize(std::ostream &os, T const &value)
    {
        value.write(os);
    }

    template <typename T>
    typename std::enable_if_t<not n502::uses_write_v<T>> // returns void (the default in enable_if)
    serialize(std::ostream &os, T const &value)
    {
        os << value;
    }
} // namespace n506

namespace n507
{
    // define a function parameter that has a default argument

    template <typename T>
    void
    serialize(std::ostream &os, T const &value, typename std::enable_if_t<n502::uses_write_v<T>> * = nullptr)
    {
        value.write(os);
    }

    template <typename T>
    void
    serialize(std::ostream &os, T const &value, typename std::enable_if_t<not n502::uses_write_v<T>> * = nullptr)
    {
        os << value;
    }
} // namespace n507

namespace n508
{
    // Default template arguments are not part of a function template’s signature!
    //
    //                  anonymous type variable with a default type value (void)
    //                    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    struct integral_wrapper
    {
        T value;

        integral_wrapper(T v) : value(v)
        {
        }
    };

    template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    struct floating_wrapper
    {
        T value;

        floating_wrapper(T v) : value(v)
        {
        }
    };
} // namespace n508

// Using constexpr if

namespace n509
{
    template <typename T>
    void
    serialize(std::ostream &os, T const &value)
    {
        if constexpr (n502::uses_write_v<T>)
        {
            value.write(os);
        }
        else
        {
            os << value;
        }
    }
} // namespace n509

namespace n510
{
    template <unsigned int n>
    constexpr unsigned int
    factorial()
    {
        if constexpr (n < 2)
        {
            return 1;
        }
        else
        {
            return n * factorial<n - 1>();
        }
    }
} // namespace n510

namespace n511
{
    template <typename T>
    constexpr bool
    are_equal(T const &a, T const &b)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::abs(a - b) < 0.001; // epsilon calculation
        }
        else
        {
            return a == b;
        }
    }
} // namespace n511

// Exploring the standard type traits

namespace n512
{
    template <typename T>
    void
    f()
    {
        if constexpr (std::is_arithmetic_v<T>) // integral or a floating-point type
        {
            // ...
        }
        else
        {
            static_assert(false);
        }
    }
} // namespace n512

namespace n513
{
    template <typename T>
    std::string
    as_string(T value)
    {
        if constexpr (std::is_null_pointer_v<T>)
        {
            return "null";
        }
        else if constexpr (std::is_arithmetic_v<T>) // integral or a floating-point type
        {
            return std::to_string(value);
        }
        else
        {
            static_assert(false);
        }
    }
} // namespace n513

namespace n514
{
    struct foo
    {
        int a;
    };

    struct bar
    {
        int a = 0;
    };

    struct tar
    {
        int a;

        tar() : a(0)
        {
        }
    };
} // namespace n514

namespace n515
{
    // passing by value

    template <typename T>
    std::string
    as_string(T value)
    {
        if constexpr (std::is_null_pointer_v<T>)
        {
            return "null";
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return value ? "true" : "false";
        }
        else if constexpr (std::is_arithmetic_v<T>)
        {
            return std::to_string(value);
        }
        else
        {
            static_assert(false);
        }
    }
} // namespace n515

namespace n516
{
    // passing by r-value

    template <typename T>
    std::string
    as_string(T &&value)
    {
        // For debugging:
        // std::cout << __PRETTY_FUNCTION__ << '\n';

        if constexpr (std::is_null_pointer_v<T>)
        {
            return "null";
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return value ? "true" : "false";
        }
        else if constexpr (std::is_arithmetic_v<T>)
        {
            return std::to_string(value);
        }
        else
        {
            // For debugging:
            // return "true";

            static_assert(false);
        }
    }
} // namespace n516

namespace n517
{
    // passing by r-value

    template <typename T>
    std::string
    as_string(T &&value)                               // passing l-value or r-value
    {
        using value_type = std::remove_reference_t<T>; // stripping reference
        // using value_type = std::decay_t<T>;         // stripping reference, and cv-qualifiers

        if constexpr (std::is_null_pointer_v<value_type>)
        {
            return "null";
        }
        else if constexpr (std::is_same_v<value_type, bool>)
        {
            return value ? "true" : "false";
        }
        else if constexpr (std::is_arithmetic_v<value_type>)
        {
            return std::to_string(value);
        }
        else
        {
            static_assert(false);
        }
    }
} // namespace n517

namespace n518
{
    template <typename T, size_t S>
    using list_t = std::conditional_t<S == 1, T, std::vector<T>>;
}

namespace n520
{
    namespace detail
    {
        template <bool b>
        struct copy_fn
        {

            template <typename InputIt, typename OutputIt>
            constexpr static OutputIt
            copy(InputIt first, InputIt last, OutputIt d_first)
            {
                std::cout << "Generic Copy" << '\n';

                while (first != last)
                {
                    *d_first++ = *first++;
                }
                return d_first;
            }
        };

        template <>
        struct copy_fn<true>
        {
            template <typename InputIt, typename OutputIt>
            constexpr static OutputIt *
            copy(InputIt *first, InputIt *last, OutputIt *d_first)
            {
                std::cout << "Memory Copy" << '\n';

                std::memmove(d_first, first, (last - first) * sizeof(InputIt));
                return d_first + (last - first);
            }
        };
    } // namespace detail

    template <typename InputIt, typename OutputIt>
    constexpr OutputIt
    copy(InputIt first, InputIt last, OutputIt d_first)
    {
        using input_type  = std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>;
        using output_type = std::remove_cv_t<typename std::iterator_traits<OutputIt>::value_type>;

        constexpr bool opt = std::is_same_v<input_type, output_type> && std::is_pointer_v<InputIt> &&
                             std::is_pointer_v<OutputIt> && std::is_trivially_copy_assignable_v<input_type>;

        return detail::copy_fn<opt>::copy(first, last, d_first);
    }
} // namespace n520

namespace n519
{
    namespace
    {
        template <typename, typename... Ts>
        struct has_common_type : std::false_type
        {
        };

        template <typename... Ts>
        struct has_common_type<std::void_t<std::common_type_t<Ts...>>, Ts...> : std::true_type
        {
        };

        template <typename... Ts>
        constexpr bool has_common_type_v = sizeof...(Ts) < 2 || has_common_type<void, Ts...>::value;
    } // namespace

    template <typename... Ts, typename = std::enable_if_t<has_common_type_v<Ts...>>>
    void
    process(Ts &&...ts)
    {
        // instead of enable_if we could use static_assert
        // static_assert(has_common_type_v<Ts...>, "Arguments must have a common type.");

        std::println("{}", demangle(typeid(std::common_type_t<Ts...>).name()));
    }
} // namespace n519

int
main()
{
    {
        std::println("\n====================== using namespace n501 =============================");
        using namespace n501;

        static_assert(is_floating_point<float>::value);
        static_assert(is_floating_point<double>::value);
        static_assert(is_floating_point<long double>::value);

        static_assert(is_floating_point_v<float>);
        static_assert(is_floating_point_v<double>);
        static_assert(is_floating_point_v<long double>);

        process_real_number(42.0);  // processing a real number: 42
        process_real_number(42.0f); // processing a real number: 42
        process_real_number(42.0L); // processing a real number: 42

        // process_real_number(42);   // error: int (42) is not a floating point number
        // process_real_number(true); // error: bool (true) is not a floating point number

        // non-floating points
        static_assert(!is_floating_point<int>::value);
        static_assert(!is_floating_point<bool>::value);
    }

    {
        std::println("\n====================== using namespace n502 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        // widget uses write()
        w.write(std::cout); // 1, one

        // gadnget does not use write()
        // g.write(std::cout);   // error: No member named 'write' in 'n502::gadget'

        // gadget uses <<
        std::cout << g; // 2, two

        // both use the serialize API (are treated the same way)
        serialize(std::cout, w); // 1, one
        serialize(std::cout, g); // 2, two
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        // SFINAE = Substitution Failure Is Not An Error

        // SFINAE stands for Substitution Failure Is Not An Error. When the compiler encounters
        // the use of a function template, it substitutes the arguments in order to instantiate the
        // template. If an error occurs at this point, it is NOT REGARDED AS ILL-INFORMED CODE, only as
        // a DEDUCTION FAILURE. The function is removed from the overload set instead of causing an
        // error. Only if there is no match in the overload set does an error occur.
        //
        // Could also be called: Matching Failure Is Not An Error:
        // no match -> ignored (no error)
        //    match -> add to function overload set

        // C++ array
        std::array arr1{1, 2, 3, 4, 5};
        std::println("{}", *n503::begin(arr1)); // 1

        // C-style array
        int arr2[]{5, 4, 3, 2, 1};
        std::println("{}", *n503::begin(arr2)); // 5
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        int a = 42;
        n503::increment(a); // OK

        std::string s{"42"};
        // error: Cannot increment value of type 'std::basic_string<char>'
        // Error shows up in the template, not here.
        // n503::increment(s);
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        // Creating an array of size zero is a SFINAE error.

        int arr1[]{1, 2, 3, 4, 5}; //
        handle(arr1);              // handle odd array: 5 elements

        int arr2[]{1, 2, 3, 4};    //
        handle(arr2);              // handle even array: 4 elements
    }

    {
        std::println("\n====================== using namespace n504 =============================");
        using namespace n504;

        int_foo i_foo;
        int_bar i_bar;

        int x = 0;

        handle(i_foo);         // handle a foo
        handle(i_bar);         // handle a bar

        std::println("{}", x); // 0
        // handle(x);          // error: doesn't have a foo_type or bar_type
    }

    {
        std::println("\n====================== using namespace n505 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n505::serialize(std::cout, w); // 1,one
        n505::serialize(std::cout, g); // 2,two
    }

    {
        std::println("\n====================== using namespace n506 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n506::serialize(std::cout, w); // 1,one
        n506::serialize(std::cout, g); // 2,two
    }

    {
        std::println("\n====================== using namespace n507 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n507::serialize(std::cout, w); // 1,one
        n507::serialize(std::cout, g); // 2,two
    }

    {
        std::println("\n====================== using namespace n508 =============================");
        using namespace n508;

        integral_wrapper w1{42};   // OK
        floating_wrapper w2{42.0}; // OK

        // integral_wrapper w3{42.0}; // error
        // integral_wrapper w4{"42"}; // error

        // floating_wrapper w5{42};   // error
        // floating_wrapper w6{"42"}; // error
    }

    {
        std::println("\n====================== using namespace n509 =============================");
        using namespace n502;

        // 'constexpr if' is a compile-time version of the if-statement.
        // The syntax for 'constexpr if' is 'if constexpr(condition)'.

        widget w{1, "one"};
        gadget g{2, "two"};

        n509::serialize(std::cout, w); // 1,one
        n509::serialize(std::cout, g); // 2,two
    }

    {
        std::println("\n====================== using namespace n510 =============================");
        using namespace n510;

        std::println("{}", factorial<1>()); // 1
        std::println("{}", factorial<2>()); // 2
        std::println("{}", factorial<3>()); // 6
        std::println("{}", factorial<4>()); // 24
        std::println("{}", factorial<5>()); // 120
    }

    {
        std::println("\n====================== using namespace n511 =============================");
        using namespace n511;

        std::println("{}", are_equal(1, 1));                               // true
        std::println("{}", are_equal(1.999998, 1.999997));                 // true
        std::println("{}", are_equal(1.999998, 2.999997));                 // false
        std::println("{}", are_equal(std::string{"1"}, std::string{"1"})); // true
        // are_equal(n502::widget{1, "one"}, n502::widget{1, "two"});      // error
    }

    {
        std::println("\n====================== using namespace n512 =============================");
        using namespace n512;

        f<int>();
        f<double>();
        // f<n502::widget>();
    }

    {
        std::println("\n====================== using namespace n513 =============================");
        using namespace n513;

        std::println("{}", as_string(nullptr)); // null
        std::println("{}", as_string(true));    // 1
        std::println("{}", as_string('a'));     // 97
        std::println("{}", as_string(42));      // 42
        std::println("{}", as_string(42.0));    // 42.000000
        // std::println("{}", as_string("42")); // error
    }

    {
        std::println("\n====================== using namespace n514 =============================");
        using namespace n514;

        std::println("{}", std::is_trivial_v<foo>);            // true
        std::println("{}", std::is_trivial_v<bar>);            // false
        std::println("{}", std::is_trivial_v<tar>);            // false

        std::println("{}", std::is_trivially_copyable_v<foo>); // true
        std::println("{}", std::is_trivially_copyable_v<bar>); // true
        std::println("{}", std::is_trivially_copyable_v<tar>); // true
    }

    {
        std::println("\n====================== using namespace n515 =============================");
        using namespace n515;

        std::println("{}", as_string(nullptr)); // null
        std::println("{}", as_string(true));    // true
        std::println("{}", as_string('a'));     // 97
        std::println("{}", as_string(42));      // 42
        std::println("{}", as_string(42.0));    // 42.000000
    }

    {
        std::println("\n====================== using namespace n516 =============================");
        using namespace n516;

        // args are all r-values
        std::println("{}", as_string(nullptr)); // null
        std::println("{}", as_string(true));    // true
        std::println("{}", as_string('a'));     // 97
        std::println("{}", as_string(42));      // 42
        std::println("{}", as_string(42.0));    // 42.000000

        // l-values cause problems (they are called as l-value references)
        // bool f = true;
        // std::println("{}", as_string(f)); // T = bool &

        // int n = 42;
        // std::println("{}", as_string(n)); // T = int &
    }

    {
        std::println("\n====================== using namespace n517 =============================");
        using namespace n517;

        std::println("{}", as_string(nullptr)); // null
        std::println("{}", as_string(true));    //  true
        std::println("{}", as_string('a'));     //  97
        std::println("{}", as_string(42));      //  42
        std::println("{}", as_string(42.0));    //  42.000000

        bool f = true;
        std::println("{}", as_string(f));       //  true

        int n = 42;
        std::println("{}", as_string(n));       //  42
    }

    {
        std::println("\n====================== using namespace n518 =============================");
        using namespace n518;

        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }

    {
        std::println("\n====================== using namespace n520 =============================");

        // Without the n520 qualification, a process called ARGUMENT-DEPENDENT LOOKUP (ADL) would
        // kick in. This would result in resolving the call to copy to the std::copy function
        // because the arguments we pass are found in the std namespace!

        std::vector<int> v1{1, 2, 3, 4, 5};
        std::vector<int> v2(5);

        n520::copy(std::begin(v1), std::end(v1), std::begin(v2)); // Generic Copy

        assert(v1[0] == v2[0]);
        assert(v1[1] == v2[1]);
        assert(v1[2] == v2[2]);
        assert(v1[3] == v2[3]);
        assert(v1[4] == v2[4]);

        int a1[5] = {1, 2, 3, 4, 5};
        int a2[5];

        n520::copy(a1, a1 + 5, a2); // Memory Copy
        assert(a1[0] == a2[0]);
        assert(a1[1] == a2[1]);
        assert(a1[2] == a2[2]);
        assert(a1[3] == a2[3]);
        assert(a1[4] == a2[4]);
    }

    {
        std::println("\n====================== using namespace n519 =============================");
        using namespace n519;

        int a = 1;
        process(a);

        process(1);           // int
        process(1, 2, 3);     // int
        process(1, 2.0, '3'); // double

        // process(1, 2.0, "3"); // error
    }
}
