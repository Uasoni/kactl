/**
 * Author: ulf lundstrom
 * Date: 2009-02-26
 * License: CC0
 * Source: my head with inspiration from tiny_kactl
 * Description: class to handle points in the plane.
 * 	T can be e.g. double or long long. (avoid int.)
 * Status: works fine, used a lot
 */
#pragma once

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(Point p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(Point p) const { return tie(x,y)==tie(p.x,p.y); }
	Point operator+(Point p) const { return Point(x+p.x, y+p.y); }
	Point operator-(Point p) const { return Point(x-p.x, y-p.y); }
	Point operator*(T d) const { return Point(x*d, y*d); }
	Point operator/(T d) const { return Point(x/d, y/d); }
	T dot(Point p) const { return x*p.x + y*p.y; }
	T cross(Point p) const { return x*p.y - y*p.x; }
	T cross(Point a, Point b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// Angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	Point unit() const { return *this/dist(); } // makes dist()=1
	Point perp() const { return Point(-y, x); } // rotates +90 degrees
	Point normal() const { return perp().unit(); }
	// returns Point rotated 'a' radians ccw around the origin
	Point rotate(double a) const {
		return Point(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, Point p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};
