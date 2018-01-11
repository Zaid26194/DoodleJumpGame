#ifndef DOODLEJUMP_H
#define DOODLEJUMP_H

#include <memory>
#include <string>

#include "engine/input/inputselector.h"
#include "engine/graphics/camera/camera2d.h"
#include "engine/graphics/texture/texturecache.h"
#include "engine/graphics/util/util.h"
#include "engine/physics/orientation2d.h"
#include "game/entity/doodler.h"
#include "game/entity/jumppad.h"
#include "game/input/inputmapper.h"
#include "game/input/playercommands.h"
#include "engine/graphics/sprite2d/simplesprite2d.h"
#include "game/graphics/multisprite/multisprite.h"

#include "tinyxml2.h"

#include "engine/input/inputselector.h"
#include "game/input/qttobxinputmapper.h"

#include <QMessageBox>

class GraphicsObject;
class Shader;
class SpriteBatcher;
class TextureLoader;

struct SimpleSprite2DFileData;
struct MultiSpriteFileData;


class DoodleJump
{
public:
    DoodleJump();
    ~DoodleJump();

    // Graphics
    void initializeGraphics();
    void drawFrame();
    void resize( Rect2D<int> dims );
    void setAspectRatio( float aspectRatio );

    // Time
    void tick( float seconds );

    // Game I/O
    void startButtonPressed();
    void restartButtonPressed();
    void pauseButtonPressed();

    int getScore() { return m_score; }

    void setPlayerCommands(PlayerCommands incomingPC) { pc = incomingPC; }


private:
    void loadGraphicsAssets();
    void loadShaders();
    void loadSprites();
    void loadDoodlerSprite();
    void loadJumpPad();
    void loadEnemy();
    void loadBullet();
    void loadBackgroundSprite();

    int texPathToSpriteHandle( const std::string &path );

    SimpleSprite2DData spriteFileDataToSpriteData( const SimpleSprite2DFileData& d);
    MultiSpriteData multiSpriteFileDataToMultiSpriteData( const MultiSpriteFileData& fdata );

    PlayerCommands pc;

    void clearScreen();

    void setBlendState();
    void setCameraState();

    void queueSprites();
    void drawSprites();
    void queueBackgroundSprite();
    void queueDoodlerSprite();
    void queueInitialJumpPadSprite();
    void queueJumpPadSprite();
    void queueEnemySprite();
    void queueBulletSprite();

    void drawBackground();

    void generateJumpPad();
    void generateEnemy();

    void moveLeft();
    void moveRight();
    void shoot();
    void falling();
    void bounceFromPlatform();
    void killedByNinja();
    void changeCameraHeight();

    void Swap(SimpleSprite2D &first, SimpleSprite2D &second);

    // Sprite example
    std::unique_ptr<SimpleSprite2D> m_backgroundSprite;
    SimpleSprite2DData m_backgroundSpriteData;

    // MultiSprite example
    MultiSpriteData m_doodlerSpriteData;
    std::unique_ptr<MultiSprite> m_doodlerSprite;

    //JumpPad Sprite
    std::unique_ptr<SimpleSprite2D> m_jumpPadSprite;
    SimpleSprite2DData m_jumpPadSpriteData;
    std::vector<SimpleSprite2D> JumpPadBuffer;

    //Enemies Sprite
    std::unique_ptr<SimpleSprite2D> m_ninjaDoodlerSprite;
    SimpleSprite2DData m_ninjaDoodlerSpriteData;
    std::vector<SimpleSprite2D> NinjaDoodlerBuffer;

    //Bullet Sprite
    std::unique_ptr<SimpleSprite2D> m_doodleBulletSprite;
    SimpleSprite2DData m_doodleBulletSpriteData;
    std::vector<SimpleSprite2D> DoodleBulletBuffer;

    // Graphics
    Camera2D m_camera;
    std::unique_ptr<GraphicsObject> m_graphicsObject;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<TextureLoader> m_textureLoader;
    TextureCache m_textureCache;
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;

    float m_gameWidth;

    float dy = 0;
    float maxDoodleHeight = 0;

    Orientation2D doodler;
    Orientation2D yMovement;

    //Physics stuff
    float velocity = 0;
    float initialVelocity = .21f;
    float acceleration = -0.8f;
    float Time = 0;                 //had to make this capital because srand(time(0)) is a thing
    float height = 0;

    // Game state
    int m_score;
    bool gameRunning = true;
    bool bulletDeletionFlag = false;
    bool qMessageBoxFlag = false;

    //variables to deal with random
    float floor = 2.5;
    float ceiling = -2.5f;
    float range = (ceiling - floor);

    //Qstuff
    QMessageBox qmessagebox;
};

#endif // DOODLEJUMP_H
