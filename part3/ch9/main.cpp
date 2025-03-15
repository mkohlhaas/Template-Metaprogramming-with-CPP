#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <format>
#include <iostream>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace n901
{
    int
    sum_proper_divisors(int const number)
    {
        int result = 1;

        for (int i = 2; i <= std::sqrt(number); ++i)
        {
            if (number % i == 0)
            {
                result += (i == (number / i)) ? i : (i + number / i);
            }
        }

        return result;
    }

    bool
    is_abundant(int const number)
    {
        return sum_proper_divisors(number) > number;
    }
} // namespace n901

namespace n902
{
    template <typename R>
    struct step_iterator;

    template <typename R>
    struct step_sentinel
    {
        using base      = std::ranges::iterator_t<R>;
        using size_type = std::ranges::range_difference_t<R>;

        step_sentinel() = default;

        constexpr step_sentinel(base end) : end_{end}
        {
        }
        constexpr bool is_at_end(step_iterator<R> it) const;

      private:
        base end_;
    };

    template <typename R>
    struct step_iterator : std::ranges::iterator_t<R>
    {
        using base           = std::ranges::iterator_t<R>;
        using value_type     = typename std::ranges::range_value_t<R>;
        using reference_type = typename std::ranges::range_reference_t<R>;

        constexpr step_iterator(base start, base end, std::ranges::range_difference_t<R> step)
            : pos_{start}, end_{end}, step_{step}
        {
        }

        constexpr step_iterator
        operator++(int)
        {
            auto ret = *this;
            pos_     = std::ranges::next(pos_, step_, end_);
            return ret;
        }

        constexpr step_iterator &
        operator++()
        {
            pos_ = std::ranges::next(pos_, step_, end_);
            return *this;
        }

        constexpr reference_type
        operator*() const
        {
            return *pos_;
        }

        constexpr bool
        operator==(step_sentinel<R> s) const
        {
            return s.is_at_end(*this);
        }

        constexpr base const
        value() const
        {
            return pos_;
        }

      private:
        base                               pos_;
        base                               end_;
        std::ranges::range_difference_t<R> step_;
    };

    template <typename R>
    constexpr bool
    step_sentinel<R>::is_at_end(step_iterator<R> it) const
    {
        return end_ == it.value();
    }

    template <std::ranges::view R>
    struct step_view : public std::ranges::view_interface<step_view<R>>
    {
      private:
        R                                  base_;
        std::ranges::range_difference_t<R> step_;

      public:
        step_view() = default;

        constexpr step_view(R base, std::ranges::range_difference_t<R> step) : base_(std::move(base)), step_(step)
        {
        }

        constexpr R
        base() const &
            requires std::copy_constructible<R>
        {
            return base_;
        }
        constexpr R
        base() &&
        {
            return std::move(base_);
        }

        constexpr std::ranges::range_difference_t<R> const &
        increment() const
        {
            return step_;
        }

        constexpr auto
        begin()
        {
            return step_iterator<R const>(std::ranges::begin(base_), std::ranges::end(base_), step_);
        }

        constexpr auto
        begin() const
            requires std::ranges::range<R const>
        {
            return step_iterator<R const>(std::ranges::begin(base_), std::ranges::end(base_), step_);
        }

