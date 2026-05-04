#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/plane.h"
#include "helper/cube.h"
#include "helper/objmesh.h"
#include <memory>

class SceneBasic_Uniform : public Scene
{
private:
    Torus torus;
    Plane plane;
    Cube pedestal;
    std::unique_ptr<ObjMesh> sword;

    GLSLProgram prog;      
    GLSLProgram shadowProg; 
    GLSLProgram bloomProg;  

    GLuint swordTex;
    GLuint planeTex;
    GLuint pedestalTex;
    GLuint shadowFBO;
    GLuint shadowDepthTex;
    static const int SHADOW_WIDTH = 4096;
    static const int SHADOW_HEIGHT = 4096;
    GLuint hdrFBO;
    GLuint hdrColorTex;
    GLuint hdrDepthRBO;

    GLuint quadVAO, quadVBO;

    glm::mat4 model, view, projection;
    glm::mat4 lightSpaceMatrix; 

    glm::vec3 lightPos;

    float angle;
    float tPrev;
    float rotSpeed;

    bool bloomEnabled;

    void compile();
    void setMatrices(GLSLProgram& shader);
    void setupShadowFBO();  
    void setupHdrFBO();      
    void setupScreenQuad();  
    void renderScene(GLSLProgram& shader); 

public:
    SceneBasic_Uniform();
    void initScene();
    void update(float t);
    void render();
    void resize(int, int);

    void increaseRotSpeed() { rotSpeed += 0.2f; }
    void decreaseRotSpeed() { rotSpeed = glm::max(0.0f, rotSpeed - 0.2f); }
    void toggleBloom() { bloomEnabled = !bloomEnabled; }
};

#endif