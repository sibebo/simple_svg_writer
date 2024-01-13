//#include <catch2/catch_test_macros.hpp>
#include "../catch2/catch_amalgamated.hpp"

#include "../src/simple_svg_writer.h"

class Test : public simple_svg::Base<Test>
{
public:
    Test() : Base("test") {}
};


TEST_CASE("Base, default constructor")
{
    class Test  t;

    SECTION("raw")
    {
        CHECK(t.Tag() == "test");
        CHECK(t.Attributes().empty() == true);
        CHECK(t.ToText() == "<test/>");
    }
    SECTION("id")
    {
        t.Id("hello");

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "id");
        CHECK(a[0].Value() == "hello");
        CHECK(t.ToText() == "<test id=\"hello\"/>");
    }
    SECTION("Class")
    {
        t.Class("hello");

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "class");
        CHECK(a[0].Value() == "hello");
    }
    SECTION("Stroke")
    {
        t.Stroke("hello");

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "stroke");
        CHECK(a[0].Value() == "hello");
    }
    SECTION("StrokeWidth")
    {
        t.StrokeWidth(1.23);

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "stroke-width");
        CHECK(a[0].Value() == "1.23");
    }
    SECTION("StrokeOpacity")
    {
        t.StrokeOpacity(1.23);

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "stroke-opacity");
        CHECK(a[0].Value() == "1.23");
    }
    SECTION("StrokeDashArray")
    {
        t.StrokeDashArray({4, 1, 2});

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "stroke-dasharray");
        CHECK(a[0].Value() == "4 1 2");
    }
    SECTION("StrokeDashOffset")
    {
        t.StrokeDashOffset(3);

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "stroke-dashoffset");
        CHECK(a[0].Value() == "3");
    }
    SECTION("StrokeLineCap")
    {
        SECTION("StrokeLineCap, butt")
        {
            t.StrokeLineCap(simple_svg::SLC_BUTT);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linecap");
            CHECK(a[0].Value() == "butt");
        }
        SECTION("StrokeLineCap, butt")
        {
            t.StrokeLineCap(simple_svg::SLC_ROUND);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linecap");
            CHECK(a[0].Value() == "round");
        }
        SECTION("StrokeLineCap, square")
        {
            t.StrokeLineCap(simple_svg::SLC_SQUARE);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linecap");
            CHECK(a[0].Value() == "square");
        }
    }
    SECTION("StrokeLineJoin")
    {
        SECTION("StrokeLineJoin, arcs")
        {
            t.StrokeLineJoin(simple_svg::SLJ_ARCS);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linejoin");
            CHECK(a[0].Value() == "arcs");
        }
        SECTION("StrokeLineJoin, bevel")
        {
            t.StrokeLineJoin(simple_svg::SLJ_BEVEL);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linejoin");
            CHECK(a[0].Value() == "bevel");
        }
        SECTION("StrokeLineJoin, miter")
        {
            t.StrokeLineJoin(simple_svg::SLJ_MITER);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linejoin");
            CHECK(a[0].Value() == "miter");
        }
        SECTION("StrokeLineJoin, miter-clip")
        {
            t.StrokeLineJoin(simple_svg::SLJ_MITER_CLIP);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linejoin");
            CHECK(a[0].Value() == "miter-clip");
        }
        SECTION("StrokeLineJoin, round")
        {
            t.StrokeLineJoin(simple_svg::SLJ_ROUND);

            auto    a = t.Attributes();
            CHECK(a.size() == 1);

            CHECK(a[0].Name() == "stroke-linejoin");
            CHECK(a[0].Value() == "round");
        }
    }
    SECTION("Fill")
    {
        t.Fill("hello");

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "fill");
        CHECK(a[0].Value() == "hello");
    }
    SECTION("FillOpacity")
    {
        t.FillOpacity(1.23);

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "fill-opacity");
        CHECK(a[0].Value() == "1.23");
    }
    SECTION("Opacity")
    {
        t.Opacity(1.23);

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "opacity");
        CHECK(a[0].Value() == "1.23");
    }
    SECTION("Transform")
    {
        simple_svg::Transform tr;
        tr.Rotate(45, {0, 0}).Translate({100,100});
        t.Transform(tr);

        auto    a = t.Attributes();
        CHECK(a.size() == 1);

        CHECK(a[0].Name() == "transform");
        CHECK(a[0].Value() == "rotate(45 0 0) translate(100 100)");
    }
    SECTION("Title")
    {
        t.Title("Hello World!");
        
        auto    s = t.ToText();
        
        CHECK(s == "<test><title>Hello World!</title></test>");
    }
    SECTION("chaining")
    {
        t.Id("hello_id").Class("hello_class").Stroke("hello_stroke");

        auto    a = t.Attributes();
        CHECK(a.size() == 3);

        CHECK(a[0].Name() == "id");
        CHECK(a[0].Value() == "hello_id");

        CHECK(a[1].Name() == "class");
        CHECK(a[1].Value() == "hello_class");

        CHECK(a[2].Name() == "stroke");
        CHECK(a[2].Value() == "hello_stroke");

        CHECK(t.ToText() == "<test id=\"hello_id\" class=\"hello_class\" stroke=\"hello_stroke\"/>");

        SECTION("Replacing")
        {
            t.Class("world_class");

            auto    a = t.Attributes();
            CHECK(a.size() == 3);

            CHECK(a[0].Name() == "id");
            CHECK(a[0].Value() == "hello_id");

            CHECK(a[1].Name() == "class");
            CHECK(a[1].Value() == "world_class");

            CHECK(a[2].Name() == "stroke");
            CHECK(a[2].Value() == "hello_stroke");

            CHECK(t.ToText() == "<test id=\"hello_id\" class=\"world_class\" stroke=\"hello_stroke\"/>");
        }
    }
}
