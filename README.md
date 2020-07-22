# SoftwareRendererGoBrrr
Writing a software renderer for learning purposes. And yes acronym for the project is SRGB which is a completely unrelated thing

## **CURRENT FEATURES**

- Working SDL2 window with capability to swap between buffers.
- Working templated vector math library
- more to come...

### **TODO:**


- [x]  Window class to initialize SDL2 and provide buffer swapping to draw frame.
- [x]  Write emplated buffer class for z-buffer(float) and pixel buffer(uint32_t).
- [x]  Write templated vector math library.
- [x]  Rasterizer interger bresenham line drawing function.
- [x]  OBJ mesh file parser and Model class to hold data from OBJ file
- [ ]  Finish model class functions (getNormal, getFace etc...)
- [x]  Wireframe drawer (Make it work on any file): Works now, but requires wireframe to be in NDC space (-1, 1)
- [x]  Fix buffer to index with (0 0) at bottom left.(Left with 0,0 at top left for now. Easily changed in buffer class or in viewport transform)
- [x]  Matrix4 class for homgenous coordinate transforms and add w to Vector3
- [x]  Fix w in Vector3 not being initialized properly to 1 for some reason being -1204195803 (uninitialized)
- [x]  Write simple triangle rasterizer. Bounding box and barycentric coordinate functions in Rasterizer class
- [x]  Expand rasterizer to have flat shading. aka face normal lighting.
- [ ]  Refactor a bunch of code into functions and better locations. Usually marked by ALLCAPS comments
- [ ]  Get gourad shading to work
- [ ]  Implement z buffer(reverse z buffer/ logarithmic later as well?)
