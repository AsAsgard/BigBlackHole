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

TEST_CASE("Other math operations")
{
    cRational x(3,4);
    cRational y(-2,7);
    cRational z = x - y;
    cRational f(29,28);
    cRational l(-21,8);
    cRational bzz(5,6);
    bzz -= x;
    z /= 2;
    z = z* 2;
    CHECK ((z == bzz) ==false);
    CHECK ((z == f) == true);
    CHECK (((z-=f) == 0) == true);
    CHECK (((x/y) == l) == true);
    CHECK (((x*y) == cRational(-3,14)) == true );
    CHECK (((x*y) != cRational(-2,14)) == true );
    CHECK (((x*4) == cRational(3,1)) == true );
    CHECK (((4*y) == cRational(-8,7)) == true );
    CHECK (((4/x) == cRational(16,3)) == true );
    CHECK (((5/y) == -17.5) == true);
    CHECK_THROWS (4/cRational(0,3));
}

TEST_CASE("OUTPUT")
{
    cRational x(3,4);
    cRational y(-2,7);
    cout << x*y << "   " << cRational(-3,14) << endl;
    cout << 4*y << "   " << cRational(-8,7) << endl;
    cout << 4/x << "   " << cRational(16,3) << endl;
}


TEST_SUITE_BEGIN("Comparison");

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

TEST_CASE("All operators")
{
    cRational x(3,8);
    cRational y(3,8);
    CHECK((x>y) == false);
    CHECK((x<y) == false);
    CHECK((y==x) == true);
    CHECK((y==0.375) == true);
    CHECK((x>=0.375) == true);
    CHECK((x>=0.376) == false);
    CHECK((1<=y) == false);
    CHECK((-y>0) == false);
    CHECK((0<(+y)) == true);
    CHECK((0<(+y+(-1))) == false);
    CHECK((0<(+y)) == true);
    CHECK((y!=x+1) == true);
    CHECK((y!=x) == false);
    cRational z(26,5);
    CHECK((z==5) == false);
    CHECK((static_cast<int>(z)==5) == true);
    CHECK((static_cast<int>(z)==6) == false);
    z.SetRational(29,5);
    CHECK((static_cast<int>(z)==6) == true);
    z.SetRational(-29,5);
    CHECK((static_cast<int>(z)==-6) == true);
    CHECK((static_cast<int>(z)==-5) == false);
    z.SetRational(-26,5);
    CHECK((static_cast<int>(z)==-5) == true);
}

TEST_SUITE_END();

TEST_CASE("INPUT")
{
    cRational x;
    cin >> x; // 3/8
    cRational y(3,8);
    CHECK((x>y) == false);
    CHECK((x<y) == false);
    CHECK((y==x) == true);
    CHECK((y==0.375) == true);
    CHECK((x>=0.375) == true);
    CHECK((x>=0.376) == false);
    CHECK((1<=y) == false);
    CHECK((-y>0) == false);
    CHECK((0<(+y)) == true);
    CHECK((0<(+y+(-1))) == false);
    CHECK((0<(+y)) == true);
    CHECK((y!=x+1) == true);
    CHECK((y!=x) == false);
    cRational z;
    cin >> z; // 26/5
    CHECK((z==5) == false);
    CHECK((static_cast<int>(z)==5) == true);
    CHECK((static_cast<int>(z)==6) == false);
    cin >> z; // 29/5
    CHECK((static_cast<int>(z)==6) == true);
    cin >> z; // -29/5
    CHECK((static_cast<int>(z)==-6) == true);
    CHECK((static_cast<int>(z)==-5) == false);
    cin >> z; // -26/5
    CHECK((static_cast<int>(z)==-5) == true);
}


