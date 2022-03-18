#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <windows.h>

// Define a constant for the value of PI
#define PI 3.1415f

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

float angle;
float r1 = 7.5f; // Diameter of the cylinder
float r2 = 15.0f;
float r3 = 5.0f;
float step = (PI / 100.0f);
float normal[3];
float corners[3][3];
float ver[220][3];
float tmp[2][3];


static float mouseX = 0.0f;
static float mouseY = 0.0f;
static bool mouseLeftDown = false;
static float cameraAngleX = 0.0f;
static float cameraAngleY = 0.0f;
static int opt = 0;



void ProcessMenu(int value)
{
    switch (value) {
    case 1:
        opt = 1;
        break;
    case 2:
        opt = 2;
        break;
    case 3:
        opt = 3;
        break;
    case 4:
        opt = 0;
        break;
    }
    glutPostRedisplay();
}






// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
    float length;

    // Calculate the length of the vector
    length = (float)sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]));

    // Keep the program from blowing up by providing an exceptable
    // value for vectors that may calculated too close to zero.
    if (length == 0.0f)
        length = 1.0f;

    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
}

// 背景色数组
GLclampf color[][4][3] = {
    {
        {1.0, 1.0, 1.0},
        {1.0, 0.0, 1.0},
        {0.0, 1.0, 1.0},
        {1.0, 1.0, 0.0}
    },
    {
        {0.0, 1.0, 1.0},
        {1.0, 0.0, 1.0},
        {0.5, 0.3, 1.0},
        {1.0, 1.0, 0.0}
    },
    {
        {1.0, 1.0, 1.0},
        {1.0, 0.2, 0.7},
        {0.0, 0.0, 0.4},
        {1.0, 0.5, 0.0}
    },
    {
        {0.5, 0.2, 1.0},
        {0.4, 0.0, 1.0},
        {0.2, 0.8, 0.5},
        {0.73, 0.4, 0.2}
    }
};


void background1(GLclampf c[][4][3])
{
    glBegin(GL_POLYGON);
    glColor3f(c[opt][0][0], c[opt][0][1], c[opt][0][2]);
    glVertex3f(-500.0, -500.0, -400.0);

    glColor3f(c[opt][1][0], c[opt][1][1], c[opt][1][2]);
    glVertex3f(-500.0, 500.0, -400.0);

    glColor3f(c[opt][2][0], c[opt][2][1], c[opt][2][2]);
    glVertex3f(500.0, 500.0, -400.0);

    glColor3f(c[opt][3][0], c[opt][3][1], c[opt][3][2]);
    glVertex3f(500.0, -500.0, -400.0);

    glEnd();
}














void Display(void)
{
    // glDisable(GL_DEPTH_TEST); // Hidden surface removal
    glClear(GL_COLOR_BUFFER_BIT);

    background1(color);

    glFlush();
     // Hidden surface removal
}



void RenderCenterCube(void)
{
    float backPos[][3] = {
        {60.0f, 100.0f, 0.0f},
        {140.0f, 100.0f, 0.0f},
        {60.0f, 100.0f, 15.0f},
        {140.0f, 100.0f, 15.0f},
        {60.0f, 50.0f, 0.0f},
        {140.0f, 50.0f, 0.0f},
        {60.0f, 50.0f, 15.0f},
        {140.0f, 50.0f, 15.0f}
    };


    glColor3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS); //  -> Y
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(backPos[0]);
    glVertex3fv(backPos[2]);
    glVertex3fv(backPos[3]);
    glVertex3fv(backPos[1]);
    glEnd();

    glBegin(GL_QUADS); //  -> Z
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(backPos[2]);
    glVertex3fv(backPos[6]);
    glVertex3fv(backPos[7]);
    glVertex3fv(backPos[3]);
    glEnd();

    glBegin(GL_QUADS); //  -> -Y
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(backPos[6]);
    glVertex3fv(backPos[4]);
    glVertex3fv(backPos[5]);
    glVertex3fv(backPos[7]);
    glEnd();

    glBegin(GL_QUADS); //  -> -Z
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3fv(backPos[1]);
    glVertex3fv(backPos[5]);
    glVertex3fv(backPos[4]);
    glVertex3fv(backPos[0]);
    glEnd();

    glBegin(GL_QUADS); //  -> X
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(backPos[3]);
    glVertex3fv(backPos[7]);
    glVertex3fv(backPos[5]);
    glVertex3fv(backPos[1]);
    glEnd();

    glBegin(GL_QUADS); //  -> -X
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(backPos[0]);
    glVertex3fv(backPos[4]);
    glVertex3fv(backPos[6]);
    glVertex3fv(backPos[2]);
    glEnd();
}

