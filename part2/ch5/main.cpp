#include <array>
#include <cstring>
#include <iostream>
#include <ostream>
#include <print>
#include <string>
#include <vector>

template <class T>
constexpr bool always_false = std::false_type::value;

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
    void
    process_real_number(T const value)
    {
        static_assert(is_floating_point<T>::value);

        std::println("processing a real number: {}", value);
    }
} // namespace n501

namespace n502
{
    using namespace std;

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

    // class template with anonymous template parameter
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

    template <typename T>
    void
    serialize(ostream &os, T const &value)
    {
        serializer<uses_write_v<T>>::serialize(os, value);
    }
} // namespace n502

namespace n503
{
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

    template <typename T>
    void
    increment(T &val)
    {
        // Substituting for T any type for which the post-fix operator++ is not implemented is a failure.
        // This failure is an error and will not be ignored by the compiler!
        val++;
    }

    template <typename T, size_t N>
    void
    handle(T (&arr [[maybe_unused]])[N], char (*)[N % 2 == 0] = nullptr)
    {
        std::println("handle even array: {} elements", N);
    }

    template <typename T, size_t N>
    void
    handle(T (&arr [[maybe_unused]])[N], char (*)[N % 2 == 1] = nullptr)
    {
        std::println("handle odd array: {} elements", N);
    }
} // namespace n503

namespace n504
{
    template <typename T>
    struct foo
    {
        using foo_type = T;
    };

    template <typename T>
    struct bar
    {
        using bar_type = T;
    };

    struct int_foo : foo<int>
    {
        // ...
    };
    struct int_bar : bar<int>
    {
        // ...
    };

    template <typename T>
    decltype(typename T::foo_type(), void())
    handle(T const &)
    {
        std::println("handle a foo");
    }

    template <typename T>
    decltype(typename T::bar_type(), void())
    handle(T const &)
    {
        std::println("handle a bar");
    }
} // namespace n504

namespace n505
{
    template <bool B, typename T = void>
    struct enable_if
    {
    };

    template <typename T>
    struct enable_if<true, T>
    {
        using type = T;
    };

    template <typename T, enable_if<n502::uses_write_v<T>>::type * = nullptr>
    void
    serialize(std::ostream &os, T const &value)
    {
        value.write(os);
    }

    template <typename T, enable_if<!n502::uses_write_v<T>>::type * = nullptr>
    void
    serialize(std::ostream &os, T const &value)
    {
        os << value;
    }
} // namespace n505

namespace n506
{
    template <typename T>
    typename std::enable_if<n502::uses_write_v<T>>::type
    serialize(std::ostream &os, T const &value)
    {
        value.write(os);
    }

    template <typename T>
    typename std::enable_if<!n502::uses_write_v<T>>::type
    serialize(std::ostream &os, T const &value)
    {
        os << value;
    }
} // namespace n506

namespace n507
{
    template <typename T>
    void
    serialize(std::ostream &os, T const &value, typename std::enable_if<n502::uses_write_v<T>>::type * = nullptr)
    {
        value.write(os);
    }

    template <typename T>
    void
    serialize(std::ostream &os, T const &value, typename std::enable_if<!n502::uses_write_v<T>>::type * = nullptr)
    {
        os << value;
    }
} // namespace n507

namespace n508
{
    // anonymous type variable with a default type value
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>, T>>
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
        if constexpr (n > 1)
        {
            return n * factorial<n - 1>();
        }
        else
        {
            return 1;
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
            // epsilon calculation
            return std::abs(a - b) < 0.001;
        }
        else
        {
            return a == b;
        }
    }
} // namespace n511

namespace n512
{
    template <typename T>
    void
    f()
    {
        if constexpr (std::is_arithmetic_v<T>)
        {
            // ...
        }
        else
        {
            static_assert(always_false<T>, "Must be arithmetic");
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
        else if constexpr (std::is_arithmetic_v<T>)
        {
            return std::to_string(value);
        }
        else
        {
            static_assert(always_false<T>);
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
        int a = 0;
        tar() : a(0)
        {
        }
    };
} // namespace n514

namespace n515
{
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
            static_assert(always_false<T>);
        }
    }
} // namespace n515

