#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>
#include <cmath>

namespace simple_svg
{

//-----------------------------------------------------------------------------
inline std::string to_string(double value)
{
    std::stringstream   s;
    s << value;
    return s.str();
}

//-----------------------------------------------------------------------------
class Point
{
    double  x{0.0};
    double  y{0.0};

public:
    Point() = default;
    Point(const Point&) = default;
    Point(Point&&) = default;
    Point& operator=(const Point&) = default;
    Point& operator=(Point&&) = default;

    Point(double x, double y) : x(x), y(y) {}

    double  X() const {return x;}
    double  Y() const {return y;}

    std::string ToText() const
    {
        std::stringstream stream;
        stream << x << ',' << y;
        return stream.str();
    }

    double  Length() const {return std::sqrt(x*x + y*y);}

    friend std::ostream& operator<<(std::ostream &stream, const Point &point)
    {
        return stream << point.ToText();
    }

    friend  Point   operator+(const Point &a, const Point &b) {return {a.x + b.x, a.y + b.y};}
    friend  Point   operator-(const Point &a, const Point &b) {return {a.x - b.x, a.y - b.y};}
    friend  Point   operator/(const Point &a, double divisor) {return {a.x / divisor, a.y / divisor};}
    friend  Point   operator*(const Point &a, double factor)  {return {a.x * factor, a.y * factor};}
    friend  Point   operator*(double factor, const Point &a)  {return a * factor;}
    friend  double  operator*(const Point &a, const Point &b) {return a.x * b.x + a.y * b.y;}
    friend  bool    operator==(const Point &a, const Point &b) {return std::fabs(a.x - b.x) < 1e-3 && std::fabs(a.y - b.y) < 1e-3;}
};

//-----------------------------------------------------------------------------
class Attribute
{
    std::string name;
    std::string value;
public:
    Attribute() = default;
    Attribute(const Attribute&) = default;
    Attribute(Attribute&&) = default;
    Attribute& operator=(const Attribute&) = default;
    Attribute& operator=(Attribute&&) = default;

    Attribute(std::string name, std::string value) : name(name), value(value) {}
    Attribute(std::string name, double value) : name(name), value(to_string(value)) {}
    Attribute(std::string name, int32_t value) : name(name), value(std::to_string(value)) {}
    Attribute(std::string name, bool value) : name(name), value(value ? "true" : "false") {}

    std::string Name() const {return name;}
    std::string Value() const {return value;}
    void        Value(std::string value) {this->value = value;}

    std::string ToText() const
    {
        return name + "=\"" + value + "\"";
    }

