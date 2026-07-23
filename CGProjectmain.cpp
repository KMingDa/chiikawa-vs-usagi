#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <GL/glut.h>

#include "CGProjectmain.hpp"
#include "CGProject.hpp"

// Use the correct namespace for your virtual world
using CGProject::MyVirtualWorld;

MyVirtualWorld myvirtualworld;

using namespace std;

MyWindow   window;
MyWorld    world;
MyViewer   viewer;
MySetting  setting;
MyAxis     worldaxis;

bool animateCamera = true;
float animationTime = 0.0f;
float totalAnimationDuration = 3.0f; // in seconds
float animationAngle = 0.0f;

float zoomStartX, zoomStartY, zoomStartZ;
float zoomEndX = 0.0f, zoomEndY = 32.0f, zoomEndZ = 30.0f; // Final zoom position
bool zoomToWinner = false;
float zoomTime;
float zoomDuration = 4.0f;

void renderBitmapString(float x, float y, void *font, const char *string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

bool nightActivated = false;  // false initially (means it's day)
void updateLighting(bool night)
{
    if (night)
    {
        static GLfloat ambientNight[]  = { 0.1f, 0.1f, 0.15f, 1.0f };
        static GLfloat diffuseNight[]  = { 0.5f, 0.5f, 0.7f, 1.0f };
        static GLfloat specularNight[] = { 0.6f, 0.6f, 0.8f, 1.0f };
        static GLfloat positionMoon[]  = { 100.0f, 150.0f, 0.0f, 0.0f };

        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientNight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseNight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularNight);
        glLightfv(GL_LIGHT0, GL_POSITION, positionMoon);

        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 100.0f);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularNight);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 180.0f);
    }
    else
    {
        // Day (sun) directional light settings
        static GLfloat ambientDay[]  = { 0.2f, 0.20f, 0.15f, 1.0f };
        static GLfloat diffuseDay[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
        static GLfloat specularDay[] = { 0.7f, 0.7f, 0.6f, 1.0f };
        static GLfloat positionSun[] = { 100.0f, 150.0f, 0.0f, 0.0f };

        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientDay);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseDay);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularDay);
        glLightfv(GL_LIGHT0, GL_POSITION, positionSun);

        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularDay);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0f);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);
}


