#include <array>
#include <functional>
#include <iostream>
#include <print>
#include <tuple>

namespace n301
{
#include <stdarg.h>

    int
    min(int count, ...)
    {
        va_list args;
        va_start(args, count);

        int val = va_arg(args, int);

        for (int i = 1; i < count; i++)
        {
            int n = va_arg(args, int);
            if (n < val)
            {
                val = n;
            }
        }

        va_end(args);

        return val;
    }
} // namespace n301

namespace n302
{
#include <stdarg.h>

    template <typename T>
    T
    min(int count, ...)
    {
        va_list args;
        va_start(args, count);
        T val = va_arg(args, T);

        for (int i = 1; i < count; i++)
        {
            T n = va_arg(args, T);
            if (n < val)
            {
                val = n;
            }
        }

        va_end(args);

        return val;
    }
} // namespace n302

namespace n303
{
    template <typename T>
    T
    min(T a)
    {
        return a;
    }

    template <typename T>
    T
    min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T, typename... Args>
    T
    min(T a, Args... args)
    {
        return min(a, min(args...));
    }
} // namespace n303

namespace n304
{
    int
    min(int a, int b)
    {
        return a < b ? a : b;
    }

    int
    min(int a, int b, int c)
    {
        return min(a, min(b, c));
    }

    int
    min(int a, int b, int c, int d)
    {
        return min(a, min(b, min(c, d)));
    }

    int
    min(int a, int b, int c, int d, int e)
    {
        return min(a, min(b, min(c, min(d, e))));
    }
} // namespace n304

namespace n305
{
    template <typename T>
    T
    min(T a, T b)
    {
        std::println("single: {}", __PRETTY_FUNCTION__);
        return a < b ? a : b;
    }

    template <typename T, typename... Args>
    T
    min(T a, Args... args)
    {
        std::println("variadic: {}", __PRETTY_FUNCTION__);
        return min(a, min(args...));
    }
} // namespace n305

namespace n306
{
    template <typename T, typename... Args>
    T
    sum(T a, Args... args)
    {
        if constexpr (sizeof...(args) == 0)
        {
            return a;
        }
        else
        {
            return a + sum(args...);
        }
    }
} // namespace n306

namespace n307
{
    template <typename T>
    T
    sum(T a)
    {
        return a;
    }

    template <typename T, typename... Args>
    T
    sum(T a, Args... args)
    {
        return a + sum(args...);
    }
} // namespace n307

namespace n308
{
    template <typename T1, typename T2, typename T3, typename T4>
    constexpr auto
    get_type_sizes()
    {
        // return std::array<std::size_t, 4>{sizeof(T1), sizeof(T2), sizeof(T3), sizeof(T4)};
        return std::array{sizeof(T1), sizeof(T2), sizeof(T3), sizeof(T4)};
    }
} // namespace n308

namespace n309
{
    template <typename... Ts>
    auto
    get_type_sizes()
    {
        // return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
        return std::array{sizeof(Ts)...};
    }
} // namespace n309

namespace n310
{
    template <typename... Ts, typename... Us>
    constexpr auto
    multipacks(Ts... args1, Us... args2)
    {
        std::println("{} {}", sizeof...(args1), sizeof...(args2));
    }
} // namespace n310

namespace n311
{
    template <typename... Ts, typename... Us>
    constexpr auto
    multipacks(Ts... args1, Us... args2)
    {
        static_assert(sizeof...(args1) == sizeof...(args2), "Packs must be of equal sizes.");
    }
} // namespace n311

namespace n312
{
    bool
    twice_as(int a, int b)
    {
        return a >= b * 2;
    }

    double
    sum_and_div(int a, int b, double c)
    {
        return (a + b) / c;
    }

    template <typename, typename>
    struct func_pair;

    template <typename R1, typename... A1, typename R2, typename... A2>
    struct func_pair<R1(A1...), R2(A2...)>
    {
        std::function<R1(A1...)> f;
        std::function<R2(A2...)> g;
    };
} // namespace n312

namespace n313
{
    template <typename T, typename... Ts>
    struct tuple
    {
        tuple(T const &t, Ts const &...ts) : value(t), rest(ts...)
        {
        }

        constexpr int
        size() const
        {
            return 1 + rest.size();
        }

        T            value;
        tuple<Ts...> rest;
    };

    template <typename T>
    struct tuple<T>
    {
        tuple(const T &t) : value(t)
        {
        }

        constexpr int
        size() const
        {
            return 1;
        }

        T value;
    };

