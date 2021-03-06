#include <iostream>
#include <fstream>
#include "simple_svg_writer.h"

using namespace std;


void    Test()
{
    simple_svg::Polyline    pl;
    pl.Add({0,0});
    pl.Add({10,50});
    pl.Add({-100,100});
    pl.Add({0,150});
    pl.Stroke("purple").StrokeWidth(2).Fill("none");

    simple_svg::Polygon pg({{0, 0}, {100, 100}, {50,100}, {200, 200}});
    pg.Stroke("red").Fill("green").StrokeWidth(4).Id("polygon");

    simple_svg::Circle  c({50, 50}, 25);
    c.Stroke("red").Fill("green").StrokeWidth(8).Id("circle");
    simple_svg::Ellipse e({50, 50}, 25, 15);
    e.Stroke("blue").Fill("yellow").StrokeWidth(1);
    simple_svg::Line    l({50, 50}, {25, 15});
    l.Stroke("brown").StrokeWidth(4);


    simple_svg::Layer   layer_1("lag_1");
    layer_1.Append(std::move(pl));

    simple_svg::Layer   layer_2("lag_2");
    layer_2.Append(pg);
    layer_2.Append(l);

    simple_svg::Layer   layer_3("lag_3");
    layer_3.Append(c);
    layer_3.Append(e);


    simple_svg::Document    d;
    d.ViewBox(-100, 0, 300, 200);
    d.Append(layer_1);
    d.Append(layer_2);
    d.Append(layer_3);

    std::ofstream   file("../test.svg");
    file << d;
}


int main()
{
    try {
        Test();
        return 0;

        simple_svg::Rect    rect(0, 0, 100, 100);
        rect.Stroke("red").StrokeWidth(2).Fill("green");
        std::cout << rect << '\n';

        simple_svg::Polyline    polyline({{0, 0}, {100, 100}, {50,100}, {200, 200}});
        polyline.Stroke("yellow").StrokeWidth(8).Fill("none");
        std::cout << polyline << '\n';

        std::cout << simple_svg::Polygon({{0, 0}, {100, 100}, {50,100}, {200, 200}}).Stroke("red").Fill("green").StrokeWidth(2).Id("hej") << '\n';
        std::cout << simple_svg::Circle(50, 50, 25).Stroke("red").Fill("green").StrokeWidth(2).Id("hej") << '\n';
        std::cout << simple_svg::Ellipse(50, 50, 25, 15).Stroke("blue").Fill("yellow").StrokeWidth(2) << '\n';
        std::cout << simple_svg::Line(50, 50, 25, 15).Stroke("brown").StrokeWidth(4) << '\n';

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "HOW!!!" << std::endl;
    }


    cout << "Hello World!" << endl;
    return 0;
}