void myDisplayFunc(void)
{
    float windowWidth = window.width;
    float windowHeight = window.height;
    //Rotate World
    if (animateCamera) {
        float linearT = animationTime / totalAnimationDuration;
        if (linearT > 1.0f) {
            linearT = 1.0f;
            animateCamera = false;

            myvirtualworld.showHealthBars = true; // Health bars appear after animation

            myvirtualworld.healthBarAnimationProgress = 0.0f; // Start health bar animation
        }

        // Ease-in-out using sine
        float t = 0.5f - 0.5f * cosf(linearT * 3.14159f);

        if (t > 1.0f) {
            t = 1.0f;
            animateCamera = false;
        }

        float startRadius = 250.0f;  // Far away
        float endRadius = 70.0f;     // Current camera radius
        float radius = startRadius + t * (endRadius - startRadius);

        float startHeight = 250.0f;
        float endHeight = viewer.eyeY;
        float height = startHeight + t * (endHeight - startHeight);

        animationAngle = 360.0f * t; // Spin 1 full rotation

        float camX = radius * sinf(animationAngle * 3.14159f / 180.0f);
        float camZ = radius * cosf(animationAngle * 3.14159f / 180.0f);
        float camY = height;

        glLoadIdentity();
        gluLookAt(camX, camY, camZ,
                  viewer.centerX, viewer.centerY, viewer.centerZ,
                  viewer.upX, viewer.upY, viewer.upZ);
    }else if (zoomToWinner) {
        float t = zoomTime / zoomDuration;
        if (t > 1.0f) t = 1.0f;

        // Ease-in-out interpolation
        float smoothT = 0.5f - 0.5f * cosf(t * 3.14159f);

        float camX = zoomStartX + (zoomEndX - zoomStartX) * smoothT;
        float camY = zoomStartY + (zoomEndY - zoomStartY) * smoothT;
        float camZ = zoomStartZ + (zoomEndZ - zoomStartZ) * smoothT;

        glLoadIdentity();
        gluLookAt(camX, camY, camZ,
                  0.0f, 18.0f, 0.0f,  // Look at the center
                  viewer.upX, viewer.upY, viewer.upZ);
    } else {
        glLoadIdentity();
        gluLookAt(viewer.eyeX, viewer.eyeY, viewer.eyeZ,
                  viewer.centerX, viewer.centerY, viewer.centerZ,
                  viewer.upX, viewer.upY, viewer.upZ);
    }

    // Health update logic
    if (myvirtualworld.characters.projectileActive) {
        if (myvirtualworld.characters.checkProjectileCollisionWithChiikawa()) {
            myvirtualworld.characters.healthChikawa -= 0.9f;  // Decrease health
        }
        if (myvirtualworld.characters.checkProjectileCollisionWithUsagi()) {
            myvirtualworld.characters.healthUsagi -= 10.0f;  // Decrease health
        }
    }




    if (myvirtualworld.characters.isChiikawaDefeated || myvirtualworld.characters.isUsagiDefeated) {
        // On win, switch to night mode permanently
        nightActivated = true;
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Night color
    } else {
        // Before win, day mode
        nightActivated = false;
        glClearColor(0.73f, 0.91f, 0.98f, 1.0f); // Day sky blue
    }
    updateLighting(nightActivated);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glPushMatrix();

    // Apply transformations to the world (rotate, scale, etc.)
    glTranslatef(world.posX, world.posY, world.posZ);
    glRotatef(world.rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(world.rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(world.rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(world.scaleX, world.scaleY, world.scaleZ);

    // Draw the world axis (helps visualize rotation and position)
    //worldaxis.draw();

    // Draw the background first
    myvirtualworld.bg.grass();  // Draw grass
    myvirtualworld.bg.pudding();  // Draw pudding
    if (nightActivated) {
        myvirtualworld.bg.moon();  // Draw moon only at night
    } else {
        myvirtualworld.bg.sun();   // Draw sun only at day
    }
    myvirtualworld.bowlsurface.drawbowl();
    myvirtualworld.mountainSurface.drawmountain();
    // Draw the environment elements (trees, bushes, etc.)
    myvirtualworld.bg.draw();
    myvirtualworld.characters.updateAndDrawProjectile();
    myvirtualworld.characters.drawDebugArrowAboveUsagi();
    myvirtualworld.characters.drawDebugArrowAboveChiikawa();

     // Draw the health bars as 3D objects
    if (myvirtualworld.showHealthBars && !myvirtualworld.characters.isChiikawaDefeated && !myvirtualworld.characters.isUsagiDefeated) {

        myvirtualworld.drawHealthBar3D(myvirtualworld.characters.chikawa1X, myvirtualworld.characters.chikawa1Y, myvirtualworld.characters.chikawa1Z, myvirtualworld.characters.healthChikawa, 100.0f, "Chiikawa");

        myvirtualworld.drawHealthBar3D(myvirtualworld.characters.usagiX, myvirtualworld.characters.usagiY, myvirtualworld.characters.usagiZ, myvirtualworld.characters.healthUsagi, 100.0f, "Usagi");

    }

    // Draw the characters (Chiikawa and Usagi)
    myvirtualworld.characters.drawChiikawaAndUsagi();

    glPopMatrix();

    glFlush();   // Send any buffered output to be rendered
        // --- 2D overlay for winner text ---
    if (myvirtualworld.characters.isChiikawaDefeated || myvirtualworld.characters.isUsagiDefeated) {
        const char* winnerText = NULL;

        if (myvirtualworld.characters.isChiikawaDefeated && !myvirtualworld.characters.isUsagiDefeated) {
            winnerText = "USAGI WINS!";
        } else if (myvirtualworld.characters.isUsagiDefeated && !myvirtualworld.characters.isChiikawaDefeated) {
            winnerText = "CHIIKAWA WINS!";
        } else {
            winnerText = "IT'S A DRAW!";
        }

        // Calculate width of the string using GLUT
        void* font = GLUT_BITMAP_TIMES_ROMAN_24;
        int textWidth = 0;
        for (int i = 0; winnerText[i] != '\0'; ++i) {
            textWidth += glutBitmapWidth(font, winnerText[i]);
        }

        // Center the text
        float textX = (windowWidth - textWidth) / 2.0f;
        float textY = windowHeight * 0.95f;

        // Switch to orthographic projection
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, windowWidth, 0, windowHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f);
        renderBitmapString(textX, textY, font, winnerText);
        glEnable(GL_LIGHTING);

        // Restore matrices
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    glutSwapBuffers(); // Swap the buffers (to show everything at once)
    myvirtualworld.tickTime(); // Tick the clock for animation



    //Rotate World
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f; // in seconds
    lastTime = currentTime;

    if (animateCamera) {
        animationTime += deltaTime;
    }


    if (myvirtualworld.showHealthBars && myvirtualworld.healthBarAnimationProgress < 1.0f) {

        myvirtualworld.healthBarAnimationProgress += deltaTime / myvirtualworld.healthBarAnimationDuration;

        if (myvirtualworld.healthBarAnimationProgress > 1.0f) {

            myvirtualworld.healthBarAnimationProgress = 1.0f;

        }

    }





    if (myvirtualworld.characters.isChiikawaDefeated || myvirtualworld.characters.isUsagiDefeated) {
        if (!zoomToWinner) {
            zoomToWinner = true;
            zoomTime = 0.0f;
            // Save current camera position
            zoomStartX = viewer.eyeX;
            zoomStartY = viewer.eyeY;
            zoomStartZ = viewer.eyeZ;
        }


        myvirtualworld.showHealthBars = false; // Hide health bars if someone wins

        myvirtualworld.healthBarAnimationProgress = 0.0f; // Reset animation progress
    }

    if (zoomToWinner) {
        zoomTime += deltaTime;
    }

    glutPostRedisplay(); // Force OpenGL to call myDisplayFunc() again
}
void myReshapeFunc(int width, int height)
{
    window.width  = width;
    window.height = height;
    glViewport(0, 0, width, height);

    // Update the aspect ratio
    viewer.aspectRatio = static_cast<GLdouble>(width) / static_cast<GLdouble>(height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewer.fieldOfView, viewer.aspectRatio, viewer.zNear, viewer.zFar);
    glMatrixMode(GL_MODELVIEW);
}

// Declare movement flags for chikawas and usagi
bool moveForwardChikawa1 = false;
bool moveBackwardChikawa1 = false;
bool moveLeftChikawa1 = false;
bool moveRightChikawa1 = false;

bool moveForwardUsagi = false;   // Changed from chikawa2 to usagi
bool moveBackwardUsagi = false;  // Changed from chikawa2 to usagi
bool moveLeftUsagi = false;      // Changed from chikawa2 to usagi
bool moveRightUsagi = false;     // Changed from chikawa2 to usagi

void myKeyboardFunc(unsigned char key, int x, int y)
{
    // Disable Chiikawa control if defeated
    if (myvirtualworld.characters.isChiikawaDefeated || myvirtualworld.characters.isUsagiDefeated) return;

    float step = 1.0f;
    float angle = 0.0f;

    switch (key)
    {
        case 'w': case 'W':
            angle = 180.0f;
            moveForwardChikawa1 = true;
            myvirtualworld.characters.moveChikawa(0.0f, 0.0f, -step, angle);
            break;
        case 's': case 'S':
            angle = 0.0f;
            moveBackwardChikawa1 = true;
            myvirtualworld.characters.moveChikawa(0.0f, 0.0f, step, angle);
            break;
        case 'a': case 'A':
            angle = 270.0f;
            moveLeftChikawa1 = true;
            myvirtualworld.characters.moveChikawa(-step, 0.0f, 0.0f, angle);
            break;
        case 'd': case 'D':
            angle = 90.0f;
            moveRightChikawa1 = true;
            myvirtualworld.characters.moveChikawa(step, 0.0f, 0.0f, angle);
            break;
        case 27:
            exit(0);
            break;
    }

    switch (key)
    {
        case '+':
        case '=':  // Some keyboards require Shift+= for '+'
            viewer.eyeZ -= 1.0f; // Zoom in
            break;
        case '-':
        case '_':
            viewer.eyeZ += 1.0f; // Zoom out
            break;
    }
    if ((key == 13 || key == 'x' || key == 'X')) {
    // Trigger attacks every time
    if (key == 13) {
        myvirtualworld.characters.attackUsagi = true;
        myvirtualworld.characters.attackAngleUsagi = 0.0f;
    } else {
        myvirtualworld.characters.attackChikawa = true;
        myvirtualworld.characters.attackAngleChikawa = 0.0f;
    }

    // Night activation only once
    if (!nightActivated) {
        nightActivated = true;
    }
}


    glutPostRedisplay();
}
void mySpecialFunc(int key, int x, int y)
{
    // Disable Usagi control if defeated
    if (myvirtualworld.characters.isUsagiDefeated || myvirtualworld.characters.isChiikawaDefeated) return;

    float step = 1.0f;
    float angle = 0.0f;

    switch (key)
    {
        case GLUT_KEY_UP:
            angle = 180.0f;
            moveForwardUsagi = true;
            myvirtualworld.characters.moveUsagi(0.0f, 0.0f, -step, angle);
            break;
        case GLUT_KEY_DOWN:
            angle = 0.0f;
            moveBackwardUsagi = true;
            myvirtualworld.characters.moveUsagi(0.0f, 0.0f, step, angle);
            break;
        case GLUT_KEY_LEFT:
            angle = 270.0f;
            moveLeftUsagi = true;
            myvirtualworld.characters.moveUsagi(-step, 0.0f, 0.0f, angle);
            break;
        case GLUT_KEY_RIGHT:
            angle = 90.0f;
            moveRightUsagi = true;
            myvirtualworld.characters.moveUsagi(step, 0.0f, 0.0f, angle);
            break;

        case GLUT_KEY_HOME:
            myDataInit();
            break;

        case GLUT_KEY_F1:
            setting.shadingMode = !setting.shadingMode;
            if (setting.shadingMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        case GLUT_KEY_F2:
            worldaxis.toggle();
            break;

        case GLUT_KEY_F3:
            GLboolean lightingIsOn;
            glGetBooleanv(GL_LIGHTING, &lightingIsOn);
            if (lightingIsOn == GL_TRUE)
                glDisable(GL_LIGHTING);
            else
                glEnable(GL_LIGHTING);
            break;
    }

    glutPostRedisplay();
}



void myMouseFunc(int button, int state, int x, int y)
{
    y = window.height - y;
    switch (button)
    {
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN && !setting.mouseRightMode)
            {
                setting.mouseX = x;
                setting.mouseY = y;
                setting.mouseRightMode = true;
            }
            if (state == GLUT_UP && setting.mouseRightMode)
            {
                setting.mouseRightMode = false;
            }
            break;
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN && !setting.mouseLeftMode)
            {
                setting.mouseX = x;
                setting.mouseY = y;
                setting.mouseLeftMode = true;
            }
            if (state == GLUT_UP && setting.mouseLeftMode)
            {
                setting.mouseLeftMode = false;
            }
            break;
    }
}

void myMotionFunc(int x, int y)
{
    y = window.height - y;
    GLint xinc = x - setting.mouseX;
    GLint yinc = y - setting.mouseY;

    if (setting.mouseRightMode)
    {
        world.rotate(0.0f, 0.0f, -xinc * 0.5);
    }
    if (setting.mouseLeftMode)
    {
        world.rotate(-yinc * 0.5, xinc * 0.5, 0.0f);
    }

    setting.mouseX = x;
    setting.mouseY = y;
    glutPostRedisplay();
}

void myDataInit()
{
 window.title = "TCG6223 Computer Graphics";
 window.posX = 100;
 window.posY = 100;
 window.width  = 800;
 window.height = 500;

 world.rotateX  = 0.0;
 world.rotateY  = 0.0;
 world.rotateZ  = 0.0;
 world.posX     = 0.0;
 world.posY     = 0.0;
 world.posZ     = 0.0;
 world.scaleX   = 1.0;
 world.scaleY   = 1.0;
 world.scaleZ   = 1.0;

 viewer.eyeX    = 0.0;
 viewer.eyeY    = 0.0;
 viewer.eyeZ    = 40.0;
 viewer.centerX = 0.0;
 viewer.centerY = 0.0;
 viewer.centerZ = 0.0;
 viewer.upX     = 0.0;
 viewer.upY     = 1.0;
 viewer.upZ     = 0.0;
 viewer.zNear   = 0.1;
 viewer.zFar    = 500.0;
 viewer.fieldOfView = 60.0;
 viewer.aspectRatio = static_cast<GLdouble> (window.width) / window.height;

 setting.posInc   = 1.0;
 setting.angleInc = 2.0;
 setting.mouseX   = 0;
 setting.mouseY   = 0;

 setting.mouseRightMode = false;
 setting.mouseLeftMode = false;

 setting.shadingMode = true;
}

void myViewingInit()
{
    // Set the camera to a further distance
    viewer.eyeX    = 0.0f;
    viewer.eyeY    = 20.0f;
    viewer.eyeZ    = 70.0f; // Increased the Z-value to move the camera further away
    viewer.centerX = 0.0f;
    viewer.centerY = 0.0f;

    viewer.centerZ = 0.0f;
    viewer.upX     = 0.0f;
    viewer.upY     = 1.0f;
    viewer.upZ     = 0.0f;
    viewer.zNear   = 0.1f;
    viewer.zFar    = 500.0f;
    viewer.fieldOfView = 60.0f;
    viewer.aspectRatio = static_cast<GLdouble> (window.width) / window.height;

    // Apply the changes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewer.fieldOfView,
                    viewer.aspectRatio,
                    viewer.zNear,
                    viewer.zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewer.eyeX,   viewer.eyeY,   viewer.eyeZ,   // Move the camera further back by increasing eyeZ
               viewer.centerX, viewer.centerY, viewer.centerZ,
               viewer.upX,    viewer.upY,    viewer.upZ );
}


void myLightingInit()
{
    static GLfloat ambient[]  = { 0.2f, 0.20f, 0.15f, 1.0f };  // Slight warm ambient
    static GLfloat diffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };    // Warm sunlight diffuse
    static GLfloat specular[] = { 0.7f, 0.7f, 0.6f, 1.0f };    // Warm specular highlights
    static GLfloat position[] = { 100.0f, 150.0f, 0.0f, 1.0f }; // Sun position (positional light)
    GLfloat shininess = 64.0f;                                  // Moderate shininess

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glEnable(GL_NORMALIZE);
}

