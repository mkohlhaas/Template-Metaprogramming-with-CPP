#include "wrapper.h"
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <ostream>
#include <print>
#include <type_traits>

// Defining function templates

namespace n201
{
    template <typename T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }

    class foo
    {
      private:
        int value;

      public:
        explicit foo(int const i) : value(i)
        {
        }

        int
        get() const
        {
            return value;
        }

        explicit
        operator int() const
        {
            return value;
        }
    };

    foo
    operator+(foo const a, foo const b)
    {
        return foo(static_cast<int>(a) + static_cast<int>(b));
    }

    template <typename Pointer, typename Predicate>
    int
    count_if(Pointer start, Pointer end, Predicate p)
    {
        std::println("{}", __PRETTY_FUNCTION__);
        int total = 0;
        for (Pointer i = start; i != end; i++)
        {
            if (p(*i))
            {
                ++total;
            }
        }
        return total;
    }
} // namespace n201

//  Defining class templates

namespace n202
{
    template <typename T>
    class wrapper
    {
      public:
        wrapper(T const v) : value(v)
        {
        }

        T const &
        get() const
        {
            return value;
        }

      private:
        T value;
    };
} // namespace n202

namespace n203
{
    // declarations

    template <typename T>
    class wrapper;

    void use_wrapper(wrapper<int> *ptr);

    // definitions

    template <typename T>
    class wrapper
    {
      public:
        wrapper(T const v) : value(v)
        {
        }

        T const &
        get() const
        {
            return value;
        }

      private:
        T value;
    };

    void
    use_wrapper(wrapper<int> *ptr)
    {
        std::println("{}", ptr->get());
    }
} // namespace n203

// Defining member function templates

namespace n204
{
    // member function of class template
    template <typename T>
    struct composition
    {
        T
        add(T const a, T const b)
        {
            return a + b;
        }
    };
} // namespace n204

namespace n205
{
    // member function template of class
    class composition
    {
      public:
        template <typename T>
        T
        add(T const a, T const b)
        {
            return a + b;
        }
    };
} // namespace n205

namespace n206
{
    // member function template of class template
    template <typename T>
    class wrapper
    {
      public:
        wrapper(T const v) : value(v)
        {
        }

        T const &
        get() const
        {
            return value;
        }

        // member function template of class template
        // function template (must have a different type parameter)
        template <typename U>
        U
        as() const
        {
            return static_cast<U>(value);
        }

      private:
        T value;
    };
} // namespace n206

// Understanding template parameters

// Type template parameters

namespace n207
{
    template <typename T>
    class wrapper1
    {
        // ...
    };

    template <typename T = int>
    class wrapper2
    {
        // ...
    };

    // forward declarations don't need type variable names
    template <typename>
    class wrapper3;

    // default type
    template <typename = int>
    class wrapper4;

    // variadic template with parameter pack
    template <typename... T>
    class wrapper5
    {
        // ...
    };

    // Constraints specify requirements on template arguments.
    // A named set of constraints is called a concept.
    template <typename T>
    concept WrappableType = std::is_trivial_v<T>;

    // Concepts can be used like a type.
    template <WrappableType T>
    class wrapper6
    {
        // ...
    };

    template <WrappableType T = int>
    class wrapper7
    {
        // ...
    };

    template <WrappableType... T>
    class wrapper8
    {
        // ...
    };
} // namespace n207

// Non-type template parameters

namespace n208
{
    template <int V>
    class foo1
    {
        //...
    };

    template <int V = 42>
    class foo2
    {
        //...
    };

    template <int... V>
    class foo3
    {
        //...
    };
} // namespace n208

namespace n209
{
    template <typename T, size_t S>
    class buffer
    {
      private:
        T data_[S];

      public:
        constexpr T const *
        data() const
        {
            return data_;
        }

        constexpr T &
        operator[](size_t const index)
        {
            return data_[index];
        }

        constexpr T const &
        operator[](size_t const index) const
        {
            return data_[index];
        }
    };

    template <typename T, size_t S>
    buffer<T, S>
    make_buffer()
    {
        return {};
    }
} // namespace n209

namespace n210
{
    struct device
    {
        virtual void output() = 0;
        virtual ~device()
        {
        }
    };

    // (function) pointer as template parameter
    template <typename Command, void (Command::*action)()>
    struct smart_device : device
    {
        smart_device(Command *command) : cmd(command)
        {
        }

        void
        output() override
        {
            (cmd->*action)();
        }

      private:
        Command *cmd;
    };

    struct hello_command
    {
        void
        say_hello_in_english()
        {
            std::println("Hello, world!");
        }

        void
        say_hello_in_spanish()
        {
            std::println("Hola mundo!");
        }
    };
} // namespace n210

namespace n211
{
    struct device
    {
        virtual void output() = 0;
        virtual ~device()
        {
        }
    };

    // pointer to member function
    template <typename Command, void (Command::*action)()>
    struct smart_device : device
    {
        // using a reference instead of pointer
        smart_device(Command &command) : cmd(command)
        {
        }

        void
        output() override
        {
            (cmd.*action)();
        }

      private:
        Command &cmd;
    };

