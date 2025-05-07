#include <cxxabi.h>
#include <map>
#include <memory>
#include <mutex>
#include <print>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

#pragma GCC diagnostic ignored "-Wunused"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

// Advanced Template Concepts

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

// Understanding name binding and dependent names

namespace n401
{
    // Dependent names are names that depend on template parameters.

    // Name lookup:
    // - For non-dependent names, it is performed at the point of the template definition    (early-binding).
    // - For     dependent names, it is performed at the point of     template instantiation (late-binding).

    template <typename T> // [1] template declaration
    struct parser;

    void                  // [2] handle(double) definition
    handle(double value)
    {
        std::println("processing a double: {}", value);
    }

    template <typename T> // [3] template definition
    struct parser
    {
        void
        parse()
        {
            handle(42); // [4] non-dependent name (calls handle(double))
        }
    };

    void                // [5] handle(int) definition
    handle(int value)
    {
        std::println("processing an int: {}", value);
    }
} // namespace n401

namespace n402
{
    // primary
    template <typename T> // [1] template definition
    struct handler
    {
        void
        handle(T value)   // handle is a dependent name
        {
            std::println("handler<T>: {}", value);
        }
    };

    template <typename T> // [2] template definition
    struct parser
    {
        void
        parse(T arg)
        {
            arg.handle(42); // [3] dependent name (handle depends on template parameter arg)
        }
    };

    template <>             // [4] template specialization
    struct handler<int>
    {
        void
        handle(int value)
        {
            std::println("handler<int>: {}", value);
        }
    };
} // namespace n402

// Two-phase name lookup

namespace n403
{
    // - The first phase occurs at the point of the definition when the template syntax is
    //   checked and names are categorized as dependent or non-dependent.
    // - The second phase occurs at the point of instantiation when the template arguments
    //   are substituted for the template parameters. Name binding for dependent names
    //   happens at this point.

    template <typename T>
    struct base_parser
    {
        void
        init()
        {
            std::println("init");
        }
    };

    template <typename T>
    struct parser : base_parser<T>
    {
        void
        parse()
        {
            // init(); error: Use of undeclared identifier 'init'
            //         init is a non-dependent name and must be known at this point

            this->init();           // now init is a dependent type
            base_parser<T>::init(); // now init is a dependent type

            std::println("parse");
        }
    };
} // namespace n403

namespace n404
{
    template <typename T>
    struct base_parser
    {
        void
        init()
        {
            std::println("init");
        }
    };

    template <typename T>
    struct parser : base_parser<T>
    {
        void
        parse()
        {
            this->init();

            std::println("parse");
        }
    };

    template <>
    struct base_parser<int>
    {
        void
        init()
        {
            std::println("specialized init");
        }
    };
} // namespace n404

// Dependent type names

namespace n405
{
    template <typename T>
    struct base_parser
    {
        using value_type = T;
    };

    template <typename T>
    struct parser : base_parser<T>
    {
        void
        parse()
        {
            // value_type v{};                 // error: Unknown type name 'value_type'
            // value_type is a non-dependent name; not known at this point

            // base_parser<T>::value_type v{}; // error: Missing 'typename' prior to dependent type name
            // A specialization of base_parser may follow and value_type could be defined as something else than a type.

            // tell the compiler explicitly that this is a type (by default the compiler assumes it's not a type):
            typename base_parser<T>::value_type v{};

            std::println("parse");
        }
    };
} // namespace n405

namespace n446
{
    // In C++20, typename is implicit in the following contexts:
    // The compiler is now able to deduce type names in a multitude of contexts.
    // - In using declarations
    // - In the declaration of data members
    // - In the declaration or definition of function parameters
    // - In trailing return types
    // - In default arguments of type-parameters of a template
    // - In the type-id of a static_cast, const_cast, reinterpret_cast, or dynamic_cast statement

    struct dictionary_traits
    {
        using key_type = int;
        using map_type = std::map<key_type, std::string>;

        static constexpr int identity = 1;
    };

    template <typename T>
    struct dictionary : T::map_type                  // typename not needed
                                                     // 'typename' is redundant; base classes are implicitly types
    {
        int         start_key{T::identity};          // typename not needed
        T::key_type next_key;                        // typename not needed

        using value_type = T::map_type::mapped_type; // typename not needed (would be std::string for dictionary_traits)

        void
        add(T::key_type const &, value_type const &) // typename not needed
        {
        }
    };
} // namespace n446

// Dependent template names

// NOTE: Write code without `template` and in case of error just add it (possibly with the help of language server).
//       Write code with    `typename` and after it works try to remove it.

namespace n406
{
    template <typename T>
    struct base_parser
    {
        template <typename U>
        void
        init()
        {
            std::println("init");
        }

        template <typename U>
        struct token
        {
        };
    };

    template <typename T>
    struct parser : base_parser<T>
    {
        void
        parse()
        {
            // token and init are templated (class and function template)!!!
            // Use `template` to refer to them.

            // Keep in mind that the template keyword can only follow the
            // - scope resolution operator (::),     X::template    foo<T>()
            // - member access through pointer (->), this->template foo<T>()
            // - and the member access (.)           obj.template   foo<T>()

            // error: Use 'template' keyword to treat 'init' as a dependent template name:
            // base_parser<T>::init<int>();
            base_parser<T>::template init<int>();

            // here it's obvious ('using') that we are dealing with a type -> typename not necessary
            // using token_type = typename base_parser<T>::template token<int>;
            using token_type = base_parser<T>::template token<int>;
            token_type t1{};

            // must use typename to tell the compiler it's a type:
            // template can also refer to a class (here inner class)
            typename base_parser<T>::template token<int> t2{};

            std::println("parse");
        }
    };
} // namespace n406

// Current instantiation

namespace n407
{
    // This example is now obsolete. Compiler can deduce everything.

    // Names are categorized as dependent (those that depend on
    // a template parameter) and non-dependent (those that don’t depend on a template
    // parameter). Name binding happens at the point of definition for non-dependent types
    // and at the point of instantiation for dependent types. In some cases, the keywords
    // typename and template are required to disambiguate the use of names and tell the
    // compiler that a name refers to a type or a template.

    template <typename T>
    struct parser
    {
        // CI = current implementation
        parser          *p1;        // parser is the CI
        parser<T>       *p2;        // parser<T> is the CI
        n407::parser<T> *p3;        // ::parser<T> is the CI
        parser<T *>      p4;        // parser<T*> is the CI

        struct token
        {
            token              *t1; // token is the CI
            parser<T>::token   *t2; // parser<T>::token is the CI
            parser<T *>::token *t3; // parser<T*>::token is the CI
        };
    };

    template <typename T>
    struct parser<T *>
    {
        parser<T *> *p1; // parser<T*> is the CI
        parser<T>   *p2; // parser<T> is the CI
    };
} // namespace n407

// Exploring template recursion

namespace n408
{
    // class template

    template <unsigned int N>
    struct factorial
    {
        static constexpr unsigned int value = N * factorial<N - 1>::value;
    };

    template <>
    struct factorial<0>
    {
        static constexpr unsigned int value = 1;
    };

    // API
    template <unsigned int N>
    inline constexpr unsigned int factorial_v = factorial<N>::value;
} // namespace n408

namespace n409
{
    // variable template

    template <unsigned int N>
    inline constexpr unsigned int factorial = N * factorial<N - 1>;

    template <>
    inline constexpr unsigned int factorial<0> = 1;
} // namespace n409

namespace n409b
{
    // function template

    template <unsigned int n>
    constexpr unsigned int
    factorial()
    {
        return n * factorial<n - 1>();
    }

    template <>
    constexpr unsigned int
    factorial<0>()
    {
        return 1;
    }
} // namespace n409b

namespace n410
{
    // using constexpr of factorial function (no template!!!)

    constexpr unsigned int
    factorial(unsigned int const n)
    {
        return n > 1 ? n * factorial(n - 1) : 1;
    }
} // namespace n410

