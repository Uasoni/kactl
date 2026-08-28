/**
 * Author: ulf lundstrom with inspiration from tiny_kactl
 * Date: 2009-04-14
 * License: CC0
 * Source:
 * Description: class to handle points in 3D space.
 * 	T can be e.g. double or long long.
 * Usage:
 * Status: tested, except for phi and theta
 */
#pragma once

template<class T> struct Point3D {
	typedef const Point3D& point_reference;
	T x, y, z;
	explicit Point3D(T x=0, T y=0, T z=0) : x(x), y(y), z(z) {}
	bool operator<(point_reference p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z); }
	bool operator==(point_reference p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z); }
	Point3D operator+(point_reference p) const { return Point3D(x+p.x, y+p.y, z+p.z); }
	Point3D operator-(point_reference p) const { return Point3D(x-p.x, y-p.y, z-p.z); }
	Point3D operator*(T d) const { return Point3D(x*d, y*d, z*d); }
	Point3D operator/(T d) const { return Point3D(x/d, y/d, z/d); }
	T dot(point_reference p) const { return x*p.x + y*p.y + z*p.z; }
	Point3D cross(point_reference p) const {
		return Point3D(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
	}
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
	//azimuthal Angle (longitude) to x-axis in interval [-pi, pi]
	double phi() const { return atan2(y, x); } 
	//zenith Angle (latitude) to the z-axis in interval [0, pi]
	double theta() const { return atan2(sqrt(x*x+y*y),z); }
	Point3D unit() const { return *this/(T)dist(); } //makes dist()=1
	//returns unit vector normal to *this and p
	Point3D normal(Point3D p) const { return cross(p).unit(); }
	//returns Point rotated 'Angle' radians ccw around axis
	Point3D rotate(double angle, Point3D axis) const {
		double s = sin(angle), c = cos(angle); Point3D u = axis.unit();
		return u*dot(u)*(1-c) + (*this)*c - cross(u)*s;
	}
};
