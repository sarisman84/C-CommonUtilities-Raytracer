#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include <ppl.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#undef _CRT_SECURE_NO_WARNINGS

#include "CScene.h"
#include "Util.h"

// Enable to run raytracing in parallel
#define RUN_IN_PARALLEL

int main(int argc, char* argv[])
{
	const int width = 800;
	const int height = 600;


	CScene scene(width, height);

	std::string filename = "uppgift5_scen.txt";
	if (argc > 1)
		filename = argv[1];

	std::cout << "Loading scene: \"" << filename << "\"\n";

	scene.Load(filename.c_str());

	uint8_t* pixels = new uint8_t[width * height * 3];

	std::cout << "Rendering...\n";

#ifdef RUN_IN_PARALLEL
	concurrency::parallel_for(0, height, [&](int j)
#else
	for (int j = height - 1; j >= 0; --j)
#endif
	{
		for (int i = 0; i < width; ++i)
		{
			int index = 3 * (j * width + i);

			SRGB color = scene.Raytrace(i, height - 1 - j);
			color = ToneMap(color);

			int ir = int(255.99 * LinearToSrgb(fmin(color.r, 1.f)));
			int ig = int(255.99 * LinearToSrgb(fmin(color.g, 1.f)));
			int ib = int(255.99 * LinearToSrgb(fmin(color.b, 1.f)));

			pixels[index + 0] = ir;
			pixels[index + 1] = ig;
			pixels[index + 2] = ib;
		}

		

#ifdef RUN_IN_PARALLEL
	});
#else
}
#endif

	std::string imageFilename = filename.substr(0, filename.find_last_of('.')) + ".png";

	std::cout << "Writing image: \"" << imageFilename << "\"\n";

	stbi_write_png(imageFilename.c_str(), width, height, 3, pixels, width * 3);
	delete[] pixels;

	return 0;
}