    struct hello_command
    {
        void
        say_hello_in_english()
        {
            std::println("Hello, world!");
        }

        void
        say_hello_in_spanish()
        {
            std::println("Hola mundo!");
        }
    };
} // namespace n211

namespace n212
{
    struct device
    {
        virtual void output() = 0;
        virtual ~device()
        {
        }
    };

    template <void (*action)()>
    struct smart_device : device
    {
        void
        output() override
        {
            // the same:
            // (*action)();
            action();
        }
    };

    void
    say_hello_in_english()
    {
        std::println("Hello, world!");
    }

    void
    say_hello_in_spanish()
    {
        std::println("Hola mundo!");
    }
} // namespace n212

namespace n213
{
    template <auto x>
    struct foo
    {
        // ...
    };
} // namespace n213

namespace n214
{
    template <size_t N>
    struct string_literal
    {
        constexpr string_literal(const char (&str)[N])
        {
            std::copy_n(str, N, value); // copy from 'str' to 'value'
        }

        char value[N];
    };

    // template <string_literal s>
    template <string_literal>
    struct foo
    {
        // ...
    };
} // namespace n214

namespace n215
{
    template <auto... x>
    struct foo
    {
        // ...
    };
} // namespace n215

// Template template parameters

namespace n216
{
    template <typename T>
    class simple_wrapper
    {
      public:
        T value;
    };

    template <typename T>
    class fancy_wrapper
    {
      public:
        fancy_wrapper(T const v) : value{v}
        {
        }

        T const &
        get() const
        {
            return value;
        }

        template <typename U>
        U
        as() const
        {
            return static_cast<U>(value);
        }

      private:
        T value;
    };

    template <typename T, typename U, template <typename> typename W = fancy_wrapper>
    class wrapping_pair
    {
      public:
        wrapping_pair(T const a, U const b) : item1{a}, item2{b}
        {
        }

        W<T> item1;
        W<U> item2;
    };
} // namespace n216

// Default template arguments

namespace n217
{
    template <typename T = int>
    class foo
    {
        // ...
    };

    template <typename T = int, typename U = double>
    class bar
    {
        // ...
    };
} // namespace n217

namespace n218
{
    // class template
    //
    // U must also have a default type..
    // template <typename T = int, typename U>
    // class bar { };

    // function template
    //
    // No default type necessary for U.
    template <typename T = int, typename U>
    void
    func()
    {
        // ...
    }
} // namespace n218

namespace n219
{
    // Merging all template declarations and its definition
    // (the same way they are merged for default function arguments).

    // order is important
    template <typename T, typename U = double>
    struct foo;

    template <typename T = int, typename U>
    struct foo;

    // semantically the same (after merging):
    // template <typename T = int, typename U = double>
    template <typename T, typename U>
    struct foo
    {
        T a;
        U b;
    };
} // namespace n219

namespace n220
{
    template <typename T = int>
    struct foo;

    // Template parameter redefines default argument.
    // template <typename T = int> // error
    template <typename T>
    struct foo
    {
        // ...
    };
} // namespace n220

namespace n221
{
    template <typename T>
    struct foo
    {
      protected:
        using value_type = T;
    };

    // when you uncomment variable in main: 'value_type' is a protected member of 'n221::foo<int>':
    // Member access restrictions are checked at the declaration, not at the instantiation of the template.
    // -> error shows up here, not in main()
    // same: template <typename T, typename U = typename T::value_type>
    template <typename T, typename U = T::value_type>
    struct bar
    {
        // ...
    };
} // namespace n221

// Understanding template instantiation

// Implicit instantiation

namespace n222
{
    template <typename T>
    struct foo
    {
        void
        f()
        {
            // ...
        }

        void
        g()
        {
            // clang and gcc perform semantic check/validation and throw an error although g() is not used!
            // VC++ doesn't perform semantic and ignores the parts of the template that are not used, provided
            // that the code is syntactically correct.
            // static_assert(false);
        }
    };
} // namespace n222

namespace n223
{
    template <typename T>
    struct foo
    {
        void
        f()
        {
            std::println("{}", __PRETTY_FUNCTION__);
            // ...
        }

        void
        g()
        {
            std::println("{}", __PRETTY_FUNCTION__);
            // ...
        }
    };
} // namespace n223

namespace n224
{
    template <typename T>
    struct control
    {
        // ...
    };

    template <typename T>
    struct button : public control<T>
    {
        // ...
    };

    // pointer to a templated class doesn't trigger its instantiation...
    void
    show(button<int> *ptr)
    {
        // ...but pointer conversion does: triggers button<int> instantiation.
        // Conversion between button<int>* and control<int>* takes place.
        // Therefore, at this point, the compiler must instantiate button<int>.
        control<int> *c [[maybe_unused]] = ptr;
    }
} // namespace n224

namespace n225
{
    // Every specialization of a class template has its own copy of static members!!
    template <typename T>
    struct foo
    {
        static T shared_data;
    };

    template <typename T>
    T foo<T>::shared_data = 0;
} // namespace n225

namespace n226
{
    template <typename T>
    struct wrapper
    {
        T value;
    };

