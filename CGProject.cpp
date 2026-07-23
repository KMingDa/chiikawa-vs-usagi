#include <GL/glut.h>
#include <string>
#include <fstream>
#include <cmath> // Include for M_PI and sinf/cosf
#include "CGProject.hpp"

bool someoneWon;

using namespace CGProject;
MyVirtualWorld::MyVirtualWorld()
{
    numberOfTexture = 0;
    textureNumbers = NULL;
    textureison = false;
    showHealthBars = false; // Initialize to false
    healthBarAnimationProgress = 0.0f; // Initialize animation progress
}

extern float healthChikawa;
extern float healthUsagi;

MyVirtualWorld::~MyVirtualWorld()
{
    if (textureNumbers != NULL)
    delete[] textureNumbers;
}

void MyVirtualWorld::setupTextures()
{
    numberOfTexture = 11;
    textureNumbers = new GLuint[numberOfTexture];
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glGenTextures(numberOfTexture, textureNumbers);

    MyImage myimage[numberOfTexture];
    myimage[0].loadJPG("images/grass.jpg");
    myimage[1].loadJPG("images/trunk.jpg");
    myimage[2].loadJPG("images/leaves.jpg");
    myimage[3].loadJPG("images/rock.jpg");
    myimage[4].loadJPG("images/leaves2.jpg");
    myimage[5].loadJPG("images/Chiikawa_neutral.jpg");
    myimage[6].loadJPG("images/Chiikawa_hurt.jpg");
    myimage[7].loadJPG("images/Chiikawa_win.jpg");
    myimage[8].loadJPG("images/Usagi_neutral.jpg");
    myimage[9].loadJPG("images/Usagi_hurt.jpg");
    myimage[10].loadJPG("images/Usagi_win.jpg");

    for (int i = 0; i < numberOfTexture; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureNumbers[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     myimage[i].width, myimage[i].height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, myimage[i].buffer);
    }

    bg.setTextures(textureNumbers);
    characters.setTextures(textureNumbers);
}

void Background::setTextures(GLuint* textures) {
    textureNumbers = textures;
}

void Characters::setTextures(GLuint* textures) {
    textureNumbers = textures;
}

bool Characters::checkCharacterCollision() {
    return (chiikawaBox.xMin <= usagiBox.xMax && chiikawaBox.xMax >= usagiBox.xMin) &&
           (chiikawaBox.yMin <= usagiBox.yMax && chiikawaBox.yMax >= usagiBox.yMin) &&
           (chiikawaBox.zMin <= usagiBox.zMax && chiikawaBox.zMax >= usagiBox.zMin);
}

void Characters::updateBoundingBoxes() {
    float sizeX = 9.0f, sizeY = 10.0f, sizeZ = 5.0f;

    chiikawaBox.xMin = chikawa1X - sizeX / 2.0f;
    chiikawaBox.xMax = chikawa1X + sizeX / 2.0f;
    chiikawaBox.yMin = chikawa1Y;
    chiikawaBox.yMax = chikawa1Y + sizeY;
    chiikawaBox.zMin = chikawa1Z - sizeZ / 2.0f;
    chiikawaBox.zMax = chikawa1Z + sizeZ / 2.0f;

    usagiBox.xMin = usagiX - sizeX / 2.0f;
    usagiBox.xMax = usagiX + sizeX / 2.0f;
    usagiBox.yMin = usagiY;
    usagiBox.yMax = usagiY + sizeY;
    usagiBox.zMin = usagiZ - sizeZ / 2.0f;
    usagiBox.zMax = usagiZ + sizeZ / 2.0f;
}


