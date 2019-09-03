#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

namespace simple_svg
{

inline std::string to_string(double value)
{
    std::stringstream   s;
    s << value;
    return s.str();
}

class Attribute
{
    std::string name;
    std::string value;
public:
    Attribute(std::string name, std::string value) : name(name), value(value) {}
    Attribute(std::string name, double value) : name(name), value(to_string(value)) {}
    Attribute(std::string name, int32_t value) : name(name), value(std::to_string(value)) {}
    Attribute(std::string name, bool value) : name(name), value(value ? "true" : "false") {}

    std::string ToText() const
    {
        return name + "=\"" + value + "\"";
    }

    friend std::ostream& operator<<(std::ostream &stream, const Attribute &attribute)
    {
        return stream << attribute.ToText();
    }
};

class Base
{
    std::string             tag;
    std::vector<Attribute>  parameters;
    std::vector<Attribute>  attributes;

protected:
    virtual std::string Extras() const {return {};}

public:
    Base(const std::string &tag) : tag(tag) {}
    Base(const std::string &tag, const std::vector<Attribute> &parameters)
        : tag(tag),
          parameters(parameters)
    {}

    virtual ~Base() {}


    Base&   AddParameter(const Attribute &parameter)
    {
        parameters.push_back(parameter);
        return *this;
    }

    Base&   AddAttribute(const Attribute &attribute)
    {
        attributes.push_back(attribute);
        return *this;
    }

    Base&   Id(const std::string &id)
    {
        attributes.emplace_back("id", id);
        return *this;
    }

    Base&   Class(const std::string &class_name)
    {
        attributes.emplace_back("class", class_name);
        return *this;
    }

    Base&   Stroke(const std::string &stroke)
    {
        attributes.emplace_back("stroke", stroke);
        return *this;
    }

    Base&   StrokeWidth(const double &stroke_width)
    {
        attributes.emplace_back("stroke-width", stroke_width);
        return *this;
    }

    Base&   Fill(const std::string &fill)
    {
        attributes.emplace_back("fill", fill);
        return *this;
    }

    std::string ToText() const
    {
        std::ostringstream  stream;
        stream << "<" << tag;

        stream << ' ' << Extras();

        for (const auto &parameter : parameters)
        {
            stream << ' ' << parameter;
        }
        for (const auto &attribute : attributes)
        {
            stream << ' ' << attribute;
        }
        stream << "/>";

        return stream.str();
    }

    friend std::ostream& operator<<(std::ostream &stream, const Base &base)
    {
        return stream << base.ToText();
    }
};


class Rect : public Base
{
public:
    Rect() : Base("rect") {}
    Rect(double x, double y, double w, double h)
        : Base("rect", {{"x", x}, {"y", y}, {"width", w}, {"height", h}})
    {}
    Rect(double w, double h)
        : Base("rect", {{"width", w}, {"height", h}})
    {}
};


class Point
{
    double  x{0.0};
    double  y{0.0};

public:
    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    std::string ToText() const
    {
        std::stringstream stream;
        stream << x << ',' << y;
        return stream.str();
    }

    friend std::ostream& operator<<(std::ostream &stream, const Point &point)
    {
        return stream << point.ToText();
    }
};

class PolyBase : public Base
{
    std::vector<Point> points;

protected:
    virtual std::string Extras() const override
    {
        std::ostringstream  stream;

        for (const auto &p : points)
        {
            stream << p << " ";
        }

        return Attribute("points", stream.str()).ToText();
    }

public:
    PolyBase(std::string tag) : Base(tag) {}
    PolyBase(std::string tag, const std::vector<Point> &points)
        : Base(tag),
          points(points)
    {}
    virtual ~PolyBase() override {}

    PolyBase&   Add(const Point &point)
    {
        points.push_back(point);
        return *this;
    }

    PolyBase&   Add(double x, double y)
    {
        points.push_back({x, y});
        return *this;
    }
};

class Polyline : public PolyBase
{
public:
    Polyline() : PolyBase("polyline") {}
    Polyline(const std::vector<Point> &points)
        : PolyBase("polyline", points)
    {}
    virtual ~Polyline() override {}
};

class Polygon : public PolyBase
{
public:
    Polygon() : PolyBase("polygon") {}
    Polygon(const std::vector<Point> &points)
        : PolyBase("polygon", points)
    {}
    virtual ~Polygon() override {}
};

class Line : public Base
{
public:
    Line() : Base("line") {}
    Line(double from_x, double from_y, double to_x, double to_y)
        : Base("line", {{"x1",from_x},{"y1",from_y},{"x2",to_x},{"y2",to_y}})
    {}
    virtual ~Line() override {}
};

class Circle : public Base
{
public:
    Circle() : Base("circle") {}
    Circle(double center_x, double center_y, double radius)
        : Base("circle", {{"cx",center_x},{"cy",center_y},{"r",radius}})
    {}
    virtual ~Circle() override {}
};

class Ellipse : public Base
{
public:
    Ellipse() : Base("circle") {}
    Ellipse(double center_x, double center_y, double radius_x, double radius_y)
        : Base("ellipse", {{"cx",center_x},{"cy",center_y},{"rx",radius_x},{"ry",radius_y}})
    {}
    virtual ~Ellipse() override {}
};


//class Group
//{
//    std::vector<std::shared_ptr<Base>>  objects;
//
//public:
//    Group() {}
//
//    Group&  Append(const Base& object)
//    {
//        objects.push_back(std::make_shared<Base>(&object));
//        return *this;
//    }
//};


} // namespace simple_svg
