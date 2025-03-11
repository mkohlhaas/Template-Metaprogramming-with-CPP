#include <any>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace n701
{
    // polymorphic (game) classes

    struct game_unit
    {
        virtual void attack() = 0;
    };

    struct knight : game_unit
    {
        void
        attack() override
        {
            std::println("knight draws sword");
        }
    };

    struct mage : game_unit
    {
        void
        attack() override
        {
            std::println("mage spells magic curse");
        }
    };

    struct knight_mage : game_unit
    {
        void
        attack() override
        {
            std::println("knight-mage draws magic sword");
        }
    };

    void
    fight(std::vector<game_unit *> const &units)
    {
        for (auto unit : units)
        {
            unit->attack();
        }
    }

    struct attack
    {
        int value;
    };

    struct defence
    {
        int value;
    };

    // Based on the operands for the +-operator, the compiler is selecting the appropriate overload.
    // This is called STATIC POLYMORPHISM. (All this happens at compile time.)

    attack
    operator+(attack const &a, int value)
    {
        return attack{a.value + value};
    }

    knight_mage
    operator+(knight const &, mage const &)
    {
        return knight_mage{};
    }

    // Functions can also be overloaded.

    void
    increment(attack &a)
    {
        a.value++;
    }

    void
    increment(defence &d)
    {
        d.value++;
    }

    // We could replace both functions with a template.
    template <typename T>
    void
    increment(T &t)
    {
        t.value++;
    }

    // Summary:
    // Overloaded functions and templates are the mechanisms to implement static polymorphism.

} // namespace n701

namespace n702
{
    // CRTP

    template <typename T>
    struct Base
    {
        void
        f()
        {
            static_cast<T *>(this)->do_f();
        }
    };

    struct Derived : public Base<Derived>
    {
        void
        do_f()
        {
            std::println("Derived::f()");
        }
    };

    template <typename T>
    void
    process(Base<T> &b)
    {
        b.f();
    }
} // namespace n702

namespace n703
{
    // [1] There is a base class template that defines the (static) interface.
    template <typename T>
    struct game_unit
    {
        // [3] Member functions of the base class call member functions of its type template parameter (which are the
        //     derived(!) classes).
        void
        attack()
        {
            static_cast<T *>(this)->do_attack();
        }
    };

    // [2] Derived classes are THEMSELVES(!) the template argument for the base class template.
    struct knight : game_unit<knight>
    {
        void
        do_attack()
        {
            std::println("draw sword");
        }
    };

    // [2] Derived classes are themselves the template argument for the base class template.
    struct mage : game_unit<mage>
    {
        void
        do_attack()
        {
            std::println("spell magic curse");
        }
    };

    // fight() must be a template function now.
    template <typename T>
    void
    fight(std::vector<game_unit<T> *> const &units)
    {
        for (auto unit : units)
        {
            unit->attack();
        }
    }
} // namespace n703

namespace n704
{
    // limiting number of instances of T to N
    template <typename T, size_t N>
    struct limited_instances
    {
        static std::atomic<size_t> count;

        limited_instances()
        {
            if (count >= N)
            {
                throw std::logic_error{"Too many instances"};
            }
            ++count;
        }

        ~limited_instances()
        {
            --count;
        }
    };

    // initialization of non-const static data member
    template <typename T, size_t N>
    std::atomic<size_t> limited_instances<T, N>::count = 0;

    struct excalibur : limited_instances<excalibur, 1>
    {
        // ...
    };

    struct book_of_magic : limited_instances<book_of_magic, 3>
    {
        // ...
    };
} // namespace n704

namespace n705
{
    template <typename T>
    struct movable_unit
    {
        void
        advance(size_t steps)
        {
            while (steps--)
            {
                static_cast<T *>(this)->step_forth();
            }
        }

        void
        retreat(size_t steps)
        {
            while (steps--)
            {
                static_cast<T *>(this)->step_back();
            }
        }
    };

    struct knight : movable_unit<knight>
    {
        void
        step_forth()
        {
            std::println("knight moves forward");
        }

        void
        step_back()
        {
            std::println("knight moves back");
        }
    };

    struct mage : movable_unit<mage>
    {
        void
        step_forth()
        {
            std::println("mage moves forward");
        }

        void
        step_back()
        {
            std::println("mage moves back");
        }
    };
} // namespace n705

namespace n706
{
    struct knight
    {
        void
        step_forth()
        {
            std::println("knight moves forward");
        }

        void
        step_back()
        {
            std::println("knight moves back");
        }
    };

    struct mage
    {
        void
        step_forth()
        {
            std::println("mage moves forward");
        }

        void
        step_back()
        {
            std::println("mage moves back");
        }
    };

    template <typename T>
    void
    advance(T &t, size_t steps)
    {
        while (steps--)
        {
            t.step_forth();
        }
    }

    template <typename T>
    void
    retreat(T &t, size_t steps)
    {
        while (steps--)
        {
            t.step_back();
        }
    }
} // namespace n706

namespace n707
{
    struct hero
    {
        hero(std::string_view n) : name(n)
        {
        }

