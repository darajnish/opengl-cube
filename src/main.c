/*
MIT License

Copyright (c) 2021 Rajnish Mishra

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <GL/gl.h>
#include <GL/glut.h>

// Setup window size 500x500
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
// Setup cube controls' rotation speed
#define ROTATION_SPEED 2
#define TITLE "The Rotating Cube"
#define MSG "F: Speed    W,S,A,D: Rotate    SPACE: Pause/Resume    R: Frame    Q: Quit" //  

// A struct to hold (x,y,z) cordinates of a point
typedef struct {
    float x;
    float y;
    float z;
} Point3D;

// A struct to hold (R,G,B) values of color
typedef struct {
    float R;
    float G;
    float B;
} ColorRGB;

// All faces and vertices of the cube, each row represents the four vertices of each face
Point3D vertices[6][4] = {
    {{ 0.5, 0.5, 0.5}, { 0.5,-0.5, 0.5}, {-0.5,-0.5, 0.5}, {-0.5, 0.5, 0.5}},
    {{-0.5, 0.5, 0.5}, {-0.5,-0.5, 0.5}, {-0.5,-0.5,-0.5}, {-0.5, 0.5,-0.5}},
    {{-0.5, 0.5,-0.5}, {-0.5,-0.5,-0.5}, { 0.5,-0.5,-0.5}, { 0.5, 0.5,-0.5}},
    {{ 0.5, 0.5,-0.5}, { 0.5,-0.5,-0.5}, { 0.5,-0.5, 0.5}, { 0.5, 0.5, 0.5}},
    {{-0.5, 0.5,-0.5}, { 0.5, 0.5,-0.5}, { 0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}},
    {{-0.5,-0.5,-0.5}, { 0.5,-0.5,-0.5}, { 0.5,-0.5, 0.5}, {-0.5,-0.5, 0.5}}
};

// All the colors for each face of the cube
ColorRGB colors[6] = {
    {0.376, 0.007, 0.933},
    {0.839, 0.007, 0.933},
    {0.933, 0.376, 0.007},
    {0.564, 0.933, 0.007},
    {0.376, 0.490, 0.545},
    {0.898, 0.223, 0.207}
};

// variables to hold temporary values of the angles and given inputs from the keyboard
float aX = 30, aY = 1, aZ = 0, tY = 0;
int frame = 0, stop = 0;

// A function which gets called whenever GL wants to display
void display();
// This function sets initial setup for display 
void display_init();
// This function gets called whenever a key is pressed
void keyboard(unsigned char, int, int);

int main(int argc, char** argv) {
    // initializes GLUT
    glutInit(&argc, argv);
    // Set display mode, we need double buffers, RGB colors and one of the two buffers for depth
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Set GLUT window size
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT);
    // Set GLUT window title
    glutCreateWindow("Cube");
    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    // Set display callback function 
    glutDisplayFunc(display);
    // Set idle callback function
    glutIdleFunc(display);
    // Set keyboard callback function
    glutKeyboardFunc(keyboard);
    // Run initial setup
    display_init();
    /* Handover the controls to GL main loop and let the callback functions execute */
    glutMainLoop();
    return 0;
}

/**
* This function can take a string and can draw them from a given (x,y,z) cordinate
* with bitmap fonts
*/
void drawString(float x, float y, float z, char *string, int big_font) {
    char* c;
    glRasterPos3f(x, y, z);

    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(big_font? GLUT_BITMAP_HELVETICA_18 : GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void display_init() {
    // Set matrix to projection mode
    glMatrixMode(GL_PROJECTION);
    // Load identity matrix into our matrix
    glLoadIdentity();
    // Set max and min limits of x,y and z axis 
    glOrtho(-50, 50, -50, 50, -50, 50);
    // Change the matrix back to modelview mode
    glMatrixMode(GL_MODELVIEW);
    // Set background color
    glClearColor(0, 0, 0, 0);
}

/**
*  This function will draw a Square in the 3D space with the given length of side,
*  the (x,y,z) cordinates of its 4 vertices and a color to fill.
*/
void drawQuad(GLint size, Point3D points[4], ColorRGB color) {
    int i;
    // Set polygon mode to fill it front and back
    glPolygonMode(GL_FRONT_AND_BACK, frame ? GL_LINE: GL_FILL);
    // Set the given color
    glColor4f(color.R, color.G, color.B, 1);
    // Start giving vertices of the Square
    glBegin(GL_QUADS);
    for (i = 0; i < 4; i++)
        glVertex3i(size*points[i].x, size*points[i].y, size*points[i].z);
    glEnd();
}

/**
*  This function will draw a cube using 6 Squares in the 3D space
*/
void drawCube(int size) {
    int i;
    for (i=0; i< 6; i++) {
        drawQuad(size, vertices[i], colors[i]);
    }
}

void display() {
    // Clear color and depth buffer bits
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Push a new matrix in the stack copy the values from the current one
    glPushMatrix();
    // Do all the rotations for current values of angles with x,y and z axes 
    glRotatef(aX, 1, 0, 0);
    glRotatef(aZ, 0, 0, 1);
    glRotatef(tY, 0, 1, 0);
    // Now draw cube over the rotated space
    drawCube(30);
    // Copy the values of the new matrix to current matrix and pop off the new one
    glPopMatrix();

    // Set the color for the title and draw the title string
    glColor4f(0, 0.650, 1, 1);
    drawString(-16.0, 45.0, 0.0, TITLE, 1);
    // Set the color for the info message string and draw 
    glColor4f(0.470, 0.564, 0.611, 1);
    drawString(-47.0, -47.0, 0.0, MSG, 0);

    // Flush and Swap matrix buffers to the screen
    glFlush();
    glutSwapBuffers();

    // Update the value of y-axis rotation if rotation isn't stopped
    if (!stop) {
        tY += aY;
        if (tY >= 360) {
            tY = 0;
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // map 'W' to increment x-axis rotation angle
        case 'w':
        case 'W': 
                    aX += ROTATION_SPEED;
                    if (aX >= 359) {
                        aX = 0;
                    }
        break;
        // map 'S' to decrement x-axis rotation angle
        case 's':
        case 'S': 
                    aX -= ROTATION_SPEED;
                    if (aX <= 0) {
                        aX = 359;
                    }
        break;
        // map 'A' to increment z-axis rotation angle
        case 'a':
        case 'A': 
                    aZ += ROTATION_SPEED;
                    if (aZ >= 359) {
                        aZ = 0;
                    }
        break;
        // map 'D' to decrement z-axis rotation angle
        case 'd':
        case 'D': 
                    aZ -= ROTATION_SPEED;
                    if (aZ <= 0) {
                        aZ = 359;
                    }
        break;
        // map 'F' to increment y-axis rotation speed
        case 'f':
        case 'F':
                    aY += 1;
                    if (aY >= 360) {
                        aY = 0;
                    }
        break;
        // map 'R' to toggle frame draw switch
        case 'r':
        case 'R':
                    frame ^= 1;
        break;
        // map 'SPACE' to toggle stop y-axis rotation switch
        case 0x20:
                    stop ^= 1;
        break;
        // map 'Q' to exit
        case 0x1B:
        case 'q':
        case 'Q': exit(0);
        break;
    }
}
