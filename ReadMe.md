# COMP3015 CW2 

## Introduction 
the openGL project showcases a rotating wooden sword on top of a wood pedestal, the scene is meant to resemble a museum style display. the project demonstrates two advanced shader texhniques, Shadow Mapping with PCf filtering as well as Bloom/HDR tone mapping.   
<img width="792" height="590" alt="image" src="https://github.com/user-attachments/assets/cd78d521-c91d-4269-ac33-215340b96976" />

### The project relies on:
* GLAD
* GLM
* GLFW

## How to Run
1. Unzip project folder
2. Open 'Project_Template.sln' in VisualStudio 2022
3. Build and run the project
4. The application will open a window titled 'CW2'
5. alternatively an executable is provided in the .zip folder

### Controls
* W - Increase rotation speed
* S - Decrease rotation speed
* Q - Toggle bloom on/off
* ESC - Exit

## The Code 
Like CW1, the project is built on the template provided in Lab 1, with the entry point being main.cpp, with a SceneRunner handling the GLFW window, opeb GL contect and main game loop. Each fram 'Update()' is called then 'render()' on the scene.

### Classes and Roles
**SceneBasic_Uniform** - main scene class, inherits from Scene and implements core logic:
* 'initScene()' - loads sword model and textures, compile all three shader programs, sets up shadow FBO and HDR FBO, sets initial uniforms
* 'update()' - advances rotation angle each frame
* 'render()' - runs the three pass render pipeline (shadow pass, mains scene pass, bloom post process pass)
* 'setupShadowFBO()' - creates depth texture and framebuffer for shadow mapping
* 'setupHdrFBO()' - creates the floating point framebuffer for HDR rendering
* 'setupScreenQuad()' creates a full screen quad for post process pass

**ObjMesh** - from the lab helper files, used to load sword OBJ model
**Plane** - from the lab helper files, generates floor mesh
**Cube** - from the lab helper files, used for pedastal
**Texture** - from the lab helper files, handles texture loading

### Shader Files
**'scene.vert' / 'scene.frag'** - main scene shader. Vertex shader transforms the positions and normals into view space, and transformers each vertex into light space for shadow lookup. Fragment shader implements Blinn-Phong lighting with a spotligfht cone, PCf shashadow sampling and light attenuation.
**'shadowmap.vert / 'shadowmap. frag'** - depth only pass rendere from the light's POV. only rites depth values, no colour output.
**'bloom.vert / 'bloom.frag'** - full screen post process shader. extacts bright pixels above a threshold, applies Gaussian weighted blur, blends the result back on the scen, then applied Reinhard tone mapping and gamma correction.

### The Render Pipeline
What differentiates this project from CW1 the most is the three pass render pipeline:

**Pass 1 - Shadow Map:** the entire scen is rendered from the light's perspective into a 2048x2048 depth texture. This records how far each surface is from the light.

**Pass 2 - Main Scene:** The scene is rendered normally into a floating point HDR framebuffer. Each fragment samples the shadow map using a 3x3 PCF Kernel to determine if it's in shadow. Colour values can exceed 1.0 here, which is what makes bloom possible.

**Pass 3 - Bloom/HDR:** A dull screen quad is drawn using the HDR texture from Pass 2. Bright pixels are blurred and belnded back as a glow effect. Reinhard tone mapping and gamme acorrection bring the result into displayable range.

## Evaluation 
the scene I created was meant to resemble a simple scene, similar to that of CW1 with the football pitch, however this time a museum display was chosen instead, with a roman sword rotating on a wooden pedestal as if displayed to visitors. the dynamic shadow clearly demonstrates shadow mapping working in real time and toggling bloom with 'Q' exhibits the post processing stage working independently of lighting.

compared to CW1, this projcet introduces a significantly more complex rendering architectire, moving from a single render pass to a three pass pipeline. Overall im happy with how the project went.
in terms of limitations, though I think this deliverable is less buggy and more impressive, I think the scene itself could do with work, it would be better if the scene resembled a musuem as much as CW1 resembled a football scene, hwoever due to the more complex nature of this project not as much time was assigned to that.

## Additonal Notes 
* Sword model and texture sourced from: https://www.turbosquid.com/FullPreview/1982865?dd_referrer=
* Pedestal texture sourced from: https://www.turbosquid.com/3d-models/chess-set-654392
* AI usage has been declared in the AI declaration form

## Links
* YouTube Video: (fill)
* Github Repository: https://github.com/Bainzzz/comp3015cw2/new/master

* 
