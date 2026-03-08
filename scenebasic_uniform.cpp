#include "scenebasic_uniform.h"
#include <iostream>
#include <sstream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
#include "helper/glutils.h"
#include "helper/texture.h"

SceneBasic_Uniform::SceneBasic_Uniform() :
    torus(0.7f, 0.3f, 50, 50),
    plane(30.0f, 15.0f, 1, 1),
    sky(100.0f),
    angle(0.0f),
    tPrev(0.0f),
    rotSpeed(glm::pi<float>() / 8.0f)
{
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    // Load skybox texture
    GLuint cubeTex = Texture::loadCubeMap("media/texture/suburban_soccer_park");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    // Light uniforms
    prog.use();
    prog.setUniform("Light.Position", vec4(0.0f, 10.0f, 0.0f, 1.0f));
    prog.setUniform("Light.La", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Light.Ld", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Light.Direction", vec3(0.0f, -1.0f, 0.0f));
    prog.setUniform("Light.Cutoff", cos(glm::radians(45.0f)));
    prog.setUniform("Light.Exponent", 50.0f);
}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();

        skyProg.compileShader("shader/skybox.vert");
        skyProg.compileShader("shader/skybox.frag");
        skyProg.link();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;

    angle += rotSpeed * deltaT;
    if (angle > glm::two_pi<float>())
        angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotating camera
    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    // Draw skybox
    skyProg.use();
    model = mat4(1.0f);
    mat4 mv = view * model;
    skyProg.setUniform("MVP", projection * mv);
    sky.render();

    // Switch to main shader
    prog.use();

    // Update light position to view space
    prog.setUniform("Light.Position", view * vec4(0.0f, 10.0f, 0.0f, 1.0f));
    prog.setUniform("Light.Direction", mat3(view) * vec3(0.0f, -1.0f, 0.0f));

    // Draw torus
    prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Kd", vec3(0.4f, 0.6f, 0.9f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    setMatrices();
    torus.render();

    // Draw plane
    prog.setUniform("Material.Ka", vec3(0.1f, 0.2f, 0.1f));
    prog.setUniform("Material.Kd", vec3(0.2f, 0.5f, 0.2f));
    prog.setUniform("Material.Ks", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Shininess", 10.0f);

    model = mat4(1.0f);
    setMatrices();
    plane.render(); 
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}