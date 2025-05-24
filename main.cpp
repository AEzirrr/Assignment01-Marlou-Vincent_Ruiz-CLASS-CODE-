#include "model.h"
#include "camera.h"
#include "Physics/MyVector.h" 
#include "Physics/P6Particle.h" 
#include "Physics/PhysicsWorld.h" 
#include "RenderParticle.h"
#include "chrono"

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timeStep(16ms);

#define pi 3.141592653589793238462643383279502884197

// camera speed values
float camSpeed = 0.0035f;
float camLookSpeed = 0.035f;

// Stores the state of the key (pressed or released)
std::unordered_map<int, bool> keyStates;

// Value for tracking object spawn timer
double lastSpawn = 0.0;

// Values for camera position and rotation
float x_cam = 0;
float y_cam = 0;
float z_cam = 0;

float yaw = 270;
float pitch = 0;

// Value for object's scale
float scaleValX = 1;
float scaleValY = 1;
float scaleValZ = 1;

// Values of object's rotation
float thetaX = 0;
float thetaY = 0;
float thetaZ = 0;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/*void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}*/

/*void ProcessInput() {
    double currTime = glfwGetTime();

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (keyStates[GLFW_KEY_W]) {
        x_cam += front.x * camSpeed;
        y_cam += front.y * camSpeed;
        z_cam += front.z * camSpeed;
    };

    if (keyStates[GLFW_KEY_S]) {
        x_cam -= front.x * camSpeed;
        y_cam -= front.y * camSpeed;
        z_cam -= front.z * camSpeed;
    };

    if (keyStates[GLFW_KEY_A]) {
        x_cam -= right.x * camSpeed;
        y_cam -= front.y * camSpeed;
        z_cam -= right.z * camSpeed;
    };

    if (keyStates[GLFW_KEY_D]) {
        x_cam += right.x * camSpeed;
        y_cam += front.y * camSpeed;
        z_cam += right.z * camSpeed;
    };

    // Moves camera pitch and yaw based on arrow inputs (Look up and down)
    if (keyStates[GLFW_KEY_UP]) pitch += camLookSpeed;
    if (keyStates[GLFW_KEY_DOWN]) pitch -= camLookSpeed;

    if (keyStates[GLFW_KEY_LEFT]) yaw -= camLookSpeed;
    if (keyStates[GLFW_KEY_RIGHT]) yaw += camLookSpeed;
} */

bool AtCenter(const Physics::P6Particle& particle, float epsilon = 0.1f) {
    const auto& pos = particle.position;
    return (std::abs(pos.x) < epsilon) &&
        (std::abs(pos.y) < epsilon) &&
        (std::abs(pos.z) < epsilon);
}

