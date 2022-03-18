#include "Tema1.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include <random>
#include "Transform2D.h"
#include "Object2D.h"
#include "Hitbox.h"

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

std::vector<glm::mat3> arrows;
std::vector<glm::mat3> shurikanes;
std::vector<glm::mat3> arrowsTips;
std::vector<glm::mat3> baloons;
std::vector<glm::mat3> baloonsbeingDestroyed;
std::vector<glm::mat3> baloonTriangles;
std::vector<glm::mat3> baloonStrings1;
std::vector<glm::mat3> baloonStrings2;
std::vector<float> arrowSpeed;
std::vector<float> shurikaneMidairTime;
std::vector<float> scaleFactor;
glm::vec3 bowCenter; //= glm::vec3(0, 0, 0);
glm::ivec2 resolution;
glm::mat3 bowMatrix;
float bowY;
float bowX;
float bowAngle;
float bowTimer = 1;
float bowCharge = 0;
float baloonTimer = 0;
float shurikaneTimer = 0;

int score = 0;

bool areYouDead = false;
bool isEmptyBow = false;

Mesh *Tema1::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	GLuint VBO_ID;
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Crete the IBO and bind it
	unsigned int IBO_ID = 0;
	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void *)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void *)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void *)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Tema1::DestroyAnimation()
{
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	float elipseX = 50;
	float elipseY = 50;

	bowY = resolution.y / 2;
	bowX = resolution.y / 10;
	bowAngle = 0;

	Mesh *bow = Object2D::CreateBow("bow", corner, corner + glm::vec3(0, 50, 0), corner + glm::vec3(0, -50, 0), 30, glm::vec3(0, 1, 0));
	AddMeshToList(bow);

	Mesh *arrow = Object2D::CreateArrow("arrow", corner, corner + glm::vec3(100, 0, 0), glm::vec3(0, 1, 0));
	AddMeshToList(arrow);

	Mesh *tip = Object2D::CreateTriangle("tip", corner + glm::vec3(0, 10, 0), corner + glm::vec3(0, -10, 0), corner + glm::vec3(10, 0, 0), glm::vec3(0, 1, 0));
	AddMeshToList(tip);

	Mesh *baloon = Object2D::CreateBaloon("baloon", corner, elipseX, elipseY, glm::vec3(0, 1, 0));
	AddMeshToList(baloon);

	Mesh *baloonTriangle = Object2D::CreateTriangle("baloonTriangle", corner, corner + glm::vec3(-20, -30, 0), corner + glm::vec3(20, -30, 0), glm::vec3(0, 1, 0));
	AddMeshToList(baloonTriangle);

	Mesh *string1 = Object2D::CreateArrow("string1", corner, corner + glm::vec3(20, -20, 0), glm::vec3(0, 1, 0));
	AddMeshToList(string1);
	Mesh *string2 = Object2D::CreateArrow("string2", corner, corner + glm::vec3(-20, -20, 0), glm::vec3(0, 1, 0));
	AddMeshToList(string2);

	Mesh *rectangle1 = Object2D::CreateRectangle("rectangle1", corner, 20, 70, glm::vec3(1, 1, 1), false);
	AddMeshToList(rectangle1);

	Mesh *rectangle2 = Object2D::CreateRectangle("rectangle2", corner, 18, 68, glm::vec3(0, 0, 1), true);
	AddMeshToList(rectangle2);

	Mesh *rectangle3 = Object2D::CreateRectangle("rectangle3", corner, 20, 70, glm::vec3(1, 1, 0), true);
	AddMeshToList(rectangle3);

	Mesh *triangle = Object2D::CreateTriangle("triangle", corner, corner + glm::vec3(-20, -20, 0), corner + glm::vec3(0, -40, 0), glm::vec3(0, 1, 0));
	AddMeshToList(triangle);

	Mesh *triangleDebug = Object2D::CreateTriangle("triangleDebug", corner + glm::vec3(0, 10, 0), corner + glm::vec3(-10, -10, 0), corner + glm::vec3(10, -10, 0), glm::vec3(0.5, 1, 1));
	AddMeshToList(triangleDebug);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	if (areYouDead)
	{
		printf("You are bad and you should feel bad");
		exit(0);
	}

	baloonTimer += deltaTimeSeconds;
	shurikaneTimer += deltaTimeSeconds;

	if (bowTimer < 1)
	{
		bowTimer += deltaTimeSeconds;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bowX, bowY);
	modelMatrix *= Transform2D::Rotate(bowAngle);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bowX, bowY - 70);
	RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bowX, bowY - 70);
	modelMatrix *= Transform2D::Scale(bowTimer, 1);
	RenderMesh2D(meshes["rectangle2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bowX, bowY - 110);
	RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bowX, bowY - 110);
	modelMatrix *= Transform2D::Scale(bowCharge, 1);
	RenderMesh2D(meshes["rectangle3"], shaders["VertexColor"], modelMatrix);

	if (bowTimer > 1)
	{
		isEmptyBow = false;
	}

	if (baloonTimer > 1.5)
	{
		float randNr = rand() % 25;
		randNr = randNr - 12.5;
		randNr *= 0.01;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(resolution.x * (0.75 + randNr), 0);
		baloons.push_back(modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(resolution.x * (0.75 + randNr), 0);
		modelMatrix *= Transform2D::Translate(0, -30);
		baloonTriangles.push_back(modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(resolution.x * (0.75 + randNr), 0);
		modelMatrix *= Transform2D::Translate(0, -60);
		baloonStrings1.push_back(modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(resolution.x * (0.75 + randNr), 0);
		modelMatrix *= Transform2D::Translate(20, -80);
		baloonStrings2.push_back(modelMatrix);

		baloonTimer = 0;
	}

	if (shurikaneTimer > 2)
	{

		float randNr = rand() % 100;
		randNr = randNr * 0.01;
		modelMatrix = glm::mat3(1);

		modelMatrix *= Transform2D::Translate(resolution.x, resolution.y * randNr);

		shurikanes.push_back(modelMatrix);

		modelMatrix *= Transform2D::Rotate(M_PI / 2);

		shurikanes.push_back(modelMatrix);

		modelMatrix *= Transform2D::Rotate(M_PI / 2);

		shurikanes.push_back(modelMatrix);

		modelMatrix *= Transform2D::Rotate(M_PI / 2);

		shurikanes.push_back(modelMatrix);

		shurikaneMidairTime.push_back(0);

		shurikaneTimer = 0;
	}

	if (!isEmptyBow)
	{
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bowX, bowY);
		modelMatrix *= Transform2D::Rotate(bowAngle);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bowX + 100, bowY);
		modelMatrix *= Transform2D::Translate(-100, 0);
		modelMatrix *= Transform2D::Rotate(bowAngle);
		modelMatrix *= Transform2D::Translate(100, 0);
		RenderMesh2D(meshes["tip"], shaders["VertexColor"], modelMatrix);
	}

	for (int i = 0; i < baloons.size();)
	{
		if (baloons.at(i)[2][1] > resolution.y + 100)
		{
			baloons.erase(baloons.begin() + i);
			baloonStrings1.erase(baloonStrings1.begin() + i);
			baloonStrings2.erase(baloonStrings2.begin() + i);
			baloonTriangles.erase(baloonTriangles.begin() + i);
		}
		else
		{
			baloons.at(i) *= Transform2D::Translate(0, deltaTimeSeconds * 200);
			baloonTriangles.at(i) *= Transform2D::Translate(0, deltaTimeSeconds * 200);
			baloonStrings1.at(i) *= Transform2D::Translate(0, deltaTimeSeconds * 200);
			baloonStrings2.at(i) *= Transform2D::Translate(0, deltaTimeSeconds * 200);
			RenderMesh2D(meshes["baloon"], shaders["VertexColor"], baloons.at(i));
			RenderMesh2D(meshes["baloonTriangle"], shaders["VertexColor"], baloonTriangles.at(i));
			RenderMesh2D(meshes["string1"], shaders["VertexColor"], baloonStrings1.at(i));
			RenderMesh2D(meshes["string2"], shaders["VertexColor"], baloonStrings2.at(i));
			i++;
		}
	}

	for (int i = 0; i < arrows.size();)
	{
		if ((arrows.at(i)[2][0] > resolution.x + 100) || (arrows.at(i)[2][1] > resolution.y + 100))
		{
			arrows.erase(arrows.begin() + i);
			arrowsTips.erase(arrowsTips.begin() + i);
			arrowSpeed.erase(arrowSpeed.begin() + i);
		}
		else
		{
			arrows.at(i) *= Transform2D::Translate(deltaTimeSeconds * (150 + arrowSpeed.at(i)), 0);
			arrowsTips.at(i) *= Transform2D::Translate(deltaTimeSeconds * (150 + arrowSpeed.at(i)), 0);
			RenderMesh2D(meshes["arrow"], shaders["VertexColor"], arrows.at(i));
			RenderMesh2D(meshes["tip"], shaders["VertexColor"], arrowsTips.at(i));
			i++;
		}
	}
	for (int i = 0; i < shurikanes.size(); i += 4)
	{
		shurikaneMidairTime.at(i / 4) += deltaTimeSeconds * (300);

		shurikanes.at(i) *= Transform2D::Translate(-deltaTimeSeconds * (300), 0);
		shurikanes.at(i + 1) *= Transform2D::Translate(0, deltaTimeSeconds * (300));
		shurikanes.at(i + 2) *= Transform2D::Translate(deltaTimeSeconds * (300), 0);
		shurikanes.at(i + 3) *= Transform2D::Translate(0, -deltaTimeSeconds * (300));

		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], shurikanes.at(i));
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], shurikanes.at(i + 1));
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], shurikanes.at(i + 2));
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], shurikanes.at(i + 3));
	}

	for (int i = 0; i < arrowsTips.size(); i++)
	{
		for (int j = 0; j < baloons.size();)
		{

			float x = (arrowsTips.at(i)[2][0] - baloons.at(j)[2][0]) * (arrowsTips.at(i)[2][0] - baloons.at(j)[2][0]) +
					  (arrowsTips.at(i)[2][1] - baloons.at(j)[2][1]) * (arrowsTips.at(i)[2][1] - baloons.at(j)[2][1]);
			float distance = sqrt(x);
			if (distance < 50)
			{
				baloonsbeingDestroyed.push_back(baloons.at(j));
				scaleFactor.push_back(1);

				baloons.erase(baloons.begin() + j);
				baloonStrings1.erase(baloonStrings1.begin() + j);
				baloonStrings2.erase(baloonStrings2.begin() + j);
				baloonTriangles.erase(baloonTriangles.begin() + j);

				score++;
				printf("Scor: %d \n", score);
			}
			else
			{
				j++;
			}
		}
	}

	for (int i = 0; i < shurikanes.size(); i++)
	{
		float box1BottomLeftX = shurikanes.at(i)[2][0] - 40;
		float box2TopRightX = bowX + 30;
		float box1TopRightX = shurikanes.at(i)[2][0] + 40;
		float box2BottonLeftX = bowX;
		float box1BottomLeftY = shurikanes.at(i)[2][1] - 40;
		float box2TopRightY = bowY + 50;
		float box1TopRightY = shurikanes.at(i)[2][1] + 40;
		float box2BottomLeftY = bowY - 50;

		if (
			(box1BottomLeftX < box2TopRightX) &&
			(box1TopRightX > box2BottonLeftX) &&
			(box1BottomLeftY < box2TopRightY) &&
			(box1TopRightY > box2BottomLeftY))
		{
			areYouDead = true;
		}
	}

	for (int i = 0; i < baloonsbeingDestroyed.size();)
	{
		scaleFactor.at(i) -= deltaTimeSeconds;
		if (scaleFactor.at(i) > 0)
		{
			baloonsbeingDestroyed.at(i) *= Transform2D::Scale(scaleFactor.at(i), scaleFactor.at(i));
			RenderMesh2D(meshes["baloon"], shaders["VertexColor"], baloonsbeingDestroyed.at(i));
			i++;
		}
		else
		{
			baloonsbeingDestroyed.erase(baloonsbeingDestroyed.begin() + i);
			scaleFactor.erase(scaleFactor.begin() + i);
		}
	}
	// printf("%f\n", bowCharge);
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W))
	{
		bowY += deltaTime * 250;
	}
	if (window->KeyHold(GLFW_KEY_S))
	{
		bowY -= deltaTime * 250;
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (bowCharge < 1)
			bowCharge += deltaTime * 0.7;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	int adjustedMouseX;
	int adjustedMouseY;
	adjustedMouseX = mouseX - bowX;
	adjustedMouseY = bowY - mouseY;

	if (deltaY > 0)
	{
		adjustedMouseY += deltaY; // bowY - mouseY;
	}

	if (deltaX > 0)
	{
		adjustedMouseX += deltaY; // bowY - mouseY;
	}

	// float z = (adjustedMouseX * adjustedMouseX + adjustedMouseY * adjustedMouseY);
	//  z = sqrt(z);
	// bowAngle = asin(adjustedMouseY / z);
	bowAngle = atan2(adjustedMouseY, adjustedMouseX);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if ((button == 1) && !isEmptyBow)
	{

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bowX, bowY);
		modelMatrix *= Transform2D::Rotate(bowAngle);
		arrows.push_back(modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bowX + 100, bowY);
		modelMatrix *= Transform2D::Translate(-100, 0);
		modelMatrix *= Transform2D::Rotate(bowAngle);
		modelMatrix *= Transform2D::Translate(100, 0);
		arrowsTips.push_back(modelMatrix);

		arrowSpeed.push_back(bowCharge * 250);
		bowTimer = 0;
		bowCharge = 0;

		isEmptyBow = true;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}