        void
        ally_with(hero &u)
        {
            connections.insert(&u);
            u.connections.insert(this);
        }

      private:
        std::string      name;
        std::set<hero *> connections;

        friend std::ostream &operator<<(std::ostream &os, hero const &obj);
    };

    std::ostream &
    operator<<(std::ostream &os, hero const &obj)
    {
        for (hero *c : obj.connections)
        {
            os << obj.name << " --> [" << c->name << "]" << '\n';
        }

        return os;
    }

    struct hero_party : std::vector<hero>
    {
        // ...
    };
} // namespace n707

namespace n708
{
    template <typename T>
    struct base_unit
    {
        template <typename U>
        void ally_with(U &other);
    };

    // CRTP
    struct hero : base_unit<hero>
    {
        hero(std::string_view n) : name(n)
        {
        }

        // making hero into an iterable
        hero *
        begin()
        {
            return this;
        }

        hero *
        end()
        {
            return this + 1;
        }

      private:
        std::string      name;
        std::set<hero *> connections;

        template <typename U>
        friend struct base_unit;

        template <typename U>
        friend std::ostream &operator<<(std::ostream &os, base_unit<U> &object);
    };

    // CRTP (adding functionality to std::vector)
    struct hero_party : std::vector<hero>, base_unit<hero_party>
    {
    };

    template <typename T>
    template <typename U>
    void
    base_unit<T>::ally_with(U &other)
    {
        for (hero &from : *static_cast<T *>(this))
        {
            for (hero &to : other)
            {
                from.connections.insert(&to);
                to.connections.insert(&from);
            }
        }
    }

    template <typename T>
    std::ostream &
    operator<<(std::ostream &os, base_unit<T> &object)
    {
        for (hero &obj : *static_cast<T *>(&object))
        {
            for (hero *c : obj.connections)
            {
                os << obj.name << " --> [" << c->name << "]" << '\n';
            }
        }
        return os;
    }
} // namespace n708

namespace n709a
{
    struct building
    {
        // ...
    };
} // namespace n709a

namespace n709b
{
    struct building : std::enable_shared_from_this<building>
    {
        // ...
    };
} // namespace n709b

namespace n709c
{
    struct executor
    {
        void
        execute(std::function<void(void)> const &task)
        {
            threads.push_back(std::thread([task]() {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(250ms);

                task();
            }));
        }

        ~executor()
        {
            for (auto &t : threads)
            {
                t.join();
            }
        }

      private:
        std::vector<std::thread> threads;
    };

    struct building : std::enable_shared_from_this<building>
    {
        building()
        {
            std::println("building created");
        }

        ~building()
        {
            std::println("building destroyed");
        }

        void
        upgrade()
        {
            if (exec)
            {
                exec->execute([self = shared_from_this()]() { self->do_upgrade(); });
            }
        }

        void
        set_executor(executor *e)
        {
            exec = e;
        }

      private:
        void
        do_upgrade()
        {
            std::println("upgrading");
            operational = false;

            // using namespace std::chrono_literals;
            // std::this_thread::sleep_for(1000ms);

            operational = true;
            std::println("building is functional");
        }

        bool      operational = false;
        executor *exec        = nullptr;
    };
} // namespace n709c

namespace n710a
{
    struct hit_and_run
    {
        void
        fight()
        {
            std::println("hit once hard then run");
        }
    };

    struct last_man_standing
    {
        void
        fight()
        {
            std::println("duel until one falls");
        }
    };

    template <typename Strategy>
    struct knight : public Strategy
    {
        void
        attack()
        {
            std::println("draw sword");
            Strategy::fight();
        }
    };

    template <typename Strategy>
    struct mage : public Strategy
    {
        void
        attack()
        {
            std::println("spell magic curse");
            Strategy::fight();
        }
    };
} // namespace n710a

namespace n710b
{
    struct knight
    {
        void
        step_forth()
        {
            std::println("knight moves forward");
        }

        void
        step_back()
        {
            std::println("knight moves back");
        }
    };

    struct mage
    {
        void
        step_forth()
        {
            std::println("mage moves forward");
        }

        void
        step_back()
        {
            std::println("mage moves back");
        }
    };

    template <typename T>
    struct movable_unit : T
    {
        void
        advance(size_t steps)
        {
            while (steps--)
            {
                T::step_forth();
            }
        }

        void
        retreat(size_t steps)
        {
            while (steps--)
            {
                T::step_back();
            }
        }
    };
} // namespace n710b

namespace n710c
{
    struct aggressive_style
    {
        void
        fight()
        {
            std::println("attack! attack attack!");
        }
    };

    struct moderate_style
    {
        void
        fight()
        {
            std::println("attack then defend");
        }
    };

    template <typename T>
    struct lone_warrior : T
    {
        void
        fight()
        {
            std::println("fighting alone.");
            T::fight();
        }
    };

    template <typename T>
    struct team_warrior : T
    {
        void
        fight()
        {
            std::println("fighting with a team.");
            T::fight();
        }
    };

    struct game_unit
    {
        virtual void attack() = 0;
        virtual ~game_unit()  = default;
    };