    friend std::ostream& operator<<(std::ostream &stream, const Attribute &attribute)
    {
        return stream << attribute.ToText();
    }
};

class Transform
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform
    std::vector<std::string>    transforms;
public:
    Transform&  matrix(double a, double b, double c, double d, double e, double f)
    {
        std::stringstream   stream;
        stream << "matrix(" << a << " " << b << " " << c << " " << d << " " << e << " " << f << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    Transform&  Translate(double dx, double dy=0.0)
    {
        std::stringstream   stream;
        stream << "translate(" << dx << " " << dy << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    Transform&  Translate(const Point &dp)
    {
        return Scale(dp.X(), dp.Y());
    }

    Transform&  Scale(double scale_x, double scale_y)
    {
        std::stringstream   stream;
        stream << "scale(" << " " << scale_x << " " << scale_y << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    Transform&  Scale(const Point &scale)
    {
        return Scale(scale.X(), scale.Y());
    }

    Transform&  Scale(double scale_x)
    {
        return Scale(scale_x, scale_x);
    }

    Transform&  Rotate(double angle, double about_x=0.0, double about_y=0.0)
    {
        std::stringstream   stream;
        stream << "rotate(" << angle << " " << about_x << " " << about_y << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    Transform&  Rotate(double angle, const Point about)
    {
        return Rotate(angle, about.X(), about.Y());
    }

    Transform&  SkewX(double skew_x)
    {
        std::stringstream   stream;
        stream << "skewX(" << " " << skew_x << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    Transform&  SkewY(double skew_y)
    {
        std::stringstream   stream;
        stream << "skewY(" << " " << skew_y << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    Attribute   AsAttribute() const
    {
        std::stringstream   stream;

        for (size_t i = transforms.size(); i != 0; --i)
        //for (const auto &t : transforms)
        {
            stream << transforms[i-1] << " ";
        }

        return {"transform", stream.str()};
    }
};

//-----------------------------------------------------------------------------
class Base
{
    std::string             tag;
    std::vector<Attribute>  attributes;

protected:
    virtual std::string Extras() const {return {};}

public:
    Base() = default;
    Base(const Base&) = default;
    Base(Base&&) = default;
    Base& operator=(const Base&) = default;
    Base& operator=(Base&&) = default;

    Base(const std::string &tag) : tag(tag) {}
    Base(const std::string &tag, const std::vector<Attribute> &attributes)
        : tag(tag),
          attributes(attributes)
    {}

    virtual ~Base() {}

    std::string     Tag() const {return tag;}
    const auto&     Attributes() const {return attributes;}

    Base&   AddAttribute(const Attribute &attribute)
    {
        auto ii = std::find_if(attributes.begin(), attributes.end(), [attribute](const auto &a){return a.Name().compare(attribute.Name())==0;});
        if (ii != attributes.end())
        {
            ii->Value(attribute.Value());
        }
        else
        {
            attributes.push_back(attribute);
        }
        return *this;
    }

    Base&   Id(const std::string &id)
    {
        return AddAttribute({"id", id});
    }

    Base&   Class(const std::string &class_name)
    {
        return AddAttribute({"class", class_name});
    }

    Base&   Stroke(const std::string &stroke)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke
    {
        return AddAttribute({"stroke", stroke});
    }

    Base&   StrokeWidth(const double &stroke_width)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-width
    {
        return AddAttribute({"stroke-width", stroke_width});
    }

    Base&   StrokeOpacity(const double &stroke_opacity)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-opacity
    {
        return AddAttribute({"stroke-opacity", stroke_opacity});
    }

    Base&   Fill(const std::string &fill)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/fill
    {
        return AddAttribute({"fill", fill});
    }

    Base&   FillOpacity(const double &fill_opacity)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/fill-opacity
    {
        return AddAttribute({"fill-opacity", fill_opacity});
    }

    Base&   Opacity(const double &opacity)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/opacity
    {
        return AddAttribute({"opacity", opacity});
    }

    Base&   Transform(const Transform &transform)
    {
        return AddAttribute(transform.AsAttribute());
    }

    virtual std::string ToText() const
    {
        std::ostringstream  stream;
        stream << "<" << tag;

        stream << ' ' << Extras();

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
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/rect
public:
    Rect(const Rect&) = default;
    Rect(Rect&&) = default;
    Rect& operator=(const Rect&) = default;
    Rect& operator=(Rect&&) = default;

    Rect() : Base("rect") {}
    Rect(double x, double y, double w, double h)
        : Base("rect", {{"x", x}, {"y", y}, {"width", w}, {"height", h}})
    {}
    Rect(double w, double h)
        : Base("rect", {{"width", w}, {"height", h}})
    {}
    Rect(const Point &from, const Point &to)
        : Base("rect", {{"x", from.X()}, {"y", from.Y()}, {"width", to.X() - from.X()}, {"height", to.Y() - from.Y()}})
    {}
    virtual ~Rect() {}
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
        return Add({x, y});
    }

    PolyBase&   Add(const std::vector<Point> &points)
    {
        for (const auto &p : points)
        {
            Add(p);
        }

        return *this;
    }
};

class Polyline : public PolyBase
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
public:
    Polyline(const Polyline&) = default;
    Polyline(Polyline&&) = default;
    Polyline& operator=(const Polyline&) = default;
    Polyline& operator=(Polyline&&) = default;

    Polyline() : PolyBase("polyline") {}
    Polyline(const std::vector<Point> &points)
        : PolyBase("polyline", points)
    {}
    virtual ~Polyline() override {}
};

class Polygon : public PolyBase
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polygon
public:
    Polygon(const Polygon&) = default;
    Polygon(Polygon&&) = default;
    Polygon& operator=(const Polygon&) = default;
    Polygon& operator=(Polygon&&) = default;

    Polygon() : PolyBase("polygon") {}
    Polygon(const std::vector<Point> &points)
        : PolyBase("polygon", points)
    {}
    virtual ~Polygon() override {}
};

class Line : public Base
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/line
public:
    Line(const Line&) = default;
    Line(Line&&) = default;
    Line& operator=(const Line&) = default;
    Line& operator=(Line&&) = default;

    Line() : Base("line") {}
    Line(double from_x, double from_y, double to_x, double to_y)
        : Base("line", {{"x1",from_x},{"y1",from_y},{"x2",to_x},{"y2",to_y}})
    {}
    Line(const Point &from, const Point &to)
        : Base("line", {{"x1",from.X()},{"y1",from.Y()},{"x2",to.X()},{"y2",to.Y()}})
    {}
    virtual ~Line() override {}
};

class Circle : public Base
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
public:
    Circle(const Circle&) = default;
    Circle(Circle&&) = default;
    Circle& operator=(const Circle&) = default;
    Circle& operator=(Circle&&) = default;

    Circle() : Base("circle") {}
    Circle(double center_x, double center_y, double radius)
        : Base("circle", {{"cx",center_x},{"cy",center_y},{"r",radius}})
    {}
    Circle(const Point &center, double radius)
        : Base("circle", {{"cx",center.X()},{"cy",center.Y()},{"r",radius}})
    {}
    virtual ~Circle() override {}
};

class Ellipse : public Base
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/ellipse
public:
    Ellipse(const Ellipse&) = default;
    Ellipse(Ellipse&&) = default;
    Ellipse& operator=(const Ellipse&) = default;
    Ellipse& operator=(Ellipse&&) = default;

    Ellipse() : Base("ellipse") {}
    Ellipse(double center_x, double center_y, double radius_x, double radius_y)
        : Base("ellipse", {{"cx",center_x},{"cy",center_y},{"rx",radius_x},{"ry",radius_y}})
    {}
    Ellipse(const Point &center, double radius_x, double radius_y)
        : Base("ellipse", {{"cx",center.X()},{"cy",center.Y()},{"rx",radius_x},{"ry",radius_y}})
    {}
    virtual ~Ellipse() override {}
};

class Use : public Base
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/use
public:
    Use(const Use&) = default;
    Use(Use&&) = default;
    Use& operator=(const Use&) = default;
    Use& operator=(Use&&) = default;

