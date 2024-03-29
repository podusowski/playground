#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "packed_vector.hpp"

TEST_CASE("some basics on empty vector")
{
    packed_vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.begin() == v.end());
}

TEST_CASE("some basics on one element")
{
    packed_vector<int> v{1};
    REQUIRE(v.begin() != v.end());
    REQUIRE(*std::begin(v) == 1);
}

void check_three_elements(packed_vector<int>& v)
{
    REQUIRE(v.size() == 3);
    auto begin = v.begin();
    REQUIRE(*begin == 1);
    REQUIRE(*std::next(begin) == 2);
    REQUIRE(*std::next(begin, 2) == 3);
    REQUIRE(std::next(begin, 3) == v.end());
}

TEST_CASE("some basics on few elements")
{
    packed_vector<int> v{1, 2, 3};
    check_three_elements(v);
}

TEST_CASE("elements inserted from pair of iterators")
{
    std::vector<int> source{1, 2, 3};
    packed_vector<int> v(source.begin(), source.end());
    check_three_elements(v);
}

TEST_CASE("dynamic insert")
{
    packed_vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.begin() == v.end());

    v.push_back(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v.begin() != v.end());
    REQUIRE(*std::begin(v) == 1);

    v.push_back(2);
    REQUIRE(v.size() == 2);
    REQUIRE(*std::begin(v) == 1);
    REQUIRE(*std::next(v.begin()) == 2);

    v.push_back(3);
    REQUIRE(v.size() == 3);

    v.push_back(4);
    REQUIRE(v.size() == 4);
}

struct Foo
{
    ~Foo()
    {
        destructors++;
    }

    static std::size_t destructors;
};

std::size_t Foo::destructors = 0;

TEST_CASE("destructors are called upon exit")
{
    Foo::destructors = 0;

    {
        packed_vector<Foo> v{Foo{}, Foo{}};
        REQUIRE(v.size() == 2);
        REQUIRE(Foo::destructors == 0);
    }

    REQUIRE(Foo::destructors == 2);
}