    template <typename T>
    struct knight : T, game_unit
    {
        void
        attack()
        {
            std::println("draw sword.");
            T::fight();
        }
    };

    template <typename T>
    struct mage : T, game_unit
    {
        void
        attack()
        {
            std::println("spell magic curse.");
            T::fight();
        }
    };
} // namespace n710c

namespace n711a
{
    namespace details
    {
        template <typename Iter, typename Distance>
        void
        advance(Iter &it, Distance n, std::random_access_iterator_tag)
        {
            it += n;
        }

        template <typename Iter, typename Distance>
        void
        advance(Iter &it, Distance n, std::bidirectional_iterator_tag)
        {
            if (n > 0)
            {
                while (n--)
                {
                    ++it;
                }
            }
            else
            {
                while (n++)
                {
                    --it;
                }
            }
        }

        template <typename Iter, typename Distance>
        void
        advance(Iter &it, Distance n, std::input_iterator_tag)
        {
            while (n--)
            {
                ++it;
            }
        }
    } // namespace details

    template <typename Iter, typename Distance>
    void
    advance(Iter &it, Distance n)
    {
        details::advance(it, n, typename std::iterator_traits<Iter>::iterator_category{});
    }
} // namespace n711a

namespace n711b
{
    template <std::random_access_iterator Iter, typename Distance>
    void
    advance(Iter &it, Distance n)
    {
        it += n;
    }

    template <std::bidirectional_iterator Iter, typename Distance>
    void
    advance(Iter &it, Distance n)
    {
        if (n > 0)
        {
            while (n--)
            {
                ++it;
            }
        }
        else
        {
            while (n++)
            {
                --it;
            }
        }
    }

    template <std::input_iterator Iter, typename Distance>
    void
    advance(Iter &it, Distance n)
    {
        while (n--)
        {
            ++it;
        }
    }
} // namespace n711b

namespace n712a
{
    struct knight
    {
        void
        attack()
        {
            std::println("draw sword");
        }
    };

    struct mage
    {
        void
        attack()
        {
            std::println("spell magic curse");
        }
    };

    struct game_unit
    {
        virtual void attack() = 0;
        virtual ~game_unit()  = default;
    };

    struct knight_unit : game_unit
    {
        knight_unit(knight &u) : k(u)
        {
        }
        void
        attack() override
        {
            k.attack();
        }

      private:
        knight &k;
    };

    struct mage_unit : game_unit
    {
        mage_unit(mage &u) : m(u)
        {
        }
        void
        attack() override
        {
            m.attack();
        }

      private:
        mage &m;
    };

    void
    fight(std::vector<game_unit *> const &units)
    {
        for (auto u : units)
        {
            u->attack();
        }
    }
} // namespace n712a

namespace n712b
{
    struct knight
    {
        void
        attack()
        {
            std::println("draw sword");
        }
    };

    struct mage
    {
        void
        attack()
        {
            std::println("spell magic curse");
        }
    };

    struct game_unit
    {
        virtual void attack() = 0;
        virtual ~game_unit()  = default;
    };

    template <typename T>
    struct game_unit_wrapper : public game_unit
    {
        game_unit_wrapper(T &unit) : t(unit)
        {
        }

        void
        attack() override
        {
            t.attack();
        }

      private:
        T &t;
    };

    void
    fight(std::vector<game_unit *> const &units)
    {
        for (auto u : units)
        {
            u->attack();
        }
    }
} // namespace n712b

namespace n712c
{
    struct knight
    {
        void
        attack()
        {
            std::println("draw sword");
        }
    };

    struct mage
    {
        void
        attack()
        {
            std::println("spell magic curse");
        }
    };

    struct game
    {
        struct game_unit
        {
            virtual void attack() = 0;
            virtual ~game_unit()  = default;
        };

        template <typename T>
        struct game_unit_wrapper : public game_unit
        {
            game_unit_wrapper(T &unit) : t(unit)
            {
            }

            void
            attack() override
            {
                t.attack();
            }

          private:
            T &t;
        };

        template <typename T>
        void
        addUnit(T &unit)
        {
            units.push_back(std::make_unique<game_unit_wrapper<T>>(unit));
        }

        void
        fight()
        {
            for (auto &u : units)
            {
                u->attack();
            }
        }

      private:
        std::vector<std::unique_ptr<game_unit>> units;
    };
} // namespace n712c

namespace n712d
{
    struct knight
    {
        void
        attack()
        {
            std::println("draw sword");
        }
    };

    struct mage
    {
        void
        attack()
        {
            std::println("spell magic curse");
        }
    };

    struct unit
    {
        template <typename T>
        unit(T &&obj) : unit_(std::make_shared<unit_model<T>>(std::forward<T>(obj)))
        {
        }

        void
        attack()
        {
            unit_->attack();
        }

        struct unit_concept
        {
            virtual void attack()   = 0;
            virtual ~unit_concept() = default;
        };

        template <typename T>
        struct unit_model : public unit_concept
        {
            unit_model(T &unit) : t(unit)
            {
            }

            void
            attack() override
            {
                t.attack();
            }

          private:
            T &t;
        };

