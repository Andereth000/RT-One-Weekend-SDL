/******************************************************************************
	All code adapted from "Ray Tracing In One Weekend" by Peter Shirley
    Reference: https://raytracing.github.io/
	Edited by: Ethan Anderson
*******************************************************************************/

#include <iostream>
#include <cmath>
#include <SDL.h>
#include <chrono>
#include <random>
#include <vector>
// Multithreading
#include <algorithm>
#include <execution>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
#include "utility"
#include "material.h"

hitable* original_scene();
hitable* random_scene();

int image_width = 800;
int image_height = 400;

vec3 lookfrom(3, 3, 2);
vec3 lookat(0, 0, -1);
float dist_to_focus = (lookfrom - lookat).length();
float aperture = 0.0;
camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(image_width) / float(image_height), aperture, dist_to_focus);

vec3 color(const ray& r, hitable* world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

void render_scene(uint32_t* pixels, int image_width, int image_height, int ns) {

    std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;
    m_ImageVerticalIter.resize(image_height);
    m_ImageHorizontalIter.resize(image_width);
    for (int i = 0; i < image_height; i++)
        m_ImageVerticalIter[i] = i;
    for (int i = 0; i < image_width; i++)
        m_ImageHorizontalIter[i] = i;

	// Uncomment to use the original scene
    // Original Scene
    //hitable* list[5];
    //float R = cos(M_PI / 4);
    //list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    ////list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    //list[1] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    //list[2] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    //list[3] = new sphere(vec3(-2, 0, -1), 0.45, new dielectric(1.5));
    //list[4] = new bounded_plane(vec3(0, -0.5, -1), vec3(0, 1, 0), 5.0f, 5.0f, new lambertian(vec3(0.8, 0.8, 0.0)));

	// Uncomment to override camera settings for original scene
    //hitable* world = new hitablelist(list, 5);
    /*vec3 lookfrom(3, 3, 2);
    vec3 lookat(0, 0, -1);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 2.0;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(image_width) / float(image_height), aperture, dist_to_focus);*/


	// Uncomment to use the random scene
    //Random Scene
    hitable* world = random_scene();

	// Uncomment to override camera settings for random scene
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(image_width) / float(image_height), aperture, dist_to_focus);

#define MT 1
#if MT
    std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
        [&](int j)
        {
#if 0
            std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
            [&, j](int i)
                {
                    vec3 col(0, 0, 0);
                    for (int s = 0; s < ns; s++)
                    {
                        float u = float(i + random_double()) / float(image_width);
                        float v = float(j + random_double()) / float(image_height);
                        ray r = cam.get_ray(u, v);
                        vec3 p = r.point_at_parameter(2.0);
                        col += color(r, world, 0);
                    }

                    col /= float(ns);
                    col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                    int ir = static_cast<int>(255.99 * col[0]);
                    int ig = static_cast<int>(255.99 * col[1]);
                    int ib = static_cast<int>(255.99 * col[2]);

                    ir = std::min(255, std::max(0, ir));
                    ig = std::min(255, std::max(0, ig));
                    ib = std::min(255, std::max(0, ib));

                    uint32_t pixel_color = (255 << 24) | (ir << 16) | (ig << 8) | ib;
                    pixels[(image_height - 1 - j) * image_width + i] = pixel_color;
                });

#endif
            for (int i = 0; i < image_width; i++) {
                vec3 col(0, 0, 0);
                for (int s = 0; s < ns; s++)
                {
                    float u = float(i + random_double()) / float(image_width);
                    float v = float(j + random_double()) / float(image_height);
                    ray r = cam.get_ray(u, v);
                    vec3 p = r.point_at_parameter(2.0);
                    col += color(r, world, 0);
                }

                col /= float(ns);
                col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
                int ir = static_cast<int>(255.99 * col[0]);
                int ig = static_cast<int>(255.99 * col[1]);
                int ib = static_cast<int>(255.99 * col[2]);

                ir = std::min(255, std::max(0, ir));
                ig = std::min(255, std::max(0, ig));
                ib = std::min(255, std::max(0, ib));

                uint32_t pixel_color = (255 << 24) | (ir << 16) | (ig << 8) | ib;
                pixels[(image_height - 1 - j) * image_width + i] = pixel_color;
            }
        });

#else
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + random_double()) / float(image_width);
                float v = float(j + random_double()) / float(image_height);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = static_cast<int>(255.99 * col[0]);
            int ig = static_cast<int>(255.99 * col[1]);
            int ib = static_cast<int>(255.99 * col[2]);

            ir = std::min(255, std::max(0, ir));
            ig = std::min(255, std::max(0, ig));
            ib = std::min(255, std::max(0, ib));

            uint32_t pixel_color = (255 << 24) | (ir << 16) | (ig << 8) | ib;
            pixels[(image_height - 1 - j) * image_width + i] = pixel_color;
        }
    }
#endif
}

hitable* random_scene() {
    int n = 500;
    hitable** list = new hitable * [n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(random_double() * random_double(), random_double() * random_double(), random_double() * random_double())));
                }
                else if (choose_mat < 0.95) {  // metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + random_double()), 0.5 * (1 + random_double()), 0.5 * random_double()), 0.5 * random_double()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitablelist(list, i);
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    /*int image_width = 800;
    int image_height = 400;*/
	// Set the number of samples per pixel (anti-aliasing)
    int ns = 100;


    SDL_Window* window = SDL_CreateWindow("Not-So-Real-Time Ray Tracing",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        image_width, image_height,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

	// Create a texture to hold the pixel data
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        image_width, image_height);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    uint32_t* pixels = new uint32_t[image_width * image_height];

    render_scene(pixels, image_width, image_height, ns);

	// Update the texture with the pixel data and render it to the window
    SDL_UpdateTexture(texture, NULL, pixels, image_width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    bool running = true;

    while (running)
    {
        auto start_time = std::chrono::high_resolution_clock::now();

		// Handle keyboard input for camera movement
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    cam.move_forward(0.1f);
                    break;
                case SDLK_s:
                    cam.move_backward(0.1f);
                    break;
                case SDLK_a:
                    cam.move_left(0.1f);
                    break;
                case SDLK_d:
                    cam.move_right(0.1f);
                    break;
                case SDLK_e:
                    cam.move_up(0.1f);
                    break;
                case SDLK_q:
                    cam.move_down(0.1f);
                    break;
                }
            }
        }

       render_scene(pixels, image_width, image_height, ns);

        SDL_UpdateTexture(texture, NULL, pixels, image_width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

		// Calculate frame time and FPS
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> frame_time = end_time - start_time;
        float fps = 1.0f / frame_time.count();
        std::cout << "\rFrame Time: " << frame_time.count() * 1000 << " ms (" << fps << " FPS)" << std::flush;

        //SDL_Delay(100); // ~60 FPS
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