    // Explicit Class Instantiation
    template struct wrapper<int>; // [1]
} // namespace n226

// Explicit Class Instantiation
// You have to use namespace scoping if outside the namespace.
// A general 'using namespace n226' is not sufficient.
template struct n226::wrapper<double>; // [2]

namespace n226
{
    template <typename T>
    T
    add(T const a, T const b)
    {
        return a + b;
    }

    // Explicit Function Instantiation
    template int add(int, int); // [1]
} // namespace n226

// Explicit Function Instantiation
// You have to use namespace scoping if outside the namespace.
template double n226::add(double, double); // [2]

namespace n227
{
    template <typename T>
    class foo
    {
      private:
        struct bar
        {
        };

        T
        f(bar const arg [[maybe_unused]])
        {
            return {};
        }
    };

    // Explicit Method Instantiation
    // (private) member access specification is ignored in explicit instantiation definitions
    //
    // Both the class foo<T>::bar and the function foo<T>::f() are private to the foo<T>
    // class, but they can be used in the explicit instantiation definition!
    template int foo<int>::f(foo<int>::bar);
} // namespace n227

// Understanding template specialization

// Explicit (full) specialization

namespace n228
{
    // primary template
    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };

    // specialized template
    // explicit (full) specialization
    template <>
    struct is_floating_point<float>
    {
        constexpr static bool value = true;
    };

    // specialized template
    // explicit (full) specialization
    template <>
    struct is_floating_point<double>
    {
        constexpr static bool value = true;
    };

    // specialized template
    // explicit (full) specialization
    template <>
    struct is_floating_point<long double>
    {
        constexpr static bool value = true;
    };

    // variable template
    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace n228

namespace n229
{
    // declaration
    template <typename T>
    struct is_floating_point;

    // specialization first
    template <>
    struct is_floating_point<float>
    {
        constexpr static bool value = true;
    };

    // primary second
    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };
} // namespace n229

namespace n230
{
    // primary
    template <typename>
    struct foo
    {
        // ...
    };

    // explicit specialization declaration (but never defined!)
    template <>
    struct foo<int>;
} // namespace n230

namespace n231
{
    template <typename T>
    struct foo
    {
    };

    template <typename T>
    void
    func(foo<T>)
    {
        std::println("primary template");
    }

    // template<> void func<int>(foo<int>)
    //                     ^^^^^
    //         superfluous bc compiler can deduce
    template <>
    void
    func(foo<int>)
    {
        std::println("int specialization");
    }
} // namespace n231

namespace n232
{
    // primary function template
    template <typename T>
    void
    func(T a [[maybe_unused]])
    {
        std::println("primary template");
    }

    // specialization
    template <>
    void
    // error if you uncomment default value: Default argument not permitted on an explicit specialization
    func(int a [[maybe_unused]] /* = 0 */)
    {
        std::println("int specialization");
    }
} // namespace n232

namespace n233
{
    // class template
    template <typename T>
    struct foo
    {
        static T value;
    };

    // class (static) member template
    template <typename T>
    T foo<T>::value = 0;

    // class (static) member template specialization
    template <>
    int foo<int>::value = 42;
} // namespace n233

namespace n234
{
    // primary function template
    template <typename T, typename U>
    void
    func(T a, U b)
    {
        std::println("primary template: {} {}", a, b);
    }

    // int, int - explicit/full specialization
    template <>
    void
    func(int a, int b)
    {
        std::println("int-int specialization: {} {}", a, b);
    }

    // int, double - explicit/full specialization
    template <>
    void
    func(int a, double b)
    {
        std::println("int-double specialization: {} {}", a, b);
    }
} // namespace n234

// Partial specialization

namespace n235
{
    // A template A is considered more specialized than a template B if it accepts
    // a subset of the types that B accepts, but not the other way around.

    // Only classes can be partially specialized!
    // Functions are always fully specialized!

    template <typename T, int S>
    struct collection
    {
        void
        operator()()
        {
            std::println("primary template");
        }
    };

    // int S is specialized...
    template <typename T>    // ...has template parameter list and
    struct collection<T, 10> // ...has template argument list
    {
        void
        operator()()
        {
            std::println("partial specialization <T, 10>");
        }
    };

    // typename T is specialized
    template <int S>
    struct collection<int, S>
    {
        void
        operator()()
        {
            std::println("partial specialization <int, S>");
        }
    };

    // pointer to T is specialized (we still need 'typename T' in the template parameter list)
    template <typename T, int S>
    struct collection<T *, S>
    {
        void
        operator()()
        {
            std::println("partial specialization <T*, S>");
        }
    };
} // namespace n235

namespace n236
{
    template <int A, int B>
    struct foo
    {
    };

    template <int A>
    struct foo<A, A>
    {
    };

    template <int A>
    struct foo<A, A + 1> // expressions (A + 1) are allowed!
    {
    };
} // namespace n236

namespace n237
{
    // primary
    template <typename T, size_t S>
    std::ostream &
    pretty_print(std::ostream &os, std::array<T, S> const &arr)
    {
        os << '[';
        if (S > 0)
        {
            size_t i = 0;
            for (; i < S - 1; ++i)
            {
                os << arr[i] << ',';
            }
            os << arr[S - 1];
        }
        os << ']' << std::endl;

        return os;
    }
} // namespace n237

