#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

uniform mat4 MVP;

out vec3 color;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0);
	color = VertexColor;
}