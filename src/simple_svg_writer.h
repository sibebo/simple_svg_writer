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

inline  constexpr double  ToDegrees(double radians) {return radians * 180.0 / 3.1415926535897932384626433832795028841972;}
inline  constexpr double  ToRadians(double degrees) {return degrees * 3.1415926535897932384626433832795028841972 / 180.0;}



/**
 * Polar is a helper class holding a polar coordinate set: (angle, modulus).
*/
class Polar
{
    double  radians;
    double  modulus;
public:
    Polar() = default;
    Polar(const Polar&) = default;
    Polar(Polar&&) = default;
    Polar& operator=(const Polar&) = default;
    Polar& operator=(Polar&&) = default;

    Polar(double radians, double modulus) : radians(radians), modulus(modulus) {}
    double  Radians() const {return radians;}
    double  Modulus() const {return modulus;}
    double  X() const {return modulus * std::cos(radians);}
    double  Y() const {return modulus * std::sin(radians);}
};


//-----------------------------------------------------------------------------
/**
 * class Point represents a point, (x,y).
*/
class Point
{
    constexpr double  Deg() const {return 45.0 / std::atan(1.0);}

    double  x{0.0};
    double  y{0.0};

public:
    Point() = default;
    Point(const Point&) = default;
    Point(Point&&) = default;
    Point& operator=(const Point&) = default;
    Point& operator=(Point&&) = default;

    /**
     * Constructor instantiating Point with (x,y)
     * @param x the x-value (absissa) of the point.
     * @param y the y-value (ordinate) of the point.
    */
    Point(double x, double y) : x(x), y(y) {}

    Point(const Polar &p) : x(p.X()), y(p.Y()) {}

    /**
     * X() return the x-value (absissa) of the point.
     * @return the x-value.
    */
    double  X() const {return x;}

    /**
     * Y() return the y-value (ordinate) of the point.
     * @return the y-value.
    */
    double  Y() const {return y;}

    /**
     * Constructs a Point from a Polar.
     * @see class Polar.
    */
    Polar   ToPolar() const {return {Radians(), Length()};}

    /**
     * FromPolar sets the point from a Polar.
     * @param p     a Polar.
     * @return      reference to this Point.
     * @see class Polar.
    */
    Point&  FromPolar(const Polar &p)
    {
        x = p.X();
        y = p.Y();
        return *this;
    }

    /**
     * ToText returns a string holding the point's coordinates as "x,y".
     * @return a string holding the point's coordinates as "x,y".
    */
    std::string ToText() const
    {
        std::stringstream stream;
        stream << x << ',' << y;
        return stream.str();
    }

    /**
     * Lenght returns the lenght from origo (0,0) to the point.
     * @return lenght from origo (0,0) to the point.
    */
    double  Length() const {return std::sqrt(x*x + y*y);}

    /**
     * Degrees returns the direction angle in radians of the vector from origo to point.
     * @return  direction angle in radians of the vector from origo to point.
    */
    double  Radians() const {return std::atan2(y, x);}

    /**
     * Degrees returns the direction angle in degrees of the vector from origo to point.
     * @return  direction angle in degrees of the vector from origo to point.
    */
    double  Degrees() const {return Deg() * Radians();}

    /**
     * Traverse returns the 2D transvers vector of vector represented by point, i.e. rotated +90 deg.
     * @return  a point rotated +90 deg around origo (0,0).
    */
    Point   Traverse() const
    {
        return {-y, x};
    }

    /**
     * Convenience operator<< for wrting the point.
     * @param stream    the output stream.
     * @param point     the point to write to the stream.
     * @return          reference to the output stream after writing.
     * @see ToText().
    */
    friend std::ostream& operator<<(std::ostream &stream, const Point &point)
    {
        return stream << point.ToText();
    }

    /**
     * + operator for Point
     * @param a     a Point.
     * @param b     a Point.
     * @return      the points added (a+b).
    */
    friend  Point   operator+(const Point &a, const Point &b) {return {a.x + b.x, a.y + b.y};}

    /**
     * - operator for Point
     * @param a     a Point.
     * @param b     a Point.
     * @return      the points subtracted (a-b).
    */
    friend  Point   operator-(const Point &a, const Point &b) {return {a.x - b.x, a.y - b.y};}


