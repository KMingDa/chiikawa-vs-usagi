#ifndef CGPROJECT_HPP
#define CGPROJECT_HPP

#include "CGProjectmain.hpp"
#include "CGimageloader.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace CGProject {

struct BoundingBox {
    float xMin, xMax;
    float yMin, yMax;
    float zMin, zMax;
};

// Character class to handle Chikawas and Usagi
class Characters
{
public:
    struct DirtParticle {
        float x, y, z;        // Position
        float vx, vy, vz;     // Velocity
        float life;           // Remaining lifetime
        bool active;
    };

    static const int MAX_PARTICLES = 50;
    DirtParticle dirtParticles[MAX_PARTICLES];


    struct SmokeParticle {
        float x, y, z;
        float vx, vy, vz;
        float size;
        float alpha;
        float life;
    };
    std::vector<SmokeParticle> explosionSmoke;


    struct Particle {
        float x, y, z;
        float r, g, b, a;
        float life;
    };
    std::vector<Particle> projectileTrail;
    float projectileTrailTime = 0.0f;


    static const int MAX_CONFETTI = 100;  // Make it static const
    struct Confetti {
        float x, y, z;
        float velocityY;
        float r, g, b;
        float rotX, rotY, rotZ;
        float rotSpeedX, rotSpeedY, rotSpeedZ;
        bool active;
    };

    Confetti confetti[MAX_CONFETTI]; // now valid
    bool confettiInitialized = false;
    int currentConfettiIndex = 0;
    float confettiSpawnTimer = 0.0f;

     void draw();
     void tickTime(long int elapseTime);

    float chikawa1X, chikawa1Y, chikawa1Z;
    float usagiX, usagiY, usagiZ;
    void setTextures(GLuint* textures);
    BoundingBox chiikawaBox;
    BoundingBox usagiBox;
    void updateBoundingBoxes();
    bool checkCharacterCollision();



    float chikawa1Angle;
    float usagiAngle;
    float healthChikawa;
    float healthUsagi;

    float startRotationChikawa1;
    float targetRotationChikawa1;
    float currentRotationChikawa1;
    float rotationTimeChikawa1;
    float rotationDuration;

    float startRotationUsagi1;
    float targetRotationUsagi1;
    float currentRotationUsagi1;
    float rotationTimeUsagi1;

    float legSwingAngleChikawa1;
    float legSwingAngleUsagi1;

    float legSwingTimeChikawa1;
    float legSwingTimeUsagi1;

    float prevChikawa1X;
    float prevChikawa1Z;

    float prevUsagiX;
    float prevUsagiZ;

    bool attackUsagi;
    float attackAngleUsagi;
    bool attackInProgress;

    bool attackChikawa;
    float attackAngleChikawa;
    bool attackInProgress2;

    bool hasFiredProjectile;

    bool isMovingChikawa1;
    bool isMovingUsagi1;

    bool projectileActive;
    float projectileX, projectileY, projectileZ;

    float projectileVelX;
    float projectileVelZ;

    bool chiikawaHit;
    float chiikawaHitTimer;

    bool chiikawaInAir;
    float chiikawaHitTime;
    float chiikawaAirDuration;
    float chiikawaInitialY;
    float chiikawaAirPeak;

    float chiikawaAirVelX;
    float chiikawaAirVelZ;
    float bounceOffset;

    bool usagiHit;

    bool  usagiInAir;
    float usagiHitTime;
    float usagiAirDuration;
    float usagiInitialY;
    float usagiAirPeak;

    float usagiAirVelX;
    float usagiAirVelZ;

    float hitEffectTimeChikawa;
    bool playHitEffectChikawa;

    float hitEffectTimeUsagi;
    bool playHitEffectUsagi;

    bool explosionActive;
    float explosionX, explosionY, explosionZ;
    float explosionTime;
    const float explosionDuration;

    bool moveForwardChikawa1, moveBackwardChikawa1, moveLeftChikawa1, moveRightChikawa1;
    bool moveForwardUsagi, moveBackwardUsagi, moveLeftUsagi, moveRightUsagi;

    // Defeated state flags
    bool isChiikawaDefeated;
    bool isUsagiDefeated;
    GLuint* textureNumbers = NULL;
    // Fall animation variables
    float chiikawaFallAngle;
    float usagiFallAngle;
    float fallAnimationTimeChikawa;
    float fallAnimationTimeUsagi;
    const float fallDuration;
    void draw3DStar(float size, float thickness, int points, float rotationDegrees);
    void initConfetti(float centerX, float centerY, float centerZ);
    void updateAndDrawConfetti(float deltaTime);

