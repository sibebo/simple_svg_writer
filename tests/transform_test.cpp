//#include <catch2/catch_test_macros.hpp>
#include "../catch2/catch_amalgamated.hpp"

#include "../src/simple_svg_writer.h"

#include <sstream>

TEST_CASE("simple_svg::Transform, Matrix")
{
    simple_svg::Transform   t;
    CHECK(t.AsAttribute().Name() == "transform");

    SECTION("Matrix")
    {
        t.Matrix(1.2, 
                 2.3, 
                 3.4, 
                 4.5, 
                 5.6, 
                 6.7);
        
        CHECK(t.AsAttribute().Value() == "matrix(1.2 2.3 3.4 4.5 5.6 6.7)");
    }
    
}
TEST_CASE("simple_svg::Transform, Translate")
{
    simple_svg::Transform   t;
    SECTION("Translate, pair")
    {
        t.Translate(1.2, 
                    2.3);
        
        CHECK(t.AsAttribute().Value() == "translate(1.2 2.3)");
    }
    SECTION("Translate, single")
    {
        t.Translate(1.2);
        
        CHECK(t.AsAttribute().Value() == "translate(1.2 0)");
    }
    SECTION("Translate, Point")
    {
        t.Translate(simple_svg::Point{1.2, 
                                      2.3});
        
        CHECK(t.AsAttribute().Value() == "translate(1.2 2.3)");
    }

}
TEST_CASE("simple_svg::Transform, Scale")
{
    simple_svg::Transform   t;
    SECTION("Scale, pair")
    {
        t.Scale(1.2, 
                2.3);
        
        CHECK(t.AsAttribute().Value() == "scale(1.2 2.3)");
    }
    SECTION("Scale, Point")
    {
        t.Scale(simple_svg::Point{1.2, 
                                  2.3});
        
        CHECK(t.AsAttribute().Value() == "scale(1.2 2.3)");
    }
    SECTION("Scale, single")
    {
        t.Scale(1.2);
        
        CHECK(t.AsAttribute().Value() == "scale(1.2 1.2)");
    }
}
TEST_CASE("simple_svg::Transform, Rotation")
{
    simple_svg::Transform   t;
    SECTION("Rotate, pair")
    {
        t.Rotate(45, 
                 1.2, 
                 2.3);
        
        CHECK(t.AsAttribute().Value() == "rotate(45 1.2 2.3)");
    }
    SECTION("Rotate, Point")
    {
        t.Rotate(45, 
                 simple_svg::Point{1.2, 
                                   2.3});
        
        CHECK(t.AsAttribute().Value() == "rotate(45 1.2 2.3)");
    }
    SECTION("Rotate, single")
    {
        t.Rotate(45);
        
        CHECK(t.AsAttribute().Value() == "rotate(45)");
    }
    
}
TEST_CASE("simple_svg::Transform, SkewX")
{
    simple_svg::Transform   t;
    SECTION("SkewX")
    {
        t.SkewX(1.2);
        
        CHECK(t.AsAttribute().Value() == "skewX(1.2)");
    }
}
TEST_CASE("simple_svg::Transform, SkewY")
{
    simple_svg::Transform   t;
    SECTION("SkewX")
    {
        t.SkewY(1.2);
        
        CHECK(t.AsAttribute().Value() == "skewY(1.2)");
    }
}
TEST_CASE("simple_svg::Transform, chaining")
{
    simple_svg::Transform   t;
    SECTION("SkewX")
    {
        t.Rotate(45, 0, 0).SkewX(25).Translate(100, 0).Scale(2, 3).SkewY(30).Translate(0, 100);
        
        CHECK(t.AsAttribute().Value() == "rotate(45 0 0) skewX(25) translate(100 0) scale(2 3) skewY(30) translate(0 100)");
    }
}
