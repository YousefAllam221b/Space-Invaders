//  Includes
#include <stdio.h>
#include <vector>
#include <math.h>
#include <random>
#include <glut.h>
#include <vector>
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void KeyPressed(unsigned char key, int x, int y);
void KeyReleased(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
//-----------------

//  Global Variables
bool a, w, s, d = false;

// Spaceship values
int spaceshipOriginX = 325;
int spaceshipOriginY = 100;
int translationSpaceshipX = 0;
int translationSpaceshipY = 0;
double spaceshipWidth = 150;
double spaceshipHeight = 100;

// Enemy values
int enemyOriginX = 325;
int enemyOriginY = 350;
int translationEnemyX = 0;
int translationEnemyY = 0;
double enemyWidth = 150;
double enemyHeight = 100;
bool rightDirection = false;



//-----------------

void main(int argc, char** argr) {
    glutInit(&argc, argr);

    glutInitWindowSize(800, 500);
    glutInitWindowPosition(150, 0);

    glutCreateWindow("Space Invaders");
    //glutFullScreen();
    glutDisplayFunc(Display);
    glutKeyboardFunc(KeyPressed);      // sets the Keyboard handler function; called when a key is pressed
    glutKeyboardUpFunc(KeyReleased);  // sets the KeyboardUp handler function; called when a key is released
    //glutMouseFunc(Mouse);       // sets the Mouse handler function; called when a mouse button is clicked
    //glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
    Timer(0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 700, 0, 700);

    glutMainLoop();
}

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// draws a circle using OpenGL's gluDisk, given (x,y) of its center and it's radius
void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}

// draws a partial circle using OpenGL's gluDisk, given (x,y) of its center, it's radius, starting and sweep angles.
void drawPartialCircle(int x, int y, float r, int startAngle, int sweepAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluPartialDisk(quadObj, 0, r, 50, 50, startAngle, sweepAngle);
    glPopMatrix();
}

void drawSpaceship()
{
    glPushMatrix();
    // Spaceship inital position
    glTranslatef(translationSpaceshipX, translationSpaceshipY, 0);

    // Bounding Box
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(spaceshipOriginX - spaceshipWidth / 3 - 5, spaceshipOriginY - 5, spaceshipWidth + 10, spaceshipHeight + 10);

    // Mode Setup
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    // Body
    glColor3f(0.0f, 0.0f, 1.0f);
    drawRect(spaceshipOriginX, spaceshipOriginY, spaceshipWidth / 3, spaceshipHeight * 3 / 4);

    // Left Triangle
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(spaceshipOriginX, spaceshipOriginY);
    glVertex2f(spaceshipOriginX - spaceshipWidth / 3, spaceshipOriginY + spaceshipHeight * 3 / 4);
    glVertex2f(spaceshipOriginX, spaceshipOriginY + spaceshipHeight * 3 / 4);
    glEnd();

    // Right Triangle
    glColor3f(0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    glVertex2f(spaceshipOriginX + spaceshipWidth / 3, spaceshipOriginY);
    glVertex2f(spaceshipOriginX + spaceshipWidth * 2 / 3, spaceshipOriginY + spaceshipHeight * 3 / 4);
    glVertex2f(spaceshipOriginX + spaceshipWidth / 3, spaceshipOriginY + spaceshipHeight * 3 / 4 );
    glEnd();

    // Half Circle
    glColor3f(1.0f, 1.0f, 0.0f);
    drawPartialCircle(spaceshipOriginX + spaceshipWidth / 6, spaceshipOriginY + spaceshipHeight * 3 / 4, spaceshipWidth / 6, 270, 180);

    // Render Spaceship
    glPopMatrix();
}

void drawEnemy()
{
    glPushMatrix();
    glTranslatef(translationEnemyX, translationEnemyY, 0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Body
    glColor3f(0.0f, 0.0f, 1.0f);
    drawRect(enemyOriginX, enemyOriginY, enemyWidth / 3, enemyHeight * 3 / 4);

    // Left Triangle
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(enemyOriginX, enemyOriginY);
    glVertex2f(enemyOriginX - enemyWidth / 3, enemyOriginY + enemyHeight * 3 / 4);
    glVertex2f(enemyOriginX, enemyOriginY + enemyHeight * 3 / 4);
    glEnd();

    glPopMatrix();
}

// Mouse handler function
//   button: the mouse button clicked: left, right or middle
//   state:  the state of the button: clicked (down) or released (up)
//   x  : the X coordinate of the mouse cursor at the moment of clicking the mouse button
//   y  : the Y coordinate of the mouse cursor at the moment of clicking the mouse button
//void Mouse(int button, int state, int x, int y) {
//    // calculate the Y coordinate of the tip of the arm
//    int barTipY = bar1Y + bar2Y + bar3Y + 400;
//
//    // if the tip of the bar is inside the ball's circumference
//    if (barTipY <= ballY + 15 && barTipY >= ballY - 15)
//        // color the background green
//        glClearColor(0, 1, 0, 0);
//    else
//        // otherwise, color it red
//        glClearColor(1, 0, 0, 0);
//
//    // ask OpenGL to recall the display function to reflect the changes on the window
//    glutPostRedisplay();
//}


void Timer(int value) {
    // set the ball's Y coordinate to a random number between 10 and 780 (since the window's height is 800)
    //ballY = rand() % 780 + 10;
    //ballY = 500;
    if (rightDirection)
    {
        if (translationEnemyX > -(spaceshipOriginX - 70))
            translationEnemyX -= 10;
        else
            rightDirection = false;
    }
    else 
    {
        if (translationEnemyX < enemyOriginX - 70)
            translationEnemyX += 10;
        else
            rightDirection = true;
    }

    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();

    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(25, Timer, 0);
}

void Display() {
    // Color setup
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);

    // Drawing Spaceship
    drawSpaceship();

    // Drawing Enemy
    drawEnemy();

    glFlush();
}

void KeyPressed(unsigned char key, int x, int y)
{
    // Setting moving booleans
    if (key == 'a') a = true;
    if (key == 'd') d = true;
    if (key == 'w') w = true;
    if (key == 's') s = true;

    // Moving the Spaceship
    if (a == true && translationSpaceshipX >= -(spaceshipOriginX - 70)) translationSpaceshipX -= 10;
    if (d == true && translationSpaceshipX <= spaceshipOriginX - 70) translationSpaceshipX += 10;
    if (w == true && translationSpaceshipY <= glutGet(GLUT_WINDOW_HEIGHT) - 30) translationSpaceshipY += 10;
    if (s == true && translationSpaceshipY >= -80) translationSpaceshipY -= 10;
    glutPostRedisplay();
}

void KeyReleased(unsigned char key, int x, int y)
{
    // Clearing moving booleans
    if (key == 'a') a = false;
    if (key == 'd') d = false;
    if (key == 'w') w = false;
    if (key == 's') s = false;
    glutPostRedisplay();
}