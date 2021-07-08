# SoftwareRendererGoBrrr
I am writing a software renderer to get an understanding of how a 3D computer graphics pipeline and low level graphic APIs such as OpenGL, DirectX and Vulkan work at a very basic level. The renderer uses SDL2 for hardware abstraction and stbimage for image loading, the rest of the engine has been written from scratch for learning purposes.   

Most of the renderer including the main features such as flat/gourad/phong shading(flat and gourad have since been deprecated) and the main "engine" was written as a summer project in July 2020. After a break from the project I decided to come back to it with more knowledge and rework the renderer to more closely match the vertex/index buffer pipeline that modern graphics APIs use and added physically based rendering shader.

CMake can be used to build the project

(The project name is an old meme that happend to have the same acronym as the sRGB colour space and I found that funny at the time(i still do))




## Renderer examples
**Chest render(3 directional lights):**     
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/Chest1.PNG" alt="Chest render" width="450" height=100%>  <img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/Chest2.PNG" alt="Chest render 2" width="450" height=100%>       
**Cerberus gun (3 directional lights):**       
<img src="https://raw.githubusercontent.com/danimtz/SoftwareRendererGoBrrr/master/SRGB/assets/RenderImages/Gun/Gun1.PNG" alt="Cerberus gun render" width="900" height=100%>      
**Fire hydrant render(2 directional lights) (The small triangles on the hydrant "handle" are like that in the textures provided):**     
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Hydrant/Hydrant2.PNG" alt="Hydrant render" width="450" height=100%>  <img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Hydrant/Hydrant.PNG" alt="Hydrant render 2" width="450" height=100%> 

**Chest PBR textures mapped onto mesh:**    
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/ChestAlbedo.PNG" alt="Chest Albedo" width="300" height=100%>
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/ChestNormalMap.PNG" alt="Chest Normal" width="300" height=100%>
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/ChestMetallic.PNG" alt="Chest Metallic" width="300" height=100%> 
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/ChestRoughness.PNG" alt="Chest Roughness" width="300" height=100%>
<img src="https://github.com/danimtz/SoftwareRendererGoBrrr/blob/07cf90ae1329c613683526e77bd61a327cad11eb/SRGB/assets/RenderImages/Chest/ChestAO.PNG" alt="Chest AO" width="300" height=100%>

*In order: albedo, normal map, metallic map, roughness map, ambient occlusion map*



## **Main features**

- Parallellized forward renderer and rasterizer using OpenMP
- Vertex data loaded into vertex and index buffers
- Programmable vertex and fragment shaders
- Phong/Blinn-Phong shaders
- Perspective correct texture interpolation
- Tangent space normal mapping
- Metallic-Roughness workflow
- Multiple directional lights
- Physically Based Rendering using Cook Torrance BRDF shader
- Swap between shaders and scenes at runtime using the numkeys/arrowkeys
- Fustrum culling
- Winding order backface culling
- Reverse depth buffer
- View/Camera-space lighting


### **Other features**
- SDL2 backend for window display
- Templated vector math and matrix library
- .obj file loader
- Texture loading through stb image
- Free and orbital camera system

### **Missing features and issues**
- No shadow rendering implementation
- No anti-aliasing implementation
- No texture mipmaps can lead to moire patterns

## **Renderer Controls**

- **Scene selection:** Use the **left** and **right** **arrow keys** to load next/preivious scene (takes a couple of seconds since objects are loaded/unloaded each time).
- **Shader selection:** Use **numkeys** to swap current shader  
    **1:**  Phong shading  
    **2:**  Blinn-Phong shading (with normal mapping when availible)  
    **3:**  Physically based rendering shader  
    **4:**  Albedo texture only  
    **5:**  Normal map only  
    **6:**  Metallic map only  
    **7:**  Roughness map only  
    **8:**  Ambient Occlusion map only  
- **Camera:** 
  - Hold left mouse button and drag around to move camera. Mouse wheel for zoom.
  - Press **R** to reset camera.
  - Press **T** to change from orbital to free moving camera.
  - Use **Spacebar** and **LShift** to move camera up and down respectively.
  - Use **A** and **D** to pan camera laterally.
  - Use **W** and **S** to move camera about the camera's forward vector.


## OBJ files and texture credit:
* [Chest, Fire Hydrant and Normal Tangent Test: @emackey PBR test models](https://emackey.github.io/testing-pbr/)
* [Cerberus Gun: Andrew Maximov](http://artisaverb.info/Cerberus.html) 

(Please contact me to correct any references if I have accidentally miscredited the original creator of any textre/mesh used)





## **Todo list:(OLD CHECKLIST)**
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
- [x]  Split model into mesh and model and make obj file parser for the mesh create a vertex buffer with interleaved position, normal and texture data and an index buffer to avoid repeated vertices.
- [x] Added scene swapping functionality but not from a file, hard coded in a switch statement for now

## License     
MIT License

Copyright (c) 2020 Daniel Martinez Amigo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
