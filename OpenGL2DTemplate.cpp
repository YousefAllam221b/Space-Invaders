//  Includes
#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void KeyPressed(unsigned char key, int x, int y);
void KeyReleased(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void EnemyTimer(int value);
void PowerupTimer(int value);
void Display();
bool areCollided(int x1, int y1, int w1, int l1, int x2, int y2, int w2, int l2);
void resetValues();
void drawHeart(int x, int y);
void drawlifeBar(int enemyLifeWidth);
void PowerupTimerTimeout(int value);
void print(int x, int y, char* string);
//-----------------
class Bullet
{
public: int x; int y;
};
//  Global Variables
bool a, w, s, d = false;

// Spaceship values
int spaceshipOriginX = 325;
int spaceshipOriginY = 100;
int translationSpaceshipX = 0;
int translationSpaceshipY = 0;
int spaceshipWidth = 150;
int spaceshipLength = 120;
int spaceshipLives = 3;
int Red = 0;
int Green = 0;
int Blue = 1;

// Enemy values
int enemyOriginX = 325;
int enemyOriginY = 550;
int translationEnemyX = 0;
int translationEnemyY = 0;
double enemyWidth = 150;
double enemyLength = 100;
bool rightDirection = false;
int enemyLifeWidth = 500;

// Bullets
vector<Bullet> spaceshipBullets;
vector<Bullet> enemyBullets;
int bulletLength = 20;
int bulletWidth = 12;

bool powerupActive = false;
bool isThereAPowerup = false;
int powerupX = 0;
int powerupY = 0;


bool isGameDone = true;

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
    glutMouseFunc(Mouse);       // sets the Mouse handler function; called when a mouse button is clicked
    //glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
    EnemyTimer(0);
    Timer(0);
    PowerupTimer(0);
    //PowerupTimerTimeout(0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 700, 0, 700);

    glutMainLoop();
}
    
// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
    
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

// Draws the Player's Spaceship 
void drawSpaceship()
{

    glPushMatrix();
    glColor3f(Red,Green,Blue);
    // Spaceship inital position
    glTranslatef(translationSpaceshipX, translationSpaceshipY, 0);

    // Bounding Box
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(spaceshipOriginX - spaceshipWidth / 3, spaceshipOriginY, spaceshipWidth, 120);
    // Mode Setup
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    // Body
    drawRect(spaceshipOriginX, spaceshipOriginY, spaceshipWidth / 3, spaceshipLength * 3 / 4);

    // Left Triangle
    glVertex2f(spaceshipOriginX, spaceshipOriginY);

    glBegin(GL_TRIANGLES);
    glVertex2f(spaceshipOriginX, spaceshipOriginY);
    glVertex2f(spaceshipOriginX - spaceshipWidth / 3, spaceshipOriginY + spaceshipLength * 3 / 4);
    glVertex2f(spaceshipOriginX, spaceshipOriginY + spaceshipLength * 3 / 4);
    glEnd();

    // Right Triangle
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    glVertex2f(spaceshipOriginX + spaceshipWidth / 3, spaceshipOriginY);
    glVertex2f(spaceshipOriginX + spaceshipWidth * 2 / 3, spaceshipOriginY + spaceshipLength * 3 / 4);
    glVertex2f(spaceshipOriginX + spaceshipWidth / 3, spaceshipOriginY + spaceshipLength * 3 / 4 );
    glEnd();

    // Half Circle
    glColor3f(1.0f, 1.0f, 0.0f);
    drawPartialCircle(spaceshipOriginX + spaceshipWidth / 6, spaceshipOriginY + spaceshipLength * 3 / 4, spaceshipWidth / 6, 270, 180);

    // Barrel
    glColor3f(1.0f, 1.0f, 0.0f);
    drawRect(spaceshipOriginX + spaceshipWidth / 6 - bulletWidth / 2, spaceshipOriginY + spaceshipLength * 3 / 4 + spaceshipWidth / 6 - 10, bulletWidth, bulletLength);

    // Render Spaceship
    glPopMatrix();
}