void RenderConcave(void)
{
    int i = 0;
    glColor3f(0.0f, 0.32f, 0.64f); //凹面颜色
    // 凹面半圆采样
    for (angle = (1.0f * PI); angle <= (2.0f * PI); angle += step) {
        ver[i][0] = r3 * (float)cos(angle) + 100;
        ver[i][1] = 50.0f;
        ver[i][2] = r3 * (float)sin(angle) + 40;
        i++;
    }

    glBegin(GL_QUADS);
    for (int j = 0; j <= 99; j++) {
        tmp[0][0] = ver[j][0];
        tmp[0][1] = 0.0f;
        tmp[0][2] = ver[j][2];

        tmp[1][0] = ver[j + 1][0];
        tmp[1][1] = 0.0f;
        tmp[1][2] = ver[j + 1][2];

        normal[0] = ver[j][0]-100;
        normal[1] = 0.0f;
        normal[2] = ver[j][2]-40;

        ReduceToUnit(normal);
        glNormal3fv(normal);

        glVertex3fv(ver[j]);
        glVertex3fv(tmp[0]);
        glVertex3fv(tmp[1]);
        glVertex3fv(ver[j + 1]);
    }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS); // top panel
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int j = 0; j < 99; j++) {
        tmp[0][0] = ver[j][0];
        tmp[0][1] = 50.0f;
        tmp[0][2] = 0.0f;

        tmp[1][0] = ver[j + 1][0];
        tmp[1][1] = 50.0f;
        tmp[1][2] = 0.0f;

        glVertex3fv(ver[j]);
        glVertex3fv(tmp[0]);
        glVertex3fv(tmp[1]);
        glVertex3fv(ver[j + 1]);
    }

    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS); // bottom panel
    glNormal3f(0.0f, -1.0f, 0.0f);

    for (int j = 0; j < 100; j++)
        ver[j][1] = 0.0f;

    for (int j = 0; j < 99; j++) {
        tmp[0][0] = ver[j][0];
        tmp[0][1] = 0.0f;
        tmp[0][2] = 0.0f;

        tmp[1][0] = ver[j + 1][0];
        tmp[1][1] = 0.0f;
        tmp[1][2] = 0.0f;

        glVertex3fv(ver[j]);
        glVertex3fv(tmp[0]);
        glVertex3fv(tmp[1]);
        glVertex3fv(ver[j + 1]);
    }

    glEnd();

    glBegin(GL_QUADS); // left panel
    glVertex3f(95.0f, 50.0f, 0.0f);
    glVertex3f(95.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 0.0f, 40.0f);
    glVertex3f(95.0f, 50.0f, 40.0f);
    glEnd();

    glBegin(GL_QUADS); // right panel
    glVertex3f(105.0f, 50.0f, 0.0f);
    glVertex3f(105.0f, 0.0f, 0.0f);
    glVertex3f(105.0f, 0.0f, 40.0f);
    glVertex3f(105.0f, 50.0f, 40.0f);
    glEnd();

    glBegin(GL_QUADS); // back panel
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(95.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 50.0f, 0.0f);
    glVertex3f(105.0f, 50.0f, 0.0f);
    glVertex3f(105.0f, 0.0f, 0.0f);
    glEnd();
}