    Use() : Base("use") {}
    Use(std::string reference_id)
        : Base("use", {{"xlink:href", '#' + reference_id}})
    {}
    virtual ~Use() override {}
};

//-----------------------------------------------------------------------------
class GroupBase : public Base
{
    std::vector<std::shared_ptr<Base>>  objects;

protected:
    std::string StartTag() const
    {
        std::ostringstream  stream;
        stream << "<" << Tag();

        for (const auto &attribute : Attributes())
        {
            stream << ' ' << attribute;
        }
        stream << ">";

        return stream.str();
    }

    std::string EndTag() const
    {
        std::ostringstream  stream;
        stream << "</" << Tag() << ">";

        return stream.str();
    }

public:
    GroupBase(const GroupBase&) = default;
    GroupBase(GroupBase&&) = default;
    GroupBase& operator=(const GroupBase&) = default;
    GroupBase& operator=(GroupBase&&) = default;

    GroupBase(std::string group_tag)
        : Base(group_tag) {}
    GroupBase(std::string group_tag, const std::vector<Attribute> &attributes)
        : Base(group_tag, attributes) {}
    virtual ~GroupBase() override {}

    template<typename T>
    GroupBase&  Append(const T& object)
    {
        objects.push_back(std::make_shared<T>(object));
        return *this;
    }

    virtual std::string ToText() const override
    {
        std::ostringstream  stream;
        stream << StartTag() << '\n';

        for (const auto &object : objects)
        {
            stream << "  " << *object << '\n';
        }

        stream << EndTag();

        return stream.str();
    }

    friend std::ostream& operator<<(std::ostream &stream, const GroupBase &group_base)
    {
        return stream << group_base.ToText();
    }
};

class Text : public GroupBase
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text

    std::string text;