// increase namespace number to remove specialization
namespace n237
{
    // NOTE:
    // `pretty_print` is a function template; can only be fully specialized!
    // This is an overload, not a specialization!!!

    // specialized on arrays of chars
    template <size_t S>
    std::ostream &
    pretty_print(std::ostream &os, std::array<char, S> const &arr)
    {
        os << '[';
        for (auto const &e : arr)
        {
            os << e;
        }
        os << ']';

        return os;
    }
} // namespace n237

// Defining variable templates

namespace n239
{
    constexpr double PI = 3.1415926535897932385L;

    template <typename T>
    T
    sphere_volume(T const r)
    {
        return static_cast<T>(4 * PI * r * r * r / 3);
    }
} // namespace n239

namespace n240
{
    template <typename T>
    struct PI
    {
        static const T value;
    };

    template <typename T>
    const T PI<T>::value = T(3.1415926535897932385L);

    template <typename T>
    T
    sphere_volume(T const r)
    {
        return 4 * PI<T>::value * r * r * r / 3;
    }
} // namespace n240

namespace n241
{
    // variable template
    template <typename T>
    constexpr T PI = T(3.1415926535897932385L);

    template <typename T>
    T
    sphere_volume(T const r)
    {
        return 4 * PI<T> * r * r * r / 3;
    }
} // namespace n241

namespace n242
{
    struct math_constants
    {
        // in-class initialization with static constexpr
        template <typename T>
        static constexpr T PI = T(3.1415926535897932385L);
    };

    template <typename T>
    T
    sphere_volume(T const r)
    {
        return 4 * math_constants::PI<T> * r * r * r / 3;
    }
} // namespace n242

namespace n243
{
    // out-of-class initialization with static const

    // only declaration
    struct math_constants
    {
        template <typename T>
        static const T PI; // must be `static const`, not `static constexpr`
    };

    // out-of-class initialization
    template <typename T>
    const T math_constants::PI = T(3.1415926535897932385L);

    template <typename T>
    T
    sphere_volume(T const r)
    {
        return 4 * math_constants::PI<T> * r * r * r / 3;
    }
} // namespace n243

namespace n244
{
    template <typename T>
    constexpr T SEPARATOR = '\n';

    template <>
    constexpr wchar_t SEPARATOR<wchar_t> = L'\n';

    template <typename T>
    std::basic_ostream<T> &
    show_parts(std::basic_ostream<T> &s, std::basic_string_view<T> const &str)
    {
        using size_type = typename std::basic_string_view<T>::size_type;
        size_type start = 0;
        size_type end;

        do
        {
            end = str.find(SEPARATOR<T>, start);
            s << '[' << str.substr(start, end - start) << ']' << SEPARATOR<T>;
            start = end + 1;
        } while (end != std::string::npos);

        return s;
    }
} // namespace n244

// Defining alias templates

namespace n245
{
    template <typename T>
    struct foo
    {
        typedef T value_type;
    };
} // namespace n245

namespace n246
{
    template <typename T>
    struct foo
    {
        using value_type = T;
    };
} // namespace n246

namespace n247
{
    // An alias template is a name that refers not to a type but a family of types.

    // alias template
    template <typename T>
    using customer_addresses_t = std::map<int, std::vector<T>>;

    struct delivery_address_t
    {
    };

    struct invoice_address_t
    {
    };

    // type aliases
    using customer_delivery_addresses_t = customer_addresses_t<delivery_address_t>;
    using customer_invoice_addresses_t  = customer_addresses_t<invoice_address_t>;
} // namespace n247

namespace n248
{
    // Alias templates cannot be specialized!
    //
    // primary
    // template <typename T, size_t S>
    // using list_t = std::vector<T>;

    // specialization
    // template <typename T>
    // using list_t<T, 1> = T; // error: Partial specialization of alias templates is not permitted

    // Here's a work-around (classes can be specialized - using the right tool for the right job):
    template <typename T, size_t S>
    struct list
    {
        using type = std::vector<T>;
    };

    template <typename T>
    struct list<T, 1>
    {
        using type = T;
    };

    template <typename T, size_t S>
    using list_t = list<T, S>::type;
} // namespace n248