void RenderSquareHole(void)
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    // Left Panel
    glVertex3f(0.0f, 50.0f, 65.0f);
    glVertex3f(0.0f, 0.0f, 65.0f);
    glVertex3f(15.0f, 0.0f, 65.0f);
    glVertex3f(15.0f, 50.0f, 65.0f);

    // Right Panel
    glVertex3f(45.0f, 50.0f, 65.0f);
    glVertex3f(45.0f, 0.0f, 65.0f);
    glVertex3f(95.0f, 0.0f, 65.0f);
    glVertex3f(95.0f, 50.0f, 65.0f);

    // Top Panel
    glVertex3f(15.0f, 50.0f, 65.0f);
    glVertex3f(15.0f, 40.0f, 65.0f); //
    glVertex3f(45.0f, 40.0f, 65.0f); //
    glVertex3f(45.0f, 50.0f, 65.0f);

    // Bottom Panel
    glVertex3f(15.0f, 10.0f, 65.0f); //
    glVertex3f(15.0f, 0.0f, 65.0f);
    glVertex3f(45.0f, 0.0f, 65.0f);
    glVertex3f(45.0f, 10.0f, 65.0f); //
    glEnd();

    // 填充四角内小三角形
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(45.0f, 40.0f, 65.0f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(15.0f, 40.0f, 65.0f);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(15.0f, 10.0f, 65.0f);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(45.0f, 10.0f, 65.0f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    // side ////////////////////////////
    // top side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 65.0f);
    glVertex3f(95.0f, 50.0f, 65.0f);
    glVertex3f(95.0f, 50.0f, 0.0f);

    // Bottom section
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 65.0f);
    glVertex3f(95.0f, 0.0f, 65.0f);
    glVertex3f(95.0f, 0.0f, 0.0f);

    // Left section
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 65.0f);
    glVertex3f(0.0f, 50.0f, 65.0f);

    // right side1 凹面
    glColor3f(0.0f, 0.32f, 0.64f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 50.0f, 65.0f);
    glVertex3f(95.0f, 0.0f, 65.0f);
    glVertex3f(95.0f, 0.0f, 40.0f);
    glVertex3f(95.0f, 50.0f, 40.0f);

    // right side2
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 50.0f, 40.0f);
    glVertex3f(95.0f, 0.0f, 40.0f);
    glVertex3f(95.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 50.0f, 0.0f);

    glEnd();

    glFrontFace(GL_CW); // clock-wise polygons face out
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    // Back section
    glNormal3f(0.0f, 0.0f, -1.0f); //设置当前的法线向量

    // right Panel
    glVertex3f(0.0f, 50.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(15.0f, 0.0f, 0.0f);
    glVertex3f(15.0f, 50.0f, 0.0f);

    // left Panel
    glVertex3f(45.0f, 50.0f, 0.0f);
    glVertex3f(45.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 0.0f, 0.0f);
    glVertex3f(95.0f, 50.0f, 0.0f);

    // Top Panel
    glVertex3f(15.0f, 50.0f, 0.0f);
    glVertex3f(15.0f, 40.0f, 0.0f); //
    glVertex3f(45.0f, 40.0f, 0.0f); //
    glVertex3f(45.0f, 50.0f, 0.0f);

    // Bottom Panel
    glVertex3f(15.0f, 10.0f, 0.0f); //
    glVertex3f(15.0f, 0.0f, 0.0f);
    glVertex3f(45.0f, 0.0f, 0.0f);
    glVertex3f(45.0f, 10.0f, 0.0f); //
    glEnd();

    // 填充四角内小三角形
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(45.0f, 40.0f, 0.0f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(15.0f, 40.0f, 0.0f);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(15.0f, 10.0f, 0.0f);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 0.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(45.0f, 10.0f, 0.0f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 30, r2 * (float)sin(angle) + 25, 0.0f);
    glEnd();

    glFrontFace(GL_CCW); // Counter clock-wise polygons face out

    int i = 0;
    glColor3f(1.0f, 0.0f, 0.0f);
    for (angle = (2.2f * PI); angle > 0.0f; angle -= step) {
        ver[i][0] = r2 * (float)cos(angle) + 30;
        ver[i][1] = r2 * (float)sin(angle) + 25;
        ver[i][2] = 0.0f;
        i++;
    }

    glBegin(GL_QUADS);
    for (int j = 0; j < 218; j++) {
        tmp[0][0] = ver[j][0];
        tmp[0][1] = ver[j][1];
        tmp[0][2] = 65.0f;

        tmp[1][0] = ver[j + 1][0];
        tmp[1][1] = ver[j + 1][1];
        tmp[1][2] = 65.0f;

        normal[0] = ver[j][0];
        normal[1] = ver[j][2];

        normal[2] = 0.0f;
        ReduceToUnit(normal);
        glNormal3fv(normal);

        glVertex3fv(ver[j]);
        glVertex3fv(tmp[0]);
        glVertex3fv(tmp[1]);
        glVertex3fv(ver[j + 1]);
    }
    glEnd();
}

void RenderSquareHoleR(void)
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    // right Panel
    glVertex3f(185.0f, 50.0f, 65.0f);
    glVertex3f(185.0f, 0.0f, 65.0f);
    glVertex3f(200.0f, 0.0f, 65.0f);
    glVertex3f(200.0f, 50.0f, 65.0f);

    // left Panel
    glVertex3f(105.0f, 50.0f, 65.0f);
    glVertex3f(105.0f, 0.0f, 65.0f);
    glVertex3f(155.0f, 0.0f, 65.0f);
    glVertex3f(155.0f, 50.0f, 65.0f);

    // Top Panel
    glVertex3f(155.0f, 50.0f, 65.0f);
    glVertex3f(155.0f, 40.0f, 65.0f); //
    glVertex3f(185.0f, 40.0f, 65.0f); //
    glVertex3f(185.0f, 50.0f, 65.0f);

    // Bottom Panel
    glVertex3f(155.0f, 10.0f, 65.0f); //
    glVertex3f(155.0f, 0.0f, 65.0f);
    glVertex3f(185.0f, 0.0f, 65.0f);
    glVertex3f(185.0f, 10.0f, 65.0f); //
    glEnd();

    // 填充四角内小三角形
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(185.0f, 40.0f, 65.0f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(155.0f, 40.0f, 65.0f);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(155.0f, 10.0f, 65.0f);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(185.0f, 10.0f, 65.0f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step)
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 65.0f);
    glEnd();

    // side ////////////////////////////
    // up side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(105.0f, 50.0f, 0.0f);
    glVertex3f(105.0f, 50.0f, 65.0f);
    glVertex3f(200.0f, 50.0f, 65.0f);
    glVertex3f(200.0f, 50.0f, 0.0f);

    // Bottom section
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(105.0f, 0.0f, 0.0f);
    glVertex3f(105.0f, 0.0f, 65.0f);
    glVertex3f(200.0f, 0.0f, 65.0f);
    glVertex3f(200.0f, 0.0f, 0.0f);

    // right section
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 65.0f);
    glVertex3f(200.0f, 0.0f, 65.0f);
    glVertex3f(200.0f, 0.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 0.0f);

    // left side1 凹面
    glColor3f(0.0f, 0.32f, 0.64f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(105.0f, 50.0f, 65.0f);
    glVertex3f(105.0f, 0.0f, 65.0f);
    glVertex3f(105.0f, 0.0f, 40.0f);
    glVertex3f(105.0f, 50.0f, 40.0f);

    // left side2
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(105.0f, 50.0f, 40.0f);
    glVertex3f(105.0f, 0.0f, 40.0f);
    glVertex3f(105.0f, 0.0f, 0.0f);
    glVertex3f(105.0f, 50.0f, 0.0f);

    glEnd();
    glFrontFace(GL_CW); // clock-wise polygons face out

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    // Back section
    glNormal3f(0.0f, 0.0f, -1.0f); //设置当前的法线向量

    // right Panel
    glVertex3f(185.0f, 50.0f, 0.0f);
    glVertex3f(185.0f, 0.0f, 0.0f);
    glVertex3f(200.0f, 0.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 0.0f);

    // left Panel
    glVertex3f(105.0f, 50.0f, 0.0f);
    glVertex3f(105.0f, 0.0f, 0.0f);
    glVertex3f(155.0f, 0.0f, 0.0f);
    glVertex3f(155.0f, 50.0f, 0.0f);

    // Top Panel
    glVertex3f(155.0f, 50.0f, 0.0f);
    glVertex3f(155.0f, 40.0f, 0.0f); //
    glVertex3f(185.0f, 40.0f, 0.0f); //
    glVertex3f(185.0f, 50.0f, 0.0f);

    // Bottom Panel
    glVertex3f(155.0f, 10.0f, 0.0f); //
    glVertex3f(155.0f, 0.0f, 0.0f);
    glVertex3f(185.0f, 0.0f, 0.0f);
    glVertex3f(185.0f, 10.0f, 0.0f); //
    glEnd();

    // 填充四角内小三角形
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(185.0f, 40.0f, 0.0f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step) {
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 0.0f);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(155.0f, 40.0f, 0.0f);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step) {
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 0.0f);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(155.0f, 10.0f, 0.0f);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step) {
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 0.0f);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(185.0f, 10.0f, 0.0f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step) {
        glVertex3f(r2 * (float)cos(angle) + 170, r2 * (float)sin(angle) + 25, 0.0f);
    }
    glEnd();

    glFrontFace(GL_CCW); // Counter clock-wise polygons face out

    int i = 0;
    glColor3f(1.0f, 0.0f, 0.0f);
    for (angle = (2.2f * PI); angle > 0.0f; angle -= step) {
        ver[i][0] = r2 * (float)cos(angle) + 170;
        ver[i][1] = r2 * (float)sin(angle) + 25;
        ver[i][2] = 0.0f;
        i++;
    }

    glBegin(GL_QUADS);
    for (int j = 0; j < 218; j++) {

        tmp[0][0] = ver[j][0];
        tmp[0][1] = ver[j][1];
        tmp[0][2] = 65.0f;

        tmp[1][0] = ver[j + 1][0];
        tmp[1][1] = ver[j + 1][1];
        tmp[1][2] = 65.0f;

        // normal[0] = ver[j][0];
        // normal[1] = ver[j][2];

        // normal[2] = 0.0f;
        // ReduceToUnit(normal);
        // glNormal3fv(normal);

        glVertex3fv(ver[j]);
        glVertex3fv(ver[j + 1]);
        glVertex3fv(tmp[1]);
        glVertex3fv(tmp[0]);
    }
    glEnd();

    // glColor3f(0.0f, 0.0f, 0.0f);
    // glBegin(GL_QUADS);
    // for (int j = 0; j < 218; j++)
    // {

    //     tmp[0][0] = ver[j][0];
    //     tmp[0][1] = -ver[j][1];
    //     tmp[0][2] = ver[j][2];

    //     tmp[1][0] = ver[j+1][0];
    //     tmp[1][1] = -ver[j+1][1];
    //     tmp[1][2] = ver[j+1][2];

    //     normal[0] = ver[j][0];
    //     normal[1] = 0.0f;
    //     normal[2] = ver[j][2];

    //     ReduceToUnit(normal);
    //     // glNormal3fv(normal);

    //     glVertex3fv(ver[j]);
    //     glVertex3fv(ver[j+1]);
    //     glVertex3fv(tmp[1]);
    //     glVertex3fv(tmp[0]);
    // }
    // glEnd();
}