int main(void) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Window values
    float windowWidth = 640;
    float windowHeight = 640;

    // Window creation
    window = glfwCreateWindow(windowWidth, windowHeight, "Marlou Vincent Ruiz", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Input handling for program
    //glfwSetKeyCallback(window, Key_Callback);

    // Load vertex shader
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Load fragment shader
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    // Compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    // Compile the fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    // Link shaders
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);

    OrthographicCamera orthoCamera(
        glm::vec3(-30.0f, 25.0f, -30.0f), // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),      // Front direction
        glm::vec3(0.0f, 1.0f, 0.0f),      // Up direction
        0.0f,                             // Yaw
        0.0f,                             // Pitch
        50.0f,                            // Distance from target
        10.0f                             // Orthographic size
    );

    glm::mat4 projectionMatrix = orthoCamera.GetProjectionMatrix(windowWidth / windowHeight);

    glEnable(GL_DEPTH_TEST);


    // Load sphere
    Model3D sphereObject(
        "3D/sphere.obj", // 3D model
        "", // texture
        glm::vec3(0, 0, 0), // Spawn pos
        glm::vec3(thetaX, thetaY, thetaZ), //Rotation
        glm::vec3(scaleValX, scaleValY, scaleValZ), // Scale
        shaderProg
    );

    // Load sphere
    Model3D sphereObject2(
        "3D/sphere.obj", // 3D model
        "", // texture
        glm::vec3(0, 0, 0), // Spawn pos
        glm::vec3(thetaX, thetaY, thetaZ), //Rotation
        glm::vec3(scaleValX, scaleValY, scaleValZ), // Scale
        shaderProg
    );

    // Load sphere
    Model3D sphereObject3(
        "3D/sphere.obj", // 3D model
        "", // texture
        glm::vec3(0, 0, 0), // Spawn pos
        glm::vec3(thetaX, thetaY, thetaZ), //Rotation
        glm::vec3(scaleValX, scaleValY, scaleValZ), // Scale
        shaderProg
    );

    // Load sphere
    Model3D sphereObject4(
        "3D/sphere.obj", // 3D model
        "", // texture
        glm::vec3(0, 0, 0), // Spawn pos
        glm::vec3(thetaX, thetaY, thetaZ), //Rotation
        glm::vec3(scaleValX, scaleValY, scaleValZ), // Scale
        shaderProg
    );


    //Physics::MyVector myVectorX(5.f, 10.0f, 0.0f);
    //Physics::MyVector myVectorY(5.0f, 10.0f, 0.0f);

    //Physics::MyVector myVectorSample = myVectorX.Direction();

    //std::cout << "Magnitude of MyVectorX: " << myVectorX.Magnitude() << std::endl;
    //std::cout << "Direction of MyVectorX: " << myVectorX.Direction().x << ", " << myVectorX.Direction().y << ", " << myVectorX.Direction().z << std::endl;

    // + operator
    //myVectorSample = myVectorX + myVectorY;

    // - operators
    //myVectorSample = myVectorX - myVectorY;

    // += operator
    //myVectorX += myVectorY;

    // -= operator
    //myVectorX -= myVectorY;


    //Scalar Multiplication
    //myVectorSample = myVectorX * -1;

	//Component Product
	//myVectorSample = myVectorX.ComponentProduct(myVectorY);

    //Dot Product
    //std::cout << "Dot of X and Y: " << myVectorX.ScalarProduct(myVectorY) << std::endl;

	//Cross Product
	//myVectorSample = myVectorX.VectorProduct(myVectorY);

	Physics::PhysicsWorld physicsWorld = Physics::PhysicsWorld();

	Physics::P6Particle particle = Physics::P6Particle();
	particle.position = Physics::MyVector(8, 8, 0);
	particle.velocity = Physics::MyVector(-1, -1, 0);
    particle.acceleration = Physics::MyVector(-2, -2, 0);
	physicsWorld.AddParticle(&particle);
	sphereObject.color(glm::vec3(0.0f, 1.0f, 0.0f));

    Physics::P6Particle particle2 = Physics::P6Particle();
    particle2.position = Physics::MyVector(8, -8, 0);
    particle2.velocity = Physics::MyVector(-1, 1, 0);
    particle2.acceleration = Physics::MyVector(-2, 2, 0);
	physicsWorld.AddParticle(&particle2);
    sphereObject2.color(glm::vec3(0.0f, 1.0f, 0.0f));

    Physics::P6Particle particle3 = Physics::P6Particle();
    particle3.position = Physics::MyVector(-8, 8, 0);
    particle3.velocity = Physics::MyVector(1, -1, 0);
    particle3.acceleration = Physics::MyVector(2, -2, 0);
	physicsWorld.AddParticle(&particle3);
    sphereObject3.color(glm::vec3(0.0f, 1.0f, 0.0f));

    Physics::P6Particle particle4 = Physics::P6Particle();
    particle4.position = Physics::MyVector(-8, -8, 0);
    particle4.velocity = Physics::MyVector(1, 1, 0);
    particle4.acceleration = Physics::MyVector(2, 2, 0);
	physicsWorld.AddParticle(&particle4);
    sphereObject4.color(glm::vec3(0.0f, 1.0f, 0.0f));





	//Initializes the clock and variables
	using clock = std::chrono::high_resolution_clock;
	auto curr_time = clock::now();
	auto prev_time = curr_time;
	std::chrono::nanoseconds curr_ns(0);


    while (!glfwWindowShouldClose(window)) {

		curr_time = clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
		prev_time = curr_time;

		curr_ns += dur;
        if (curr_ns >= timeStep) {
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
			//std::cout << "MS: " << (float)ms.count() << "\n";

            curr_ns -= timeStep;
			std::cout << "P6Update"<< "\n";

			physicsWorld.Update((float)ms.count() / 1000.0f);  

			if (AtCenter(particle)) {
				particle.Destroy();
			}
        }

        std::cout << "Normal Update" << "\n";

        //ProcessInput(); // Key inputs

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!particle.IsDestroyed()) {
            sphereObject.position = (glm::vec3)particle.position;
            sphereObject.draw();
		}

        if (!particle2.IsDestroyed()) {
            sphereObject2.position = (glm::vec3)particle2.position;
            sphereObject2.draw();
        }

        if (!particle3.IsDestroyed()) {
            sphereObject3.position = (glm::vec3)particle3.position;
            sphereObject3.draw();
        }

        if (!particle4.IsDestroyed()) {
            sphereObject4.position = (glm::vec3)particle4.position;
            sphereObject4.draw();
        }

        // Calculates the camera's position and front vector
        glm::vec3 cameraPos = glm::vec3(0 + x_cam, 0 + y_cam, 0 + z_cam);
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        // Calculates the view matrix
        glm::vec3 cameraCenter = cameraPos + front;
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, glm::vec3(0, 1, 0));

        // Pass the view matrix to the shader
        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUseProgram(shaderProg);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // Pass the projection matrix to the shader
        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
