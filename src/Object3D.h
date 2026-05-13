#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Material {
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float shininess;
    glm::vec3 specular;
    bool hasTexture;
    GLuint textureID;
    glm::vec3 emissive = glm::vec3(0.0f);
};

class Object3D {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    Material material;

    Object3D(glm::vec3 pos, glm::vec3 sc, glm::vec3 rot, glm::vec3 alb, float met, float rough, GLuint texID = 0);
    ~Object3D();

    void Draw();
    glm::mat4 GetModelMatrix();
    static GLuint LoadTexture(const char* path);

private:
    unsigned int VAO, VBO;
    void SetupMesh();
};

#endif