namespace n410
{
    // wrapper wraps type T
    template <typename T>
    struct wrapper
    {
    };

    template <int N>
    struct manyfold_wrapper
    {
        using value_type = wrapper<typename manyfold_wrapper<N - 1>::value_type>;
    };

    template <>
    struct manyfold_wrapper<0>
    {
        using value_type = unsigned int;
    };
} // namespace n410

namespace n411
{
    // variable template

    template <unsigned int N>
    inline constexpr unsigned int sum = N + sum<N - 1>;

    template <>
    inline constexpr unsigned int sum<0> = 0;
} // namespace n411

// Function template argument deduction

namespace n412
{
    template <typename T>
    void
    process(T arg)
    {
        std::println("processing {}", arg);
    }

    struct account_t
    {
        int number;

        int
        get_number()
        {
            return number;
        }

        int
        from_string(std::string text)
        {
            return std::atoi(text.c_str());
        }
    };

    struct transaction_t
    {
        double amount;
    };

    struct balance_report_t
    {
    };

    struct balance_t
    {
        account_t account;
        double    amount;

        account_t
        get_account()
        {
            return account;
        }

        int
        get_account_number()
        {
            return account.number;
        }

        bool
        can_withdraw(double const value)
        {
            return amount >= value;
        };

        transaction_t
        withdraw(double const value)
        {
            amount -= value;
            return transaction_t{-value};
        }

        balance_report_t
        make_report(int const report_type)
        {
            return {};
        }
    };

    template <typename T>
    void
    process01(T)
    {
        std::println("T");
    }

    template <typename T>
    void
    process02(T const)
    {
        std::println("T const");
    }

    // template <typename T>
    // void
    // process03(T volatile)
    // {
    //     std::println("T volatile");
    // }

    template <typename T>
    void
    process04(T *)
    {
        std::println("T*");
    }

    template <typename T>
    void
    process04(T &)
    {
        std::println("T&");
    }

    template <typename T>
    void
    process05(T &&)
    {
        std::println("T&&");
    }

    template <typename T>
    void
    process06(T[5])
    {
        std::println("T[5]");
    }

    template <size_t n>
    void
    process07(account_t[5][n])
    {
        std::println("C[5][{}]", n);
    }

    template <typename T>
    void
    process08(T (*)())
    {
        std::println("T (*)()");
    }

    template <typename T>
    void
    process08(account_t (*)(T))
    {
        std::println("account_t (*)(T)");
    }

    template <typename T, typename U>
    void
    process08(T (*)(U))
    {
        std::println("T (*)(U)");
    }

    template <typename T>
    void
    process09(T (account_t::*)())
    {
        std::println("T (C::*)()");
    }

    template <typename T, typename U>
    void
    process09(T (account_t::*)(U))
    {
        std::println("T (C::*)(U)");
    }

    template <typename T, typename U>
    void
    process09(T (U::*)())
    {
        std::println("T (U::*)()");
    }

    template <typename T, typename U, typename V>
    void
    process09(T (U::*)(V))
    {
        std::println("T (U::*)(V)");
    }

    template <typename T>
    void
    process09(account_t (T::*)())
    {
        std::println("C (T::*)()");
    }

    template <typename T, typename U>
    void
    process09(transaction_t (T::*)(U))
    {
        std::println("C (T::*)(U)");
    }

    template <typename T>
    void
    process09(balance_report_t (balance_t::*)(T))
    {
        std::println("D (C::*)(T)");
    }

    template <typename T>
    void
    process10(T account_t::*)
    {
        std::println("T C::*");
    }

    template <typename T>
    void
    process10(account_t T::*)
    {
        std::println("C T::*");
    }

    template <typename T, typename U>
    void
    process10(T U::*)
    {
        std::println("T U::*");
    }

    template <typename T>
    struct wrapper
    {
        T data;
    };

    template <size_t i>
    struct int_array
    {
        int data[i];
    };

    template <typename T>
    void
    process11(wrapper<T>)
    {
        std::println("C<T>");
    }

    template <size_t i>
    void
    process12(int_array<i>)
    {
        std::println("C<{}>", i);
    }

    template <template <typename> typename TT, typename T>
    void
    process13(TT<T>)
    {
        std::println("TT<T>");
    }

    template <template <size_t> typename TT, size_t i>
    void
    process14(TT<i>)
    {
        std::println("TT<{}>", i);
    }

    template <template <typename> typename TT>
    void
    process15(TT<account_t>)
    {
        std::println("TT<C>");
    }
} // namespace n412

namespace n413
{
    template <typename T, T i>
    void
    process(double arr[i])
    {
        using index_type = T;

        std::println("processing {} doubles", i);
        std::println("index type is {}", typeid(T).name());
        std::println("index type is {}", typeid(index_type).name());
    }
} // namespace n413

namespace n414
{
    template <typename T>
    void
    process(T a = 0, T b = 42)
    {
        std::println("{}, {}", a, b);
    }
} // namespace n414

namespace n415
{
    template <typename T>
    void
    invoke(void (*pf)(T, int))
    {
        pf(T{}, 42);
    }

    namespace
    {
        template <typename T>
        void
        alpha(T, int)
        {
            std::println("alpha(T,int)");
        }
    } // namespace

    namespace
    {
        void
        beta(int, int)
        {
            std::println("beta(int,int)");
        }

        void
        beta(short, int)
        {
            std::println("beta(short,int)");
        }
    } // namespace

    namespace
    {
        void
        gamma(short, int, long long)
        {
            std::println("gamma(short,int,long long)");
        }
        void
        gamma(double, int)
        {
            std::println("gamma(double,int)");
        }
    } // namespace
} // namespace n415

namespace n416
{
    template <size_t Size>
    void
    process1(int a[Size])
    {
        std::println("process(int[Size])");
    }

    template <size_t Size>
    void
    process2(int a[5][Size])
    {
        std::println("process(int[5][{}])", Size);
    }

    template <size_t Size>
    void
    process3(int (&a)[Size]) // reference
    {
        std::println("process(int[{}]&)", Size);
    }

    template <size_t Size>
    void
    process4(int (*a)[Size]) // pointer
    {
        std::println("process(int[{}]*)", Size);
    }
} // namespace n416

namespace n417
{
    template <size_t N>
    struct ncube
    {
        static constexpr size_t dimensions = N;
    };

    template <size_t N>
    void
    process(ncube<N - 1> cube)
    {
        std::println("{}", cube.dimensions);
    }
} // namespace n417

// Class template argument deduction

namespace n418
{
    template <typename T>
    struct wrapper
    {
        T data;
    };

    // Before C++17, template argument deduction only worked for functions but not classes.

    // but easy to write maker helper function
    template <typename T>
    constexpr wrapper<T>
    make_wrapper(T &&data)
    {
        return wrapper{data};
    }

    // deduction guide (not needed in C++20)
    template <typename T>
    wrapper(T) -> wrapper<T>;

    // maker helper function using template function argument deduction
    template <typename T, typename... Ts, typename Allocator = std::allocator<T>>
    auto
    make_vector(T &&first, Ts &&...args)
    {
        return std::vector<std::decay_t<T>, Allocator> //
            {std::forward<T>(first),                   //
             std::forward<Ts>(args)...};
    }
} // namespace n418

namespace n419
{
    template <typename T>
    struct point_t
    {
        point_t(T x, T y) : m_x(x), m_y(y)
        {
        }

        T
        get_x() const
        {
            return m_x;
        }

        T
        get_y() const
        {
            return m_y;
        }

      private:
        T m_x;
        T m_y;
    };
} // namespace n419

namespace n420
{
    template <typename T>
    struct range_t
    {
        template <typename Iter>
        range_t(Iter first, Iter last)
        {
            std::copy(first, last, std::back_inserter(data));
        }

      private:
        std::vector<T> data;
    };

    // deduction guide
    // function template
    template <typename Iter>
    // "constructor"               ->     return type
    range_t(Iter first, Iter last) -> range_t<typename std::iterator_traits<Iter>::value_type>;
} // namespace n420

