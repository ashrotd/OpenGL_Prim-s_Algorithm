#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>
  // Add this for usleep
const int edgeAddDelayMillis = 1000;  // 1000 milliseconds (1 second) delay between edge additions

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
bool animateMST = false;
float animationProgress = 0.0f;
bool animationDirection = true;

struct Vertex {
    float x;
    float y;
    char label;
};

struct Edge {
    char vertex1Label;
    char vertex2Label;
    float cost;
    bool inMST; // Flag to indicate whether the edge is in the MST

    Edge() : inMST(false) {}
};


std::vector<Vertex> vertices;
std::vector<Edge> edges;
 // To store edges considered by Prim's algorithm


bool edgeCreationMode = false;
char nextLabel = 'A';
bool primAlgorithmActive = false;


void drawCircle(float centerX, float centerY, float radius, int segments) {
    // ... (no changes to the drawCircle function)
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw vertices
    glColor3f(0.529f, 0.808f, 0.922f); // Set color to blue
    for (const auto& vertex : vertices) {
        drawCircle(vertex.x, vertex.y, 0.03f, 50); // Adjust the radius and segments as needed

        // Draw the vertex label
        glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for labels
        glRasterPos2f(vertex.x + 0.03f, vertex.y + 0.03f);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, vertex.label);
    }

    // Draw edges
    glBegin(GL_LINES);
    for (const auto& edge : edges) {
        std::cout<<"IN MSt!! "<<edge.inMST<<std::endl;
        Vertex vertex1, vertex2;
        bool foundVertex1 = false, foundVertex2 = false;
        
        // Find the vertices corresponding to the edge labels
        for (const auto& vertex : vertices) {
            if (vertex.label == edge.vertex1Label) {
                vertex1 = vertex;
                foundVertex1 = true;
            }
            if (vertex.label == edge.vertex2Label) {
                vertex2 = vertex;
                
                foundVertex2 = true;
            }

            if (foundVertex1 && foundVertex2) {
                
                break;
            }
        }
        std::cout<<"In mst "<<edge.inMST<<std::endl;
        if (edge.inMST && animateMST) {
            float greenValue = 1.0f - animationProgress;
            float redValue = animationProgress;
            glColor3f(redValue, greenValue, 0.0f);
        } else if (!edge.inMST) {
            glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for other edges
        }
        // Draw the edge if both vertices were found
        if (foundVertex1 && foundVertex2) {
            glVertex2f(vertex1.x, vertex1.y);
            glVertex2f(vertex2.x, vertex2.y);
            // Calculate the midpoint
            
            
        }
    }
    glEnd();

    // Display the instruction message
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for the message
    glRasterPos2f(-0.9f, 0.9f); // Set the position for the message
    std::string message = "Left-click to create vertices, Right-click to create edges";
    
    for (const char& c : message) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Larger font
    }
     // Update the animation progress based on direction
    if (animateMST) {
        if (animationDirection) {
            animationProgress += 0.005f;
            if (animationProgress >= 1.0f) {
                animationProgress = 1.0f;
                animationDirection = false;
            }
        } else {
            animationProgress -= 0.005f;
            if (animationProgress <= 0.0f) {
                animationProgress = 0.0f;
                animationDirection = true;
            }
        }
    }

    glutPostRedisplay();
    glutSwapBuffers();
}


char selectedVertexLabel = 0; // Track the first selected vertex label

