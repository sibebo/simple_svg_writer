//#include <catch2/catch_test_macros.hpp>
#include "../catch2/catch_amalgamated.hpp"

#include "../src/simple_svg_writer.h"

#include <sstream>

TEST_CASE("simple_svg::Attribute")
{
    SECTION("construction")
    {
        simple_svg::Attribute   a;

        CHECK(a.Name() == "");
        CHECK(a.Value() == "");
        CHECK(a.ToText() == "=\"\"");
    }
    SECTION("construction, char*")
    {
        simple_svg::Attribute   a("test", "value");

        CHECK(a.Name() == "test");
        CHECK(a.Value() == "value");
        CHECK(a.ToText() == "test=\"value\"");
    }
    SECTION("construction, std::string")
    {
        simple_svg::Attribute   a("test", std::string("value"));

        CHECK(a.Name() == "test");
        CHECK(a.Value() == "value");
        CHECK(a.ToText() == "test=\"value\"");
    }
    SECTION("construction, double")
    {
        simple_svg::Attribute   a("test", 1.234);

        CHECK(a.Name().compare("test") == 0);
        CHECK(std::stod(a.Value()) == Catch::Approx(1.234));
    }
    SECTION("construction, int32_t")
    {
        simple_svg::Attribute   a("test", 42);

        CHECK(a.Name() == "test");
        CHECK(std::stoi(a.Value()) == 42);
        CHECK(a.ToText() == "test=\"42\"");
    }
    SECTION("construction, bool")
    {
        simple_svg::Attribute   a_true("test", true);

        CHECK(a_true.Name() == "test");
        CHECK(a_true.Value() == "true");
        CHECK(a_true.ToText() == "test=\"true\"");

        simple_svg::Attribute   a_false("test", false);

        CHECK(a_false.Name() == "test");
        CHECK(a_false.Value() == "false");
        CHECK(a_false.ToText() == "test=\"false\"");
    }
    SECTION("Value")
    {
        simple_svg::Attribute   a;
        
        a.Value("hello world");

        CHECK(a.Name() == "");
        CHECK(a.Value() == "hello world");
        CHECK(a.ToText() == "=\"hello world\"");
    }
    SECTION("operator<<")
    {
        simple_svg::Attribute   a("test", "value");
        
        std::ostringstream  s;
        s << a;

        CHECK(s.str() == a.ToText());
    }
}
