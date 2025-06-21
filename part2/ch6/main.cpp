#include <print>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// A CONSTRAINT is a modern way to define requirements on template parameters.
// A CONSTRAINT is a predicate that evaluates to true or false at compile-time.
// A CONCEPT is a set of named constraints

// Understanding the need for concepts

namespace n601
{
    template <typename T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }
} // namespace n601

namespace n602
{
    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    T
    add(T const a, T const b)
    {
        return a + b;
    }
} // namespace n602

namespace n603
{
    template <typename T>
    T
    add(T const a, T const b)
    {
        static_assert(std::is_arithmetic_v<T>, "Arithmetic type required");
        return a + b;
    }
} // namespace n603

namespace n604
{
    // REQUIRES CLAUSE doesn't use curly braces
    // A requires clause determines whether a function participates in overload resolution
    // or not. This happens based on the value of a compile-time Boolean expression.

    template <typename T>
        requires std::is_arithmetic_v<T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }
} // namespace n604

namespace n605
{
    // different order but the same (matter of personal taste)

    template <typename T>
    T
    add(T const a, T const b)
        requires std::is_arithmetic_v<T>
    {
        return a + b;
    }
} // namespace n605

// Defining concepts

namespace n608
{
    template <typename T>
        requires std::is_arithmetic_v<T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    T
    mul(T const a, T const b)
    {
        return a * b;
    }
} // namespace n608

namespace n606
{
    // REQUIRES EXPRESSION uses curly braces
    // A requires expression determines whether a set of one or more expressions is
    // well-formed, without having any side effects on the behavior of the program. A
    // requires expression is a Boolean expression that can be used with a requires clause.
    template <typename T>
    concept arithmetic = requires { std::is_arithmetic_v<T>; };

    template <arithmetic T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    T
    mul(T const a, T const b)
    {
        return a * b;
    }
} // namespace n606

namespace n607
{
    // simple requirement (doesn't use `requires` keyword)
    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template <arithmetic T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }
} // namespace n607

namespace n610
{
    // old style
    template <typename T, typename U = void>
    struct is_container : std::false_type
    {
    };

    template <typename T>
    struct is_container<T, std::void_t<typename T::value_type,               //
                                       typename T::size_type,                //
                                       typename T::allocator_type,           //
                                       typename T::iterator,                 //
                                       typename T::const_iterator,           //
                                       decltype(std::declval<T>().size()),   //
                                       decltype(std::declval<T>().begin()),  //
                                       decltype(std::declval<T>().end()),    //
                                       decltype(std::declval<T>().cbegin()), //
                                       decltype(std::declval<T>().cend())>>  //
        : std::true_type
    {
    };

    template <typename T, typename U = void>
    constexpr bool is_container_v = is_container<T, U>::value;

    struct foo
    {
        // ...
    };

} // namespace n610

namespace n611
{
    // with concepts
    // note naming (`container` instead of `is_container` bc it's used like a data type)
    template <typename T>
    concept container = requires(T t) {
        typename T::value_type;     // type requirement
        typename T::size_type;      // ..
        typename T::allocator_type; // ..
        typename T::iterator;       // ..
        typename T::const_iterator; // type requirement
        t.size();                   // simple requirement
        t.begin();                  // ..
        t.end();                    // ..
        t.cbegin();                 // ..
        t.cend();                   // simple requirement
    };

    struct foo
    {
    };

    template <container C>
    void
    process(C &&)
    {
        // ...
    }
} // namespace n611

// Exploring requires expressions

// - simple requirements
// - type requirements
// - compound requirements
// - nested requirements

// Simple requirements

namespace n612
{
    // syntax: requires (parameter-list) { requirement-seq }

    template <typename T>
    concept arithmetic = requires { std::is_arithmetic_v<T>; }; // empty parameter list can be omitted

    template <typename T>
    concept addable = requires(T a, T b) { a + b; };

