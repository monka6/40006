#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <vector>
#include <sstream>
#include "Polygon.h"

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);

void invalidCommand();
bool isEmpty(std::vector<Polygon>& data);

double computeArea(std::vector<Point>& points);
double areaEvenOdd(std::string& arg, std::vector<Polygon>& data);
double areaMean(std::vector<Polygon>& data);
double areaNumOfVertexes(size_t arg, std::vector<Polygon>& data);
double maxAreaVertexes(std::string& arg, std::vector<Polygon>& data);
double minAreaVertexes(std::string& arg, std::vector<Polygon>& data);
int countEvenOdd(std::string arg, std::vector<Polygon>& data);
int countVertexes(size_t arg, std::vector<Polygon>& data);
bool isRightAngle(const Point& a, const Point& b, const Point& c);
int countRects(std::vector<Polygon>& data);
int orientation(const Point& p, const Point& q, const Point& r);
bool pointOnSegment(const Point& p, const Point& q, const Point& r);
bool intersectionOfSeg(const Point& a1, const Point& a2, const Point& b1, const Point& b2);
bool pointInPolygon(const Point& point, const Polygon& polygon);
bool polygonsIntersect(const Polygon& a, const Polygon& b);
int countIntersectingPolygons(const std::vector<Polygon>& a, const Polygon& b);


#endif

