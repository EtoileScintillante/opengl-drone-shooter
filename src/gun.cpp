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

glm::mat4 getGunModelMatrix(glm::vec3 gunPosition, glm::mat4 cameraView, float scaleFactor, float rotation)
{
    glm::mat4 gunModel = glm::mat4(1.0); 
    gunModel = glm::translate(gunModel, gunPosition); // place gun in bottom right corner
    gunModel = glm::rotate(gunModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate gun so it points inwards
    gunModel = glm::scale(gunModel, glm::vec3(scaleFactor)); // make gun a bit smaller
    gunModel = glm::inverse(cameraView) * gunModel;

    return gunModel;
}

void startRecoilAnimation(Shader &shader, glm::mat4 &modelMatrixGun, float &angle, bool &down)
{
    if (angle > 20.0f) // rotate up as long as angle has not surpassed this value
    {
        down = true;
    }

    modelMatrixGun = glm::rotate(modelMatrixGun, glm::radians(angle), glm::vec3(0.0f,  0.0f, 1.0f)); // rotate upwards
    shader.setMat4("model", modelMatrixGun);
    
    angle+=6.0; // increase angle with every frame
}

void goBackToBase(Shader &shader, glm::mat4 &modelMatrixGun, float &angle, bool &shot, bool &down, bool &startRecoil)
{
    if (angle > 0) // rotate down as long as base position is not reached
    {
        modelMatrixGun = glm::rotate(modelMatrixGun, glm::radians(angle), glm::vec3(0.0f,  0.0f, 1.0f)); // rotate downwards
        shader.setMat4("model", modelMatrixGun);
    }

    if (angle <= 0) // set all values back to what they were before player shot gun
    {
        angle = 0;
        shot = false;
        down = false;
        startRecoil = false;
    }

    angle-=6.0; // decrease angle with every frame
}