// Forwarding references

namespace n421
{
    struct wrapper
    {
        int data;
    };

    namespace
    {
        void
        f(wrapper &v) // accepts lvalues
        {
            std::println("f(wrapper&): {}", v.data);
        }

        void
        f(wrapper const &v) // accepts const lvalues and rvalues
        {
            std::println("f(wrapper const &): {}", v.data);
        }
    } // namespace

    namespace
    {
        // lvalue
        void
        g(wrapper &v) // accepts lvalues
        {
            std::println("g(wrapper&): {}", v.data);
        }

        // rvalue
        void
        g(wrapper &&v) // accepts rvalues
        {
            std::println("g(wrapper&&): {}", v.data);
        }

        // const lvalue
        void
        g(wrapper const &v) // accepts const lvalues and rvalues
        {
            std::println("g(wrapper&&): {}", v.data);
        }
    } // namespace

    namespace
    {
        void
        h(wrapper &&v)
        {
            std::println("h(wrapper&&): {}", v.data);
        }
    } // namespace
} // namespace n421

namespace n422
{
    struct wrapper
    {
        int data;
    };

    namespace
    {
        void
        g(wrapper &v)
        {
            std::println("g(wrapper&): {}", v.data);
        }

        void
        g(wrapper &&v)
        {
            std::println("g(wrapper&&): {}", v.data);
        }
    } // namespace

    namespace
    {
        void
        h(wrapper &v)
        {
            g(v);
        }

        void
        h(wrapper &&v)
        {
            g(v);
        }
    } // namespace
} // namespace n422

namespace n423
{
    struct wrapper
    {
        int data;
    };

    namespace
    {
        void
        g(wrapper &v)
        {
            std::println("g(wrapper&): {}", v.data);
        }

        void
        g(wrapper &&v)
        {
            std::println("g(wrapper&&): {}", v.data);
        }
    } // namespace

    namespace
    {
        // template<class T>
        // void wrapper(T&& arg)
        // {
        //     // arg is always lvalue
        //     foo(std::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
        // }
        //
        // std::forward<T>(arg)
        //
        // When arg is a forwarding reference, this overload forwards the argument
        // to another function with the VALUE CATEGORY it had when passed to
        // the calling function.

        void
        h(wrapper &v)
        {
            g(std::forward<wrapper &>(v)); // not a function template, we need to specify explicitly the original type
        }

        void
        h(wrapper &&v)
        {
            g(std::forward<wrapper &&>(v)); // not a function template, we need to specify explicitly the original type
        }
    } // namespace
} // namespace n423

namespace n424
{
    namespace
    {
        // template <typename T>
        // void
        // f(T &)
        // {
        //     std::println("f(T&)");
        // }

        template <typename T>
        void
        f(T &&) // forwarding reference (can take l-values and r-values)
        {
            std::println("f(T&&)");
        }

        template <typename T>
        void
        f(T const &&) // rvalue reference (not a forwarding/universal reference!)
        {
            std::println("f(T const&&)");
        }

        template <typename T>
        void
        f(std::vector<T> &&) // rvalue reference
        {
            std::println("f(vector<T>&&)");
        }
    } // namespace

    template <typename T>
    struct S
    {
        void
        f(T &&) // rvalue (not a forwarding/universal reference!)
        {
            std::println("S.f(T&&)");
        }
    };
} // namespace n424

namespace n425
{
    struct foo
    {
        int data;
    };

    void
    g(foo &v)
    {
        std::println("g(foo&)");
    }

    void
    g(foo &&v)
    {
        std::println("g(foo&&)");
    }

    template <typename T>
    void
    h(T &v)
    {
        g(v);
    }

    template <typename T>
    void
    h(T &&v)
    {
        g(v);
    }
} // namespace n425

namespace n426
{
    struct foo
    {
        int data;
    };

    void
    g(foo &v)
    {
        std::println("g(foo&)");
    }

    void
    g(foo &&v)
    {
        std::println("g(foo&&)");
    }

    // Not needed anymore for perfect forwarding!
    // template <typename T>
    // void
    // h(T &v)
    // {
    //     g(v); // v is an l-value
    // }

    template <typename T>
    void
    h(T &&v)
    {
        g(std::forward<T>(v)); // l-values remain l-values (so do r-values)
        // g(std::forward<T &&>(v)); // the same, doesn't matter
    }
} // namespace n426

namespace n427
{
    struct foo
    {
        int data;
    };

    void
    g(foo &v)
    {
        std::println("g(foo&)");
    }

    void
    g(foo &&v)
    {
        std::println("g(foo&&)");
    }

    template <typename T>
    void
    h(T &&v)
    {
        g(std::forward<T>(v));
        // g(std::forward<T &&>(v)); // the same, doesn't matter
    }
} // namespace n427

// The decltype specifier

namespace n428
{
    int
    f()
    {
        return 42;
    }

    int
    g()
    {
        return 0;
    }

    int
    g(int a)
    {
        return a;
    }

    int
    g(int a, int b)
    {
        return a + b;
    }

    struct wrapper
    {
        int val;

        int
        get() const
        {
            return val;
        }
    };
} // namespace n428

namespace n429
{
    struct foo
    {
        int          a = 0;
        volatile int b = 0;
        const int    c = 42;
    };
} // namespace n429

namespace n430
{
    template <typename T>
    T
    minimum(T &&a, T &&b)
    {
        return a < b ? a : b;
    }
} // namespace n430

namespace n431
{
    // C++11: we can use the auto specifier with a trailing return type using decltype.

    template <typename T, typename U>
    auto
    minimum(T &&a, U &&b) -> decltype(a < b ? a : b)
    {
        return a < b ? a : b;
    }
} // namespace n431

namespace n432
{
    // C++14: decltype(auto)

    template <typename T, typename U>
    decltype(auto)
    minimum(T &&a, U &&b)
    {
        return a < b ? a : b;
    }
} // namespace n432

namespace n433
{
    // Just using auto has the same effect than decltype(auto) in this case.
    // no const and ref specifiers involved

    template <typename T, typename U>
    auto
    minimum(T &&a, U &&b)
    {
        return a < b ? a : b;
    }
} // namespace n433

namespace n434
{
    template <typename T>
    T const &
    func(T const &ref)
    {
        return ref;
    }

    template <typename T>
    auto // const and ref qualifiers are being discarded
    func_caller(T &&ref)
    {
        return func(std::forward<T>(ref));
    }
} // namespace n434

namespace n435
{
    template <typename T>
    T const &
    func(T const &ref)
    {
        return ref;
    }

    template <typename T>
    decltype(auto) // perfect returning (= perfect forwarding of return type)
    func_caller(T &&ref)
    {
        return func(std::forward<T>(ref));
    }
} // namespace n435

// The std::declval type operator

namespace n436
{
    template <typename T, typename U>
    struct composition
    {
        // uses default constructor for T and U
        using result_type = decltype(T{} + U{}); // not decltype(T + U) because these are types
    };

    // wrapper has no default constructor
    struct wrapper
    {
        int value;

        wrapper(int const v) : value(v)
        {
        }

        friend wrapper
        operator+(int const a, wrapper const &w)
        {
            return wrapper(a + w.value);
        }

        friend wrapper
        operator+(wrapper const &w, int const a)
        {
            return wrapper(a + w.value);
        }
    };
} // namespace n436

namespace n437
{
    template <typename T, typename U>
    struct composition
    {
        // no default constructor needed for T and U
        using result_type = decltype(std::declval<T>() + std::declval<U>());
    };

    // wrapper has no default constructor
    struct wrapper
    {
        wrapper(int const v) : value(v)
        {
        }

        int value;

        friend wrapper
        operator+(int const a, wrapper const &w)
        {
            return wrapper(a + w.value);
        }

        friend wrapper
        operator+(wrapper const &w, int const a)
        {
            return wrapper(a + w.value);
        }
    };
} // namespace n437

// Understanding friendship in templates

