#define GLEW_STATIC
#include <cstdio>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <SOIL.h>
#include <vector>
#include "camera.h"
#include "cylinder.h"
#include "cuboid.h"
#include "mesh.h"
#include "animation.h"
#include "constant.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const GLuint WIDTH = 800, HEIGHT = 600;

// camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraVector = glm::vec3(0.0f, 1.0f, 0.0f);	/*Kamera bedzie zwrocona na przeciwko tego wektora */
Camera camera(cameraPosition);
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting 
glm::vec3 lightPos(8.0f, 2.0f, 4.0f);

/*Starting point*/
glm::vec3 board_start_point = glm::vec3(-BOARD_WIDTH / 2, -(CYLINDER_HEIGHT / 2 + BOARD_HEIGHT), -BOARD_LENGHT + CYLINDER_RADIUS);
glm::vec3 plane_start_point = glm::vec3(-PLANE_WIDTH / 2, -(CYLINDER_HEIGHT / 2 + BOARD_HEIGHT + PLANE_HEIGHT), -PLANE_LENGTH / 2);
glm::vec3 column_left_start_point = glm::vec3(-BOARD_WIDTH * 2, -(CYLINDER_HEIGHT / 2 + BOARD_HEIGHT + PLANE_HEIGHT), -CYLINDER_RADIUS);
glm::vec3 column_right_start_point = glm::vec3(BOARD_WIDTH * 2 - COLUMN_WIDTH, -(CYLINDER_HEIGHT / 2 + BOARD_HEIGHT + PLANE_HEIGHT), -CYLINDER_RADIUS);
glm::vec3 top_start_point = glm::vec3(-TOP_WIDTH / 2, -(CYLINDER_HEIGHT / 2 + BOARD_HEIGHT) + COLUMN_HEIGHT - PLANE_HEIGHT, -CYLINDER_RADIUS);


int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 01", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, WIDTH, HEIGHT);
		// Build, compile and link shader program
		Shader theProgram("gl_03.vert", "gl_03.frag");
		Shader lampShader("gl_03_lamp.vert", "gl_03_lamp.frag");


		// prepare textures
		Texture metal_texture(GL_TEXTURE0, "metal.png");
		Texture wood_texture(GL_TEXTURE0, "wood.png");
		Texture brick_texture(GL_TEXTURE0, "brick.png");
		Texture concrete_texture(GL_TEXTURE0, "concrete.png");
		Texture brick2_texture(GL_TEXTURE0, "brick2.png");


		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Cylinder cylinder(CYLINDER_RADIUS, CYLINDER_HEIGHT, 100);
		Mesh cylinder_mesh = cylinder.getCylinderMesh(&metal_texture);

		Cuboid board(board_start_point, glm::vec3(BOARD_WIDTH, BOARD_HEIGHT, BOARD_LENGHT), 10);
		Mesh board_mesh = board.getMesh(&wood_texture);
		
		Cuboid plane(plane_start_point, glm::vec3(PLANE_WIDTH, PLANE_HEIGHT, PLANE_LENGTH), 10);
		Mesh plane_mesh = plane.getMesh(&brick2_texture);

		Cuboid column_left(column_left_start_point, glm::vec3(COLUMN_WIDTH, COLUMN_HEIGHT, COLUMN_LENGTH), 10);
		Mesh column_left_mesh = column_left.getMesh(&brick_texture);

		Cuboid column_right(column_right_start_point, glm::vec3(COLUMN_WIDTH, COLUMN_HEIGHT, COLUMN_LENGTH), 10);
		Mesh column_right_mesh = column_right.getMesh(&brick_texture);

		Cuboid top(top_start_point, glm::vec3(TOP_WIDTH, TOP_HEIGHT, TOP_LENGTH), 10);
		Mesh top_mesh = top.getMesh(&concrete_texture);

		Cuboid cuboid_light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f), 10);
		Mesh cuboid_light_mesh = cuboid_light.getMesh(&metal_texture);

		theProgram.use();
		theProgram.setInt("material.diffuse", 0);
		theProgram.setInt("material.specular", 1);

		Animation animation;
		
		// main loop
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			processInput(window);

			//run animation
			animation.update_translations_models(deltaTime);
			

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind Textures using texture units
			// input
			// -----
			processInput(window);

			//Seting shaders
			theProgram.use();
			theProgram.setVec3("light.position", lightPos);
			theProgram.setVec3("viewPos", camera.Position);

			// light properties
			theProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
			theProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			theProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

			// material properties
			theProgram.setFloat("material.shininess", 10.0f);

			// pass projection matrix to shader (note that in this case it could change every frame)
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			theProgram.setMat4("projection", projection);

			// camera/view transformation
			glm::mat4 view = camera.GetViewMatrix();
			theProgram.setMat4("view", view);

			glm::mat4 model;
			float angle = 20.0f;
			model = glm::rotate(model, glm::radians(angle), cameraVector);
			model = glm::translate(model, animation.cylinder_model_translation);
			theProgram.setMat4("model", model);
			cylinder_mesh.Draw(theProgram);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(angle), cameraVector);
			model = glm::translate(model, animation.board_model_translation);
			theProgram.setMat4("model", model);
			board_mesh.Draw(theProgram);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(angle), cameraVector);
			theProgram.setMat4("model", model);
			plane_mesh.Draw(theProgram);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(angle), cameraVector);
			theProgram.setMat4("model", model);
			column_left_mesh.Draw(theProgram);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(angle), cameraVector);
			theProgram.setMat4("model", model);
			column_right_mesh.Draw(theProgram);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(angle), cameraVector);
			theProgram.setMat4("model", model);
			top_mesh.Draw(theProgram);

			//Uzycie lamp shadera
			lampShader.use();
			lampShader.setMat4("projection", projection);
			lampShader.setMat4("view", view);
			model = glm::mat4();
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lampShader.setMat4("model", model);

			cuboid_light_mesh.Draw(theProgram);
			// Swap the screen buffers
			glfwSwapBuffers(window);
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
