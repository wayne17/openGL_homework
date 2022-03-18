#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>
#define GL_PI 3.1415f

static int transformation = 3; // (1, 2, 3) -> (平移， 旋转， 缩放)
static int v = 1; // (1, 3, 6) -> (慢， 中， 快)


static int flag = 1; // 控制变化方向
static int ind = 0x1B02;    // 控制线框类型
static float xmove = 0.0;   // 控制平移距离
static float rate = 1.0;    // 控制缩放
static int ang = 360;     // 控制旋转角度
static int c = 0;     // 控制旋转角度


GLclampf color[][3] = {
    {0.785, 0.703, 0.792},
    {0.531, 0.672, 0.8554},
    {0.0078, 0.664, 0.551},
    {0.12109, 0.55078, 0.86718}
};

// 菜单事件处理
void ProcessMenu(int value)
{
    switch (value) {
    case 1:
        ind = 0x1B02;
        break;
    case 2:
        ind = 0x1B01;
        break;
    case 3:
        v = 1;
        break;
    case 4:
        v = 3;
        break;
    case 5:
        v = 6;
        break;
    case 6:
        transformation = 1;
        break;
    case 7:
        transformation = 2;
        break;
    case 8:
        transformation = 3;
        break;
    case 9:
        c = 1;
        break;
    case 10:
        c = 2;
        break;
    case 11:
        c = 3;
        break;
    case 12:
        c = 0;
        break;
    default:
        v = 3;
        transformation = 1;
        break;
    }
    glutPostRedisplay();
}

void Background(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color[c][0], color[c][1], color[c][2], 1.0f);
    glPointSize(1.0);

    glBegin(GL_POINTS);
    glColor3f(0.2, 0.26, 0.15);
    glVertex2f(0, 0);
    glEnd();

}

void RenderScene(void)
{
    // glClear(GL_COLOR_BUFFER_BIT);
    Background();

    glPolygonMode(GL_FRONT_AND_BACK, ind);

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0, 0);
    glVertex2f(-0.3, -0.2);
    glVertex2f(0.3, -0.2);
    glVertex2f(0, 0.34);
    glEnd();


    glPointSize(5.0);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glBegin(GL_POINTS);     // 中心点
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(0, 0);
    glEnd();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPushMatrix();
    glTranslatef(xmove, 0.0, 0.0);
    glScalef(rate, rate, 1.0);
    

    glRotatef(ang, 0.0, 0.0, 01.0);

    // std::cout << cameraAngleY << std::endl;

    glFlush();
    glutSwapBuffers();
}

void responseFunc(void)
{
    if (3 == transformation) // zoom condition
    {
        //初始化状态
        xmove = 0.0;
        ang = 360.0;

        if (rate >= 2.8)
            flag = -1; 
        else if (rate <= 0.99)
            flag = +1;

        rate += v * flag / 100.0;

        // std::cout << rate << std::endl;
        glutPostRedisplay();

    } else if (2 == transformation) // rotate condition
    {
        //初始化状态
        xmove = 0.0;
        rate = 1.0;

        if (ang >= 360)
            flag = -1;
        else if (ang <= 0.0)
            flag = 1;

        ang += v * flag;
        // std::cout << ang << std::endl;
        
        glutPostRedisplay();

    } else // if (1 == transformation) // move condition
    {
        //初始化状态
        ang = 360.0;
        rate = 1.0;

        if (xmove >= 1.1)
            flag = -1;
        else if (xmove <= -1.1)
            flag = 1;

        xmove += v * flag / 100.0;
        // std::cout << xmove << std::endl;
        glutPostRedisplay();
    }


}


// This function does any needed initialization on the rendering
// context.
void SetupRC()
{ // Black background
    glClearColor(0.785, 0.703, 0.792, 1.0f);
    // Set drawing color to green
    // glColor3f(0.0f, 1.0f, 0.0f);

}

//
void ChangeSize(int w, int h) // 改变窗口大小时保持图形比例合理
{
    GLfloat nRange = 1.0f;
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
        glOrtho(-nRange, nRange, -nRange * h / w, nRange * h / w, -nRange, nRange);
    else
        glOrtho(-nRange * w / h, nRange * w / h, -nRange, nRange, -nRange, nRange);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    GLsizei h = 500;
    GLsizei w = 700;
    GLsizei x = 600, y = 300;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(w, h);
    glutInitWindowPosition(x, y);
    glutCreateWindow("task 1");

    // menu
    int lineTypeOption, SpeedOption, transformationOption, mainMenu, Bcolor;

    lineTypeOption = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("line", 2);
    glutAddMenuEntry("fill", 1);

    SpeedOption = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("slow", 3);
    glutAddMenuEntry("medium", 4);
    glutAddMenuEntry("fast", 5);

    transformationOption = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("move", 6);
    glutAddMenuEntry("rotate", 7);
    glutAddMenuEntry("zoom", 8);

    Bcolor = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("color 1", 9);
    glutAddMenuEntry("color 2", 10);
    glutAddMenuEntry("color 3", 11);
    glutAddMenuEntry("color 4", 12);

    mainMenu = glutCreateMenu(ProcessMenu);
    glutAddSubMenu("lineType", lineTypeOption); //
    glutAddSubMenu("Speed", SpeedOption); //
    glutAddSubMenu("transformation", transformationOption);
    glutAddSubMenu("background color", Bcolor);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(ChangeSize);
    // glutDisplayFunc(Background);
    glutIdleFunc(&responseFunc);
    glutDisplayFunc(RenderScene);
    
    SetupRC();
    glutMainLoop();

    return 0;
}