namespace n438
{
    // Friends have access to all - including the non-public - members of a class.

    // If you need to grant access to some private members but not all, this is
    // possible with the help of the client-attorney pattern.
    //
    // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Friendship_and_the_Attorney-Client

    struct wrapper
    {
        wrapper(int const v) : value(v)
        {
        }

      private:
        int value;

        friend void print(wrapper const &w);
    };

    void
    print(wrapper const &w)
    {
        std::println("{}", w.value);
    }
} // namespace n438

namespace n439
{
    struct wrapper
    {
        wrapper(int const v) : value(v)
        {
        }

      private:
        int value;

        template <typename T>
        friend void print(wrapper const &);

        template <typename T>
        friend struct printer;
    };

    // T not used in print()
    template <typename T>
    void
    print(wrapper const &w)
    {
        std::println("{}", w.value);
    }

    // T not used in printer
    template <typename T>
    struct printer
    {
        void
        operator()(wrapper const &w)
        {
            std::println("{}", w.value);
        }
    };
} // namespace n439

namespace n440
{
    // declarations

    struct wrapper;

    template <typename T>
    void print(wrapper const &w);

    template <typename T>
    struct printer;

    // definitions

    struct wrapper
    {
        wrapper(int const v) : value(v)
        {
        }

      private:
        int value;

        friend void print<int>(wrapper const &); // only int specializations are friends
        friend struct printer<int>;              // only int specializations are friends
    };

    // is not a friend
    template <typename T>
    void
    print(wrapper const &w)
    {
        // error: 'value' is a private member of 'n440::wrapper'
        // std::println("{}", w.value);
    }

    // is a friend
    template <>
    void
    print<int>(wrapper const &w)
    {
        std::println("{}", w.value);
    }

    // is not a friend
    template <typename T>
    struct printer
    {
        void
        operator()(wrapper const &w)
        {
            // error: 'value' is a private member of 'n440::wrapper'
            // std::println("{}", w.value);
        }
    };

    // is a friend
    template <>
    struct printer<int>
    {
        void
        operator()(wrapper const &w)
        {
            std::println("{}", w.value);
        }
    };
} // namespace n440

namespace n441
{
    template <typename T>
    struct wrapper
    {
        wrapper(T const v) : value(v)
        {
        }

      private:
        T value;

        // only int specialization of wrapper are friends
        friend void print(wrapper<int> const &);
    };

    // is a friend
    void
    print(wrapper<int> const &w)
    {
        std::println("{}", w.value);
    }

    // is not a friend
    void
    print(wrapper<char> const &w)
    {
        // std::println("{}", w.value << '\n'; // error
    }
} // namespace n441

namespace n442
{

    // declarations

    template <typename T>
    struct wrapper;

    template <typename T>
    void print(wrapper<T> const &w);

    template <typename T>
    struct printer;

    // definitions

    template <typename T>
    struct wrapper
    {
        wrapper(T const v) : value(v)
        {
        }

      private:
        T value;

        friend void print<int>(wrapper<int> const &);
        friend struct printer<int>;
    };

    // is not a friend
    template <typename T>
    void
    print(wrapper<T> const &w)
    {
        // std::println("{}", w.value);
    }

    // is a friend
    template <>
    void
    print<int>(wrapper<int> const &w)
    {
        std::println("{}", w.value);
    }

    // is not a friend
    template <typename T>
    struct printer
    {
        void
        operator()(wrapper<T> const &w)
        {
            // ...
            // std::println("{}", w.value);
        }
    };

    // is a friend
    template <>
    struct printer<int>
    {
        void
        operator()(wrapper<int> const &w)
        {
            std::println("{}", w.value);
        }
    };
} // namespace n442

namespace n443
{
    // BAD EXAMPLE !!!

    // declarations

    template <typename T>
    struct printer;

    // definitions

    template <typename T>
    struct wrapper
    {
        wrapper(T const v) : value(v)
        {
        }

      private:
        T value;

        // every wrapper is a friend (U can be different from T)
        template <typename U>
        friend void print(wrapper<U> const &);

        // every printer is a friend (U can be different from T)
        template <typename U>
        friend struct printer;
    };

    // friend
    template <typename T>
    void
    print(wrapper<T> const &w)
    {
        std::println("{}", w.value);
    }

    // friend
    template <typename T>
    struct printer
    {
        void
        operator()(wrapper<T> const &w)
        {
            std::println("{}", w.value);
        }
    };
} // namespace n443

namespace n444
{
    // declarations

    template <typename T>
    struct wrapper;

    template <typename T>
    void print(wrapper<T> const &w);

    template <typename T>
    struct printer;

    // definitions

    template <typename T>
    struct wrapper
    {
        wrapper(T const v) : value(v)
        {
        }

      private:
        T value;

        friend void print<T>(wrapper<T> const &);
        friend struct printer<T>;
    };

    // friend
    template <typename T>
    void
    print(wrapper<T> const &w)
    {
        std::println("{}", w.value);
    }

    // friend
    template <typename T>
    struct printer
    {
        void
        operator()(wrapper<T> const &w)
        {
            std::println("{}", w.value);
        }
    };
} // namespace n444

namespace n445
{
    template <typename T>
    struct connection
    {
        connection(std::string const &host, int const port) : ConnectionString(host + ":" + std::to_string(port))
        {
        }

      private:
        std::string ConnectionString;
        friend T; // T (= a type - can be a struct or class) is a friend of connection
    };

    struct executor
    {
        void
        run()
        {
            // executor is a friend and has access to private ConnectionString of connection
            connection<executor> c("localhost", 1234);
            std::println("{}", c.ConnectionString);
        }
    };
} // namespace n445

#if 1
namespace std
{
    // When encountering the name of a class template in a variable declaration or
    // function-style cast, the compiler proceeds to build a set of so-called deduction guides.
    //
    // There are fictional function templates representing constructor signatures of a fictional
    // class type. Users can also provide deduction guides and these are added to the list
    // of compiler-generated guides.
    //
    // This set of implicitly deduced guides is generated from the constructors of the class
    // template. This includes the default constructor, the copy constructor, the move
    // constructor, and all the conversion constructors, with the arguments copied in the exact
    // order. If the constructor is explicit, then so is the deduction guide. However, if the class
    // template does not have any user-defined constructor, a deduction guide is created for
    // a hypothetical default constructor. A deduction guide for a hypothetical copy constructor
    // is always created.
    //
    // The syntax is similar to that of functions with a trailing return type but without the auto keyword.
    // Deduction guides can be either functions or function templates.
    //
    // They MUST be provided in the same namespace as the class template they apply to.

    // custom deduction guides for std::pair

    // function template
    template <typename T>
    // "constructor"        ->     return type
    pair(T &, char const *) -> pair<T, std::string>;

    // function template
    template <typename T>
    pair(T &&, char const *) -> pair<T, std::string>;

    // function template
    template <typename T>
    pair(char const *, T &&) -> pair<std::string, T>;

    // function
    pair(char const *, char const *) -> pair<std::string, std::string>;
} // namespace std
#endif