// Draws the Enemy's Spaceship 
void drawEnemy()
{
    //glPushMatrix();
    //glTranslatef(translationEnemyX, translationEnemyY, 0);
    //
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //// Body
    //glColor3f(0.0f, 0.0f, 1.0f);
    //drawRect(enemyOriginX, enemyOriginY, enemyWidth / 3, enemyHeight * 3 / 4);

    //// Left Triangle
    //glColor3f(0.0f, 1.0f, 0.0f);
    //glBegin(GL_TRIANGLES);
    //glVertex2f(enemyOriginX, enemyOriginY);dw
    //glVertex2f(enemyOriginX - enemyWidth / 3, enemyOriginY + enemyHeight * 3 / 4);
    //glVertex2f(enemyOriginX, enemyOriginY + enemyHeight * 3 / 4);
    //glEnd();

    //glPopMatrix();
    glPushMatrix();
    glTranslatef(translationEnemyX, translationEnemyY, 0);
    // enemyheight = 25
    drawCircle(enemyOriginX, enemyOriginY, 15);
    glVertex2f(enemyOriginX, enemyOriginY);
    glBegin(GL_TRIANGLES);
    glVertex2f(enemyOriginX, enemyOriginY);
    glVertex2f(enemyOriginX - enemyWidth / 3, enemyOriginY + enemyLength * 3 / 4);
    glVertex2f(enemyOriginX + enemyWidth / 3, enemyOriginY + enemyLength * 3 / 4);
    glEnd();

    drawRect(enemyOriginX - 6, enemyOriginY - 30, bulletWidth, 15);
    glPopMatrix();
}

// Draws the Player's and the Enemy's Bullet 
void drawBullet(int x, int y)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0, 0, 1);
    drawRect(x, y, bulletWidth, bulletLength + bulletWidth / 2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawRect(x, y, bulletWidth, bulletLength);
    glColor3f(1, 0, 1);
    drawPartialCircle(x + bulletWidth/2, y + bulletLength, bulletWidth/2, 270, 180);
}

// Draws the Player's and the Enemy's Bullet 
void drawEnemyBullet(int x, int y)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(x, y - bulletWidth / 2, bulletWidth, bulletLength + bulletWidth / 2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawRect(x, y, bulletWidth, bulletLength);
    glColor3f(1, 0, 1);
    drawPartialCircle(x + bulletWidth / 2, y, bulletWidth / 2, 90, 180);
}

void drawPowerup(int x, int y)
{
    glColor3b(0, 0, 1);
    drawCircle(x, y, 25);
}

// Handles the Mouse clicks
void Mouse(int button, int state, int x, int y) {

    if (!isGameDone) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            Bullet bullet;
            bullet.x = spaceshipOriginX + translationSpaceshipX + spaceshipWidth / 6 - bulletWidth / 2;
            bullet.y = spaceshipOriginY + translationSpaceshipY + spaceshipLength * 3 / 4 + spaceshipWidth / 6;
            spaceshipBullets.push_back(bullet);
        }
    }
    if (isGameDone)
    {
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            isGameDone = false;
            glClearColor(1, 1, 1, 0);
        }
    }
}

