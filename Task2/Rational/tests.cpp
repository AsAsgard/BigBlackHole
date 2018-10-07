#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "crational.h"

TEST_CASE("Privedenie")
{
    cRational x(4,5);
    CHECK(x.GetNumer() == 4);
    CHECK(x.GetDenom() == 5);
    x.SetRational(8,7);
    CHECK(x.GetNumer() == 8);
    CHECK(x.GetDenom() == 7);
    x.SetRational(4,2);
    CHECK(x.GetNumer() == 2);
    CHECK(x.GetDenom() == 1);
    x.SetRational(-2,10);
    CHECK(x.GetNumer() == -1);
    CHECK(x.GetDenom() == 5);
    x.SetRational(-2,-8);
    CHECK(x.GetNumer() == 1);
    CHECK(x.GetDenom() == 4);
    x.SetRational(-1024,96);
    CHECK(x.GetNumer() == -32);
    CHECK(x.GetDenom() == 3);
    cRational r(x);
    CHECK(r.GetNumer() == -32);
    CHECK(r.GetDenom() == 3);
}

TEST_CASE("Slozhenie")
{
    cRational x(4,5);
    cRational y;
    y = x+5;
    CHECK(y.GetNumer() == 29);
    CHECK(y.GetDenom() == 5);
    y.SetRational(25,3);
    cRational z = x+y;
    CHECK(z.GetNumer() == 137);
    CHECK(z.GetDenom() == 15);
}

TEST_CASE("Bolshe")
{
    cRational x(3,8);
    cRational y(14,4);
    cRational z(-32,3);
    CHECK((x>y) == false);
    CHECK((x>z) == true);
    CHECK((y>z) == true);
    CHECK((x>1) == false);
    CHECK((x>0) == true);
    CHECK((z>-11) == true);
    CHECK((y>2.5) == true);
    CHECK((3>2.5) == true);
    CHECK((0>z) == true);
}

TEST_SUITE_BEGIN("Output");

TEST_SUITE_END();