int
main()
{
    {
        std::println("====================== using namespace n201 =============================");
        using namespace n201;

        auto a1 = add(42, 21);
        std::println("{}", a1); // 63
        auto a2 = add<int>(42, 21);
        std::println("{}", a2); // 63
        auto a3 = add<>(42, 21);
        std::println("{}", a3); // 63

        auto b = add<short>(42, 21);
        std::println("{}", b);  // 63

        // auto d1 = add(42.0, 21); // error
        auto d2 = add<double>(42.0, 21);
        std::println("{}", d2);      // 63

        auto f = add(foo(42), foo(21));
        std::println("{}", f.get()); // 63

        int  arr[]{1, 1, 2, 3, 5, 8, 11};
        auto odds = count_if(std::begin(arr), std::end(arr), [](int const n) { return n % 2 == 1; });
        std::println("{}", odds);    // 5
    }

    {
        std::println("\n====================== using namespace n202 =============================");
        using namespace n202;

        wrapper         a(42);   // wraps an int
        wrapper<int>    b(42);   // wraps an int
        wrapper<short>  c(42);   // wraps a short
        wrapper<double> d(42.0); // wraps a double
        wrapper         e(42.0); // wraps a double
        wrapper         f("42"); // wraps a char const *
    }

    {
        std::println("\n====================== using namespace n203 =============================");
        using namespace n203;

        wrapper<int> a(42); // wraps an int
        use_wrapper(&a);    // 42
    }

    {
        std::println("\n====================== using namespace n204 =============================");
        using namespace n204;

        composition<int> c;
        std::println("{}", c.add(41, 21)); // 62
    }

    {
        std::println("\n====================== using namespace n205 =============================");
        using namespace n205;

        composition c;
        std::println("{}", c.add<int>(41, 21)); // 62
        std::println("{}", c.add(41, 21));      // 62
    }

    {
        std::println("\n====================== using namespace n206 =============================");
        using namespace n206;

        // wrapper<double> a(42.0);
        wrapper a(42.0);

        auto d = a.get(); // double

        // member function template of class template
        auto n = a.as<int>(); // int
        // int  n = a.as();    // error

        std::println("{}", d); // 42
        std::println("{}", n); // 42
    }

    {
        std::println("\n====================== using namespace n209 =============================");
        using namespace n209;

        buffer<int, 10> b1;
        b1[0] = 42;
        std::println("{}", b1[0]); // 42

        auto b2 = make_buffer<int, 2 * 5>();
        std::println("{}", b2[0]); // 0
        std::println("{}", b2[1]); // 0
        std::println("{}", b2[2]); // 0

        buffer<int, 3 * 5> b3;

        static_assert(std::is_same_v<decltype(b1), decltype(b2)>);
        static_assert(not std::is_same_v<decltype(b2), decltype(b3)>);
        static_assert(not std::is_same_v<decltype(b1), decltype(b3)>);
    }

    {
        std::println("\n====================== using namespace n209 =============================");
        using namespace n209;

        buffer<int, 10>    b1;
        buffer<int, 2 * 5> b2;

        std::println("is same: {}", std::is_same_v<buffer<int, 10>, buffer<int, 2 * 5>>); // is same: true
        std::println("is same: {}", std::is_same_v<decltype(b1), decltype(b2)>);          // is same: true

        buffer<int, 3 * 5> b3;
        static_assert(std::is_same_v<decltype(b1), decltype(b2)>);
        static_assert(not std::is_same_v<decltype(b1), decltype(b3)>);
    }

    {
        std::println("\n====================== using namespace n210 =============================");
        using namespace n210;

        hello_command cmd;

        auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(&cmd);
        w1->output(); // Hello, world!

        auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(&cmd);
        w2->output(); // Hola mundo!
    }

    {
        std::println("\n====================== using namespace n211 =============================");
        using namespace n211;

        hello_command cmd;

        auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(cmd);
        w1->output(); // Hello, world!

        auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(cmd);
        w2->output(); // Hola mundo!
    }

    {
        std::println("\n====================== using namespace n212 =============================");
        using namespace n212;

        auto w1 = std::make_unique<smart_device<&say_hello_in_english>>();
        w1->output(); // Hello, world!

        auto w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
        w2->output(); // Hola mundo!

        static_assert(not std::is_same_v<decltype(w1), decltype(w2)>);
    }

    {
        std::println("\n====================== using namespace n212 =============================");
        using namespace n212;

        // pointer to base class
        std::unique_ptr<device> w1 = std::make_unique<smart_device<&say_hello_in_english>>();
        w1->output(); // Hello, world!

        std::unique_ptr<device> w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
        w2->output(); // Hola mundo!

        // types are the same
        static_assert(std::is_same_v<decltype(w1), decltype(w2)>);
    }

    {
        std::println("\n====================== using namespace n213 =============================");
        using namespace n213;

        [[maybe_unused]] foo<42>   f1; // foo<int>
        [[maybe_unused]] foo<42.0> f2; // foo<double> (allowed in C++20)
        // string literals cannot be used as arguments for non-type template parameters:
        // [[maybe_unused]] foo<"42"> f3;
    }

    {
        std::println("\n====================== using namespace n214 =============================");
        using namespace n214;

        // work-around in C++20
        [[maybe_unused]] foo<"42"> f;
    }

    {
        std::println("\n====================== using namespace n215 =============================");
        using namespace n215;

        // each parameter is deduced independently
        [[maybe_unused]] foo<42, 42.0, false, 'x'> f;
    }

    {
        std::println("\n====================== using namespace n216 =============================");
        using namespace n216;

        // wrapping_pair<int, double> p1(42, 42.5);
        wrapping_pair p1(42, 42.5);
        std::println("{} {}", p1.item1.get(), p1.item2.get()); // 42 42.5
        // std::println("{} {}", p1.item1.value, p1.item2.value); // error: value is private

        wrapping_pair<int, double, simple_wrapper> p2(42, 42.5);
        std::println("{} {}", p2.item1.value, p2.item2.value); // 42 42.5
    }

    {
        std::println("\n====================== using namespace n219 =============================");
        using namespace n219;

        foo f{42, 42.5};
        std::println("{} {}", f.a, f.b); // 42 42.5
    }

    {
        std::println("\n====================== using namespace n221 =============================");
        using namespace n221;

        // error: ‘using n221::foo<int>::value_type = int’ is protected within this context
        // bar<foo<int>> x;
    }

    {
        std::println("\n====================== using namespace n222 =============================");
        using namespace n222;

        foo<int> a;
        a.f();
        a.g();
    }

    {
        std::println("\n====================== using namespace n223 =============================");
        using namespace n223;

        foo<int>    *p = nullptr; // no instantiation here as pointers have always same size
        foo<int>     x;           // initialization happens here
        foo<double> *q = nullptr; // no instantiation here as pointers have always same size

        p->f();                   // void n223::foo<T>::f() [with T = int]
        p->g();                   // void n223::foo<T>::g() [with T = int]

        x.f();                    // void n223::foo<T>::f() [with T = int]
        x.g();                    // void n223::foo<T>::g() [with T = int]

        q->f();                   // void n223::foo<T>::f() [with T = double]
        q->g();                   // void n223::foo<T>::g() [with T = double]
    }

    {
        std::println("\n====================== using namespace n223 =============================");
        using namespace n223;

        // The compiler is required to instantiate the following:
        // - foo<int> when the x variable is declared [1]
        // - foo<int>::f() when the x.f() call occurs [2]
        // - foo<double> and foo<double>::g() when the q->g() call occurs. [3]

        [[maybe_unused]] foo<int> *p;
        foo<int>                   x; // [1]
        foo<double>               *q = nullptr;

        x.f();                        // void n223::foo<T>::f() [with T = int]    // [2]
        q->g();                       // void n223::foo<T>::g() [with T = double] // [3]
    }

    {
        std::println("\n====================== using namespace n225 =============================");
        using namespace n225;

        // Every specialization of a class template has its own copy of static members!!
        // Makes sense!

        foo<int>    a;
        foo<double> b;
        foo<double> c;

        // b and c are the same -> share static var 'data' !!!
        static_assert(std::is_same_v<decltype(b), decltype(c)>);
        static_assert(not std::is_same_v<decltype(a), decltype(b)>);
        static_assert(not std::is_same_v<decltype(a), decltype(c)>);

        std::println("{}", a.shared_data); // 0
        std::println("{}", b.shared_data); // 0
        std::println("{}", c.shared_data); // 0

        b.shared_data = 42;
        std::println("{}", a.shared_data); // 0
        std::println("{}", b.shared_data); // 42 // b and c share the same static member (as expected)
        std::println("{}", c.shared_data); // 42
    }

    {
        std::println("\n====================== using namespace ext =============================");
        using namespace ext;

        // Explicit instantiation

        // Why would you tell the compiler to generate instantiation from a template?
        // The answer is that it helps distribute libraries, reduce build times, and executable sizes.
        // If you are building a library that you want to distribute as a .lib file and that library
        // uses templates, the template definitions that are not instantiated are not put into the library.
        // But that leads to increased build times of your user code every time you use the library.
        // By forcing instantiations of templates in the library, those definitions are put into the
        // object files and the .lib file you are distributing. As a result, your user code only needs
        // to be linked to those available functions in the library file. This is what the Microsoft
        // MSVC CRT libraries do for all the stream, locale, and string classes. The libstdc++ library
        // does the same for string classes and others.

        // 'wrapper<int>' has been explicitly instantiated in source1.cpp.
        // The compiler won't instantiate wrapper<int> here.
        wrapper<int> a{0};

        std::println("{}", a.data);
        f(); // f() is defined in source1.cpp
        g(); // g() is defined in source2.cpp

        // Class Member Function Instantiation:
        // When you do explicit template declarations (not shown here), keep in mind that a class member function that
        // is defined within the body of the class is always considered inline and therefore it will always be
        // instantiated. Therefore, you can only use the extern keyword for member functions that are defined outside of
        // the class body.
    }

    {
        std::println("\n====================== using namespace n228 =============================");
        using namespace n228;

        std::println("{}", is_floating_point<int>::value);         // false; using generic template
        std::println("{}", is_floating_point<float>::value);       // true;  using specialized template
        std::println("{}", is_floating_point<double>::value);      // true;  using specialized template
        std::println("{}", is_floating_point<long double>::value); // true;  using specialized template
        std::println("{}", is_floating_point<std::string>::value); // false; using generic template
    }

    {
        std::println("\n====================== using namespace n228 =============================");
        using namespace n228;

        std::println("{}", is_floating_point_v<int>);         // false; using variable template
        std::println("{}", is_floating_point_v<float>);       // true
        std::println("{}", is_floating_point_v<double>);      // true
        std::println("{}", is_floating_point_v<long double>); // true
        std::println("{}", is_floating_point_v<std::string>); // false
    }

    {
        std::println("\n====================== using namespace n229 =============================");
        using namespace n229;

        std::println("{}", is_floating_point<int>::value);   // false
        std::println("{}", is_floating_point<float>::value); // true
    }

    {
        std::println("\n====================== using namespace n230 =============================");
        using namespace n230;

        // foo<int> has been declared but never defined!
        [[maybe_unused]] foo<double> a; // OK (compiler uses primary template)
        [[maybe_unused]] foo<int>   *b; // OK (pointer poses no problem)
        // foo<int>                  c; // error, foo<int> never defined
    }

    {
        std::println("\n====================== using namespace n231 =============================");
        using namespace n231;

        func(foo<int>{});    // int specialization
        func(foo<double>{}); // primary template
    }

    {
        std::println("\n====================== using namespace n232 =============================");
        using namespace n232;

        func(42.0); // primary template
        func(42);   // int specialization
    }

    {
        std::println("\n====================== using namespace n233 =============================");
        using namespace n233;

        // each specialization has its own copy of static members
        foo<double> a, b;
        foo<int>    c;

        std::println("{}", a.value); // 0
        std::println("{}", b.value); // 0
        std::println("{}", c.value); // 42

        a.value = 100;

        std::println("{}", a.value); // 100
        std::println("{}", b.value); // 100
        std::println("{}", c.value); // 42
    }

    {
        std::println("\n====================== using namespace n234 =============================");
        using namespace n234;

        func(1, 2);     // int-int specialization: 1 2
        func(1, 2.0);   // int-double specialization: 1 2
        func(1.0, 2.0); // primary template: 1 2
    }

    {
        std::println("\n====================== using namespace n235 =============================");
        using namespace n235;

        collection<char, 42>{}();  // primary template
        collection<int, 42>{}();   // partial specialization <int, S>
        collection<char, 10>{}();  // partial specialization <T, 10>
        collection<int *, 20>{}(); // partial specialization <T*, S>

        // Ambiguous partial specializations of 'collection<int, 10>'
        // error: collection<T,10> or collection<int,S>
        // collection<int, 10>{}();

        // Ambiguous partial specializations of 'collection<char *, 10>'
        // error: collection<T,10> or collection<T*,S>
        // collection<char *, 10>{}();
    }

    {
        std::println("\n====================== using namespace n237 =============================");
        using namespace n237;

        // Bad example as function templates can only be fully/explicitly specialized.
        // What is done here, is overloading!!!

        // What is key to understand here is that it's not the pretty_print function
        // template that is partially specialized but the std::array class template. Function
        // templates cannot be specialized and what we have here are overloaded functions.
        // However, std::array<char,S> is a specialization of the primary class template
        // std::array<T, S>.

        std::array arr{1, 1, 2, 3, 5, 8, 13, 21};
        pretty_print(std::cout, arr); // [1,1,2,3,5,8,13,21]

        std::array<char, 9> str;
        std::strcpy(str.data(), "template");
        pretty_print(std::cout, str); // without overload: [t,e,m,p,l,a,t,e,]
    }

    {
        std::println("\n====================== using namespace n237 =============================");
        using namespace n237;

        std::array<char, 9> str;
        std::strcpy(str.data(), "template");
        pretty_print(std::cout, str); // with overload: [template]
    }

    {
        std::println("\n====================== using namespace n239 =============================");
        using namespace n239;

        // no templates
        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);
        std::println("sphere volumes: {} {}", v1, v2); // sphere volumes: 310339.1 310339.08869221417
    }

    {
        std::println("\n====================== using namespace n240 =============================");
        using namespace n240;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);
        std::println("sphere volumes: {} {}", v1, v2); // sphere volumes: 310339.1 310339.08869221417
    }

    {
        std::println("\n====================== using namespace n241 =============================");
        using namespace n241;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);
        std::println("sphere volumes: {} {}", v1, v2); // sphere volumes: 310339.1 310339.08869221417
    }

    {
        std::println("\n====================== using namespace n242 =============================");
        using namespace n242;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);
        std::println("sphere volumes: {} {}", v1, v2); // sphere volumes: 310339.1 310339.08869221417
    }

    {
        std::println("\n====================== using namespace n243 =============================");
        using namespace n243;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);
        std::println("sphere volumes: {} {}", v1, v2); // sphere volumes: 310339.1 310339.08869221417
    }

    {
        std::println("\n====================== using namespace n244 =============================");
        using namespace n244;

        show_parts<char>(std::cout, "one\ntwo\nthree"); // [one]
                                                        // [two]
                                                        // [three]
        show_parts<wchar_t>(std::wcout, L"one line");   // [one line]
    }

    // Defining alias templates

    {
        std::println("\n====================== type aliases =====================================");

        typedef int                                      index_t [[maybe_unused]];
        typedef std::vector<std::pair<int, std::string>> NameValueList [[maybe_unused]];
        typedef int (*fn_ptr [[maybe_unused]])(int, char);
    }

    {
        std::println("\n====================== type aliases =====================================");

        using index_t [[maybe_unused]]       = int;
        using NameValueList [[maybe_unused]] = std::vector<std::pair<int, std::string>>;
        using fn_ptr1 [[maybe_unused]]       = int (*)(int, char); // old syntax
        using fn_ptr2 [[maybe_unused]]       = int(int, char);     // cool syntax!!!
    }

    {
        std::println("\n====================== using namespace n247 =============================");
        using namespace n248;

        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }

    // Exploring generic lambdas and lambda templates

    {
        std::println("\n====================== lambdas ==========================================");

        int arr[] = {1, 6, 3, 8, 4, 2, 9};
        std::sort(std::begin(arr), std::end(arr), [](int const a, int const b) { return a < b; });
        std::println("{}", arr[0]); // 1
        std::println("{}", arr[6]); // 9

        int  pivot = 5;
        auto count = std::count_if(std::begin(arr), std::end(arr), [pivot](int const a) { return a > pivot; });

        std::println("{}", count); // 3
    }

    {
        std::println("\n====================== lambda templates 2 ===============================");

        auto l1 = [](int a) { return a + a; };           // C++11, regular lambda (returns int)
        auto l2 = [](auto a) { return a + a; };          // C++14, generic lambda
        auto l3 = []<typename T>(T a) { return a + a; }; // C++20, template lambda

        int v1 = l1(42);
        int v2 = l1(42.0);                               // automatic conversion to int
        // auto v = l1(std::string{"42"});               // error

        auto        v3 = l2(42);
        auto        v4 = l2(42.0);              // v4 is a double
        std::string v5 = l2(std::string{"42"}); // string concatenation

        auto        v6 = l3(42);
        auto        v7 = l3(42.0);
        std::string v8 = l3(std::string{"42"});

        std::println("{}", v1); // 84
        std::println("{}", v2); // 84
        std::println("{}", v3); // 84
        std::println("{}", v4); // 84
        std::println("{}", v5); // 4242
        std::println("{}", v6); // 84
        std::println("{}", v7); // 84
        std::println("{}", v8); // 4242
    }

    {
        std::println("\n====================== lambda templates 3 ===============================");

        auto l1 = [](int a, int b) { return a + b; };
        auto l2 = [](auto a, auto b) { return a + b; };                   // generic lambda
        auto l3 = []<typename T, typename U>(T a, U b) { return a + b; }; // template lambda
        auto l4 = [](auto a, decltype(a) b) { return a + b; };

        // auto v = l1(std::string{ "42" }, '1');
        // auto v = l4(std::string{ "42" }, '1');

        auto v1 = l1(42, 1);
        auto v2 = l1(42.0, 1.0);

        auto v3 = l2(42, 1);
        auto v4 = l2(42.0, 1);
        auto v5 = l2(std::string{"42"}, '1');
        auto v6 = l2(std::string{"42"}, std::string{"1"});

        auto v7  = l3(42, 1);
        auto v8  = l3(42.0, 1);
        auto v9  = l3(std::string{"42"}, '1');
        auto v10 = l3(std::string{"42"}, std::string{"42"});

        auto v11 = l4(42.0, 1);
        auto v12 = l4(42, 1.0);

        std::println("{}", v1);  // 43
        std::println("{}", v2);  // 43

        std::println("{}", v3);  // 43
        std::println("{}", v4);  // 43
        std::println("{}", v5);  // 421
        std::println("{}", v6);  // 421

        std::println("{}", v7);  // 43
        std::println("{}", v8);  // 43
        std::println("{}", v9);  // 421
        std::println("{}", v10); // 4242

        std::println("{}", v11); // 43
        std::println("{}", v12); // 43
    }

    {
        std::println("\n====================== lambda templates 4 ===============================");

        auto l = []<typename T, size_t N>(std::array<T, N> const &arr) {
            return std::accumulate(arr.begin(), arr.end(), static_cast<T>(0));
        };

        // auto v1 = l(1); // error: 1 is not an array

        auto v2 = l(std::array<int, 3>{1, 2, 3});
        auto v3 = l(std::array{1, 2, 3});

        std::println("{}", v2); // 6
        std::println("{}", v3); // 6
    }

    {
        std::println("\n====================== lambda templates 5 ===============================");

        // a, b must be same type
        auto l = []<typename T>(T a, T b) { return a + b; };

        auto v1 [[maybe_unused]] = l(42, 1); // OK
        // auto v = l(42.0, 1);              // error

        auto v2 [[maybe_unused]] = l(42.0, 1.0); // OK
        // auto v = l(42, false);                // error

        auto v3 = l(std::string{"42"}, std::string{"1"}); // OK
        // auto v = l(std::string{ "42" }, '1');          // error

        std::println("{}", v1); // 43
        std::println("{}", v2); // 43
        std::println("{}", v3); // 421
    }

    {
        std::println("\n====================== lambda templates 6 ===============================");

        // regular lambda

        std::function<int(int)> factorial = [&factorial](int const n) {
            if (n < 2)
            {
                return 1;
            }
            else
            {
                return n * factorial(n - 1);
            }
        };

        std::println("{}", factorial(5)); // 120
    }

    {
        std::println("\n====================== lambda templates 7 ===============================");

        // generic lambda

        auto factorial = [](auto f, int const n) {
            if (n < 2)
            {
                return 1;
            }
            else
            {
                return n * f(f, n - 1);
            }
        };

        std::println("{}", factorial(factorial, 5)); // 120
    }
}
