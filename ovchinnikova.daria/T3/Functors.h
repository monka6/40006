#ifndef FUNCTORS_H_
#define FUNCTORS_H_
#include "Polygon.h"
#include "Functions.h"



struct VertexCountComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return a.points.size() < b.points.size();
    }
};

struct AreaComparator {
    bool operator()(Polygon& a, Polygon& b) const {
        return computeArea(a.points) < computeArea(b.points);
    }
};

#endif