
# COMP3015 CW1 - FootBall Scene

## Introduction 
<p>The scene is made to resemble a Football on a grass field, consisting of a textured ball, a green plane, some fog designed to obscure the plane edges slightly (though not working fully as intended) as well as a spotlight shining above the ball (though this is also slightly buggy). The project also allows for keyboard controls that speed or slow down the camera's circling movement. </p>

## Running solution
<p>To run the scene, open the solution file Project_Template.sln, an executable file is also provided within the '.zip' folder  </p>

### Controls:
- W - Speeds up Camera Rotation
- S - Slows down Camera Rotation (hold the button enough and rotation stops completely)
- ESC - Exit

## The Code 
<p> The project is built on the template provided in Lab 1, the entry point is main.cpp where a scenerunner is created and passes a SceneBasic_uniform instance. SceneRunner handles the GLFW window, OpenGL context setup and the main game loop. Each frame it calls 'update()' then 'render()' on the scene. </p>

### Classes and Roles:
