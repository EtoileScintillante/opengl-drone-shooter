#include "box.h"

AABBox::AABBox(){};

AABBox::AABBox(glm::vec3 vmin, glm::vec3 vmax)
{
    this->bounds[0] = vmin;
    this->bounds[1] = vmax;

    if (!isValid())
    {
        fix();
    }
}

bool AABBox::isValid()
{
    return bounds[0].x < bounds[1].x && bounds[0].y < bounds[1].y && bounds[0].z < bounds[1].z;
}

void AABBox::fix()
{
    if (bounds[0].x > bounds[1].x)
    {
        float temp = bounds[0].x;
        bounds[0].x = bounds[1].x;
        bounds[1].x = temp;
    }
    if (bounds[0].y > bounds[1].y)
    {
        float temp = bounds[0].y;
        bounds[0].y = bounds[1].y;
        bounds[1].y = temp;
    }
    if (bounds[0].z > bounds[1].z)
    {
        float temp = bounds[0].z;
        bounds[0].z = bounds[1].z;
        bounds[1].z = temp;
    }
}

bool AABBox::intersect(Ray ray, float length)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
    tmax = (bounds[1 - ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
    tymin = (bounds[ray.sign[1]].y - ray.orig.y) * ray.invdir.y;
    tymax = (bounds[1 - ray.sign[1]].y - ray.orig.y) * ray.invdir.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[ray.sign[2]].z - ray.orig.z) * ray.invdir.z;
    tzmax = (bounds[1 - ray.sign[2]].z - ray.orig.z) * ray.invdir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    length = tmin;

    if (length < 0)
    {
        length = tmax;
        if (length < 0)
            return false;
    }

    return true;
}