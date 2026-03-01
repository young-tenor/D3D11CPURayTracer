#include "pch.h"
#include "Object.h"

Object::Object(const glm::vec3 &ambient) : ambient(ambient) {
}

Object::Object(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) : ambient(ambient), diffuse(diffuse), specular(specular) {
}