      private:
        std::shared_ptr<unit_concept> unit_;
    };

    void
    fight(std::vector<unit> &units)
    {
        for (auto &u : units)
        {
            u.attack();
        }
    }
} // namespace n712d

namespace n712e
{
    struct knight
    {
        void
        attack()
        {
            std::println("draw sword");
        }
    };

    struct mage
    {
        void
        attack()
        {
            std::println("spell magic curse");
        }
    };

    void
    fight_knight(void *k)
    {
        reinterpret_cast<knight *>(k)->attack();
    }

    void
    fight_mage(void *m)
    {
        reinterpret_cast<mage *>(m)->attack();
    }

    using fight_fn = void (*)(void *);

    void
    fight(std::vector<std::pair<void *, fight_fn>> const &units)
    {
        for (auto &u : units)
        {
            u.second(u.first);
        }
    }
} // namespace n712e

// namespace n713
// {
//     class async_bool
//     {
//         std::function<bool()> check;
//
//       public:
//         async_bool() = delete;
//         // async_bool(std::function<bool()> checkIt) : check(checkIt)
//         // {
//         // }
//
//         async_bool(bool val) : check([val]() { return val; })
//         {
//         }
//
//         static async_bool
//         yes()
//         {
//             return async_bool{static_cast<bool>([]() { return true; })};
//         }
//
//         static async_bool
//         no()
//         {
//             return async_bool{static_cast<bool>([]() { return false; })};
//         }
//
//         bool
//         operator&&(bool fore) const
//         {
//             return fore && check();
//         }
//         bool
//         operator!() const
//         {
//             return !check();
//         }
//         operator bool() const
//         {
//             return check();
//         }
//     };
// } // namespace n713

namespace n714
{
    template <typename... Ts>
    struct typelist
    {
    };

    struct empty_type
    {
    };

    namespace detail
    {
        template <typename TL>
        struct length;

        template <template <typename...> typename TL, typename... Ts>
        struct length<TL<Ts...>>
        {
            using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
        };

        template <typename TL>
        struct front_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct front_type<TL<T, Ts...>>
        {
            using type = T;
        };

        template <template <typename...> typename TL>
        struct front_type<TL<>>
        {
            using type = empty_type;
        };

        template <typename TL>
        struct back_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct back_type<TL<T, Ts...>>
        {
            using type = back_type<TL<Ts...>>::type;
        };

        template <template <typename...> typename TL, typename T>
        struct back_type<TL<T>>
        {
            using type = T;
        };

        template <template <typename...> typename TL>
        struct back_type<TL<>>
        {
            using type = empty_type;
        };

        template <typename TL, typename T>
        struct push_back_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct push_back_type<TL<Ts...>, T>
        {
            using type = TL<Ts..., T>;
        };

        template <typename TL, typename T>
        struct push_front_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct push_front_type<TL<Ts...>, T>
        {
            using type = TL<T, Ts...>;
        };

        template <typename TL>
        struct pop_front_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct pop_front_type<TL<T, Ts...>>
        {
            using type = TL<Ts...>;
        };

        template <template <typename...> typename TL>
        struct pop_front_type<TL<>>
        {
            using type = TL<>;
        };

        template <std::ptrdiff_t N, typename R, typename TL>
        struct pop_back_type;

        template <std::ptrdiff_t N, typename... Ts, typename U, typename... Us>
        struct pop_back_type<N, typelist<Ts...>, typelist<U, Us...>>
        {
            using type = typename pop_back_type<N - 1, typelist<Ts..., U>, typelist<Us...>>::type;
        };

        template <typename... Ts, typename... Us>
        struct pop_back_type<0, typelist<Ts...>, typelist<Us...>>
        {
            using type = typelist<Ts...>;
        };

        template <typename... Ts, typename U, typename... Us>
        struct pop_back_type<0, typelist<Ts...>, typelist<U, Us...>>
        {
            using type = typelist<Ts...>;
        };

        template <>
        struct pop_back_type<-1, typelist<>, typelist<>>
        {
            using type = typelist<>;
        };

        template <std::size_t I, std::size_t N, typename TL>
        struct at_type;

        template <std::size_t I, std::size_t N, template <typename...> typename TL, typename T, typename... Ts>
        struct at_type<I, N, TL<T, Ts...>>
        {
            using type = std::conditional_t<I == N, T, typename at_type<I, N + 1, TL<Ts...>>::type>;
        };

        template <std::size_t I, std::size_t N>
        struct at_type<I, N, typelist<>>
        {
            using type = empty_type;
        };
    } // namespace detail

    template <typename TL>
    using length_t = typename detail::length<TL>::type;

    template <typename TL>
    constexpr std::size_t length_v = length_t<TL>::value;

    static_assert(length_t<typelist<int, double, char>>::value == 3);
    static_assert(length_v<typelist<int, double, char>> == 3);
    static_assert(length_v<typelist<int, double>> == 2);
    static_assert(length_v<typelist<int>> == 1);

    template <typename TL>
    using front_t = typename detail::front_type<TL>::type;

