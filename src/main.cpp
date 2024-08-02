#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include "../resources/glfw/include/GLFW/glfw3.h"



#define WIDTH 500
#define HEIGHT 500
#define TITLE "Double Pendulum | Hasan Efe Aksoy"

#define PI 3.14159265
#define BIG 1000000




double xpos, ypos;


const float originX = 0.0f;
const float originY = 0.0f;

float xPos1 = 0.0f;
float yPos1 = 0.0f;
float xPos2 = 0.0f;
float yPos2 = 0.0f;

const float m1 = 12.0f;
const float m2 = 12.0f;

float ang1 = PI;
float ang2 = PI + 0.0001f;

float angVel1 = 0.0f;
float angVel2 = 0.0f;

float angAcc1 = 0.0f;
float angAcc2 = 0.0f;

const float lenght1 = HEIGHT / 4;
const float lenght2 = HEIGHT / 4;

const float g = 1.0f;



int main()
{
    GLFWwindow* window = nullptr;
    
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, 0);
    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();





    while (!glfwWindowShouldClose(window))
    {   
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        
        glColor3f(1.0f, 1.0f, 1.0f);



        float numerator1 = -g * (2*m1 + m2) * sin(ang1) - m2 * g * sin(ang1 - 2*ang2) - 2 * sin(ang1 - ang2) * m2 * (angVel2*angVel2 * lenght2 + angVel1*angVel1 * lenght1 * cos(ang1 - ang2));
        float numerator2 = 2 * sin(ang1 - ang2) * (angVel1*angVel1 * lenght1 * (m1 + m2) + g * (m1 + m2) * cos(ang1) + angVel2*angVel2 * lenght2 * m2 * cos(ang1 - ang2));
        float denominator1 = lenght1 * (2 * m1 + m2 - m2 * cos(2*ang1 - 2*ang2));
        float denominator2 = lenght2 * (2 * m1 + m2 - m2 * cos(2*ang1 - 2*ang2));

        angAcc1 = numerator1 / denominator1;
        angAcc2 = numerator2 / denominator2;
        
        angVel1 += angAcc1;
        angVel2 += angAcc2;
        
        ang1 += angVel1;
        ang2 += angVel2;


        xPos1 = originX + lenght1 * sin(ang1);
        yPos1 = originY - lenght1 * cos(ang1);

        xPos2 = xPos1 + lenght2 * sin(ang2);
        yPos2 = yPos1 - lenght2 * cos(ang2);


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos = xpos - WIDTH / 2;
            ypos = HEIGHT / 2 - ypos;

            angVel1 = 0.0f;
            angVel2 = 0.0f;
            ang1 = atan2(ypos - originY, xpos - originX);
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos = xpos - WIDTH / 2;
            ypos = HEIGHT / 2 - ypos;

            angVel1 = 0.0f;
            angVel2 = 0.0f;
            ang2 = atan2(ypos - yPos1, xpos - xPos1);
        }



        glBegin(GL_LINE_STRIP);
        glVertex2f(originX, originY);
        glVertex2f(xPos1, yPos1);
        glVertex2f(xPos2, yPos2);
        glEnd();
        
        glBegin(GL_POLYGON);
        for (float i = 0; i < 2*PI; i += 2*PI/15) {
            glVertex2f(xPos1 + m1 * cos(i), yPos1 + m1 * sin(i));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (float i = 0; i < 2*PI; i += 2*PI/15) {
            glVertex2f(xPos2 + m2 * cos(i), yPos2 + m2 * sin(i));
        }
        glEnd();
        


        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    glfwTerminate();
    return 0;
}