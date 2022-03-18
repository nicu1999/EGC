#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

class Pair {
public:
	glm::mat4 mat;
	glm::vec2 vec;
	Pair(glm::mat4 m, glm::vec2 v) {
		mat = m;
		vec = v;
	}
};


float timeElapsed = 0;
float spawnPlatformTime = 0;
float spawningTime = 2;
bool lastLanes[] = {true,true,true};
std::vector<Pair> platforms;
std::vector<float> platformTimes;
float characterX;
float characterSpeed = 0;
float timeTillDeath = 0;
float inAirTime = 0;
float combustibil = 100;
int platformMatrix[10][3];
int nextMove;
int nrOfRemovedPlatforms;
int standingPanel = 0;
bool canPowerUp = true;
bool firstPerson = false;
bool updateCamera = false;
bool dead = false;
bool isJumping = false;
Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void matrixInit(int platformMatrix[][3]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			platformMatrix[i][j] = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		platformMatrix[0][i] = 1;
		platformMatrix[1][i] = 1;
	}
	int col = rand() % 3; //poszitia initiala
	int row = 2; // al doilea rand
	for (row; row < 10; row++) {
		nextMove = rand() % 3;
		if (nextMove < col) {
			for (int i = nextMove; i <= col; i++) {
				platformMatrix[row][i] = 1;
				if ((rand() % 10) < 1) {
					platformMatrix[row][i] = 3;
				}
				if ((rand() % 10) < 1) {
					platformMatrix[row][i] = 4;
				}
				if ((rand() % 10) < 1) {
					platformMatrix[row][i] = 5;
				}
			}
		}
		else if (nextMove == col) {
			platformMatrix[row][col] = 1;
			if ((rand() % 10) < 1) {
				platformMatrix[row][col] = 3;
			}
			if ((rand() % 10) < 1) {
				platformMatrix[row][col] = 4;
			}
			if ((rand() % 10) < 1) {
				platformMatrix[row][col] = 5;
			}
		}
		else if (nextMove > col) {
			for (int i = col; i <= nextMove; i++) {
				platformMatrix[row][i] = 1;
				if ((rand() % 10) < 1) {
					platformMatrix[row][i] = 3;
				}
				if ((rand() % 10) < 1) {
					platformMatrix[row][i] = 4;
				}
				if ((rand() % 10) < 1) {
					platformMatrix[row][i] = 5;
				}
			}
		}
		col = nextMove;
	}
}

void matrixUpdate(int platformMatrix[][3]) {
	int aux1, aux2, aux3;
	nrOfRemovedPlatforms = 0;
	for (int i = 0; i < 3; i++) {
		if ( platformMatrix[0][i] != 0) {
			nrOfRemovedPlatforms++;
		}
	}
	for(int i = 1; i < 10 ; i++) {
		platformMatrix[i - 1][0] = platformMatrix[i][0];
		platformMatrix[i - 1][1] = platformMatrix[i][1];
		platformMatrix[i - 1][2] = platformMatrix[i][2];
	}

	for (int i = nrOfRemovedPlatforms; i < platforms.size(); i++) {
		platforms.at(i).vec.x -= 1;
	}

	for (int i = 0; i < 3; i++) {
		platformMatrix[9][i] = 0;
	}
	int col = nextMove;
	nextMove = rand() % 3;
	if (nextMove < col) {
		for (int i = nextMove; i <= col; i++) {
			platformMatrix[9][i] = 1;
			if ((rand() % 10) < 1) {
				platformMatrix[9][i] = 3;
			}
			if ((rand() % 10) < 1) {
				platformMatrix[9][i] = 4;
			}
			if ((rand() % 10) < 1) {
				platformMatrix[9][i] = 5;
			}
		}
	}
	else if (nextMove == col) {
		platformMatrix[9][col] = 1;
		if ((rand() % 10) < 1) {
			platformMatrix[9][col] = 3;
		}
		if ((rand() % 10) < 1) {
			platformMatrix[9][col] = 4;
		}
		if ((rand() % 10) < 1) {
			platformMatrix[9][col] = 5;
		}
	}
	else if (nextMove > col) {
		for (int i = col; i <= nextMove; i++) {
			platformMatrix[9][i] = 1;
			if ((rand() % 10) < 1) {
				platformMatrix[9][i] = 3;
			}
			if ((rand() % 10) < 1) {
				platformMatrix[9][i] = 4;
			}
			if ((rand() % 10) < 1) {
				platformMatrix[9][i] = 5;
			}
		}
	}
}

