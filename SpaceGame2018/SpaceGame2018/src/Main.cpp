#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/MiscUtils.hpp"
#include "Camera.h"
#include "utils/Ray.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"void main()\n"
"{\n"
"   	gl_Position = projection *view* vec4(aPos, 1.0f);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"uniform vec4 ourColor;\n" // we set this variable in the OpenGL code.

"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = ourColor;\n"
//"   FragColor = vec4(0.0f, 0.0f, 0.7f, 1.0f);\n"
"}\n\0";

float locStartX = -0.5f;
float locStartY = -0.5f;
float locEndX = 0.0f;
float locEndY = 0.0f;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
    GLFWwindow* window;
	bool firstMouse = true;
	int width = 640;
	int height = 480;
	float lastX = width / 2.0f;
	float lastY = height / 2.0f;
	bool displayCursor = false;


	float vertices[] = {
	-1.5f, -0.5f, -5.0f, // left  
	 0.5f, -0.5f, -5.0f, // right 
	 0.0f,  0.5f, -5.0f  // top   
	};
	void Line(glm::vec3 start, glm::vec3 end);

	float mousePosX, mousePosY;


	glm::vec3 CreateRay() {
		// these positions must be in range [-1, 1] (!!!), not [0, width] and [0, height]
		float mouseX = mousePosX / (width  * 0.5f) - 1.0f;
		float mouseY = mousePosY / (height * 0.5f) - 1.0f;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 invVP = glm::inverse(projection *view);
		glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
		glm::vec4 worldPos = invVP * screenPos;

		glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

		return dir;
	}
	glm::vec3 mouseRay;

int main(void)
{
	glm::vec3 rayOrigin(-5, -2, 0);
	glm::vec3 rayDir(2, 2, 0);
	glm::vec3 sphereOrigin(0, 0, 0);
	float sphereRad = 3;

	Ray ray;

	glm::vec2 intersec = ray.raySphereIntersect(rayOrigin, rayDir, sphereOrigin, sphereRad);

	std::cout << intersec.x << std::endl;

		if (intersec.x == -1)
	{
		std::cout << intersec.y << "ray doesn't hit sphere!! " << std::endl;

	}
	else
	{
		glm::vec3 insectpointNear = rayOrigin + rayDir * intersec.x;
		std::cout << intersec.x << " " << insectpointNear.x << " " << insectpointNear.y << " " << insectpointNear.z << " " << std::endl;
		glm::vec3 insectpointFar = rayOrigin + rayDir * intersec.y;
		std::cout << intersec.y << " " << insectpointFar.x << " " << insectpointFar.y << " " << insectpointFar.z << " " << std::endl;

	}


	float rayCloses = intersec.x - intersec.y;

	std::cout << " rayclose " << rayCloses;


	MiscUtils misc;


    /* Initialize the library */
    if (!glfwInit())
        return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const char* name = "Space Game";

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew didnt init!!" << std::endl;
	}


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------





	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);


	//camera.Position.x = 40;
	//camera.Position.z = 40;





    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

		glm::vec3 start(-0.5f, -0.5f, -5.0f);
		glm::vec3 end(0.5f, 0.5f, -5.0f);

		


		Line(camera.Position,camera.Position*mouseRay);
	


		/*unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);*/
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);


		if (!displayCursor)
		{
			glfwSetCursorPosCallback(window, mouse_callback);
			// tell GLFW to capture our mouse
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstMouse = true;
		}
		// per-frame time logic
// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// draw our first triangle
		glUseProgram(shaderProgram);

		// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
// -----------------------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

		glm::mat4 view = camera.GetViewMatrix();
		float col = .5f;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "myColor"), 1, GL_FALSE, col);



		float greenValue = 0.9f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

		misc.frameCount();
    }

    glfwTerminate();
    return 0;
}

void Line(glm::vec3 start, glm::vec3 end)
{


	vertices[0] = { start.x };
	vertices[1] = { start.y };
	vertices[2] = { start.z };

	vertices[3] = { start.x };
	vertices[4] = { start.y };
	vertices[5] = { start.z };

	vertices[6] = { end.x };
	vertices[7] = { end.y };
	vertices[8] = { end.z };
}
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
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		displayCursor = true;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		displayCursor = false;	

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		mouseRay = CreateRay();
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mousePosX = xpos;
	mousePosY=ypos;
	if (displayCursor == false)
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
