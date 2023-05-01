//Zach Pedersen
//This is my work
//Prof Citro
//CST-310

#include <iostream> 

// GLEW
#define GLEW_STATIC 
#include <GL/glew.h> 


// GLFW
#include <GLFW/glfw3.h> 

// SOIL
#include <SOIL/SOIL.h> 

// GLM Mathematics
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

// Other includes
#include "shader.h" 
#include "Camera.h" 
#include "Model.h" 

const GLuint WIDTH = 800, HEIGHT = 600; 

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); 
void do_movement(); 

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f)); 
GLfloat lastX = WIDTH / 2.0; 
GLfloat lastY = HEIGHT / 2.0; 
bool keys[1024]; 

glm::vec3 lightPos(1.0f, 1.0f, -2.0f); 

GLfloat deltaTime = 0.0f; 
GLfloat lastFrame = 0.0f; 

int main() {
    // Init GLFW
    glfwInit(); // Initialize GLFW
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    // Create GLFWwindow
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project 10", nullptr, nullptr); 
    glfwMakeContextCurrent(window); 

    // Set required callback functions
    glfwSetKeyCallback(window, key_callback); // Set key_callback method

    glewExperimental = GL_TRUE; // Set glew to experimental

    glewInit(); // Initialize GLEW

    glViewport(0, 0, WIDTH, HEIGHT); // Define viewport dimensions

    glEnable(GL_DEPTH_TEST); // Set up OpenGL options

    // INSERT SHADERS HERE FOR PROJECT 10
    Shader checkerboardShader("checkerboard.vs", "checkerboard.frag"); 
    // Shader cubeShader("cube.vs", "cube.frag"); 
    // Shader cylinderShader("cylinder.vs", "cylinder.frag"); 
    // Shader sphereShader("sphere.vs", "sphere.frag"); 
    Shader cubeShader("MoreCubes.vs", "MoreCubes.frag"); 
    // Models for Cylinder and Sphere
    Model cylinderModel("cylinder.obj"); 
    Model sphereModel("sphere.obj"); 

    GLfloat vertices[] = {
        // Coordinates: 3 Position, 3 Color, 2 Texture
        // Back face of cube
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f, // Bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f, // Bottom right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f, // Upper right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f, // Upper right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f, // Upper left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f, // Bottom left

        // Front face of cube
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f, // Bottom left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, // Bottom right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f, // Upper right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f, // Upper right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f, // Upper left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f, // Bottom left

        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // Upper close
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f, // Upper far
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // Lower far
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // Lower far
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, // Lower close
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // Upper close

        // Right face
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f, // Upper close
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f, // Upper far
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, // Lower far
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, // Lower far
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, // Lower close
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f, // Upper close

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f, // Left far
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f, // Right far
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f, // Right close
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f, // Right close
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, // Left close
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f, // Left far

        // Top Face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f, // Left far
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f, // Right far
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f, // Right close
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f, // Right close
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f, // Left close
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f // Left far
    };
    GLuint VBO, VAO; 
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO); 

    glBindVertexArray(VAO);  // Bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); 
    glEnableVertexAttribArray(0); // Enable vertex attrib array with 0
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(2); // Enable with 2
    // Normalized Position Attribute
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); 
    glEnableVertexAttribArray(4); // Enable with 4

    glBindVertexArray(0); // Unbind VAO

    // DEFINE TEXTURES HERE Project 10 --> NOTE FOR PROJECT 10

    //SPHERE TEXTURE
    GLuint testureCyl;
    glGenTextures(1, &testureCyl);
    glBindTexture(GL_TEXTURE_2D, testureCyl); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int Sphwidth, spheight;
    // Load image, create texture and generate mipmaps
    unsigned char* Sphearimage = SOIL_load_image("Bump-Picture.jpg", &Sphwidth, &spheight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Sphwidth, spheight, 0, GL_RGB, GL_UNSIGNED_BYTE, Sphearimage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(Sphearimage);
    glBindTexture(GL_TEXTURE_2D, 0);
    //glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    
    GLuint sky;
    glGenTextures(1, &sky);
    glBindTexture(GL_TEXTURE_2D, sky); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int Skywidth, skyheight;
    // Load image, create texture and generate mipmaps
    unsigned char* Skeyimage = SOIL_load_image("skybox.jpg", &Skywidth, &skyheight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Skywidth, skyheight, 0, GL_RGB, GL_UNSIGNED_BYTE, Skeyimage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(Skeyimage);
    glBindTexture(GL_TEXTURE_2D, 0);



    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime for camera movement
        GLfloat currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame; 
        lastFrame = currentFrame; 

        // Check for events
        glfwPollEvents(); // Callback glfwPollEvents to check for events
        do_movement(); 

        glClearColor(0.0f, 0.1f, 0.0f, 0.0f); // Set background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        
        // Initialize Camera
        glm::mat4 view = glm::mat4(1.0f); // Initialize view to identity
        view = camera.GetViewMatrix(); // Set view based on camera
        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f); // Initialize projection using initial values
        glm::mat4 model = glm::mat4(1.0f); // Initialize model to be 4x4 identity
        


        // CHECKERBOARD
        checkerboardShader.Use(); // Use checkerboard shader

        GLint squareColorLoc = glGetUniformLocation(checkerboardShader.Program, "squareColor"); 
        GLint lightColorLoc = glGetUniformLocation(checkerboardShader.Program, "lightColor"); 
        GLint lightPosLoc = glGetUniformLocation(checkerboardShader.Program, "lightPos"); 
        GLint viewPosLoc = glGetUniformLocation(checkerboardShader.Program, "viewPos"); 

        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Pass white color to lightColorLoc uniform
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); 
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); 

        glm::mat4 view_square = view; // Create mat4 view_square equal to view generic defined above

        GLint modelLoc = glGetUniformLocation(checkerboardShader.Program, "model"); 
        GLint viewLoc = glGetUniformLocation(checkerboardShader.Program, "view"); 
        GLint projLoc = glGetUniformLocation(checkerboardShader.Program, "projection"); 

        for (int i = 0; i < 8; i++) { // For 8 rows
            for (int j = 0; j < 8; j++) { // For 8 columns
                if ((i+j) % 2 == 0) { // Check if i+j is odd or even for color purposes
                    glUniform3f(squareColorLoc, 1.0f, 0.0f, 0.0f); 
                } else {
                    glUniform3f(squareColorLoc, 1.0f, 1.0f, 1.0f); 
                }
                view_square = glm::translate(view_square, glm::vec3(j-4.0f, -0.5f, i-9.0f)); 
                view_square = glm::scale(view_square, glm::vec3(1.0f, 0.1f, 1.0f)); 
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_square)); 
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
                // Draw square
                glBindVertexArray(VAO); // Bind vertex arrays
                glDrawArrays(GL_TRIANGLES, 0, 36); // Draw arrays for cube
                view_square = view; 
                
            }
        }


        // CUBE
        cubeShader.Use(); // Activate cube shader
        glBindTexture(GL_TEXTURE_2D, sky);
        glBindVertexArray(0); // Bind VAO

        // Set uniform locations
        GLint cubeColorLoc = glGetUniformLocation(cubeShader.Program, "cubeColor"); // Retrieve uniform location
        lightColorLoc = glGetUniformLocation(cubeShader.Program, "lightColor"); // Reset uniform location for cubeShader
        lightPosLoc = glGetUniformLocation(cubeShader.Program, "lightPos"); // Reset uniform location for cubeShader
        viewPosLoc = glGetUniformLocation(cubeShader.Program, "viewPos"); // Reset uniform location for cubeShader

        // Pass to shaders
        glUniform3f(cubeColorLoc, 1.0f, 0.0f, 0.0f); // Pass cube color to uniform
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Pass light color to uniform
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); // Pass light position to uniform
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); // Pass camera position to uniform

        glm::mat4 view_cube = view; // Create mat4 view_cube equal to identity view
        view_cube = glm::translate(view_cube, glm::vec3(0.0f, 0.0f, -5.0f)); // Translate cube back

        // Get uniform location
        modelLoc = glGetUniformLocation(cubeShader.Program, "model"); // Reset modelLoc using cubeShader
        viewLoc = glGetUniformLocation(cubeShader.Program, "view"); // Reset viewLoc using cubeShader
        projLoc = glGetUniformLocation(cubeShader.Program, "projection"); // Reset projLoc using cubeShader
        // Pass locations to shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Pass model to shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_cube)); // Pass view_cube to shader
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection to shader
        // Draw cube
        glActiveTexture(GL_TEXTURE0);
        
        glBindVertexArray(VAO); // Bind vertex arrays
        glDrawArrays(GL_TRIANGLES, 0, 36); // Draw cube

        
        // CYLINDER
        cubeShader.Use(); // Activate cylinder shader
        glBindTexture(GL_TEXTURE_2D, testureCyl);
        glBindVertexArray(0); // Bind VAO
       

        GLint cylinderColorLoc = glGetUniformLocation(cubeShader.Program, "cylinderColor"); 
        lightColorLoc = glGetUniformLocation(cubeShader.Program, "lightColor"); 
        lightPosLoc = glGetUniformLocation(cubeShader.Program, "lightPos"); 
        viewPosLoc = glGetUniformLocation(cubeShader.Program, "viewPos"); 

        glUniform3f(cylinderColorLoc, 0.0f, 1.0f, 0.0f); 
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); 
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); 
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); 

        modelLoc = glGetUniformLocation(cubeShader.Program, "model"); // Reset view location for cylinderShader
        viewLoc = glGetUniformLocation(cubeShader.Program, "view"); // Reset view location for cylinderShader
        projLoc = glGetUniformLocation(cubeShader.Program, "projection"); // Reset view location for cylinderShader

        glm::mat4 view_cylinder = view; // Create mat4 view_cylinder using generic view identity
        view_cylinder = glm::translate(view_cylinder, glm::vec3(1.2f, -3.0f, -5.5f)); 
        view_cylinder = glm::scale(view_cylinder, glm::vec3(0.5, 3.0, 0.5)); 

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_cylinder)); 
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 

        
        cylinderModel.Draw(cubeShader); // Draw obj model

       // Swap screen buffers


        
        // SPHERE
        cubeShader.Use(); // Activate sphereShader

        GLint sphereColorLoc = glGetUniformLocation(cubeShader.Program, "sphereColor"); 
        lightColorLoc = glGetUniformLocation(cubeShader.Program, "lightColor"); 
        lightPosLoc = glGetUniformLocation(cubeShader.Program, "lightPos"); 
        viewPosLoc = glGetUniformLocation(cubeShader.Program, "viewPos"); 

        glUniform3f(sphereColorLoc, 0.0f, 0.0f, 1.0f); 
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); 
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); 
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); 

        modelLoc = glGetUniformLocation(cubeShader.Program, "model"); 
        viewLoc = glGetUniformLocation(cubeShader.Program, "view"); 
        projLoc = glGetUniformLocation(cubeShader.Program, "projection"); 

        glm::mat4 view_sphere = view; // Create mat4 view_sphere equal to view identity
        view_sphere = glm::translate(view_sphere, glm::vec3(-1.5f, 0.0f, -5.0f)); 
        view_sphere = glm::scale(view_sphere, glm::vec3(0.5f, 0.5f, 0.5f)); // Scale down sphere
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_sphere)); 
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 

        sphereModel.Draw(cubeShader); // Draw sphere obj model
        glBindTexture(GL_TEXTURE_2D, testureCyl);
        glBindVertexArray(0); // Bind zero at end

        glfwSwapBuffers(window); 

    }
    // Deallocate resources
    glDeleteVertexArrays(1, &VAO); // Deallocate vertex arrays
    glDeleteBuffers(1, &VBO); // Deallocate buffers
    glfwTerminate(); // Terminate window
    return 0; // Returns 0 for end of int main()

}

