#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/objmesh.h"
#include "helper/plane.h"
#include "helper/skybox.h"
#include <memory>

class SceneBasic_Uniform : public Scene
{
private:
    std::unique_ptr<ObjMesh> ball;
    Plane plane;
    SkyBox sky;
    GLSLProgram prog;
    GLSLProgram skyProg;
    GLuint ballTex;

    glm::mat4 model, view, projection;

    float angle;
    float tPrev;
    float rotSpeed;

    void compile();
    void setMatrices();

public:
    SceneBasic_Uniform();
    void initScene();
    void update(float t);
    void render();
    void resize(int, int);

    void increaseRotSpeed() { rotSpeed += 0.01f; }
    void decreaseRotSpeed() { rotSpeed = glm::max(0.0f, rotSpeed - 0.01f); }
};

#endif // SCENEBASIC_UNIFORM_H