    static_assert(std::is_same_v<front_t<typelist<>>, empty_type>);
    static_assert(std::is_same_v<front_t<typelist<int>>, int>);
    static_assert(std::is_same_v<front_t<typelist<int, double, char>>, int>);

    template <typename TL>
    using back_t = typename detail::back_type<TL>::type;

    static_assert(std::is_same_v<back_t<typelist<>>, empty_type>);
    static_assert(std::is_same_v<back_t<typelist<int>>, int>);
    static_assert(std::is_same_v<back_t<typelist<int, double, char>>, char>);

    template <typename TL, typename T>
    using push_back_t = typename detail::push_back_type<TL, T>::type;

    static_assert(std::is_same_v<push_back_t<typelist<>, int>, typelist<int>>);
    static_assert(std::is_same_v<push_back_t<typelist<char>, int>, typelist<char, int>>);
    static_assert(std::is_same_v<push_back_t<typelist<double, char>, int>, typelist<double, char, int>>);

    template <typename TL, typename T>
    using push_front_t = typename detail::push_front_type<TL, T>::type;

    static_assert(std::is_same_v<push_front_t<typelist<>, int>, typelist<int>>);
    static_assert(std::is_same_v<push_front_t<typelist<char>, int>, typelist<int, char>>);
    static_assert(std::is_same_v<push_front_t<typelist<double, char>, int>, typelist<int, double, char>>);

    template <typename TL>
    using pop_front_t = typename detail::pop_front_type<TL>::type;

    static_assert(std::is_same_v<pop_front_t<typelist<>>, typelist<>>);
    static_assert(std::is_same_v<pop_front_t<typelist<char>>, typelist<>>);
    static_assert(std::is_same_v<pop_front_t<typelist<double, char>>, typelist<char>>);

    template <typename TL>
    using pop_back_t =
        typename detail::pop_back_type<static_cast<std::ptrdiff_t>(length_v<TL>) - 1, typelist<>, TL>::type;

    static_assert(std::is_same_v<pop_back_t<typelist<>>, typelist<>>);
    static_assert(std::is_same_v<pop_back_t<typelist<double>>, typelist<>>);
    static_assert(std::is_same_v<pop_back_t<typelist<double, char>>, typelist<double>>);
    static_assert(std::is_same_v<pop_back_t<typelist<double, char, int>>, typelist<double, char>>);

    template <std::size_t I, typename TL>
    using at_t = typename detail::at_type<I, 0, TL>::type;

    static_assert(std::is_same_v<at_t<0, typelist<>>, empty_type>);
    static_assert(std::is_same_v<at_t<0, typelist<int>>, int>);
    static_assert(std::is_same_v<at_t<0, typelist<int, char>>, int>);

    static_assert(std::is_same_v<at_t<1, typelist<>>, empty_type>);
    static_assert(std::is_same_v<at_t<1, typelist<int>>, empty_type>);
    static_assert(std::is_same_v<at_t<1, typelist<int, char>>, char>);

    static_assert(std::is_same_v<at_t<2, typelist<>>, empty_type>);
    static_assert(std::is_same_v<at_t<2, typelist<int>>, empty_type>);
    static_assert(std::is_same_v<at_t<2, typelist<int, char>>, empty_type>);
} // namespace n714

namespace n715
{
    struct game_unit
    {
        int attack;
        int defense;
    };

    struct upgrade_defense
    {
        void
        operator()(game_unit &u)
        {
            u.defense = static_cast<int>(u.defense * 1.2);
        }
    };

    struct upgrade_attack
    {
        void
        operator()(game_unit &u)
        {
            u.attack += 2;
        }
    };

    using namespace n714;

    template <typename TL>
    struct apply_functors
    {
      private:
        template <size_t I>
        static void
        apply(game_unit &unit)
        {
            using F = at_t<I, TL>;
            std::invoke(F{}, unit);
        }

        template <size_t... I>
        static void
        apply_all(game_unit &unit, std::index_sequence<I...>)
        {
            (apply<I>(unit), ...);
        }

      public:
        void
        operator()(game_unit &unit) const
        {
            apply_all(unit, std::make_index_sequence<length_v<TL>>{});
        }
    };

    void
    upgrade_unit(game_unit &unit)
    {
        using upgrade_types = typelist<upgrade_defense, upgrade_attack>;
        apply_functors<upgrade_types>{}(unit);
    }

    template <typename... Ts>
    struct transformer
    {
        using input_types  = typelist<Ts...>;
        using output_types = typelist<std::add_const_t<Ts>...>;
    };

    static_assert(std::is_same_v<transformer<int, double>::output_types, typelist<int const, double const>>);
} // namespace n715

namespace n716
{
    template <typename T>
    struct vector
    {
        vector(std::size_t const n) : data_(n)
        {
        }

        vector(std::initializer_list<T> &&l) : data_(l)
        {
        }

        std::size_t
        size() const noexcept
        {
            return data_.size();
        }

        T const &
        operator[](const std::size_t i) const
        {
            return data_[i];
        }

        T &
        operator[](const std::size_t i)
        {
            return data_[i];
        }

      private:
        std::vector<T> data_;
    };