void RenderTriWithHoleL(void)
{
    float r1 = 7.5f; // Diameter of the cylinder
    float normal[3], corners[3][3]; // Storeage for vertex calculations
    float step = (PI / 100.0f); // Approximate the cylinder wall with
    float ver[220][3];
    float ver1[220][3];

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS); // 前面
    // Pointing straight out Z
    glNormal3f(0.0f, 0.35f, 0.5f);

    // Left Panel
    glVertex3f(0.0f, 100.0f, 15.0f);
    glVertex3f(0.0f, 50.0f, 65.0f);
    glVertex3f(22.5f, 50.0f, 65.0f);
    glVertex3f(22.5f, 100.0f, 15.0f);

    // Right Panel
    glVertex3f(37.5f, 100.0f, 15.0f);
    glVertex3f(37.5f, 50.0f, 65.0f);
    glVertex3f(60.0f, 50.0f, 65.0f);
    glVertex3f(60.0f, 100.0f, 15.0f);

    // Top Panel
    glVertex3f(22.5f, 100.0f, 15.0f);
    glVertex3f(22.5f, 72.5f, 42.5f); //左上
    glVertex3f(37.5f, 72.5f, 42.5f); //右上
    glVertex3f(37.5f, 100.0f, 15.0f);

    // Bottom Panel
    glVertex3f(22.5f, 57.5f, 57.5); //左下
    glVertex3f(22.5f, 50.0f, 65.0f);
    glVertex3f(37.5f, 50.0f, 65.0f);
    glVertex3f(37.5f, 57.5f, 57.5); //右下
    glEnd();

    // 前面的   填充四角内小三角形
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(37.5f, 72.5f, 42.5f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(37.5f, 57.5f, 57.5);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(22.5f, 72.5f, 42.5f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(22.5f, 57.5f, 57.5);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    //  // Top length section ////////////////////////////
    // // Normal points up Y axis
    glBegin(GL_QUADS); // 四个侧面
    glNormal3f(0.0f, 1.0f, 0.0f); // 上面
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 15.0f);
    glVertex3f(60.0f, 100.0f, 15.0f);
    glVertex3f(60.0f, 100.0f, 0.0f);

    // Bottom section
    glNormal3f(0.0f, -1.0f, 0.0f); // down face
    glVertex3f(0.0f, 50.0f, 0.0f);
    glVertex3f(60.0f, 50.0f, 0.0f);
    glVertex3f(60.0f, 50.0f, 65.0f);
    glVertex3f(0.0f, 50.0f, 65.0f);

    // Left section
    glNormal3f(-1.0f, 0.0f, 0.0f); //
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 65.0f);
    glVertex3f(0.0f, 100.0f, 15.0f);

    // Right Section
    glNormal3f(1.0f, 0.0f, 0.0f); //
    glVertex3f(60.0f, 100.0f, 0.0f);
    glVertex3f(60.0f, 50.0f, 0.0f);
    glVertex3f(60.0f, 50.0f, 65.0f);
    glVertex3f(60.0f, 100.0f, 15.0f);

    glEnd();

    glFrontFace(GL_CW); // clock-wise polygons face out
    // glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS); // 后面
    // // Back section
    // // Pointing straight out Z
    glNormal3f(0.0f, 0.0f, -1.0f); //设置当前的法线向量

    // // Left Panel
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(22.5f, 100.0f, 0.0f);
    glVertex3f(22.5f, 50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);

    // // Right Panel
    glVertex3f(37.5f, 100.0f, 0.0f);
    glVertex3f(60.0f, 100.0f, 0.0f);
    glVertex3f(60.0f, 50.0f, 0.0f);
    glVertex3f(37.5f, 50.0f, 0.0f);

    // // Top Panel
    glVertex3f(22.5f, 100.0f, 0.0f);
    glVertex3f(37.5f, 100.0f, 0.0f);
    glVertex3f(37.5f, 72.5f, 0.0f); //
    glVertex3f(22.5f, 72.5f, 0.0f); //

    // // Bottom Panel
    glVertex3f(22.5f, 57.5f, 0.0f); //
    glVertex3f(37.5f, 57.5f, 0.0f); //
    glVertex3f(37.5f, 50.0f, 0.0f);
    glVertex3f(22.5f, 50.0f, 0.0f);

    glEnd();

    //  // 后面的 填充四角内小三角形
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    // glNormal3f(0.0f, -0.2f, 0.0f);//设置当前的法线向量
    glVertex3f(37.5f, 72.5f, 0.0f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(37.5f, 57.5f, 0.0f);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(22.5f, 57.5f, 0.0f);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(22.5f, 72.5f, 0.0f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 30, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glFrontFace(GL_CCW); // Counter clock-wise polygons face out

    int i = 0; //背景圆采点
    for (angle = (2.2f * PI); angle > 0.0f; angle -= step) {
        ver[i][0] = r1 * (float)sin(angle) + 30;
        ver[i][1] = r1 * (float)cos(angle) + 65;
        ver[i][2] = 0.0f;
        i++;
    }

    //前景圆采点
    for (angle = (2.2f * PI), i = 0; angle > 0.0f; angle -= step) {
        ver1[i][0] = r1 * (float)sin(angle) + 30;
        ver1[i][1] = r1 * (float)cos(angle) + 65;
        ver1[i][2] = -r1 * (float)cos(angle) + 50;
        i++;
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (int j = 0; j < 218; j++) {
        // normal[0] = ver[j][0];
        // normal[1] = ver[j][2];
        // normal[2] = 0.0f;
        // ReduceToUnit(normal);
        // glNormal3fv(normal);
        glVertex3fv(ver[j]);
        glVertex3fv(ver1[j]);
        glVertex3fv(ver1[j + 1]);
        glVertex3fv(ver[j + 1]);
    }

    glEnd();
}

void RenderTriWithHoleR(void)
{
    float normal[3], corners[3][3]; // Storeage for vertex calculations
    float ver1[220][3];

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS); // 前面
    // Pointing straight out Z
    glNormal3f(0.0f, 0.35f, 0.5f);

    // Left Panel
    glVertex3f(140.0f, 100.0f, 15.0f);
    glVertex3f(140.0f, 50.0f, 65.0f);
    glVertex3f(162.5f, 50.0f, 65.0f);
    glVertex3f(162.5f, 100.0f, 15.0f);

    // Right Panel
    glVertex3f(177.5f, 100.0f, 15.0f);
    glVertex3f(177.5f, 50.0f, 65.0f);
    glVertex3f(200.0f, 50.0f, 65.0f);
    glVertex3f(200.0f, 100.0f, 15.0f);

    // Top Panel
    glVertex3f(162.5f, 100.0f, 15.0f);
    glVertex3f(162.5f, 72.5f, 42.5f); //左上
    glVertex3f(177.5f, 72.5f, 42.5f); //右上
    glVertex3f(177.5f, 100.0f, 15.0f);

    // Bottom Panel
    glVertex3f(162.5f, 57.5f, 57.5); //左下
    glVertex3f(162.5f, 50.0f, 65.0f);
    glVertex3f(177.5f, 50.0f, 65.0f);
    glVertex3f(177.5f, 57.5f, 57.5); //右下
    glEnd();

    // 前面的   填充四角内小三角形
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(177.5f, 72.5f, 42.5f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(177.5f, 57.5f, 57.5);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(162.5f, 72.5f, 42.5f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(162.5f, 57.5f, 57.5);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, -r1 * (float)cos(angle) + 50);
    }
    glEnd();

    //  // Top length section ////////////////////////////
    // // Normal points up Y axis
    glBegin(GL_QUADS); // 四个侧面
    glNormal3f(0.0f, 1.0f, 0.0f); // 上面
    glVertex3f(140.0f, 100.0f, 0.0f);
    glVertex3f(140.0f, 100.0f, 15.0f);
    glVertex3f(200.0f, 100.0f, 15.0f);
    glVertex3f(200.0f, 100.0f, 0.0f);

    // Bottom section
    glNormal3f(0.0f, -1.0f, 0.0f); // down face
    glVertex3f(140.0f, 50.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 65.0f);
    glVertex3f(140.0f, 50.0f, 65.0f);

    // Left section
    glNormal3f(-1.0f, 0.0f, 0.0f); //
    glVertex3f(140.0f, 100.0f, 0.0f);
    glVertex3f(140.0f, 50.0f, 0.0f);
    glVertex3f(140.0f, 50.0f, 65.0f);
    glVertex3f(140.0f, 100.0f, 15.0f);

    // Right Section
    glNormal3f(1.0f, 0.0f, 0.0f); //
    glVertex3f(200.0f, 100.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 65.0f);
    glVertex3f(200.0f, 100.0f, 15.0f);

    glEnd();

    glFrontFace(GL_CW); // clock-wise polygons face out
    // glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS); // 后面
    // // Back section
    // // Pointing straight out Z
    glNormal3f(0.0f, 0.0f, -1.0f); //设置当前的法线向量

    // // Left Panel
    glVertex3f(140.0f, 100.0f, 0.0f);
    glVertex3f(162.5f, 100.0f, 0.0f);
    glVertex3f(162.5f, 50.0f, 0.0f);
    glVertex3f(140.0f, 50.0f, 0.0f);

    // // Right Panel
    glVertex3f(177.5f, 100.0f, 0.0f);
    glVertex3f(200.0f, 100.0f, 0.0f);
    glVertex3f(200.0f, 50.0f, 0.0f);
    glVertex3f(177.5f, 50.0f, 0.0f);

    // // Top Panel
    glVertex3f(162.5f, 100.0f, 0.0f);
    glVertex3f(177.5f, 100.0f, 0.0f);
    glVertex3f(177.5f, 72.5f, 0.0f); //
    glVertex3f(162.5f, 72.5f, 0.0f); //

    // // Bottom Panel
    glVertex3f(162.5f, 57.5f, 0.0f); //
    glVertex3f(177.5f, 57.5f, 0.0f); //
    glVertex3f(177.5f, 50.0f, 0.0f);
    glVertex3f(162.5f, 50.0f, 0.0f);

    glEnd();

    //  // 后面的 填充四角内小三角形
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    // glNormal3f(0.0f, -0.2f, 0.0f);//设置当前的法线向量
    glVertex3f(177.5f, 72.5f, 0.0f);
    for (angle = 0.0f; angle < (0.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(177.5f, 57.5f, 0.0f);
    for (angle = (0.5f * PI); angle < (1.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(162.5f, 57.5f, 0.0f);
    for (angle = (1.0f * PI); angle < (1.5f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(162.5f, 72.5f, 0.0f);
    for (angle = (1.5f * PI); angle < (2.0f * PI); angle += step) {
        glVertex3f(r1 * (float)sin(angle) + 170, r1 * (float)cos(angle) + 65, 0);
    }
    glEnd();

    glFrontFace(GL_CCW); // Counter clock-wise polygons face out

    int i = 0; //背景圆采点
    for (angle = (2.2f * PI); angle > 0.0f; angle -= step) {
        ver[i][0] = r1 * (float)sin(angle) + 170;
        ver[i][1] = r1 * (float)cos(angle) + 65;
        ver[i][2] = 0.0f;
        i++;
    }

    //前景圆采点
    for (angle = (2.2f * PI), i = 0; angle > 0.0f; angle -= step) {
        ver1[i][0] = r1 * (float)sin(angle) + 170;
        ver1[i][1] = r1 * (float)cos(angle) + 65;
        ver1[i][2] = -r1 * (float)cos(angle) + 50;
        i++;
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (int j = 0; j < 218; j++) {
        // normal[0] = ver[j][0];
        // normal[1] = ver[j][2];
        // normal[2] = 0.0f;
        // ReduceToUnit(normal);
        // glNormal3fv(normal);
        glVertex3fv(ver[j]);
        glVertex3fv(ver1[j]);
        glVertex3fv(ver1[j + 1]);
        glVertex3fv(ver[j + 1]);
    }

    glEnd();
}


// 鼠标按下事件
void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
            mouseLeftDown = true;
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }
}  
// 记录鼠标位置变化
void mouseMotionCB(int x, int y)
{
    if (mouseLeftDown)
    {
        cameraAngleY += (x - mouseX) * 0.3f;
        cameraAngleX += (y - mouseY) * 0.3f;
        mouseX = x;
        mouseY = y;
        // std::cout << "mouseX: " << cameraAngleY << std::endl;
    }

    glutPostRedisplay();
}


// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    // glScalef(3.0, 3.0, 3.0);glTranslatef(-70, -100, 0);
    // Save the matrix state
    Display();
    glMatrixMode(GL_MODELVIEW);
    // gluLookAt(-0.3,0.3,0.4, 0.0,0.0,0.0,  0.0,1.0,0.0);
    glPushMatrix();

    // Rotate about x and y axes
    glRotatef(xRot + cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot + cameraAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);

    // Render just the shaft of the bold


    RenderSquareHole();
    RenderSquareHoleR();
    RenderCenterCube();
    RenderConcave();
    RenderTriWithHoleL();
    RenderTriWithHoleR();
    glPopMatrix();
    
    // Swap buffers
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    // Light values and coordinates
    GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat specular[] = { 0.9f, 0.3f, 0.7f, 1.0f };
    GLfloat lightPos[] = {120.0f, 200.0f, 20.0f, 1.0f };
    GLfloat specref[] = {0.6f, 0.6f, 0.6f, 1.0f };

    glEnable(GL_DEPTH_TEST); // Hidden surface removal
    glDepthFunc(GL_LESS); // 深度测试类型
    //
    glCullFace(GL_BACK); // 剔除背面
    // glEnable(GL_CULL_FACE); // Do not calculate inside of solid object
    glFrontFace(GL_CCW);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    // Position and turn on the light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a moderate shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 64);

    

    // Black background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

///////////////////////////////////////////////////////////////////////////////
// Process arrow keys
void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        xRot -= 3.0f;

    if (key == GLUT_KEY_DOWN)
        xRot += 3.0f;

    if (key == GLUT_KEY_LEFT)
        yRot -= 3.0f;

    if (key == GLUT_KEY_RIGHT)
        yRot += 3.0f;

    if (key == GLUT_KEY_F5)
        zRot -= 3.0f;


    xRot = (GLfloat)((const int)xRot % 360);
    yRot = (GLfloat)((const int)yRot % 360);
    zRot = (GLfloat)((const int)zRot % 360);
    // Refresh the Window
    glutPostRedisplay();
}

void SpaceKey(unsigned char key, int x, int y)
{
    if (key == 32)
        zRot += 3.0f;
    
    zRot = (GLfloat)((const int)zRot % 360);
    glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
    GLfloat nRange = 230.0f;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h)
        glOrtho(-nRange, nRange, -nRange * h / w, nRange * h / w, -nRange * 2.0f, nRange * 2.0f);
    else
        glOrtho(-nRange * w / h, nRange * w / h, -nRange, nRange, -nRange * 2.0f, nRange * 2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(870, 650);
    glutInitWindowPosition(500, 300);
    glutCreateWindow("task 2");

    // menu
    int colorOption, mainMenu;
    colorOption = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("color 1", 1);
    glutAddMenuEntry("color 2", 2);
    glutAddMenuEntry("color 3", 3);
    glutAddMenuEntry("color 4", 4);

    mainMenu = glutCreateMenu(ProcessMenu);
    glutAddSubMenu("background color", colorOption);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(SpaceKey);

    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    SetupRC();
    glutMainLoop();

    return 0;
}
