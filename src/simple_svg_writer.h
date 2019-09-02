#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace simple_svg
{

class Attribute
{
    std::string name;
    std::string value;
public:
    Attribute(std::string name, std::string value) : name(name), value(value) {}
    Attribute(std::string name, double value) : name(name), value(std::to_string(value)) {}
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

protected:
    virtual std::string Extras() const override {return {};}

public:
    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    std::string ToText() const
    {
        return std::to_string(x) + ',' + std::to_string(y);
    }

    friend std::ostream& operator<<(std::ostream &stream, const Point &point)
    {
        return stream << point.ToText();
    }
};

class Polyline : public Base
{
    std::vector<Point> points;
public:
    Polyline() : Base("polyline") {}
    Polyline(const std::vector<Point> &points)
        : Base("polyline"),
          points(points)
    {}

    Polyline&   Add(const Point &point)
    {
        points.push_back(point);
    }
    Polyline&   Add(double x, double y)
    {
        points.push_back({x, y});
    }

    std::string ToText()
    {
        AddParameter({{"d", ""}});
    }
};


} // namespace simple_svg
