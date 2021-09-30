#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// struct for storing the obj file
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

Model loadOBJ(const char* objPath)
{
	// function to load the obj file
	// Note: this simple function cannot load all obj files.

	struct V {
		// struct for identify if a vertex has showed up
		unsigned int index_position, index_uv, index_normal;
		bool operator == (const V& v) const {
			return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
		}
		bool operator < (const V& v) const {
			return (index_position < v.index_position) ||
				(index_position == v.index_position && index_uv < v.index_uv) ||
				(index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
		}
	};

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::map<V, unsigned int> temp_vertices;

	Model model;
	unsigned int num_vertices = 0;

	std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

	std::ifstream file;
	file.open(objPath);

	// Check for Error
	if (file.fail()) {
		std::cerr << "Impossible to open the file! Do you use the right path? See Tutorial 6 for details" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		// process the object file
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			// geometric vertices
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			// texture coordinates
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			// vertex normals
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			// Face elements
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			// Check if there are more than three vertices in one face.
			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					// the vertex never shows before
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					model.vertices.push_back(vertex);
					model.indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					// reuse the existing vertex
					unsigned int index = temp_vertices[vertices[i]];
					model.indices.push_back(index);
				}
			} // for
		} // else if
		else {
			// it's not a vertex, texture coordinate, normal or face
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();

	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
	return model;
}

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

Shader spacecraftShader, planetShader, rockShader[200], 
	universeShader_1, universeShader_2, universeShader_3, universeShader_4, universeShader_5, universeShader_6,
	alienpeopleShader_1, alienpeopleShader_2, alienpeopleShader_3,
	alienvehicleShader_1, alienvehicleShader_2, alienvehicleShader_3,
	chickenShader_1, chickenShader_2, chickenShader_3, icecreamShader;

Model spacecraftObj, planetObj, rockObj[200],
	universeObj_1, universeObj_2, universeObj_3, universeObj_4, universeObj_5, universeObj_6,
	alienpeopleObj_1, alienpeopleObj_2, alienpeopleObj_3,
	alienvehicleObj_1, alienvehicleObj_2, alienvehicleObj_3,
	chickenObj_1, chickenObj_2, chickenObj_3, icecreamObj;

GLuint spacecraftVAO, spacecraftEBO, spacecraftVBO,
	planetVAO, planetEBO, planetVBO,
	rockVAO[200], rockEBO[200], rockVBO[200],
	universeVAO_1, universeEBO_1, universeVBO_1,
	universeVAO_2, universeEBO_2, universeVBO_2,
	universeVAO_3, universeEBO_3, universeVBO_3,
	universeVAO_4, universeEBO_4, universeVBO_4,
	universeVAO_5, universeEBO_5, universeVBO_5,
	universeVAO_6, universeEBO_6, universeVBO_6,
	alienpeopleVAO_1, alienpeopleEBO_1, alienpeopleVBO_1,
	alienpeopleVAO_2, alienpeopleEBO_2, alienpeopleVBO_2,
	alienpeopleVAO_3, alienpeopleEBO_3, alienpeopleVBO_3,
	alienvehicleVAO_1, alienvehicleEBO_1, alienvehicleVBO_1,
	alienvehicleVAO_2, alienvehicleEBO_2, alienvehicleVBO_2,
	alienvehicleVAO_3, alienvehicleEBO_3, alienvehicleVBO_3,
	chickenVAO_1, chickenEBO_1, chickenVBO_1,
	chickenVAO_2, chickenEBO_2, chickenVBO_2,
	chickenVAO_3, chickenEBO_3, chickenVBO_3,
	icecreamVAO, icecreamEBO, icecreamVBO;

Texture spacecraftTexture[2], planetTexture[2], rockTexture[200],
	universeTexture_1, universeTexture_2, universeTexture_3, universeTexture_4, universeTexture_5, universeTexture_6,
	alienpeopleTexture_1, alienpeopleTexture_2, alienpeopleTexture_3,
	alienvehicleTexture_1[2], alienvehicleTexture_2[2], alienvehicleTexture_3[2],
	chickenTexture_1, chickenTexture_2, chickenTexture_3, icecreamTexture;

GLfloat radius[rock_num], theta[rock_num], z_val[rock_num];
bool set_t[rock_num];
int self_rotate;
int key_up, key_left, key_x, key_y, key_w;
int mouse_left;
int is_clicked;
double old_x, old_y;
bool up_pressed, down_pressed, left_pressed, right_pressed;