        constexpr auto
        end()
        {
            return step_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto
        end() const
            requires std::ranges::range<R const>
        {
            return step_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto
        size() const
            requires std::ranges::sized_range<R const>
        {
            auto d = std::ranges::size(base_);
            return step_ == 1 ? d : static_cast<int>((d + 1) / step_);
        }

        constexpr auto
        size()
            requires std::ranges::sized_range<R>
        {
            auto d = std::ranges::size(base_);
            return step_ == 1 ? d : static_cast<int>((d + 1) / step_);
        }
    };

    template <class R>
    step_view(R &&base, std::ranges::range_difference_t<R> step) -> step_view<std::ranges::views::all_t<R>>;

    namespace details
    {
        using test_range_t = std::ranges::views::all_t<std::vector<int>>;
        static_assert(std::input_iterator<step_iterator<test_range_t>>);
        static_assert(std::sentinel_for<step_sentinel<test_range_t>, step_iterator<test_range_t>>);

        struct step_view_fn_closure
        {
            std::size_t step_;
            constexpr step_view_fn_closure(std::size_t step) : step_(step)
            {
            }

            template <std::ranges::range R>
            constexpr auto
            operator()(R &&r) const
            {
                return step_view(std::forward<R>(r), step_);
            }
        };

        struct step_view_fn
        {
            template <std::ranges::range R>
            constexpr auto
            operator()(R &&r, std::size_t step) const
            {
                return step_view(std::forward<R>(r), step);
            }

            constexpr auto
            operator()(std::size_t step) const
            {
                return step_view_fn_closure(step);
            }
        };

        template <std::ranges::range R>
        constexpr auto
        operator|(R &&r, step_view_fn_closure &&a)
        {
            return std::forward<step_view_fn_closure>(a)(std::forward<R>(r));
        }
    } // namespace details

    namespace views
    {
        inline constexpr details::step_view_fn step;
    }
} // namespace n902

namespace n903
{
    template <typename R>
    struct replicate_iterator;

    template <typename R>
    struct replicate_sentinel
    {
        using base      = std::ranges::iterator_t<R>;
        using size_type = std::ranges::range_difference_t<R>;

        replicate_sentinel() = default;

        constexpr replicate_sentinel(base end) : end_{end}
        {
        }
        constexpr bool is_at_end(replicate_iterator<R> it) const;

      private:
        base end_;
    };

    template <typename R>
    struct replicate_iterator : std::ranges::iterator_t<R>
    {
        using base           = std::ranges::iterator_t<R>;
        using value_type     = typename std::ranges::range_value_t<R>;
        using reference_type = typename std::ranges::range_reference_t<R>;

        constexpr replicate_iterator(base start, std::ranges::range_difference_t<R> count) : pos_{start}, count_{count}
        {
        }

        constexpr replicate_iterator
        operator++(int)
        {
            if (step_ == count_)
            {
                step_ = 1;
                pos_++;
            }
            else
            {
                step_++;
            }

            return pos_;
        }

        constexpr replicate_iterator &
        operator++()
        {
            if (step_ == count_)
            {
                step_ = 1;
                pos_++;
            }
            else
            {
                step_++;
            }

            return (*this);
        }

        constexpr reference_type
        operator*() const
        {
            return *pos_;
        }

        constexpr bool
        operator==(replicate_sentinel<R> s) const
        {
            return s.is_at_end(*this);
        }

        constexpr base const
        value() const
        {
            return pos_;
        }

      private:
        base                               pos_;
        std::ranges::range_difference_t<R> count_;
        std::ranges::range_difference_t<R> step_ = 1;
    };

    template <typename R>
    constexpr bool
    replicate_sentinel<R>::is_at_end(replicate_iterator<R> it) const
    {
        return end_ == it.value();
    }

    template <std::ranges::view R>
    struct replicate_view : public std::ranges::view_interface<replicate_view<R>>
    {
      private:
        R                                  base_;
        std::ranges::range_difference_t<R> count_;

      public:
        replicate_view() = default;

        constexpr replicate_view(R base, std::ranges::range_difference_t<R> count)
            : base_(std::move(base)), count_(count)
        {
        }

        constexpr R
        base() const &
            requires std::copy_constructible<R>
        {
            return base_;
        }
        constexpr R
        base() &&
        {
            return std::move(base_);
        }

        constexpr std::ranges::range_difference_t<R> const &
        increment() const
        {
            return count_;
        }

        constexpr auto
        begin()
        {
            return replicate_iterator<R const>(std::ranges::begin(base_), count_);
        }

        constexpr auto
        begin() const
            requires std::ranges::range<R const>
        {
            return replicate_iterator<R const>(std::ranges::begin(base_), count_);
        }