    template <typename T>
    concept logger = requires(T t) {
        t.error("just"); // Takes a single parameter of `const char*` or some type that can be constructed from it.
        t.warning("a");  // The actual values passed as arguments have no importance (they are never performed).
        t.info("demo");  // They are only checked for type correctness.
    };

    template <logger Logger>
    void
    log_error(Logger &)
    {
        // ...
    }

    struct console_logger
    {
        void
        error(std::string_view)
        {
            // ...
        }

        void
        warning(std::string_view)
        {
            // ...
        }

        void
        info(std::string_view)
        {
            // ...
        }
    };

    struct stream_logger
    {
        void
        error(std::string_view, bool = false)
        {
            // ...
        }

        void
        warning(std::string_view, bool = false)
        {
            // ...
        }

        void
        info(std::string_view, bool) // bool has no default value!
        {
            // ...
        }
    };
} // namespace n612

// Type requirements

namespace n613
{
    namespace
    {
        template <typename T>
        concept KVP = requires {
            typename T::key_type;   // type requirement
            typename T::value_type; // ..
        };

        template <typename T, typename V>
        struct key_value_pair
        {
            using key_type   = T;
            using value_type = V;

            key_type   key;
            value_type value;
        };
    } // namespace

    namespace
    {
        template <typename T>
            requires std::is_arithmetic_v<T>
        struct container
        {
            // ...
        };

        template <typename T>
        concept containerizeable = requires { typename container<T>; };

    } // namespace
} // namespace n613

// Compound requirements

namespace n614
{
    namespace
    {
        template <typename T>
        void
        f(T) noexcept
        {
            // ...
            // doesn't throw errors
            // ...
        }

        template <typename T>
        void
        g(T)
        {
            // ...
            // could throw an error
            // ...
        }
    } // namespace

    namespace
    {
        // We want to check an expression for exceptions and/or return types.
        // syntax: { expression } [noexcept] [-> type_constraint];
        // (both noexcept and type_constraint are optional)
        template <typename F, typename... Args>
        concept NonThrowing = requires(F &&func, Args... args) {
            { func(args...) } noexcept; // checking for noexcept specifier
        };

        template <typename F, typename... Args>
            requires NonThrowing<F, Args...>
        void
        invoke(F &&func, Args... args)
        {
            func(args...);
        }
    } // namespace
} // namespace n614

namespace n615
{
    // checking for return values (must be a type constraint; not the actual return type)
    template <typename T>
    concept timer = requires(T t) {
        { t.start() } -> std::same_as<void>;            // start() returns void
        { t.stop() } -> std::convertible_to<long long>; // stop() returns anything convertible to long long
    };

    struct timerA
    {
        void
        start()
        {
            // ...
        }

        long long
        stop()
        {
            return 0;
        }
    };

    struct timerB
    {
        void
        start()
        {
            // ...
        }

        int
        stop()
        {
            return 0;
        }
    };

    struct timerC
    {
        void
        start()
        {
            // ...
        }

        void
        stop()
        {
            // ...
            // void is not convertible to long long
            // ...
        }

        long long
        getTicks()
        {
            return 0;
        }
    };

} // namespace n615

// Nested requirements

namespace n616
{
    // std::conjunction_v performs a logical AND on the sequence of traits.
    template <typename T, typename... Ts>
    inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

    template <typename... T>
    concept HomogenousRange = requires(T... t) { // `requires`
        (... + t);                               // simple requirement (no `requires`)
        requires are_same_v<T...>;               // nested requirement: `requires` inside `requires`
        requires sizeof...(T) > 1;               // nested requirement: `requires` inside `requires`
    };

    template <typename... T>
        requires HomogenousRange<T...>
    auto
    add(T &&...t)
    {
        return (... + t);
    }
} // namespace n616

// Composing constraints

// with && (conjunction) and || (disjunction); short-circuited

