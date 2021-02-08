/*
* Almost all graphics programs have some class(es) for storing geometric vectors and colors. 
* In many systems these vectors are 4D (3D plus a homogeneous coordinate for geometry, and RGB plus an alpha transparency channel for colors). 
* For our purposes, three coordinates suffices. We’ll use the same class vec3 for colors, locations, directions, offsets, whatever. 
* Some people don’t like this because it doesn’t prevent you from doing something silly, like adding a color to a location. 
* They have a good point, but we’re going to always take the “less code” route when not obviously wrong. 
* In spite of this, we do declare two aliases for vec3: point3 and color. Since these two types are just aliases for vec3,
* you won't get warnings if you pass a color to a function expecting a point3, for example. We use them only to clarify intent and use.
*/

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3
{
public:
	double e[3]; // Array containing X, Y and Z

public:
	vec3() : e{ 0,0,0 } {}; // Default constructor
	vec3(double e0, double e1, double e2) : e{ e0,e1,e2 } {} // Modified contructor

	double x() const { return e[0]; } // GetX
	double y() const { return e[1]; } // GetY
	double z() const { return e[2]; } // GetZ

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) { return *this *= 1.0 / t; }

	double length() const { return std::sqrt(length_squared()); }

	double length_squared()const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

	/// <summary>
	/// If the random unit vector generated is exactly opposite the normal vector the sum will be 0
	/// </summary>
	/// <returns>True if the vector is close to zero in all directions</returns>
	bool near_zero() const {
		const auto s = 1e-8;
		return(fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	inline static vec3 random() { return vec3(random_double(), random_double(), random_double()); }

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));	}
};

	// Type aliases for vec3
	using point3 = vec3;   // 3D point
	using colour = vec3;    // RGB colour



// vec3 Utility Functions
	inline std::ostream& operator<<(std::ostream& out, const vec3& v) { return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2]; }
	inline vec3 operator+(const vec3& u, const vec3& v) { return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]); }
	inline vec3 operator-(const vec3& u, const vec3& v) { return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]); }
	inline vec3 operator*(const vec3& u, const vec3& v) { return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]); }
	inline vec3 operator*(double t, const vec3& v) { return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); }
	inline vec3 operator*(const vec3& v, double t) { return t * v; }
	inline vec3 operator/(vec3 v, double t) { return (1 / t) * v; }
	inline double dot(const vec3& u, const vec3& v) {
		return	u.e[0] * v.e[0]
			+ u.e[1] * v.e[1]
			+ u.e[2] * v.e[2];
	}

	inline vec3 cross(const vec3& u, const vec3& v) {
		return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
			u.e[2] * v.e[0] - u.e[0] * v.e[2],
			u.e[0] * v.e[1] - u.e[1] * v.e[0]);
	}

	inline vec3 unit_vector(vec3 v) { return v / v.length(); }

	inline vec3 random_in_unit_sphere() {
		while (true) {
			auto p = vec3::random(-1, 1);
			if (p.length_squared() >= 1) continue;
			return p;
		} // end while
	}

	inline vec3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); } // True Lambertian Reflection

	inline vec3 random_in_hemisphere(const vec3& normal) {
		vec3 in_unit_sphere = random_in_unit_sphere();
		if (dot(in_unit_sphere, normal) > 0.0) return in_unit_sphere;
		else return -in_unit_sphere;
	}

	vec3 random_in_unit_disk() {
		while (true) {
			auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
			if (p.length_squared() >= 1)continue;
			return p;
		}
	}

	vec3 reflect(const vec3& unit_vector, const vec3& n) { return unit_vector - 2 * dot(unit_vector, n) * n; }

	vec3 refract(const vec3& unit_vector, const vec3& n, double etai_over_etat) {
		auto cos_theta = fmin(dot(-unit_vector, n), 1.0);
		vec3 r_out_perp = etai_over_etat * (unit_vector + cos_theta * n);
		vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
		return r_out_perp + r_out_parallel;
	}

#endif