        constexpr auto
        end()
        {
            return replicate_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto
        end() const
            requires std::ranges::range<R const>
        {
            return replicate_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto
        size() const
            requires std::ranges::sized_range<R const>
        {
            return count_ * std::ranges::size(base_);
        }

        constexpr auto
        size()
            requires std::ranges::sized_range<R>
        {
            return count_ * std::ranges::size(base_);
        }
    };

    template <class R>
    replicate_view(R &&base, std::ranges::range_difference_t<R> count) -> replicate_view<std::ranges::views::all_t<R>>;

    namespace details
    {
        using test_range_t = std::ranges::views::all_t<std::vector<int>>;
        static_assert(std::input_iterator<replicate_iterator<test_range_t>>);
        static_assert(std::sentinel_for<replicate_sentinel<test_range_t>, replicate_iterator<test_range_t>>);

        struct replicate_view_fn_closure
        {
            std::size_t step_;
            constexpr replicate_view_fn_closure(std::size_t step) : step_(step)
            {
            }

            template <std::ranges::range R>
            constexpr auto
            operator()(R &&r) const
            {
                return replicate_view(std::forward<R>(r), step_);
            }
        };

        struct replicate_view_fn
        {
            template <std::ranges::range R>
            constexpr auto
            operator()(R &&r, std::size_t step) const
            {
                return replicate_view(std::forward<R>(r), step);
            }

            constexpr auto
            operator()(std::size_t step) const
            {
                return replicate_view_fn_closure(step);
            }
        };

        template <std::ranges::range R>
        constexpr auto
        operator|(R &&r, replicate_view_fn_closure &&a)
        {
            return std::forward<replicate_view_fn_closure>(a)(std::forward<R>(r));
        }
    } // namespace details

    namespace views
    {
        inline constexpr details::replicate_view_fn replicate;
    }
} // namespace n903

struct Item
{
    int         id;
    std::string name;
    double      price;
};

int
main()
{
    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        std::println("abundant numbers: ");

        for (int i = 1; i <= 100; i++)
        {
            if (is_abundant(i))
            {
                std::print("{} ", i);
            }
        }
    }

    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        std::println("abundant numbers: ");

        for (int i : std::views::iota(1, 101) | std::views::filter(is_abundant))
        {
            std::print("{} ", i);
        }
    }

    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        std::println("abundant numbers: ");

        for (int i : std::ranges::filter_view(std::ranges::iota_view(1, 101), is_abundant))
        {
            std::print("{} ", i);
        }
    }

    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        std::vector<int> nums{10, 12, 14, 16, 18, 20};
        for (int i : nums | std::views::filter(is_abundant))
        {
            std::print("{} ", i);
        }
    }