    /**
     * / operator for Point
     * @param a         a Point.
     * @param divisor   a divisor.
     * @return          both coordinates of the point are divided by divisor (a/divisor).
    */
    friend  Point   operator/(const Point &a, double divisor) {return {a.x / divisor, a.y / divisor};}

    /**
     * * operator for Point
     * @param a         a Point.
     * @param factor    a factor.
     * @return          both coordinates of the point are multiplied by factor (a*factor).
     * @see operator(double, const Point&).
    */
    friend  Point   operator*(const Point &a, double factor)  {return {a.x * factor, a.y * factor};}

    /**
     * * operator for Point
     * @param factor    a factor.
     * @param a         a Point.
     * @return          both coordinates of the point are multiplied by factor (factor*a).
     * @see operator(const Point&, double).
    */
    friend  Point   operator*(double factor, const Point &a)  {return a * factor;}

    /**
     * * operator for Point implementing the scalar (or dot) product (a*b)
     * @param a         a Point.
     * @param b         a Point.
     * @return          scalar (dot) product of a and b (a*b).
    */
    friend  double  operator*(const Point &a, const Point &b) {return a.x * b.x + a.y * b.y;}

    /**
     * == operator for Point
     * @param factor    a factor.
     * @param a         a Point.
     * @return          true if a and b are identical within a margin of 1e-3.
    */
    friend  bool    operator==(const Point &a, const Point &b) {return std::fabs(a.x - b.x) < 1e-3 && std::fabs(a.y - b.y) < 1e-3;}
};

//-----------------------------------------------------------------------------
/**
 * class Attribute implements an svg attribute consisting of a (name, value) pair.
*/
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
    Attribute(std::string name, const char* value) : name(name), value(value) {}
    Attribute(std::string name, double value) : name(name), value(to_string(value)) {}
    Attribute(std::string name, int32_t value) : name(name), value(std::to_string(value)) {}
    Attribute(std::string name, bool value) : name(name), value(value ? "true" : "false") {}

    /**
     * Name returns the name of the attribute.
     * @return  name of the attribute.
    */
    std::string Name() const {return name;}

    /**
     * Value returns the value of the attribute.
     * @return  value of the attribute.
    */
    std::string Value() const {return value;}

    /**
     * Value sets the value of the attribute.
     * @param value     new value of the attribute.
    */
    void        Value(std::string value) {this->value = value;}

    /**
     * ToText return a string representation of the attribute as: name="value".
     * @return string holding the string representation of the attribute, name="value".
    */
    std::string ToText() const
    {
        return name + "=\"" + value + "\"";
    }

    /**
     * Convenience operator<< for wrting the attribute.
     * @param stream    the output stream.
     * @param attribute the attibute to write to the stream.
     * @return          reference to the output stream after writing.
     * @see ToText().
    */
    friend std::ostream& operator<<(std::ostream &stream, const Attribute &attribute)
    {
        return stream << attribute.ToText();
    }
};

/**
 * class Transform handles a suite of 2D transforms.
*/
class Transform
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform
    std::vector<std::string>    transforms;
