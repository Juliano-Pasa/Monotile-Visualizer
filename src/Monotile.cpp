#include "Monotile.hpp"

#include "glm/gtc/constants.hpp"

namespace monotile
{
	Monotile::Monotile(int hexagons)
	{
		this->hexagons = hexagons;
		models.reserve(hexagons);

		glm::vec3 position = glm::vec3(0.f);
		float radius = 0.05f;

		for (int i = 0; i < hexagons; i++)
		{
			models.push_back(GetHexagonVertices(position + glm::vec3(i * radius * 2, 0, 0), radius));
		}
	}

	Model Monotile::GetHexagonVertices(glm::vec3 position, float radius)
	{
		Model model;
		model.vertices.reserve(13);
		model.indices.reserve(14);

		model.vertices.push_back(position);
		model.indices.push_back(0);
		float steps = glm::two_pi<float>() / 6;

		glm::vec3 previousPosition = glm::vec3(position.x + radius, position.y, position.z);
		glm::vec3 hexVertex, kiteVertex;

		for (int i = 1; i < 7; i++)
		{
			hexVertex = glm::vec3(
				position.x + radius * cos(i * steps), 
				position.y + radius * sin(i * steps), 
				position.z);
			kiteVertex = (hexVertex + previousPosition) / 2.f;

			model.vertices.push_back(kiteVertex);
			model.vertices.push_back(hexVertex);
			
			model.indices.push_back(i * 2 - 1);
			model.indices.push_back(i * 2);

			previousPosition = hexVertex;
		}
		model.indices.push_back(1);

		return model;
	}

	Model Monotile::GetMonotileVertices(glm::vec3 position, float a, GLenum drawMode)
	{
		Model model;
		model.vertices.reserve(13);
		model.vertices.push_back(position);	// 0

		while (a > 1 + sqrt(3.0f))
		{
			a -= 1 + sqrt(3.0f);
		}

		float currentAngle = 0;
		float scale = 0.1f;
		float b = sqrt(3.0f) + 1.0f - a;

		model.vertices.push_back(position = glm::vec3(		// 1
			position.x + a * cos(currentAngle),
			position.y + a * sin(currentAngle),
			0.0f));

		currentAngle += glm::pi<float>() / 3.0f;
		model.vertices.push_back(position = glm::vec3(		// 2
			position.x + a * cos(currentAngle),
			position.y + a * sin(currentAngle),
			0.0f));

		currentAngle -= glm::half_pi<float>();
		model.vertices.push_back(position = glm::vec3(		// 3
			position.x + b * cos(currentAngle),
			position.y + b * sin(currentAngle),
			0.0f));

		currentAngle += glm::pi<float>() / 3.0f;
		model.vertices.push_back(position = glm::vec3(		// 4
			position.x + b * cos(currentAngle),
			position.y + b * sin(currentAngle),
			0.0f));

		currentAngle += glm::half_pi<float>();
		model.vertices.push_back(position = glm::vec3(		// 5
			position.x + a * cos(currentAngle),
			position.y + a * sin(currentAngle),
			0.0f));
		
		currentAngle -= glm::pi<float>() / 3.0f;
		model.vertices.push_back(position = glm::vec3(		// 6
			position.x + a * cos(currentAngle),
			position.y + a * sin(currentAngle),
			0.0f));

		currentAngle += glm::half_pi<float>();
		model.vertices.push_back(position = glm::vec3(		// 7
			position.x + b * cos(currentAngle),
			position.y + b * sin(currentAngle),
			0.0f));

		currentAngle -= glm::pi<float>() / 3.0f;
		model.vertices.push_back(position = glm::vec3(		// 8
			position.x + b * cos(currentAngle),
			position.y + b * sin(currentAngle),
			0.0f));

		currentAngle += glm::half_pi<float>();
		model.vertices.push_back(position = glm::vec3(		// 9
			position.x + a * cos(currentAngle),
			position.y + a * sin(currentAngle),
			0.0f));

		currentAngle += glm::pi<float>() / 3.0f;
		model.vertices.push_back(position = glm::vec3(		// 10
			position.x + 2.0f * a * cos(currentAngle),
			position.y + 2.0f * a * sin(currentAngle),
			0.0f));

		currentAngle += glm::pi<float>() / 3.0f;
		model.vertices.push_back(position = glm::vec3(		// 11
			position.x + a * cos(currentAngle),
			position.y + a * sin(currentAngle),
			0.0f));

		currentAngle -= glm::half_pi<float>();
		model.vertices.push_back(position = glm::vec3(		// 12
			position.x + b * cos(currentAngle),
			position.y + b * sin(currentAngle),
			0.0f));

		if (drawMode == GL_TRIANGLE_FAN)
		{
			// Indices organized for a triangle fan rendering
			model.indices.reserve(26);

			model.indices.push_back(12); // Pivot on vertex 12
			model.indices.push_back(0);
			model.indices.push_back(1);
			model.indices.push_back(2);
			model.indices.push_back(11);
			model.indices.push_back(-1);

			model.indices.push_back(3); // Pivot on vertex 3
			model.indices.push_back(4);
			model.indices.push_back(5);
			model.indices.push_back(2);
			model.indices.push_back(-1);

			model.indices.push_back(6); // Pivot on vertex 6
			model.indices.push_back(7);
			model.indices.push_back(5);
			model.indices.push_back(-1);

			model.indices.push_back(9); // Pivot on vertex 9
			model.indices.push_back(10);
			model.indices.push_back(11);
			model.indices.push_back(7);
			model.indices.push_back(8);
			model.indices.push_back(-1);

			if (a > 1.37f)
			{
				model.indices.push_back(5);
				model.indices.push_back(7);
				model.indices.push_back(11);
				model.indices.push_back(2);
			}
			else
			{
				model.indices.push_back(7);
				model.indices.push_back(11);
				model.indices.push_back(2);
				model.indices.push_back(5);
			}
		}
		
		if (drawMode == GL_LINE_LOOP)
		{
			// Indices organized for a line loop rendering
			model.indices.reserve(model.vertices.size());
			for (int i = 0; i < model.vertices.size(); i++)
			{
				model.indices.push_back(i);
			}
		}

		for (int i = 0; i < model.vertices.size(); i++)
		{
			model.vertices[i] *= scale;
			model.vertices[i].x *= -1;
		}

		return model;
	}
}