#include "gun.h"

void walkingMotion(float &gunY, float &gunZ, float currentFrame)
{
    gunZ += sin(currentFrame*15) * 0.004f;
    gunY += sin(currentFrame*15) * 0.004f;
}