void myInit()
{
    myDataInit();

 glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
 glutInitWindowPosition(window.posX, window.posY); // Set top-left position
 glutInitWindowSize(window.width, window.height); //Set width and height
 glutCreateWindow(window.title.c_str());// Create display window

 glutDisplayFunc(myDisplayFunc);  // Specify the display callback function
 glutReshapeFunc(myReshapeFunc);
 glutKeyboardFunc(myKeyboardFunc);
 glutSpecialFunc(mySpecialFunc);
 glutMotionFunc(myMotionFunc);
 glutMouseFunc(myMouseFunc);

 glPointSize(4.0);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LESS);
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 glFrontFace(GL_CCW);
 glShadeModel (GL_SMOOTH);
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

 glEnable(GL_CULL_FACE);


 myViewingInit();

 myLightingInit();

 myvirtualworld.init();

}



void myWelcome()
{
    cout << "*****************************************************************\n";
    cout << "*                   TCG6223 Computer Graphics                   *\n";
    cout << "*                  FIST, Multimedia University                  *\n";
    cout << "*****************************************************************\n";
    cout << "| Press:                                                        |\n";
    cout << "|   <a>,<d>,<w>,<s>,<q>,<e> => move world                       |\n";
    cout << "|   <arrows>                => rotate world                     |\n";
    cout << "|   HOME                    => restore defaults                 |\n";
    cout << "|   ESC                     => exit                             |\n";
    cout << "|                                                               |\n";
    cout << "|   F1                      => toggle shading / wire-frame mode |\n";
    cout << "|   F2                      => toggle rendering of axes         |\n";
    cout << "|   F3                      => toggle lighting on / off         |\n";
    cout << "|                                                               |\n";
    cout << "| Mouse (Left Drag or Right Drag) => rotate world               |\n";
    cout << "|                                                               |\n";
    cout << "*****************************************************************\n";
    cout << "|                      H A V E   F U N  !!!                     |\n";
    cout << "*****************************************************************\n";
}

//--------------------------------------------------------------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);


    myWelcome();

    myInit();

    glutMainLoop(); // Display everything and wait
}
//--------------------------------------------------------------------
