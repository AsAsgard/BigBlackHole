#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "derivation.h"

TEST_CASE("constants")
{
    CHECK(derivation("1") == "0");
    CHECK(derivation("-2") == "0");
    CHECK(derivation("0*x") == "0");
}

TEST_SUITE_BEGIN("monomials");

TEST_CASE("trivial monomials")
{
    CHECK(derivation("x") == "1");
    CHECK(derivation("+x") == "1");
    CHECK(derivation("-x") == "-1");
    CHECK(derivation("+1*x") == "1");
    CHECK(derivation("2*x") == "2");
}

TEST_CASE("unreduced monomials")
{
    CHECK(derivation("x+x+x+x+x") == "5");
    CHECK(derivation("x-x+x-x") == "0");
}

TEST_CASE("random monomials")
{
    CHECK(derivation("x^2") == "2*x");
    CHECK(derivation("2*x^3") == "6*x^2");
    CHECK(derivation("-x^3") == "-3*x^2");
}

TEST_SUITE_END();

TEST_CASE("using random polynomes")
{
    CHECK(derivation("x^2+x") == "2*x+1");
    CHECK(derivation("2*x^100+100*x^2") == "200*x^99+200*x");
    CHECK(derivation("x^10000+x+1") == "10000*x^9999+1");
    CHECK(derivation("-x^2-x^3") == "-3*x^2-2*x");
    CHECK(derivation("x+x+x+x+x+x+x+x+x+x") == "10");
}
