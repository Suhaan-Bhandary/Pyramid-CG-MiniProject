#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"


// Vertices coordinates
GLfloat pyramidVertices[] =
{	//     COORDINATES     /        COLORS      /   TexCoord  //
	-1.5f, 0.0f,  1.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-1.5f, 0.0f, -1.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	1.5f, 0.0f, -1.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	1.5f, 0.0f,  1.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	0.0f, 1.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for pyramidVertices order
GLuint pyramidIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

// Vertices coordinates
GLfloat planeVertex[] =
{
	100.0f, 0.0f, 100.0f,  		0.83f, 0.70f, 0.44f,		0.0f, 0.0f,
	100.0f, 0.0f, -100.0f, 		0.83f, 0.70f, 0.44f,		15.0f, 0.0f,
	-100.0f, 0.0f, -100.0f,		0.83f, 0.70f, 0.44f,		15.0f, 15.0f,
	-100.0f, 0.0f, 100.0f,		0.83f, 0.70f, 0.44f,		0.0f, 15.0f,
};

// Indices for pyramidVertices order
GLuint planeIndices[] =
{
	0, 1, 2,
	0, 2, 3,
};

const unsigned int windowWidth = 1366;
const unsigned int windowHeight = 768;

int main() {
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Going 3D", NULL, NULL);

	if (window == NULL) {
		std::cout << "Window creation failed with glfw!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Making the window as current
	glfwMakeContextCurrent(window);

	// Loading the glad file
	gladLoadGL();

	// Defining the view port inside the window the window
	glViewport(0, 0, windowWidth, windowHeight);

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader pyramidShaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO pyramidVAO;
	pyramidVAO.Bind();

	// Generates Vertex Buffer Object and links it to pyramidVertices
	VBO pyramidVBO(pyramidVertices, sizeof(pyramidVertices));

	// Generates Element Buffer Object and links it to pyramidIndices
	// When we make ebo, it is stored VAO so make sure to unbind it after VAO
	EBO pyramidEBO(pyramidIndices, sizeof(pyramidIndices));

	// Links VBO to VAO
	pyramidVAO.LinkAttrib(pyramidVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	pyramidVAO.LinkAttrib(pyramidVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	pyramidVAO.LinkAttrib(pyramidVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	pyramidVAO.Unbind();
	pyramidVBO.Unbind();
	pyramidEBO.Unbind();

	Texture pyramidTexture("pyramid.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pyramidTexture.texUnit(pyramidShaderProgram, "tex0", 0);




//	------------------------------------------------------------------------------------------------------------------
	// Generates Shader object using shaders defualt.vert and default.frag
	Shader planeShaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO planeVAO;
	planeVAO.Bind();

	// Generates Vertex Buffer Object and links it to pyramidVertices
	VBO planeVBO(planeVertex, sizeof(planeVertex));

	// Generates Element Buffer Object and links it to pyramidIndices
	// When we make ebo, it is stored VAO so make sure to unbind it after VAO
	EBO planeEBO(planeIndices, sizeof(planeIndices));

	// Links VBO to VAO
	planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	planeVAO.LinkAttrib(planeVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	planeVAO.Unbind();
	planeVBO.Unbind();
	planeEBO.Unbind();

	Texture grassTecture("grass.bmp", GL_TEXTURE_2D, GL_TEXTURE0, GL_BGR, GL_UNSIGNED_BYTE);
	grassTecture.texUnit(planeShaderProgram, "tex0", 0);
//	------------------------------------------------------------------------------------------------------------------
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Used to sync rendering loop to the refresh rate of the monitor
	glfwSwapInterval(1);

	Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.2f, 5.0f));


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background

		glClearColor(135.0f / 256.0f, 206.0f / 256.0f, 235.0f / 256.0f, 1.0f);

		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Tell OpenGL which Shader Program we want to use
		pyramidShaderProgram.Activate();

		camera.Inputs(window);

		camera.gravityForce(window);

		camera.Matrix(45.0f, 0.1f, 50.0f, pyramidShaderProgram, "camMatrix");

		pyramidTexture.Bind();

		// Bind the VAO so OpenGL knows to use it
		pyramidVAO.Bind();

		// Draw primitives, number of pyramidIndices, datatype of pyramidIndices, index of pyramidIndices
		glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(int), GL_UNSIGNED_INT, 0);






		//	--------------------------------------------------------------------------------------------------
		// Tell OpenGL which Shader Program we want to use
		planeShaderProgram.Activate();


		camera.Matrix(45.0f, 0.1f, 100.0f, planeShaderProgram, "camMatrix");
		grassTecture.Bind();

		// Bind the VAO so OpenGL knows to use it
		planeVAO.Bind();

		// Draw primitives, number of pyramidIndices, datatype of pyramidIndices, index of pyramidIndices
		glDrawElements(GL_TRIANGLES, sizeof(planeIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
//		--------------------------------------------------------------------------------------------------

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	pyramidVAO.Delete();
	pyramidVBO.Delete();
	pyramidEBO.Delete();

	planeVAO.Delete();
	planeVBO.Delete();
	planeEBO.Delete();

	// Deleting the texture
	pyramidTexture.Delete();
	pyramidTexture.Delete();

	pyramidShaderProgram.Delete();
	planeShaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}
