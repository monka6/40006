#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <iterator>
#include <algorithm>
#include "Polygon.h"
#include "Functions.h"
#include "Functors.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: incorrect file\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }
    std::vector<Polygon> polygons;
    while (!file.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(file),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );

        if (file.fail() && !file.eof()) {
            file.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string command;
    std::cout << std::fixed << std::setprecision(1);
    while (std::cin >> command) {
        std::cin.clear();
        if (command == "AREA") {
            std::string arg;
            if (!(std::cin >> arg)) {
                invalidCommand();
                continue;
            }
            if (arg == "ODD" || arg == "EVEN") {
                std::cout << areaEvenOdd(arg, polygons) << '\n';
            }
            else if (arg == "MEAN") {
                if (isEmpty(polygons)) {
                    invalidCommand();
                }
                else {
                    std::cout << areaMean(polygons) << '\n';
                }
            }
            else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
                int num = std::stoi(arg);
                if (num >= 3) {
                    std::cout << areaNumOfVertexes(num, polygons) << '\n';
                }
                else {
                    invalidCommand();
                }
            }
            else {
                invalidCommand();
            }
        }
        else if (command == "MAX" || command == "MIN") {
            if (isEmpty(polygons)) {
                invalidCommand();
                continue;
            }
            std::string arg;
            if (!(std::cin >> arg)) {
                invalidCommand();
                continue;
            }
            if (arg == "AREA" || arg == "VERTEXES") {
                double result = (command == "MAX")
                    ? maxAreaVertexes(arg, polygons)
                    : minAreaVertexes(arg, polygons);
                std::cout << (arg == "VERTEXES" ? static_cast<int>(result) : result) << '\n';
            }
            else {
                invalidCommand();
            }
        }
        else if (command == "COUNT") {
            std::string arg;
            if (!(std::cin >> arg)) {
                invalidCommand();
                continue;
            }
            if (arg == "EVEN" || arg == "ODD") {
                std::cout << countEvenOdd(arg, polygons) << '\n';
            }
            else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
                int num = std::stoi(arg);
                if (num >= 3) {
                    std::cout << countVertexes(num, polygons) << '\n';
                }
                else {
                    invalidCommand();
                }
            }
            else {
                invalidCommand();
            }
        }
        else if (command == "RECTS") {
            std::cout << countRects(polygons) << '\n';
        }
        else if (command == "INTERSECTIONS") {
            Polygon polygon;
            if (!(std::cin >> polygon) || polygon.points.size() < 3) {
                invalidCommand();
            }
            else {
                std::cout << countIntersectingPolygons(polygons, polygon) << '\n';
            }
        }
        else {
            invalidCommand();
        }
    }
    return 0;
}

