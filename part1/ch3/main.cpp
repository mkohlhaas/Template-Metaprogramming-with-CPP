#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <ios>
#include <iostream>
#include <print>
#include <tuple>

// Variadic Templates

// Understanding the need for variadic templates

namespace n301
{
#include <stdarg.h>

    int
    min(int a, ...)
    {
        va_list args;
        va_start(args, a);

        int val = va_arg(args, int);

        for (int i = 1; i < a; i++)
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
    min(int a, ...)
    {
        va_list args;
        va_start(args, a);
        T val = va_arg(args, T);

        for (int i = 1; i < a; i++)
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

// Variadic function templates

namespace n303
{
    // Overloaded functions. Template functions cannot partially specialized!

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

    template <typename T, typename... Ts> // template parameter pack -> (T1, T2, T3, ...)
    T
    min(T a, Ts... as)                    // function parameter pack -> (T1 a1, T2 a2, T3, a3, ...)
    {
        return min(a, min(as...));        // parameter pack expansion -> a1, a2, a3, ...
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

    template <typename T, typename... Ts>
    T
    min(T a, Ts... args)
    {
        std::println("variadic: {}", __PRETTY_FUNCTION__);
        return min(a, min(args...));
    }
} // namespace n305

// Parameter packs

namespace n306
{
    // using `constexp if`-statement

    template <typename T, typename... Ts>
    T
    sum(T a, Ts... as)
    {

        // sizeof...(Ts) = sizeof...(as) = size of parameter pack Ts = number of elements in Ts
        // sizeof(Ts)...                 = sizeof T1, sizeof T2, sizeof T3, ... (expansion of Ts)

        if constexpr (sizeof...(as) == 0)
        {
            return a;
        }
        else
        {
            return a + sum(as...);
        }
    }
} // namespace n306

namespace n307
{
    // classical approach

    template <typename T>
    T
    sum(T a)
    {
        return a;
    }

    template <typename T, typename... Ts>
    T
    sum(T a, Ts... as)
    {
        return a + sum(as...);
    }
} // namespace n307

namespace n309
{
    template <typename... Ts>
    auto
    get_type_sizes()
    {
        // sizeof...(Ts) is evaluated at compile-time
        // sizeof(Ts)... is expanded  at compile-time
        return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
    }
} // namespace n309

namespace n308
{
    // expands to something like this

    template <typename T1, typename T2, typename T3, typename T4>
    constexpr auto
    get_type_sizes()
    {
        return std::array<std::size_t, 4>{sizeof(T1), sizeof(T2), sizeof(T3), sizeof(T4)};
    }
} // namespace n308

namespace n310
{
    // Typically, the parameter pack is the trailing parameter of a function or template.
    // However, if the compiler can deduce the arguments, then a parameter pack can be
    // followed by other parameters including more parameter packs.

    template <typename... Ts, typename... Us>
    constexpr auto
    multipacks(Ts... ts, Us... us)
    {
        std::println("{} {}", sizeof...(ts), sizeof...(us));
    }
} // namespace n310

namespace n311
{
    template <typename... Ts, typename... Us>
    constexpr auto
    multipacks(Ts... ts, Us... us)
    {
        static_assert(sizeof...(ts) == sizeof...(us), "Packs must be of equal sizes.");
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

    // primary
    template <typename, typename>
    struct func_pair;

    // specialization
    template <typename R1, typename... A1, typename R2, typename... A2>
    struct func_pair<R1(A1...), R2(A2...)> // this makes it a specialization
    {
        std::function<R1(A1...)> f;
        std::function<R2(A2...)> g;
    };
} // namespace n312

// Understanding parameter packs expansion

namespace n318
{
    // Parameter packs can appear in a multitude of contexts.

    template <typename... T>
    struct outer
    {
        // template <T... args>
        template <T...>
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
        auto s1 = sum(args...);
        auto s2 = sum(42, args...);
        auto s3 = sum(step_it(args)...);
        std::println("{}", s1);
        std::println("{}", s2);
        std::println("{}", s3);
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
        // Der größte Ausrichtungswert aller Elemente im Pack wird verwendet.
        alignas(T...) char a;
    };

    template <int... args>
    struct alignment2
    {
        // Der größte Ausrichtungswert aller Elemente im Pack wird verwendet.
        alignas(args...) char a;
    };
} // namespace n318

// Understanding parameter packs expansion

// Variadic class templates

namespace n313
{
    // primary
    template <typename T, typename... Ts>
    struct tuple // no angle brackets -> primary
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

    // specialization
    // tuple with one element
    template <typename T>
    struct tuple<T> // angle brackets -> specialization
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

    // primary class template
    // Variadic class template to find return type.
    template <size_t N, typename T, typename... Ts>
    struct nth_type : nth_type<N - 1, Ts...> // recursive inheritance
    {
        static_assert(N < sizeof...(Ts) + 1, "index out of bounds");
    };

    // specialization
    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...>
    {
        using value_type = T;
    };

    template <size_t N, typename... Ts>
    using nth_type_t = nth_type<N, Ts...>::value_type;

    // We need a class template bc it can be specialized, function templates can't.
    //
    // primary class template
    // Variadic class template to find return value.
    template <size_t N>
    struct getter
    {
        template <typename... Ts>
        static nth_type_t<N, Ts...> &
        get(tuple<Ts...> &t)
        {
            return getter<N - 1>::get(t.rest);
        }
    };

    // specialization
    // recursive base case
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

    // nice wrapper API for getter template
    //
    // function template
    template <size_t N, typename... Ts>
    nth_type_t<N, Ts...> &
    get(tuple<Ts...> &t)
    {
        return getter<N>::get(t);
    }
} // namespace n313

// Fold expressions

// Four types of folds: (op is a binary operator)
// Unary  right fold (E op ...)      becomes (E1 op (... op (EN-1 op EN)))
// Unary  left  fold (... op E)      becomes (((E1 op E2) op ...) op EN)
// Binary right fold (E op ... op I) becomes (E1 op (... op (EN−1 op (EN op I))))
// Binary left  fold (I op ... op E) becomes ((((I op E1) op E2) op ...) op EN)

namespace n314
{
    template <typename... T>
    int
    sum(T... n)
    {
        return (... + n); // unary left fold: ((((arg0 + arg1) + arg2) + ... ) + argN)
    }

    template <typename... T>
    int
    sum_from_zero(T... n)
    {
        return (0 + ... + n); // binary left fold (has an init expression -> 0)
    }
} // namespace n314

namespace n315
{
    template <typename... T>
    int
    suml(T... n)
    {
        return (... + n); // unary left fold
    }

    template <typename... T>
    int
    sumr(T... n)
    {
        return (n + ...); // unary right fold: (arg0 + (arg1 + (arg2 + (... + argN))))
    }

    // also works for expressions
    template <typename... T>
    void
    printl(T... args)
    {
        // acts like repetition (replaces loops)
        (..., (std::cout << args)) << '\n'; // using comma operator (unary left fold)
    }

    template <typename... T>
    void
    printr(T... args)
    {
        ((std::cout << args), ...) << '\n'; // using comma operator (unary right fold)
    }

    template <typename... T>
    void
    print(T... args)
    {
        (std::cout << ... << args) << '\n'; // binary left fold (init expression -> std::cout)
    }

    template <typename T, typename... Args>
    void
    push_back_many(std::vector<T> &v, Args &&...args)
    {
        // (v.push_back(args), ...); // unary right fold
        (..., v.push_back(args)); // unary left fold
    }
} // namespace n315

// Variadic alias templates

namespace n317
{
    // Everything that can be templatized can also be made variadic.
    // An alias template is an alias for a family of types.
    // A variadic alias template is a name for a family of types with a variable number of template parameters.

    namespace
    {
        template <typename T, typename... Args>
        struct foo
        {
        };

        // variadic alias template
        template <typename... Args>
        using int_foo = foo<int, Args...>;
    } // namespace

    namespace
    {
        template <typename T, T... Ints> // all Ints are of type T (not Ts or similar pattern)
        struct integer_sequence
        {
        };

        // variadic alias template
        template <std::size_t... Ints>
        using index_sequence = integer_sequence<std::size_t, Ints...>;

        template <typename Tuple, std::size_t... Ints>
        auto
        select_tuple(Tuple &&tuple, index_sequence<Ints...>)
        {
            return std::make_tuple(std::get<Ints>(std::forward<Tuple>(tuple))...);
        }

        namespace
        {
            // primary (recursive inheritance)
            template <typename T, std::size_t N, T... Is>
            struct make_integer_sequence : make_integer_sequence<T, N - 1, Is...>
            {
            };

            // specialization
            template <typename T, T... Is>
            struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...>
            {
            };

            namespace
            {
                // variadic alias template
                template <std::size_t N>
                using make_index_sequence = make_integer_sequence<std::size_t, N>;

                // variadic alias template
                template <typename... T>
                using index_sequence_for = make_index_sequence<sizeof...(T)>;
            } // namespace
        } // namespace
    } // namespace
} // namespace n317

// Variadic variable templates

namespace n316
{
    template <int... R>
    constexpr int Sum = (... + R); // unary left fold

    template <int... I>
    constexpr auto indexes = std::make_index_sequence<5>();
} // namespace n316

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

        multipacks(1, 2, 3, 4, 5, 6);                               // 0,6
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

    {
        std::println("\n====================== using namespace n312 =============================");
        using namespace n312;

        func_pair<bool(int, int), double(int, int, double)> funcs{twice_as, sum_and_div};

        std::println("{}", funcs.f(42, 12));       // true
        std::println("{}", funcs.g(42, 12, 10.0)); // 5.4
    }

    {
        std::println("\n====================== using namespace n318 =============================");
        using namespace n318;

        [[maybe_unused]] outer<int, double, char[5]> a;

        tagger<int, double, char[5], short, float>();

        make_it(42);
        make_it(42, 'a');

        do_sums(1, 2, 3, 4); // 1 2 3 4

        parenthesized(1, 2, 3, 4);

        brace_enclosed(1, 2, 3, 4);

        captures(1, 2, 3, 4);

        auto arr = make_array(1, 2, 3, 4);
        std::println("{}", arr[0]); // 1
        std::println("{}", arr[1]); // 2
        std::println("{}", arr[2]); // 3
        std::println("{}", arr[3]); // 4

        alignment1<int, double> al1{'a'};
        std::println("{}", al1.a);  // a

        alignment2<1, 4, 8> al2{'a'};
        std::println("{}", al2.a);  // a
    }

    {
        std::println("\n====================== using namespace n318 =============================");
        using namespace n318;

        A a;
        B b;
        C c;
        X x(a, b, c);

        x.A::execute(); // A::execute
        x.B::execute(); // B::execute
        x.C::execute(); // C::execute
    }

    {
        std::println("\n====================== using namespace n313 =============================");
        using namespace n313;

        tuple<int>               one(42);
        tuple<int, double>       two(42, 42.0);
        tuple<int, double, char> three(42, 42.0, 'a');

        tuple one1(42);
        tuple two1(42, 42.0);
        tuple three1(42, 42.0, 'a');

        std::println("{}", one.value);                                         // 42
        std::println("{}", two.value);                                         // 42
        std::println("{}", three.value);                                       // 42

        std::println("{}", get<0>(one));                                       // 42
        std::println("{} {}", get<0>(two), get<1>(two));                       // 42 42
        std::println("{} {} {}", get<0>(three), get<1>(three), get<2>(three)); // 42 42 a

        // std::println("{} {}", get<0>(two), get<1>(two), get<3>(two));
        //                                                 ^^^^^^
        //                               error: No matching function for call to 'get'.
    }

    {
        std::println("\n====================== using namespace n314 =============================");
        using namespace n314;

        // std::println("{}", sum());                     // error
        std::println("{}", sum(1));                       // 1
        std::println("{}", sum(1, 2));                    // 3
        std::println("{}", sum(1, 2, 3, 4, 5));           // 15

        std::println("{}", sum_from_zero());              // 0
        std::println("{}", sum_from_zero(1));             // 1
        std::println("{}", sum_from_zero(1, 2));          // 3
        std::println("{}", sum_from_zero(1, 2, 3));       // 6
        std::println("{}", sum_from_zero(1, 2, 3, 4));    // 10
        std::println("{}", sum_from_zero(1, 2, 3, 4, 5)); // 15
    }

    {
        std::println("\n====================== using namespace n315 =============================");
        using namespace n315;

        printl('d', 'o', 'g'); // dog
        printr('d', 'o', 'g'); // dog
        print('d', 'o', 'g');  // dog
    }

    {
        std::println("\n====================== using namespace n315 =============================");
        using namespace n315;

        std::vector<int> v;
        push_back_many(v, 1, 2, 3, 4, 5);
        std::println("{}", v.size()); // 5
        std::println("{}", v[0]);     // 1
        std::println("{}", v[4]);     // 5
    }

    {
        std::println("\n====================== using namespace n316 =============================");
        using namespace n316;

        std::println("{}", Sum<1>);             // 1
        std::println("{}", Sum<1, 2>);          // 3
        std::println("{}", Sum<1, 2, 3, 4, 5>); // 15
    }

    {
        std::println("\n====================== using namespace n317 =============================");
        using namespace n317;

        foo<double, char, int> f1;
        foo<int, char, double> f2;
        int_foo<char, double>  f3;

        static_assert(!std::is_same_v<decltype(f1), decltype(f2)>);
        static_assert(std::is_same_v<decltype(f2), decltype(f3)>);
    }

    {
        std::println("\n====================== using namespace n317 =============================");
        using namespace n317;

        std::tuple<int, char, double> t1{42, 'x', 42.99};

        auto t2 = select_tuple(t1, index_sequence<0, 2>{});

        assert((t2 == std::tuple{42, 42.99}));
    }
}
