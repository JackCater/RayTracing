#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class camera
{
private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;

public:
	/// <summary>
	/// Creates a camera object from one point in 3D space, looking at another point in 3D space
	/// </summary>
	/// <param name="lookfrom">The 3D co-ordinates where the camera is</param>
	/// <param name="lookat">The 3D co-ordinates where the camera is pointing towards</param>
	/// <param name="vup">The up vector</param>
	/// <param name="vfov">Vertical field of view in degrees</param>	
	/// <param name="aspect_ratio">The aspect ratio</param>
	/// <param name="aperture">The aperture</param>	
	/// <param name="focus_dist">The focus distance</param>
	camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		auto w = unit_vector(lookfrom - lookat);
		auto u = unit_vector(cross(vup, w));
		auto v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w * focus_dist;

		lens_radius = aperture / 2;
	}

	ray get_ray(double s, double t) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}
};

#endif