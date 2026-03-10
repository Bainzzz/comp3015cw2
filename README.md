
# COMP3015 CW1 - FootBall Scene

## Introduction 
<p>The scene is made to resemble a Football on a grass field, consisting of a textured ball, a green plane, some fog designed to obscure the plane edges slightly (though not working fully as intended) as well as a spotlight shining above the ball (though this is also slightly buggy). The project also allows for keyboard controls that speed or slow down the camera's circling movement. </p> 

<img width="798" height="603" alt="image" src="https://github.com/user-attachments/assets/357f5bc2-a7cd-44c1-b2e5-2708304fb91b" />


## Running solution
<p>To run the scene, open the solution file Project_Template.sln, an executable file is also provided within the '.zip' folder  </p>

### Controls:
- W - Speeds up Camera Rotation
- S - Slows down Camera Rotation (hold the button enough and rotation stops completely)
- ESC - Exit

## The Code 
<p> The project is built on the template provided in Lab 1, the entry point is main.cpp where a scenerunner is created and passes a SceneBasic_uniform instance. SceneRunner handles the GLFW window, OpenGL context setup and the main game loop. Each frame it calls 'update()' then 'render()' on the scene. </p>

### Classes and Roles:
<p> ScenBasic_Uniform - main scene class. Inherits from scene and implements core logic. </p>

- initScene() -loads football model, textures , compiles shaders, sets initial uniforms.
- update() - advances camera rotation angle each frame
- render() - draws skybox first, then football and then plane

<p> ObjMesh - from lab helper files </p>
<p> Plane - from lab helper files, generating a flat plane mesh </p>
<p> SkyBox - from lab helper files, renders cube map skybox </p>
<p> Texture - from lab helper files, handles loading 2D textures and cube maps using stb_image </p>

### Shader Files
<p> 'basic_uniform.vert' / 'basic_uniform.frag' is the main scene shader. The vertex shader transforms positions and and normals into view space and passes transitions and normals into view space and passes co-ords to fragment shader. '.frag' implements Blinn-Phong lighting, fog and texture sampling. </p>

<p> 'skybox.vert' / 'skybox.frag' a simple skybox shader that samples a cube map texture. </p>

## Evaluation
<p> The scene I created mirrors that of a football field with a football sitting on a green plane thats meant to reflect the grass field and a skybox that shows a football pitch. The spotlight is meant to resemble that of a floodlight however this effect isn't captured as I intended, similary to the 'fog' effects. I chose this idea as it's easily recognisable and is easy to convey what is going on even if not fully developed, which was a wise choice. I believe that the project is a fair reflection of my skills within the module and builds off the lab content, I do believe I haven't achieved the aesthetic I desired however I've been able to implement many features and achieve a basic manifestation of my idea. </p>

### Additional Notes 
- the skybox was converted from a HDR file found on Poly Haven to Cube Map Converter
    - The Skybox - (https://polyhaven.com/a/suburban_soccer_park)
    - Site used to convert HDR (https://matheowis.github.io/HDRI-to-CubeMap/) 
- Football model and texture was sourced from TurboSquid
    - Football model (https://www.turbosquid.com/3d-models/soccer-ball-2296819)
- AI usage has been declared in the AI Declaration form

## Links:
- YouTube Video: https://youtu.be/T6N6RN4ywZo
