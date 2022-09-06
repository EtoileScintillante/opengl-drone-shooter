#include "gun.h"

void walkingMotion(float &gunY, float &gunZ, float currentFrame)
{
    gunZ += sin(currentFrame*15) * 0.004f;
    gunY += sin(currentFrame*15) * 0.004f;
}

void drawhandGun(Model gun, Shader &shader)
{
    gun.drawSpecificMesh(shader, 1);
    gun.drawSpecificMesh(shader, 3);
    gun.drawSpecificMesh(shader, 4);
    gun.drawSpecificMesh(shader, 6);
}

void configureGunShader(Shader &shader, glm::vec3 gunPosition, glm::mat4 cameraView, float scaleFactor, float rotation)
{
    glm::mat4 gunModel = glm::mat4(1.0); 
    gunModel = glm::translate(gunModel, gunPosition); // place gun in bottom right corner
    gunModel = glm::rotate(gunModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate gun so it points inwards
    //float angle = glfwGetTime();
    //std::cout << angle << std::endl;
    //gunModel = glm::rotate(gunModel, glm::radians(angle), glm::vec3(0.0f,  0.0f,1.0f)); 
    gunModel = glm::scale(gunModel, glm::vec3(scaleFactor)); // make gun a bit smaller
    shader.setMat4("view", cameraView); 
    shader.setMat4("model", glm::inverse(cameraView) * gunModel);
}