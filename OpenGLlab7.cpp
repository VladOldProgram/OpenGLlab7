#include <iostream>
#include <glut.h>
#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

GLfloat cylinderRotateX, cylinderRotateY, sphereRotateX, sphereRotateY = 0.0f;

void RotateObjects(int key, int x, int y)
{
    switch (key) {
    case (GLUT_KEY_RIGHT): {
        cylinderRotateY += 3;
        sphereRotateY += 3;
        break;
    }
    case (GLUT_KEY_LEFT): {
        cylinderRotateY -= 3;
        sphereRotateY -= 3;
        break;
    }
    case (GLUT_KEY_UP): {
        cylinderRotateX += 3;
        sphereRotateX += 3;
        break;
    }
    case (GLUT_KEY_DOWN): {
        cylinderRotateX -= 3;
        sphereRotateX -= 3;
        break;
    }
    }

    glutPostRedisplay();
}

GLuint bottomCylinderTexture, sideCylinderTexture, topCylinderTexture;
GLuint sphereTexture;
GLuint floorTexture, backWallTexture;

struct vector
{
    float x, y, z;
};

#define sqr(a) (a)*(a)
void CalculateNormal(vector a, vector b, vector c, vector* n)
{
    float wrki;
    vector v1, v2;
    v1.x = a.x - b.x;
    v1.y = a.y - b.y;
    v1.z = a.z - b.z;
    v2.x = b.x - c.x;
    v2.y = b.y - c.y;
    v2.z = b.z - c.z;

    n->x = (v1.y * v2.z - v1.z * v2.y);
    n->y = (v1.z * v2.x - v1.x * v2.z);
    n->z = (v1.x * v2.y - v1.y * v2.x);
    wrki = sqrt(sqr(n->x) + sqr(n->y) + sqr(n->z));
    n->x /= wrki;
    n->y /= wrki;
    n->z /= wrki;
}