void sendDataToOpenGL()
{
	//icecream
	icecreamShader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	icecreamObj = loadOBJ("resources/icecream.obj");
	icecreamTexture.setupTexture("resources/texture/iceCream.bmp");

	//spacecraft
	spacecraftShader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	spacecraftObj = loadOBJ("resources/spacecraft.obj");
	spacecraftTexture[0].setupTexture("resources/texture/spacecraftTexture.bmp");
	spacecraftTexture[1].setupTexture("resources/texture/leisure_spacecraftTexture.bmp");

	//planet
	planetShader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	planetObj = loadOBJ("resources/planet.obj");
	planetTexture[0].setupTexture("resources/texture/planetTexture.bmp");
	planetTexture[1].setupTexture("resources/texture/planetNormal.bmp");

	//universe
	universeShader_1.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	universeObj_1 = loadOBJ("resources/universe.obj");
	universeTexture_1.setupTexture("resources/texture/universe skybox/back.bmp");

	universeShader_2.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	universeObj_2 = loadOBJ("resources/universe.obj");
	universeTexture_2.setupTexture("resources/texture/universe skybox/bottom.bmp");

	universeShader_3.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	universeObj_3 = loadOBJ("resources/universe.obj");
	universeTexture_3.setupTexture("resources/texture/universe skybox/front.bmp");

	universeShader_4.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	universeObj_4 = loadOBJ("resources/universe.obj");
	universeTexture_4.setupTexture("resources/texture/universe skybox/left.bmp");

	universeShader_5.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	universeObj_5 = loadOBJ("resources/universe.obj");
	universeTexture_5.setupTexture("resources/texture/universe skybox/right.bmp");

	universeShader_6.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	universeObj_6 = loadOBJ("resources/universe.obj");
	universeTexture_6.setupTexture("resources/texture/universe skybox/top.bmp");

	//rock
	for (int i = 0; i < rock_num; i++) {
		rockShader[i].setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
		rockObj[i] = loadOBJ("resources/rock.obj");
		rockTexture[i].setupTexture("resources/texture/rockTexture.bmp");
	}



	//chicken
	chickenShader_1.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	chickenObj_1 = loadOBJ("resources/chicken.obj");
	chickenTexture_1.setupTexture("resources/texture/chickenTexture.bmp");

	chickenShader_2.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	chickenObj_2 = loadOBJ("resources/chicken.obj");
	chickenTexture_2.setupTexture("resources/texture/chickenTexture.bmp");

	chickenShader_3.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	chickenObj_3 = loadOBJ("resources/chicken.obj");
	chickenTexture_3.setupTexture("resources/texture/chickenTexture.bmp");

	//alienpeople
	alienpeopleShader_1.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	alienpeopleObj_1 = loadOBJ("resources/alienpeople.obj");
	alienpeopleTexture_1.setupTexture("resources/texture/alienTexture.bmp");

	alienpeopleShader_2.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	alienpeopleObj_2 = loadOBJ("resources/alienpeople.obj");
	alienpeopleTexture_2.setupTexture("resources/texture/alienTexture.bmp");

	alienpeopleShader_3.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	alienpeopleObj_3 = loadOBJ("resources/alienpeople.obj");
	alienpeopleTexture_3.setupTexture("resources/texture/alienTexture.bmp");

	//alienvehicle
	alienvehicleShader_1.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	alienvehicleObj_1 = loadOBJ("resources/alienvehicle.obj");
	alienvehicleTexture_1[0].setupTexture("resources/texture/alienTexture.bmp");
	alienvehicleTexture_1[1].setupTexture("resources/texture/colorful_alien_vehicleTexture.bmp");

	alienvehicleShader_2.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	alienvehicleObj_2 = loadOBJ("resources/alienvehicle.obj");
	alienvehicleTexture_2[0].setupTexture("resources/texture/alienTexture.bmp");
	alienvehicleTexture_2[1].setupTexture("resources/texture/colorful_alien_vehicleTexture.bmp");

	alienvehicleShader_3.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	alienvehicleObj_3 = loadOBJ("resources/alienvehicle.obj");
	alienvehicleTexture_3[0].setupTexture("resources/texture/alienTexture.bmp");
	alienvehicleTexture_3[1].setupTexture("resources/texture/colorful_alien_vehicleTexture.bmp");

	//icecream
	glGenVertexArrays(1, &icecreamVAO);
	glBindVertexArray(icecreamVAO);
	glGenBuffers(1, &icecreamVBO);
	glBindBuffer(GL_ARRAY_BUFFER, icecreamVBO);
	glBufferData(GL_ARRAY_BUFFER, icecreamObj.vertices.size() * sizeof(Vertex), &icecreamObj.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &icecreamEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icecreamEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, icecreamObj.indices.size() * sizeof(unsigned int), &icecreamObj.indices[0], GL_STATIC_DRAW);

	//spacecraft
	glGenVertexArrays(1, &spacecraftVAO);
	glBindVertexArray(spacecraftVAO);
	glGenBuffers(1, &spacecraftVBO);
	glBindBuffer(GL_ARRAY_BUFFER, spacecraftVBO);
	glBufferData(GL_ARRAY_BUFFER, spacecraftObj.vertices.size() * sizeof(Vertex), &spacecraftObj.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &spacecraftEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spacecraftEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, spacecraftObj.indices.size() * sizeof(unsigned int), &spacecraftObj.indices[0], GL_STATIC_DRAW);

	//planet
	glGenVertexArrays(1, &planetVAO);
	glBindVertexArray(planetVAO);
	glGenBuffers(1, &planetVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
	glBufferData(GL_ARRAY_BUFFER, planetObj.vertices.size() * sizeof(Vertex), &planetObj.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &planetEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planetEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planetObj.indices.size() * sizeof(unsigned int), &planetObj.indices[0], GL_STATIC_DRAW);

	//universe
	glGenVertexArrays(1, &universeVAO_1);
	glBindVertexArray(universeVAO_1);
	glGenBuffers(1, &universeVBO_1);
	glBindBuffer(GL_ARRAY_BUFFER, universeVBO_1);
	glBufferData(GL_ARRAY_BUFFER, universeObj_1.vertices.size() * sizeof(Vertex), &universeObj_1.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &universeEBO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, universeObj_1.indices.size() * sizeof(unsigned int), &universeObj_1.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &universeVAO_2);
	glBindVertexArray(universeVAO_2);
	glGenBuffers(1, &universeVBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, universeVBO_2);
	glBufferData(GL_ARRAY_BUFFER, universeObj_2.vertices.size() * sizeof(Vertex), &universeObj_2.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &universeEBO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, universeObj_2.indices.size() * sizeof(unsigned int), &universeObj_2.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &universeVAO_3);
	glBindVertexArray(universeVAO_3);
	glGenBuffers(1, &universeVBO_3);
	glBindBuffer(GL_ARRAY_BUFFER, universeVBO_3);
	glBufferData(GL_ARRAY_BUFFER, universeObj_3.vertices.size() * sizeof(Vertex), &universeObj_3.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &universeEBO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, universeObj_3.indices.size() * sizeof(unsigned int), &universeObj_3.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &universeVAO_4);
	glBindVertexArray(universeVAO_4);
	glGenBuffers(1, &universeVBO_4);
	glBindBuffer(GL_ARRAY_BUFFER, universeVBO_4);
	glBufferData(GL_ARRAY_BUFFER, universeObj_4.vertices.size() * sizeof(Vertex), &universeObj_4.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &universeEBO_4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, universeObj_4.indices.size() * sizeof(unsigned int), &universeObj_4.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &universeVAO_5);
	glBindVertexArray(universeVAO_5);
	glGenBuffers(1, &universeVBO_5);
	glBindBuffer(GL_ARRAY_BUFFER, universeVBO_5);
	glBufferData(GL_ARRAY_BUFFER, universeObj_5.vertices.size() * sizeof(Vertex), &universeObj_5.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &universeEBO_5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, universeObj_5.indices.size() * sizeof(unsigned int), &universeObj_5.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &universeVAO_6);
	glBindVertexArray(universeVAO_6);
	glGenBuffers(1, &universeVBO_6);
	glBindBuffer(GL_ARRAY_BUFFER, universeVBO_6);
	glBufferData(GL_ARRAY_BUFFER, universeObj_6.vertices.size() * sizeof(Vertex), &universeObj_6.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &universeEBO_6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, universeObj_6.indices.size() * sizeof(unsigned int), &universeObj_6.indices[0], GL_STATIC_DRAW);

	//rock
	for (int i = 0; i < rock_num; i++) {
		glGenVertexArrays(1, &rockVAO[i]);
		glBindVertexArray(rockVAO[i]);
		glGenBuffers(1, &rockVBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, rockVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, rockObj[i].vertices.size() * sizeof(Vertex), &rockObj[i].vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glGenBuffers(1, &rockEBO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rockEBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, rockObj[i].indices.size() * sizeof(unsigned int), &rockObj[i].indices[0], GL_STATIC_DRAW);
	}


	//chicken
	glGenVertexArrays(1, &chickenVAO_1);
	glBindVertexArray(chickenVAO_1);
	glGenBuffers(1, &chickenVBO_1);
	glBindBuffer(GL_ARRAY_BUFFER, chickenVBO_1);
	glBufferData(GL_ARRAY_BUFFER, chickenObj_1.vertices.size() * sizeof(Vertex), &chickenObj_1.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &chickenEBO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chickenEBO_1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chickenObj_1.indices.size() * sizeof(unsigned int), &chickenObj_1.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &chickenVAO_2);
	glBindVertexArray(chickenVAO_2);
	glGenBuffers(1, &chickenVBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, chickenVBO_2);
	glBufferData(GL_ARRAY_BUFFER, chickenObj_2.vertices.size() * sizeof(Vertex), &chickenObj_2.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &chickenEBO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chickenEBO_2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chickenObj_2.indices.size() * sizeof(unsigned int), &chickenObj_2.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &chickenVAO_3);
	glBindVertexArray(chickenVAO_3);
	glGenBuffers(1, &chickenVBO_3);
	glBindBuffer(GL_ARRAY_BUFFER, chickenVBO_3);
	glBufferData(GL_ARRAY_BUFFER, chickenObj_3.vertices.size() * sizeof(Vertex), &chickenObj_3.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &chickenEBO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chickenEBO_3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chickenObj_3.indices.size() * sizeof(unsigned int), &chickenObj_3.indices[0], GL_STATIC_DRAW);


	//alienpeople
	glGenVertexArrays(1, &alienpeopleVAO_1);
	glBindVertexArray(alienpeopleVAO_1);
	glGenBuffers(1, &alienpeopleVBO_1);
	glBindBuffer(GL_ARRAY_BUFFER, alienpeopleVBO_1);
	glBufferData(GL_ARRAY_BUFFER, alienpeopleObj_1.vertices.size() * sizeof(Vertex), &alienpeopleObj_1.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &alienpeopleEBO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienpeopleEBO_1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alienpeopleObj_1.indices.size() * sizeof(unsigned int), &alienpeopleObj_1.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &alienpeopleVAO_2);
	glBindVertexArray(alienpeopleVAO_2);
	glGenBuffers(1, &alienpeopleVBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, alienpeopleVBO_2);
	glBufferData(GL_ARRAY_BUFFER, alienpeopleObj_2.vertices.size() * sizeof(Vertex), &alienpeopleObj_2.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &alienpeopleEBO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienpeopleEBO_2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alienpeopleObj_2.indices.size() * sizeof(unsigned int), &alienpeopleObj_2.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &alienpeopleVAO_3);
	glBindVertexArray(alienpeopleVAO_3);
	glGenBuffers(1, &alienpeopleVBO_3);
	glBindBuffer(GL_ARRAY_BUFFER, alienpeopleVBO_3);
	glBufferData(GL_ARRAY_BUFFER, alienpeopleObj_3.vertices.size() * sizeof(Vertex), &alienpeopleObj_3.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &alienpeopleEBO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienpeopleEBO_3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alienpeopleObj_3.indices.size() * sizeof(unsigned int), &alienpeopleObj_3.indices[0], GL_STATIC_DRAW);

	//alienvehicle
	glGenVertexArrays(1, &alienvehicleVAO_1);
	glBindVertexArray(alienvehicleVAO_1);
	glGenBuffers(1, &alienvehicleVBO_1);
	glBindBuffer(GL_ARRAY_BUFFER, alienvehicleVBO_1);
	glBufferData(GL_ARRAY_BUFFER, alienvehicleObj_1.vertices.size() * sizeof(Vertex), &alienvehicleObj_1.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &alienvehicleEBO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienvehicleEBO_1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alienvehicleObj_1.indices.size() * sizeof(unsigned int), &alienvehicleObj_1.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &alienvehicleVAO_2);
	glBindVertexArray(alienvehicleVAO_2);
	glGenBuffers(1, &alienvehicleVBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, alienvehicleVBO_2);
	glBufferData(GL_ARRAY_BUFFER, alienvehicleObj_2.vertices.size() * sizeof(Vertex), &alienvehicleObj_2.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &alienvehicleEBO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienvehicleEBO_2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alienvehicleObj_2.indices.size() * sizeof(unsigned int), &alienvehicleObj_2.indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &alienvehicleVAO_3);
	glBindVertexArray(alienvehicleVAO_3);
	glGenBuffers(1, &alienvehicleVBO_3);
	glBindBuffer(GL_ARRAY_BUFFER, alienvehicleVBO_3);
	glBufferData(GL_ARRAY_BUFFER, alienvehicleObj_3.vertices.size() * sizeof(Vertex), &alienvehicleObj_3.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glGenBuffers(1, &alienvehicleEBO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienvehicleEBO_3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, alienvehicleObj_3.indices.size() * sizeof(unsigned int), &alienvehicleObj_3.indices[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void paintGL(void)  //always run
{
	if (up_pressed) key_up++;
	if (down_pressed) key_up--;
	if (left_pressed) key_left++;
	if (right_pressed) key_left--;


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model(1.0f);


	//spacecraft

	static std::pair<GLfloat, GLfloat> position{ 0.0, 0.0 };

	model = glm::rotate(model, glm::radians(mouse_left * 2.0f), glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(key_left * -0.01f, key_up * 0.01f, 0.0f));
	glm::vec4 transformed_origin = model * glm::vec4(glm::vec3(0), 1);
	position.first += transformed_origin[0];
	position.second += transformed_origin[1];


	auto distance = [](std::pair<double, double> x, std::pair<double, double> y) -> double {
		auto sqr = [](double x) -> double {
			return x * x;
		};

		return sqrt(sqr(x.first - y.first) + sqr(x.second - y.second));
	};

	static bool bye_chicken[3], bye_vehicles[3], bye_icecream;
	if (distance(position, { -1, 1 }) < 0.2) bye_chicken[0] = true;
	if (distance(position, { -1, 3 }) < 0.2) bye_chicken[1] = true;
	if (distance(position, { -1, 5 }) < 0.2) bye_chicken[2] = true;
	if (distance(position, { -3, 1 }) < 1.0) bye_vehicles[0] = true;
	if (distance(position, { -3, 3 }) < 1.0) bye_vehicles[1] = true;
	if (distance(position, { -3, 5 }) < 1.0) bye_vehicles[2] = true;
	if (distance(position, { -3, 0 }) < 0.2) bye_icecream = true;


	model = glm::translate(glm::mat4(1.0f), glm::vec3(position.first, position.second, 0.0f));
	model = glm::rotate(model, glm::radians(mouse_left * 2.0f), glm::vec3(0, 0, 1));
	key_left = key_up = 0;

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.01f, 100.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(-1.0, 0.0, zz2*0.2), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	glm::vec4 from(glm::vec3(0.0, -1.0, 0.7), 1.0f), to(glm::vec3(0, 0, 0.6), 1.0f);
	from = model * from;
	to = model * to;
	glm::mat4 view = glm::lookAt(glm::vec3(from), glm::vec3(to), glm::vec3(0, 0, 1));
	
	self_rotate++;

	model = glm::scale(model, glm::vec3(0.001, 0.001, 0.001));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	auto do_shading = [&projection, &view, &model](Shader& sh, int nm_flag = 0) -> void {
		sh.use();
		sh.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		sh.setVec3("lightPosition", -10.0f, -10.0f, 0.0f);

		sh.setVec3("lightColor1", key_w * 0.2f, 0.0f, key_w * 0.2f);
		sh.setVec3("lightPosition1", 10.0f, 10.0f, 0.0f);

		sh.setVec3("viewPosition", position.first, position.second, 0.0f);
		//sh.setVec3("viewPos", 0.0f, 0.0f, 0.0f);
		sh.setMat4("projection", projection);
		sh.setMat4("view", view);
		sh.setMat4("model", model);

		sh.setInt("textureDiffuse", 0);

		if (nm_flag) {
			sh.setInt("normalMappingFlag", key_x % 2);
			sh.setInt("textureSampler", 1);
		}
	};

	do_shading(spacecraftShader);

	int bye_all = (int)bye_chicken[0] + bye_chicken[1] + bye_chicken[2] + 
		bye_vehicles[0] + bye_vehicles[1] + bye_vehicles[2];
	if (bye_all < 6) spacecraftTexture[0].bind(0);
	else spacecraftTexture[1].bind(0);
	glBindVertexArray(spacecraftVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spacecraftEBO);
	glDrawElements(GL_TRIANGLES, spacecraftObj.indices.size(), GL_UNSIGNED_INT, 0); 

	
	//planet
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 5.0f, 0.0f));
	if (!key_y) model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 720), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

	do_shading(planetShader);
	do_shading(planetShader, 1);

	planetTexture[0].bind(0);
	planetTexture[1].bind(1);
	glBindVertexArray(planetVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planetEBO);
	glDrawElements(GL_TRIANGLES, planetObj.indices.size(), GL_UNSIGNED_INT, 0);
	
	//rock
	for (int i = 0; i < rock_num; i++) {
		if (!set_t[i]) {
			set_t[i] = true;

			auto randf = [](GLfloat const MAX) -> GLfloat {
				return (rand() % 100) / 100.0f * MAX;
			};

			radius[i] = 3.0f + randf(1.0f);
			theta[i] = randf(6.28318530718f);
			z_val[i] = randf(0.5f) - 0.5f / 2.0f;
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 5.0f, 0.0f));
		model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 2 / 720), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(radius[i] * cos(theta[i]), radius[i] * sin(theta[i]), z_val[i]));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

		do_shading(rockShader[i]);

		rockTexture[i].bind(0);
		glBindVertexArray(rockVAO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rockEBO[i]);
		glDrawElements(GL_TRIANGLES, rockObj[i].indices.size(), GL_UNSIGNED_INT, 0);
	}

	const glm::vec3 universe_scale{ 2.3f, 2.3f, 2.8f };

	if (!bye_icecream) {
		//icecream
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3, 0, 0.5));
		model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		do_shading(icecreamShader);
		icecreamTexture.bind(0);
		glBindVertexArray(icecreamVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icecreamEBO);
		glDrawElements(GL_TRIANGLES, icecreamObj.indices.size(), GL_UNSIGNED_INT, 0);
	}

	//universe_1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, -20, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(universe_scale));
	do_shading(universeShader_1);
	universeTexture_1.bind(0);
	glBindVertexArray(universeVAO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_1);
	glDrawElements(GL_TRIANGLES, universeObj_1.indices.size(), GL_UNSIGNED_INT, 0); 

	//universe_2
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, -20, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(universe_scale));
	do_shading(universeShader_2);
	universeTexture_2.bind(0);
	glBindVertexArray(universeVAO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_2);
	glDrawElements(GL_TRIANGLES, universeObj_2.indices.size(), GL_UNSIGNED_INT, 0);

	//universe_3
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(0, -20, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(universe_scale));
	do_shading(universeShader_3);
	universeTexture_3.bind(0);
	glBindVertexArray(universeVAO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_3);
	glDrawElements(GL_TRIANGLES, universeObj_3.indices.size(), GL_UNSIGNED_INT, 0);

	//universe_4
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(0, -20, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(universe_scale));
	do_shading(universeShader_4);
	universeTexture_4.bind(0);
	glBindVertexArray(universeVAO_4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_4);
	glDrawElements(GL_TRIANGLES, universeObj_4.indices.size(), GL_UNSIGNED_INT, 0);

	//universe_5
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(0, -20, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(universe_scale));
	do_shading(universeShader_5);
	universeTexture_5.bind(0);
	glBindVertexArray(universeVAO_5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_5);
	glDrawElements(GL_TRIANGLES, universeObj_5.indices.size(), GL_UNSIGNED_INT, 0);

	//universe_6
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, -20, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(universe_scale));
	do_shading(universeShader_6);
	universeTexture_6.bind(0);
	glBindVertexArray(universeVAO_6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, universeEBO_6);
	glDrawElements(GL_TRIANGLES, universeObj_6.indices.size(), GL_UNSIGNED_INT, 0);
	





	if (!bye_chicken[0]) {
		//chick1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.5f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));

		do_shading(chickenShader_1);

		chickenTexture_1.bind(0);
		glBindVertexArray(chickenVAO_1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chickenEBO_1);
		glDrawElements(GL_TRIANGLES, chickenObj_1.indices.size(), GL_UNSIGNED_INT, 0);
	}

	if (!bye_chicken[1]) {
		//chick2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 3.0f, 0.5f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));

		do_shading(chickenShader_2);

		chickenTexture_2.bind(0);
		glBindVertexArray(chickenVAO_2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chickenEBO_2);
		glDrawElements(GL_TRIANGLES, chickenObj_2.indices.size(), GL_UNSIGNED_INT, 0);
	}

	if (!bye_chicken[2]) {
		//chick3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 5.0f, 0.5f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));

		do_shading(chickenShader_3);

		chickenTexture_3.bind(0);
		glBindVertexArray(chickenVAO_3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chickenEBO_3);
		glDrawElements(GL_TRIANGLES, chickenObj_3.indices.size(), GL_UNSIGNED_INT, 0);
	}






	//alienpeople1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(45 * 360 / 36.0f), glm::vec3(1, 0, 0));

	do_shading(alienpeopleShader_1);

	alienpeopleTexture_1.bind(0);
	glBindVertexArray(alienpeopleVAO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienpeopleEBO_1);
	glDrawElements(GL_TRIANGLES, alienpeopleObj_1.indices.size(), GL_UNSIGNED_INT, 0);

	//alienpeople2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 3.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(45 * 360 / 36.0f), glm::vec3(1, 0, 0));

	do_shading(alienpeopleShader_2);

	alienpeopleTexture_2.bind(0);
	glBindVertexArray(alienpeopleVAO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienpeopleEBO_2);
	glDrawElements(GL_TRIANGLES, alienpeopleObj_2.indices.size(), GL_UNSIGNED_INT, 0);

	//alienpeople3
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 5.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(45 * 360 / 36.0f), glm::vec3(1, 0, 0));

	do_shading(alienpeopleShader_3);

	alienpeopleTexture_3.bind(0);
	glBindVertexArray(alienpeopleVAO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienpeopleEBO_3);
	glDrawElements(GL_TRIANGLES, alienpeopleObj_3.indices.size(), GL_UNSIGNED_INT, 0);





	//alienvehicle1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(45 * 360 / 36.0f), glm::vec3(1, 0, 0));

	do_shading(alienvehicleShader_1);

	if (!bye_vehicles[0]) alienvehicleTexture_1[0].bind(0);
	else alienvehicleTexture_1[1].bind(0);
	glBindVertexArray(alienvehicleVAO_1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienvehicleEBO_1);
	glDrawElements(GL_TRIANGLES, alienvehicleObj_1.indices.size(), GL_UNSIGNED_INT, 0);

	//alienvehicle2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 3.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(45 * 360 / 36.0f), glm::vec3(1, 0, 0));

	do_shading(alienvehicleShader_2);

	if (!bye_vehicles[1]) alienvehicleTexture_2[0].bind(0);
	else alienvehicleTexture_2[1].bind(0);
	glBindVertexArray(alienvehicleVAO_2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienvehicleEBO_2);
	glDrawElements(GL_TRIANGLES, alienvehicleObj_2.indices.size(), GL_UNSIGNED_INT, 0);

	//alienpeople3
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 5.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(self_rotate * 360.0f / 180), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(45 * 360 / 36.0f), glm::vec3(1, 0, 0));

	do_shading(alienvehicleShader_3);

	if (!bye_vehicles[2]) alienvehicleTexture_3[0].bind(0);
	else alienvehicleTexture_3[1].bind(0);
	glBindVertexArray(alienvehicleVAO_3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alienvehicleEBO_3);
	glDrawElements(GL_TRIANGLES, alienvehicleObj_3.indices.size(), GL_UNSIGNED_INT, 0);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Sets the mouse-button callback for the current window.	
	is_clicked = action;
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	// Sets the cursor position callback for the current window
	if (is_clicked) {
		if (x < old_x) {
			mouse_left++;
		} else if (x > old_x) {
			mouse_left--;
		}
	}
	old_x = x;
	old_y = y;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Sets the scoll callback for the current window.
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.
	if (key == GLFW_KEY_X && action == GLFW_PRESS) { key_x++; key_x %= 2; }
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) { key_y++; key_y %= 2; }

	if (key == GLFW_KEY_W && action == GLFW_PRESS) key_w++;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) key_w--;
	
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) up_pressed = true;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) down_pressed = true;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) left_pressed = true;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) right_pressed = true;
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE) up_pressed = false;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) down_pressed = false;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) left_pressed = false;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) right_pressed = false;


}


int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final Project :)", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);                                                                  //    
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
