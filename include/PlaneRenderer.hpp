#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/common.hpp>

#include "glslprogram.hpp"
#include "Monotile.hpp"

namespace monotile
{
	class PlaneRenderer
	{
	public:
		PlaneRenderer(GLFWwindow* window);

		void Initialize(int totalHexs);
		void Update(double dt);
		void Render();

	private:
		void GenHexs(int totalHexs);
		void GenMonos(int total, float a, float offset);
		void GenBuffers();
		void GenConnections();
		void InitializeConnections();
		void UpdateBuffers();

		void ProcessInput();
		bool LineIntersection(glm::vec3 a0, glm::vec3 a1, glm::vec3 b0, glm::vec3 b1);
		bool CounterClockWise(glm::vec3 a, glm::vec3 b, glm::vec3 c);

		void Debug();

		GLuint vaoID;
		GLuint primitiveRestartIndex;
		unsigned int handle[3];

		std::vector<vec3> vertices;
		std::vector<unsigned int> indices;
		std::vector<vec3> colors;
		unsigned int currentIndex;

		GLSLProgram shader;
		GLFWwindow* window;

		glm::mat4 modelViewProjectionMatrix;

		bool wireframe = false;
		float speed;
		float baseSpeed;
		int totalTiles;

		float side;
		std::vector<std::pair<unsigned int, unsigned int>> connections;
	};
}