    template <size_t N, typename T, typename... Ts>
    struct nth_type : nth_type<N - 1, Ts...>
    {
        static_assert(N < sizeof...(Ts) + 1, "index out of bounds");
    };

    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...>
    {
        using value_type = T;
    };

    template <size_t N>
    struct getter
    {
        template <typename... Ts>
        static typename nth_type<N, Ts...>::value_type &
        get(tuple<Ts...> &t)
        {
            return getter<N - 1>::get(t.rest);
        }
    };

    template <>
    struct getter<0>
    {
        template <typename T, typename... Ts>
        static T &
        get(tuple<T, Ts...> &t)
        {
            return t.value;
        }
    };

    template <size_t N, typename... Ts>
    typename nth_type<N, Ts...>::value_type &
    get(tuple<Ts...> &t)
    {
        return getter<N>::get(t);
    }
} // namespace n313

namespace n314
{
    template <typename... T>
    int
    sum(T... args)
    {
        return (... + args);
    }

    template <typename... T>
    int
    sum_from_zero(T... args)
    {
        return (0 + ... + args);
    }
} // namespace n314

namespace n315
{
    template <typename... T>
    int
    suml(T... args)
    {
        return (... + args);
    }

    template <typename... T>
    int
    sumr(T... args)
    {
        return (args + ...);
    }

    template <typename... T>
    void
    printl(T... args)
    {
        (..., (std::cout << args)) << '\n';
    }

    template <typename... T>
    void
    printr(T... args)
    {
        ((std::cout << args), ...) << '\n';
    }

    template <typename... T>
    void
    print(T... args)
    {
        (std::cout << ... << args) << '\n';
    }

    template <typename T, typename... Args>
    void
    push_back_many(std::vector<T> &v, Args &&...args)
    {
        (v.push_back(args), ...);
    }
} // namespace n315

namespace n316
{
    template <int... R>
    constexpr int Sum = (... + R);

    template <int... I>
    constexpr auto indexes = std::make_index_sequence<5>();
} // namespace n316

namespace n317
{
    template <typename T, typename... Args>
    struct foo
    {
    };

    template <typename... Args>
    using int_foo = foo<int, Args...>;

    template <typename T, T... Ints>
    struct integer_sequence
    {
    };

    template <std::size_t... Ints>
    using index_sequence = integer_sequence<std::size_t, Ints...>;

    template <typename T, std::size_t N, T... Is>
    struct make_integer_sequence : make_integer_sequence<T, N - 1, N - 1, Is...>
    {
    };

    template <typename T, T... Is>
    struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...>
    {
    };

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;

    template <typename Tuple, std::size_t... Ints>
    auto
    select_tuple(Tuple &&tuple, index_sequence<Ints...>)
    {
        return std::make_tuple(std::get<Ints>(std::forward<Tuple>(tuple))...);
    }
} // namespace n317

namespace n318
{
    template <typename... T>
    struct outer
    {
        template <T... args>
        struct inner
        {
        };
    };

    template <typename... T>
    struct tag
    {
    };

    template <typename T, typename U, typename... Args>
    void
    tagger()
    {
        [[maybe_unused]] tag<T, U, Args...> t1;
        [[maybe_unused]] tag<T, Args..., U> t2;
        [[maybe_unused]] tag<Args..., T, U> t3;
        [[maybe_unused]] tag<U, T, Args...> t4;
    }

    template <typename... Args>
    void
    make_it(Args... args [[maybe_unused]])
    {
    }

    template <typename T>
    T
    step_it(T value)
    {
        return value + 1;
    }

    template <typename... T>
    int
    sum(T... args)
    {
        return (... + args);
    }

    template <typename... T>
    void
    do_sums(T... args)
    {
        auto s1 [[maybe_unused]] = sum(args...);
        auto s2 [[maybe_unused]] = sum(42, args...);
        auto s3 [[maybe_unused]] = sum(step_it(args)...);
    }

    template <typename... T>
    struct sum_wrapper
    {
        sum_wrapper(T... args)
        {
            value = (... + args);
        }

        std::common_type_t<T...> value;
    };

    template <typename... T>
    void
    parenthesized(T... args)
    {
        std::array<std::common_type_t<T...>, sizeof...(T)> arr [[maybe_unused]]{args...};

        sum_wrapper sw1(args...);   // value = 1 + 2 + 3 + 4
        sum_wrapper sw2(++args...); // value = 2 + 3 + 4 + 5
    }

    template <typename... T>
    void
    brace_enclosed(T... args)
    {
        int arr1 [[maybe_unused]][sizeof...(args) + 1] = {args..., 0};       // 1,2,3,4,5
        int arr2 [[maybe_unused]][sizeof...(args)]     = {step_it(args)...}; // 2,3,4,5
    }