void disactivePowerup(int value)
{
    powerupActive = false;
    Blue = 1;
    Green = 0;
}
void disappearPowerup(int value)
{
    isThereAPowerup = false;
}
// Handles the Periodic tasks.
void Timer(int value) {
    // set the ball's Y coordinate to a random number between 10 and 780 (since the window's height is 800)
    //ballY = rand() % 780 + 10;
    //ballY = 500;
    if (!isGameDone)
    {
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

    
        for (auto& bullet : spaceshipBullets)
        {
            bullet.y += 10;
        }
        for (auto& bullet : enemyBullets)
        {
            bullet.y -= 10;
        }
        int x1 = enemyOriginX + translationEnemyX - enemyWidth/2;
        int y1 = enemyOriginY - enemyLength / 2;
        int x2 = spaceshipOriginX + translationSpaceshipX - spaceshipWidth / 3;
        int y2 = spaceshipOriginY + translationSpaceshipY;

        //printf("%f", spaceshipOriginX + translationSpaceshipX - spaceshipWidth / 3);
        int i = 0;

        if (!powerupActive)
        {
            for (auto& bullet : enemyBullets)
            {
                int spaceshipCurrentX = spaceshipOriginX + translationSpaceshipX - spaceshipWidth / 3;
                int spaceshipCurrentY = spaceshipOriginY + translationSpaceshipY - spaceshipLength / 2;


                int collision = areCollided(bullet.x - bulletWidth / 2, bullet.y - bulletLength / 2, bulletWidth, bulletLength + bulletWidth / 2, spaceshipCurrentX, spaceshipCurrentY, spaceshipWidth, spaceshipLength);
                if (collision)
                {
                    spaceshipLives--;
                    enemyBullets.erase(enemyBullets.begin() + i);
                    if (spaceshipLives <= 0)
                    {
                        printf("YOU ARE DEAD");
                        isGameDone = true;
                        resetValues();
                        glClearColor(1, 0, 0, 0);
                    }
                }
                else
                {
                    i++;
                }
            }
        }
        i = 0;
        for (auto& bullet : spaceshipBullets)
        {
            int enemyCurrentX = enemyOriginX + translationEnemyX - enemyWidth / 2;
            int enemyCurrentY = enemyOriginY + translationEnemyY - enemyLength / 2;
            int collision = areCollided(bullet.x, bullet.y, bulletWidth, bulletLength + bulletWidth / 2, enemyCurrentX, enemyCurrentY, enemyWidth, enemyLength);
            if (collision)
            {
                enemyLifeWidth-=100;
                spaceshipBullets.erase(spaceshipBullets.begin() + i);
                if (enemyLifeWidth <= 0)
                {
                    printf("YOU WON");
                    isGameDone = true;
                    resetValues();
                    glClearColor(0, 1, 0, 0);
                }
            }
            else
            {
                i++;
            }
        }
        
        if (isThereAPowerup)
        {
            bool acquirePowerup  = areCollided(x2, y2, spaceshipWidth, spaceshipLength, powerupX - 12.5, powerupY - 12.5, 25,25);
            if (acquirePowerup)
            {
                printf("TAKEN");
                powerupActive = true;
                isThereAPowerup = false;
                isThereAPowerup = false;
                Blue = 0;
                Green = 1;
                powerupX = 0;
                powerupY = 0;
                glutTimerFunc(5000, disactivePowerup, 0);
            }
        }
    }
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();

    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(25, Timer, 0);
}

// Handles the Periodic tasks.
void EnemyTimer(int value) {

    if (!isGameDone) {
        Bullet bullet;
        bullet.x = enemyOriginX + translationEnemyX;
        bullet.y = enemyOriginY;
        enemyBullets.push_back(bullet);
        glutPostRedisplay();
    }
    glutTimerFunc(1200, EnemyTimer, 0);
}

void PowerupTimer(int value)
{
    if (!isGameDone) {
        if (!isThereAPowerup)
        {
            powerupX = rand() % 450 + 150;
            powerupY = rand() % 450 + 150;
            isThereAPowerup = true;
            glutTimerFunc(5000, disappearPowerup, 0);
        }
    }
    glutTimerFunc(15000, PowerupTimer, 0);
}

void PowerupTimerTimeout(int value)
{
    if (isThereAPowerup)
    {
        isThereAPowerup = false;
    }
    glutTimerFunc(5000, PowerupTimer, 0);
}

// The Display function for the game
void Display() {

    // Color setup
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);
    // Drawing Spaceship Bullets
    for (auto& bullet : spaceshipBullets)
    {
        drawBullet(bullet.x, bullet.y);
    }
    // Drawing Enemy Bullets
    for (auto& bullet : enemyBullets)
    {
        drawBullet(bullet.x, bullet.y);
    }
    // Drawing Spaceship
    drawSpaceship();
    // Drawing Enemy
    drawEnemy();
    int heartShift = 0;
    for (int i = 0; i < spaceshipLives; i++) {
        drawHeart(100 - heartShift, 650);
        heartShift += 30;
    }
    drawlifeBar(enemyLifeWidth);
    glColor3f(0, 0, 0);

    if (isThereAPowerup)
    {
        drawPowerup(powerupX, powerupY);
    }
    if (isGameDone)
    {
        char* p0s[20];
        glColor3f(0, 0, 0);
        sprintf((char*)p0s, "Click Right button to start");
        print(250, 300, (char*)p0s);
    }
    glFlush();
}

// Handles the keyboard key press
void KeyPressed(unsigned char key, int x, int y)
{
    // Setting moving booleans
    if (key == 'a') a = true;
    if (key == 'd') d = true;
    if (key == 'w') w = true;
    if (key == 's') s = true;

    // Moving the Spaceship
    if (!isGameDone){
        if (a == true && translationSpaceshipX >= -(spaceshipOriginX - 70)) translationSpaceshipX -= 10;
        if (d == true && translationSpaceshipX <= spaceshipOriginX - 70) translationSpaceshipX += 10;
        if (w == true && translationSpaceshipY <= glutGet(GLUT_WINDOW_HEIGHT) - 30) translationSpaceshipY += 10;
        if (s == true && translationSpaceshipY >= -80) translationSpaceshipY -= 10;
    }
    glutPostRedisplay();
}

