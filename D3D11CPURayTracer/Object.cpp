#include "pch.h"
#include "Object.h"

Object::Object(glm::vec3 ambient) : ambient(ambient) {
}

Object::Object(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : ambient(ambient), diffuse(diffuse), specular(specular) {
}
