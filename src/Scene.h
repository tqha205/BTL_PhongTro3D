#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object3D.h"
#include "Shader.h"

class Scene {
public:
    std::vector<Object3D*> Objects;
    
    void Initialize();
    void Update(float deltaTime);
    void DrawAll(Shader& shader);
    void ToggleDoor();
    
    // Door state
    bool isDoorOpen = false;
    float doorAngle = 0.0f;
    Object3D* bathroomDoor = nullptr;
    Object3D* bathroomDoorHandle = nullptr;
    Object3D* mainDoor = nullptr;
    Object3D* mainDoorHandle = nullptr;
    float mainDoorAngle = 0.0f;
    
    // Lighting state
    bool isLightOn = true;
    void ToggleLights();
    Object3D* recessedLights[4] = {nullptr, nullptr, nullptr, nullptr};
    
private:
    void AddBox(glm::vec3 pos, glm::vec3 sc, glm::vec3 alb, float met = 0.0f, float rough = 1.0f, glm::vec3 rot = glm::vec3(0), GLuint texID = 0);
    void AddCylinder(glm::vec3 pos, float radius, float height, glm::vec3 alb, float met = 0.0f, float rough = 1.0f, glm::vec3 rot = glm::vec3(0));
    void AddSphere(glm::vec3 pos, float radius, glm::vec3 alb, float met = 0.0f, float rough = 1.0f);
};

#endif
