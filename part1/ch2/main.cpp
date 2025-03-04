#include "wrapper.h"
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <print>

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
        return foo((int)a + (int)b);
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
                total++;
            }
        }
        return total;
    }
} // namespace n201

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
    template <typename T>
    class wrapper;

    void use_wrapper(wrapper<int> *ptr);

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

namespace n204
{
    template <typename T>
    class composition
    {
      public:
        T
        add(T const a, T const b)
        {
            return a + b;
        }
    };
} // namespace n204

namespace n205
{
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

namespace n207
{
    template <typename T>
    class wrapper1
    { /* ... */
    };

    template <typename T = int>
    class wrapper2
    { /* ... */
    };

    template <typename>
    class wrapper3;

    template <typename = int>
    class wrapper4;

    template <typename... T>
    class wrapper5
    { /* ... */
    };

    template <typename T>
    concept WrappableType = std::is_trivial_v<T>;

    template <WrappableType T>
    class wrapper6
    { /* ... */
    };

    template <WrappableType T = int>
    class wrapper7
    { /* ... */
    };

    template <WrappableType... T>
    class wrapper8
    { /* ... */
    };
} // namespace n207

namespace n208
{
    template <int V>
    class foo1
    { /*...*/
    };

    template <int V = 42>
    class foo2
    { /*...*/
    };

    template <int... V>
    class foo3
    { /*...*/
    };
} // namespace n208

namespace n209
{
    template <typename T, size_t S>
    class buffer
    {
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

    template <typename Command, void (Command::*action)()>
    struct smart_device : device
    {
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
            (*action)();
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
    };
} // namespace n213

namespace n214
{
    template <size_t N>
    struct string_literal
    {
        constexpr string_literal(const char (&str)[N])
        {
            std::copy_n(str, N, value);
        }

        char value[N];
    };

    template <string_literal x>
    struct foo
    {
    };
} // namespace n214

namespace n215
{
    template <auto... x>
    struct foo
    { /* ... */
    };
} // namespace n215

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
        fancy_wrapper(T const v) : value(v)
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
        wrapping_pair(T const a, U const b) : item1(a), item2(b)
        {
        }

        W<T> item1;
        W<U> item2;
    };
} // namespace n216

namespace n217
{
    template <typename T = int>
    class foo
    { /*...*/
    };

    template <typename T = int, typename U = double>
    class bar
    { /*...*/
    };
} // namespace n217

namespace n218
{
    // template <typename T = int, typename U>
    // class bar { };

    template <typename T = int, typename U>
    void
    func()
    {
    }
} // namespace n218

namespace n219
{
    template <typename T, typename U = double>
    struct foo;

    template <typename T = int, typename U>
    struct foo;

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

    // template <typename T = int> // error
    // struct foo {};
} // namespace n220

namespace n221
{
    template <typename T>
    struct foo
    {
      protected:
        using value_type = T;
    };

    template <typename T, typename U = typename T::value_type>
    struct bar
    {
        using value_type = U;
    };
} // namespace n221

// namespace n222
// {
// template <typename T> struct foo
// {
//     void
//     f()
//     {
//     }
//
//     void
//     g()
//     {
//         int a = "42";
//     }
// };
// } // namespace n222

namespace n223
{
    template <typename T>
    struct foo
    {
        void
        f()
        {
        }
        void
        g()
        {
        }
    };
} // namespace n223

namespace n224
{
    template <typename T>
    struct control
    {
    };

    template <typename T>
    struct button : public control<T>
    {
    };

    void
    show(button<int> *ptr)
    {
        control<int> *c [[maybe_unused]] = ptr;
    }
} // namespace n224

namespace n225
{
    template <typename T>
    struct foo
    {
        static T data;
    };

    template <typename T>
    T foo<T>::data = 0;
} // namespace n225

// namespace n226
// {
// template <typename T> struct wrapper
// {
//     T value;
// };
//
// template struct wrapper<int>; // [1]
// } // namespace n226
//
// template struct n226::wrapper<double>; // [2]
//
// namespace n226
// {
// template <typename T>
// T
// add(T const a, T const b)
// {
//     return a + b;
// }
//
// template int add(int, int); // [1]
// } // namespace n226
//
// template int n226::add(int, int); // [2]

namespace n227
{
    template <typename T>
    class foo
    {
        struct bar
        {
        };

