#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
private:
	point3 centre;
	double radius;
	std::shared_ptr<material> mat_ptr;

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	sphere() {}

	/// <summary>
	/// Default constructor
	/// </summary>
	/// <param name="cen">3D centre point</param>
	/// <param name="r">Radius</param>
	/// <param name="m">Material</param>
	sphere(point3 cen, double r, std::shared_ptr<material> m) :centre(cen), radius(r), mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - centre;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;

	auto sqrtd = sqrt(discriminant); // square root of discriminant

	// Find the nearest root thet lies in the acceptable range
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) return false;

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - centre) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}
#endif