public:
    Text(const Text&) = default;
    Text(Text&&) = default;
    Text& operator=(const Text&) = default;
    Text& operator=(Text&&) = default;

    Text(double x, double y, const std::string &text)
        : GroupBase("text", {{"x",x},{"y",y}}),
          text(text)
    {}
    Text(const Point &where, const std::string &text)
        : GroupBase("text", {{"x",where.X()},{"y",where.Y()}}),
          text(text)
    {}
    virtual ~Text() override {}

    Text&   TextAnchor(const std::string &text_anchor)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-anchor
    {
        AddAttribute({"text-anchor", text_anchor});
        return *this;
    }

    Text&   Right() {return TextAnchor("start");}
    Text&   Center() {return TextAnchor("middle");}
    Text&   Left() {return TextAnchor("end");}

    Text&   DominantBaseline(const std::string &dominant_baseline)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/dominant-baseline
    {
        AddAttribute({"dominant-baseline", dominant_baseline});
        return *this;
    }

    Text&   Auto() {return DominantBaseline("auto");}       ///< standard "on line" vertical alignment.
    Text&   Middle() {return DominantBaseline("middle");}   ///< "centered" vertical alignment.
    Text&   Hanging() {return DominantBaseline("hanging");} ///< "hangning" vertical alignment.

    Text&   FontFamily(const std::string &font_family)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/font-family
    {
        AddAttribute({"font-family", font_family});
        return *this;
    }

    Text&   FontSize(const std::string &font_size)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/font-size
    {
        AddAttribute({"font-size", font_size});
        return *this;
    }
    Text&   FontSize(const double &font_size) {return FontSize(std::to_string(font_size) + "pt");}

    Text&   FontStyle(const std::string &font_style)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/font-style
    {
        AddAttribute({"font-style", font_style});
        return *this;
    }

    Text&   FontWeight(const std::string &font_weight)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/font-weight
    {
        AddAttribute({"font-weight", font_weight});
        return *this;
    }

    Text&   Bold() {return FontWeight("bold");}
    Text&   Italic() {return FontStyle("italic");}
    Text&   Oblique() {return FontStyle("italic");}
    Text&   Normal() {return FontStyle("normal").FontWeight("normal");}

    virtual std::string ToText() const override
    {
        std::ostringstream  stream;
        stream << StartTag();
        stream << text;
        stream << EndTag();

        return stream.str();
    }
};

class Group : public GroupBase
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/g
public:
    Group(const Group&) = default;
    Group(Group&&) = default;
    Group& operator=(const Group&) = default;
    Group& operator=(Group&&) = default;

    Group() : GroupBase("g") {}
    virtual ~Group() override {}

    friend std::ostream& operator<<(std::ostream &stream, const Group &group)
    {
        return stream << group.ToText();
    }
};

class Layer : public GroupBase
{
    // Implemented as Inkscape layer based on a group, <g>, with special attributes.
public:
    Layer(const Layer&) = default;
    Layer(Layer&&) = default;
    Layer& operator=(const Layer&) = default;
    Layer& operator=(Layer&&) = default;

    Layer()
        : GroupBase("g", {{"inkscape:groupmode", std::string("layer")}})
    {}
    Layer(const std::string &name)
        : GroupBase("g", {{"inkscape:label", name}, {"inkscape:groupmode", std::string("layer")}})
    {}
    virtual ~Layer() override {}

    friend std::ostream& operator<<(std::ostream &stream, const Layer &group)
    {
        return stream << group.ToText();
    }
};

class Document : public GroupBase
{
public:
    Document(const Document&) = default;
    Document(Document&&) = default;
    Document& operator=(const Document&) = default;
    Document& operator=(Document&&) = default;

    //<?xml version="1.0"?>
    //<svg width="100" height="100" xmlns="http://www.w3.org/2000/svg">
    //</svg>
    Document()
        : GroupBase(
              "svg",
    {{"xmlns", std::string("http://www.w3.org/2000/svg")},
    {"xmlns:xlink", std::string("http://www.w3.org/1999/xlink")},
    {"xmlns:inkscape",std::string("http://www.inkscape.org/namespaces/inkscape")}})
    {}
    Document(double width, double height)
        : GroupBase(
              "svg",
    {{"width",to_string(width)},
    {"height",to_string(height)},
    {"xmlns", std::string("http://www.w3.org/2000/svg")},
    {"xmlns:xlink", std::string("http://www.w3.org/1999/xlink")},
    {"xmlns:inkscape",std::string("http://www.inkscape.org/namespaces/inkscape")}})
    {}
    virtual ~Document() override {}

    Document&   ViewBox(double x_min, double y_min, double width, double height)
    {
        std::ostringstream  stream;
        stream << x_min << ' ' << y_min << ' ' << width << ' ' << height;
        AddAttribute({"viewBox", stream.str()});

        return *this;
    }

    virtual std::string ToText() const override
    {
        std::ostringstream  stream;
        stream << "<?xml version=\"1.0\"?>" << '\n';

        stream << GroupBase::ToText();

        return stream.str();
    }

    friend std::ostream& operator<<(std::ostream &stream, const Document &document)
    {
        return stream << document.ToText();
    }
};

} // namespace simple_svg