namespace n617
{
    template <typename T>
        requires std::is_integral_v<T> && std::is_signed_v<T>
    T
    decrement(T value)
    {
        return --value;
    }
} // namespace n617

namespace n618
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept Signed = std::is_signed_v<T>;

    template <typename T>
    concept SignedIntegral = Integral<T> && Signed<T>;

    template <SignedIntegral T>
    T
    decrement(T value)
    {
        return --value;
    }
} // namespace n618

namespace n619
{
    template <typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    T
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n619

namespace n620
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept FloatingPoint = std::is_floating_point_v<T>;

    template <typename T>
    concept Number = Integral<T> || FloatingPoint<T>;

    template <Number T>
    T
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n620

namespace n621
{
    template <typename T>
    concept A = std::is_integral_v<T>;

    template <typename T>
    concept B = std::is_floating_point_v<T>;

    // disjunctions
    template <typename T>
        requires A<T> || B<T>
    void
    f1()
    {
        // ...
    }

    template <typename T>
        requires(A<T> || B<T>)
    void
    f2()
    {
        // ...
    }

    // Logical Expressions
    //
    // When used inside a cast expression or a logical NOT, the && and || tokens define a logical expression!
    // The entire expression is first checked for correctness, and then its Boolean value is determined.

    template <typename T>
        requires A<T> && (!A<T> || B<T>) // logical expression: will always be false
    void f3()
    {
        // ...
    }

    template <typename T>
        requires(!(A<T> || B<T>))
    void
    f4()
    {
        // ...
    }

    template <typename T>
        requires(static_cast<bool>(A<T> || B<T>))
    void
    f5()
    {
        // ...
    }
} // namespace n621

namespace n622
{
    // Conjunctions and disjunctions cannot be used to constrain template parameter packs.
    // Here's a workaround:

    template <typename... T>
    //  requires std::is_integral_v<T> && ...  // error: this is not allowed
        requires(std::is_integral_v<T> && ...) // now it's a fold expression of type-traits! (No short-circuiting!)
    auto
    add(T... args)
    {
        return (args + ...);
    }
} // namespace n622

namespace n623
{
    // we just give the thing a name (concept = named constraint)
    template <typename T>
    concept Integral = std::is_integral_v<T>; // RHS = boolean value

    // and now it works
    template <typename... T>
        requires(Integral<T> && ...) // fold with concepts creates a conjunction (with short-circuiting!)
    auto
    add(T... args)
    {
        return (args + ...);
    }
} // namespace n623

// Learning about the ordering of templates with constraints

namespace n624
{
    int
    add(int a, int b)
    {
        return a + b;
    }

