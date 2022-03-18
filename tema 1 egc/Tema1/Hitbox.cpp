#include "Hitbox.h"
#include <string>
#include <Core/Engine.h>
#include <include/glm.h>

Hitbox::Hitbox(std::string name, glm::vec3 botLeft, glm::vec3 tpRight)
{
	name = name;
	bottomLeft = botLeft;
	topRight = tpRight;
}

Hitbox::~Hitbox() {}

bool Hitbox::isCollision(Hitbox box1, Hitbox box2)
{
	if (
		(box1.bottomLeft.x < box2.topRight.x) &&
		(box1.topRight.x > box2.bottomLeft.x) &&
		(box1.bottomLeft.y < box2.topRight.y) &&
		(box1.topRight.y > box2.bottomLeft.y))
		return true;
	else
		return false;
}