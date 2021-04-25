# SoftwareRendererGoBrrr
Writing a software renderer to get an understanding of how 3D computer graphics and low level graphic API such as OpenGL, DirectX and VUlkan work at a very basic level. The renderer uses SDL2 for hardware abstraction and stbimage for image loading, the rest of the engine has been written from scratch for learning purposes.
-Build system using premake soon to be added

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
- [x]  Fix projection matrix clipping. (Problem was using comparator operators like this in an if statement (max >= x >= min) and expecting it to work.)
- [x]  Reverse z buffer in projection matrix working
- [x]  Correctly transform scene lights
- [x]  Get texture UV map to map correctly to model
- [x]  Create shader class (Interface polymorphism)
- [x]  Fix gourad shader vertex normals
- [x]  Code buildVertexNormals() function in model (For bunny with no vn in obj file)
- [x]  Phong Shading
- [x]  Optimize rasterizer
- [x]  Camera movement and input handling
- [ ]  Fixed point arithmetic/subpixel precision rasterizer: UNLIKELY TO BE ADDED FOR NOW
- [x]  Fix projection matrix appearing as orthographic not perspective
- [x]  Fix renderer looking extremely skewed after perspective divide. Might have to do with lookat matrix
- [x]  OMP parallelization of renderer
- [x]  Multiple and coloured directional light support
- [x]  Blinn Phong shader
- [x]  Added materials
- [x]  Fix normals for flat shader being wrong way around buildFaceNormals()
- [ ]  Texture bilinear interpolation
- [x]  Fix specular light reflections not being correctly interpolated(It was the view vectors being normalized before interpolating them. F)
- [ ]  Split model into mesh and model and make obj file parser for the mesh create a vertex buffer with interleaved position, normal and texture data and an index buffer to avoid repeated vertices.
-[ ] Add a loadScene() function to Scene class that can read JSON file with scene data to load a scene's models, lights etc...
-[ ] Add a scene swapper/manager class that handles swapping between scenes