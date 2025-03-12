#pragma once

#include "Model.hpp"

#include <GL/glew.h>

#include <vector>
#include <glm/common.hpp>

namespace monotile
{
	class Monotile
	{
	public:
		Monotile(int hexagons);
		std::vector<Model> models;

		static Model GetHexagonVertices(glm::vec3 position, float radius = 1.f);
		static Model GetMonotileVertices(glm::vec3 position, float a, GLenum drawMode);

	private:
		int hexagons;
	};
}