    template <typename T>
    T
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n624

namespace n625
{
    template <typename T>
    T
    add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T>
    T
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n625

namespace n626
{
    template <typename T>
        requires(sizeof(T) == 4)
    T
    add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T>
    T
    add(T a, T b)
    {
        std::println("{} {}", sizeof(a), sizeof(b));
        return a + b;
    }
} // namespace n626

namespace n627
{
    template <typename T>
        requires std::is_integral_v<T>
    T
    add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T> && (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n627

namespace n628
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
        requires Integral<T>
    T
    add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires Integral<T> && (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n628

namespace n629
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <Integral T>
    T
    add(T a, T b)
    {
        return a + b;
    }

    template <Integral T>
        requires(sizeof(T) == 4)
    T
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n629

// Constraining non-template member functions

namespace n630
{
    template <typename T>
    struct wrapper
    {
        T value;

        bool
        operator==(std::string_view str)
            requires std::is_convertible_v<T, std::string_view>
        {
            return value == str;
        }
    };
} // namespace n630

namespace n631a
{
    template <typename T>
    struct wrapper
    {
        T value;

        wrapper(T const &v) : value(v)
        {
        }
    };
} // namespace n631a

namespace n631b
{
    template <typename T>
    struct wrapper
    {
        T value;

        // before C++20 (using SFINAE)
        template <typename U,
                  typename = std::enable_if_t<std::is_copy_constructible_v<U> //
                                              && std::is_convertible_v<U, T>>>
        wrapper(U const &v) : value(v)
        {
        }
    };
} // namespace n631b

namespace n631c
{
    template <typename T>
    struct wrapper
    {
        T value;

        // C++20
        wrapper(T const &v)
            requires std::is_copy_constructible_v<T>
            : value(v)
        {
        }
    };
} // namespace n631c

namespace n632a
{
    void
    handle(int)
    {
        // ...
    }

    // Non-templated function cannot have a requires clause!
    // void
    // handle(long v)
    //     requires(sizeof(long) > sizeof(int))
    // {
    //     // ...
    // }
} // namespace n632a

namespace n632b
{
    void
    handle(long)
    {
        if constexpr (sizeof(long) > sizeof(int))
        {
            // ...
        }
        else
        {
            // ...
        }
    }
} // namespace n632b

// Constraining class templates

namespace n633a
{
    // std::integral is a predefined concept
    template <std::integral T>
    struct wrapper
    {
        T value;
    };
} // namespace n633a

namespace n633b
{
    template <std::integral T>
    struct wrapper
    {
        T value;
    };

    struct bytes
    {
        uint8_t byte4;
        uint8_t byte3;
        uint8_t byte2;
        uint8_t byte1;
    };

    using Bytes = struct bytes;

    template <std::integral T>
        requires(sizeof(T) == 4)
    struct wrapper<T>
    {
        union {
            T     value;
            Bytes bytes;
        };
    };
} // namespace n633b

// Constraining variable templates and template aliases

namespace n634
{
    // std::floating_point is a predefined concept
    template <std::floating_point T>
    constexpr T PI = T(3.1415926535897932385L);
} // namespace n634

namespace n635a
{
    template <std::integral T>
    using integral_vector = std::vector<T>;
}

namespace n635b
{
    template <typename T>
        requires std::integral<T>
    using integral_vector = std::vector<T>;
}

// Learning more ways to specify constraints

namespace n637a
{
    template <typename T>
    concept addable = requires(T a, T b) { a + b; }; // requires expression

    template <typename T>
        requires addable<T>                          // requires clause with concept
    auto
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n637a

namespace n637b
{
    // confusing syntax (requires requires ...); prefer n637a

    template <typename T>
    //                "anonymous concept"
    //           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        requires requires(T a, T b) { a + b; } // requires clause with requires expression
    auto
    add(T a, T b)
    {
        return a + b;
    }
} // namespace n637b

// Using concepts to constrain auto parameters

namespace n636a
{
    // In C++20 you can use the auto specifier in the function parameter list.
    // This has the effect of transforming the function into a template function.
    // Such a function using auto for function parameters is called an ABBREVIATED FUNCTION TEMPLATE.

    // abbreviated function template
    auto
    add(auto a, auto b)
    {
        return a + b;
    }

    // basically the same
    template <>
    auto
    add(char const *a, char const *b)
    {
        return std::string(a) + std::string(b);
    }
} // namespace n636a

namespace n636b
{
    // constrained abbreviated function template (using concepts)
    auto
    add(std::integral auto a, std::integral auto b)
    {
        return a + b;
    }
} // namespace n636b

namespace n636c
{
    // constrained auto can also be used for variadic function templates

    auto
    add(std::integral auto... args)
    {
        return (args + ...);
    }
} // namespace n636c

namespace n636d
{
    // constrained auto with generic lambdas
    auto sum = [](std::integral auto a, std::integral auto b) { return a + b; };

    // constrained auto with template(d) lambdas
    auto twice = []<std::integral T>(T a) { return a + a; };

} // namespace n636d

int
main()
{
    {
        std::println("\n====================== using namespace n601 =============================");

        using namespace n601;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));       // 43
        std::println("{}", add(42.0, 1.0));   // 43
        std::println("{}", add("42"s, "1"s)); // 421

        // add("42", "1");                    // error: cannot add two pointers
    }

    {
        std::println("\n====================== using namespace n602 =============================");

        using namespace n602;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        // add("42"s, "1"s);                   // error: no matching overloaded function found
        // add("42", "1");                     // error: no matching overloaded function found
    }

    {
        std::println("\n====================== using namespace n603 =============================");

        using namespace n603;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        // add("42"s, "1"s);                // error: Arithmetic type required
        // add("42", "1");                  // error: Arithmetic type required
    }

    {
        std::println("\n====================== using namespace n604 =============================");

        using namespace n604;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        // add("42"s, "1"s); // error: the associated constraints are not satisfied
        // add("42", "1");   // error: the associated constraints are not satisfied
    }

    {
        std::println("\n====================== using namespace n605 =============================");

        using namespace n605;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        // add("42"s, "1"s); // error: the associated constraints are not satisfied
        // add("42", "1");   // error: the associated constraints are not satisfied
    }

    {
        std::println("\n====================== using namespace n608 =============================");

        using namespace n608;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        std::println("{}", mul(42, 1));     // 42
        std::println("{}", mul(42.0, 1.0)); // 42

        // add("42"s, "1"s); // error: the associated constraints are not satisfied
        // add("42", "1");   // error: the associated constraints are not satisfied
    }

    {
        std::println("\n====================== using namespace n606 =============================");

        // A concept is a set of named constraints.
        // A concept is defined with the concept keyword and template syntax.
        // Concepts are assigned a Boolean value, but concept names should not contain verbs,
        // more like datatype names and attributes.

        using namespace n606;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        std::println("{}", mul(42, 1));     // 42
        std::println("{}", mul(42.0, 1.0)); // 42

        // add("42"s, "1"s); // error: the associated constraints are not satisfied
        // add("42", "1");   // error: the associated constraints are not satisfied
    }

    {
        std::println("\n====================== using namespace n607 =============================");

        using namespace n607;
        using namespace std::string_literals;

        std::println("{}", add(42, 1));     // 43
        std::println("{}", add(42.0, 1.0)); // 43

        // add("42"s, "1"s); // error: the associated constraints are not satisfied
        // add("42", "1");   // error: the associated constraints are not satisfied
    }

    {
        std::println("\n====================== using namespace n610 =============================");

        using namespace n610;

        static_assert(not is_container_v<foo>);
        static_assert(is_container_v<std::vector<foo>>);
    }

    {
        std::println("\n====================== using namespace n611 =============================");

        using namespace n611;

        static_assert(not container<foo>);
        static_assert(container<std::vector<foo>>);

        // process(foo{});                         // error
        process(std::vector{foo{}, foo{}, foo{}}); // Ok
    }

    {
        std::println("\n====================== using namespace n612 =============================");

        using namespace n612;

        console_logger cl;
        log_error(cl);

        stream_logger sl [[maybe_unused]];
        // log_error(sl); // error: the associated constraints are not satisfied
    }

    {
        std::println("\n====================== using namespace n614 =============================");

        using namespace n614;

        invoke(f<int>, 42);

        // invoke(g<int>, 42); // error
    }

    {
        std::println("\n====================== using namespace n613 =============================");

        using namespace n613;
        using namespace std;

        static_assert(KVP<key_value_pair<int, string>>);
        static_assert(not KVP<pair<int, string>>);   // std::pair's inner types are called first_type, second_type;
                                                     // not key_type, value_type

        static_assert(not containerizeable<string>); // string is not an arithmetic type
        static_assert(containerizeable<int>);        // int is an arithmetic type
    }

    {
        std::println("\n====================== using namespace n615 =============================");

        using namespace n615;

        // are timers
        static_assert(timer<timerA>);
        static_assert(timer<timerB>);

        // is not a timer
        static_assert(not timer<timerC>);
    }

    {
        std::println("\n====================== using namespace n616 =============================");

        using namespace n616;

        static_assert(HomogenousRange<int, int>);

        static_assert(not HomogenousRange<int>);
        static_assert(not HomogenousRange<int, double>);
        static_assert(not HomogenousRange<float, double>);

        std::println("{}", add(1, 2));     // 3
        std::println("{}", add(1.0, 2.0)); // 3

        // add(1);
        // add(1, 2.0);
        // add(1.0f, 2.0);
    }

    {
        std::println("\n====================== using namespace n617 =============================");

        using namespace n617;

        std::println("{}", decrement(5)); // 4

        // std::println("{}", decrement("foo")); // error
    }

    {
        std::println("\n====================== using namespace n618 =============================");

        using namespace n618;

        std::println("{}", decrement(5)); // 4

        // std::println("{}", decrement("foo")); // error
    }

    {
        std::println("\n====================== using namespace n619 =============================");

        using namespace n619;

        std::println("{}", add(5, 4)); // 9
    }

    {
        std::println("\n====================== using namespace n620 =============================");

        using namespace n620;

        std::println("{}", add(5, 4)); // 9
    }

    {
        std::println("\n====================== using namespace n621 =============================");

        using namespace n621;

        f1<int>();
        f2<int>();
        f4<std::string>();
        f5<int>();

        // f3<int>(); // error
        // f4<int>(); // error
    }

    {
        std::println("\n====================== using namespace n622 =============================");

        using namespace n622;

        std::println("{}", add(1, 2, 3));       // 6
        std::println("{}", add(1, 2, 3, 4, 5)); // 15

        // add(1, 42.0);
    }

    {
        std::println("\n====================== using namespace n623 =============================");

        using namespace n623;

        std::println("{}", add(1, 2, 3));       // 6
        std::println("{}", add(1, 2, 3, 4));    // 10
        std::println("{}", add(1, 2, 3, 4, 5)); // 15

        // add(1, 42.0);
    }

    {
        std::println("\n====================== using namespace n624 =============================");

        // Constraints normalization is the process of transforming the constraint expression
        // into conjunctions and disjunctions of atomic constraints.

        using namespace n624;

        std::println("{}", add(1, 2));     // 3
        std::println("{}", add(1.0, 2.0)); // 3
    }

    {
        std::println("\n====================== using namespace n625 =============================");

        using namespace n625;

        std::println("{}", add(1.0, 2.0)); // 3
        std::println("{}", add(1, 2));     // 3
    }

    {
        std::println("\n====================== using namespace n626 =============================");

        using namespace n626;

        std::println("{}", add((short)1, (short)2)); // 3
        // std::println("{}", add(1, 2));            // could be either one
    }

    {
        std::println("\n====================== using namespace n627 =============================");

        using namespace n627;

        std::println("{}", add((short)1, (short)2)); // 3
        // std::println("{}", add(1, 2));            // could be either one (using type traits)
    }

    {
        std::println("\n====================== using namespace n628 =============================");

        using namespace n628;

        std::println("{}", add((short)1, (short)2)); // 3
        std::println("{}", add(1, 2));               // 3 (with concepts this is the more specific one)
    }

    {
        std::println("\n====================== using namespace n629 =============================");

        using namespace n629;

        std::println("{}", add((short)1, (short)2)); // 3
        std::println("{}", add(1, 2));               // 3
    }

    {
        std::println("\n====================== using namespace n630 =============================");

        using namespace n630;

        wrapper<int> a [[maybe_unused]]{42};

        // error: 42 not convertible to string_view
        // if (a == 42)
        // {
        //     // ...
        // }

        wrapper<char const *> b{"42"};

        if (b == "42")
        {
            // ...
        }
    }

    {
        std::println("\n====================== using namespace n631a ============================");

        using namespace n631a;

        wrapper<int> a [[maybe_unused]] = 42;

        // error: Call to deleted constructor of 'std::unique_ptr<int>'
        // wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42);
    }

    {
        std::println("\n====================== using namespace n631b ============================");

        using namespace n631b;

        wrapper<int> a [[maybe_unused]] = 42;

        // error: No viable conversion from 'unique_ptr<int>' to 'wrapper<std::unique_ptr<int>>'
        // wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42);
    }

    {
        std::println("\n====================== using namespace n631c ============================");

        using namespace n631c;

        wrapper<int> a [[maybe_unused]] = 42;

        // error: No viable conversion from 'unique_ptr<int>' to 'wrapper<std::unique_ptr<int>>'
        // wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42);
    }

    {
        std::println("\n====================== using namespace n633a ============================");

        // Constraining class templates

        using namespace n633a;

        wrapper<int> a [[maybe_unused]]{42};

        // Constraints not satisfied for class template 'wrapper' [with T = double]
        // wrapper<double> b{ 42.0 };
    }

    {
        std::println("\n====================== using namespace n633b ============================");

        using namespace n633b;

        wrapper<short> a{42};
        std::println("{}", a.value);              // 42

        wrapper<int> b{0x11223344};
        std::println("{:x}", b.value);            // 11223344
        std::println("{:x}", (int)b.bytes.byte1); // 11
        std::println("{:x}", (int)b.bytes.byte2); // 22
        std::println("{:x}", (int)b.bytes.byte3); // 33
        std::println("{:x}", (int)b.bytes.byte4); // 44
    }

    {
        std::println("\n====================== using namespace n634 =============================");

        // Constraining variable templates

        using namespace n634;

        std::println("{}", PI<double>);

        // std::println("{}", PI<int>); // error: Constraints not satisfied for variable template 'PI' [with T = int]
    }

    {
        std::println("\n====================== using namespace n635a ============================");

        // Constraining template aliases

        using namespace n635a;

        integral_vector<int> v1{1, 2, 3};

        // error: Constraints not satisfied for alias template 'integral_vector' [with T = double]
        // integral_vector<double> v2{1.0, 2.0, 3.0};
    }

    {
        std::println("\n====================== using namespace n635b ============================");

        // Constraining template aliases

        using namespace n635b;

        integral_vector<int> v1{1, 2, 3};

        // Constraints not satisfied for alias template 'integral_vector' [with T = double]
        // integral_vector<double> v2 {1.0, 2.0, 3.0};
    }

    {
        std::println("\n====================== using namespace n637a ============================");

        // Learning more ways to specify constraints

        using namespace n637a;

        std::println("{}", add(1, 2)); // 3
    }

    {
        std::println("\n====================== using namespace n637b ============================");

        // Learning more ways to specify constraints

        using namespace n637b;

        std::println("{}", add(1, 2)); // 3
    }

    {
        std::println("\n====================== using namespace n636a ============================");

        // Using concepts to constrain auto parameters

        using namespace n636a;

        // no constraints
        std::println("{}", add(4, 2));     // 6
        std::println("{}", add(4.0, 2));   // 6
        std::println("{}", add("4", "2")); // 42
    }

    {
        std::println("\n====================== using namespace n636b ============================");

        // Using concepts to constrain auto parameters

        using namespace n636b;

        // with constraints
        std::println("{}", add(4, 2)); // 6

        // std::println("{}", add(4.2, 0)); // error
    }

    {
        std::println("\n====================== using namespace n636c ============================");

        using namespace n636c;

        add(1, 2, 3);

        // add(1.0, 2.0, 3.0); // error
    }

    {
        std::println("\n====================== using namespace n636d ============================");

        using namespace n636d;

        std::println("{}", sum(1, 2)); // 3
        std::println("{}", twice(2));  // 4
    }
}