// Handles the keyboard key release
void KeyReleased(unsigned char key, int x, int y)
{
    // Clearing moving booleans
    if (key == 'a') a = false;
    if (key == 'd') d = false;
    if (key == 'w') w = false;
    if (key == 's') s = false;
    glutPostRedisplay();
}
bool areCollided(int x1, int y1, int w1, int l1, int x2, int y2, int w2, int l2)
{
    bool areCollided = false;
    int maxValueX1 = x1 + w1;
    int maxValueY1 = y1 + l1;
    int maxValueX2 = x2 + w2;
    int maxValueY2 = y2 + l2;
    if (x1 <= x2 && x2 <= maxValueX1 && y1 <= y2 && y2 <= maxValueY1)
    {
        // checks (x2,y2)
        //printf("(x2,y2)");
        areCollided = true;
    }
    if (x1 <= x2 && x2 <= maxValueX1 && y1 <= maxValueY2 && maxValueY2 <= maxValueY1)
    {
        // checks (x2,y2 + l2)
        //printf("(x2,y2 + l2)");
        areCollided = true;
    }
     if (x1 <= maxValueX2 && maxValueX2 <= maxValueX1 && y1 <= y2 && y2 <= maxValueY1)
    {
        // checks (x2 + w2,y2)
        //printf("(x2 + w2,y2)");
        areCollided = true;
    }
    if (x1 <= maxValueX2 && maxValueX2 <= maxValueX1 && y1 <= maxValueY2 && maxValueY2 <= maxValueY1)
    {
        // checks (x2 + w2,y2 + l2)
        //printf("(x2 + w2,y2 + l2)");
        areCollided = true;
    }
    //////////////////////////////////////////////////////////////////////
    if (x2 <= x1 && x1 <= maxValueX2 && y2 <= y1 && y1 <= maxValueY2)
    {
        // checks (x1,y1)
        //printf("(x1,y1)");
        areCollided = true;
    }
    if (x2 <= x1 && x1 <= maxValueX2 && y2 <= maxValueY1 && maxValueY1 <= maxValueY2)
    {
        // checks (x1,y1 + l1)
        //printf("(x1,y1 + l1)");
        areCollided = true;
    }
    if (x2 <= maxValueX1 && maxValueX1 <= maxValueX2 && y2 <= y1 && y1 <= maxValueY2)
    {
        // checks (x1 + w1,y1)
        //printf("(x1 + w1,y1)");
        areCollided = true;
    }
    if (x2 <= maxValueX1 && maxValueX1 <= maxValueX2 && y2 <= maxValueY1 && maxValueY1 <= maxValueY2)
    {
        // checks (x1 + w1,y1 + l1)
        //printf("(x1 + w1,y1 + l1)");
        areCollided = true;
    }
    //printf("\n---------------\n");
    return areCollided;

}

void resetValues()
{
    a, w, s, d = false;
    translationSpaceshipX = 0;
    translationSpaceshipY = 0;
    spaceshipLives = 3;
    translationEnemyX = 0;
    translationEnemyY = 0;
    rightDirection = false;
    enemyLifeWidth = 500;
    spaceshipBullets.clear();
    enemyBullets.clear();
    powerupActive = false;
    isThereAPowerup = false;
    powerupX = 0;
    powerupY = 0;
}

void drawlifeBar(int enemyLifeWidth) {

    glColor3f(0.0f, 0.0f, 0.0f);//Black border of life Bar
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(150, 650, 499, 20);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawRect(149, 650, enemyLifeWidth, 20);
}

void drawHeart(int x, int y) {
    glColor3f(1, 0, 0);
    drawCircle(x - 6, y, 7);
    drawCircle(x + 6, y, 7);
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y - 14);
    glVertex2f(x - 12.5, y - 4);
    glVertex2f(x + 12.5, y - 4);
    glEnd();
}
void print(int x, int y, char* string)
{
    int len, i;

    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    //get the length of the string to display
    len = (int)strlen(string);

    //loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}