int
main()
{
    {
        std::println("\n====================== using namespace n401 =============================");
        using namespace n401;

        parser<int> p; // [6] template instantiation
        p.parse();     // processing a double: 42
    }

    {
        std::println("\n====================== using namespace n402 =============================");
        using namespace n402;

        handler<int> h;         // [5] template instantiation

        parser<handler<int>> p; // [6] template instantiation

        p.parse(h);             // handler<int> : 42
    }

    {
        std::println("\n====================== using namespace n403 =============================");
        using namespace n403;

        parser<int> p;
        p.parse(); // init
                   // parse
    }

    {
        std::println("\n====================== using namespace n404 =============================");
        using namespace n404;

        parser<int> p1;
        p1.parse(); // specialized init
                    // parse

        std::println("------------------");

        parser<double> p2;
        p2.parse(); // init
                    // parse
    }

    {
        std::println("\n====================== using namespace n405 =============================");
        using namespace n405;

        parser<int> p;
        p.parse(); // parse
    }

    {
        std::println("\n====================== using namespace n406 =============================");
        using namespace n406;

        parser<int> p;
        p.parse(); // init
                   // parse
    }

    {
        std::println("\n====================== using namespace n446 =============================");
        using namespace n446;

        dictionary<dictionary_traits> d;
        d.add(1, "2");
    }

    {
        std::println("\n====================== using namespace n407 =============================");
        using namespace n407;

        parser<int> p;
    }

    {
        std::println("\n====================== using namespace n408 =============================");
        using namespace n408;

        std::println("{}", factorial<0>::value);  // 1
        std::println("{}", factorial<1>::value);  // 1
        std::println("{}", factorial<2>::value);  // 2
        std::println("{}", factorial<3>::value);  // 6
        std::println("{}", factorial<4>::value);  // 24
        std::println("{}", factorial<5>::value);  // 120
        std::println("{}", factorial<12>::value); // 479001600

        std::println("{}", factorial_v<0>);       // 1
        std::println("{}", factorial_v<1>);       // 1
        std::println("{}", factorial_v<2>);       // 2
        std::println("{}", factorial_v<3>);       // 6
        std::println("{}", factorial_v<4>);       // 24
        std::println("{}", factorial_v<5>);       // 120
        std::println("{}", factorial_v<12>);      // 479001600
    }

    {
        std::println("\n====================== using namespace n409 =============================");
        using namespace n409;

        std::println("{}", factorial<0>);  // 1
        std::println("{}", factorial<1>);  // 1
        std::println("{}", factorial<2>);  // 2
        std::println("{}", factorial<3>);  // 6
        std::println("{}", factorial<4>);  // 24
        std::println("{}", factorial<5>);  // 120
        std::println("{}", factorial<12>); // 479001600
    }

    {
        std::println("\n====================== using namespace n409b ============================");
        using namespace n409b;

        std::println("{}", factorial<1>()); // 1
        std::println("{}", factorial<2>()); // 2
        std::println("{}", factorial<3>()); // 6
        std::println("{}", factorial<4>()); // 24
        std::println("{}", factorial<5>()); // 120
    }

    {
        std::println("\n====================== using namespace n410 =============================");
        using namespace n410;

        std::println("{}", factorial(0)); // 1
        std::println("{}", factorial(1)); // 1
        std::println("{}", factorial(2)); // 2
        std::println("{}", factorial(3)); // 6
        std::println("{}", factorial(4)); // 24
        std::println("{}", factorial(5)); // 120
    }

    {
        std::println("\n====================== using namespace n410 =============================");
        using namespace n410;

        std::println("{}", demangle(typeid(manyfold_wrapper<0>::value_type).name()));
        std::println("{}", demangle(typeid(manyfold_wrapper<1>::value_type).name()));
        std::println("{}", demangle(typeid(manyfold_wrapper<2>::value_type).name()));
        std::println("{}", demangle(typeid(manyfold_wrapper<3>::value_type).name()));

        // std::println("{}", demangle(typeid(manyfold_wrapper<3000>::value_type).name()));
        // error: limit for recursively nested template exceeded (GCC 900, Clang 1024)

        // Output:
        // unsigned int
        // n410::wrapper<unsigned int>
        // n410::wrapper<n410::wrapper<unsigned int> >
        // n410::wrapper<n410::wrapper<n410::wrapper<unsigned int> > >
    }

    {
        std::println("\n====================== using namespace n411 =============================");
        using namespace n411;

        std::println("{}", sum<100>); // 5050
        std::println("{}", sum<256>); // 32896
        std::println("{}", sum<900>); // 405450

        static_assert(sum<100> == (100 * (100 + 1)) / 2);
        static_assert(sum<256> == (256 * (256 + 1)) / 2);
        static_assert(sum<900> == (900 * (900 + 1)) / 2);
    }

    {
        std::println("\n====================== using namespace n412/1 ===========================");
        using namespace n412;

        process(42);        // T is int
        process<int>(42);   // T is int, redundant
        process<short>(42); // T is short
    }

    {
        std::println("\n====================== using namespace n412/2 ===========================");
        using namespace n412;

        account_t ac{42}; //
        process01(ac);    // T
        process02(ac);    // T const

        // process03(ac); // T volatile (obsolete)
    }

    {
        std::println("\n====================== using namespace n412/3 ===========================");
        using namespace n412;

        account_t ac{42}; //
        process04(&ac);   // T*
        process04(ac);    // T&
        process05(ac);    // T&&
    }

    {
        std::println("\n====================== using namespace n412/4 ===========================");
        using namespace n412;

        account_t ac{42};     //
        process06(&ac);       // T[5], decays to a pointer

        account_t arr1[5]{};  //
        process06(arr1);      // T[5]

        account_t arr2[5][3]; //
        process07(arr2);      // C[5][3]
    }

    {
        std::println("\n====================== using namespace n412/5 ===========================");
        using namespace n412;

        // pf = pointer to function
        account_t (*pf1)()        = nullptr;
        account_t (*pf2)(int)     = nullptr;
        transaction_t (*pf3)(int) = nullptr;
        double (*pf4)(int)        = nullptr;

        process08(pf1); // T (*)()
        process08(pf2); // account_t (*)(T)
        process08(pf3); // T (*)(U)
        process08(pf4); // T (*)(U)
    }

    {
        std::println("\n====================== using namespace n412/6 ===========================");
        using namespace n412;

        // pfm = pointer to function member
        int (account_t::*pfm1)()                 = &account_t::get_number;
        int (account_t::*pfm2)(std::string)      = &account_t::from_string;
        int (balance_t::*pfm3)()                 = &balance_t::get_account_number;
        bool (balance_t::*pfm4)(double)          = &balance_t::can_withdraw;
        account_t (balance_t::*pfm5)()           = &balance_t::get_account;
        transaction_t (balance_t::*pfm6)(double) = &balance_t::withdraw;
        balance_report_t (balance_t::*pfm7)(int) = &balance_t::make_report;

        process09(pfm1); // T (C::*)()
        process09(pfm2); // T (C::*)(U)
        process09(pfm3); // T (U::*)()
        process09(pfm4); // T (U::*)(V)
        process09(pfm5); // C (T::*)()
        process09(pfm6); // C (T::*)(U)
        process09(pfm7); // D (C::*)(T)
    }

    {
        std::println("\n====================== using namespace n412/7 ===========================");
        using namespace n412;

        // pointers to data members
        process10(&account_t::number);  // T C::*
        process10(&balance_t::account); // C T::*
        process10(&balance_t::amount);  // T U::*
    }

    {
        std::println("\n====================== using namespace n412/8 ===========================");
        using namespace n412;

        wrapper<double> wd{42.0};            //
        process11(wd);                       // C<T>

        int_array<5> ia{};                   //
        process12(ia);                       // C<5>

        process13(wd);                       // TT<T>
        process14(ia);                       // TT<5>

        wrapper<account_t> wa1{{42}};        // NOTE: double braces
        process15(wa1);                      // TT<C>
        std::println("{}", wa1.data.number); // 42

        wrapper<account_t> wa2{42};          //
        process15(wa2);                      // TT<C>
        std::println("{}", wa2.data.number); // 42
    }

    {
        std::println("\n====================== using namespace n413 =============================");
        using namespace n413;

        double arr[]{1.0, 2.0, 3.0, 4.0, 5.0}; //
        process<int, 5>(arr);                  // OK
        // process(arr);                       // error: No matching function for call to 'process'
    }

    {
        std::println("\n====================== using namespace n414 =============================");
        using namespace n414;

        // doesn't deduce from default params
        // process();   // error: No matching function for call to 'process'

        // must explicitly specify template argument
        process<int>(); // 0, 42
        process(10);    // 10, 42
    }

    {
        std::println("\n====================== using namespace n415 =============================");
        using namespace n415;

        // invoke(&alpha); // error: alpha is a templated function
        // invoke(&beta);  // error: two matching beta functions

        invoke(&gamma); // gamma(double,int)
    }

    {
        std::println("\n====================== using namespace n416 =============================");
        using namespace n416;

        // C-style arrays

        // A limitation of the compiler is the argument deduction of the primary
        // dimension of an array. The reason is this is not part of function parameter types.
        // The exceptions to this limitation are the cases when the dimension refers to a
        // reference or pointer type.

        int arr1[10];
        int arr2[5][10];

        // process1(arr1);   // error: no matching function (compiler cannot deduce primary dimension of array)

        // primary dimension explicitly specified
        process2(arr2); // process(int[5][10])

        // reference to an array
        process3(arr1); // process(int[10]&)

        // pointer to an array
        process4(&arr1); // process(int[10]*)
    }

    {
        std::println("\n====================== using namespace n417 =============================");
        using namespace n417;

        ncube<5> cube;

        // process(cube);    // error
        // process<5>(cube); // error

        // must specify N
        process<6>(cube); // 5
    }

    {
        std::println("\n====================== using namespace n418 =============================");

        std::pair<int, double> p1{42, 42.0}; // types explicitly specified
        std::pair              p2{42, 42.0}; // compiler can deduce types

        std::vector<int> v1{1, 2, 3, 4, 5};  // types explicitly specified
        std::vector      v2{1, 2, 3, 4, 5};  // compiler can deduce types

        using namespace n418;

        wrapper<int> w1{42};                 // types explicitly specified
        wrapper      w2{42};                 // compiler can deduce types
    }

    {
        std::println("\n====================== using namespace n418 =============================");

        auto p = std::make_pair(42, 42.0);                       // pair<int, double>

        using namespace n418;

        auto v = make_vector(1, 2, 3, 4, 5);                     // vector<int>
        auto w = make_wrapper(42);                               // wrapper<int>

        std::println("first: {} second: {}", p.first, p.second); // first: 42 second: 42
        std::println("size: {}", v.size());                      // size: 5
        std::println("data: {}", w.data);                        // data: 42
    }

    {
        std::println("\n====================== using namespace n419 =============================");
        using namespace n419;

        auto p = new point_t(1, 2);                           // point_t<int> * (deduced by compiler)
        std::println("x: {}, y: {}", p->get_x(), p->get_y()); // x: 1, y: 2

        std::mutex mt;
        auto       lock = std::lock_guard(mt);                // std::lock_guard<mutex> (deduced by compiler)
    }

    {
        // without custom deduction guides
        std::pair p1{1, "one"};     // std::pair<int, const char*>
        std::pair p2{"two", 2};     // std::pair<const char*, int>
        std::pair p3{"3", "three"}; // std::pair<const char*, const char*>

        std::println("{} {}", p1.first, p1.second);
        std::println("{} {}", p2.first, p2.second);
        std::println("{} {}", p3.first, p3.second);
    }

    {
        auto one = 1;

        // (de-)activate namespace std just before main() function
        // ----------------------------------------------------------------------------------------------------------
        //                          |   without deduction guides              |    with deduction guides            |
        // ----------------------------------------------------------------------------------------------------------
        std::pair p0{one, "one"};   // std::pair<int, const char *>           | std::pair<int, std::string>         |
        std::pair p1{1, "one"};     // std::pair<int, const char *>           | std::pair<int, std::string>         |
        std::pair p2{"two", 2};     // std::pair<const char *, int>           | std::pair<std::string, int>         |
        std::pair p3{"3", "three"}; // std::pair<const char *, const char *>  | std::pair<std::string, std::string> |
        // ----------------------------------------------------------------------------------------------------------

        std::println("{} {}", p1.first, p1.second);
        std::println("{} {}", p2.first, p2.second);
        std::println("{} {}", p3.first, p3.second);
    }

    {
        std::println("\n====================== using namespace n420 =============================");
        using namespace n420;

        int     arr[] = {1, 2, 3, 4, 5};
        range_t r(std::begin(arr), std::end(arr)); // range_t<int>
    }

    {
        std::pair<int, std::string> p1{1, "one"};
        std::pair                   p2{2, "two"};

        // Class template argument deduction only works if NO template arguments are provided.
        // std::pair<>    p3{3, "three"}; // error: Too few template arguments for class template 'pair'
        // std::pair<int> p4{4, "four"};  // error: Too few template arguments for class template 'pair'
    }

    {
        std::vector v1{42, 48}; // vector<int>
        std::vector v2{v1, v1}; // vector<vector<int>>
        std::vector v3{v1};     // vector<int> (!!!) (deduction depends on both the number of arguments and their type)

        std::println("{}", demangle(typeid(decltype(v1)).name())); // std::vector<int>
        std::println("{}", demangle(typeid(decltype(v2)).name())); // std::vector<std::vector<int>>
        std::println("{}", demangle(typeid(decltype(v3)).name())); // std::vector<int>

        // v3 is a copy of v1
        std::println("{}", v1[0]); // 42
        std::println("{}", v1[1]); // 48
        std::println("{}", v3[0]); // 42
        std::println("{}", v3[1]); // 48

        v1[0] = 0;
        std::println("{}", v1[0]); // 0
        std::println("{}", v3[0]); // 42
    }

    {
        std::println("\n====================== using namespace n421 =============================");
        using namespace n421;

        wrapper  x  = {42}; // x  is l-value
        wrapper &rx = x;    // rx is l-value

        f(x);               // f(wrapper&): 42
        f(rx);              // f(wrapper&): 42
        f(wrapper{42}); // error: wrapper{42} is an r-value (a temporary object); with a 'wrapper const &' it would work
    }

    {
        std::println("\n====================== using namespace n421 =============================");
        using namespace n421;

        wrapper        x  = {42}; // x  is l-value
        wrapper const &rx = x;    // rx is l-value

        g(x);                     // g(wrapper&): 42
        g(rx);                    // g(wrapper&): 42
        g(wrapper{42});           // g(wrapper&&): 42
    }

    {
        std::println("\n====================== using namespace n421 =============================");
        using namespace n421;

        wrapper  x  = {42}; // x  is an l-value
        wrapper &rx = x;    // rx is an l-value

        // Passing rvalues to a function has following purposes:
        // - temporary object:
        //   either the object is temporary and does not exist outside the call and
        //   the function can do anything with it, or
        // - ownership (move semantics):
        //   the function is supposed to take ownership of the object that is received
        //   This is the purpose of the move constructor and the move assignment operator.

        // h(x);          // error: cannot bind an lvalue to an rvalue reference
        // h(rx);         // error
        h(wrapper{42});   // h(wrapper&&): 42
        h(std::move(x));  // h(wrapper&&): 42 (std::move makes sort of a cast from an lvalue to an rvalue)
        h(std::move(rx)); // h(wrapper&&): 42
    }

    {
        std::println("\n====================== using namespace n422 =============================");
        using namespace n422;

        // One purpose of rvalue references is to enable MOVE SEMANTICS.
        // But it has yet another one and that is to enable PERFECT FORWARDING.

        wrapper x{42};

        h(x);           // g(wrapper&): 42
        h(wrapper{42}); // g(wrapper&): 42 (not what we want!!!)
    }

    {
        std::println("\n====================== using namespace n423 =============================");
        using namespace n423;

        wrapper x{42};

        h(x);           // g(wrapper&): 42
        h(wrapper{42}); // g(wrapper&&): 42 (now perfectly forwarded!!!)
    }

    {
        std::println("\n====================== using namespace n424 =============================");
        using namespace n424;

        // In templates, rvalue references work slightly differently, and sometimes they are
        // rvalue references, but other times they are actually lvalue references. So
        // they accept everything.
        //
        // References that exhibit this behavior are called FORWARDING REFERENCES.
        // Often referred to as UNIVERSAL REFERENCES.
        //
        // Forwarding references can take l-values and r-values.
        //
        // Forwarding references are only present in the context of an rvalue reference to a template parameter.
        // It has to have the form T&& and nothing else. (T const&&, std::vector<T>&&, etc. are not forwarding
        // references.)

        int x = 42;
        f(x);                    // f(T&&)
        f(42);                   // f(T&&)

        int const cx = 100;
        f(cx);                   // f(T&&)
        f(std::move(cx));        // f(T const&&) (is a better match for this r-value)

        std::vector<int> v{42};
        f(v);                    // f(T&&)
        f(std::vector<int>{42}); // f(vector<T>&&) (is a better match for this r-value)

        S<int> s;
        s.f(42);                 // S.f(T&&)

        // The T&& is an rvalue reference because f is not a template but a non-template member function of a class
        // template, so this rule for forwarding references does not apply.
        // s.f(x);               // error: Rvalue reference to type 'int' cannot bind to lvalue of type 'int'
    }

    {
        std::println("\n====================== Reference Collapsing =============================");

        // Forwarding references are a special case of function template argument deduction.
        // Their purpose is to enable perfect forwarding with templates and they are made
        // possible by a new C++11 feature called REFERENCE COLLAPSING.

        // Prior to C++11, it was not possible to take a reference to a reference. However, that is now
        // possible in C++11 for typedefs and templates.

        // The rule is pretty simple: an rvalue reference to an rvalue reference collapses to an rvalue
        // reference; all other combinations collapse to an lvalue reference.

        // Minimizes &

        // Type | Type of reference | Type of variable | Case
        // --------------------------------------------------
        // T&   | T&                | T&               | [1]
        // T&   | T&&               | T&               | [2]
        // T&&  | T&                | T&               | [3]
        // T&&  | T&&               | T&&              | [4]

        // These rules only apply when both types are references.

        using lrefint = int &;
        using rrefint = int &&;

        int       x  = 42;
        lrefint  &r1 = x;       // type of r1 is int&  | [1]
        lrefint &&r2 = x;       // type of r2 is int&  | [2]
        rrefint  &r3 = x;       // type of r3 is int&  | [3]
        rrefint &&r4 = 1;       // type of r4 is int&& | [4]

        std::println("{}", x);  // 42
        std::println("{}", r1); // 42
        std::println("{}", r2); // 42
        std::println("{}", r3); // 42
        std::println("{}", r4); // 1
    }

    {
        std::println("\n====================== Reference Collapsing (auto) ======================");

        // When auto&& is found, it means a forwarding reference.
        // The same does not apply for anything else, such as cv-qualified forms like auto const&&.

        int   x = 42;
        auto &r = x;    // int&  (l-value reference)

        auto &&rx = x;  // int&  (forwarding reference collapses to l-value reference) [3]
        auto &&rc = 42; // int&& (forwarding reference collapses to r-value reference) [4]

        // rcx is not a forwarding reference (bc of const), but an r-value reference:
        // auto const &&rcx = x; // error: Rvalue reference to type 'const int' cannot bind to lvalue of type 'int'

        std::vector v{42};

        auto  &rv2 = v[0];       // int& [3]
        auto &&rv1 = v[0];       // int& [3]

        std::println("{}", x);   // 42
        std::println("{}", rc);  // 42
        std::println("{}", r);   // 42
        std::println("{}", rx);  // 42
        std::println("{}", rv1); // 42
        std::println("{}", rv2); // 42
    }

    {
        std::println("\n====================== using namespace n425 =============================");
        using namespace n425;

        foo x{42};

        h(x);       // g(foo&)
        h(foo{42}); // g(foo&)
    }

    {
        std::println("\n====================== using namespace n426 =============================");
        using namespace n426;

        // Same as n423 but in a templating context.
        // Only one version of h() needed.

        foo x{42};

        h(x);       // g(foo&)
        h(foo{42}); // g(foo&)
    }

    {
        std::println("\n====================== using namespace n427 =============================");
        using namespace n427;

        // Same behavior in a templating context.

        foo x{42};

        h(x);       // g(foo&)
        h(foo{42}); // g(foo&&)
    }

    {
        std::println("\n====================== using namespace n428 =============================");
        using namespace n428;

        // `decltype` is a type specifier used to deduce the type of an expression.
        //
        // Typically used in function templates with the `auto` specifier to specify the return type.

        // Rules:
        //
        // R1 :If the expression is an IDENTIFIER or a class member access, then the result is the
        //     TYPE OF THE ENTITY that is named by the expression. If the entity does not exist,
        //     or it is a function that has an overload set (more than one function with the same
        //     name exists), then the compiler will generate an error.
        // R2: If the expression is a FUNCTION CALL or an overloaded operator function, then the
        //     result is the RETURN TYPE of the function. If the overloaded operator is wrapped in
        //     parentheses, these are ignored.
        // R3: If the expression is an LVALUE, then the result type is an lvalue
        //     REFERENCE TO THE TYPE OF EXPRESSION.
        // R4: If the expression is SOMETHING ELSE, then the result type is the TYPE OF THE EXPRESSION.

        int          a  = 42;
        int         &ra = a;
        const double d  = 42.99;
        long         arr[10];
        long         l = 0;
        char        *p = nullptr;
        char         c = 'x';
        wrapper      w1{1};
        wrapper     *w2 = new wrapper{2};

        decltype(a)         e1;            // R1, int
        decltype(ra)        e2 = a;        // R1, int&
        decltype(f)         e3;            // R1, int [!!]
        decltype(d)         e8 = 1;        // R1, const double
        decltype(arr)       e9;            // R1, long[10]
        decltype(w1.val)    e11;           // R1, int
        decltype(w1.get())  e12;           // R1, int
        decltype(w2->val)   e13;           // R1, int
        decltype(w2->get()) e14;           // R1, int
        decltype(p)         e19 = nullptr; // R1, char*

        //  decltype(g)      e5;   // R1, error: Reference to overloaded function could not be resolved [!!]

        decltype(f())  e4; // R2, int
        decltype(g(1)) e6; // R2, int

                           // only interesting case: EXPRESSION of an lvalue turns to lvalue reference.
        decltype(arr[1]) e10 = l;      // R3, long&
        decltype(*p)     e20 = c;      // R3, char&
        decltype(p[0])   e21 = c;      // R3, char&
        decltype(a = 0)  e18 = a;      // R3, int& (assignment operator returns an l-value) [!!]

        decltype(&f)    e7  = nullptr; // R4, int(*)() [!!]
        decltype(42)    e15 = 1;       // R4, int
        decltype(1 + 2) e16;           // R4, int (rvalue) [!!]
        decltype(a + 1) e17;           // R4, int (rvalue) [!!]

        delete w2;
    }

    {
        std::println("\n====================== Unevaluated Context ==============================");

        int a = 42;

        // a is not evaluated (only type is deduced)
        decltype(a = 1) e = a;

        std::println("{}", a); // 42
        std::println("{}", e); // 42
    }

    {
        std::println("\n====================== using namespace n412 =============================");
        using namespace n412;

        // There is an exception to this rule concerning template instantiation. When the expression
        // used with the decltype specifier contains a template, the template is instantiated before
        // the expression is evaluated at compile time:

        decltype(wrapper<double>::data) e1; // double

        int a = 42;

        //  wrapper<char> is instantiated here even if the type is only deduced from the variable a (due to the use of
        //  the comma operator -> expression)
        decltype(a)                     e2 = a; // R1, int
        decltype(wrapper<int>::data)    e3 = a; // R1, int
        decltype(wrapper<int>::data, a) e4 = a; // R3, int&
    }

    {
        std::println("\n====================== using namespace n429 =============================");
        using namespace n429;

        foo           f;
        foo const     cf;
        volatile foo *pf = &f;

        // only target type is relevant (e.g. specifiers of the object are ignored)

        decltype(f.a) e1 = 0;   // int
        decltype(f.b) e2 = 0;   // int volatile
        decltype(f.c) e3 = 0;   // int const

        decltype(cf.a) e4 = 0;  // int
        decltype(cf.b) e5 = 0;  // int volatile
        decltype(cf.c) e6 = 0;  // int const

        decltype(pf->a) e7 = 0; // int
        decltype(pf->b) e8 = 0; // int volatile
        decltype(pf->c) e9 = 0; // int const

                                // lvalue or rvalue does not affect the deduced type
        decltype(foo{}.a) e10 = 0; // int
        decltype(foo{}.b) e11 = 0; // int volatile
        decltype(foo{}.c) e12 = 0; // int const
    }

    {
        std::println("\n====================== using namespace n429 =============================");
        using namespace n429;

        // Parentheses around the expression make a difference!
        // The const or volatile specifiers of the used object are now relevant!

        foo           f;
        foo const     cf;
        volatile foo *pf = &f;

        int x          = 1;
        int volatile y = 2;
        int const z    = 3;

        decltype((f.a)) e1 = x;      // int&                 (l-value reference)
        decltype((f.b)) e2 = y;      // int volatile &       (l-value reference)
        decltype((f.c)) e3 = z;      // int const &          (l-value reference)

        decltype((cf.a)) e4 = x;     // int const &          (l-value reference)
        decltype((cf.b)) e5 = y;     // int const volatile & (l-value reference)
        decltype((cf.c)) e6 = z;     // int const &          (l-value reference)

        decltype((pf->a)) e7 = x;    // int volatile &       (l-value reference)
        decltype((pf->b)) e8 = y;    // int volatile &       (l-value reference)
        decltype((pf->c)) e9 = z;    // int const volatile & (l-value reference)

        decltype((foo{}.a)) e14 = 0; // int &&               (r-value reference)
        decltype((foo{}.b)) e15 = 0; // int volatile &&      (r-value reference)
        decltype((foo{}.c)) e16 = 0; // int const &&         (r-value reference)
    }

    {
        std::println("\n====================== Qualifiers =======================================");

        int       a    = 0;
        int      &ra   = a;
        int const c    = 42;
        int volatile d = 99;

        // Because decltype is a type specifier, the redundant const and volatile qualifiers
        // and reference specifiers are ignored.

        decltype(ra)                 e2 = a;       // int&
        decltype(ra) & /* ignored */ e1 = a;       // int&

        decltype(c)                     e4 = 1;    // int const
        decltype(c) const /* ignored */ e3 = 1;    // int const

        decltype(d) e6                        = 1; // int volatile
        decltype(d) volatile /* ignored */ e5 = 1; // int volatile
    }

    {
        std::println("\n====================== using namespace n430 =============================");
        using namespace n430;

        // The decltype specifier's real purpose is to be used in templates,
        // where the return value of a function depends on its template arguments
        // and is not known before instantiation.

        auto m1 = minimum(1, 5);        // int
        auto m2 = minimum(18.49, 9.99); // double
        // auto m3 = minimum(1, 9.99);  // error: arguments of different type

        std::println("{}", m1); // 1
        std::println("{}", m2); // 9.0
    }

    {
        std::println("\n====================== using namespace n431 =============================");
        using namespace n431;

        auto m1 = minimum(1, 5);        // int
        auto m2 = minimum(18.49, 9.99); // double
        auto m3 = minimum(1, 9.99);     // double

        std::println("{}", m1);         // 1
        std::println("{}", m2);         // 9.9
        std::println("{}", m3);         // 1
    }

    {
        std::println("\n====================== using namespace n432 =============================");
        using namespace n432;

        auto m1 = minimum(1, 5);        // int
        auto m2 = minimum(18.49, 9.99); // double
        auto m3 = minimum(1, 9.99);     // double

        std::println("{}", m1);         // 1
        std::println("{}", m2);         // 9.9
        std::println("{}", m3);         // 1
    }

    {
        std::println("\n====================== using namespace n433 =============================");
        using namespace n433;

        auto m1 = minimum(1, 5);        // int
        auto m2 = minimum(18.49, 9.99); // double
        auto m3 = minimum(1, 9.99);     // double

        std::println("{}", m1);         // 1
        std::println("{}", m2);         // 9.9
        std::println("{}", m3);         // 1
    }

    {
        std::println("\n====================== using namespace n434 =============================");
        using namespace n434;

        int a = 42;

        // const and ref qualifiers are being discarded with auto
        decltype(func(a))        r1 = func(a);        // int const &
        decltype(func_caller(a)) r2 = func_caller(a); // int (const and ref qualifiers are being discarded)

        std::println("{}", r1);                       // 42
        std::println("{}", r2);                       // 42
    }

    {
        std::println("\n====================== using namespace n435 =============================");
        using namespace n435;

        // decltype is a type specifier used to deduce the type of an expression.
        // It can be used in different contexts, but its purpose is for templates
        // to determine the return type of a function and to ensure the perfect
        // forwarding of it.

        int a = 42;

        // const and ref qualifiers are not being discarded with decltype(auto)
        // -> perfect forwarding
        decltype(func(a))        r1 = func(a);        // int const&
        decltype(func_caller(a)) r2 = func_caller(a); // int const& (perfectly returned)

        std::println("{}", r1);                       // 42
        std::println("{}", r2);                       // 42
    }

    {
        std::println("\n====================== using namespace n436 =============================");
        using namespace n436;

        // using onyl decltype

        // int{}, double{} no problem
        static_assert(std::is_same_v<double, composition<int, double>::result_type>);

        // error: there is no default constructor for wrapper (wrapper{})
        // static_assert(std::is_same_v<wrapper, composition<int, wrapper>::result_type>);
    }

    {
        std::println("\n====================== using namespace n437 =============================");
        using namespace n437;

        // now using decltype with std::declval
        // std::declval produces a value of a type T without using a default constructor.

        static_assert(std::is_same_v<double, composition<int, double>::result_type>);
        static_assert(std::is_same_v<wrapper, composition<int, wrapper>::result_type>);
    }

    {
        std::println("\n====================== using namespace n438 =============================");
        using namespace n438;

        wrapper w{42}; //
        print(w);      // 42
    }

    {
        std::println("\n====================== using namespace n439 =============================");
        using namespace n439;

        wrapper w{42};

        print<int>(w);        // 42
        print<char>(w);       // 42

        printer<int>()(w);    // 42
        printer<double>()(w); // 42
    }

    {
        std::println("\n====================== using namespace n440 =============================");
        using namespace n440;

        wrapper w{42};

        print<int>(w);        // 42
        print<char>(w);       // [empty]

        printer<int>()(w);    // 42
        printer<double>()(w); // [empty]
    }

    {
        std::println("\n====================== using namespace n442 =============================");
        using namespace n442;

        // friends
        wrapper<int> w1{42};
        print<int>(w1);     // 42
        printer<int>{}(w1); //

        // not friends
        wrapper<char> w2{'a'};
        print<char>(w2);     // [empty]
        printer<char>{}(w2); //
    }

    {
        std::println("\n====================== using namespace n443 =============================");
        using namespace n443;

        // friend
        wrapper<int> w1{42};
        print(w1);          // 42
        printer<int>{}(w1); // 42

        // friend
        wrapper<char> w2{'a'}; //
        print(w2);             // a
        printer<char>{}(w2);   // a

        // ???
        // print<char>(w1);     // error
        // printer<char>{}(w1); // error

        // ???
        // print<int>(w2);      // error
        // printer<int>{}(w2);  // error
    }

    {
        std::println("\n====================== using namespace n444 =============================");
        using namespace n444;

        // friend
        wrapper<int> w1{42};
        print(w1);          // 42
        printer<int>{}(w1); // 42

        // not friend
        // print<char>(w1);     // error
        // printer<char>{}(w1); // error

        // friend
        wrapper<char> w2{'a'};
        print(w2);           // a
        printer<char>{}(w2); // a

        // not friend
        // print<int>(w2);      // error
        // printer<int>{}(w2);  // error
    }
    {
        std::println("\n====================== using namespace n445 =============================");
        using namespace n445;

        // granting friendship to a type template parameter

        executor e; //
        e.run();    // localhost:1234
    }
}
