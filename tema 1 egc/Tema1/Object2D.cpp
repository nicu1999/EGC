#include "Object2D.h"

#include <Core/Engine.h>

#define DEG2RAD 3.14159/180.0

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateBaloon(std::string name, glm::vec3 center, float radiusX, float radiusY, glm::vec3 color)
{
	int i;
	std::vector<VertexFormat> vertices;

	for (i = 0; i < 360; i++) {
		float rad = i * DEG2RAD;
		vertices.push_back(VertexFormat(center + glm::vec3(cos(rad) * radiusX, sin(rad) * radiusY, 0), color));
	}
	Mesh* baloon = new Mesh(name);
	
	std::vector<unsigned short> indices;
	for (i = 0; i < 360; i++) {
		indices.push_back(i);
	}
	baloon->SetDrawMode(GL_TRIANGLE_FAN);
	baloon->InitFromData(vertices, indices);
	return baloon;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 color)
{
	//    A
	// 	/	\
	// B-----C	
	int i;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(A, color),
		VertexFormat(B, color),
		VertexFormat(C, color),
	};
	Mesh* triangle = new Mesh(name);

	std::vector<unsigned short> indices = { 0, 1, 2 };

	triangle->SetDrawMode(GL_TRIANGLES);
	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 middleString, glm::vec3 topString, glm::vec3 bottomString, float radius, glm::vec3 color)
{
	int i;
	std::vector<VertexFormat> vertices;
	glm::vec3 center = middleString;
	center.x = (topString.x + bottomString.x) / 2;
	center.y = (topString.y + bottomString.y) / 2;
	float radiusX = radius;
	float radiusY = center.y - topString.y;
	vertices.push_back(VertexFormat(middleString, color));
	vertices.push_back(VertexFormat(bottomString, color));
	for (i = 270; i < 450; i++) {
		float rad = i * DEG2RAD;
		vertices.push_back(VertexFormat(center + glm::vec3(cos(rad) * radiusX, sin(rad) * radiusY, 0), color));
	}
	vertices.push_back(VertexFormat(topString, color));
	
	Mesh* bow = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1};
	for (i = 2; i < 182; i++) {
		indices.push_back(i);
	}
	indices.push_back(183);

	bow->SetDrawMode(GL_LINE_LOOP);
	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreateArrow(std::string name, glm::vec3 end, glm::vec3 tip, glm::vec3 color)
{
	int i;

	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(tip, color));
	vertices.push_back(VertexFormat(end, color));


	Mesh* bow = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1 };

	bow->SetDrawMode(GL_LINES);
	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill) {


	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(- width / 2, -height / 2, 0), color),
		VertexFormat(center + glm::vec3(-width / 2, height / 2, 0), color),
		VertexFormat(center + glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(center + glm::vec3(width / 2, -height / 2, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}