    // transform - get halves
    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        for (auto i : std::views::iota(1, 101) | std::views::filter(is_abundant) |
                          std::views::transform([](int const n) { return n / 2; }))
        {
            std::print("{} ", i);
        }
    }

    // take first N
    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        for (auto i : std::views::iota(1, 101) | std::views::filter(is_abundant) | std::views::take(5))
        {
            std::print("{} ", i);
        }
    }

    // take last N in reverse order
    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        for (auto i :
             std::views::iota(1, 101) | std::views::reverse | std::views::filter(is_abundant) | std::views::take(5))
        {
            std::print("{} ", i);
        }
    }

    // take last N in ascending order
    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;
        for (auto i : std::views::iota(1, 101) | std::views::reverse | std::views::filter(is_abundant) |
                          std::views::take(5) | std::views::reverse)
        {
            std::print("{} ", i);
        }
    }

    // all except for first N and last N
    {
        std::println("\n====================== using namespace n901 =============================");

        using namespace n901;

        std::println("trim N");

        for (auto i : std::views::iota(1, 101) | std::views::filter(is_abundant) | std::views::drop(5) |
                          std::views::reverse | std::views::drop(5) | std::views::reverse)
        {
            std::print("{} ", i);
        }
    }

    {
        std::println("\n====================== 1 ================================================");

        std::vector<int> v{1, 5, 3, 2, 4};
        std::sort(v.begin(), v.end());
    }

    {
        std::println("\n====================== 2 ================================================");

        std::vector<int> v{1, 5, 3, 2, 4};
        std::ranges::sort(v);
    }

    {
        std::println("\n====================== 3 ================================================");

        std::array<int, 5> a{1, 5, 3, 2, 4};
        auto               even = std::count_if(a.begin(), a.end(), [](int const n) { return n % 2 == 0; });
        assert(even == 2);
    }

    {
        std::println("\n====================== 4 ================================================");

        std::array<int, 5> a{1, 5, 3, 2, 4};
        auto               even = std::ranges::count_if(a, [](int const n) { return n % 2 == 0; });
        assert(even == 2);
    }

    {
        std::println("\n====================== 5 ================================================");

        std::println("square of even numbers (1):");

        std::vector<int> v{1, 5, 3, 2, 8, 7, 6, 4};

        // copy only the even elements
        std::vector<int> temp;
        std::copy_if(v.begin(), v.end(), std::back_inserter(temp), [](int const n) { return n % 2 == 0; });

        // sort the sequence
        std::sort(temp.begin(), temp.end(), [](int const a, int const b) { return a > b; });

        // remove the first two
        temp.erase(temp.begin() + temp.size() - 2, temp.end());

        // transform the elements
        std::transform(temp.begin(), temp.end(), temp.begin(), [](int const n) { return n * n; });

        // print each element
        std::for_each(temp.begin(), temp.end(), [](int const n) { std::println("{}", n); });
    }

    {
        std::println("\n====================== 6 ================================================");

        std::println("square of even numbers (2):");

        std::vector<int> v{1, 5, 3, 2, 8, 7, 6, 4};

        std::ranges::sort(v);
        auto r = v | std::ranges::views::filter([](int const n) { return n % 2 == 0; }) | std::ranges::views::drop(2) |
                 std::ranges::views::reverse | std::ranges::views::transform([](int const n) { return n * n; });

        std::ranges::for_each(r, [](int const n) { std::println("{}", n); });
    }

    {
        std::println("\n====================== 7 ================================================");

        std::println("square of even numbers (3):");

        namespace rv = std::ranges::views;

        std::vector<int> v{1, 5, 3, 2, 8, 7, 6, 4};

        std::ranges::sort(v);
        auto r = rv::transform(rv::reverse(rv::drop(rv::filter(v, [](int const n) { return n % 2 == 0; }), 2)),
                               [](int const n) { return n * n; });

        std::ranges::for_each(r, [](int const n) { std::println("{}", n); });
    }

    {
        std::println("\n====================== 8 ================================================");

        namespace rv = std::ranges::views;

        std::vector<int> v{1, 5, 3, 2, 4, 7, 6, 8};
        auto             r1 [[maybe_unused]] = rv::reverse(v);
        auto             r2 [[maybe_unused]] = v | rv::reverse;
    }

    {
        std::println("\n====================== 9 ================================================");

        namespace rv = std::ranges::views;

        std::vector<int> v{1, 5, 3, 2, 4, 7, 6, 8};

        auto r1 [[maybe_unused]] = rv::take(v, 2);
        auto r2 [[maybe_unused]] = rv::take(2)(v);
        auto r3 [[maybe_unused]] = v | rv::take(2);
    }

    {
        std::println("\n====================== 10 ===============================================");

        namespace rv = std::ranges::views;

        std::vector<int> v{1, 5, 3, 2, 4, 7, 6, 8};

        for (auto i : v | rv::reverse | rv::filter([](int const n) { return n % 2 == 1; }) | rv::take(2))
        {
            std::println("{}", i);
        }
    }

    {
        std::println("\n====================== 11 ===============================================");

        namespace rv = std::ranges::views;

        std::vector<int> v{1, 5, 3, 2, 4, 7, 16, 8};
        for (auto i : v | rv::take_while([](int const n) { return n < 10; }) |
                          rv::drop_while([](int const n) { return n % 2 == 1; }))
        {
            std::println("{}", i);
        }
    }

    {
        std::println("\n====================== 12 ===============================================");

        namespace rv = std::ranges::views;

        std::vector<std::tuple<int, double, std::string>> v = {{1, 1.1, "one"}, {2, 2.2, "two"}, {3, 3.3, "three"}};

        std::println("keys:");
        for (auto i : v | rv::keys)
        {
            std::println("{}", i);
        }

        std::println("values:");
        for (auto i : v | rv::values)
        {
            std::println("{}", i);
        }

        std::println("elements:");
        for (auto i : v | rv::elements<2>)
        {
            std::println("{}", i);
        }
    }

    {
        std::println("\n====================== 13 ===============================================");

        namespace rv = std::ranges::views;

        std::println("join:");

        std::vector<std::string> words{"a", "join", "example"};
        for (auto s : words | rv::join)
        {
            std::print("{} ", s);
        }
    }

    {
        std::println("\n====================== 14 ===============================================");

        namespace rv = std::ranges::views;

        std::println("join:");

        std::vector<std::vector<int>> v{{1, 2, 3}, {4}, {5, 6}};

        for (int const i : v | rv::join)
        {
            std::print("{} ", i);
        }

        // for(int const i : v | rv::join_with(0))
        //   std::println("{}",i << ' ';  // print 1 2 3 0 4 0 5 6
    }

    {
        std::println("\n====================== 15 ===============================================");

        namespace rv = std::ranges::views;
        std::string                text{"this is a demo!"};
        constexpr std::string_view delim{" "};
        for (auto const word : text | rv::split(delim))
        {
            std::println("{}", std::string_view(word.begin(), word.end()));
        }
    }

    /*
    {
       std::array<int, 4> a{ 1, 2, 3, 4 };
       std::vector<double> v{ 10.0, 20.0, 30.0 };

       auto z = rv::zip(a, v) // { {1, 10.0}, {2, 20.0}, {3, 30.0} }
    }

    {
       std::array<int, 4> a{ 1, 2, 3, 4 };
       std::vector<double> v{ 10.0, 20.0, 30.0 };

       auto z = rv::zip_transform(std::multiplies<double>(), a, v) // { {1, 10.0}, {2, 20.0}, {3, 30.0} }
    }
    */

    {
        std::println("\n====================== 16 ===============================================");

        std::vector<int> v{8, 5, 3, 2, 4, 7, 6, 1};
        auto             r = std::views::iota(1, 10);

        std::println("size(v) = {}", std::ranges::size(v));
        std::println("size(r) = {}", std::ranges::size(r));

        std::println("empty(v) = {}", std::ranges::empty(v));
        std::println("empty(r) = {}", std::ranges::empty(r));

        std::println("first(v) = {}", *std::ranges::begin(v));
        std::println("first(r) = {}", *std::ranges::begin(r));

        std::println("rbegin(v) = {}", *std::ranges::rbegin(v));
        std::println("rbegin(r) = {}", *std::ranges::rbegin(r));

        std::println("data(v) = {}", *std::ranges::data(v));
    }

    {
        std::println("\n====================== 17 ===============================================");

        for (auto i : std::ranges::iota_view(1, 10))
        {
            std::print("{} ", i);
        }

        for (auto i : std::views::iota(1, 10))
        {
            std::print("{} ", i);
        }
    }

    {
        std::println("\n====================== 18 ===============================================");

        constexpr std::ranges::empty_view<int> ev;
        static_assert(std::ranges::empty(ev));
        static_assert(std::ranges::size(ev) == 0);
        static_assert(std::ranges::data(ev) == nullptr);
    }

    {
        std::println("\n====================== 19 ===============================================");

        constexpr std::ranges::single_view<int> sv{42};
        static_assert(!std::ranges::empty(sv));
        static_assert(std::ranges::size(sv) == 1);
        static_assert(*std::ranges::data(sv) == 42);
    }

    {
        std::println("\n====================== 20 ===============================================");

        auto v1 = std::ranges::views::iota(1, 10);
        std::ranges::for_each(v1, [](int const n) { std::print("{} ", n); });

        auto v2 = std::ranges::views::iota(1) | std::ranges::views::take(9);
        std::ranges::for_each(v2, [](int const n) { std::print("{} ", n); });
    }

    {
        std::println("\n====================== 21 ===============================================");

        auto                text   = "19.99 7.50 49.19 20 12.34";
        auto                stream = std::istringstream{text};
        std::vector<double> prices;
        double              price;
        while (stream >> price)
        {
            prices.push_back(price);
        }

        auto total = std::accumulate(prices.begin(), prices.end(), 0.0);
        std::println("total: {}", total);
    }

    {
        std::println("\n====================== 22 ===============================================");

        auto                text   = "19.99 7.50 49.19 20 12.34";
        auto                stream = std::istringstream{text};
        std::vector<double> prices;
        for (double const price : std::ranges::istream_view<double>(stream))
        {
            prices.push_back(price);
        }
        auto total = std::accumulate(prices.begin(), prices.end(), 0.0);
        std::println("total: {}", total);
    }

    {
        std::println("\n====================== 23 ===============================================");

        auto                text   = "19.99 7.50 49.19 20 12.34";
        auto                stream = std::istringstream{text};
        std::vector<double> prices;
        std::ranges::for_each(std::ranges::istream_view<double>(stream),
                              [&prices](double const price) { prices.push_back(price); });
        auto total = std::accumulate(prices.begin(), prices.end(), 0.0);
        std::println("total: {}", total);
    }

    {
        std::println("\n====================== 24 ===============================================");

        auto l_odd = [](int const n) { return n % 2 == 1; };

        std::vector<int> v{1, 1, 2, 3, 5, 8, 13};
        std::vector<int> o;
        auto             e1 [[maybe_unused]] = std::copy_if(v.begin(), v.end(), std::back_inserter(o), l_odd);

        int  arr[]               = {1, 1, 2, 3, 5, 8, 13};
        auto e2 [[maybe_unused]] = std::copy_if(std::begin(arr), std::end(arr), std::back_inserter(o), l_odd);
    }

    {
        std::println("\n====================== 25 ===============================================");

        auto l_odd = [](int const n) { return n % 2 == 1; };

        std::vector<int> v{1, 1, 2, 3, 5, 8, 13};
        std::vector<int> o;
        auto             e1 [[maybe_unused]] = std::ranges::copy_if(v, std::back_inserter(o), l_odd);

        int  arr[]               = {1, 1, 2, 3, 5, 8, 13};
        auto e2 [[maybe_unused]] = std::ranges::copy_if(arr, std::back_inserter(o), l_odd);

        auto r [[maybe_unused]]  = std::ranges::views::iota(1, 10);
        auto e3 [[maybe_unused]] = std::ranges::copy_if(r, std::back_inserter(o), l_odd);
    }

    {
        std::println("\n====================== 26 ===============================================");

        namespace rv = std::ranges::views;

        std::vector<Item> items{{1, "pen", 5.49}, {2, "ruler", 3.99}, {3, "pensil case", 12.50}};

        std::vector<Item> copies;

        std::ranges::copy_if(items, std::back_inserter(copies), [](Item const &i) { return i.name[0] == 'p'; });

        copies.clear();

        std::ranges::copy_if(
            items, std::back_inserter(copies), [](std::string const &name) { return name[0] == 'p'; }, &Item::name);

        copies.clear();

        std::vector<std::string> names;
        std::ranges::copy_if(items | rv::transform(&Item::name), std::back_inserter(names),
                             [](std::string const &name) { return name[0] == 'p'; });
    }

    {
        std::println("\n====================== using namespace n902 =============================");

        using namespace n902;

        std::println("step(1)");
        for (auto i : std::views::iota(1, 10) | n902::views::step(1))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("step(2)");
        for (auto i : std::views::iota(1, 10) | n902::views::step(2))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("step(3)");
        for (auto i : std::views::iota(1, 10) | n902::views::step(3))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("step(4)");
        for (auto i : std::views::iota(1, 10) | n902::views::step(4))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("step(5)");
        for (auto i : std::views::iota(1, 10) | n902::views::step(5))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("step(2) | take(3)");
        for (auto i : std::views::iota(1, 10) | n902::views::step(2) | std::views::take(3))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("step(2) | take(3)");
        auto r = n902::views::step(std::views::iota(1, 10), 2);
        auto t = std::ranges::take_view(r, 3);
        for (auto i : t)
        {
            std::print("{} ", i);
        }
    }

    {
        std::println("\n====================== using namespace n902 =============================");

        using namespace n902;

        auto v  = std::views::iota(1, 10) | n902::views::step(1);
        auto it = v.begin();
        it++; // post-increment
        ++it; // pre-increment
    }

    {
        std::println("\n====================== using namespace n903 =============================");

        using namespace n903;

        std::println("replicate(2)");
        for (auto i : std::views::iota(1, 5) | n903::views::replicate(2))
        {
            std::print("{} ", i);
        }

        std::println();

        std::println("replicate(2)");
        for (auto i : std::views::iota(1, 5) | n903::views::replicate(2) | std::views::take(5))
        {
            std::print("{} ", i);
        }
    }
}
