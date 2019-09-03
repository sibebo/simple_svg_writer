#include <iostream>
#include "simple_svg_writer.h"

using namespace std;

int main()
{
    try {
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