public:

    /**
     * Matrix performs a free transform defined by the specified matrix. It's structure is:
     * ```
     *  | x' |   | a c e |   | x |
     *  | y' | = | b d f | * | y |
     *  | 1  |   | 0 0 1 |   | 1 |
     * ```
    */
    Transform&  Matrix(double a, double b, double c, double d, double e, double f)
    {
        std::stringstream   stream;
        stream << "matrix(" << a << " " << b << " " << c << " " << d << " " << e << " " << f << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * Translate translates according to (dx,dy).
     * @param dx    the translation in x-direction.
     * @param dy    the translation in y-direction.
     * @return a reference to the Transform object.
     * @see Translate(const Point&).
    */
    Transform&  Translate(double dx, double dy)
    {
        std::stringstream   stream;
        stream << "translate(" << dx << " " << dy << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * Translate translates horisontally according to dx.
     * @param dx    the translation in x-direction.
     * @return a reference to the Transform object.
     * @see Translate(const Point&).
    */
    Transform&  Translate(double dx)
    {
        return Translate(dx, 0.0);
    }

    /**
     * Translate translates according to dp.
     * @param dp    point specifying the translation in x and y.
     * @return a reference to the Transform object.
     * @see Translate(double, double).
    */
    Transform&  Translate(const Point &dp)
    {
        return Translate(dp.X(), dp.Y());
    }

    /**
     * Scale scales according to (scale_x, scale_y).
     * @param scale_x   scale factor in x-direction.
     * @param scale_y   scale factor in y-direction.
     * @return a reference to the Transform object.
     * @see Scale(const Point&).
     * @see Scale(double).
    */
    Transform&  Scale(double scale_x, double scale_y)
    {
        std::stringstream   stream;
        stream << "scale(" << scale_x << " " << scale_y << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * Scale scales according to dp.
     * @param dp    point specifying the scaling in x and y.
     * @return a reference to the Transform object.
     * @see Scale(double, double).
     * @see Scale(double).
    */
    Transform&  Scale(const Point &scale)
    {
        return Scale(scale.X(), scale.Y());
    }

    /**
     * Scale scales according to scale.
     * @param scale     scale factor for both x and y directions.
     * @return a reference to the Transform object.
     * @see Scale(double, double).
     * @see Scale(const Point&).
    */
    Transform&  Scale(double scale)
    {
        return Scale(scale, scale);
    }

    /**
     * Rotate rotates angle around point (about_x, about_y).
     * @param angle     angle to rotate [degrees].
     * @param about_x   rotation center's x-value.
     * @param about_y   rotation center's y-value.
     * @return a reference to the Transform object.
     * @see Rotate(double, const Point&).
    */
    Transform&  Rotate(double angle, double about_x, double about_y)
    {
        std::stringstream   stream;
        stream << "rotate(" << angle << " " << about_x << " " << about_y << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * Rotate rotates angle around point about.
     * @param angle     angle to rotate [degrees].
     * @param about     rotation center.
     * @return a reference to the Transform object.
     * @see Rotate(double, double, double).
    */
    Transform&  Rotate(double angle, const Point &about)
    {
        return Rotate(angle, about.X(), about.Y());
    }

    /**
     * Rotate rotates angle around origin.
     * @param angle     angle to rotate [degrees].
     * @return a reference to the Transform object.
     * @see Rotate(double, double, double).
    */
    Transform&  Rotate(double angle)
    {
        std::stringstream   stream;
        stream << "rotate(" << angle << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * SkewX skews in the x-direction.
     * @param skew_x    skew value for the x-direction.
     * @return a reference to the Transform object.
     * @see SkewY(double).
    */
    Transform&  SkewX(double skew_x)
    {
        std::stringstream   stream;
        stream << "skewX(" << skew_x << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * SkewY skews in the y-direction.
     * @param skew_y    skew value for the y-direction.
     * @return a reference to the Transform object.
     * @see SkewX(double).
    */
    Transform&  SkewY(double skew_y)
    {
        std::stringstream   stream;
        stream << "skewY(" << skew_y << ')';
        transforms.push_back(stream.str());

        return *this;
    }

    /**
     * AsAttribute returns the Transform represented as an Attribute.
     * @returns an Attribute.
     * @see class Attribute.
    */
    Attribute   AsAttribute() const
    {
        std::stringstream   stream;

        // Reverse order: - should it be so???
        //for (size_t i = transforms.size(); i != 0; --i)
        ////for (const auto &t : transforms)
        //{
        //    stream << transforms[i-1];
        //    if (i != 1) stream << " ";  // if not the last, add a trailing separator space.
        //}

        bool is_first{true};
        for (const auto &t : transforms)
        {
            if (!is_first) stream << ' '; // if not the first, add a leading separator space.
            stream << t;

            is_first = false;
        }

        return {"transform", stream.str()};
    }
};

//-----------------------------------------------------------------------------
class AbstractBase
{
public:
    AbstractBase() = default;
    AbstractBase(const AbstractBase&) = default;
    AbstractBase(AbstractBase&&) = default;
    AbstractBase& operator=(const AbstractBase&) = default;
    AbstractBase& operator=(AbstractBase&&) = default;
    virtual ~AbstractBase() {}

    virtual std::string ToText() const = 0;

    friend std::ostream& operator<<(std::ostream &stream, const AbstractBase *base)
    {
        return stream << base->ToText();
    }

protected:
private:
};

template <typename SpecializedType>
class Base : public AbstractBase
{
    std::string             tag;
    std::vector<Attribute>  attributes;

    std::string             title;

protected:
    virtual std::string Extras() const {return {};}

public:
    Base() = default;
    Base(const Base&) = default;
    Base(Base&&) = default;
    Base& operator=(const Base&) = default;
    Base& operator=(Base&&) = default;

    Base(const std::string &tag)
        : AbstractBase(),
          tag(tag)
    {}
    Base(const std::string &tag, const std::vector<Attribute> &attributes)
        : AbstractBase(),
          tag(tag),
          attributes(attributes)
    {}

    virtual ~Base() {}

    virtual std::string     Tag() const {return tag;}
    const auto&     Attributes() const {return attributes;}

    SpecializedType&   AddAttribute(const Attribute &attribute)
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
        return static_cast<SpecializedType&>(*this);
    }

    SpecializedType&   AddAttributes(const std::vector<Attribute> &attributes)
    {
        for (const auto &a : attributes)
        {
            AddAttribute(a);
        }

        return static_cast<SpecializedType&>(*this);
    }

    auto&   Id(const std::string &id)
    {
        return AddAttribute({"id", id});
    }

    auto&   Class(const std::string &class_name)
    {
        return AddAttribute({"class", class_name});
    }

    auto&   Title(const std::string &title)
    {
        this->title = title;
        return static_cast<SpecializedType&>(*this);
    }

    auto&   Stroke(const std::string &stroke)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke
    {
        return AddAttribute({"stroke", stroke});
    }

    auto&   StrokeWidth(const double &stroke_width)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-width
    {
        return AddAttribute({"stroke-width", stroke_width});
    }

    auto&   StrokeOpacity(const double &stroke_opacity)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-opacity
    {
        return AddAttribute({"stroke-opacity", stroke_opacity});
    }

    auto&   Fill(const std::string &fill)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/fill
    {
        return AddAttribute({"fill", fill});
    }

    auto&   FillOpacity(const double &fill_opacity)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/fill-opacity
    {
        return AddAttribute({"fill-opacity", fill_opacity});
    }

    auto&   Opacity(const double &opacity)
    /// @see https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/opacity
    {
        return AddAttribute({"opacity", opacity});
    }

    auto&   Transform(const Transform &transform)
    {
        return AddAttribute(transform.AsAttribute());
    }

    virtual std::string ToText() const override
    {
        std::ostringstream  stream;
        stream << "<" << tag;                           // start tag.

        if (auto extras = Extras(); !extras.empty())
        {
            stream << ' ' << extras;
        }

        for (const auto &attribute : attributes)
        {
            stream << ' ' << attribute.ToText();
        }

        if (title.empty())
        {
            stream << "/>";                             // closing single-line tag.
        }
        else
        {
            stream << ">";                              // closing start tag.
            stream << "<title>" << title << "</title>";
            stream << "</" << tag << ">";               // end tag.
        }
        return stream.str();
    }

    //friend std::ostream& operator<<(std::ostream &stream, const Base<SpecializedType> &base)
    //{
    //    return stream << base.ToText();
    //}
};


class Rect : public Base<Rect>
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

    void    Set(double x, double y, double w, double h)
    {
        AddAttributes({{"x", x}, {"y", y}, {"width", w}, {"height", h}});
    }

    void    Set(double w, double h)
    {
        AddAttributes({{"width", w}, {"height", h}});
    }

    void    Set(const Point &from, const Point &to)
    {
        AddAttributes({{"x", from.X()}, {"y", from.Y()}, {"width", to.X() - from.X()}, {"height", to.Y() - from.Y()}});
    }
};

template <typename SpecializedType>
class PolyBase : public Base<SpecializedType>
{
    std::vector<Point> points;

protected:
    virtual std::string Extras() const override
    {
        std::ostringstream  stream;

        for (const auto &p : points)
        {
            stream << p.ToText() << " ";
        }

        return Attribute("points", stream.str()).ToText();
    }

public:
    PolyBase(std::string tag) : Base<SpecializedType>(tag) {}
    PolyBase(std::string tag, const std::vector<Point> &points)
        : Base<SpecializedType>(tag),
          points(points)
    {}
    virtual ~PolyBase() override {}

    auto&   Add(const Point &point)
    {
        points.push_back(point);
        return static_cast<SpecializedType&>(*this);
    }

    auto&   Add(double x, double y)
    {
        return Add({x, y});
    }

    auto&   Add(const std::vector<Point> &points)
    {
        for (const auto &p : points)
        {
            Add(p);
        }

        return static_cast<SpecializedType&>(*this);
    }
};

class Polyline : public PolyBase<Polyline>
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

class Polygon : public PolyBase<Polygon>
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

class Path : public Base<Path>
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Paths

    std::vector<std::string>    parts;
    virtual std::string Extras() const override
    {
        std::ostringstream  stream;
        stream << "d=\"";

        bool    initial{true};
        for (const auto &p : parts)
        {
            if (!initial)
            {
                stream << " " << p;
            }
            else
            {
                stream << p;
                initial = false;
            }
        }

        stream << '\"';

        return stream.str();
    }

public:
    Path(const Path&) = default;
    Path(Path&&) = default;
    Path& operator=(const Path&) = default;
    Path& operator=(Path&&) = default;

    Path() : Base("path") {}
    virtual ~Path() override {}

    Path&   MoveTo(const Point &p, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "m" : "M")
               << " " << p.X()
               << " " << p.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   MoveAbsolute(const Point &p)
    {
        return MoveTo(p, false);
    }

    Path&   MoveRelative(const Point &p)
    {
        return MoveTo(p, true);
    }

    Path&   LineTo(const Point &p, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "l" : "L")
               << " " << p.X()
               << " " << p.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   LineAbsolute(const Point &p)
    {
        return LineTo(p, false);
    }

    Path&   LineRelative(const Point &p)
    {
        return LineTo(p, true);
    }

    Path&   HorizontalLineTo(double x, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "h" : "H")
               << " " << x
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   HorizontalLineAbsolute(double x)
    {
        return HorizontalLineTo(x, false);
    }

    Path&   HorizontalLineRelative(double x)
    {
        return HorizontalLineTo(x, true);
    }

    Path&   VerticalLineTo(double y, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "v" : "V")
               << " " << y
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   VerticaLineAbsolute(double y)
    {
        return VerticalLineTo(y, false);
    }

    Path&   VerticaLineRelative(double y)
    {
        return VerticalLineTo(y, true);
    }

    Path&   Close()
    {
        parts.push_back("Z");
        return *this;
    }

    Path&   Cubic(const Point &p_c1, const Point &p_c2, const Point &p_end, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "c" : "C")
               << " " << p_c1.X()
               << " " << p_c1.Y()
               << "," << p_c2.X()
               << " " << p_c2.Y()
               << "," << p_end.X()
               << " " << p_end.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   CubicAbsolute(const Point &p_c1, const Point &p_c2, const Point &p_end)
    {
        return Cubic(p_c1, p_c2, p_end, false);
    }

    Path&   CubicRelative(const Point &p_c1, const Point &p_c2, const Point &p_end)
    {
        return Cubic(p_c1, p_c2, p_end, true);
    }

    Path&   Stitch(const Point &p_c2, const Point &p_end, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "s" : "S")
               << " " << p_c2.X()
               << " " << p_c2.Y()
               << "," << p_end.X()
               << " " << p_end.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   StitchAbsolute(const Point &p_c2, const Point &p_end)
    {
        return Stitch(p_c2, p_end, false);
    }

    Path&   StitchRelative(const Point &p_c2, const Point &p_end)
    {
        return Stitch(p_c2, p_end, true);
    }

    Path&   Quadratic(const Point &p_c, const Point &p_end, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "q" : "Q")
               << " " << p_c.X()
               << " " << p_c.Y()
               << "," << p_end.X()
               << " " << p_end.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   QuadraticAbsolute(const Point &p_c, const Point &p_end)
    {
        return Quadratic(p_c, p_end, false);
    }

    Path&   QuadraticRelative(const Point &p_c, const Point &p_end)
    {
        return Quadratic(p_c, p_end, true);
    }

    Path&   Stitch(const Point &p_end, bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "t" : "T")
               << " " << p_end.X()
               << " " << p_end.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   StitchAbsolute(const Point &p_end)
    {
        return Stitch(p_end, false);
    }

    Path&   StitchRelative(const Point &p_end)
    {
        return Stitch(p_end, true);
    }

    Path&   Arch(double radius_x,
                 double radius_y,
                 double x_axis_rotation,
                 bool large_arc_flag,
                 bool sweep_flag,
                 const Point &p_end,
                 bool relative = false)
    {
        std::ostringstream  stream;
        stream << (relative ? "a" : "A")
               << " " << radius_x
               << " " << radius_y
               << " " << x_axis_rotation
               << " " << (large_arc_flag ? "1" : "0")
               << " " << (sweep_flag ? "1" : "0")
               << " " << p_end.X()
               << " " << p_end.Y()
                  ;
        parts.push_back(stream.str());
        return *this;
    }

    Path&   ArchAbsolute(double radius_x,
                         double radius_y,
                         double x_axis_rotation,
                         bool large_arc_flag,
                         bool sweep_flag,
                         const Point &p_end)
    {
        return Arch(radius_x, radius_y, x_axis_rotation, large_arc_flag, sweep_flag, p_end, false);
    }

    Path&   ArchRelative(double radius_x,
                         double radius_y,
                         double x_axis_rotation,
                         bool large_arc_flag,
                         bool sweep_flag,
                         const Point &p_end)
    {
        return Arch(radius_x, radius_y, x_axis_rotation, large_arc_flag, sweep_flag, p_end, true);
    }
};

class Line : public Base<Line>
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

class Circle : public Base<Circle>
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

class Ellipse : public Base<Ellipse>
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

class Use : public Base<Use>
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
template <typename SpecializedType>
class GroupBase : public Base<SpecializedType>
{
    //std::vector<std::shared_ptr<Base<SpecializedType>>>  objects;
    std::vector<std::shared_ptr<AbstractBase>>  objects;

protected:
    std::string StartTag() const
    {
        std::ostringstream  stream;
        stream << "<" << this->Tag();

        for (const auto &attribute : this->Attributes())
        {
            stream << ' ' << attribute.ToText();
        }
        stream << ">";

        return stream.str();
    }

    std::string EndTag() const
    {
        std::ostringstream  stream;
        stream << "</" << this->Tag() << ">";

        return stream.str();
    }

public:
    GroupBase(const GroupBase&) = default;
    GroupBase(GroupBase&&) = default;
    GroupBase& operator=(const GroupBase&) = default;
    GroupBase& operator=(GroupBase&&) = default;

    GroupBase(std::string group_tag)
        : Base<SpecializedType>(group_tag) {}
    GroupBase(std::string group_tag, const std::vector<Attribute> &attributes)
        : Base<SpecializedType>(group_tag, attributes) {}
    virtual ~GroupBase() override {}

    /**
     * Append appends an SVG object to the group base.
     * @param object    a SVG object, e.g. simple_svg::Rect.
     * @return          a reference to the internal version of the appended object.
    */
    template<typename T>
    auto&  Append(const T& object)
    {
        objects.push_back(std::make_shared<T>(object));
        //return static_cast<SpecializedType&>(*this);
        return static_cast<T&>(*objects.back());
    }

    virtual std::string ToText() const override
    {
        std::ostringstream  stream;
        stream << StartTag() << '\n';

        for (const auto &object : objects)
        {
            //stream << "  " << object->ToText() << '\n';
            stream << "  " << object << '\n';
        }

        stream << EndTag();

        return stream.str();
    }

    //friend std::ostream& operator<<(std::ostream &stream, const GroupBase &group_base)
    //{
    //    return stream << group_base.ToText();
    //}
};

class Text : public GroupBase<Text>
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

    Text&   SetTextBuffer(std::string text) {this->text = text; return *this;}
    std::string&   GetTextBuffer() {return text;}

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

class Group : public GroupBase<Group>
{
    // https://developer.mozilla.org/en-US/docs/Web/SVG/Element/g
public:
    Group(const Group&) = default;
    Group(Group&&) = default;
    Group& operator=(const Group&) = default;
    Group& operator=(Group&&) = default;

    Group() : GroupBase("g") {}
    virtual ~Group() override {}

    //friend std::ostream& operator<<(std::ostream &stream, const Group &group)
    //{
    //    return stream << group.ToText();
    //}
};

class Layer : public GroupBase<Layer>
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

    //friend std::ostream& operator<<(std::ostream &stream, const Layer &group)
    //{
    //    return stream << group.ToText();
    //}
};

class Document : public GroupBase<Document>
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

        stream << GroupBase<Document>::ToText();

        return stream.str();
    }

    friend std::ostream& operator<<(std::ostream &stream, const Document &document)
    {
        return stream << document.ToText();
    }
};

} // namespace simple_svg