        T
        f(bar const arg [[maybe_unused]])
        {
            return {};
        }
    };

    template int foo<int>::f(foo<int>::bar);
} // namespace n227

namespace n228
{
    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };

    template <>
    struct is_floating_point<float>
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<double>
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<long double>
    {
        constexpr static bool value = true;
    };

    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace n228

namespace n229
{
    template <typename T>
    struct is_floating_point;

    template <>
    struct is_floating_point<float>
    {
        constexpr static bool value = true;
    };

    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };
} // namespace n229

namespace n230
{
    template <typename>
    struct foo
    {
    }; // primary template

    template <>
    struct foo<int>; // explicit specialization declaration
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

    template <>
    void
    func(foo<int>)
    {
        std::println("int specialization");
    }
} // namespace n231

namespace n232
{
    template <typename T>
    void
    func(T a [[maybe_unused]])
    {
        std::println("primary template");
    }

    template <>
    void
    func(int a [[maybe_unused]] /*= 0*/) // error: default argument
    {
        std::println("int specialization");
    }
} // namespace n232

namespace n233
{
    template <typename T>
    struct foo
    {
        static T value;
    };

    template <typename T>
    T foo<T>::value = 0;

    template <>
    int foo<int>::value = 42;
} // namespace n233

namespace n234
{
    template <typename T, typename U>
    void
    func(T a [[maybe_unused]], U b [[maybe_unused]])
    {
        std::println("primary template");
    }

    template <>
    void
    func(int a [[maybe_unused]], int b [[maybe_unused]])
    {
        std::println("int-int specialization");
    }

    template <>
    void
    func(int a [[maybe_unused]], double b [[maybe_unused]])
    {
        std::println("int-double specialization");
    }
} // namespace n234

namespace n235
{
    template <typename T, int S>
    struct collection
    {
        void
        operator()()
        {
            std::println("primary template");
        }
    };

    template <typename T>
    struct collection<T, 10>
    {
        void
        operator()()
        {
            std::println("partial specialization <T, 10>");
        }
    };

    template <int S>
    struct collection<int, S>
    {
        void
        operator()()
        {
            std::println("partial specialization <int, S>");
        }
    };

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

    // template <int A>
    // struct foo<A, A + 1> {}; // error
} // namespace n236

namespace n237
{
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
        os << ']';

        return os;
    }
} // namespace n237

namespace n238
{
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
} // namespace n238

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
    template <class T>
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
        template <class T>
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
    struct math_constants
    {
        template <class T>
        static const T PI;
    };

    template <class T>
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
    template <typename T>
    using customer_addresses_t = std::map<int, std::vector<T>>;

    struct delivery_address_t
    {
    };
    struct invoice_address_t
    {
    };

    using customer_delivery_addresses_t = customer_addresses_t<delivery_address_t>;
    using customer_invoice_addresses_t  = customer_addresses_t<invoice_address_t>;
} // namespace n247

namespace n247
{
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
    using list_t = typename list<T, S>::type;
} // namespace n247

namespace n248
{

}