void DrawSphere(float radius, int precision)
{
    float theta1, theta2, theta3 = 0.0f;
    float ex, ey, ez = 0.0f;
    float px, py, pz = 0.0f;

    for (int i = 0; i < precision / 2; i++) {
        theta1 = i * M_PI * 2 / precision - M_PI / 2;
        theta2 = (i + 1) * M_PI * 2 / precision - M_PI / 2;
        glBindTexture(GL_TEXTURE_2D, sphereTexture);
        glBegin(GL_TRIANGLE_STRIP);
        {
            for (int j = 0; j <= precision; j++) {
                theta3 = j * M_PI * 2 / precision;

                ex = cos(theta2) * cos(theta3);
                ey = sin(theta2);
                ez = cos(theta2) * sin(theta3);
                px = radius * ex;
                py = radius * ey;
                pz = radius * ez;

                glNormal3f(-ex, -ey, -ez);
                glTexCoord2f(j / (float)precision, 2 * (i + 1) / (float)precision);
                glVertex3f(px, py, pz);

                ex = cos(theta1) * cos(theta3);
                ey = sin(theta1);
                ez = cos(theta1) * sin(theta3);
                px = radius * ex;
                py = radius * ey;
                pz = radius * ez;
                
                glNormal3f(-ex, -ey, -ez);
                glTexCoord2f(j / (float)precision, 2 * i / (float)precision);
                glVertex3f(px, py, pz);
            }
        }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void DrawCylinder(float radius, float height, int countOfSides)
{
    float a = 0.5f;
    float b = 0.5f;
    float c = 0.25f;
    vector n, A, B, C;

    glBindTexture(GL_TEXTURE_2D, sideCylinderTexture);
    glBegin(GL_QUAD_STRIP);
    for (int i = 1; i <= countOfSides; i++) {
        A.x = radius * cos(2 * M_PI * i / countOfSides);
        A.y = radius * sin(2 * M_PI * i / countOfSides);
        A.z = -height / 2;
        B.x = radius * cos(2 * M_PI * i / countOfSides);
        B.y = radius * sin(2 * M_PI * i / countOfSides);
        B.z = height / 2;
        C.x = radius * cos(2 * M_PI * (i - 1) / countOfSides);
        C.y = radius * sin(2 * M_PI * (i - 1) / countOfSides);
        C.z = height / 2;
        CalculateNormal(A, B, C, &n);
        glNormal3f(-n.x, -n.y, -n.z);
        glTexCoord2f((i - 1) / (float)countOfSides, 0.0f + c);
        glVertex3f(radius * cos(2 * M_PI * i / countOfSides), radius * sin(2 * M_PI * i / countOfSides), -height / 2);
        glNormal3f(-n.x, -n.y, -n.z);
        glTexCoord2f((i - 1) / (float)countOfSides, 1.0f - c);
        glVertex3f(radius * cos(2 * M_PI * i / countOfSides), radius * sin(2 * M_PI * i / countOfSides), height / 2);
    }
    A.x = radius * cos(2 * M_PI * 1 / countOfSides);
    A.y = radius * sin(2 * M_PI * 1 / countOfSides);
    A.z = -height / 2;
    B.x = radius * cos(2 * M_PI * 1 / countOfSides);
    B.y = radius * sin(2 * M_PI * 1 / countOfSides);
    B.z = height / 2;
    C.x = radius * cos(2 * M_PI * countOfSides - 1 / countOfSides);
    C.y = radius * sin(2 * M_PI * countOfSides - 1 / countOfSides);
    C.z = height / 2;
    CalculateNormal(A, B, C, &n);
    glNormal3f(-n.x, -n.y, -n.z);
    glTexCoord2f(1.0f, 0.0f + c);
    glVertex3f(radius * cos(2 * M_PI * 1 / countOfSides), radius * sin(2 * M_PI * 1 / countOfSides), -height / 2);
    glNormal3f(-n.x, -n.y, -n.z);
    glTexCoord2f(1.0f, 1.0f - c);
    glVertex3f(radius * cos(2 * M_PI * 1 / countOfSides), radius * sin(2 * M_PI * 1 / countOfSides), height / 2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, bottomCylinderTexture);
    glBegin(GL_POLYGON);
    A.x = radius * cos(2 * M_PI * 3 / 10);
    A.y = radius * sin(2 * M_PI * 3 / 10);
    A.z = -height / 2;
    B.x = radius * cos(2 * M_PI * 2 / 10);
    B.y = radius * sin(2 * M_PI * 2 / 10);
    B.z = -height / 2;
    C.x = radius * cos(2 * M_PI * 1 / 10);
    C.y = radius * sin(2 * M_PI * 1 / 10);
    C.z = -height / 2;
    CalculateNormal(A, B, C, &n);
    for (int i = 1; i <= countOfSides; i++) {
        glNormal3f(-n.x, -n.y, -n.z);
        glTexCoord2f(a + b * cos(2 * M_PI * i / countOfSides), a + b * sin(2 * M_PI * i / countOfSides));
        glVertex3f(radius * cos(2 * M_PI * i / countOfSides), radius * sin(2 * M_PI * i / countOfSides), -height / 2);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, topCylinderTexture);
    glBegin(GL_POLYGON);
    A.x = radius * cos(2 * M_PI * 1 / 10);
    A.y = radius * sin(2 * M_PI * 1 / 10);
    A.z = height / 2;
    B.x = radius * cos(2 * M_PI * 2 / 10);
    B.y = radius * sin(2 * M_PI * 2 / 10);
    B.z = height / 2;
    C.x = radius * cos(2 * M_PI * 3 / 10);
    C.y = radius * sin(2 * M_PI * 3 / 10);
    C.z = height / 2;
    CalculateNormal(A, B, C, &n);
    glNormal3f(n.x, n.y, n.z);
    for (int i = 1; i <= countOfSides; i++) {
        glNormal3f(n.x, n.y, n.z);
        glTexCoord2f(a + b * cos(2 * M_PI * i / countOfSides), a + b * sin(2 * M_PI * i / countOfSides));
        glVertex3f(radius * cos(2 * M_PI * i / countOfSides), radius * sin(2 * M_PI * i / countOfSides), height / 2);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawScene()
{
    // Верхняя поверхность
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-2.0f, 1.5f, 5.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-2.0f, 1.5f, -2.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(2.0f, 1.5f, -2.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(2.0f, 1.5f, 5.0f);
    glEnd();

    // Правая поверхность
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, -1.5f, 5.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, -1.5f, -2.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 1.5f, -2.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 1.5f, 5.0f);
    glEnd();

    // Левая поверхность
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, -1.5f, 5.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 1.5f, 5.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 1.5f, -2.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, -1.5f, -2.0f);
    glEnd();

    // Задняя поверхность
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, backWallTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glNormal3f(0.0f, 0.0, 1.0f);
    glVertex3f(-2.0f, -1.5f, -2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glNormal3f(0.0f, 0.0, 1.0f);
    glVertex3f(2.0f, -1.5f, -2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glNormal3f(0.0f, 0.0, 1.0f);
    glVertex3f(2.0f, 1.5f, -2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glNormal3f(0.0f, 0.0, 1.0f);
    glVertex3f(-2.0f, 1.5f, -2.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Нижняя поверхность
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, -1.5f, 5.0f);
    glTexCoord2f(1.0f, 0.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(2.0f, -1.5f, 5.0f);
    glTexCoord2f(1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(2.0f, -1.5f, -2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, -1.5f, -2.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    GLfloat light0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, -3.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glPopMatrix();
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f); 
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f); 
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.02f);
    glEnable(GL_LIGHT0);

    glTranslatef(0.0f, 0.0f, -8.0f);

    // Сцена
    DrawScene();

    // Цилиндр
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(310, 1.0f, 0.0f, 0.0f);
    glRotatef(cylinderRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(cylinderRotateY, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    DrawCylinder(0.35, 1.5, 20);
    glPopMatrix();

    // Сфера
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    glRotatef(sphereRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(sphereRotateY, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    DrawSphere(1.0f, 30);
    glPopMatrix();

    glutSwapBuffers();
}

void LoadTexture(GLuint* texture, const char* imageName)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    int width, height, cnt;
    unsigned char* data = stbi_load(imageName, &width, &height, &cnt, 0);
    if (data) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
            0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else std::cout << "Failed to load texture" << std::endl;
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 800);
    glutCreateWindow("OpenGL lab 7");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    LoadTexture(&bottomCylinderTexture, "bottomCylinderTexture.jpg");
    LoadTexture(&sideCylinderTexture, "sideCylinderTexture.png");
    LoadTexture(&topCylinderTexture, "topCylinderTexture.jpg");
    LoadTexture(&sphereTexture, "sphereTexture.png");
    LoadTexture(&floorTexture, "floorTexture.jpg");
    LoadTexture(&backWallTexture, "backWallTexture.jpg");

    glutDisplayFunc(Display);
    glutSpecialFunc(RotateObjects);

    glutMainLoop();

    return 0;
}