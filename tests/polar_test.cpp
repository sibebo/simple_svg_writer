//#include <catch2/catch_test_macros.hpp>
#include "../catch2/catch_amalgamated.hpp"

#include "../src/simple_svg_writer.h"

#include <sstream>

TEST_CASE("ToRadians")
{
    CHECK(simple_svg::ToRadians(30.0) == Catch::Approx(0.5235987756));
    CHECK(simple_svg::ToDegrees(0.5235987756) == Catch::Approx(30.0));
}

TEST_CASE("simple_svg::Polar")
{
    simple_svg::Polar   p(10, 20);

    SECTION("construction")
    {
        simple_svg::Polar   p(simple_svg::ToRadians(30.0), 5.0);

        CHECK(p.Radians() == Catch::Approx(0.5235987756));
        CHECK(p.Modulus() == Catch::Approx(5.0));

        CHECK(p.X() == Catch::Approx(4.3301270189));
        CHECK(p.Y() == Catch::Approx(2.5));
    }
}
