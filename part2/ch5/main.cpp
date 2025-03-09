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
    struct widget
    {
        int         id;
        std::string name;

        std::ostream &
        write(std::ostream &os) const
        {
            os << id << ',' << name << '\n';
            return os;
        }
    };

    struct gadget
    {
        int         id;
        std::string name;

        friend std::ostream &operator<<(std::ostream &os, gadget const &o);
    };

    std::ostream &
    operator<<(std::ostream &os, gadget const &o)
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

    template <bool>
    struct serializer
    {
        template <typename T>
        static void
        serialize(std::ostream &os, T const &value)
        {
            os << value;
        }
    };

    template <>
    struct serializer<true>
    {
        template <typename T>
        static void
        serialize(std::ostream &os, T const &value)
        {
            value.write(os);
        }
    };

    template <typename T>
    void
    serialize(std::ostream &os, T const &value)
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
        val++;
    }

    template <typename T, size_t N>
    void
    handle(T (&arr [[maybe_unused]])[N], char (*)[N % 2 == 0] = 0)
    {
        std::println("handle even array");
    }

    template <typename T, size_t N>
    void
    handle(T (&arr [[maybe_unused]])[N], char (*)[N % 2 == 1] = 0)
    {
        std::println("handle odd array");
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
    };
    struct int_bar : bar<int>
    {
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

    template <typename T, typename enable_if<n502::uses_write_v<T>>::type * = nullptr>
    void
    serialize(std::ostream &os, T const &value)
    {
        value.write(os);
    }

    template <typename T, typename enable_if<!n502::uses_write_v<T>>::type * = nullptr>
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
    template <typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
    struct integral_wrapper
    {
        T value;

        integral_wrapper(T v) : value(v)
        {
        }
    };

    template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
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
    bool
    are_equal(T const &a, T const &b)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
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
        }
        else
        {
            static_assert(always_false<T>, "Must be arithmetic"); // ill-formed: invalid for every T
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
        using value_type = std::decay_t<T>;

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
        // static_assert(has_common_type_v<Ts...>,
        //                    "Arguments must have a common type.");

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

        // floating points
        static_assert(is_floating_point<float>::value);
        static_assert(is_floating_point<double>::value);
        static_assert(is_floating_point<long double>::value);

        // non-floating points
        static_assert(!is_floating_point<int>::value);
        static_assert(!is_floating_point<bool>::value);

        process_real_number(42.0);  // 42
        process_real_number(42.0f); // 42
        process_real_number(42.0L); // 42
        // process_real_number(42); // error: int (42) is not a floating point number
        // process_real_number(true); // error: bool (true) is not a floating point number
    }

    {
        std::println("\n====================== using namespace n502 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        w.write(std::cout);      // 1, one
        std::cout << g;          // 2, two

        serialize(std::cout, w); // 1, one
        serialize(std::cout, g); // 2, two
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        std::array<int, 5> arr1{1, 2, 3, 4, 5};
        std::println("{}", *n503::begin(arr1));

        int arr2[]{5, 4, 3, 2, 1};
        std::println("{}", *begin(arr2));
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        int a = 42;
        n503::increment(a); // OK

        std::string s{"42"};
        // n503::increment(s);  // error
    }

    {
        std::println("\n====================== using namespace n503 =============================");
        using namespace n503;

        int arr1[]{1, 2, 3, 4, 5};
        handle(arr1);

        int arr2[]{1, 2, 3, 4};
        handle(arr2);
    }

    {
        std::println("\n====================== using namespace n504 =============================");
        using namespace n504;

        int_foo fi;
        int_bar bi;
        int     x = 0;
        handle(fi);
        handle(bi);
        // handle(x);

        std::println("{}", x);
    }

    {
        std::println("\n====================== using namespace n505 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n505::serialize(std::cout, w);
        n505::serialize(std::cout, g);
    }

    {
        std::println("\n====================== using namespace n506 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n506::serialize(std::cout, w);
        n506::serialize(std::cout, g);
    }

    {
        std::println("\n====================== using namespace n507 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n507::serialize(std::cout, w);
        n507::serialize(std::cout, g);
    }

    {
        std::println("\n====================== using namespace n508 =============================");
        using namespace n508;

        integral_wrapper w1{42}; // OK
        // integral_wrapper w2{ 42.0 }; // error
        // integral_wrapper w3{ "42" }; // error

        // floating_wrapper w4{ 42 };   // error
        floating_wrapper w5{42.0}; // OK
                                   // floating_wrapper w6{ "42" }; // error
    }

    {
        std::println("\n====================== using namespace n509 =============================");
        using namespace n502;

        widget w{1, "one"};
        gadget g{2, "two"};

        n509::serialize(std::cout, w);
        n509::serialize(std::cout, g);
    }

    {
        std::println("\n====================== using namespace n510 =============================");
        using namespace n510;

        std::println("{}", factorial<1>());
        std::println("{}", factorial<2>());
        std::println("{}", factorial<3>());
        std::println("{}", factorial<4>());
        std::println("{}", factorial<5>());
    }

    {
        std::println("\n====================== using namespace n511 =============================");
        using namespace n511;

        are_equal(1, 1);
        are_equal(1.999998, 1.999997);
        are_equal(std::string{"1"}, std::string{"1"});
        // are_equal(n502::widget{ 1, "one" }, n502::widget{ 1, "two" });
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

        std::println("{}", as_string(nullptr));
        std::println("{}", as_string(true));
        std::println("{}", as_string('a'));
        std::println("{}", as_string(42));
        std::println("{}", as_string(42.0));
        // std::println("{}", as_string("42")); // error
    }

    {
        std::println("\n====================== using namespace n514 =============================");
        using namespace n514;

        // std::println("{}", std::boolalpha);
        std::println("{}", std::is_trivial_v<foo>);
        std::println("{}", std::is_trivial_v<bar>);
        std::println("{}", std::is_trivial_v<tar>);

        std::println("{}", std::is_trivially_copyable_v<foo>);
        std::println("{}", std::is_trivially_copyable_v<bar>);
        std::println("{}", std::is_trivially_copyable_v<tar>);
    }

    {
        std::println("\n====================== using namespace n515 =============================");
        using namespace n515;

        std::println("{}", as_string(nullptr));
        std::println("{}", as_string(true));
        std::println("{}", as_string('a'));
        std::println("{}", as_string(42));
        std::println("{}", as_string(42.0));
    }

    {
        std::println("\n====================== using namespace n516 =============================");
        using namespace n516;

        std::println("{}", as_string(nullptr));
        std::println("{}", as_string(true));
        std::println("{}", as_string('a'));
        std::println("{}", as_string(42));
        std::println("{}", as_string(42.0));

        // bool f = true;
        // std::println("{}", f);
        // std::println("{}", as_string(f));

        // int n = 42;
        // std::println("{}", n);
        // std::println("{}", as_string(n));
    }

    {
        std::println("\n====================== using namespace n517 =============================");
        using namespace n517;

        std::println("{}", as_string(nullptr));
        std::println("{}", as_string(true));
        std::println("{}", as_string('a'));
        std::println("{}", as_string(42));
        std::println("{}", as_string(42.0));

        bool f = true;
        std::println("{}", as_string(f));

        int n = 42;
        std::println("{}", as_string(n));
    }

    {
        std::println("\n====================== using namespace n518 =============================");
        using namespace n518;

        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }

    {
        std::println("\n====================== using namespace n519 =============================");
        using namespace n519;

        process(1);           // int
        process(1, 2, 3);     // int
        process(1, 2.0, '3'); // double
        // process(1, 2.0, "3"); // error
    }

    {
        std::println("\n=========================================================================");

        std::vector<int> v1{1, 2, 3, 4, 5};
        std::vector<int> v2(5);

        n520::copy(std::begin(v1), std::end(v1), std::begin(v2));

        int a1[5] = {1, 2, 3, 4, 5};
        int a2[5];

        n520::copy(a1, a1 + 5, a2);
    }
}
