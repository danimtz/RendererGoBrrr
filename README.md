# SoftwareRendererGoBrrr
Writing a software renderer for learning purposes. And yes acronym for the project is SRGB which is a completely unrelated thing

## **CURRENT FEATURES**

- Working SDL2 window with capability to swap between buffers.
- Working templated vector math library
- Working flat shading renderer
- Working Gourad and Phong shading renderer
- Texture loading through stb image lib
- more to come...

### **TODO:**


- [x]  Window class to initialize SDL2 and provide buffer swapping to draw frame.
- [x]  Write emplated buffer class for z-buffer(float) and pixel buffer(uint32_t).
- [x]  Write templated vector math library.
- [x]  Rasterizer interger bresenham line drawing function.
- [x]  OBJ mesh file parser and Model class to hold data from OBJ file
- [x]  Finish model class functions (getNormal, getFace etc...)
- [x]  Wireframe drawer (Make it work on any file): Works now, but requires wireframe to be in NDC space (-1, 1)
- [x]  Fix buffer to index with (0 0) at bottom left.(Left with 0,0 at top left for now. Easily changed in buffer class or in viewport transform)
- [x]  Matrix4 class for homgenous coordinate transforms and add w to Vector3
- [x]  Fix w in Vector3 not being initialized properly to 1 for some reason being -1204195803 (uninitialized)
- [x]  Write simple triangle rasterizer. Bounding box and barycentric coordinate functions in Rasterizer class
- [x]  Expand rasterizer to have flat shading. aka face normal lighting.
- [x]  Implement z buffer
- [x]  Model matrix/View matrix/Projection matrix implementations
- [ ]  Fix projection matrix clipping
- [x]  Reverse z buffer in projection matrix working
- [x]  Correctly transform scene lights
- [x]  Get texture UV map to map correctly to model
- [x]  Create shader class (Interface polymorphism)
- [x]  Fix gourad shader vertex normals
- [x]  Code buildVertexNormals() function in model (For bunny with no vn in obj file)
- [x]  Phong Shading
- [x]  Optimize rasterizer
- [x]  Camera movement and input handling
- [ ]  Fixed point arithmetic/subpixel precision rasterizer
- [x]  Fix projection matrix appearing as orthographic not perspective
- [x]  Fix renderer looking extremely skewed after perspective divide. Might have to do with lookat matrix
- [x]  OMP parallelization of renderer
- [x]  Multiple and coloured directional light support
- [x]  Blinn Phong shader
- [x]  Added materials
- [x]  Fix normals for flat shader being wrong way around buildFaceNormals()
- [ ]  Texture bilinear interpolation
- [ ]  Fix specular light reflections not being correctly interpolated I THINK!!!!