    template <typename T, typename U>
    auto
    operator+(vector<T> const &a, vector<U> const &b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());
        vector<result_type> result(a.size());
        for (std::size_t i = 0; i < a.size(); ++i)
        {
            result[i] = a[i] + b[i];
        }
        return result;
    }

    template <typename T, typename U>
    auto
    operator*(vector<T> const &a, vector<U> const &b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());
        vector<result_type> result(a.size());
        for (std::size_t i = 0; i < a.size(); ++i)
        {
            result[i] = a[i] * b[i];
        }
        return result;
    }

    template <typename T, typename S>
    auto
    operator*(S const &s, vector<T> const &v)
    {
        using result_type = decltype(std::declval<T>() + std::declval<S>());

        vector<result_type> result(v.size());
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            result[i] = s * v[i];
        }

        return result;
    }
} // namespace n716

namespace n717
{
    template <typename T, typename C = std::vector<T>>
    struct vector
    {
        vector() = default;

        vector(std::size_t const n) : data_(n)
        {
        }

        vector(std::initializer_list<T> &&l) : data_(l)
        {
        }

        vector(C const &other) : data_(other)
        {
        }

        template <typename U, typename X>
        vector(vector<U, X> const &other) : data_(other.size())
        {
            for (std::size_t i = 0; i < other.size(); ++i)
            {
                data_[i] = static_cast<T>(other[i]);
            }
        }

        template <typename U, typename X>
        vector &
        operator=(vector<U, X> const &other)
        {
            data_.resize(other.size());
            for (std::size_t i = 0; i < other.size(); ++i)
            {
                data_[i] = static_cast<T>(other[i]);
            }

            return *this;
        }

        std::size_t
        size() const noexcept
        {
            return data_.size();
        }

        T
        operator[](const std::size_t i) const
        {
            return data_[i];
        }

        T &
        operator[](const std::size_t i)
        {
            return data_[i];
        }

        C &
        data() noexcept
        {
            return data_;
        }

        C const &
        data() const noexcept
        {
            return data_;
        }

      private:
        C data_;
    };

    template <typename L, typename R>
    struct vector_add
    {
        vector_add(L const &a, R const &b) : lhv(a), rhv(b)
        {
        }

        auto
        operator[](std::size_t const i) const
        {
            return lhv[i] + rhv[i];
        }

        std::size_t
        size() const noexcept
        {
            return lhv.size();
        }

      private:
        L const &lhv;
        R const &rhv;
    };

    template <typename L, typename R>
    struct vector_mul
    {
        vector_mul(L const &a, R const &b) : lhv(a), rhv(b)
        {
        }

        auto
        operator[](std::size_t const i) const
        {
            return lhv[i] * rhv[i];
        }

        std::size_t
        size() const noexcept
        {
            return lhv.size();
        }

      private:
        L const &lhv;
        R const &rhv;
    };

    template <typename S, typename R>
    struct vector_scalar_mul
    {
        vector_scalar_mul(S const &s, R const &b) : scalar(s), rhv(b)
        {
        }

        auto
        operator[](std::size_t const i) const
        {
            return scalar * rhv[i];
        }

        std::size_t
        size() const noexcept
        {
            return rhv.size();
        }

      private:
        S const &scalar;
        R const &rhv;
    };

    template <typename T, typename L, typename U, typename R>
    auto
    operator+(vector<T, L> const &a, vector<U, R> const &b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());

        return vector<result_type, vector_add<L, R>>(vector_add<L, R>(a.data(), b.data()));
    }

    template <typename T, typename L, typename U, typename R>
    auto
    operator*(vector<T, L> const &a, vector<U, R> const &b)
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());

        return vector<result_type, vector_mul<L, R>>(vector_mul<L, R>(a.data(), b.data()));
    }

    template <typename T, typename S, typename E>
    auto
    operator*(S const &a, vector<T, E> const &v)
    {
        using result_type = decltype(std::declval<T>() + std::declval<S>());

        return vector<result_type, vector_scalar_mul<S, E>>(vector_scalar_mul<S, E>(a, v.data()));
    }
} // namespace n717

