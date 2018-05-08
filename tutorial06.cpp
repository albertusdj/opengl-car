#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Test", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	
	

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	GLuint texture = loadBMP_custom("woodtexture.bmp");
	GLuint TextureID = glGetUniformLocation(programID, "carTexture");

	
	GLuint frontwheelprogramID = LoadShaders("FrontWheelVertexShader.vertexshader", "FragmentShader.fragmentshader");
	GLuint FrontWheelMatrixID = glGetUniformLocation(frontwheelprogramID, "MVPFrontWheel");

	GLuint backwheelprogramID = LoadShaders("BackWheelVertexShader.vertexshader", "FragmentShader.fragmentshader");
	GLuint BackWheelMatrixID = glGetUniformLocation(backwheelprogramID, "MVPBackWheel");

	static const GLfloat g_frontwheel_buffer_data[] = {
		-0.5f, 0.1f, 0.3f,
		-0.3f, 0.1f, 0.3f,
		-0.3f, -0.1f, 0.3f,
		-0.3f, -0.1f, 0.3f,
		-0.5f, -0.1f, 0.3f,
		-0.5f, 0.1f, 0.3f,

		-0.5f, 0.1f, 0.2f,
		-0.3f, 0.1f, 0.2f,
		-0.3f, -0.1f, 0.2f,
		-0.3f, -0.1f, 0.2f,
		-0.5f, -0.1f, 0.2f,
		-0.5f, 0.1f, 0.2f,

		-0.5f, 0.1f, -0.3f,
		-0.3f, 0.1f, -0.3f,
		-0.3f, -0.1f, -0.3f,
		-0.3f, -0.1f, -0.3f,
		-0.5f, -0.1f, -0.3f,
		-0.5f, 0.1f, -0.3f,

		-0.5f, 0.1f, -0.2f,
		-0.3f, 0.1f, -0.2f,
		-0.3f, -0.1f, -0.2f,
		-0.3f, -0.1f, -0.2f,
		-0.5f, -0.1f, -0.2f,
		-0.5f, 0.1f, -0.2f,
		//penghubung
		-0.3f, 0.1f, 0.3f,
		-0.3f, 0.1f, 0.2f,
		-0.3f, -0.1f, 0.2f,
		-0.3f, -0.1f, 0.2f,
		-0.3f, -0.1f, 0.3f,
		-0.3f, 0.1f, 0.3f,

		-0.5f, 0.1f, 0.3f,
		-0.5f, 0.1f, 0.2f,
		-0.5f, -0.1f, 0.2f,
		-0.5f, -0.1f, 0.2f,
		-0.5f, -0.1f, 0.3f,
		-0.5f, 0.1f, 0.3f,

		-0.5f, 0.1f, 0.3f,
		-0.5f, 0.1f, 0.2f,
		-0.3f, 0.1f, 0.2f,
		-0.3f, 0.1f, 0.2f,
		-0.3f, 0.1f, 0.3f,
		-0.5f, 0.1f, 0.3f,

		-0.5f, -0.1f, 0.3f,
		-0.5f, -0.1f, 0.2f,
		-0.3f, -0.1f, 0.2f,
		-0.3f, -0.1f, 0.2f,
		-0.3f, -0.1f, 0.3f,
		-0.5f, -0.1f, 0.3f,

		-0.3f, 0.1f, -0.3f,
		-0.3f, 0.1f, -0.2f,
		-0.3f, -0.1f, -0.2f,
		-0.3f, -0.1f, -0.2f,
		-0.3f, -0.1f, -0.3f,
		-0.3f, 0.1f, -0.3f,

		-0.5f, 0.1f, -0.3f,
		-0.5f, 0.1f, -0.2f,
		-0.5f, -0.1f, -0.2f,
		-0.5f, -0.1f, -0.2f,
		-0.5f, -0.1f, -0.3f,
		-0.5f, 0.1f, -0.3f,

		-0.5f, 0.1f, -0.3f,
		-0.5f, 0.1f, -0.2f,
		-0.3f, 0.1f, -0.2f,
		-0.3f, 0.1f, -0.2f,
		-0.3f, 0.1f, -0.3f,
		-0.5f, 0.1f, -0.3f,

		-0.5f, -0.1f, -0.3f,
		-0.5f, -0.1f, -0.2f,
		-0.3f, -0.1f, -0.2f,
		-0.3f, -0.1f, -0.2f,
		-0.3f, -0.1f, -0.3f,
		-0.5f, -0.1f, -0.3f,
	};

	static const GLfloat g_backwheel_buffer_data[] = {
		0.3f, 0.1f, 0.3f,
		0.5f, 0.1f, 0.3f,
		0.5f, -0.1f, 0.3f,
		0.5f, -0.1f, 0.3f,
		0.3f, -0.1f, 0.3f,
		0.3f, 0.1f, 0.3f,

		0.3f, 0.1f, 0.2f,
		0.5f, 0.1f, 0.2f,
		0.5f, -0.1f, 0.2f,
		0.5f, -0.1f, 0.2f,
		0.3f, -0.1f, 0.2f,
		0.3f, 0.1f, 0.2f,

		0.3f, 0.1f, -0.3f,
		0.5f, 0.1f, -0.3f,
		0.5f, -0.1f, -0.3f,
		0.5f, -0.1f, -0.3f,
		0.3f, -0.1f, -0.3f,
		0.3f, 0.1f, -0.3f,

		0.3f, 0.1f, -0.2f,
		0.5f, 0.1f, -0.2f,
		0.5f, -0.1f, -0.2f,
		0.5f, -0.1f, -0.2f,
		0.3f, -0.1f, -0.2f,
		0.3f, 0.1f, -0.2f,
		//penghubung
		0.5f, 0.1f, 0.3f,
		0.5f, 0.1f, 0.2f,
		0.5f, -0.1f, 0.2f,
		0.5f, -0.1f, 0.2f,
		0.5f, -0.1f, 0.3f,
		0.5f, 0.1f, 0.3f,

		0.3f, 0.1f, 0.3f,
		0.3f, 0.1f, 0.2f,
		0.3f, -0.1f, 0.2f,
		0.3f, -0.1f, 0.2f,
		0.3f, -0.1f, 0.3f,
		0.3f, 0.1f, 0.3f,

		0.3f, 0.1f, 0.3f,
		0.3f, 0.1f, 0.2f,
		0.5f, 0.1f, 0.2f,
		0.5f, 0.1f, 0.2f,
		0.5f, 0.1f, 0.3f,
		0.3f, 0.1f, 0.3f,

		0.3f, -0.1f, 0.3f,
		0.3f, -0.1f, 0.2f,
		0.5f, -0.1f, 0.2f,
		0.5f, -0.1f, 0.2f,
		0.5f, -0.1f, 0.3f,
		0.3f, -0.1f, 0.3f,

		0.5f, 0.1f, -0.3f,
		0.5f, 0.1f, -0.2f,
		0.5f, -0.1f, -0.2f,
		0.5f, -0.1f, -0.2f,
		0.5f, -0.1f, -0.3f,
		0.5f, 0.1f, -0.3f,

		0.3f, 0.1f, -0.3f,
		0.3f, 0.1f, -0.2f,
		0.3f, -0.1f, -0.2f,
		0.3f, -0.1f, -0.2f,
		0.3f, -0.1f, -0.3f,
		0.3f, 0.1f, -0.3f,

		0.3f, 0.1f, -0.3f,
		0.3f, 0.1f, -0.2f,
		0.5f, 0.1f, -0.2f,
		0.5f, 0.1f, -0.2f,
		0.5f, 0.1f, -0.3f,
		0.3f, 0.1f, -0.3f,

		0.3f, -0.1f, -0.3f,
		0.3f, -0.1f, -0.2f,
		0.5f, -0.1f, -0.2f,
		0.5f, -0.1f, -0.2f,
		0.5f, -0.1f, -0.3f,
		0.3f, -0.1f, -0.3f,
	};

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-0.9f, 0.0f, 0.2f, //Sisi kiri
		0.9f, 0.0f, 0.2f,
		-0.9f, 0.2f, 0.2f,
		0.9f, 0.2f, 0.2f,
		0.9f, 0.0f, 0.2f,
		-0.9f, 0.2f, 0.2f,
		-0.4f, 0.2f, 0.2f,
		0.9f, 0.2f, 0.2f,
		0.4f, 0.4f, 0.2f,
		0.4f, 0.4f, 0.2f,
		-0.2f, 0.4f, 0.2f,
		-0.4f, 0.2f, 0.2f,

		-0.9f, 0.0f, -0.2f, //Sisi kanan
		0.9f, 0.0f, -0.2f,
		-0.9f, 0.2f, -0.2f,
		0.9f, 0.2f, -0.2f,
		0.9f, 0.0f, -0.2f,
		-0.9f, 0.2f, -0.2f,
		-0.4f, 0.2f, -0.2f,
		0.9f, 0.2f, -0.2f,
		0.4f, 0.4f, -0.2f,
		0.4f, 0.4f, -0.2f,
		-0.2f, 0.4f, -0.2f,
		-0.4f, 0.2f, -0.2f,

		-0.9f, 0.2f, 0.2f,  //bumper
		-0.9f, 0.0f, 0.2f,
		-0.9f, 0.0f, -0.2f,
		-0.9f, 0.0f, -0.2f,
		-0.9f, 0.2f, -0.2f,
		-0.9f, 0.2f, 0.2f,

		-0.9f, 0.2f, 0.2f,  //kap mobil
		-0.4f, 0.2f, 0.2f,
		-0.4f, 0.2f, -0.2f,
		-0.4f, 0.2f, -0.2f,
		-0.9f, 0.2f, -0.2f,
		-0.9f, 0.2f, 0.2f,

		-0.4f, 0.2f, 0.2f,  //kaca depan
		-0.2f, 0.4f, 0.2f,
		-0.2f, 0.4f, -0.2f,
		-0.2f, 0.4f, -0.2f,
		-0.4f, 0.2f, -0.2f,
		-0.4f, 0.2f, 0.2f,

		-0.2f, 0.4f, 0.2f,  //atap
		0.4f, 0.4f, 0.2f,
		0.4f, 0.4f, -0.2f,
		0.4f, 0.4f, -0.2f,
		-0.2f, 0.4f, -0.2f,
		-0.2f, 0.4f, 0.2f,

		0.4f, 0.4f, 0.2f, //kaca belakang
		0.9f, 0.2f, 0.2f,
		0.9f, 0.2f, -0.2f,
		0.9f, 0.2f, -0.2f,
		0.4f, 0.4f, -0.2f,
		0.4f, 0.4f, 0.2f, 

		0.9f, 0.2f, 0.2f, //belakang
		0.9f, 0.0f, 0.2f,
		0.9f, 0.0f, -0.2f,
		0.9f, 0.0f, -0.2f,
		0.9f, 0.2f, -0.2f,
		0.9f, 0.2f, 0.2f,

		-0.9f, 0.0f, 0.2f, //alas
		0.9f, 0.0f, 0.2f,
		0.9f, 0.0f, -0.2f,
		0.9f, 0.0f, -0.2f,
		-0.9f, 0.0f, -0.2f,
		-0.9f, 0.0f, 0.2f
	};

	//One color for each vertex. They were generated randomly.
	// static const GLfloat g_color_buffer_data[] = {
	// 	0.583f,  0.771f,  0.014f,
	// 	0.609f,  0.115f,  0.436f,
	// 	0.327f,  0.483f,  0.844f,
	// 	0.822f,  0.569f,  0.201f,
	// 	0.435f,  0.602f,  0.223f,
	// 	0.310f,  0.747f,  0.185f,
	// 	0.597f,  0.770f,  0.761f,
	// 	0.559f,  0.436f,  0.730f,
	// 	0.359f,  0.583f,  0.152f,
	// 	0.483f,  0.596f,  0.789f,
	// 	0.559f,  0.861f,  0.639f,
	// 	0.583f,  0.771f,  0.014f,
	// 	0.609f,  0.115f,  0.436f,
	// 	0.327f,  0.483f,  0.844f,
	// 	0.822f,  0.569f,  0.201f,
	// 	0.435f,  0.602f,  0.223f,
	// 	0.310f,  0.747f,  0.185f,
	// 	0.597f,  0.770f,  0.761f,
	// 	0.559f,  0.436f,  0.730f,
	// 	0.359f,  0.583f,  0.152f,
	// 	0.483f,  0.596f,  0.789f,
	// 	0.559f,  0.861f,  0.639f,
	// 	0.583f,  0.771f,  0.014f,
	// 	0.609f,  0.115f,  0.436f,
	// 	0.327f,  0.483f,  0.844f,
	// 	0.822f,  0.569f,  0.201f,
	// 	0.435f,  0.602f,  0.223f,
	// 	0.310f,  0.747f,  0.185f,
	// 	0.597f,  0.770f,  0.761f,
	// 	0.559f,  0.436f,  0.730f,
	// 	0.359f,  0.583f,  0.152f,
	// 	0.483f,  0.596f,  0.789f,
	// 	0.559f,  0.861f,  0.639f,
	// 	0.583f,  0.771f,  0.014f,
	// 	0.609f,  0.115f,  0.436f,
	// 	0.327f,  0.483f,  0.844f,
	// 	0.822f,  0.569f,  0.201f,
	// 	0.435f,  0.602f,  0.223f,
	// 	0.310f,  0.747f,  0.185f,
	// 	0.597f,  0.770f,  0.761f,
	// 	0.559f,  0.436f,  0.730f,
	// 	0.359f,  0.583f,  0.152f,
	// 	0.483f,  0.596f,  0.789f,
	// 	0.559f,  0.861f,  0.639f,
	// 	0.583f,  0.771f,  0.014f,
	// 	0.609f,  0.115f,  0.436f,
	// 	0.327f,  0.483f,  0.844f,
	// 	0.822f,  0.569f,  0.201f,
	// 	0.435f,  0.602f,  0.223f,
	// 	0.310f,  0.747f,  0.185f,
	// 	0.597f,  0.770f,  0.761f,
	// 	0.559f,  0.436f,  0.730f,
	// 	0.359f,  0.583f,  0.152f,
	// 	0.483f,  0.596f,  0.789f,
	// 	0.559f,  0.861f,  0.639f,
	// 	0.583f,  0.771f,  0.014f,
	// 	0.609f,  0.115f,  0.436f,
	// 	0.327f,  0.483f,  0.844f,
	// 	0.822f,  0.569f,  0.201f,
	// 	0.435f,  0.602f,  0.223f,
	// 	0.310f,  0.747f,  0.185f,
	// 	0.597f,  0.770f,  0.761f,
	// 	0.559f,  0.436f,  0.730f,
	// 	0.359f,  0.583f,  0.152f,
	// 	0.483f,  0.596f,  0.789f,
	// 	0.559f,  0.861f,  0.639f,
	// };

	static const GLfloat g_uv_buffer_data[] = {
		0.0f, 0.0f, //Sisi kiri
		1.0f, 0.0f,
		0.0f, 0.11111f,
		1.0f, 0.11111f,
		1.0f, 0.0f,
		0.0f, 0.11111f,
		0.27778f, 0.11111f,
		1.0f, 0.11111f,
		0.72222f, 0.22222f,
		0.72222f, 0.22222f,
		0.38889f, 0.22222f,
		0.27778f, 0.11111f,

		0.0f, 0.0f, //Sisi kanan
		1.0f, 0.0f,
		0.0f, 0.11111f,
		1.0f, 0.11111f,
		1.0f, 0.0f,
		0.0f, 0.11111f,
		0.27778f, 0.11111f,
		1.0f, 0.11111f,
		0.72222f, 0.22222f,
		0.72222f, 0.22222f,
		0.38889f, 0.22222f,
		0.27778f, 0.11111f,

		0.4f, 0.2f,   //bumper
		0.4f, 0.0f,
		0.0f, 0.0f, 
		0.0f, 0.0f, 
		0.0f, 0.2f, 
		0.4f, 0.2f, 

		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 0.2f,
		0.2f, 0.2f,
		0.0f, 0.2f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 0.2f,
		0.2f, 0.2f,
		0.0f, 0.2f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 0.2f,
		0.2f, 0.2f,
		0.0f, 0.2f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 0.2f,
		0.2f, 0.2f,
		0.0f, 0.2f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 0.2f,
		0.2f, 0.2f,
		0.0f, 0.2f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 0.2f,
		0.2f, 0.2f,
		0.0f, 0.2f,
		0.0f, 0.0f,
	};

	static const GLfloat g_wheel_uv_buffer_data[] = {
		0.0f, 0.2f,
		0.2f, 0.2f,
		0.2f, 0.0f,
		0.2f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.2f, 0.2f,
		0.2f, 0.0f,
		0.2f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.2f, 0.2f,
		0.2f, 0.0f,
		0.2f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.2f, 0.2f,
		0.2f, 0.0f,
		0.2f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.1f, 0.2f,
		0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.2f,
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// GLuint colorbuffer;
	// glGenBuffers(1, &colorbuffer);
	// glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint frontwheelvertexbuffer;
	glGenBuffers(1, &frontwheelvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, frontwheelvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_frontwheel_buffer_data), g_frontwheel_buffer_data, GL_STATIC_DRAW);

	GLuint backwheelvertexbuffer;
	glGenBuffers(1, &backwheelvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, backwheelvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_backwheel_buffer_data), g_backwheel_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	GLuint wheeluvbuffer;
	glGenBuffers(1, &wheeluvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wheeluvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_wheel_uv_buffer_data), g_wheel_uv_buffer_data, GL_STATIC_DRAW);

	float angle = 0;

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//--------------car----------------------------------------------------
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : texture
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 66*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		//------------------front wheel------------------------------------------------------------------------
		// Compute the MVP matrix from keyboard and mouse input
		glUseProgram(frontwheelprogramID);
		
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrixFrontWheel = getProjectionMatrix();
		glm::mat4 ViewMatrixFrontWheel = getViewMatrix();
		glm::mat4 ModelMatrixFrontWheel = glm::mat4(1.0);

		glm::mat4 FrontwheelSubTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.0f, 0.0f));
		glm::mat4 FrontwheelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 FrontwheelAddTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.0f, 0.0f));
		glm::mat4 FrontwheelTransform = FrontwheelAddTranslation * FrontwheelRotation * FrontwheelSubTranslation;
		
		glm::mat4 MVPFrontWheel = ProjectionMatrixFrontWheel * ViewMatrixFrontWheel * ModelMatrixFrontWheel * FrontwheelTransform;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(FrontWheelMatrixID, 1, GL_FALSE, &MVPFrontWheel[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, frontwheelvertexbuffer);
		glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : texture
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, wheeluvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 72*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(FrontWheelMatrixID, 1, GL_FALSE, &MVPFrontWheel[0][0]);

		
		
		//--------------------------------------backwheel-----------------------------------------------------
		// Compute the MVP matrix from keyboard and mouse input
		glUseProgram(backwheelprogramID);
		
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrixBackWheel = getProjectionMatrix();
		glm::mat4 ViewMatrixBackWheel = getViewMatrix();
		glm::mat4 ModelMatrixBackWheel = glm::mat4(1.0);

		glm::mat4 BackwheelSubTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.0f, 0.0f));
		glm::mat4 BackwheelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 BackwheelAddTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.0f, 0.0f));
		glm::mat4 BackwheelTransform = BackwheelAddTranslation * BackwheelRotation * BackwheelSubTranslation;

		glm::mat4 MVPBackWheel = ProjectionMatrixBackWheel * ViewMatrixBackWheel * ModelMatrixBackWheel * BackwheelTransform;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(BackWheelMatrixID, 1, GL_FALSE, &MVPBackWheel[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, backwheelvertexbuffer);
		glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : texture
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, wheeluvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 72*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(BackWheelMatrixID, 1, GL_FALSE, &MVPBackWheel[0][0]);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		angle -= 1.0f;

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

