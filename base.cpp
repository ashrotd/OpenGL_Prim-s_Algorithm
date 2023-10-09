#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>


const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

std::vector<glm::vec2> vertices; // Stores the vertices clicked by the user
std::vector<std::pair<int, int>> edges; // Stores the edges as pairs of vertex indices
bool addingEdge = false; // Indicates whether the user is currently adding an edge
int selectedVertex = -1; // Index of the currently selected vertex

std::vector<glm::vec2> points; // Stores the points clicked by the user
bool drawing = false; // Indicates whether the user is currently drawing

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            // Convert screen coordinates to normalized device coordinates
            glm::vec2 point(xpos / WINDOW_WIDTH * 2 - 1, 1 - ypos / WINDOW_HEIGHT * 2);

            if (!addingEdge) {
                // If not adding an edge, check if the user clicked on a vertex
                for (size_t i = 0; i < vertices.size(); i++) {
                    if (glm::distance(vertices[i], point) < 0.02f) {
                        selectedVertex = i;
                        addingEdge = true;
                        break;
                    }
                }
            }

            points.push_back(point);
            drawing = true;
        }
        else if (action == GLFW_RELEASE) {
            drawing = false;
        }
    }
}

// Draw Vertex as circle
void drawCircle(float centerX, float centerY, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Center of the circle

    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = centerX + radius * cosf(theta);
        float y = centerY + radius * sinf(theta);
        glVertex2f(x, y);
    }

    glEnd();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.529f, 0.808f, 0.922f); // Set color to blue

    for (const glm::vec2& point : points) {
        // Draw a small circle at each point
        drawCircle(point.x, point.y, 0.02f, 50); // Adjust the radius and segments as needed
    }
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Prim's Algorithm Visualization", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window)) {
        if (drawing) {
            // Redraw the scene if the user is currently drawing
            draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for edge information

    float rightX = 0.8f; // Adjust this value as needed
    float rightY = 0.9f; // Starting position for displaying edge information
    float lineHeight = 0.03f; // Adjust as needed for line spacing

    for (const auto& edge : edges) {
        std::string costStr = std::to_string(edge.cost);
        costStr.erase(costStr.find_last_not_of('0') + 1, std::string::npos);
        std::string edgeInfo = std::to_string(edge.vertex1Label) + " - " + std::to_string(edge.vertex2Label) + ": " + std::to_string(edge.cost);

        // Draw edge information on the right side
        glRasterPos2f(rightX, rightY);
        for (const char& c : edgeInfo) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }

        // Move down for the next line of edge information
        rightY -= lineHeight;
    }