#pragma once
#include <string>
#include <include/glm.h>

class Hitbox
{
public:
	glm::vec3 bottomLeft;
	glm::vec3 topRight;
	std::string name;
	Hitbox(std::string name, glm::vec3 bottomLeft, glm::vec3 topRight);
	~Hitbox();
	bool isCollision(Hitbox box1, Hitbox box2);
};