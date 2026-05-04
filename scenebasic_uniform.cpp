#include "scenebasic_uniform.h"
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

#include "helper/glutils.h"
#include "helper/texture.h"

static const vec3 LIGHT_POS(0.0f, 10.0f, 2.0f); //

SceneBasic_Uniform::SceneBasic_Uniform() :

    torus(1.5f, 0.4f, 50, 50),
    plane(10.0f, 10.0f, 1, 1),
    angle(0.0f),
    tPrev(0.0f),
    rotSpeed(glm::pi<float>() / 8.0f),
    bloomEnabled(true),
    lightPos(LIGHT_POS),
    shadowFBO(0), shadowDepthTex(0),
    hdrFBO(0), hdrColorTex(0), hdrDepthRBO(0),
    quadVAO(0), quadVBO(0)
{
}


void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/scene.vert");
        prog.compileShader("shader/scene.frag");
        prog.link();

        shadowProg.compileShader("shader/shadowmap.vert");
        shadowProg.compileShader("shader/shadowmap.frag");
        shadowProg.link();

        bloomProg.compileShader("shader/bloom.vert");
        bloomProg.compileShader("shader/bloom.frag");
        bloomProg.link();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    sword = ObjMesh::load("media/Sword_obj.obj", true, false);
    swordTex = Texture::loadTexture("media/swordwood.jpg");
    pedestalTex = Texture::loadTexture("media/Wood.jpg");

    setupShadowFBO();
    setupHdrFBO();
    setupScreenQuad();

    prog.use();
    prog.setUniform("Light.La", vec3(0.15f, 0.15f, 0.15f)); 
    prog.setUniform("Light.Ld", vec3(3.0f, 2.5f, 1.5f));     
    prog.setUniform("Light.Ls", vec3(4.0f, 3.5f, 2.0f));      
    prog.setUniform("Light.Direction",
        glm::normalize(vec3(view * vec4(0.0f, -1.0f, 0.0f, 0.0f))));
    prog.setUniform("Light.Cutoff", cos(glm::radians(25.0f)));
    prog.setUniform("Light.Exponent", 8.0f);
    prog.setUniform("BallTex", 0); 
    prog.setUniform("ShadowMap", 1); 

    bloomProg.use();
    bloomProg.setUniform("HdrBuffer", 0);

    projection = mat4(1.0f);
}

void SceneBasic_Uniform::setupShadowFBO()
{
    glGenFramebuffers(1, &shadowFBO);

    glGenTextures(1, &shadowDepthTex);
    glBindTexture(GL_TEXTURE_2D, shadowDepthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = { 1,1,1,1 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, shadowDepthTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void SceneBasic_Uniform::setupHdrFBO()
{
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    glGenTextures(1, &hdrColorTex);
    glBindTexture(GL_TEXTURE_2D, hdrColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
        GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, hdrColorTex, 0);

    glGenRenderbuffers(1, &hdrDepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, hdrDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, hdrDepthRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneBasic_Uniform::setupScreenQuad()
{
    float verts[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}



void SceneBasic_Uniform::setMatrices(GLSLProgram& shader)
{
    if (&shader == &prog) {
        mat4 mv = view * model;
        prog.setUniform("ModelViewMatrix", mv);
        prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        prog.setUniform("MVP", projection * mv);
        prog.setUniform("ModelMatrix", model);
        prog.setUniform("LightSpaceMatrix", lightSpaceMatrix);
    }
    else {
        shader.setUniform("MVP", lightSpaceMatrix * model);
    }
}

void SceneBasic_Uniform::renderScene(GLSLProgram& shader)
{
    bool isMainPass = (&shader == &prog);

    if (isMainPass) {
        prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.15f));
        prog.setUniform("Material.Kd", vec3(0.8f, 0.7f, 0.6f));
        prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));
        prog.setUniform("Material.Shininess", 100.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, swordTex);
        prog.setUniform("HasTexture", (swordTex != 0));
    }
    model = glm::translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
    model = glm::rotate(model, angle, vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-145.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, vec3(0.3f, 0.3f, 0.2f));
    setMatrices(shader);
    sword->render();

    if (isMainPass) {
        prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.1f));
        prog.setUniform("Material.Kd", vec3(0.4f, 0.35f, 0.3f));
        prog.setUniform("Material.Ks", vec3(0.3f, 0.3f, 0.3f));
        prog.setUniform("Material.Shininess", 32.0f);
        glActiveTexture(GL_TEXTURE0);                         
        glBindTexture(GL_TEXTURE_2D, pedestalTex);            
        prog.setUniform("HasTexture", true);
    }
    model = glm::translate(mat4(1.0f), vec3(0.0f, 0.05f, 0.0f));
    model = glm::scale(model, vec3(3.0f, 1.0f, 3.0));
    setMatrices(shader);
    pedestal.render();

    if (isMainPass) {
        prog.setUniform("Material.Ka", vec3(0.2f, 0.15f, 0.1f));
        prog.setUniform("Material.Kd", vec3(0.5f, 0.4f, 0.3f));
        prog.setUniform("Material.Ks", vec3(0.2f, 0.2f, 0.2f));
        prog.setUniform("Material.Shininess", 20.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTex);
        prog.setUniform("HasTexture", (planeTex != 0));
    }
    model = mat4(1.0f);
    setMatrices(shader);
    plane.render();
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
    mat4 lightProj = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 1.0f, 50.0f);
    mat4 lightView = glm::lookAt(vec3(3.0f, 10.0f, 3.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProj * lightView;

    shadowProg.use();
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POLYGON_OFFSET_FILL); 
    glPolygonOffset(2.0f, 4.0f);       
    glCullFace(GL_FRONT);
    renderScene(shadowProg);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 camPos = vec3(0.0f, 4.0f, 8.0f);
    view = glm::lookAt(camPos, vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    prog.use();
    prog.setUniform("LightPosition", vec3(view * vec4(lightPos, 1.0f)));
    prog.setUniform("Light.Direction", vec3(view * vec4(0.0f, -1.0f, 0.0f, 0.0f)));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowDepthTex);

    renderScene(prog);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    bloomProg.use();
    bloomProg.setUniform("BloomEnabled", bloomEnabled);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrColorTex);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.1f, 100.0f);

    if (hdrFBO) {
        glDeleteFramebuffers(1, &hdrFBO);
        glDeleteTextures(1, &hdrColorTex);
        glDeleteRenderbuffers(1, &hdrDepthRBO);
        setupHdrFBO();
    }
}