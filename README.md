# RT One Weekend SDL

All code adapted from "Ray Tracing In One Weekend" by Peter Shirley, with SDL integration for sequentially displaying frames.
Reference: https://raytracing.github.io/

Planning to update this with SDL3 GPU backend and shader code conversion for true real-time performance.

# Build Instructions

This program uses SDL 2.28.2.

1. Download SDL2-devel-2.28.2-VC from https://github.com/libsdl-org/SDL/releases/tag/release-2.28.2
2. Extract SDL 2.28.2
3. Clone this repository
4. Navigate to \RT-One-Weekend-SDL\RT_One_Weekend\SDL2
5. Copy lib folder from SDL 2.28.2 to SDL2 folder
6. Run program in debug or release to generate x64 folder
7. Navigate to \RT-One-Weekend-SDL\x64\Debug OR \Release
8. Copy SDL2.dll from SDL 2.28.2 to \Debug OR \Release folder
9. Run program!

The image will take a minute to generate. To generate the image faster, change the ns (number of samples) from 100 to 10 in main.cpp.
