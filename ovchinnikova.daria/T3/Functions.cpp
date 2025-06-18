#include "Functions.h"
#include "Functors.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>

std::istream& operator>>(std::istream& in, Point& point) {
    char ch1, ch2, ch3;
    in >> ch1 >> point.x >> ch2 >> point.y >> ch3;
    if (ch1 != '(' || ch2 != ';' || ch3 != ')') {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon) {
    polygon.points.clear();
    int count;
    if (!(in >> count) || (count < 3)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < count; ++i) {
        if (in.peek() == '\n') {
            in.setstate(std::ios::failbit);
            return in;
        }
        Point point;
        if (!(in >> point)) {
            return in;
        }
        polygon.points.push_back(point);
    }
    if (in.peek() != EOF && in.peek() != '\n') {
        in.setstate(std::ios::failbit);
    }
    return in;
}

void invalidCommand() {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isEmpty(std::vector<Polygon>& data) {
    return data.empty();
}

double computeArea(std::vector<Point>& points) {
    double area = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % points.size()];
        area += (p1.x * p2.y) - (p1.y * p2.x);
    }
    return std::abs(area) / 2.0;
}

double areaEvenOdd(std::string& arg, std::vector<Polygon>& data) {
    bool isEven = (arg == "EVEN");
    return std::accumulate(data.begin(), data.end(), 0.0,
        [isEven](double sum, Polygon& poly) {
            bool condition = (poly.points.size() % 2 == 0);
            return (isEven == condition) ? sum + computeArea(poly.points) : sum;
        });
}

double areaMean(std::vector<Polygon>& data) {
    if (data.empty()) {
        return 0.0;
    }
    double total = std::accumulate(data.begin(), data.end(), 0.0,
        [](double sum, Polygon& poly) {
            return sum + computeArea(poly.points);
        });
    return total / data.size();
}

double areaNumOfVertexes(size_t arg, std::vector<Polygon>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0,
        [arg](double sum, Polygon& poly) {
            return (poly.points.size() == static_cast<size_t>(arg)) ? sum + computeArea(poly.points) : sum;
        });
}

double maxAreaVertexes(std::string& arg, std::vector<Polygon>& data) {
    if (arg == "AREA") {
        auto total = std::max_element(data.begin(), data.end(), AreaComparator());
        return computeArea(total->points);
    }
    else if (arg == "VERTEXES") {
        auto total = std::max_element(data.begin(), data.end(), VertexCountComparator());
        return total->points.size();
    }
    return 0.0;
}

double minAreaVertexes(std::string& arg, std::vector<Polygon>& data) {
    if (arg == "AREA") {
        auto total = std::min_element(data.begin(), data.end(), AreaComparator());
        return computeArea(total->points);
    }
    else if (arg == "VERTEXES") {
        auto total = std::min_element(data.begin(), data.end(), VertexCountComparator());
        return total->points.size();
    }
    return 0.0;
}

int countEvenOdd(std::string arg, std::vector<Polygon>& data) {
    bool isEven = (arg == "EVEN");
    return std::count_if(data.begin(), data.end(),
        [isEven](Polygon& poly) {
            return (poly.points.size() % 2 == 0) == isEven;
        });
}

int countVertexes(size_t arg, std::vector<Polygon>& data) {
    return std::count_if(data.begin(), data.end(),
        [arg](Polygon& poly) {
            return poly.points.size() == arg;
        });
}

bool isRightAngle(const Point& a, const Point& b, const Point& c) {
    int ba_x = a.x - b.x;
    int ba_y = a.y - b.y;
    int bc_x = c.x - b.x;
    int bc_y = c.y - b.y;
    return (ba_x * bc_x + ba_y * bc_y) == 0;
}

bool isRect(const Polygon& polygon) {
    if (polygon.points.size() != 4) {
        return false;
    }
    return isRightAngle(polygon.points[0], polygon.points[1], polygon.points[2]) &&
        isRightAngle(polygon.points[1], polygon.points[2], polygon.points[3]) &&
        isRightAngle(polygon.points[2], polygon.points[3], polygon.points[0]);
}

int countRects(std::vector<Polygon>& data) {
    return std::count_if(data.begin(), data.end(), isRect);
}

int orientation(const Point& a, const Point& b, const Point& point) {
    int pos = (b.x - a.x) * (point.y - a.y) - (b.y - a.y) * (point.x - a.x);
    if (pos == 0) {
        return 0;
    }
    return (pos > 0) ? -1 : 1;
}

bool pointOnSegment(const Point& a, const Point& b, const Point& point) {
    return b.x >= std::min(a.x, point.x) && b.x <= std::max(a.x, point.x)
        && b.y >= std::min(a.y, point.y) && b.y <= std::max(a.y, point.y);
}

bool intersectionOfSeg(const Point& a1, const Point& b1, const Point& a2, const Point& b2) {
    int orient1 = orientation(a1, b1, a2);
    int orient2 = orientation(a1, b1, b2);
    int orient3 = orientation(a2, b2, a1);
    int orient4 = orientation(a2, b2, b1);
    if (orient1 * orient2 < 0 && orient3 * orient4 < 0) {
        return true;
    }
    else if ((orient1 == 0 && pointOnSegment(a1, a2, b1)) || (orient2 == 0 && pointOnSegment(a1, b2, b1)) ||
        (orient3 == 0 && pointOnSegment(a2, a1, b2)) || (orient4 == 0 && pointOnSegment(a2, b1, b2))) {
        return true;
    }
    else {
        return false;
    }
}

bool pointInPolygon(const Point& point, const Polygon& polygon) {
    bool isInside = false;
    const auto& points = polygon.points;
    int vertexes = points.size();
    if (vertexes < 3) {
        return false;
    }

    for (int i = 0, j = vertexes - 1; i < vertexes; j = i++) {
        if (((points[i].y > point.y) != (points[j].y > point.y)) &&
            (point.x < (points[j].x - points[i].x) * (point.y - points[i].y)
                / (points[j].y - points[i].y) + points[i].x)) {
            isInside = !isInside;
        }
    }
    return isInside;
}

bool polygonsIntersect(const Polygon& a, const Polygon& b) {
    auto checkSideIntersection = std::bind(
        intersectionOfSeg,
        std::placeholders::_1, std::placeholders::_2,
        std::placeholders::_3, std::placeholders::_4
    );

    int na = a.points.size();
    int nb = b.points.size();
    for (int i = 0; i < na; ++i) {
        const Point& a1 = a.points[i];
        const Point& a2 = a.points[(i + 1) % na];
        for (int j = 0; j < nb; ++j) {
            const Point& b1 = b.points[j];
            const Point& b2 = b.points[(j + 1) % nb];
            if (checkSideIntersection(a1, a2, b1, b2)) {
                return true;
            }
        }
    }
    if (!a.points.empty() && !b.points.empty()) {
        if (pointInPolygon(a.points[0], b) || pointInPolygon(b.points[0], a)) {
            return true;
        }
    }
    return false;
}

int countIntersectingPolygons(const std::vector<Polygon>& polygons,
    const Polygon& target) {
    return std::count_if(polygons.begin(), polygons.end(),
        [&target](const Polygon& poly) {
            return polygonsIntersect(poly, target);
        });
}


