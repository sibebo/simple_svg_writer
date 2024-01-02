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
    pl.Title("MyPolyline");

    simple_svg::Polygon pg({{0, 0}, {100, 100}, {50,100}, {200, 200}});
    pg.Stroke("red").Fill("green").StrokeWidth(4).Id("polygon");
    pg.Title("MyPolygon");

    simple_svg::Circle  c({50, 50}, 25);
    c.Stroke("red").Fill("green").StrokeWidth(8).Id("circle");
    simple_svg::Ellipse e({50, 50}, 25, 15);
    e.Stroke("blue").Fill("yellow").StrokeWidth(1);
    simple_svg::Line    l({50, 50}, {25, 15});
    l.Stroke("brown").StrokeWidth(4);


    simple_svg::Layer   layer_1("lag_1");
    //layer_1.Append(std::move(pl));
    layer_1.Append(pl);

    simple_svg::Layer   layer_2("lag_2");
    layer_2.Append(pg);
    layer_2.Append(l);

    simple_svg::Layer   layer_3("lag_3");
    layer_3.Append(c);
    layer_3.Append(e);

    //layer_1.Append(simple_svg::Text({0,0}, "Hej Verden"));
    auto    hej = simple_svg::Text({0,0}, "Hej Verden").Middle().Center();
    layer_1.Append(hej);

    simple_svg::Layer   layer_4("lag_4");
    simple_svg::Text    t4({0,0}, "Daw, ven");

    simple_svg::Transform   tr4;
    tr4.Rotate(45.0, {0.0, 0.0});
    tr4.Rotate(45.0, {0.0, 0.0});
    t4.Transform(tr4);

    layer_4.Append(t4);


    simple_svg::Layer   layer_5("lag_5");
    simple_svg::Text    t5({0,0}, "Daw, ven");

    simple_svg::Transform   tr5;
    tr5.Rotate(45.0, {0.0, 0.0}).Translate(150, 150).Rotate(45.0, {0.0, 0.0});
    t5.Transform(tr5);

    layer_5.Append(t5);

    simple_svg::Layer   layer_6("lag_6");
    layer_6.Append(simple_svg::Rect({20, 20}, {30, 30}).Fill("green"));
    auto &r = layer_6.Append(simple_svg::Rect({20, 40}, {30, 50}).Fill("red"));
    layer_6.Append(simple_svg::Rect({10, 60}, {20, 70}).Fill("brown"));
    //auto& r = layer_6.back<simple_svg::Rect>();

    simple_svg::Document    d;
    d.ViewBox(-110, -10, 350, 350);
    d.Append(layer_6);
    d.Append(layer_1);
    d.Append(layer_2);
    d.Append(layer_3);
    d.Append(layer_4);
    d.Append(layer_5);

    r.Fill("blue");
    r.Set({15, 40}, {35, 60});

    std::ofstream   file("../test.svg");
    file << d;
}

void    Something()
{
    simple_svg::Rect    r({0, 0}, {200, 200});
    r.Fill("beige").Stroke("darkgreen").StrokeWidth(1);

    simple_svg::Layer   background_layer("background");
    background_layer.Append(r);

    simple_svg::Polygon pg({{0,100},{100,0},{200,100},{100,200}});
    pg.Fill("darkgreen").Stroke("brown").StrokeWidth(4);
    simple_svg::Circle  c({100,100},75);
    c.Fill("yellow").Stroke("blue").StrokeWidth(2).FillOpacity(0.50);

    simple_svg::Layer   drawing_layer("drawing");
    drawing_layer.Append(pg);
    drawing_layer.Append(c);

    simple_svg::Text    t({100,100}, "Hello");
    t.Fill("black").Stroke("none");
    t.FontFamily("sans-serif")
            .Bold()
            .FontSize(16)
            .Italic()
            .Center()
            .Middle()
            ;

    simple_svg::Layer   text_layer("text");
    text_layer.Append(t);

    simple_svg::Document    d(200, 200);
    d.ViewBox(0, 0, 200, 200);
    d.Append(background_layer);
    d.Append(drawing_layer);
    d.Append(text_layer);

    std::ofstream("../something.svg") << d;
}

