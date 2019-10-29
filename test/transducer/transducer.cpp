#include <zug/reducing/last.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/transducer.hpp>

#include <catch2/catch.hpp>

using namespace zug;

TEST_CASE("transducer: comp composition")
{
    auto add_one = transducer<int>(map([](auto x) { return x + 1; }));

    auto xform  = comp(add_one, add_one);
    auto result = transduce(xform, last, 0, std::vector<int>{10});

    CHECK(result == 12);
}

TEST_CASE("transducer: operator| composition")
{
    auto add_one = transducer<int>(map([](auto x) { return x + 1; }));

    auto xform  = add_one | add_one;
    auto result = transduce(xform, last, 0, std::vector<int>{10});

    CHECK(result == 12);
}

TEST_CASE("transducer: operator| composition with non type-erased transducer")
{
    auto add_one = transducer<int>(map([](auto x) { return x + 1; }));
    auto add_two = map([](auto x) { return x + 2; });

    SECTION("type-erased | non type-erased")
    {
        auto xform  = add_one | add_two;
        auto result = transduce(xform, last, 0, std::vector<int>{10});

        CHECK(result == 13);
    }

    SECTION("non type-erased | type-erased")
    {
        auto xform  = add_two | add_one;
        auto result = transduce(xform, last, 0, std::vector<int>{10});

        CHECK(result == 13);
    }
}

TEST_CASE(
    "transducer: operator| composition with non-composable type transducer")
{
    auto add_five = [](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            return step(
                ZUG_FWD(s),
                compat::invoke([](auto x) { return x + 5; }, ZUG_FWD(is)...));
        };
    };
    auto add_one = transducer<int>(map([](auto x) { return x + 1; }));

    SECTION("transducer | non-composable")
    {
        auto xform  = add_one | add_five;
        auto result = transduce(xform, last, 0, std::vector<int>{660});

        CHECK(result == 666);
    }

    SECTION("non-composable | transducer")
    {
        auto xform  = add_five | add_one;
        auto result = transduce(xform, last, 0, std::vector<int>{660});

        CHECK(result == 666);
    }
}