// Method for key input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { // If ESC pressed
        glfwSetWindowShouldClose(window, GL_TRUE); // Close window
    } if (key >= 0 && key < 1024) { // Allow for 1024 key presses
        if (action == GLFW_PRESS) { // If pressed
            keys[key] = true; // Set keys[key] = true [key pressed]
        } else if (action == GLFW_RELEASE) { // If released
            keys[key] = false; // Set keys[key] = false [key not pressed]
        }
    }
}

// Initiates movement based on keyboard input
void do_movement() {
    if (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]) {
        if (keys[GLFW_KEY_UP]) { 
            camera.ProcessKeyboard(FORWARD, deltaTime); 
        } else if (keys[GLFW_KEY_DOWN]) { 
            camera.ProcessKeyboard(BACKWARD, deltaTime); 
        } else if (keys[GLFW_KEY_COMMA]) { 
            camera.ProcessKeyboard(UPROLL, deltaTime); 
        } else if (keys[GLFW_KEY_PERIOD]) { 
            camera.ProcessKeyboard(DOWNROLL, deltaTime); 
        }
    }
    else if (keys[GLFW_KEY_LEFT_CONTROL] || keys[GLFW_KEY_RIGHT_CONTROL]) { 
        if (keys[GLFW_KEY_DOWN]) { 
            camera.ProcessKeyboard(UPPITCH, deltaTime); 
        } else if (keys[GLFW_KEY_UP]) { 
            camera.ProcessKeyboard(DOWNPITCH, deltaTime); 
        } else if (keys[GLFW_KEY_RIGHT]) { 
            camera.ProcessKeyboard(UPYAW, deltaTime); 
        } else if (keys[GLFW_KEY_LEFT]) { 
            camera.ProcessKeyboard(DOWNYAW, deltaTime); 
        }
    }
    else if (keys[GLFW_KEY_RIGHT]) { 
        camera.ProcessKeyboard(RIGHT, deltaTime); 
    }
    else if (keys[GLFW_KEY_LEFT]) { // If left arrow
        camera.ProcessKeyboard(LEFT, deltaTime); 
    }
    else if (keys[GLFW_KEY_UP]) { // If up arrow
        camera.ProcessKeyboard(UP, deltaTime); 
    }
    else if (keys[GLFW_KEY_DOWN]) { 
        camera.ProcessKeyboard(DOWN, deltaTime); 
    }
    else if (keys[GLFW_KEY_R]) { 
        camera.ResetCamera(); 
    }
}

  