int
main()
{
    {
        std::println("====================== using namespace n201");
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

        int arr[]{1, 1, 2, 3, 5, 8, 11};
        int odds = count_if(std::begin(arr), std::end(arr), [](int const n) { return n % 2 == 1; });
        std::println("{}", odds);    // 5
    }

    {
        std::println("\n====================== using namespace n202");
        using namespace n202;

        wrapper         a(42);   // wraps an int
        wrapper<int>    b(42);   // wraps an int
        wrapper<short>  c(42);   // wraps a short
        wrapper<double> d(42.0); // wraps a double
        wrapper         e("42"); // wraps a char const *
    }

    {
        std::println("\n====================== using namespace n203");
        using namespace n203;

        wrapper<int> a(42); // wraps an int
        use_wrapper(&a);
    }

    {
        std::println("\n====================== using namespace n204");
        using namespace n204;

        composition<int> c;
        std::println("{}", c.add(41, 21));
    }

    {
        std::println("\n====================== using namespace n205");
        using namespace n205;

        composition c;
        std::println("{}", c.add<int>(41, 21));
        std::println("{}", c.add(41, 21));
    }

    {
        std::println("\n====================== using namespace n206");
        using namespace n206;

        // wrapper<double> a(42.0);
        wrapper a(42.0);

        auto d = a.get();     // double
        auto n = a.as<int>(); // int
        // int  n = a.as();   // error
        std::println("{}", d);
        std::println("{}", n);
    }

    {
        using namespace n209;
        std::println("\n====================== using namespace n209");

        buffer<int, 10> b1;
        b1[0] = 42;
        std::println("{}", b1[0]);

        auto b2 [[maybe_unused]] = make_buffer<int, 10>();
    }

    {
        using namespace n209;
        std::println("\n====================== using namespace n209");

        buffer<int, 10>    b1;
        buffer<int, 2 * 5> b2;

        std::println("{}", std::is_same_v<buffer<int, 10>, buffer<int, 2 * 5>>);
        std::println("{}", std::is_same_v<decltype(b1), decltype(b2)>);

        static_assert(std::is_same_v<decltype(b1), decltype(b2)>);

        buffer<int, 3 * 5> b3;
        static_assert(!std::is_same_v<decltype(b1), decltype(b3)>);
    }

    {
        using namespace n210;
        std::println("\n====================== using namespace n210");

        hello_command cmd;

        auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(&cmd);
        w1->output();

        auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(&cmd);
        w2->output();
    }

    {
        using namespace n211;
        std::println("\n====================== using namespace n211");

        hello_command cmd;

        auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(cmd);
        w1->output();

        auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(cmd);
        w2->output();
    }

    {
        using namespace n212;
        std::println("\n====================== using namespace n212");

        auto w1 = std::make_unique<smart_device<&say_hello_in_english>>();
        w1->output();

        auto w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
        w2->output();

        static_assert(!std::is_same_v<decltype(w1), decltype(w2)>);
    }

    {
        using namespace n212;
        std::println("\n====================== using namespace n212");

        std::unique_ptr<device> w1 = std::make_unique<smart_device<&say_hello_in_english>>();
        w1->output();

        std::unique_ptr<device> w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
        w2->output();

        static_assert(std::is_same_v<decltype(w1), decltype(w2)>);
    }

    {
        using namespace n213;
        std::println("\n====================== using namespace n213");
        [[maybe_unused]] foo<42>   f1; // foo<int>
        [[maybe_unused]] foo<42.0> f2; // foo<double>
        // [[maybe_unused]] foo<"42"> f3; // error
    }

    {
        using namespace n214;
        std::println("\n====================== using namespace n214");
        [[maybe_unused]] foo<"42"> f;
    }

    {
        using namespace n215;
        std::println("\n====================== using namespace n215");
        [[maybe_unused]] foo<42, 42.0, false, 'x'> f;
    }

    {
        using namespace n216;
        std::println("\n====================== using namespace n216");
        wrapping_pair<int, double> p1(42, 42.0);
        std::println("{} {}", p1.item1.get(), p1.item2.get());

        wrapping_pair<int, double, simple_wrapper> p2(42, 42.0);
        std::println("{} {}", p2.item1.value, p2.item2.value);
    }

    {
        using namespace n219;
        std::println("\n====================== using namespace n219");
        foo f [[maybe_unused]]{42, 42.0};
    }

    {
        using namespace n221;
        std::println("\n====================== using namespace n221");
        // bar<foo<int>> x;
    }

    // {
    //     using namespace n222;
    //     std::println("//     using namespace n222");
    //     foo<int> a;
    //     a.f();
    //     // a.g();
    // }

    {
        using namespace n223;
        std::println("\n====================== using namespace n223");

        [[maybe_unused]] foo<int>    *p;
        [[maybe_unused]] foo<int>     x;
        [[maybe_unused]] foo<double> *q;
    }

    {
        using namespace n223;
        std::println("\n====================== using namespace n223");

        [[maybe_unused]] foo<int> *p;
        foo<int>                   x;
        foo<double>               *q = nullptr;

        x.f();
        q->g();
    }

    {
        using namespace n225;
        std::println("\n====================== using namespace n225");

        foo<int>    a;
        foo<double> b;
        foo<double> c;

        std::println("{}", a.data); // 0
        std::println("{}", b.data); // 0
        std::println("{}", c.data); // 0

        b.data = 42;
        std::println("{}", b.data); // 42
        std::println("{}", c.data); // 42
    }

    {
        using namespace ext;
        std::println("\n====================== using namespace ext");

        wrapper<int> a{0};

        std::println("{}", a.data);
        f();
        g();
    }

    {
        using namespace n228;
        std::println("\n====================== using namespace n228");

        std::println("{}", is_floating_point<int>::value);
        std::println("{}", is_floating_point<float>::value);
        std::println("{}", is_floating_point<double>::value);
        std::println("{}", is_floating_point<long double>::value);
        std::println("{}", is_floating_point<std::string>::value);
    }

    {
        using namespace n228;
        std::println("\n====================== using namespace n228");

        std::println("{}", is_floating_point_v<int>);
        std::println("{}", is_floating_point_v<float>);
        std::println("{}", is_floating_point_v<double>);
        std::println("{}", is_floating_point_v<long double>);
        std::println("{}", is_floating_point_v<std::string>);
    }

    {
        using namespace n229;
        std::println("\n====================== using namespace n229");

        std::println("{}", is_floating_point<int>::value);
        std::println("{}", is_floating_point<float>::value);
    }

    {
        using namespace n230;
        std::println("\n====================== using namespace n230");

        [[maybe_unused]] foo<double> a; // OK
        [[maybe_unused]] foo<int>   *b; // OK
        // foo<int> c;                  // error, foo<int> incomplete type
    }

    {
        using namespace n231;
        std::println("\n====================== using namespace n231");

        func(foo<int>{});
        func(foo<double>{});
    }

    {
        using namespace n232;
        std::println("\n====================== using namespace n232");

        func(42.0);
        func(42);
    }

    {
        using namespace n233;
        std::println("\n====================== using namespace n233");

        foo<double> a, b;
        std::println("{}", a.value);
        std::println("{}", b.value);

        foo<int> c;
        std::println("{}", c.value);

        a.value = 100;
        std::println("{}", a.value);
        std::println("{}", b.value);
        std::println("{}", c.value);
    }

    {
        using namespace n234;
        std::println("\n====================== using namespace n234");

        func(1, 2);
        func(1, 2.0);
        func(1.0, 2.0);
    }

    {
        using namespace n235;
        std::println("\n====================== using namespace n235");

        collection<char, 42>{}();  // primary template
        collection<int, 42>{}();   // partial specialization <int, S>
        collection<char, 10>{}();  // partial specialization <T, 10>
        collection<int *, 20>{}(); // partial specialization <T*, S>

        // error: collection<T,10> or collection<int,S>
        // collection<int, 10>{}();
        // error: collection<T,10> or collection<T*,S>
        // collection<char*, 10>{}();
    }

    {
        using namespace n237;
        std::println("\n====================== using namespace n237");

        std::array<int, 9> arr{1, 1, 2, 3, 5, 8, 13, 21};
        pretty_print(std::cout, arr);

        std::array<char, 9> str;
        std::strcpy(str.data(), "template");
        pretty_print(std::cout, str);
    }

    {
        using namespace n238;
        std::println("\n====================== using namespace n238");

        std::array<char, 9> str;
        std::strcpy(str.data(), "template");
        pretty_print(std::cout, str);
    }

    {
        using namespace n239;
        std::println("\n====================== using namespace n239");

        float  v1 [[maybe_unused]] = sphere_volume(42.0f);
        double v2 [[maybe_unused]] = sphere_volume(42.0);
    }

    {
        using namespace n240;
        std::println("\n====================== using namespace n240");

        float  v1 [[maybe_unused]] = sphere_volume(42.0f);
        double v2 [[maybe_unused]] = sphere_volume(42.0);
    }

    {
        using namespace n241;
        std::println("\n====================== using namespace n241");

        float  v1 [[maybe_unused]] = sphere_volume(42.0f);
        double v2 [[maybe_unused]] = sphere_volume(42.0);
    }

    {
        using namespace n242;
        std::println("\n====================== using namespace n242");

        float  v1 [[maybe_unused]] = sphere_volume(42.0f);
        double v2 [[maybe_unused]] = sphere_volume(42.0);
    }

    {
        using namespace n244;
        std::println("\n====================== using namespace n244");
        show_parts<char>(std::cout, "one\ntwo\nthree");
        show_parts<wchar_t>(std::wcout, L"one line");
    }

    {
        typedef int                                      index_t [[maybe_unused]];
        typedef std::vector<std::pair<int, std::string>> NameValueList [[maybe_unused]];
        typedef int (*fn_ptr [[maybe_unused]])(int, char);
    }

    {
        using index_t [[maybe_unused]]       = int;
        using NameValueList [[maybe_unused]] = std::vector<std::pair<int, std::string>>;
        using fn_ptr [[maybe_unused]]        = int(int, char);
    }

    {
        using namespace n247;
        std::println("\n====================== using namespace n247");
        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }

    {
        int arr[] = {1, 6, 3, 8, 4, 2, 9};
        std::sort(std::begin(arr), std::end(arr), [](int const a, int const b) { return a > b; });

        int  pivot = 5;
        auto count = std::count_if(std::begin(arr), std::end(arr), [pivot](int const a) { return a > pivot; });

        std::println("{}", count);
    }

    {
        auto l1 = [](int a) { return a + a; };           // C++11, regular lambda
        auto l2 = [](auto a) { return a + a; };          // C++14, generic lambda
        auto l3 = []<typename T>(T a) { return a + a; }; // C++20, template lambda

        auto v1 [[maybe_unused]] = l1(42);               // OK
        auto v2 [[maybe_unused]] = l1(42.0);             // warning
        // auto v3               = l1(std::string{"42"});  // error

        auto v5 [[maybe_unused]] = l2(42);                 // OK
        auto v6 [[maybe_unused]] = l2(42.0);               // OK
        auto v7 [[maybe_unused]] = l2(std::string{"42"});  // OK

        auto v8 [[maybe_unused]]  = l3(42);                // OK
        auto v9 [[maybe_unused]]  = l3(42.0);              // OK
        auto v10 [[maybe_unused]] = l3(std::string{"42"}); // OK
    }

    {
        auto l1 = [](int a, int b) { return a + b; };
        auto l2 = [](auto a, auto b) { return a + b; };
        auto l3 = []<typename T, typename U>(T a, U b) { return a + b; };
        auto l4 = [](auto a, decltype(a) b) { return a + b; };

        auto v1 [[maybe_unused]] = l1(42, 1);     // OK
        auto v2 [[maybe_unused]] = l1(42.0, 1.0); // warning
        // auto v3 = l1(std::string{ "42" }, '1'); // error

        auto v4 [[maybe_unused]] = l2(42, 1);                                // OK
        auto v5 [[maybe_unused]] = l2(42.0, 1);                              // OK
        auto v6                  = l2(std::string{"42"}, '1');               // OK
        auto v7                  = l2(std::string{"42"}, std::string{"1"});  // OK

        auto v8 [[maybe_unused]] = l3(42, 1);                                // OK
        auto v9 [[maybe_unused]] = l3(42.0, 1);                              // OK
        auto v10                 = l3(std::string{"42"}, '1');               // OK
        auto v11                 = l3(std::string{"42"}, std::string{"42"}); // OK

        auto v12 [[maybe_unused]] = l4(42.0, 1);                             // OK
        auto v13 [[maybe_unused]] = l4(42, 1.0);                             // warning
        // auto v14 = l4(std::string{ "42" }, '1');                          // error
    }

    {
        auto l = []<typename T, size_t N>(std::array<T, N> const &arr) {
            return std::accumulate(arr.begin(), arr.end(), static_cast<T>(0));
        };

        // auto v1 = l(1); // error
        auto v2 [[maybe_unused]] = l(std::array<int, 3>{1, 2, 3});
    }

    {
        auto l = []<typename T>(T a, T b) { return a + b; };

        auto v1 [[maybe_unused]] = l(42, 1); // OK
        // auto v2 = l(42.0, 1);             // error

        auto v4 [[maybe_unused]] = l(42.0, 1.0); // OK
        // auto v5 = l(42, false);               // error

        auto v6 = l(std::string{"42"}, std::string{"1"}); // OK
        // auto v6 = l(std::string{ "42" }, '1');         // error
    }

    {
        std::function<int(int)> factorial;
        factorial = [&factorial](int const n) {
            if (n < 2)
            {
                return 1;
            }
            else
            {
                return n * factorial(n - 1);
            }
        };

        std::println("{}", factorial(5));
    }

    {
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

        std::println("{}", factorial(factorial, 5));
    }
}
