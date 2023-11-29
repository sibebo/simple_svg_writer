# Simple SVG Writer

... is a simple single header SVG writer for building SVG files.


## Classes and their dependency 

```
Point

Attribute

Transform

AbstractBase
|
+-- Base
    |
    +-- Rect
    +-- Path
    +-- Line
    +-- Circle
    +-- Ellipse
    +-- Use
    |
    +-- PolyBase
    |   +-- Polyline
    |   +-- Polygon
    |
    +-- GroupBase
        +-- Text
        +-- Group
        +-- Layer
        +-- Document
```




## How to use:

```c++
simple_svg::Rect    rect{{0.0, 0.0}, {10.0, 10.0}};
rect.Stroke("red").StrokeWidth(1.0).Fill("green").Opacity(0.25);

simple_svg::Group   group;
group.Append(simple_svg::Line({0.0, 0.0}, {10.0, 10.0}).StrokeWidth(0.5).Stroke("blue"));
group.Append(simple_svg::Line({0.0, 10.0}, {10.0, 0.0}).StrokeWidth(0.5).Stroke("blue"));

auto    layer = simple_svg::Layer("my layer").Append(simple_svg::Circle{{5.0, 5.0}, 4.0}.Fill("yellow").StrokeWidth(0.2).Stroke("red").Opacity(0.25));

simple_svg::Document    doc;
doc.Append(rect).Append(group).Append(layer);

std::cout << doc;
```