void    Pather()
{
    //<svg width="320" height="320" xmlns="http://www.w3.org/2000/svg">
    //  <path d="M 10 315
    //           L 110 215
    //           A 30 50 0 0 1 162.55 162.45
    //           L 172.55 152.45
    //           A 30 50 -45 0 1 215.1 109.9
    //           L 315 10" stroke="black" fill="green" stroke-width="2" fill-opacity="0.5"/>
    //</svg>
    simple_svg::Path    path;
    path.MoveTo({10, 315})
            .LineTo({110, 215})
            .Arch(30, 50, 0, 0, 1, {162.55, 162.45})
            .LineTo({172.55, 152.45})
            .Arch(30, 50, -45, 0, 1, {215.1, 109.9})
            .LineTo({315, 10})
            .MoveTo({50, 10})
            .LineTo({10, 10})
            .LineTo({10, 50})
            .Close()
            .MoveTo({275, 315})
            .LineTo({315, 315})
            .LineTo({315, 275})
            .Close()
            ;
    path.Stroke("black").Fill("green").StrokeWidth(2).FillOpacity(0.5);

    simple_svg::Layer   layer("path1");
    layer.Append(path);

    simple_svg::Document    doc(320, 320);
    doc.Append(layer);

    std::ofstream("../path.svg") << doc;
}

void    Bezierer()
{


    simple_svg::Document    doc(190, 160);
    {
        //<path d="M 10 10 C 20 20, 40 20, 50 10" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({10,10}).Cubic({20,20},{40,20},{50,10});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 70 10 C 70 20, 110 20, 110 10" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({70,10}).Cubic({70,20},{110,20},{110,10});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 130 10 C 120 20, 180 20, 170 10" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({130,10}).Cubic({120,20},{180,20},{170,10});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 10 60 C 20 80, 40 80, 50 60" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({10,60}).Cubic({20,80},{40,80},{50,60});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 70 60 C 70 80, 110 80, 110 60" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({70,60}).Cubic({70,80},{110,80},{110,60});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 130 60 C 120 80, 180 80, 170 60" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({130,60}).Cubic({120,80},{180,80},{170,60});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 10 110 C 20 140, 40 140, 50 110" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({10,110}).Cubic({20,140},{40,140},{50,110});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 70 110 C 70 140, 110 140, 110 110" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({70,110}).Cubic({70,140},{110,140},{110,110});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //<path d="M 130 110 C 120 140, 180 140, 170 110" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({130,110}).Cubic({120,140},{180,140},{170,110});
        path.Stroke("black").Fill("none");
        doc.Append(path);
    }
    {
        //  <path d="M 10 80 C 40 10, 65 10, 95 80 S 150 150, 180 80" stroke="black" fill="transparent"/>
        simple_svg::Path    path;
        path.MoveTo({10,80}).Cubic({40,10},{65,10},{95,80}).Stitch({150,150},{180,80});
        path.Stroke("red").Fill("none");
        doc.Append(path);
    }


    std::ofstream("../bezier.svg") << doc;

}


int main()
{
    try {
        Pather();
        Bezierer();

        Something();

        Test();
        return 0;

        simple_svg::Rect    rect(0, 0, 100, 100);
        rect.Stroke("red").StrokeWidth(2).Fill("green");
        std::cout << rect.ToText() << '\n';

        simple_svg::Polyline    polyline({{0, 0}, {100, 100}, {50,100}, {200, 200}});
        polyline.Stroke("yellow").StrokeWidth(8).Fill("none");
        std::cout << polyline.ToText() << '\n';

        std::cout << simple_svg::Polygon({{0, 0}, {100, 100}, {50,100}, {200, 200}}).Stroke("red").Fill("green").StrokeWidth(2).Id("hej").ToText() << '\n';
        std::cout << simple_svg::Circle(50, 50, 25).Stroke("red").Fill("green").StrokeWidth(2).Id("hej").ToText() << '\n';
        std::cout << simple_svg::Ellipse(50, 50, 25, 15).Stroke("blue").Fill("yellow").StrokeWidth(2).ToText() << '\n';
        std::cout << simple_svg::Line(50, 50, 25, 15).Stroke("brown").StrokeWidth(4).ToText() << '\n';

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "HOW!!!" << std::endl;
    }


    cout << "Hello World!" << endl;
    return 0;
}
