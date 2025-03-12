#ifndef __MODEL_HPP_
#define __MODEL_HPP_

#include <vector>
#include "glm/vec3.hpp"

struct Model
{
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
};

#endif
