/*
Nicole Vilkoff (40321430) 
Christa Abou Arraje (40226631)
Anabel Prévost (40265371)
*/

#include <iostream>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImportedModel.h"

unsigned int renderingProgram;

//file read from
ImportedModel myModel("mug.obj");

//number of vertices and number of face references of our object
int nbFacesRef;
int nbVertices;
float* verticesShape;

//method that sets up the array send in the VBO (ordered vertices using faces references)
void setupVertices(void)
{
    nbFacesRef = myModel.getFaces().size();
    nbVertices = myModel.getVertices().size();

   //array that contains all the vertices in order taking for account the faces
   verticesShape = new float[3 * nbFacesRef];

   //fill up the array
   for (int i = 0; i < nbFacesRef; i++)
   {
       verticesShape[i*3] = myModel.getVertices()[3.0*myModel.getFaces()[i]];
       verticesShape[i*3+1] = myModel.getVertices()[3.0*myModel.getFaces()[i] +1];
       verticesShape[i*3+2] = myModel.getVertices()[3.0*myModel.getFaces()[i] +2];
   }
}

//method used to create the shader programs
unsigned int createShaderProgram()
{
    //vertex shsader program
    const char* vShaderSource =
        "#version 430 \n"
        "layout(location = 0) in vec3 aPos; \n"
        "uniform mat4 transform; \n"
        "void main(void) \n"
        "{gl_Position = transform * vec4(aPos, 1.0);}";

    //fragment shader program
    const char* fShaderSource = "#version 430 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{color = vec4(0.85,0.16,0.49,0.0);}";

    //create empty shader objects and associate their references
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);

    //specify the sources of the shader (shader, count, string, length)
    glShaderSource(vShader, 1, &vShaderSource, NULL);
    glShaderSource(fShader, 1, &fShaderSource, NULL);

    //compile the source code string of the shader
    glCompileShader(vShader);
    glCompileShader(fShader);

    //create an empty program and associates its reference
    unsigned int vfProgram = glCreateProgram();

    //attach the shaders to the program
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    //link the program
    glLinkProgram(vfProgram);

    //checks to see whether the executables vfProgram can execute given the current OpenGL state
    glValidateProgram(vfProgram);

    //return the reference of the program
    return vfProgram;
}

//declare VBO and VAO
unsigned int VBO;
unsigned int VAO;

//method used to initialize the buffers (executed before the main loop)
void init(GLFWwindow* window)
{
    //associa the reference of the created shader program to renderingProgram
    renderingProgram = createShaderProgram();

    //generate a vertex buffer array of size 1 and return its reference
    glGenVertexArrays(1, &VAO);

    //generate a buffer of size 1 and return its reference 
    glGenBuffers(1, &VBO);

    //bind the VAO to the vertex array buffer
    glBindVertexArray(VAO);

    //binds the VBO specified buffer binding point (vertex attributes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //create a new data store for the VBO (binds the content of the buffer)(buffer, size, data, usage)
    glBufferData(GL_ARRAY_BUFFER, 3 * nbFacesRef * sizeof(float), verticesShape, GL_STATIC_DRAW);
    
    //enable the generic vertex attribute array at index 0
    glEnableVertexAttribArray(0);

    //specify the location and data format of the array of generic vertex attributes at index index to use when rendering (index, size, type, normalized, stride, pointer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

}

//base state of our transformations
glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
float angle = 0.0f;
float scaleFactor = 1.0f;

//method used to display on the window (called inside the main loop)
void display(GLFWwindow* window)
{
    //use the created program to render
    glUseProgram(renderingProgram);

    //set the color of the background and fill it with it
    glClearColor(0.96, 0.65, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT); 
   
    //initialize transform as the identity matrix
    glm::mat4 transform = glm::mat4(1.0f);
    
    //do rotation, translation and scaing using matrix operation 
    transform = glm::translate(transform, translation);
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scaleFactor, scaleFactor, 1.0f));

    //assign the location of the uniform variable transform of the program(in the vertex shader) to transformLoc
    unsigned int transformLoc = glGetUniformLocation(renderingProgram, "transform");

    //modify the value of the uniform variable (location, count, transpose, value)
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    //to see only the wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, nbVertices);
}


//define the key callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //window will close when escape pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    //translation uppwards when W is pressed
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        translation.y += 0.05f;
    }
    //trnaslation downwards when S is pressed
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        translation.y -= 0.05f;
    }
    //translation to the left when A is pressed
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        translation.x -= 0.05f;
    }
    //translation to the right when D is pressed
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        translation.x += 0.05f;
    }
    //rotation of 30 degrees counterclockwise when Q is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        angle += 30;
    }
    //rotation 30 degrees clockwise when E is pressed
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        angle -= 30;
    }
    //scale times 0.05 when R is pressed
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        scaleFactor += 0.05;
    }
    //scale minus time 0.5 when F is pressed
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        scaleFactor -= 0.05;
    }
}

int main()
{
    GLFWwindow* window;

    //if the initialization failed dispaly an error message and quit
    if (!glfwInit())
    {
        std::cout << "The initialization has failed!";
        glfwTerminate();
        return -1;
    }

    //create a window
    window = glfwCreateWindow(800, 800, "Wireframe rendering", NULL, NULL);

    //if the creation of the window failed, display an error message and quit
    if (!window)
    {
        std::cout << "The creation of the window failed!";
        glfwTerminate();
        return -2;
    }

    //make the context current
    glfwMakeContextCurrent(window);

    //if the initialization of the glew library failed, return an error message and quit
    if (glewInit() != GLEW_OK)
    {
        std::cout << "The initialization of the glew library failed!";
        glfwTerminate();
        return -3;
    }

    //set the key callbacks
    glfwSetKeyCallback(window, key_callback);

    //setup the vertices of our object
    setupVertices();

    //call the init function that will associate the shader to this program
    init(window);

    //main loop that will execute until the window closes
    while (!glfwWindowShouldClose(window))
    {
        //call the display function that will display and transform our triangle in the window
        display(window);

        //swap the front and back fuffer
        glfwSwapBuffers(window);

        //process the events in the event queue (for callbacks)
        glfwPollEvents();
    }

    //liberate all the ressources from glfw and gl and display a program success message
    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(renderingProgram);
    glfwTerminate();
    std::cout << "The program was successfull!!";
};
   

