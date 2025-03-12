#include "Renderer.hpp"
#include <iostream>

namespace monotile
{
	char keyOnce[GLFW_KEY_LAST + 1];
	#define glfwGetKeyOnce(WINDOW, KEY)             \
		(glfwGetKey(WINDOW, KEY) ?              \
		 (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
		 (keyOnce[KEY] = false))

	Renderer::Renderer(GLFWwindow* window)
	{
		this->window = window;
		vaoID = -1;
		primitiveRestartIndex = -1;
		currentIndex = 0;
		side = 1.0f;
		baseSpeed = 0.2f;
		speed = baseSpeed;
		totalTiles = 1;
	}

	void Renderer::Initialize(int totalHexs)
	{
		try {
			shader.compileShader("shader/Plane.vert", GLSLShader::VERTEX);
			shader.compileShader("shader/Plane.frag", GLSLShader::FRAGMENT);

			shader.link();
			shader.use();
		}
		catch (GLSLProgramException& e) {
			std::cerr << e.what() << std::endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
		shader.printActiveAttribs();

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(primitiveRestartIndex);

		//GenHexs(totalHexs);
		GenMonos(totalTiles, side, 0.3f);
		InitializeConnections();
		GenBuffers();

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::lookAt(
			vec3(0.0f, 0.0f, -1.0f), //eye
			vec3(0.0f, 0.0f, 0.0f), //center
			vec3(0.0f, 1.0f, 0.0f)); //up
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);

		modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	}

	void Renderer::Update(double dt)
	{
		ProcessInput();

		side += speed * dt;
		if (side > 1 + sqrt(3.f) - 0.0001f)
		{
			side -= speed * dt;
			side = 0.0f;
			//speed = 0.0;
		}
		else if (side < 0.0001f)
		{
			side -= speed * dt;
			side = 1 + sqrt(3.f) - 0.01f;
			//speed = 0.0;
		}
		GenMonos(totalTiles, side, 0.3f);
		//GenConnections();
		UpdateBuffers();
	}

	void Renderer::Render()
	{
		shader.use();
		shader.setUniform("MVP", modelViewProjectionMatrix);

		glEnable(GL_PRIMITIVE_RESTART);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
		//glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
		glBindVertexArray(0);
	}

	void Renderer::GenHexs(int totalHexs)
	{
		Monotile monotile(totalHexs);

		int verticesSize = monotile.models[0].vertices.size();
		int indicesSize = monotile.models[0].indices.size();

		for (int i = 0; i < totalHexs; i++)
		{
			vertices.insert(vertices.end(), 
				std::make_move_iterator(monotile.models[i].vertices.begin()), 
				std::make_move_iterator(monotile.models[i].vertices.end()));

			for (int j = 0; j < indicesSize; j++)
			{
				indices.push_back(monotile.models[i].indices[j] + currentIndex);
			}

			monotile.models[i].vertices.erase(monotile.models[i].vertices.begin(), monotile.models[i].vertices.end());
			monotile.models[i].indices.erase(monotile.models[i].indices.begin(), monotile.models[i].indices.end());

			currentIndex += verticesSize;
			indices.push_back(primitiveRestartIndex);
		}
	}

	void Renderer::GenMonos(int total, float a, float offset)
	{
		vertices.clear();
		indices.clear();
		colors.clear();
		currentIndex = 0;

		for (int i = 0; i < total; i++)
		{
			Model model = Monotile::GetMonotileVertices(glm::vec3(-2.5f, -1.5f, 0.f), a + offset * i, GL_TRIANGLE_FAN);

			vertices.insert(vertices.end(), model.vertices.begin(), model.vertices.end());
			for (unsigned int index : model.indices)
			{
				if (index != primitiveRestartIndex)
					indices.push_back(index + currentIndex);
				else
					indices.push_back(primitiveRestartIndex);
				colors.push_back(vec3(1.0f, 0.0f, 0.0f));
			}

			model.vertices.erase(model.vertices.begin(), model.vertices.end());
			model.indices.erase(model.indices.begin(), model.indices.end());

			currentIndex = vertices.size();
			indices.push_back(primitiveRestartIndex);
		}
	}

	void Renderer::GenConnections()
	{
		std::vector<std::pair<unsigned int, unsigned int>> newConnections;

		for (auto& kv : connections)
		{
			bool collided = false;
			for (int i = 0; i < vertices.size(); i++)
			{
				glm::vec3 v1 = vertices[i];
				glm::vec3 v2 = vertices[(i + 1) % vertices.size()];

				if (i == kv.second || i + 1 == kv.first)
					continue;

				if (LineIntersection(v1, v2, vertices[kv.first], vertices[kv.second]))
				{
					collided = true;
					break;
				}
			}
			if (!collided)
				newConnections.push_back(std::make_pair(kv.first, kv.second));
		}

		currentIndex = vertices.size();
		connections = newConnections;

		for (auto& kv : connections)
		{
			vertices.push_back(vertices[kv.first]);
			vertices.push_back(vertices[kv.second]);
			colors.push_back(vec3(0.f, 0.8f, 0.0f));
			colors.push_back(vec3(0.f, 0.8f, 0.0f));
			indices.push_back(currentIndex);
			indices.push_back(currentIndex+1);
			indices.push_back(primitiveRestartIndex);

			currentIndex += 2;
		}
	}

	void Renderer::InitializeConnections()
	{
		connections.clear();
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			for (unsigned int j = i + 2; j < vertices.size(); j++)
			{
				if ((i == 1 && j == 3) || (i == 4 && j == 6) || (i == 6 && j == 8) || (i == 10 && j == 12))
					continue;
				connections.push_back(std::make_pair(i, j));
			}
		}
	}

	bool Renderer::LineIntersection(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		return CounterClockWise(p0, p2, p3) != CounterClockWise(p1, p2, p3) &&
			CounterClockWise(p0, p1, p2) != CounterClockWise(p0, p1, p3);
	}

	bool Renderer::CounterClockWise(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{
		return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
	}


	void Renderer::GenBuffers()
	{
		for (auto& vertex : vertices)
		{
			std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
		}
		for (auto& colors : colors)
		{
			std::cout << colors.x << " " << colors.y << " " << colors.z << std::endl;
		}
		for (auto index : indices)
		{
			std::cout << index << std::endl;
		}

		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		glGenBuffers(3, handle);

		glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), (GLvoid*)&vertices[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), (GLvoid*)&colors[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_DYNAMIC_DRAW);

		glBindVertexArray(0); 
	}

	void Renderer::UpdateBuffers()
	{
		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), (GLvoid*)&vertices[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), (GLvoid*)&colors[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	}

	void Renderer::Debug()
	{
		std::cout << "A | B " << side << " | " << 1 + sqrt(3.0f) - side << std::endl;

		std::cout << "Pares:\n";
		for (auto& pair : connections)
		{
			std::cout << pair.first << " | " << pair.second << std::endl;
		}
	}

	void Renderer::ProcessInput()
	{
		// toggle wireframe
		if (glfwGetKeyOnce(window, 'Q')) {
			if (speed == 0.0f)
				speed = baseSpeed;
			else
				speed = 0.0f;
		}
		if (glfwGetKeyOnce(window, 'W')) {
			if (speed == 0.0f)
				speed = -baseSpeed;
			else
				speed = 0.0f;
		}
		if (glfwGetKeyOnce(window, 'D')) {
			Debug();
		}
	}
}