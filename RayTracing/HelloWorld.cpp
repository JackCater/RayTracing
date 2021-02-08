#include <iostream>

int HelloWorld(void)
{
	// Image 

	const int nx = 256; // Width
	const int ny = 256; // Height

	// Render

	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < nx; ++i) {
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.25f;
			int ir = static_cast<int>(255.99 * r);
			int ig = static_cast<int>(255.99 * g);
			int ib = static_cast<int>(255.99 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		} // end for 
	} // end for

	std::cerr << "\nDone.\n";
	return 0;
}