int
main()
{
    {
        std::println("\n====================== using namespace n701 =============================");

        // - DYNAMIC POLYMORPHISM occurs at RUNTIME      with the help of INTERFACES           and VIRTUAL FUNCTIONS.
        // - STATIC  POLYMORPHISM occurs at COMPILE-TIME with the help of OVERLOADED FUNCTIONS and TEMPLATES.

        using namespace n701;

        knight k;
        mage   m;
        fight({&k, &m});

        knight_mage km = k + m;
        km.attack();

        attack a{42};
        a = a + 2;

        defence d{50};
        increment(a);
        increment(d);
    }

    {
        std::println("\n====================== using namespace n702 =============================");

        using namespace n702;

        Derived d;
        process(d);
    }

    {
        std::println("\n====================== using namespace n703 =============================");

        // Can we get the benefits of dynamic polymorphism at compile time? Yes!
        // -> Curiously Recurring Template Pattern (CRTP)

        using namespace n703;

        knight k;
        mage   m;

        fight<knight>({&k}); // draw sword
        fight<mage>({&m});   // spell magic curse
        // fight({&k, &m});  // error
    }

    {
        std::println("\n====================== using namespace n704 =============================");

        // Limiting the object count with CRTP

        using namespace n704;

        try
        {
            excalibur e1;
            excalibur e2; // will throw an exception
        }
        catch (std::exception &e)
        {
            std::println("{}", e.what());
        }

        try
        {
            book_of_magic b1;
            book_of_magic b2;
            book_of_magic b3;
            book_of_magic b4; // will throw an exception
        }
        catch (std::exception &e)
        {
            std::println("{}", e.what());
        }
    }

    {
        std::println("\n====================== using namespace n705 =============================");

        // Adding functionality with CRTP

        using namespace n705;

        knight k;
        k.advance(3);
        k.retreat(2);

        mage m;
        m.advance(5);
        m.retreat(3);
    }

    {
        std::println("\n====================== using namespace n706 =============================");

        // Differnt implementation with function templates.

        using namespace n706;

        knight k;
        advance(k, 3);
        retreat(k, 2);

        mage m;
        advance(m, 5);
        retreat(m, 3);
    }

    {
        std::println("\n====================== using namespace n707 =============================");

        using namespace n707;

        hero h1("Arthur");
        hero h2("Sir Lancelot");
        hero h3("Sir Gawain");

        h1.ally_with(h2);
        h2.ally_with(h3);

        std::cout << h1 << '\n';
        std::cout << h2 << '\n';
        std::cout << h3 << '\n';
    }

    {
        std::println("\n====================== using namespace n708 =============================");

        // Implementing the composite design pattern
        // Composite pattern enables to compose objects into larger structures and treat both
        // individual objects and compositions uniformly.

        using namespace n708;

        hero h1("Arthur");
        hero h2("Sir Lancelot");

        hero_party p1;
        hero_party p2;

        p1.emplace_back("Bors");
        p2.emplace_back("Cador");
        p2.emplace_back("Constantine");

        // all combinations possible (this was the goal) treating heros and hero_parties the same
        h1.ally_with(h2);
        h1.ally_with(p1);
        p1.ally_with(h2);
        p1.ally_with(p2);

        std::cout << h1 << '\n';
        std::cout << h2 << '\n';
        std::cout << p1 << '\n';
        std::cout << p2 << '\n';
    }

    {
        std::println("\n====================== using namespace n709a ============================");

        // The CRTP in the standard library

        using namespace n709a;

        building *b = new building();

        std::shared_ptr<building> p1{b};
        // std::shared_ptr<building> p2{b}; // runtime error: free(): double free detected
    }

    {
        std::println("\n====================== using namespace n709b ============================");

        using namespace n709b;

        building *b = new building();

        std::shared_ptr<building> p1{b};
        std::shared_ptr<building> p2{b->shared_from_this()}; // OK
    }

    {
        std::println("\n====================== using namespace n709c ============================");

        using namespace n709c;

        executor                  e;
        std::shared_ptr<building> b = std::make_shared<building>();
        b->set_executor(&e);
        b->upgrade();

        std::println("main finished");
    }

    // {
    //     std::println("\n====================== using namespace n710a ============================");
    //
    //     using namespace n710a;
    //
    //     knight<last_man_standing> k;
    //     mage<hit_and_run>         m;
    //
    //     k.attack();
    //     m.attack();
    // }

    // {
    //     std::println("\n====================== using namespace n710b ============================");
    //
    //     using namespace n710b;
    //
    //     movable_unit<knight> k;
    //     k.advance(3);
    //     k.retreat(2);
    //
    //     movable_unit<mage> m;
    //     m.advance(5);
    //     m.retreat(3);
    // }

    // {
    //     std::println("\n====================== using namespace n710c ============================");
    //
    //     using namespace n710c;
    //
    //     std::vector<std::unique_ptr<game_unit>> units;
    //
    //     units.emplace_back(new knight<aggressive_style>());
    //     units.emplace_back(new knight<moderate_style>());
    //     units.emplace_back(new mage<aggressive_style>());
    //     units.emplace_back(new mage<moderate_style>());
    //     units.emplace_back(new knight<lone_warrior<aggressive_style>>());
    //     units.emplace_back(new knight<lone_warrior<moderate_style>>());
    //     units.emplace_back(new knight<team_warrior<aggressive_style>>());
    //     units.emplace_back(new knight<team_warrior<moderate_style>>());
    //     units.emplace_back(new mage<lone_warrior<aggressive_style>>());
    //     units.emplace_back(new mage<lone_warrior<moderate_style>>());
    //     units.emplace_back(new mage<team_warrior<aggressive_style>>());
    //     units.emplace_back(new mage<team_warrior<moderate_style>>());
    //
    //     for (auto &u : units)
    //     {
    //         u->attack();
    //     }
    // }

    // {
    //     std::println("\n=========================================================================");
    //
    //     std::vector<int> v{1, 2, 3, 4, 5};
    //     auto             sv = std::begin(v);
    //     n711a::advance(sv, 2);
    //
    //     std::list<int> l{1, 2, 3, 4, 5};
    //     auto           sl = std::begin(l);
    //     n711a::advance(sl, 2);
    // }

    // {
    //     std::println("\n=========================================================================");
    //
    //     std::vector<int> v{1, 2, 3, 4, 5};
    //     auto             sv = std::begin(v);
    //     n711b::advance(sv, 2);
    //
    //     std::list<int> l{1, 2, 3, 4, 5};
    //     auto           sl = std::begin(l);
    //     n711b::advance(sl, 2);
    // }

    // {
    //     std::println("\n====================== using namespace n712a ============================");
    //
    //     using namespace n712a;
    //
    //     knight k;
    //     mage   m;
    //
    //     knight_unit ku{k};
    //     mage_unit   mu{m};
    //
    //     std::vector<game_unit *> v{&ku, &mu};
    //     fight(v);
    // }

    // {
    //     std::println("\n====================== using namespace n712b ============================");
    //
    //     using namespace n712b;
    //
    //     knight k;
    //     mage   m;
    //
    //     game_unit_wrapper ku{k};
    //     game_unit_wrapper mu{m};
    //
    //     std::vector<game_unit *> v{&ku, &mu};
    //     fight(v);
    // }

    // {
    //     std::println("\n====================== using namespace n712c ============================");
    //
    //     using namespace n712c;
    //
    //     knight k;
    //     mage   m;
    //
    //     game g;
    //     g.addUnit(k);
    //     g.addUnit(m);
    //
    //     g.fight();
    // }

    // {
    //     std::println("\n====================== using namespace n712d ============================");
    //
    //     using namespace n712d;
    //
    //     knight k;
    //     mage   m;
    //
    //     std::vector<unit> v{unit(k), unit(m)};
    //
    //     fight(v);
    // }

    // {
    //     std::println("\n====================== using namespace n712e ============================");
    //
    //     using namespace n712e;
    //
    //     knight k;
    //     mage   m;
    //
    //     std::vector<std::pair<void *, fight_fn>> units{
    //         {&k, &fight_knight},
    //         {&m, &fight_mage},
    //     };
    //
    //     fight(units);
    // }

    // {
    //     std::println("\n====================== using namespace n713 =============================");
    //
    //     using namespace n713;
    //
    //     async_bool b1{false};
    //     async_bool b2{true};
    //     async_bool b3{static_cast<bool>(/* [] */() {
    //         std::cout << "Y/N? ";
    //         char c;
    //         std::cin >> c;
    //         return c == 'Y' || c == 'y';
    //     })};
    //
    //     if (b1)
    //     {
    //         std::println("b1 is true");
    //     }
    //     if (b2)
    //     {
    //         std::println("b2 is true");
    //     }
    //     if (b3)
    //     {
    //         std::println("b3 is true");
    //     }
    // }

    // {
    //     std::println("\n====================== using namespace n712d ============================");
    //
    //     using namespace n712d;
    //
    //     std::any u;
    //
    //     u = knight{};
    //     if (u.has_value())
    //     {
    //         std::any_cast<knight>(u).attack();
    //     }
    //
    //     u = mage{};
    //     if (u.has_value())
    //     {
    //         std::any_cast<mage>(u).attack();
    //     }
    // }

    // {
    //     std::println("\n=========================================================================");
    //
    //     n716::vector<int> v1{1, 2, 3};
    //     n716::vector<int> v2{4, 5, 6};
    //     double            a{1.5};
    //
    //     n716::vector<double> v3 = v1 + a * v2;       // {7.0, 9.5, 12.0}
    //     n716::vector<int>    v4 = v1 * v2 + v1 + v2; // {9, 17, 27}
    // }

    // {
    //     std::println("\n=========================================================================");
    //
    //     n717::vector<int> v1{1, 2, 3};
    //     n717::vector<int> v2{4, 5, 6};
    //     double            a{1.5};
    //
    //     n717::vector<double> v3 = v1 + a * v2;        // {7.0, 9.5, 12.0}
    //
    //     int c;
    //     std::cin >> c;
    //     n717::vector<double> v31 = v1 + c * v2;       // {7.0, 9.5, 12.0}
    //     n717::vector<int>    v4  = v1 * v2 + v1 + v2; // {9, 17, 27}
    // }

    // {
    //     std::println("\n=========================================================================");
    //
    //     namespace rv = std::ranges::views;
    //     std::vector<int> v1{1, 2, 3};
    //     std::vector<int> v2{4, 5, 6};
    //     double           a{1.5};
    //
    //     auto sv2 = v2 | rv::transform([&a](int val) { return a * val; });
    //     // auto v3 = rv::zip_view(std::plus<>{}, v1, sv2);
    //
    //     for (auto e : sv2)
    //     {
    //         std::println("{}", e);
    //     }
    // }

    // {
    //     std::println("\n====================== using namespace n715 =============================");
    //
    //     using namespace n715;
    //
    //     game_unit u{100, 50};
    //     std::println("{},{}", u.attack, u.defense);
    //
    //     upgrade_unit(u);
    //     std::println("{},{}", u.attack, u.defense);
    // }
}