void Characters::drawSingleChikawa(float r, float g, float b)
{
    bool chiikawaWon = isUsagiDefeated && !isChiikawaDefeated;

    glEnable(GL_LIGHTING);
    Weapon bident;
    glDisable(GL_CULL_FACE);
    GLUquadricObj *pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);

    // Right Leg
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(-3.0f, -3.0f, 0.0f);
    glRotatef(-270.0f - legSwingAngleChikawa1, 1.0f, 0.0f, 0.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glPopMatrix();

    // Left Leg
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(3.0f, -3.0f, .0f);
    glRotatef(-270.0f + legSwingAngleChikawa1, 1.0f, 0.0f, 0.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glPopMatrix();

    // Body
    glPushMatrix();
    glColor3f(r, g, b);
    glScalef(1.25f, 1.0f, 1.0f);
    glTranslatef(0.0f, bounceOffset / 2 + 0.0f, 0.0f);
    gluSphere(pObj, 5.5f, 36, 24);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, -2.5f, -5.5f);
    gluSphere(pObj, 1.0f, 36, 24);
    glPopMatrix();

    // Right Hand
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(6.0f, bounceOffset + 0.0f, 0.0f);
    if (chiikawaWon) {
        glRotatef(-40.0f, 1.0f,  bounceAmplitude * sin(bounceTimeChikawa1), 0.0f);  // Raise left arm too
    }
    if (attackInProgress2 || attackAngleChikawa > 0.0f) {
        float angleRadians = attackAngleChikawa * 2.0f * 3.1415926f;
        float easedAngle = 60.0f * sinf(angleRadians);
        glRotatef(easedAngle, 110.0f, -120.0f, 10.0f);
    }
    glRotatef(120.0f, 1.0f, -bounceOffset / 2, 1.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, bounceOffset / 2 + 0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glRotatef(270.0f, 0.0f, 0.0f, 180.0f);
    glTranslatef(0.0f, 12.0f, 0.0f);
    bident.drawChiikawaWeapon();
    glPopMatrix();

    // Left Hand
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(-6.0f, bounceOffset + 0.0f, 0.0f);
     if (chiikawaWon) {
        glTranslatef(-0.7f, 0.0f, 0.0f);
        glRotatef(-70.0f, 1.0f,  -bounceAmplitude * sin(bounceTimeChikawa1), 0.0f);  // Raise left arm too
    }
    glRotatef(-90.0f, -1.0f, -bounceOffset / 2, 1.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, bounceOffset / 2 + 0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glPopMatrix();

    // Head
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(0.0f, bounceOffset + 6.0f, 0.0f);
    glScalef(1.25f, 1.0f, 1.0f);
    gluSphere(pObj, 7.5f, 36, 24);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    int faceTextureIndex = 5; //default
    if (chiikawaHit) {
        faceTextureIndex = 6; //hurt
    } else if (chiikawaWon) {
        faceTextureIndex = 7; //win
    }
    glBindTexture(GL_TEXTURE_2D, textureNumbers[faceTextureIndex]);


    float radius = 7.5f;
    int stacks = 16; // vertical divisions
    int slices = 32; // horizontal divisions

    // Transform to match the head
    glTranslatef(0.0f, bounceOffset + 6.0f, 0.1f);
    glScalef(1.25f, 1.0f, 1.0f); // match head scale

    // Limit vertical and horizontal angles to a small center patch
    float verticalStart = M_PI / 3.8f;        // ~47.4°
    float verticalEnd   = 1.7f * M_PI / 3.0f; // ~102°

    float horizontalStart = M_PI / 3.6f;         // ~50°
    float horizontalEnd   = 2.4f * M_PI / 3.0f;  // ~144°

    for (int i = 0; i < stacks; ++i) {
        float theta1 = verticalStart + (verticalEnd - verticalStart) * i / stacks;
        float theta2 = verticalStart + (verticalEnd - verticalStart) * (i + 1) / stacks;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float phi = horizontalStart + (horizontalEnd - horizontalStart) * j / slices;

            for (int k = 1; k >= 0; --k) {
        float theta = (k == 0) ? theta1 : theta2;
        float x = sin(theta) * cos(phi);
        float y = cos(theta);
        float z = sin(theta) * sin(phi);

        float u = (phi - horizontalStart) / (horizontalEnd - horizontalStart);
        float v = (theta - verticalStart) / (verticalEnd - verticalStart);

        glTexCoord2f(u, v);
        glNormal3f(x, y, z);
        glVertex3f(radius * x, radius * y, radius * z);
    }

        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    // Right Ear
    glPushMatrix();
    glColor3f(r, g, b);
    glRotatef(15.0, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, bounceOffset + 13.5f, 0.0f);
    gluSphere(pObj, 1.5f, 36, 24);
    glPopMatrix();

    // Left Ear
    glPushMatrix();
    glColor3f(r, g, b);
    glRotatef(-15.0, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, bounceOffset + 13.5f, 0.0f);
    gluSphere(pObj, 1.5f, 36, 24);
    glPopMatrix();

    gluDeleteQuadric(pObj);
    glEnable(GL_CULL_FACE);
}

void Characters::drawUsagi(float r, float g, float b)
{
    bool usagiWon = isChiikawaDefeated && !isUsagiDefeated;
    glEnable(GL_LIGHTING);
    Weapon wand;
    glDisable(GL_CULL_FACE);
    GLUquadricObj *pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);

    //Right Ear
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(-2.0f, bounceOffset +12.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, bounceOffset/10 + 0.0f, 0.0f);
    gluCylinder(pObj, 1.5f, 1.5f, 6.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 6.5f);
    gluSphere(pObj, 1.5f, 36, 24);
    glPopMatrix();

    //Left Ear
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(2.0f, bounceOffset +12.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, -bounceOffset/10 + 0.0f, 0.0f);
    gluCylinder(pObj, 1.5f, 1.5f, 6.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 6.5f);
    gluSphere(pObj, 1.5f, 36, 24);
    glPopMatrix();

    //Head
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(0.0f, bounceOffset +6.0f, 0.0f);
    glScalef(1.25f, 1.0f, 1.0f);
    gluSphere(pObj, 7.5f, 36, 24);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    int faceTextureIndex = 8; // neutral by default
    if (usagiHit) {
        faceTextureIndex = 9; // hurt
    } else if (usagiWon) {
        faceTextureIndex = 10; // winning expression
    }
    glBindTexture(GL_TEXTURE_2D, textureNumbers[faceTextureIndex]);

    float radius = 7.5f;
    int stacks = 16; // vertical divisions
    int slices = 32; // horizontal divisions

    // Transform to match the head
    glTranslatef(0.0f, bounceOffset + 6.0f, 0.1f);
    glScalef(1.25f, 1.0f, 1.0f); // match head scale

    // Limit vertical and horizontal angles to a small center patch
    float verticalStart = M_PI / 3.8f;        // ~47.4°
    float verticalEnd   = 1.7f * M_PI / 3.0f; // ~102°

    float horizontalStart = M_PI / 3.6f;         // ~50°
    float horizontalEnd   = 2.4f * M_PI / 3.0f;  // ~144°

    for (int i = 0; i < stacks; ++i) {
        float theta1 = verticalStart + (verticalEnd - verticalStart) * i / stacks;
        float theta2 = verticalStart + (verticalEnd - verticalStart) * (i + 1) / stacks;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float phi = horizontalStart + (horizontalEnd - horizontalStart) * j / slices;

            for (int k = 1; k >= 0; --k) {
        float theta = (k == 0) ? theta1 : theta2;
        float x = sin(theta) * cos(phi);
        float y = cos(theta);
        float z = sin(theta) * sin(phi);

        float u = (phi - horizontalStart) / (horizontalEnd - horizontalStart);
        float v = (theta - verticalStart) / (verticalEnd - verticalStart);

        glTexCoord2f(u, v);
        glNormal3f(x, y, z);
        glVertex3f(radius * x, radius * y, radius * z);
    }

        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    //Body
    glPushMatrix();
    glColor3f(r, g, b);
    glScalef(1.25f, 1.0f, 1.0f);
    glTranslatef(0.0f, bounceOffset/2 + 0.0f, 0.0f);
    gluSphere(pObj, 5.5f, 36, 24);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, -2.5f, -5.5f);
    gluSphere(pObj, 1.0f, 36, 24);
    glPopMatrix();

    //Right Hand
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(6.0f, bounceOffset +0.0f, 0.0f);

    if (usagiWon) {
        glRotatef(-40.0f, 1.0f,  bounceAmplitude * sin(bounceTimeChikawa1), 0.0f);  // Raise left arm too
    }

    if (attackInProgress || (attackAngleUsagi > 0.0f && attackAngleUsagi <= 1.0f)) {
        float easedAngle = 0.0f;
        float t = attackAngleUsagi;
        if (t < 0.1f) {
            float phaseT = t / 0.1f;
            easedAngle = 60.0f * sinf(phaseT * (3.1415926f / 2.0f));
            hasFiredProjectile = false;
        } else if (t < 0.9f) {
            easedAngle = 60.0f;
            if (!hasFiredProjectile) {
                fireProjectileFromUsagi();
                hasFiredProjectile = true;
            }
        } else {
            float phaseT = (t - 0.9f) / 0.1f;
            easedAngle = 60.0f * cosf(phaseT * (3.1415926f / 2.0f));
        }
        glRotatef(easedAngle, 110.0f, -120.0f, 1.0f);
    }

    glRotatef(90.0f, 1.0f, -bounceOffset/2, 1.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, bounceOffset/2 +0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);

    glRotatef(90.0f, 0.0f, 0.0f, 180.0f);
    glTranslatef(13.0f,  -9.0f, 0.0f);
    wand.drawUsagiWeapon();
    glPopMatrix();

    //Left Hand
    glPushMatrix();
    glColor3f(r, g, b);
     if (usagiWon) {
        glTranslatef(-0.7f, 0.0f, 0.0f);
        glRotatef(-70.0f, 1.0f,  -bounceAmplitude * sin(bounceTimeChikawa1), 0.0f);  // Raise left arm too
    }
    glTranslatef(-6.0f, bounceOffset + 0.0f, 0.0f);
    glRotatef(-90.0f, -1.0f, -bounceOffset/2, 1.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, bounceOffset/2 +0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glPopMatrix();

    //Right Leg
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(-3.0f, -3.0f, 0.0f);
    glRotatef(-270.0f+legSwingAngleUsagi1, 1.0f, 0.0f, 0.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glPopMatrix();

    //Left Leg
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(3.0f, -3.0f, 0.0f);
    glRotatef(-270.0f-legSwingAngleUsagi1, 1.0f, 0.0f, 0.0f);
    gluCylinder(pObj, 0.8f, 0.8f, 3.0f, 36, 24);
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluSphere(pObj, 0.8f, 36, 24);
    glPopMatrix();

    gluDeleteQuadric(pObj);
    glEnable(GL_CULL_FACE);
}

void Characters::drawChiikawaAndUsagi()
{
    bool chiikawaWon = isUsagiDefeated && !isChiikawaDefeated;
    bool usagiWon = isChiikawaDefeated && !isUsagiDefeated;
    someoneWon = chiikawaWon || usagiWon;

    // Initialize confetti only once when someone wins
    if (someoneWon && !confettiInitialized) {
        initConfetti(0.0f, 20.0f, 0.0f); // Drop around center (or average of both positions)
    }

    static float animationTime = 0.0f;
    static float pauseTimer = 0.0f;
    static bool isPaused = false;
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    float bounceAmplitude = 5.0f;
    float bounceSpeed = 2.1f; // cycles per second
    float cycleTime = 1.0f / bounceSpeed;

    static float lastBouncePhase = 0.0f;
    float t = fmod(animationTime * bounceSpeed, 1.0f); // Normalized 0–1

    // Detect bottom of bounce (t wraps from near 1 back to 0)
    if (!isPaused && lastBouncePhase > 0.95f && t < 0.05f) {
        isPaused = true;
        pauseTimer = 0.0f;
    }
    lastBouncePhase = t;

    if (isPaused) {
        pauseTimer += deltaTime;
        if (pauseTimer >= 0.15f) {  // Pause duration in seconds
            isPaused = false;
        }
    } else {
        animationTime += deltaTime;
    }

    float eased = 0.5f * (1.0f - cosf(t * 2.0f * 3.14159265f)); // starts and ends at 0
    eased = powf(eased, 0.3f);

    float bounce = eased * bounceAmplitude;

    if (isChiikawaDefeated || isUsagiDefeated) {
        bounceOffset = 0.0f;
    }

    // Draw Chiikawa
    glPushMatrix();
    glTranslatef(chikawa1X, chikawa1Y + 3.3f + (chiikawaWon  ? bounce : 0.0f), chikawa1Z);
    glRotatef(currentRotationChikawa1, 0.0f, 1.0f, 0.0f);
    glRotatef(chiikawaFallAngle, 1.0f, 0.0f, 0.0f);
    if(isChiikawaDefeated){
        currentRotationUsagi1 = 0.0f;
    }
    glScalef(0.5f, 0.5f, 0.5f);

    if (chiikawaInAir) {
        drawSingleChikawa(1.0f, 0.7f, 0.7f);
    } else {
        drawSingleChikawa(1.0f, 1.0f, 1.0f);
    }
    glPopMatrix();

    if (chiikawaInAir) {
        glPushMatrix();
        drawHitEffect(chikawa1X, chikawa1Y + bounceOffset, chikawa1Z, hitEffectTimeChikawa);
        glPopMatrix();
    }

    updateAndDrawExplosion(deltaTime);

    // Draw Usagi
    glPushMatrix();
    glTranslatef(usagiX, usagiY + 3.3f + (usagiWon ? bounce : 0.0f), usagiZ);
    glRotatef(currentRotationUsagi1, 0.0f, 1.0f, 0.0f);
    glRotatef(usagiFallAngle, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);

    if(isUsagiDefeated){
        currentRotationChikawa1 = 0.0f;
    }

    if (usagiInAir) {
        drawUsagi(0.988f, 0.649f, 0.612f);
    } else {
        drawUsagi(0.988f, 0.949f, 0.812f);
    }
    glPopMatrix();

    if (usagiInAir) {
        glPushMatrix();
        drawHitEffect(usagiX, usagiY + bounceOffset, usagiZ, hitEffectTimeUsagi);
        glPopMatrix();
    }

    if (someoneWon) {
        updateAndDrawConfetti(deltaTime);
    }
    updateBoundingBoxes();
}

void Characters::updateAndDrawConfetti(float deltaTime) {
    // Spawn one new confetti every second
    confettiSpawnTimer += deltaTime;
    if (confettiSpawnTimer >= 1.0f && currentConfettiIndex < MAX_CONFETTI) {
        confetti[currentConfettiIndex].active = true;
        currentConfettiIndex++;
        confettiSpawnTimer = 0.0f;
    }

    // Update and draw active confetti
    for (int i = 0; i < MAX_CONFETTI; ++i) {
        confetti[i].rotX += confetti[i].rotSpeedX * deltaTime * 60.0f;
        confetti[i].rotY += confetti[i].rotSpeedY * deltaTime * 60.0f;
        confetti[i].rotZ += confetti[i].rotSpeedZ * deltaTime * 60.0f;

        if (confetti[i].active) {
            confetti[i].y -= confetti[i].velocityY * deltaTime * 20.0f;

            // Optional: respawn confetti for a continuous stream
            if (confetti[i].y < 0.0f) {
                confetti[i].y = (rand() % 100) / 10.0f + 10.0f;
                confetti[i].x = (rand() % 200 - 100) / 10.0f;
                confetti[i].z = (rand() % 200 - 100) / 10.0f;
                confetti[i].velocityY = ((rand() % 30) / 100.0f) + 0.5f;
            }

            glPushMatrix();
            glColor3f(confetti[i].r, confetti[i].g, confetti[i].b);
            glTranslatef(confetti[i].x, confetti[i].y, confetti[i].z);
            glPushMatrix();
            glColor3f(confetti[i].r, confetti[i].g, confetti[i].b);
            glTranslatef(confetti[i].x, confetti[i].y, confetti[i].z);

            // Apply rotations
            glRotatef(confetti[i].rotX, 1.0f, 0.0f, 0.0f);
            glRotatef(confetti[i].rotY, 0.0f, 1.0f, 0.0f);
            glRotatef(confetti[i].rotZ, 0.0f, 0.0f, 1.0f);

            // Make it a flat rectangular strip
            glScalef(0.4f, 0.02f, 1.0f); // Width, thinness, height
            glutSolidCube(1.0f); // Base cube size is 1.0 scaled down
            glPopMatrix();

            glPopMatrix();
        }
    }
}


void Characters::initConfetti(float centerX, float centerY, float centerZ) {

    for (int i = 0; i < MAX_CONFETTI; ++i) {
        confetti[i].rotX = rand() % 360;
        confetti[i].rotY = rand() % 360;
        confetti[i].rotZ = rand() % 360;

        confetti[i].rotSpeedX = (rand() % 50) / 10.0f; // 0–5 deg/frame
        confetti[i].rotSpeedY = (rand() % 50) / 10.0f;
        confetti[i].rotSpeedZ = (rand() % 50) / 10.0f;
        confetti[i].x = centerX + (rand() % 200 - 100) / 10.0f; // spread x
        confetti[i].y = centerY + (rand() % 100) / 10.0f + 10.0f; // start above
        confetti[i].z = centerZ + (rand() % 200 - 100) / 10.0f; // spread z
        confetti[i].velocityY = ((rand() % 30) / 100.0f) + 0.5f;
        confetti[i].r = (rand() % 100) / 100.0f;
        confetti[i].g = (rand() % 100) / 100.0f;
        confetti[i].b = (rand() % 100) / 100.0f;
        confetti[i].active = true;
    }
    confettiInitialized = true;
}


void Characters::drawDebugArrowAboveUsagi() {
    if (isUsagiDefeated || isChiikawaDefeated) return;
    glDisable(GL_LIGHTING);
    GLUquadricObj* quad = gluNewQuadric();
    if (!quad) return;

    glPushMatrix();
    glTranslatef(usagiX, usagiY+bounceOffset + 18.0f, usagiZ);
    glColor3f(1.0f, 0.8f, 0.0f);

    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.8f, 0.8f, 3.0f, 16, 1);
    gluDisk(quad, 0.0f, 0.8f, 16, 1);
    gluDisk(quad, 0.0f, 1.5f, 16, 1);
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluDisk(quad, 0.0f, 0.8f, 16, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 1.5f, 0.0f, 3.0f, 16, 1);
    gluDisk(quad, 0.0f, 1.5f, 16, 1);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Characters::drawDebugArrowAboveChiikawa() {
    if (isUsagiDefeated || isChiikawaDefeated) return;
    glDisable(GL_LIGHTING);
    GLUquadricObj* quad = gluNewQuadric();
    if (!quad) return;

    glPushMatrix();
    glTranslatef(chikawa1X, chikawa1Y + bounceOffset + 18.0, chikawa1Z);
    glColor3f(0.976f, 0.61f, 0.673f);

    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.8f, 0.8f, 3.0f, 16, 1);
    gluDisk(quad, 0.0f, 0.8f, 16, 1);
    gluDisk(quad, 0.0f, 1.5f, 16, 1);
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluDisk(quad, 0.0f, 0.8f, 16, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 1.5f, 0.0f, 3.0f, 16, 1);
    gluDisk(quad, 0.0f, 1.5f, 16, 1);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Characters::fireProjectileFromUsagi() {
    if (isUsagiDefeated || isChiikawaDefeated) return;

    if (!projectileActive) {
        // Compute projectile direction
        float adjustedRotation = currentRotationUsagi1;
        if (fabs(fmod(currentRotationUsagi1, 360.0f) - 0.0f) < 1.0f || fabs(fmod(currentRotationUsagi1, 360.0f) - 180.0f) < 1.0f) {
            adjustedRotation += 180.0f;
        }

        float theta = adjustedRotation * (3.14159265f / 180.0f);
        float dirX = sinf(theta);
        float dirZ = -cosf(theta);

        // Set projectile starting position and velocity
        projectileX = usagiX + dirX * 9.0f;
        projectileY = usagiY + 4.1f;
        projectileZ = usagiZ + dirZ * 9.0f;
        float speed = 0.5f;
        projectileVelX = dirX * speed;
        projectileVelZ = dirZ * speed;
        projectileActive = true;

        // 🔥 Clear previous trail and reset spiral timer
        projectileTrail.clear();
        projectileTrailTime = 0.0f;
    }
}


void Characters::updateAndDrawProjectile() {
    if (!projectileActive) return;

    // Check collision
    float dx = projectileX - chikawa1X;
    float dy = projectileY - chikawa1Y;
    float dz = projectileZ - chikawa1Z;
    float distance = sqrt(dx*dx + dy*dy + dz*dz);

    if (distance < 7.0f) {
        chiikawaHit = true;
        playHitEffectChikawa = true;
        hitEffectTimeChikawa = 0.0f;
        projectileActive = false;
        explosionActive = true;
        explosionSmoke.clear();
        explosionTime = 0.0f;
        explosionX = chikawa1X;
        explosionY = chikawa1Y + 3.3f;
        explosionZ = chikawa1Z;
        healthChikawa -= 15.0f;

        float knockbackDirection = 0.0f;
        if (fabs(fmod(currentRotationUsagi1, 360.0f) - 0.0f) < 1.0f || fabs(fmod(currentRotationUsagi1, 360.0f) - 180.0f) < 1.0f) {
            knockbackDirection = 180.0f;
        }

        float theta = (currentRotationUsagi1 + knockbackDirection) * (3.14159265f / 180.0f);
        float dirX = sinf(theta);
        float dirZ = -cosf(theta);
        chiikawaAirVelX = dirX * 8.0f;
        chiikawaAirVelZ = dirZ * 8.0f;
        chiikawaInAir = true;
        chiikawaHitTime = 0.0f;
        chiikawaInitialY = chikawa1Y;
    }

    // Move projectile
    projectileX += projectileVelX;
    projectileZ += projectileVelZ;

    GLUquadricObj* pObj = gluNewQuadric();
    if (!pObj) return;

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(projectileX, projectileY, projectileZ);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Random flicker values for color and size
    float coreFlicker = 0.85f + static_cast<float>(rand()) / RAND_MAX * 0.15f;
    float shell1AlphaFlicker = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.3f;
    float shell2AlphaFlicker = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.2f;

    float coreSizeFlicker = 1.45f + static_cast<float>(rand()) / RAND_MAX * 1.5f;   // ~1.45 - 1.65
    float shell1SizeFlicker = 2.1f + static_cast<float>(rand()) / RAND_MAX * 1.1f;  // ~2.1 - 2.4
    float shell2SizeFlicker = 2.9f + static_cast<float>(rand()) / RAND_MAX * 0.9f;  // ~2.9 - 3.2

    // Core (flickering size and brightness)
    glColor4f(1.0f * coreFlicker, 0.6f * coreFlicker, 0.0f, 1.0f);
    gluSphere(pObj, coreSizeFlicker, 36, 24);

    // First shell (semi-transparent red, flickering size & alpha)
    glColor4f(1.0f, 0.3f, 0.3f, shell1AlphaFlicker);
    gluSphere(pObj, shell1SizeFlicker, 36, 24);

    // Second shell (transparent pink, flickering size & alpha)
    glColor4f(1.0f, 0.6f, 0.6f, shell2AlphaFlicker);
    gluSphere(pObj, shell2SizeFlicker, 36, 24);

    glDisable(GL_BLEND);
    glPopMatrix();
    gluDeleteQuadric(pObj);


    // --- SPIRAL PARTICLE EMISSION ---
    projectileTrailTime += 0.1f;
    const int spiralCount = 3;
    const float spiralRadius = 2.0f;

    for (int i = 0; i < spiralCount; ++i) {
        float angle = projectileTrailTime + i * (2.0f * 3.14159265f / spiralCount);
        float offsetX = cosf(angle) * spiralRadius;
        float offsetZ = sinf(angle) * spiralRadius;

        Particle p;
        p.x = projectileX + offsetX;
        p.y = projectileY;
        p.z = projectileZ + offsetZ;
        p.r = 1.0f;
        p.g = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.3f;
        p.b = 0.0f;
        p.a = 1.0f;
        p.life = 1.0f;
        projectileTrail.push_back(p);
    }

    // --- PARTICLE RENDERING ---
    glEnable(GL_BLEND);
    // --- PARTICLE RENDERING WITH SHRINKING SIZE ---
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

for (int i = 0; i < projectileTrail.size(); ) {
    Particle& p = projectileTrail[i];
    p.life -= 0.05f;
    p.a = p.life;
    p.y -= 0.05f;

    if (p.life <= 0.0f) {
        projectileTrail.erase(projectileTrail.begin() + i);
        continue;
    }

    // Particle size decreases with life
    float size = 20.0f * p.life;  // Max size 20, shrinks to 0
    glPointSize(size);

    glBegin(GL_POINTS);
    glColor4f(p.r, p.g, p.b, p.a);
    glVertex3f(p.x, p.y, p.z);
    glEnd();

    ++i;
}

glDisable(GL_BLEND);
glEnable(GL_LIGHTING);


    // Offscreen cleanup
    if (fabs(projectileX) > 100.0f || fabs(projectileZ) > 100.0f)
        projectileActive = false;
}


void Characters::drawHitEffect(float x, float y, float z, float timeElapsed) {
    int numPyramids = 6;
    float t = std::min(timeElapsed, 2.0f);
    float eased = sinf((t * 3.14159265f) / 2.0f);
    float radius = eased * 15.0f;
    float size = 1.0f - eased;
    if (size < 0.0f) size = 0.0f;

    glPushMatrix();
    glTranslatef(x, y + 5.0f, z);

    for (int i = 0; i < numPyramids; ++i) {
        float angle = (360.0f / numPyramids) * i + timeElapsed * 300.0f;
        float rad = angle * (3.14159265f / 180.0f);
        float tx = cosf(rad) * radius;
        float tz = sinf(rad) * radius;
        float maxHeight = 5.0f;
        float fallY = maxHeight * (1.0f - eased);

        glPushMatrix();
        glTranslatef(tx, fallY, tz);
        float spinX = timeElapsed * 1080.0f;
        glRotatef(spinX, 0.2f, 0.0f, 0.2f);
        draw3DStar(size, 0.3f * size, 5, timeElapsed * 720.0f);
        glPopMatrix();
    }

    glPopMatrix();
}

void Characters::draw3DStar(float size, float thickness, int points, float rotationDegrees) {
    struct Vertex {
        float x, y, z;
    };

    Vertex frontVertices[20];  // 5-point star → max 10 points, doubled = 20 max
    Vertex backVertices[20];

    float outerRadius = 4.0f * size;
    float innerRadius = 2.0f * size;

    for (int i = 0; i < points * 2; ++i) {
        float angle = (float)i * M_PI / points;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float vx = radius * sin(angle);
        float vz = radius * cos(angle);
        frontVertices[i] = { vx, thickness / 2, vz };
        backVertices[i] = { vx, -thickness / 2, vz };
    }

    glRotatef(rotationDegrees, 0, 1, 0);
    glColor4f(1.0f, 0.8f, 0.0f, 1.0f); // Yellow star

    // Front face
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, thickness / 2, 0.0f);
    for (int i = 0; i <= points * 2; ++i) {
        int idx = i % (points * 2);
        glVertex3f(frontVertices[idx].x, frontVertices[idx].y, frontVertices[idx].z);
    }
    glEnd();

    // Back face
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -thickness / 2, 0.0f);
    for (int i = points * 2; i >= 0; --i) {
        int idx = i % (points * 2);
        glVertex3f(backVertices[idx].x, backVertices[idx].y, backVertices[idx].z);
    }
    glEnd();

    // Sides
    glBegin(GL_QUADS);
    for (int i = 0; i < points * 2; ++i) {
        int next = (i + 1) % (points * 2);
        glVertex3f(frontVertices[i].x, frontVertices[i].y, frontVertices[i].z);
        glVertex3f(frontVertices[next].x, frontVertices[next].y, frontVertices[next].z);
        glVertex3f(backVertices[next].x, backVertices[next].y, backVertices[next].z);
        glVertex3f(backVertices[i].x, backVertices[i].y, backVertices[i].z);
    }
    glEnd();
}


float easeOutSine(float t) {
    return sinf(t * (M_PI / 2.0f));
}

void Characters::updateAndDrawExplosion(float deltaTime) {
    glDisable(GL_LIGHTING);
    if (!explosionActive) return;
    explosionTime += deltaTime;

    // Emit new smoke particles
    for (int i = 0; i < 4; ++i) {
        SmokeParticle sp;
        sp.x = explosionX + ((rand() % 100) / 100.0f - 0.5f) * 4.0f;
        sp.y = explosionY;
        sp.z = explosionZ + ((rand() % 100) / 100.0f - 0.5f) * 4.0f;
        sp.vx = ((rand() % 100) / 100.0f - 0.5f) * 0.2f;
        sp.vy = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
        sp.vz = ((rand() % 100) / 100.0f - 0.5f) * 0.2f;
        sp.size = 1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
        sp.alpha = 0.6f;
        sp.life = 1.0f;
        explosionSmoke.push_back(sp);
    }


    if (explosionTime > explosionDuration) {
        explosionActive = false;
        return;
    }

    GLUquadricObj* pObj = gluNewQuadric();
    if (!pObj) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glPushMatrix();
    float x = explosionX, y = explosionY, z = explosionZ;

    float t1 = explosionTime - 0.0f;
    if (t1 >= 0.0f) {
        float p1 = easeOutSine(t1 / explosionDuration);
        float radius1 = 1.0f + 5.0f * p1, alpha1 = 1.0f - (t1 / explosionDuration);
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor4f(1.0f, 1.0f, 0.8f, alpha1);
        gluSphere(pObj, radius1, 36, 24);
        glPopMatrix();
    }

    float t2 = explosionTime - 0.1f;
    if (t2 >= 0.0f) {
        float p2 = easeOutSine(t2 / explosionDuration);
        float radius2 = 1.5f + 6.0f * p2, alpha2 = 0.7f * (1.0f - (t2 / explosionDuration));
        glPushMatrix();
        glTranslatef(x + 0.5f, y + 0.2f, z - 4.3f);
        glColor4f(1.0f, 0.5f, 0.0f, alpha2);
        gluSphere(pObj, radius2, 36, 24);
        glPopMatrix();
    }

    float t3 = explosionTime - 0.2f;
    if (t3 >= 0.0f) {
        float p3 = easeOutSine(t3 / explosionDuration);
        float radius3 = 2.0f + 8.0f * p3, alpha3 = 0.4f * (1.0f - (t3 / explosionDuration));
        glPushMatrix();
        glTranslatef(x - 3.0f, y - 0.3f, z + 0.4f);
        glColor4f(1.0f, 0.2f, 0.2f, alpha3);
        gluSphere(pObj, radius3, 36, 24);
        glPopMatrix();
    }

    glPopMatrix();
        // --- Draw and update smoke particles ---
    for (int i = 0; i < explosionSmoke.size(); ) {
        SmokeParticle& sp = explosionSmoke[i];
        sp.x += sp.vx;
        sp.y += sp.vy;
        sp.z += sp.vz;
        sp.life -= deltaTime * 0.5f;
        sp.alpha = sp.life;
        sp.size += 0.03f;

        if (sp.life <= 0.0f) {
            explosionSmoke.erase(explosionSmoke.begin() + i);
            continue;
        }

        glPushMatrix();
        glTranslatef(sp.x, sp.y, sp.z);
        glColor4f(0.3f, 0.3f, 0.3f, sp.alpha);
        gluSphere(pObj, sp.size, 16, 12);
        glPopMatrix();

        ++i;
    }

    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    gluDeleteQuadric(pObj);

    if (explosionTime > explosionDuration) {
    explosionActive = false;
    explosionSmoke.clear(); // clear smoke when explosion ends
    return;
}

}


// --- THIS IS THE CORRECTED tickTime FUNCTION ---
void Characters::tickTime(long int elapseTime)
{
    float deltaSeconds = elapseTime / 1000.0f;

        // --- WINNER TELEPORTATION ---
    if (isChiikawaDefeated && !isUsagiDefeated) {
        // Chiikawa lost, teleport Usagi to (0, 20, 0)
        usagiX = 0.0f;
        usagiY = 17.0f;
        usagiZ = 0.0f;
    }
    if (isUsagiDefeated && !isChiikawaDefeated) {
        // Usagi lost, teleport Chiikawa to (0, 20, 0)
        chikawa1X = 0.0f;
        chikawa1Y = 17.0f;
        chikawa1Z = 0.0f;
    }


    // Check for defeated state
    if (healthChikawa <= 0 && !isChiikawaDefeated) {
        isChiikawaDefeated = true;
    }
    if (healthUsagi <= 0 && !isUsagiDefeated) {
        isUsagiDefeated = true;
    }

    // --- UPDATE CHIIKAWA ---
    if (isChiikawaDefeated) {
        // If defeated, only play fall animation
        if (chiikawaFallAngle < 65.0f) {
            fallAnimationTimeChikawa += deltaSeconds;
            float t = std::min(fallAnimationTimeChikawa / fallDuration, 1.0f);
            chiikawaFallAngle = 65.0f * sinf(t * (M_PI / 2.0f));
        } else {
            chiikawaFallAngle = 65.0f;
        }
    } else {
        // If alive, run all normal logic
        isMovingChikawa1 = (fabs(chikawa1X - prevChikawa1X) > 0.001f || fabs(chikawa1Z - prevChikawa1Z) > 0.001f);
        prevChikawa1X = chikawa1X;
        prevChikawa1Z = chikawa1Z;

        if (rotationTimeChikawa1 < rotationDuration) {
            rotationTimeChikawa1 += deltaSeconds;
            float t = std::min(rotationTimeChikawa1 / rotationDuration, 1.0f);
            float ease = 0.5f - 0.5f * cos(t * M_PI);
            float delta = targetRotationChikawa1 - startRotationChikawa1;
            if (delta > 180.0f) delta -= 360.0f;
            if (delta < -180.0f) delta += 360.0f;
            currentRotationChikawa1 = startRotationChikawa1 + delta * ease;
        }

        float legSwingSpeed = 40.0f;
        float legSwingAmplitude = 60.0f;
        if (isMovingChikawa1) {
            legSwingTimeChikawa1 += deltaSeconds * legSwingSpeed;
            legSwingAngleChikawa1 = sin(legSwingTimeChikawa1) * legSwingAmplitude;
        } else {
            legSwingAngleChikawa1 *= (1.0f - deltaSeconds * 5.0f);
            if (fabs(legSwingAngleChikawa1) < 0.5f) {
                legSwingAngleChikawa1 = 0.0f;
            }
        }

        if (attackChikawa) {
            if (!attackInProgress2) {
                attackInProgress2 = true;
                attackAngleChikawa = 0.0f;
                usagiHit = false;
            }
            attackAngleChikawa += 0.05f;
            if (attackAngleChikawa >= 1.0f) {
                attackAngleChikawa = 1.0f;
                attackChikawa = false;
                attackInProgress2 = false;
            }
        }

        if (attackInProgress2 && !usagiHit) {
            float dx = usagiX - chikawa1X;
            float dy = usagiY - chikawa1Y;
            float dz = usagiZ - chikawa1Z; // Corrected variable name
            float distance = sqrt(dx*dx + dy*dy + dz*dz);

            if (distance < 13.5f) {
                float knockbackDirection = 0.0f;
                if (fabs(fmod(currentRotationChikawa1, 360.0f) - 0.0f) < 1.0f || fabs(fmod(currentRotationChikawa1, 360.0f) - 180.0f) < 1.0f) {
                    knockbackDirection = 180.0f;
                }
                float theta = (currentRotationChikawa1 + knockbackDirection) * (3.14159265f / 180.0f);
                float dirX = sinf(theta);
                float dirZ = -cosf(theta);

                if ((dx * dirX + dz * dirZ) > 0.0f) {
                    usagiHit = true;
                    playHitEffectUsagi = true;
                    hitEffectTimeUsagi = 0.0f;
                    usagiAirVelX = dirX * 8.0f;
                    usagiAirVelZ = dirZ * 8.0f;
                    usagiInAir = true;
                    usagiHitTime = 0.0f;
                    usagiInitialY = usagiY;
                    healthUsagi -= 15.0f;
                }
            }
        }
    }

    // --- UPDATE USAGI ---
    if (isUsagiDefeated) {
        // If defeated, only play fall animation
        if (usagiFallAngle < 65.0f) {
            fallAnimationTimeUsagi += deltaSeconds;
            float t = std::min(fallAnimationTimeUsagi / fallDuration, 1.0f);
            usagiFallAngle = 65.0f * sinf(t * (M_PI / 2.0f));
        } else {
            usagiFallAngle = 65.0f;
        }
    } else {
        // If alive, run all normal logic
        isMovingUsagi1 = (fabs(usagiX - prevUsagiX) > 0.001f || fabs(usagiZ - prevUsagiZ) > 0.001f);
        prevUsagiX = usagiX;
        prevUsagiZ = usagiZ;

        if (rotationTimeUsagi1 < rotationDuration) {
            rotationTimeUsagi1 += deltaSeconds;
            float t = std::min(rotationTimeUsagi1 / rotationDuration, 1.0f);
            float ease = 0.5f - 0.5f * cos(t * M_PI);
            float deltaUsagi = targetRotationUsagi1 - startRotationUsagi1;
            if (deltaUsagi > 180.0f) deltaUsagi -= 360.0f;
            if (deltaUsagi < -180.0f) deltaUsagi += 360.0f;
            currentRotationUsagi1 = startRotationUsagi1 + deltaUsagi * ease;
        }

        float legSwingSpeed = 40.0f;
        float legSwingAmplitude = 60.0f;
        if (isMovingUsagi1) {
            legSwingTimeUsagi1 += deltaSeconds * legSwingSpeed;
            legSwingAngleUsagi1 = sin(legSwingTimeUsagi1) * legSwingAmplitude;
        } else {
            legSwingAngleUsagi1 *= (1.0f - deltaSeconds * 5.0f);
            if (fabs(legSwingAngleUsagi1) < 0.5f) {
                legSwingAngleUsagi1 = 0.0f;
            }
        }

        if (attackUsagi) {
            if (!attackInProgress) {
                attackInProgress = true;
                attackAngleUsagi = 0.0f;
            }
            attackAngleUsagi += 0.012f;
            if (attackAngleUsagi >= 1.0f) {
                attackAngleUsagi = 1.0f;
                attackUsagi = false;
                attackInProgress = false;
            }
        }
    }

    // --- UNIVERSAL LOGIC ---
    bounceTimeChikawa1 += elapseTime * bounceSpeed;
    bounceTimeUsagi += elapseTime * bounceSpeed; // This was unused but keeping it for consistency
    bounceOffset = bounceAmplitude * sin(bounceTimeChikawa1);

    if (playHitEffectChikawa) {
        hitEffectTimeChikawa += deltaSeconds;
        if (hitEffectTimeChikawa > 1.0f) playHitEffectChikawa = false;
    }
    if (playHitEffectUsagi) {
        hitEffectTimeUsagi += deltaSeconds;
        if (hitEffectTimeUsagi > 1.0f) playHitEffectUsagi = false;
    }

    if (chiikawaInAir) {
        chiikawaHitTime += deltaSeconds;
        float t = chiikawaHitTime / chiikawaAirDuration;
        if (t >= 1.0f) {
            chiikawaInAir = false; chiikawaHit = false; chikawa1Y = 0.0f;
        } else {
            chikawa1Y = sin(t * M_PI) * chiikawaAirPeak;
            chikawa1X += chiikawaAirVelX * deltaSeconds;
            chikawa1Z += chiikawaAirVelZ * deltaSeconds;
        }
    }
    if (usagiInAir) {
        usagiHitTime += deltaSeconds;
        float t = usagiHitTime / usagiAirDuration;
        if (t >= 1.0f) {
            usagiInAir = false; usagiHit = false; usagiY = 0.0f;
        } else {
            usagiY = sin(t * M_PI) * chiikawaAirPeak;
            usagiX += usagiAirVelX * deltaSeconds;
            usagiZ += usagiAirVelZ * deltaSeconds;
        }
    }
    updateMovement();
}


void Weapon::setup(const GLfloat* controlPoints,GLint uOrder)
{
    controlpoints = controlPoints;
    uorder = uOrder;
    glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, uorder, controlpoints);
    glEnable(GL_MAP1_VERTEX_3);
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
    gluQuadricOrientation(pObj, GLU_OUTSIDE);
}

void Weapon::evaluateBezierPoint(float t, float* out)
{
    int n = uorder - 1;
    GLfloat temp[7][3];
    for (int i = 0; i <= n; ++i) {
        temp[i][0] = controlpoints[i * 3 + 0];
        temp[i][1] = controlpoints[i * 3 + 1];
        temp[i][2] = controlpoints[i * 3 + 2];
    }
    for (int r = 1; r <= n; ++r) {
        for (int i = 0; i <= n - r; ++i) {
            temp[i][0] = (1 - t) * temp[i][0] + t * temp[i + 1][0];
            temp[i][1] = (1 - t) * temp[i][1] + t * temp[i + 1][1];
            temp[i][2] = (1 - t) * temp[i][2] + t * temp[i + 1][2];
        }
    }
    out[0] = temp[0][0];
    out[1] = temp[0][1];
    out[2] = temp[0][2];
}

void Weapon::drawChiikawaWeapon(GLenum draw_mode, GLint ures)
{
    static GLfloat controlpointsl[] = { -4.0f, 17.5f, 0.0f, -2.0f, 20.0f, 0.0f, -6.0f, 8.0f, 0.0f, 0.0f, 9.0f, 0.0f, 6.0f, 8.0f, 0.0f, 2.0f, 20.0f, 0.0f, 4.0f, 17.5f, 0.0f };
    setup(controlpointsl, 7);
    glDisable(GL_CULL_FACE);

    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.976f, 0.71f, 0.773f);
    gluCylinder(pObj, 0.5f, 0.5f, 12.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 12.5f);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluCylinder(pObj, 0.5f, 0.5f, 2.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 2.5f);
    gluSphere(pObj, 0.5f, 36, 24);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.976f, 0.71f, 0.773f);
    glScalef(1.0f, 1.0f, -1.0f);
    float bottom_point[3];
    evaluateBezierPoint(0.5f, bottom_point);
    glTranslatef(bottom_point[0], 3.0f, bottom_point[2]);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    const float torus_radius = 3.0f, tube_radius = 0.7f;
    const int slices_curve = 36, loops = 18;
    const float start_angle = 0.0f, end_angle = 180.0f;

    for (int i = 0; i < loops; i++) {
        float theta = i * (end_angle - start_angle) / loops + start_angle;
        theta *= M_PI / 180.0f;
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices_curve; j++) {
            float phi = j * 2.0f * M_PI / slices_curve;
            for (int k = 0; k <= 1; k++) {
                float beta = (i + k) * (end_angle - start_angle) / loops + start_angle;
                beta *= M_PI / 180.0f;
                float x = (torus_radius + tube_radius * cos(phi)) * cos(beta);
                float y = (torus_radius + tube_radius * cos(phi)) * sin(beta);
                float z = tube_radius * sin(phi);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -11.3f, 0.0f);
    glColor3f(0.976f, 0.71f, 0.773f);
    float radius = 0.75f;
    int slices = 50, stacks = 20;
    for (int i = 0; i < slices; ++i) {
        float t1 = (float)i / slices, t2 = (float)(i + 1) / slices;
        float pt1[3], pt2[3];
        evaluateBezierPoint(t1, pt1);
        evaluateBezierPoint(t2, pt2);
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= stacks; ++j) {
            float angle = (float)j / stacks * 2.0f * M_PI;
            float cx = cos(angle), cz = sin(angle);
            glVertex3f(pt1[0] + radius * cx, pt1[1], pt1[2] + radius * cz);
            glVertex3f(pt2[0] + radius * cx, pt2[1], pt2[2] + radius * cz);
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(5.0f, 0.0f, 0.0f, 1.0f); glTranslatef(-3.57f, 5.0f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 2.0, 36, 24); glTranslatef(0.0f, 0.0f, 2.0f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(7.5f, 0.0f, 0.0f, 1.0f); glTranslatef(-3.3f, 3.75f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 2.0, 36, 24); glTranslatef(0.0f, 0.0f, 2.0f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(15.0f, 0.0f, 0.0f, 1.0f); glTranslatef(-2.85f, 2.8f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 2.0, 36, 24); glTranslatef(0.0f, 0.0f, 2.0f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(-5.0f, 0.0f, 0.0f, 1.0f); glTranslatef(3.57f, 5.0f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 2.0, 36, 24); glTranslatef(0.0f, 0.0f, 2.0f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(-7.5f, 0.0f, 0.0f, 1.0f); glTranslatef(3.3f, 3.75f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 2.0, 36, 24); glTranslatef(0.0f, 0.0f, 2.0f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); glTranslatef(2.85f, 2.8f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 2.0, 36, 24); glTranslatef(0.0f, 0.0f, 2.0f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(1.75f, 0.0f, 0.0f, 1.0f); glTranslatef(-2.45f, 4.8f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 1.8, 36, 24); glTranslatef(0.0f, 0.0f, 1.8f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(7.5f, 0.0f, 0.0f, 1.0f); glTranslatef(-2.0f, 3.75f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 1.8, 36, 24); glTranslatef(0.0f, 0.0f, 1.8f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(18.0f, 0.0f, 0.0f, 1.0f); glTranslatef(-1.35f, 2.75f, 0.0f); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 1.8, 36, 24); glTranslatef(0.0f, 0.0f, 1.8f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(-1.75f, 0.0f, 0.0f, 1.0f); glTranslatef(2.45f, 4.8f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 1.8, 36, 24); glTranslatef(0.0f, 0.0f, 1.8f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(-7.5f, 0.0f, 0.0f, 1.0f); glTranslatef(2.0f, 3.75f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 1.8, 36, 24); glTranslatef(0.0f, 0.0f, 1.8f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glRotatef(-18.0f, 0.0f, 0.0f, 1.0f); glTranslatef(1.35f, 2.75f, 0.0f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); gluCylinder(pObj, 0.5, 0.075, 1.8, 36, 24); glTranslatef(0.0f, 0.0f, 1.8f); gluSphere(pObj, 0.075f, 16, 16); glPopMatrix();

    gluDeleteQuadric(pObj);
    glEnable(GL_CULL_FACE);
}

void Weapon::drawUsagiWeapon()
{
    glDisable(GL_CULL_FACE);
    GLUquadricObj *pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(-12.5f, -6.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(pObj, 0.5f, 36, 24);
    gluCylinder(pObj, 0.5f, 0.5f, 2.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 2.5f);
    glColor3f(1.0f, 0.8f, 0.0f);
    gluCylinder(pObj, 0.5f, 0.5f, 12.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 12.5f);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluCylinder(pObj, 0.5f, 0.5f, 2.5f, 36, 24);
    glTranslatef(0.0f, 0.0f, 2.5f);
    gluSphere(pObj, 0.5f, 36, 24);
    glPopMatrix();
    gluDeleteQuadric(pObj);
    glEnable(GL_CULL_FACE);
}

void updateLegMovement(bool moveForward, bool moveBackward, bool moveLeft, bool moveRight, float& legSwingAngle) {
    if (moveForward || moveBackward) {
        legSwingAngle = 10.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01f);
    } else {
        legSwingAngle = 0.0f;
    }
}

void Characters::updateMovement()
{
    if (moveForwardChikawa1 || moveBackwardChikawa1 || moveLeftChikawa1 || moveRightChikawa1) {
        legSwingAngleChikawa1 = sin(glutGet(GLUT_ELAPSED_TIME) / 100.0f) * 10.0f;
    }
    if (moveForwardUsagi || moveBackwardUsagi || moveLeftUsagi || moveRightUsagi) {
        legSwingAngleUsagi1 = sin(glutGet(GLUT_ELAPSED_TIME) / 100.0f) * 10.0f;
    }
     updateBoundingBoxes();
}

extern float healthChikawa;
extern float healthUsagi;

void MyVirtualWorld::drawHealthBar3D(float x, float y, float z, float health, float maxHealth, const char* name) {
    // Save the current OpenGL states
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up 2D orthographic projection for drawing health bar
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable 3D effects for 2D overlay (health bars and names)
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float barWidth = 200.0f;
    float barHeight = 20.0f;
    float currentHealthWidth = (health / maxHealth) * barWidth;

    // Define fixed screen positions for the health bars
    float screenX, screenY;
    if (strcmp(name, "Chiikawa") == 0) { // For Chiikawa (top-left)
        screenX = 10.0f;
        screenY = 560.0f; // Top of the screen, adjusted for bar height
    } else if (strcmp(name, "Usagi") == 0) { // For Usagi (top-right)
        screenX = 800.0f - barWidth - 10.0f; // 800 is screen width, 10 is padding
        screenY = 560.0f; // Top of the screen, adjusted for bar height
    } else {
        // Default positioning if name doesn't match
        screenX = 10.0f;
        screenY = 10.0f;
    }

    // Apply animation effect based on healthBarAnimationProgress
    float animatedScale = healthBarAnimationProgress; // Scales from 0 to 1
    float animatedWidth = barWidth * animatedScale;
    float animatedHeight = barHeight * animatedScale;

    // Adjust position to make it appear from the middle
    float initialOffsetX = (barWidth - animatedWidth) / 2.0f;
    float initialOffsetY = (barHeight - animatedHeight) / 2.0f;

    // Draw the background strip
    glColor4f(0.3f, 0.3f, 0.3f, 0.7f * animatedScale); // Dark gray, semi-transparent, fades in
    glBegin(GL_QUADS);
    glVertex2f(screenX + initialOffsetX, screenY + initialOffsetY);
    glVertex2f(screenX + initialOffsetX + animatedWidth, screenY + initialOffsetY);
    glVertex2f(screenX + initialOffsetX + animatedWidth, screenY + initialOffsetY + animatedHeight);
    glVertex2f(screenX + initialOffsetX, screenY + initialOffsetY + animatedHeight);
    glEnd();

    // Draw the actual health bar (green, changing color with health)
    float r = 1.0f - (health / maxHealth); // Red increases as health drops
    float g = health / maxHealth; // Green decreases as health drops
    glColor4f(r, g, 0.0f, animatedScale); // Health bar color: from green to red, fades in
    glBegin(GL_QUADS);
    glVertex2f(screenX + initialOffsetX, screenY + initialOffsetY);
    glVertex2f(screenX + initialOffsetX + currentHealthWidth * animatedScale, screenY + initialOffsetY);
    glVertex2f(screenX + initialOffsetX + currentHealthWidth * animatedScale, screenY + initialOffsetY + animatedHeight);
    glVertex2f(screenX + initialOffsetX, screenY + initialOffsetY + animatedHeight);
    glEnd();

    // Set the line width for a thicker border
    glLineWidth(4.0f * animatedScale); // Increase this value to make the border thicker, scales with animation

    // Draw the health bar border (thicker border)
    glColor4f(1.0f, 1.0f, 1.0f, animatedScale); // White border, fades in
    glBegin(GL_LINE_LOOP);
    glVertex2f(screenX + initialOffsetX, screenY + initialOffsetY);
    glVertex2f(screenX + initialOffsetX + animatedWidth, screenY + initialOffsetY);
    glVertex2f(screenX + initialOffsetX + animatedWidth, screenY + initialOffsetY + animatedHeight);
    glVertex2f(screenX + initialOffsetX, screenY + initialOffsetY + animatedHeight);
    glEnd();

    // Restore the line width to default
    glLineWidth(1.0f); // Reset line width to the default

    // Draw the character name (adjusted positions and scaled with animation)
    glColor4f(0.0f, 0.0f, 0.0f, animatedScale); // Black color for the name, fades in

    // For Chiikawa, position name at the left of the bar
    // For Usagi, position name at the right of the bar
    if (strcmp(name, "Chiikawa") == 0) {
        // Adjust Y position slightly to stay aligned during scale
        glRasterPos2f(screenX, screenY + barHeight + 5 - (barHeight * (1.0f - animatedScale)));
    } else if (strcmp(name, "Usagi") == 0) {
        glRasterPos2f(screenX*1.3, screenY + barHeight + 5 - (barHeight * (1.0f - animatedScale)));
    }

    // Use a larger and nicer font
    for (const char* c = name; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Use a larger font for better readability
    }

    // Restore the OpenGL states
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // Re-enable lighting if it was enabled before
    glPopMatrix(); // Restore Modelview Matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restore Projection Matrix
    glMatrixMode(GL_MODELVIEW); // Set back to Modelview matrix for further 3D rendering
}



bool Characters::checkProjectileCollisionWithChiikawa() {
    float dx = projectileX - chikawa1X;
    float dz = projectileZ - chikawa1Z;
    return (sqrt(dx * dx + dz * dz) < 7.0f);
}

bool Characters::checkProjectileCollisionWithUsagi() {
    float dx = projectileX - usagiX;
    float dz = projectileZ - usagiZ;
    return (sqrt(dx * dx + dz * dz) < 7.0f);
}


void Background::grass() {
    Characters characters;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[0]);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    // Initialize confetti only once when someone wins
    if (someoneWon) {
        glEnable(GL_LIGHTING);
    }

    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    const float radius = 180.0f;
    const int segments = 100;

    // === Draw textured disk ===
    glBegin(GL_TRIANGLE_FAN);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color to white to show full texture
    glTexCoord2f(0.5f, 0.5f);     // Center of texture
    glVertex3f(0.0f, 0.0f, 0.0f); // Center of disk

    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * M_PI / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        float repeatFactor = 10.0f;
        float u = 0.5f + 0.5f * cos(angle) * repeatFactor;
        float v = 0.5f + 0.5f * sin(angle) * repeatFactor;

        glTexCoord2f(u, v);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Disable texture for the rest (non-textured disks)
    glDisable(GL_TEXTURE_2D);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_FLAT);

    glTranslatef(0.0f, 0.0f, -1.0f);
    glColor3f(0.4f, 0.3f, 0.2f);
    gluDisk(quad, 0.0f, 180.0f, 100, 1);

    glTranslatef(0.0f, 0.0f, -99.0f);
    glColor3f(0.3f, 0.25f, 0.2f);
    gluDisk(quad, 0.0f, 600.0f, 100, 1);

    gluDeleteQuadric(quad);
    glPopMatrix();

    glEnable(GL_CULL_FACE);
    glColor3f(1.0f, 1.0f, 1.0f); // Reset color for next objects
    glEnable(GL_LIGHTING);
}



void MySweepSurface::drawmountain()
{
    glDisable(GL_CULL_FACE);
    glColor3f(0.2f, 0.2f, 0.2f);
    int index = 0;
    for (int i=0; i<numofverticallines-1; ++i)
    {
        glBegin(GL_QUAD_STRIP);
        for (int j=0; j<numofprofilepoints; ++j)
        {
            glVertex3fv( surfacepoints+index );
            glVertex3fv( surfacepoints+index+3*numofprofilepoints );
            index+=3;
        }
        glEnd();
    }
    glEnable(GL_CULL_FACE);
}


void Background::tree() {
    glDisable(GL_CULL_FACE);
    GLUquadric* quad = gluNewQuadric();

    // === Enable Texture 1 (Wood) for Trunk & Branches ===
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[1]); // Wood texture
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    glColor3f(1.0f, 1.0f, 1.0f); // Avoid dark tint

    // Base
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 3.0, 4.0, 1.0, 20, 20);
    glPopMatrix();

    // Middle trunk
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 1.8, 3.0, 3.0, 20, 20);
    glPopMatrix();

    // Upper trunk
    glPushMatrix();
    glTranslatef(0.0f, 18.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 1.3, 1.8, 14.0, 20, 20);
    glPopMatrix();

    // Branches
    glPushMatrix();
    glTranslatef(0.8f, 11.0f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quad, 1.0, 0.4, 7.0, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, 10.0f, 0.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quad, 1.0, 0.4, 5.0, 12, 12);
    glPopMatrix();

    // === Enable Texture 2 (Leaf) for Spheres ===
    glBindTexture(GL_TEXTURE_2D, textureNumbers[2]); // Leaf texture
    glColor3f(1.0f, 1.0f, 1.0f); // Ensure texture color is not dimmed

    glPushMatrix(); glTranslatef(0.0f, 28.0f, 0.0f); gluSphere(quad, 8.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(9.0f, 18.0f, 0.0f); gluSphere(quad, 5.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(-8.0f, 20.0f, -2.0f); gluSphere(quad, 9.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(-7.0f, 16.0f, 0.0f); gluSphere(quad, 3.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(-8.0f, 20.0f, 4.0f); gluSphere(quad, 5.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(3.0f, 22.0f, -7.0f); gluSphere(quad, 5.0, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(7.0f, 25.0f, -2.0f); gluSphere(quad, 9.0f, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 18.0f, -5.0f); gluSphere(quad, 6.5, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 20.0f, 7.0f); gluSphere(quad, 6.5, 20, 20); glPopMatrix();

    // Clean up
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quad);
    glEnable(GL_CULL_FACE);
}


void Background::pinetree1() {
    glDisable(GL_CULL_FACE);
    GLUquadric* quad = gluNewQuadric();

    // Enable texture mapping on quadrics
    gluQuadricTexture(quad, GL_TRUE);

    // --- Trunk with texture 2 ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[1]);  // texture2 = trunk texture ID

    glColor3f(1.0f, 1.0f, 1.0f);  // Use white so texture colors are accurate
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 1.0, 0.5, 10.0, 20, 20);
    gluDisk(quad, 0.0, 1.0, 20, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // --- Pine foliage cones with texture 1 ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[4]);  // texture1 = pine foliage texture ID

    glColor3f(1.0f, 1.0f, 1.0f); // White again for proper texturing

    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 5.0, 0.0, 6.0, 20, 20);
    gluDisk(quad, 0.0, 5.0, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 7.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 4.0, 0.0, 5.0, 20, 20);
    gluDisk(quad, 0.0, 4.0, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 10.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 2.5, 0.0, 3.7, 20, 20);
    gluDisk(quad, 0.0, 2.5, 20, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(quad);
    glEnable(GL_CULL_FACE);
}

void Background::pinetree2() {
    glDisable(GL_CULL_FACE);
    GLUquadric* quad = gluNewQuadric();

    gluQuadricTexture(quad, GL_TRUE);  // Enable texture coords generation

    // --- Trunk with texture 2 ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[1]);  // texture2 = trunk texture ID

    glColor3f(1.0f, 1.0f, 1.0f);  // White for texture clarity
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 1.0, 0.4, 7.0, 20, 20);
    gluDisk(quad, 0.0, 1.0, 20, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // --- Pine foliage with texture 1 ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[2]);  // texture1 = foliage texture ID

    glColor3f(1.0f, 1.0f, 1.0f); // White again for texturing
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 4.0, 0.0, 12.0, 20, 20);
    gluDisk(quad, 0.0, 4.0, 20, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(quad);
    glEnable(GL_CULL_FACE);
}


void Background::bush() {
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[2]);

    GLUquadric* quad = gluNewQuadric();

    gluQuadricTexture(quad, GL_TRUE);

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(0.0f, 5.0f, 0.0f);
    gluSphere(quad, 5.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.5f, 3.0f, 0.0f);
    gluSphere(quad, 3.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.5f, 3.0f, 0.0f);
    gluSphere(quad, 3.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 3.0f, -3.5f);
    gluSphere(quad, 3.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.5f, 2.0f, -2.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.5f, 2.0f, -2.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, 2.0f, 2.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.0f, 2.0f, 2.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 5.5f, 2.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.0f, 5.0f, 3.0f);
    gluSphere(quad, 2.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5f, 6.5f, 1.5f);
    gluSphere(quad, 4.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f, 7.5f, 0.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 7.0f, -1.0f);
    gluSphere(quad, 3.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.0f, 6.0f, -2.0f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.5f, 6.0f, -1.5f);
    gluSphere(quad, 3.0, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);  // Disable texturing after done
    glEnable(GL_CULL_FACE);
}

void Background::rock() {
    glDisable(GL_CULL_FACE);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureNumbers[3]);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Base polygon
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 0.0f, -1.0f);
    glTexCoord2f(0.2f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.5f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f( 1.5f, 0.0f, -2.0f);
    glTexCoord2f(0.7f, 0.0f); glVertex3f( 2.0f, 0.0f, -0.5f);
    glTexCoord2f(0.9f, 0.0f); glVertex3f( 1.0f, 0.0f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5f, 0.0f, 1.8f);
    glEnd();

    // Side triangles
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 0.0f, -1.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(-1.0f, 2.7f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.5f, 0.0f, -2.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.5f, 2.9f, -1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 0.0f, -0.5f);
    glEnd();

    // Top quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 2.7f, -1.0f);
    glTexCoord2f(0.33f, 1.0f); glVertex3f(0.5f, 2.9f, -1.5f);
    glTexCoord2f(0.66f, 1.0f); glVertex3f(1.0f, 2.2f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 2.5f, 1.2f);
    glEnd();

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 2.5f, 1.2f);
    glTexCoord2f(0.25f, 1.0f); glVertex3f(1.0f, 2.2f, 1.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(1.2f, 1.5f, 2.0f);
    glTexCoord2f(0.75f, 1.0f); glVertex3f(-1.2f, 1.6f, 2.2f);
    glEnd();

    // Slanted front quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 2.7f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -2.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5f, 0.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 2.9f, -1.5f);
    glEnd();

    // Left side polygon
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 0.0f, -1.0f);
    glTexCoord2f(0.25f, 0.0f); glVertex3f(-1.5f, 0.0f, 1.8f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(-1.2f, 1.6f, 2.2f);
    glTexCoord2f(0.75f, 1.0f); glVertex3f(-1.0f, 2.5f, 1.2f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-1.0f, 2.7f, -1.0f);
    glEnd();

    // Back quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f, 0.0f, 1.8f);
    glTexCoord2f(0.33f, 0.0f); glVertex3f(1.0f, 0.0f, 1.5f);
    glTexCoord2f(0.66f, 1.0f); glVertex3f(1.2f, 1.5f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.2f, 1.6f, 2.2f);
    glEnd();

    // Right polygon
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0f, 0.0f, -0.5f);
    glTexCoord2f(0.25f, 1.0f); glVertex3f(0.5f, 2.9f, -1.5f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, 2.2f, 1.0f);
    glTexCoord2f(0.75f, 1.0f); glVertex3f(1.2f, 1.5f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.5f);
    glEnd();

    // Clean up
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

void MySweepSurface::setup(const GLfloat* profilePoints, GLint numOfProfilePoints, GLfloat degreeStart, GLfloat degreeEnd, GLfloat degreeStep ){
    profilepoints = profilePoints;
    numofprofilepoints = numOfProfilePoints;
    if (surfacepoints) delete[] surfacepoints;
    numofverticallines = 1 + static_cast<int>(floor((degreeEnd - degreeStart)
    /degreeStep));
    numofsurfacepoints = numofprofilepoints
     * numofverticallines;
    surfacepoints = new GLfloat[3 * numofsurfacepoints];
    int surfptsindex = 0;
    GLfloat degree = degreeStart;
    for (int i=0; i<numofverticallines; ++i, degree+=degreeStep)
    {
         GLfloat radian = degree*M_PI/180.0;
         GLfloat c = cos(radian);
         GLfloat s = sin(radian);
         GLfloat x,y,z;
         for (int j=0, profileindex=0; j<numofprofilepoints; ++j)
         {
             surfacepoints[surfptsindex] = c*profilepoints[profileindex] + s*profilepoints[profileindex+2];
             surfacepoints[surfptsindex+1] = profilepoints[profileindex+1];
             surfacepoints[surfptsindex+2] = -s*profilepoints[profileindex] + c*profilepoints[profileindex+2];
             profileindex += 3;
             surfptsindex += 3;
         }
    }
}
void MySweepSurface::drawbowl()
{
    glDisable(GL_CULL_FACE);
    glColor3f(1.0f, 1.0f, 1.0f);
    int index = 0;
    for (int i=0; i<numofverticallines-1; ++i)
    {
        glBegin(GL_QUAD_STRIP);
        for (int j=0; j<numofprofilepoints; ++j)
        {
            glVertex3fv( surfacepoints+index );
            glVertex3fv( surfacepoints+index+3*numofprofilepoints );
            index+=3;
        }
        glEnd();
    }
    glEnable(GL_CULL_FACE);
}

void Star(float x, float y, float z, float rotationDegrees) {
    const int points = 5;
    const float outerRadius = 3.0f;
    const float innerRadius = 1.2f;
    const float thickness = 1.0f;

    struct Vertex {
        float x, y, z;
    };

    Vertex frontVertices[points * 2];
    Vertex backVertices[points * 2];

    for (int i = 0; i < points * 2; ++i) {
        float angle = (float)i * M_PI / points;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float vx = radius * sin(angle);
        float vz = radius * cos(angle);
        frontVertices[i] = { vx, thickness / 2, vz };
        backVertices[i] = { vx, -thickness / 2, vz };
    }

    glPushMatrix();
    glTranslatef(x, y, z);                  // Move star to position
    glRotatef(rotationDegrees, 1, 0, 0);    // Rotate around Y-axis
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);      // Yellow

    // Front face
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, thickness / 2, 0.0f);
    for (int i = 0; i <= points * 2; ++i) {
        int idx = i % (points * 2);
        glVertex3f(frontVertices[idx].x, frontVertices[idx].y, frontVertices[idx].z);
    }
    glEnd();

    // Back face
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -thickness / 2, 0.0f);
    for (int i = points * 2; i >= 0; --i) {
        int idx = i % (points * 2);
        glVertex3f(backVertices[idx].x, backVertices[idx].y, backVertices[idx].z);
    }
    glEnd();

    // Sides
    glBegin(GL_QUADS);
    for (int i = 0; i < points * 2; ++i) {
        int next = (i + 1) % (points * 2);
        glVertex3f(frontVertices[i].x, frontVertices[i].y, frontVertices[i].z);
        glVertex3f(frontVertices[next].x, frontVertices[next].y, frontVertices[next].z);
        glVertex3f(backVertices[next].x, backVertices[next].y, backVertices[next].z);
        glVertex3f(backVertices[i].x, backVertices[i].y, backVertices[i].z);
    }
    glEnd();

    glPopMatrix();
}

void Background::pudding() {
    glDisable(GL_CULL_FACE);

    // Draw the opaque bottom disk first (depth mask ON, no blending)
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    GLfloat puddingColor[] = { 0.9f, 0.7f, 0.0f, 0.7f };   // Semi-transparent
    GLfloat toppingColor[] = { 0.4f, 0.2f, 0.1f, 0.7f };

    glPushMatrix();
    glColor4fv(puddingColor);
    glTranslatef(0.0f, 8.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, 7.0f, 30, 1);  // bottom
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);

    // Draw stars inside
    glPushMatrix();
    glScalef(0.7f, 0.7f, 0.7f);
    Star(2.0f, 15.0f, 2.0f, 45.0f);
    Star(3.0f, 20.0f, -4.0f, 90.0f);
    Star(-4.0f, 20.0f, 3.0f, 300.0f);
    glPopMatrix();

    // Main Body
    glPushMatrix();
    glColor4fv(puddingColor);
    glTranslatef(0.0f, 8.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 7.0f, 5.5f, 8.0f, 50, 1);
    glPopMatrix();

    // Top Disc
    glPushMatrix();
    glColor4fv(puddingColor);
    glTranslatef(0.0f, 16.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, 5.5f, 30, 1);
    glPopMatrix();

    // Topping
    glPushMatrix();
    glColor4fv(toppingColor);
    glTranslatef(0.0f, 16.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 5.5f, 5.2f, 1.5f, 30, 5);
    glPopMatrix();

    // Topping Top
    glPushMatrix();
    glColor4fv(toppingColor);
    glTranslatef(0.0f, 17.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, 5.2f, 30, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}

void Background::sun() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(100.0f, 150.0f, 0.0f);
    glScalef(6.0f, 6.0f, 6.0f);

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.7f, 0.0f);
    gluSphere(quad, 2.0f, 20, 20);

    for (int i = 0; i < 12; ++i) {
        float angle = 360.0f * i / 12;
        glPushMatrix();
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glTranslatef(2.5f, 0.0f, 0.0f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);

        glColor3f(1.0f, 0.7f, 0.0f); // Use same color for rays
        gluCylinder(quad, 0.2f, 0.2f, 1.5, 10, 1);
        glPopMatrix();
    }

    glEnable(GL_LIGHTING);

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Background::moon() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(100.0f, 150.0f, 0.0f);
    glScalef(6.0f, 6.0f, 6.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(quad, 2.0f, 20, 20);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    gluDeleteQuadric(quad);
}


void Background::draw() {
    glDisable(GL_CULL_FACE);
    grass();

    for (int i = 0; i < 360; i += 10) {
        glPushMatrix();

        float angleOffset = 3.0f * sin(i * 3.14159f / 180.0f * 4);
        float angle = i + angleOffset;
        glRotatef(angle, 0.0f, 1.0f, 0.0f);

        float radiusOffset = 9.0f * cos(i * 3.14159f / 180.0f * 6);
        glTranslatef(0.0f, 0.0f, 110.0f + radiusOffset);

        int treeType = (i / 10) % 3;
        if (treeType == 0) {
            glScalef(2.5f, 2.5f, 2.5f);
            pinetree2();
        } else if (treeType == 1) {
            glScalef(2.5f, 2.5f, 2.5f);
            pinetree1();
        } else {
            tree();
        }

        glPopMatrix();
    }

    float X[] = {0.0f, -70.0f, -35.0f, 70.0f, 35.0f, -60.0f, -75.0f};
    float Z[] = {-70.0f, -0.0f, 70.0f, 0.0f, 70.0f, -65.0f, 45.0f};
    float R1[] = {0.0f, 15.0f, -10.0f, 20.0f, -15.0f, 30.0f, -20.0f};
    for (int i = 0; i < 7; ++i) {
        glPushMatrix();
        glTranslatef(X[i], 0.0f, Z[i]);
        glRotatef(R1[i], 0.0f, 1.0f, 0.0f);
        tree();
        glPopMatrix();
    }

    float X2[] = {30.0f, -20.0f, 30.0f, 65.0f};
    float Z2[] = {-80.0f, -20.0f, 35.0f, 50.0f};
    for (int i = 0; i < 4; ++i) {
        float scale = 2.0f;
        glPushMatrix();
        glTranslatef(X2[i], 0.0f, Z2[i]);
        glScalef(scale, scale, scale);
        pinetree1();
        glPopMatrix();
    }

    float X3[] = {-45.0f, -50.0f, 40.0f};
    float Z3[] = {-50.0f, 35.0f, -35.0f};
    for (int i = 0; i < 3; ++i) {
        float scale = 2.5f;
        glPushMatrix();
        glTranslatef(X3[i], 0.0f, Z3[i]);
        glScalef(scale, scale, scale);
        pinetree2();
        glPopMatrix();
    }

    for (int i = 0; i < 360; i += 15) {
    glPushMatrix();

    float angleOffset = 5.0f * sin(i * 3.14f / 180.0f * 3);
    float angleDeg = i + angleOffset;
    float angleRad = angleDeg * 3.14f / 180.0f;

    glRotatef(angleDeg, 0.0f, 1.0f, 0.0f);

    float radiusOffset = 5.0f * cos(i * 3.14f / 180.0f * 5);
    float z = 155.0f + radiusOffset;

    float scale = 15.0f + 0.5f * sin(i * 3.14f / 180.0f * 7);

    glTranslatef(0.0f, -0.9f, z);
    glScalef(scale - 1.0f, scale + 8.0f, scale - 1.0f);

    float localRot = 20.0f * sin(i * 3.14f / 180.0f * 8) + 10.0f * cos(i * 3.14f / 180.0f * 11);
    glRotatef(localRot, 0.0f, 1.0f, 0.0f);

    rock();
    glPopMatrix();

    // Manually compute rotated position
    float x = z * sin(angleRad); // rotated x
    float realZ = z * cos(angleRad); // rotated z
}


    float X4[] = {-20.0f, 20.0f, 40.0f, 68.5f, 80.0f, 30.0f, 40.0f, -70.0f, -80.0f};
    float Z4[] = {75.0f, 75.0f, 80.0f, 20.0f, 25.0f, -55.0f, -55.0f, -40.0f, -40.0f};
    float R4[] = {0.0f, 15.0f, -20.0f, 75.0f, 100.0f, 30.0f, -20.0f, 45.0f, -45.0f};
    for (int i = 0; i < 9; ++i) {
        glPushMatrix();
        glTranslatef(X4[i], 0.0f, Z4[i]);
        glRotatef(R4[i], 0.0f, 1.0f, 0.0f);
        bush();
        glPopMatrix();
    }

    float X5[] = {60.0f, 65.0f, 70.0f, 62.5f, 67.5f, 64.5f};
    float Z5[] = {-35.0f, -30.0f, -25.0f, -33.0f, -27.0f, -31.0f};
    float Y5[] = {0.0f, 0.0f, 0.0f, 3.0f, 3.5f, 6.0f};
    float S5[] = {2.5f, 2.7f, 2.3f, 2.0f, 1.7f, 1.4f};
    float R5[] = {0.0f, 15.0f, -10.0f, 20.0f, -15.0f, 30.0f};
    for (int i = 0; i < 6; ++i) {
        glPushMatrix();
        glTranslatef(X5[i], Y5[i], Z5[i]);
        glRotatef(R5[i], 0.0f, 1.0f, 0.0f);
        glScalef(S5[i], S5[i], S5[i]);
        rock();
        glPopMatrix();
    }

    float X6[] = {10.0f, 60.5f, 62.3f, 91.1f, 46.8f, 13.7f, -77.4f, 33.2f, -58.9f, 5.6f};
    float Z6[] = {-45.2f, 73.6f, -12.8f, 29.4f, -66.7f, 88.0f, -30.9f, 19.3f, 59.1f, -70.5f};
    float S6[] = {1.3f, 1.1f, 1.7f, 1.5f, 1.2f, 1.0f, 1.6f, 1.4f, 1.8f, 1.2f};
    float R6[] = {22.0f, 137.0f, 89.0f, 45.0f, 270.0f, 185.0f, 330.0f, 78.0f, 125.0f, 240.0f};
    for (int i = 0; i < 10; ++i) {
        glPushMatrix();
        glTranslatef(X6[i], 0.0f, Z6[i]);
        glRotatef(R6[i], 0.0f, 1.0f, 0.0f);
        glScalef(S6[i], S6[i], S6[i]);
        rock();
        glPopMatrix();
    }

    glEnable(GL_CULL_FACE);
}
