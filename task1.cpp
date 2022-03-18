#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>

#define GL_PI 3.1415f
#define REDISPLAYTIMERID 1

static int isLine = 2; // (1， 2) -> (填充， 线条)
static int speed = 2; // (1, 2, 3) -> (慢， 中， 快)
static int transformation = 2; // (1, 2, 3) -> (平移， 旋转， 缩放)
static int interval = 25; // 定时器间隔（用于调速度）

static float intervalRad = 0; // 半径变化间隔
static float intervalMove = 0; // 移动距离间隔
static int interAng = 360; //  角度旋转间隔
static float r = 40.0 / sqrt(3); // 初始化圆半径
static int flag = 1; // 控制变化方向

// 菜单事件处理
void ProcessMenu(int value)
{
    switch (value) {
    case 1:
        isLine = 1;
        break;
    case 2:
        isLine = 2;
        break;
    case 3:
        speed = 1;
        break;
    case 4:
        speed = 2;
        break;
    case 5:
        speed = 3;
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
    default:
        isLine = 1;
        speed = 1;
        transformation = 1;
        break;
    }
    glutPostRedisplay();
}

void Background(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(200.0);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.6, 1.0);
    glVertex2f(-300, -300);
    glColor3f(0.8, 0.1, 1.0);
    glVertex2f(-300, 300);
    glColor3f(0.3, 1.0, 0.7);
    glVertex2f(300, 300);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(300, -300);
    glEnd();
    // glFlush();
}

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    Background();
    switch (isLine) {
    case 1: // 填充三角形
        glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0, 0);
        glVertex3f((r + intervalRad) * cos((210 + interAng) * GL_PI / 180) + intervalMove, (r + intervalRad) * sin((210 + interAng) * GL_PI / 180), 0);
        glVertex3f((r + intervalRad) * cos((330 + interAng) * GL_PI / 180) + intervalMove, (r + intervalRad) * sin((330 + interAng) * GL_PI / 180), 0);
        glVertex3f((r + intervalRad) * cos((90 + interAng) * GL_PI / 180) + intervalMove, (r + intervalRad) * sin((90 + interAng) * GL_PI / 180), 0);
        glEnd();

        glColor3f(0, 1.0, 0);
        glPointSize(3);
        glBegin(GL_POINTS);
        if (3 == transformation)
            glVertex3f(0, 0, 0);
        else
            glVertex3f((intervalRad)*cos((210 + interAng) * GL_PI / 180) + intervalMove, (intervalRad)*sin((210 + interAng) * GL_PI / 180), 0);
        glEnd();

        break;

    case 2: // 线条三角形
        glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 0, 0);
        glVertex3f((r + intervalRad) * cos((210 + interAng) * GL_PI / 180) + intervalMove, (r + intervalRad) * sin((210 + interAng) * GL_PI / 180), 0);
        glVertex3f((r + intervalRad) * cos((330 + interAng) * GL_PI / 180) + intervalMove, (r + intervalRad) * sin((330 + interAng) * GL_PI / 180), 0);
        glVertex3f((r + intervalRad) * cos((90 + interAng) * GL_PI / 180) + intervalMove, (r + intervalRad) * sin((90 + interAng) * GL_PI / 180), 0);
        glEnd();

        glColor3f(0, 1.0, 0);
        glPointSize(3);
        glBegin(GL_POINTS);
        if (3 == transformation)
            glVertex3f(0, 0, 0);
        else
            glVertex3f((intervalRad)*cos((210 + interAng) * GL_PI / 180) + intervalMove, (intervalRad)*sin((210 + interAng) * GL_PI / 180), 0);
        glEnd();

        break;
    }

    // glFlush();
    glutSwapBuffers();
}

void timerFunc(int nTimerID)
{
    // 不同速度的定时器间隔
    if (1 == speed)
        interval = 70;
    else if (2 == speed)
        interval = 25;
    else
        interval = 1;

    if (3 == transformation) // zoom condition
    {
        //初始化状态
        interAng = 360;
        intervalMove = 0;

        if (intervalRad >= 100.0 - r)
            flag = -1;
        if (intervalRad <= 0.0)
            flag = 1;
        intervalRad += 1 * flag;
    } else if (2 == transformation) // rotate condition
    {
        //初始化状态
        intervalRad = 0;
        intervalMove = 0;

        if (interAng >= 360)
            flag = -1;
        if (interAng <= 0.0)
            flag = 1;
        interAng += 4 * flag;
    } else // if (1 == transformation) // move condition
    {
        //初始化状态
        interAng = 360;
        intervalRad = 0;

        if (intervalMove >= 140.0 - r)
            flag = -1;
        if (intervalMove <= r - 140.0)
            flag = 1;
        intervalMove += 2 * flag;
    }

    glutPostRedisplay();
    glutTimerFunc(interval, timerFunc, REDISPLAYTIMERID);
}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
{ // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Set drawing color to green
    glColor3f(0.0f, 1.0f, 0.0f);

    // glEnable(GL_LINE_SMOOTH); //启用
    // glHint(GL_LINE_SMOOTH,GL_NICEST);

    // glEnable(GL_POINT_SMOOTH); //启用
    // glHint(GL_POINT_SMOOTH,GL_NICEST);
}

//
void ChangeSize(int w, int h) // 改变窗口大小时保持图形比例合理
{
    GLfloat nRange = 100.0f;
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
    int lineTypeOption, SpeedOption, transformationOption, mainMenu;

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

    mainMenu = glutCreateMenu(ProcessMenu);
    glutAddSubMenu("lineType", lineTypeOption); //
    glutAddSubMenu("Speed", SpeedOption); //
    glutAddSubMenu("transformation", transformationOption);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(ChangeSize);
    // glutDisplayFunc(Background);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutTimerFunc(interval, timerFunc, REDISPLAYTIMERID);
    glutMainLoop();

    return 0;
}
