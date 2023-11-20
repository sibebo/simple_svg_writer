//#include <catch2/catch_test_macros.hpp>
#include "../catch2/catch_amalgamated.hpp"

#include "../src/simple_svg_writer.h"

#include <sstream>

TEST_CASE("simple_svg::Point")
{
    simple_svg::Point   p(10, 20);
    
    SECTION("ToText")
    {
        std::ostringstream  s;
        s << p.X() << ',' << p.Y();
        CHECK(p.ToText() == s.str());
    }
    
    SECTION("Length")
    {
        CHECK(p.X() == Catch::Approx(10.0));
        CHECK(p.Y() == Catch::Approx(20.0));
        CHECK(p.Length() == Catch::Approx(22.360679775));
    }

    SECTION("Traverse")
    {
        auto    p1 = p.Traverse();
        CHECK(p1.X() == Catch::Approx(-20.0));
        CHECK(p1.Y() == Catch::Approx(10.0));
    }

    SECTION("operator+")
    {
        auto    p1 = p + simple_svg::Point{-10, -20};
        CHECK(p1.X() == Catch::Approx(0.0));
        CHECK(p1.Y() == Catch::Approx(0.0));
    }
    
    SECTION("operator-")
    {
        auto    p1 = p - simple_svg::Point{10, 20};
        CHECK(p1.X() == Catch::Approx(0.0));
        CHECK(p1.Y() == Catch::Approx(0.0));
    }
    
    SECTION("operator*, scaling 1")
    {
        auto    p1 = p*10;
        CHECK(p1.X() == Catch::Approx(100.0));
        CHECK(p1.Y() == Catch::Approx(200.0));
    }

    SECTION("operator*, scaling 2")
    {
        auto    p1 = 10*p;
        CHECK(p1.X() == Catch::Approx(100.0));
        CHECK(p1.Y() == Catch::Approx(200.0));
    }

    SECTION("operator/, scaling")
    {
        auto    p1 = p/10;
        CHECK(p1.X() == Catch::Approx(1.0));
        CHECK(p1.Y() == Catch::Approx(2.0));
    }

    SECTION("operator*, scalar product 1")
    {
        auto    dot = p * simple_svg::Point{-5, 10};
        CHECK(dot == Catch::Approx(10*(-5) + 20*10));
    }

    SECTION("operator*, scalar product 2")
    {
        auto    dot = simple_svg::Point{-5, 10} * p;
        CHECK(dot == Catch::Approx(10*(-5) + 20*10));
    }
    
    SECTION("operator==")
    {
        auto    p1 = p;
        simple_svg::Point    p2{3,4};
        CHECK(p1 == p);
        CHECK(!(p2 == p));
    }

}