namespace n516
{
    template <typename T>
    std::string
    as_string(T &&value)
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
            static_assert(always_false<T>);
        }
    }
} // namespace n516

namespace n517
{
    template <typename T>
    std::string
    as_string(T &&value)
    {
        using value_type = std::decay_t<T>; // stripping reference, and cv-qualifiers

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
            static_assert(always_false<T>);
        }
    }
} // namespace n517

namespace n518
{
    template <typename T, size_t S>
    using list_t = typename std::conditional<S == 1, T, std::vector<T>>::type;
}

namespace n519
{
    // anonymous type variable (template parameter)
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

    template <typename... Ts, typename = std::enable_if_t<has_common_type_v<Ts...>>>
    void
    process(Ts &&...ts [[maybe_unused]])
    {
        static_assert(has_common_type_v<Ts...>, "Arguments must have a common type.");

        std::println("{}", typeid(std::common_type_t<Ts...>).name());
    }
} // namespace n519

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

int
main()
{
    {
        std::println("\n====================== using namespace n501 =============================");
        using namespace n501;

        // Type traits:
        // 1. Enable us to INSPECT PROPERTIES of types
        // 2. Perform TRANSFORMATIONS OF TYPES at compile-time,
        //    such as adding or removing the const qualifier, or adding or removing pointer
        //    or reference from a type.
        //    These type traits are also called METAFUNCTIONS.

        // Type traits are small class templates that contain a constant value whose value
        // represents the answer to a question we ask about a type.

        // The technique for building type traits that provide such information about types
        // relies on template specialization.

        // floating points
        static_assert(is_floating_point<float>::value);
        static_assert(is_floating_point<double>::value);
        static_assert(is_floating_point<long double>::value);

        process_real_number(42.0);  // processing a real number: 42
        process_real_number(42.0f); // processing a real number: 42
        process_real_number(42.0L); // processing a real number: 42

        // non-floating points
        static_assert(!is_floating_point<int>::value);
        static_assert(!is_floating_point<bool>::value);

        // process_real_number(42); // error: int (42) is not a floating point number
        // process_real_number(true); // error: bool (true) is not a floating point number
    }

    {
        std::println("\n====================== using namespace n502 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        w.write(std::cout); // 1, one

        // g.write(std::cout);   // error: No member named 'write' in 'n502::gadget'
        std::cout << g;          // 2, two

        serialize(std::cout, w); // 1, one
        serialize(std::cout, g); // 2, two
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        // SFINAE = Substitution Failure Is Not An Error

        std::array<int, 5> arr1{1, 2, 3, 4, 5};
        std::println("{}", *n503::begin(arr1)); // 1

        int arr2[]{5, 4, 3, 2, 1};
        std::println("{}", *begin(arr2));       // 5
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

        int_foo i_f;
        int_bar i_b;
        int     x = 0;

        handle(i_f);           // handle a foo
        handle(i_b);           // handle a bar

        std::println("{}", x); // 0
        // handle(x);          // error
    }

    {
        std::println("\n====================== using namespace n505 =============================");
        using namespace n502;

        // std::enable_if is a metafunction to enable SFINAE
        // and remove candidates from a function's overload set.

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

        // integral_wrapper w3{ 42.0 }; // error
        // integral_wrapper w4{ "42" }; // error

        // floating_wrapper w5{ 42 };   // error
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

        // std::println("{}", std::boolalpha);
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

        std::println("{}", as_string(nullptr)); // null
        std::println("{}", as_string(true));    // true
        std::println("{}", as_string('a'));     // 97
        std::println("{}", as_string(42));      // 42
        std::println("{}", as_string(42.0));    // 42.000000

        // bool f = true;
        // std::println("{}", as_string(f));

        // int n = 42;
        // std::println("{}", as_string(n));
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

        std::vector<int> v1{1, 2, 3, 4, 5};                       // vector of size 5
        std::vector<int> v2(5);                                   // vector of size 5

        n520::copy(std::begin(v1), std::end(v1), std::begin(v2)); // copy v1 into v2

        int a1[5] = {1, 2, 3, 4, 5};                              // array of size 5
        int a2[5];                                                // array of size 5

        n520::copy(a1, a1 + 5, a2);                               // copy a1 into a2
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
