#pragma once
#define _USE_MATH_DEFINES
#include <utility>
#include <cmath>
#include <math.h>
#include <exception>
#include <sstream>
#include "Base.h"
#include "Container.h"
using namespace std;
class Figure : public Printable {
    static size_t count;

public:
    Figure() {
        ++count;
    }

    Figure(const Figure &other) {
        count++;	
    }

    virtual ~Figure() {
        count--;
    }

    static size_t getCount() {
        return count;
    }
};

size_t Figure::count = 0;
//точка
class Point : public Figure {
	double x1;
	double y1;

public:
	Point(const double &x, const double &y) : x1(x), y1(y) {}

	double x() const {
		return x1;
	}

	double y() const {
		return y1;
	}

	double distance(const Point &other) {
		double x = x1 - other.x();
		double y = y1 - other.y();
		return sqrt(x * x + y * y);
	}

	string toString() const override {
		return "(" + to_string(x1) + "; " + to_string(y1) + ")";
	}
};


//многоульник
class Polygon : public Figure {
	Container<Point> points_;

public:
	Polygon() = default;

	void addPoint(const Point &point) {
		points_.push_back(point);
	}

	string toString() const override {
		if (points_.empty()) {
			return "Empty polygon";
		}
		stringstream ss;
		ss << "Polygon: " << points_.front();
		for (auto i = ++points_.cbegin(); i != points_.cend(); ++i) 
			ss << ", " << *i;

		return ss.str();
	}
};
//окружность
class Circle : public Figure {
    Point Center;
    double Radius;

public:

    Circle(Point center, const double &radius) : Center(move(center)), Radius(radius) {
        if (Radius <= 0) {
			throw runtime_error("Radius not possitive");
        }
    }

	double radius() const {
		return Radius;
	}

    Point center() const {
        return Center;
    }
	double perimeter() const {
		return 2 * M_PI * Radius;
	}

	double square() const {
		return M_PI * Radius * Radius;
	}

 
    
    string toString() const override {
        return "Circle: center at " + Center.toString() + ", radius = " + to_string(Radius);
    }
};


class Rect : public Figure {
    Point topLeft;
    Point bottomRight_;

public:

    Rect(Point topLeft, const Point &bottomRight) : topLeft(move(topLeft)), bottomRight_(bottomRight) {
        if (topLeft.x() >= bottomRight.x()) {
			throw runtime_error("must be passed constructor");
        }
        if (topLeft.y() <= bottomRight.y()) {
			throw runtime_error("must be passed constructor"); 
        }
    }

    Rect(Point topLeft, double width, double height) :
            topLeft(move(topLeft)),
            bottomRight_({topLeft.x() + width, topLeft.y() - height}) {
        if (width < 0 || height < 0) {
			throw runtime_error("data must be positive");
        }
    };

    double width() const {
        return bottomRight_.x() - topLeft.x();
    }

    double height() const {
        return topLeft.y() - bottomRight_.y();
    }

    double square() const {
        return width() * height();
    }

    double perimeter() const {
        return 2 * (width() + height());
    }

    string toString() const override {
        return "Rectangle top " + topLeft.toString()
               + " and bottom right point at " + bottomRight_.toString();
    }
};


class Square : public Figure {
    Point topLeft;
    double sideLength_;

public:
  

    Square(Point topLeft, const double &sideLength) : topLeft(move(topLeft)), sideLength_(sideLength) {
        if (sideLength_ < 0) {
			throw runtime_error(" must be positive"); 
        }
    }

    double side() const {
        return sideLength_;
    }

    double square() const {
        return sideLength_ * sideLength_;
    }

    double perimeter() const {
        return 4 * sideLength_;
    }

    string toString() const override {
        return "Square with top left point at " + topLeft.toString()
               + " and side length = " + to_string(sideLength_);
    }
};


class Polyline : public Figure {
    Container<Point> points_;
    double length_;

public:
    Polyline() : length_(0) {};

    void addPoint(const Point &point) {
        if (!points_.empty()) {
            length_ += points_.back().distance(point);
        }
        points_.push_back(point);
    }

    double length() const {
        return length_;
    }

    string toString() const override {
        if (points_.empty()) {
            return "Empty polyline";
        }
        stringstream ss;
        ss << "Polyline: length = " << this->length() << " and points :" << this->points_.front();
        for (auto i = ++points_.cbegin(); i != points_.cend(); ++i) {
            ss << ", " << *i;
        }

        return ss.str();
    }
};


