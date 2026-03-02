#include "pch.h"
#include "Ray.h"

Ray::Ray() {
}

Ray::Ray(const glm::vec3 &orig, const glm::vec3 &dir) : orig(orig), dir(dir) {
}

