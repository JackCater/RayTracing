#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "vec3.h"

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const = 0;
};

/// <summary>
/// Lambertian/Diffuse material
/// </summary>
class lambertian : public material {
public:
	colour albedo;

public:
	lambertian(const colour& col) : albedo(col) {}

	virtual bool scatter(const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero()) scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
public:
	colour albedo;
	double fuzz;

public:
	metal(const colour& col, double fuzz) : albedo(col), fuzz(fuzz) {}

	virtual bool scatter(const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return(dot(scattered.direction(), rec.normal) > 0);
	}
};

class dielectric : public material {
private:
	static double reflectance(double cosine, double ref_idx) {
		// Uses Schlick's approximation for reflectance
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

public:
	double refractive_index; // Refractive index

public:
	dielectric(double refractive_index) : refractive_index(refractive_index) {}

	virtual bool scatter(const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
		attenuation = colour(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / refractive_index) : refractive_index;

		vec3 unit_direction = unit_vector(ray_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) direction = reflect(unit_direction, rec.normal);
		else direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, direction);
		return true;
	}
};

#endif