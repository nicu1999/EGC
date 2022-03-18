#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{
	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateBaloon(std::string name, glm::vec3 center, float radiusX, float radiusY, glm::vec3 color);
	Mesh* CreateTriangle(std::string name, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 color);
	Mesh* CreateBow(std::string name, glm::vec3 middleString, glm::vec3 topString, glm::vec3 bottomString, float radius, glm::vec3 color);
	Mesh* CreateArrow(std::string name, glm::vec3 end, glm::vec3 tip, glm::vec3 color);
	Mesh* CreateRectangle(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill);

}