void mouse(int button, int state, int x, int y) {
    static Vertex* selectedVertices[2] = {nullptr};
    static int numSelectedVertices = 0; // Make it a static variable

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Left-click: Add a new vertex
        float xpos = static_cast<float>(x) / WINDOW_WIDTH * 2 - 1;
        float ypos = 1 - static_cast<float>(y) / WINDOW_HEIGHT * 2;

        // Check if the user clicked on an existing vertex
        Vertex* clickedVertex = nullptr;

        for (auto& vertex : vertices) {
            float distance = sqrt((xpos - vertex.x) * (xpos - vertex.x) +
                                  (ypos - vertex.y) * (ypos - vertex.y));
            if (distance < 0.02f) {
                clickedVertex = &vertex;
                break;  // Exit the loop if a vertex is found
            }
        }

        if (!clickedVertex) {
            // If no vertex is clicked, create a new vertex
            Vertex newVertex;
            newVertex.x = xpos;
            newVertex.y = ypos;
            newVertex.label = nextLabel++;

            vertices.push_back(newVertex);
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Right-click: Add an edge between two selected vertices

        float xpos = static_cast<float>(x) / WINDOW_WIDTH * 2 - 1;
        float ypos = 1 - static_cast<float>(y) / WINDOW_HEIGHT * 2;

        // Check if the user clicked on an existing vertex
        for (auto& vertex : vertices) {
            float distance = sqrt((xpos - vertex.x) * (xpos - vertex.x) +
                                  (ypos - vertex.y) * (ypos - vertex.y));
            if (distance < 0.02f) {
                if (numSelectedVertices < 2) {
                    selectedVertices[numSelectedVertices] = &vertex;
                    numSelectedVertices++;
                } else {
                    break;
                }
            }
        }

        if (numSelectedVertices == 2) {
            // Two vertices are selected, create an edge
            Edge newEdge;
            newEdge.vertex1Label = selectedVertices[0]->label;
            newEdge.vertex2Label = selectedVertices[1]->label;
            // Prompt the user to enter the cost
            std::cout << "Enter the cost for the edge between " 
                      << newEdge.vertex1Label << " and " 
                      << newEdge.vertex2Label << ": ";
            std::cin >> newEdge.cost;

            
            edges.push_back(newEdge);

            // Reset the selection
            numSelectedVertices = 0;
        }
    }

    glutPostRedisplay();
}

int getIndexByLabel(char label) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i].label == label) {
            return static_cast<int>(i);
        }
    }
    return -1; // Label not found
}

void primAlgorithm() {
    // Clear the MST flags for all edges
    for (auto& edge : edges) {
        edge.inMST = false;
    }

    // Check if there are vertices to start the algorithm
    if (vertices.empty()) {
        std::cout << "No vertices to start Prim's algorithm." << std::endl;
        return;
    }

    // Initialize a vector to track visited vertices
    std::vector<bool> visited(vertices.size(), false);

    // Start from the first vertex
    visited[0] = true;

    // Perform Prim's algorithm until all vertices are visited
    while (true) {
        float minCost = std::numeric_limits<float>::max();
        Edge minEdge;
        bool foundUnvisited = false;

        // Find the minimum cost edge connecting a visited vertex to an unvisited one
        for (auto& edge : edges) {
            if (visited[getIndexByLabel(edge.vertex1Label)] != visited[getIndexByLabel(edge.vertex2Label)]) {
                if (edge.cost < minCost) {
                    minCost = edge.cost;
                    minEdge = edge;
                    foundUnvisited = true;
                    
                    edge.inMST = true;
                    
                    std::cout<<"Min cost: "<<minCost<<std::endl;
                }
            }
        }

        if (!foundUnvisited) {
            break; // All vertices are visited, exit the loop
        }

        // Mark the selected edge as part of the MST
        minEdge.inMST = true;
        animateMST = true;
        animationProgress = 0.0f;
        std::cout << "Edge in MST: " << minEdge.vertex1Label << " - " << minEdge.vertex2Label << std::endl;
        

        // Mark the unvisited vertex as visited
        if (!visited[getIndexByLabel(minEdge.vertex1Label)]) {
            visited[getIndexByLabel(minEdge.vertex1Label)] = true;
        } else {
            visited[getIndexByLabel(minEdge.vertex2Label)] = true;
        }
    }
    
    // Call glutPostRedisplay() to update the display
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 'p' || key == 'P') {
        // Press 'p' to start Prim's algorithm
        primAlgorithmActive = true;
        primAlgorithm();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Prim's Algorithm Visualization");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