    Characters() : explosionDuration(0.5f), fallDuration(0.5f) // Initializer list for const members
    {
        // --- ALL VARIABLES ARE INITIALIZED HERE ---
        chikawa1X = -9.0f;
        chikawa1Y = 0.0f;
        chikawa1Z = 35.0f;
        usagiX = 9.0f;
        usagiY = 0.0f;
        usagiZ = 35.0f;
        chikawa1Angle = 0.0f;
        usagiAngle = 0.0f;
        healthChikawa = 100.0f;
        healthUsagi = 100.0f;

        startRotationChikawa1 = 0.0f;
        targetRotationChikawa1 = 0.0f;
        currentRotationChikawa1 = 0.0f;
        rotationTimeChikawa1 = 0.0f;
        rotationDuration = 0.3;

        startRotationUsagi1 = 0.0f;
        targetRotationUsagi1 = 0.0f;
        currentRotationUsagi1 = 0.3f;
        rotationTimeUsagi1 = 0.0f;

        legSwingAngleChikawa1 = 0.0f;
        legSwingAngleUsagi1 = 0.0f;
        legSwingTimeChikawa1 = 0.0f;
        legSwingTimeUsagi1 = 0.0f;

        prevChikawa1X = 0.0f;
        prevChikawa1Z = 0.0f;
        prevUsagiX = 0.0f;
        prevUsagiZ = 0.0f;

        attackUsagi = false;
        attackAngleUsagi = 40.0f;
        attackInProgress = false;

        attackChikawa = false;
        attackAngleChikawa = 40.0f;
        attackInProgress2 = false;

        hasFiredProjectile = false;
        isMovingChikawa1 = false;
        isMovingUsagi1 = false;

        projectileActive = false;
        projectileX = 0.0f;
        projectileY = 0.0f;
        projectileZ = 0.0f;
        projectileVelX = 0.0f;
        projectileVelZ = 0.0f;

        chiikawaHit = false;
        chiikawaHitTimer = 0.0f;
        chiikawaInAir = false;
        chiikawaHitTime = 0.0f;
        chiikawaAirDuration = 0.6f;
        chiikawaInitialY = 0.0f;
        chiikawaAirPeak = 9.0f;
        chiikawaAirVelX = 0.0f;
        chiikawaAirVelZ = 0.0f;
        bounceOffset = 0.0f;

        usagiHit = false;
        usagiInAir = false;
        usagiHitTime = 0.0f;
        usagiAirDuration = 0.6f;
        usagiInitialY = 0.0f;
        usagiAirPeak = 9.0f;
        usagiAirVelX = 0.0f;
        usagiAirVelZ = 0.0f;

        hitEffectTimeChikawa = 0.0f;
        playHitEffectChikawa = false;
        hitEffectTimeUsagi = 0.0f;
        playHitEffectUsagi = false;

        explosionActive = false;
        explosionX = 0.0f;
        explosionY = 0.0f;
        explosionZ = 0.0f;
        explosionTime = 0.0f;

        isChiikawaDefeated = false;
        isUsagiDefeated = false;
        chiikawaFallAngle = 0.0f;
        usagiFallAngle = 0.0f;
        fallAnimationTimeChikawa = 0.0f;
        fallAnimationTimeUsagi = 0.0f;

        bounceTimeChikawa1 = 0.0f;
        bounceTimeUsagi = 0.0f;
    }

    void drawSingleChikawa(float r, float g, float b);
    void drawUsagi(float r, float g, float b);
    void drawChiikawaAndUsagi();
    void updateMovement();
    void fireProjectileFromUsagi();
    void updateAndDrawProjectile();
    void drawDebugArrowAboveUsagi();
    void drawDebugArrowAboveChiikawa();
    bool checkProjectileCollisionWithChiikawa();
    bool checkProjectileCollisionWithUsagi();
    void drawHitEffect(float x, float y, float z, float timeElapsed);
    void updateAndDrawExplosion(float deltaTime);

    void moveChikawa(float dx, float dy, float dz, float angle)
    {
        float originalX = chikawa1X;
        float originalZ = chikawa1Z;
        chikawa1X += dx;
        chikawa1Z += dz;
        updateBoundingBoxes(); // Update after moving

         if (checkCharacterCollision()) {
        // Revert movement if collision detected
        chikawa1X = originalX;
        chikawa1Z = originalZ;
        } else {
            startRotationChikawa1 = currentRotationChikawa1;
            targetRotationChikawa1 = angle;
            rotationTimeChikawa1 = 0.0f;

            isMovingChikawa1 = (dx != 0 || dz != 0);
            if (isMovingChikawa1 && legSwingTimeChikawa1 == 0.0f)
                legSwingTimeChikawa1 = 0.001f;
        }
    }

