#include "ray.h"

Ray::Ray(glm::vec3 orig, glm::vec3 dir) : orig(orig), dir(dir)
{
    invdir = 1.0f / dir;
    sign[0] = (invdir.x < 0);
    sign[1] = (invdir.y < 0);
    sign[2] = (invdir.z < 0);
}