    struct A
    {
        void
        execute()
        {
            std::println("A::execute");
        }
    };
    struct B
    {
        void
        execute()
        {
            std::println("B::execute");
        }
    };
    struct C
    {
        void
        execute()
        {
            std::println("C::execute");
        }
    };

    template <typename... Bases>
    struct X : public Bases...
    {
        X(Bases const &...args) : Bases(args)...
        {
        }

        using Bases::execute...;
    };

    template <typename... T>
    void
    captures(T... args)
    {
        auto l                  = [args...] { return sum(step_it(args)...); };
        auto s [[maybe_unused]] = l();
    }

    template <typename... T>
    auto
    make_array(T... args)
    {
        return std::array<std::common_type_t<T...>, sizeof...(T)>{args...};
    };

    template <typename... T>
    struct alignment1
    {
        alignas(T...) char a;
    };

    template <int... args>
    struct alignment2
    {
        alignas(args...) char a;
    };
} // namespace n318

int
main()
{
    {
        std::println("\n====================== using namespace n301 =============================");
        using namespace n301;

        std::println("{}", min(1, 7));              // 7
        std::println("{}", min(2, 42, 7));          // 7
        std::println("{}", min(5, 1, 5, 3, -4, 9)); // -4
    }

    {
        std::println("\n====================== using namespace n302 =============================");
        using namespace n302;

        std::println("{}", min<double>(1, 7.5));              // 7.5
        std::println("{}", min<double>(2, 42.0, 7.5));        // 7.5
        std::println("{} (garbage)", min<int>(2, 42.0, 7.5)); // !!!
        std::println("{}", min<int>(5, 1, 5, 3, -4, 9));      // -4
    }

    {
        std::println("\n====================== using namespace n303 =============================");
        using namespace n303;

        std::println("{}", min(7.5));            // 7.5
        std::println("{}", min(42.0, 7.5));      // 7.5
        std::println("{}", min(1, 5, 3, -4, 9)); // -4
    }

    {
        std::println("\n====================== using namespace n304 =============================");
        using namespace n304;

        std::println("{}", min(42, 7));          // 7
        std::println("{}", min(1, 5, 3, -4, 9)); // -4
    }

    {
        std::println("\n====================== using namespace n305 =============================");
        using namespace n305;

        std::println("{}", min(1, 5, 3, -4, 9));       // -4
        std::println("{}", min(1, 8, 5, 7, 3, -4, 9)); // -4
    }

    {
        std::println("\n====================== using namespace n306 =============================");
        using namespace n306;

        std::println("{}", sum(1, 2, 3, 4, 5)); // 15
        std::println("{}", sum(1));             // 1
    }

    {
        std::println("\n====================== using namespace n307 =============================");
        using namespace n307;

        std::println("{}", sum(1, 2, 3, 4, 5)); // 15
        std::println("{}", sum(1));             // 1
    }

    {
        std::println("\n====================== using namespace n308 =============================");
        using namespace n308;

        auto sizes = get_type_sizes<short, int, long, long long>();
        for (auto const s : sizes)
        {
            std::println("{}", s); // 2 4 8 8
        }
    }

    {
        std::println("\n====================== using namespace n309 =============================");
        using namespace n309;

        auto sizes = get_type_sizes<short, int, long, long long>();
        for (auto const s : sizes)
        {
            std::println("{}", s); // 2 4 8 8
        }
    }

    {
        std::println("\n====================== using namespace n310 =============================");
        using namespace n310;

        multipacks<int>(1, 2, 3, 4, 5, 6);                          // 1,5
        multipacks<int, int, int>(1, 2, 3, 4, 5, 6);                // 3,3
        multipacks<int, int, int, int>(1, 2, 3, 4, 5, 6);           // 4,2
        multipacks<int, int, int, int, int, int>(1, 2, 3, 4, 5, 6); // 6,0
        multipacks<int, int>(1, 2, 4.0, 5.0, 6.0);                  // 2,3
        multipacks<int, int, int>(1, 2, 3, 4.0, 5.0, 6.0);          // 3,3
    }

    {
        std::println("\n====================== using namespace n311 =============================");
        using namespace n311;

        multipacks<int, int, int>(1, 2, 3, 4, 5, 6);
        multipacks<int, int, int>(1, 2, 3, 4.0, 5.0, 6.0);

        // multipacks<int>(1, 2, 3, 4, 5, 6);                          // error
        // multipacks<int, int, int, int>(1, 2, 3, 4, 5, 6);           // error
        // multipacks<int, int, int, int, int, int>(1, 2, 3, 4, 5, 6); // error
        // multipacks<int, int>(1, 2, 4.0, 5.0, 6.0);                  // error
    }

    // {
    //     std::println("\n====================== using namespace n312 =============================");
    //     using namespace n312;
    //
    //     func_pair<bool(int, int), double(int, int, double)> funcs{twice_as, sum_and_div};
    //     funcs.f(42, 12);
    //     funcs.g(42, 12, 10.0);
    // }

    // {
    //     std::println("\n====================== using namespace n313 =============================");
    //     using namespace n313;
    //
    //     tuple<int>               one(42);
    //     tuple<int, double>       two(42, 42.0);
    //     tuple<int, double, char> three(42, 42.0, 'a');
    //
    //     std::println("{}", one.value);                                         // 42
    //     std::println("{}", two.value);                                         // 42
    //     std::println("{}", three.value);                                       // 42
    //
    //     std::println("{}", get<0>(one));                                       // 42
    //     std::println("{} {}", get<0>(two), get<1>(two));                       // 42 42
    //     std::println("{} {} {}", get<0>(three), get<1>(three), get<2>(three)); // 42 42 a
    // }

    // {
    //     std::println("\n====================== using namespace n314 =============================");
    //     using namespace n314;
    //
    //     // std::println("{}", sum()); // error
    //     std::println("{}", sum(1));                 // 1
    //     std::println("{}", sum(1, 2));              // 3
    //     std::println("{}", sum(1, 2, 3, 4, 5));     // 15
    //
    //     std::println("{}", sum_from_zero());        // 0
    //     std::println("{}", sum_from_zero(1, 2, 3)); // 6
    // }

    // {
    //     std::println("\n====================== using namespace n315 =============================");
    //     using namespace n315;
    //
    //     printl('d', 'o', 'g'); // dog
    //     printr('d', 'o', 'g'); // dog
    //     print('d', 'o', 'g');  // dog
    // }

    // {
    //     std::println("\n====================== using namespace n315 =============================");
    //     using namespace n315;
    //
    //     std::vector<int> v;
    //     push_back_many(v, 1, 2, 3, 4, 5);
    //     std::println("{}", v.size()); // 5
    // }

    // {
    //     std::println("\n====================== using namespace n316 =============================");
    //     using namespace n316;
    //
    //     std::println("{}", Sum<1>);             // 1
    //     std::println("{}", Sum<1, 2>);          // 3
    //     std::println("{}", Sum<1, 2, 3, 4, 5>); // 15
    // }

    // {
    //     std::println("\n====================== using namespace n317 =============================");
    //     using namespace n317;
    //
    //     foo<double, char, int> f1;
    //     foo<int, char, double> f2;
    //     int_foo<char, double>  f3;
    //     static_assert(!std::is_same_v<decltype(f1), decltype(f2)>);
    //     static_assert(std::is_same_v<decltype(f2), decltype(f3)>);
    // }

    // {
    //     std::println("\n====================== using namespace n317 =============================");
    //     using namespace n317;
    //
    //     std::tuple<int, char, double> t1{42, 'x', 42.99};
    //
    //     auto t2 /* [[maybe_unused]] */ = select_tuple(t1, index_sequence<0, 2>{});
    // }

    // {
    //     std::println("\n====================== using namespace n318 =============================");
    //     using namespace n318;
    //
    //     [[maybe_unused]] outer<int, double, char[5]> a;
    //
    //     tagger<int, double, char[5], short, float>();
    //
    //     make_it(42);
    //     make_it(42, 'a');
    //
    //     do_sums(1, 2, 3, 4);
    //
    //     parenthesized(1, 2, 3, 4);
    //
    //     brace_enclosed(1, 2, 3, 4);
    //
    //     captures(1, 2, 3, 4);
    //
    //     auto arr [[maybe_unused]] = make_array(1, 2, 3, 4);
    //
    //     alignment1<int, double> al1 [[maybe_unused]];
    //     al1.a = 'a';
    //
    //     // alignment2<1, 4, 8> al2; // error with VC++
    //     // al2.a = 'b';
    // }

    // {
    //     std::println("\n====================== using namespace n318 =============================");
    //     using namespace n318;
    //
    //     A a;
    //     B b;
    //     C c;
    //     X x(a, b, c);
    //
    //     x.A::execute(); // A::execute
    //     x.B::execute(); // B::execute
    //     x.C::execute(); // C::execute
    // }
}