    void moveUsagi(float dx, float dy, float dz, float angle)
    {
        float originalX = usagiX;
        float originalZ = usagiZ;

        usagiX += dx;
        usagiZ += dz;

        updateBoundingBoxes();

        if (checkCharacterCollision()) {
            usagiX = originalX;
            usagiZ = originalZ;
        } else {
            startRotationUsagi1 = currentRotationUsagi1;
            targetRotationUsagi1 = angle;
            rotationTimeUsagi1 = 0.0f;

            isMovingUsagi1 = (dx != 0 || dz != 0);
            if (isMovingUsagi1 && legSwingTimeUsagi1 == 0.0f)
                legSwingTimeUsagi1 = 0.001f;
        }
    }

private:
    float bounceTimeChikawa1;
    float bounceTimeUsagi;

    const float bounceAmplitude = 0.5f;
    const float bounceSpeed = 0.005f;
};

class Weapon
{
    public:
        Weapon() { }
        ~Weapon() { }
        void setup(const GLfloat* controlPoints,GLint uOrder);
        void drawChiikawaWeapon(GLenum draw_mode = GL_LINE, GLint ures = 100);
        void drawControlPoints();
        void evaluateBezierPoint(float t, float* out);
        void drawUsagiWeapon();
    private:
        GLint uorder;
        const GLfloat* controlpoints;
        GLUquadricObj* pObj;
};

class Background
{
public:
    void grass();
    void tree();
    void pinetree1();
    void pinetree2();
    void bush();
    void rock();
    void pudding();
    void sun();
    void moon();
    void draw();
    void setTextures(GLuint* textures);
private:
    GLuint* textureNumbers = NULL;
};

class MySweepSurface
{
public:
 MySweepSurface() {
     profilepoints = NULL;
     surfacepoints = NULL;
 }
 ~MySweepSurface()
 {
 if (surfacepoints) delete[] surfacepoints;
 }
 void setup(const GLfloat* profilePoints,
 GLint numOfProfilePoints,
 GLfloat degreeStart,
 GLfloat degreeEnd,
 GLfloat degreeStep );
 void drawbowl();
 void drawmountain();
private:
 const GLfloat* profilepoints;
 GLint numofprofilepoints;
 GLfloat *surfacepoints;
 GLint numofsurfacepoints;
 GLint numofverticallines;
};

class MyVirtualWorld
{
public:
    Characters characters;
    Weapon weapon;
    Background bg;
    MySweepSurface bowlsurface;
    MySweepSurface mountainSurface;

    MyVirtualWorld();
    ~MyVirtualWorld();

    void setupTextures();

    long int timeold, timenew, elapseTime;

    void draw()
    {
        bg.pudding();
        bg.draw();
        bg.sun();
        bowlsurface.drawbowl();
        mountainSurface.drawmountain();

        characters.drawChiikawaAndUsagi();
        weapon.drawChiikawaWeapon(GL_LINE);
        weapon.drawUsagiWeapon();
    }

    void tickTime()
     {
         timenew = glutGet(GLUT_ELAPSED_TIME);
         elapseTime = timenew - timeold;
         timeold = timenew;
         characters.tickTime(elapseTime);
     }

    void init()
    {
         static GLfloat profilepoints[] =
         {  5.0f, 0.0f, 0.0f,
            4.0f, 1.0f, 0.0f,
            1.0f, 2.0f, 0.0f,
            1.0f, 6.0f, 0.0f,
            6.0f, 7.0f, 0.0f,
            6.0f, 7.0f, 0.0f,
            7.0f, 8.0f, 0.0f,
            7.0f, 10.0f, 0.0f
         };
         bowlsurface.setup(profilepoints, 8, 0, 360, 5);

        static GLfloat profile[] = {
            300.0f,  -100.0f,   0.0f,
            200.0f,  -30.0f,  0.0f,
            180.0f,  -0.1f,  0.0f
        };
        mountainSurface.setup(profile, 3, 0.0f, 360.0f, 10.0f);

        setupTextures();
    }

    void drawHealthBar3D(float x, float y, float z, float health, float maxHealth, const char* name);
    bool showHealthBars; // Added: Flag to control health bar visibility
    float healthBarAnimationProgress; // Added: Progress of the health bar appearance animation (0.0 to 1.0)
    const float healthBarAnimationDuration = 1.0f; // Added: Duration of the health bar appearance animation in seconds


private:
    int numberOfTexture;
    GLuint* textureNumbers;
    bool textureison;

};

};

#endif