void Tema2::Init()
{
	{
		srand(time(NULL));
		EngineComponents::Camera* camera = GetSceneCamera();
		camera->SetPosition(glm::vec3(1,1.3,21));
	}

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		mesh = new Mesh("character");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple cube
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cube", vertices, indices);
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("Shader1");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("Shader2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader2.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("CharacterShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/CharacterVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/CharacterFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("Shader3");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader3.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader3.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("Shader4");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader4.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader4.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;


		shader = new Shader("Shader5");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader5.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader5.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		matrixInit(platformMatrix);
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 3; j++) {
				if ( (platformMatrix[i][j] == 1) || (platformMatrix[i][j] == 3) || (platformMatrix[i][j] == 4) || (platformMatrix[i][j] == 5) ) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(j * (1 + 0.1), 0, 2.2*(9-i) ));
					platforms.push_back(Pair(modelMatrix, glm::vec2(i, j)));
					platformTimes.push_back(0);
				}
			}
		}
	}
}

Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec2)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	combustibil -= deltaTimeSeconds*2;
	if (combustibil < 0) {
		exit(0);
	}

	glm::mat4 interface = glm::mat4(1);
	interface = glm::translate(interface, glm::vec3(1, 2, 18.7));
	interface = glm::scale(interface, glm::vec3(0.5, 0.25, 0));
	RenderSimpleMesh(meshes["cube"], shaders["Shader3"], interface);
	interface = glm::mat4(1);
	interface = glm::translate(interface, glm::vec3(1, 2, 18.71));
	interface = glm::scale(interface, glm::vec3(0.48 * (combustibil / 100), 0.23, 0));
	RenderSimpleMesh(meshes["cube"], shaders["Shader4"], interface);

	//cout << combustibil << "\n";
	clock = Engine::GetElapsedTime();
	timeElapsed += deltaTimeSeconds;
	spawnPlatformTime += deltaTimeSeconds;
	EngineComponents::Camera* camera = GetSceneCamera();
	if ( (firstPerson == true) && (updateCamera == true) ) {
		camera->SetPosition(glm::vec3(characterX, 0.25, 18.5));
		updateCamera = false;
	}
	else if ((firstPerson == false) && (updateCamera == true)) {
		camera->SetPosition(glm::vec3(1, 1.3, 21));
		updateCamera = false;
	}
	

	if (spawnPlatformTime > spawningTime) {
		matrixUpdate(platformMatrix);
		canPowerUp = true;
		for (int i = 0; i < nrOfRemovedPlatforms; i++) {
			platforms.erase(platforms.begin());
			platformTimes.erase(platformTimes.begin());
		}
		spawnPlatformTime = 0;
		for (int j = 0; j < 3; j++) {
			if ( (platformMatrix[9][j] == 1) || (platformMatrix[9][j] == 3) || (platformMatrix[9][j] == 4) || (platformMatrix[9][j] == 5)) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(j * (1 + 0.1), 0, 0));
				platforms.push_back(Pair (modelMatrix, glm::vec2(9, j)));
				platformTimes.push_back(0);
			}
		}
	}

	{
		if (!firstPerson && !dead && !isJumping) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(characterX, 0.25, 18.7));

			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
			RenderSimpleMesh(meshes["character"], shaders["CharacterShader"], modelMatrix);
		}
		if (!firstPerson && dead && !isJumping) {
			timeTillDeath += deltaTimeSeconds;
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(characterX, 0.25, 18.5));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -timeTillDeath, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
			RenderSimpleMesh(meshes["character"], shaders["CharacterShader"], modelMatrix);
			if (timeTillDeath > 1) {
				exit(0);
			}
		}
		if (firstPerson && dead && !isJumping) {
			timeTillDeath += deltaTimeSeconds;
			camera->Update();
			camera->MoveDown(deltaTimeSeconds);
			if (timeTillDeath > 1) {
				exit(0);
			}
		}
		if (!firstPerson && !dead && isJumping) {
			inAirTime += deltaTimeSeconds;
			if (inAirTime > 2.7) {
				isJumping = false;
				inAirTime = 0;
			}
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(characterX, 0.25 + sin(inAirTime * M_PI / 2.7), 18.5));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
			RenderSimpleMesh(meshes["character"], shaders["CharacterShader"], modelMatrix);
		}
		if (firstPerson && !dead && isJumping) {
			inAirTime += deltaTimeSeconds;
			if (inAirTime > 2.7) {
				isJumping = false;
				inAirTime = 0;
			}
			camera->Update();
			camera->SetPosition(glm::vec3(characterX, 0.25 + sin(inAirTime * M_PI / 2.7), 18.5));
		}
	}

	int ctr = 0;

	for (int i = 0; i < platforms.size(); i++) {
		Pair p = platforms.at(i);
		glm::mat4 modelMatrix = p.mat;
		platformTimes.at(i) += deltaTimeSeconds;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.1, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, platformTimes.at(i) * 1.1));
		if (platformMatrix[(int)p.vec.x][(int)p.vec.y] == 1) { //neatinese
			RenderSimpleMesh(meshes["cube"], shaders["Shader1"], modelMatrix);
		}
		else if (platformMatrix[(int)p.vec.x][(int)p.vec.y] == 2) { //atinse
			RenderSimpleMesh(meshes["cube"], shaders["Shader2"], modelMatrix);
		}
		else if (platformMatrix[(int)p.vec.x][(int)p.vec.y] == 3) { // -cobustibil
			RenderSimpleMesh(meshes["cube"], shaders["Shader3"], modelMatrix);
		}
		else if (platformMatrix[(int)p.vec.x][(int)p.vec.y] == 4) { // +cobustibil
			RenderSimpleMesh(meshes["cube"], shaders["Shader4"], modelMatrix);
		}
		else if (platformMatrix[(int)p.vec.x][(int)p.vec.y] == 5) { // ded
			RenderSimpleMesh(meshes["cube"], shaders["Shader5"], modelMatrix);
		}
	}

	{
		if (characterX < -0.5)
			standingPanel = -1;
		if ((characterX >= -0.5) && (characterX <= 0.56))
			standingPanel = 0;
		if ((characterX > 0.56) && (characterX < 1.65))
			standingPanel = 1;
		if ((characterX >= 1.65) && (characterX <= 2.71))
			standingPanel = 2;
		if ((characterX > 2.71))
			standingPanel = 3;

		if (!isJumping) {
			if ((standingPanel == -1) || (standingPanel == 3)) {
				dead = true;
			}
			else if (platformMatrix[1][standingPanel] == 0) {
				dead = true;
			}
			else if (platformMatrix[1][standingPanel] == 5) {
				exit(0);
			}
			else if (platformMatrix[1][standingPanel] == 3) {
				if (canPowerUp) {
					canPowerUp = false;
					combustibil -= 20;
				}
				platformMatrix[1][standingPanel] = 2;
			}
			else if (platformMatrix[1][standingPanel] == 4) {
				if (canPowerUp) {
					canPowerUp = false;
					if (combustibil + 20 > 100) {
						combustibil = 100;
					} else
						combustibil += 20;
				}
				platformMatrix[1][standingPanel] = 2;
			}
			else {
				platformMatrix[1][standingPanel] = 2;
			}
		}
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int location_model = glGetUniformLocation(shader->program, "Model");

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	int location_view = glGetUniformLocation(shader->program, "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(location_view, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	int location_projection = glGetUniformLocation(shader->program, "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(location_projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int location_clock = glGetUniformLocation(shader->program, "Clock");
	glUniform1f(location_clock, abs(sinf(clock)) + abs(sinf(clock)));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}



// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	EngineComponents::Camera* camera = GetSceneCamera();
	if (window->KeyHold(GLFW_KEY_A) && !dead) {
		characterX -= deltaTime * 1.7;
		if (firstPerson) {
			camera->Update();
			camera->MoveLeft(deltaTime);
		}
	}
	if (window->KeyHold(GLFW_KEY_D) && !dead) {
		characterX += deltaTime * 1.7;
		if (firstPerson) {
			camera->Update();
			camera->MoveRight(deltaTime);
		}
	}
	if (window->KeyHold(GLFW_KEY_W) && !dead) {
		characterSpeed += deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S) && !dead) {
		characterSpeed -= deltaTime;
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C) {
		firstPerson = !firstPerson;
		updateCamera = true;
	}
	if (key == GLFW_KEY_SPACE) {
		isJumping = true;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}