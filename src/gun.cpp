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