#include "doodlejump.h"

#include <fstream>

#include "engine/graphics/graphicsobject.h"
#include "engine/graphics/dataload/shaders/shaderfilerepository.h"
#include "engine/graphics/primitives/fullscreenquad.h"
#include "engine/graphics/shaders/shader.h"
#include "engine/graphics/sprite2d/spritebatcher.h"
#include "engine/graphics/texture/textureloader.h"
#include "engine/physics/orientation2dutils.h"

#include "game/graphics/DataParse/simplesprite2dfileparser.h"
#include "game/graphics/DataParse/multispritefileparser.h"
#include "engine/graphics/sprite2d/simplesprite2dfiledata.h"
#include "game/graphics/DataParse/multispritefiledata.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "tinyxml2.h"

#include <stdlib.h>
#include <time.h>


DoodleJump::DoodleJump() :
    m_camera( 800, 600 ),
    m_textureLoader( std::make_unique<TextureLoader>() ),
    m_gameWidth( 5.0f ),
    m_score( 0 )
{
    srand(time(0));
    auto p = m_camera.getPosition();
    p.y += 2.0f;
    m_camera.setPosition( p );
}

DoodleJump::~DoodleJump()
{
    // Rule of 0?
}

void DoodleJump::startButtonPressed()
{
}

void DoodleJump::restartButtonPressed()
{
}

void DoodleJump::pauseButtonPressed()
{
}

void DoodleJump::loadSprites()
{
    m_spriteBatcher->clearSprites();
    loadJumpPad();
    generateJumpPad();
    loadEnemy();
    generateEnemy();
    loadDoodlerSprite();
    loadBullet();
    loadBackgroundSprite();
}

void DoodleJump::loadBackgroundSprite()
{
    // Load Sprite2D files to Sprite2D data structs
    // Turn Sprite2D data structs into Sprite2D objects
    m_backgroundSpriteData = spriteFileDataToSpriteData( pathToSimpleSprite2DFileData( "sprites/Background.xml" ) );
    m_backgroundSprite = std::make_unique<SimpleSprite2D>( m_backgroundSpriteData );
}

void DoodleJump::loadJumpPad()
{
   m_jumpPadSpriteData = spriteFileDataToSpriteData( pathToSimpleSprite2DFileData( "sprites/JumpPad.xml" ) );
   m_jumpPadSprite = std::make_unique<SimpleSprite2D>( m_jumpPadSpriteData );
}

void DoodleJump::loadEnemy()
{
   m_ninjaDoodlerSpriteData = spriteFileDataToSpriteData( pathToSimpleSprite2DFileData( "sprites/NinjaDoodler.xml" ) );
   m_ninjaDoodlerSprite = std::make_unique<SimpleSprite2D>( m_ninjaDoodlerSpriteData );
}

void DoodleJump::loadBullet()
{
   m_doodleBulletSpriteData = spriteFileDataToSpriteData( pathToSimpleSprite2DFileData( "sprites/DoodleBullet.xml" ) );
   m_doodleBulletSprite = std::make_unique<SimpleSprite2D>( m_doodleBulletSpriteData );
}

void DoodleJump::loadDoodlerSprite()
{
    MultiSpriteFileData fdata = pathToMultiSpriteFileData( "sprites/Doodler.xml" );
    std::string setState = "LeftUp";
    if( m_doodlerSprite )
    {
        setState = m_doodlerSprite->getState();
    }
    m_doodlerSpriteData = multiSpriteFileDataToMultiSpriteData( fdata );

    m_doodlerSprite = std::make_unique<MultiSprite>( m_doodlerSpriteData );
    m_doodlerSprite->setState( setState );
}

void DoodleJump::generateJumpPad(){
   auto jp = m_jumpPadSprite->getOrientation();
   for (int i=0;i<14;i++)
   {
       jp.position.x= floor + int((range * rand())) / (RAND_MAX + 1.0);
       jp.position.y= ((1+rand())/(RAND_MAX / ((m_camera.getPosition().y+10) - 1 + 1) + 1));
       m_jumpPadSprite->setOrientation(jp);
       JumpPadBuffer.push_back(*m_jumpPadSprite);
   }
}

void DoodleJump::generateEnemy(){
   auto nd = m_ninjaDoodlerSprite->getOrientation();
   nd.position.x= -2.5;
   nd.position.y= -2.5;
   m_ninjaDoodlerSprite->setOrientation(nd);
   NinjaDoodlerBuffer.push_back(*m_ninjaDoodlerSprite);
}

void DoodleJump::drawFrame()
{
    clearScreen();
    setBlendState();

    setCameraState();
    drawBackground();
    queueSprites();
    drawSprites();
}

void DoodleJump::drawBackground()
{
    m_spriteBatcher->clearInstances();
    m_graphicsObject->enableDepth();
    queueBackgroundSprite();
    drawSprites();
    m_graphicsObject->disableDepth();
}

void DoodleJump::tick(float dt )
{
    if(gameRunning)
        {

        falling();

        if(pc.moveLeft == true)
        {
            moveLeft();
        }

        if(pc.moveRight == true)
        {
            moveRight();
        }

        if(pc.shoot == true)
        {
            shoot();
        }

        if (colliding(m_doodlerSprite->getSpritesToDraw().back(),*m_jumpPadSprite) && velocity < -0.01)
        {
            bounceFromPlatform();
        }

        for ( int i = 0; i < JumpPadBuffer.size(); i++)
        {
            SimpleSprite2D pad = JumpPadBuffer[i];
            if (colliding(m_doodlerSprite->getSpritesToDraw().back(), pad) && velocity < -0.01)
            {
                bounceFromPlatform();
            }
        }

        for ( int i = 0; i < NinjaDoodlerBuffer.size(); i++)
        {
            if (colliding(m_doodlerSprite->getSpritesToDraw().back(), NinjaDoodlerBuffer[i]))
            {
                killedByNinja();
                break;
            }
        }

        for ( int i = 0; i < DoodleBulletBuffer.size(); i++)
        {
            if (DoodleBulletBuffer.size() != 0)
            {
                for (int j = 0; j < NinjaDoodlerBuffer.size(); j++)
                {
                    if (colliding(DoodleBulletBuffer[i], NinjaDoodlerBuffer[j]))
                    {
                        Orientation2D temp;
                        temp.position.x = floor + int((range * rand())) / (RAND_MAX + 1.0);
                        temp.position.y = m_camera.getPosition().y+6;
                        NinjaDoodlerBuffer[j].setOrientation(temp);
                    }
                }
            }
        }

        if (yMovement.position.y < (m_camera.getPosition().y-(m_gameWidth*m_camera.getAspectRatio())))
        {
            gameRunning = false;
        }

        changeCameraHeight();
    }

    if(!gameRunning)
    {
        falling();
        changeCameraHeight();
    }

    if (yMovement.position.y < (m_camera.getPosition().y-(m_gameWidth*m_camera.getAspectRatio())))
    {
        if(!qMessageBoxFlag)    //Required because otherwise the QMessageBox constantly keeps spawning
        {
            qMessageBoxFlag = true;
            QString score = QString::number(m_score);
            QString requiredScore = QString::number(m_score+1);
            qmessagebox.setText("Your score was " + score + ".\n"
                                                          "You needed " + requiredScore + " to win this round!\n"
                                                                                             "Try Again... Rookie!");
            qmessagebox.exec();
        }
    }
}

void DoodleJump::moveLeft()
{
    doodler.position.y = m_doodlerSprite->getOrientation().position.y;
    doodler.position.x = doodler.position.x + -0.1f;
    if(doodler.position.x < -2.6)
        doodler.position.x += 5.0;
    m_doodlerSprite->setState("LeftUp");
    m_doodlerSprite->setOrientation(doodler);
}

void DoodleJump::moveRight()
{
    doodler.position.y = m_doodlerSprite->getOrientation().position.y;
    doodler.position.x = doodler.position.x + 0.1f;
    if(doodler.position.x > 2.6)
        doodler.position.x += -5.0;
    m_doodlerSprite->setState("RightUp");
    m_doodlerSprite->setOrientation(doodler);
}

void DoodleJump::shoot()
{
    auto db = m_doodleBulletSprite->getOrientation();
    db.position.x= m_doodlerSprite->getOrientation().position.x;
    db.position.y= m_doodlerSprite->getOrientation().position.y + 0.75;
    m_doodleBulletSprite->setOrientation(db);
    m_doodlerSprite->setState("Shoot");
    DoodleBulletBuffer.push_back(*m_doodleBulletSprite);
}

void DoodleJump::falling()
{
    yMovement = m_doodlerSprite->getOrientation();
    velocity = initialVelocity + (acceleration*Time);
    height = velocity + (0.5*acceleration*Time*Time);  //I know that this equation is not the same as the equation of motion version but that's intentional
    Time += 0.01f;
    yMovement.position.y += height;
    m_doodlerSprite->setOrientation(yMovement);
}

void DoodleJump::bounceFromPlatform()
{
    Time = 0;
    velocity = initialVelocity;
}

void DoodleJump::killedByNinja()
{
    gameRunning = false;
    if(m_doodlerSprite->getState() == "LeftUp")
        m_doodlerSprite->setState("LeftDead");
    else if(m_doodlerSprite->getState() == "RightUp")
        m_doodlerSprite->setState("RightDead");
    else if(m_doodlerSprite->getState() == "Shoot")
        m_doodlerSprite->setState("LeftDead");
}

void DoodleJump::setCameraState()
{
    auto p = m_camera.getPosition();
    p.y = maxDoodleHeight;
    m_score = maxDoodleHeight;
    m_camera.setPosition( p );
}

void DoodleJump::changeCameraHeight()
{
    if(yMovement.position.y > maxDoodleHeight)
        maxDoodleHeight = yMovement.position.y;
}

void DoodleJump::queueBackgroundSprite()
{
    auto o = m_backgroundSprite->getOrientation();
    o.position = m_camera.getPosition();
    m_backgroundSprite->setOrientation( o );
    float width = m_gameWidth;
    float height = width * m_camera.getAspectRatio();
    glm::mat4 m = orientation2DToMat4( o );
    m = m * glm::scale( glm::vec3( width, height, 1.0f ) );
    m_spriteBatcher->queueSpriteInstance( m_backgroundSprite->getHandle(), m );
}

void DoodleJump::queueInitialJumpPadSprite()
{
    auto jp = m_jumpPadSprite->getOrientation();
    jp.position.x = 0;
    jp.position.y = 0;
    m_jumpPadSprite->setOrientation(jp);
    glm::mat4 model = dimsAndOrientation2DToMat4( m_jumpPadSprite->getDimensions(), m_jumpPadSprite->getOrientation() );
    m_spriteBatcher->queueSpriteInstance( m_jumpPadSprite->getHandle(), model );
}

void DoodleJump::queueJumpPadSprite()
{
   for ( int i = 0; i < JumpPadBuffer.size(); i++)
   {
       SimpleSprite2D pad = JumpPadBuffer[i];
       if (pad.getOrientation().position.y < (m_camera.getPosition().y-3.4))
       {
           auto jp = m_jumpPadSprite->getOrientation();
           jp.position.x = floor + int((range * rand())) / (RAND_MAX + 1.0);
           jp.position.y = pad.getOrientation().position.y+8;
           pad.setOrientation(jp);
           JumpPadBuffer[i].setOrientation(jp);
       }
       glm::mat4 model = dimsAndOrientation2DToMat4( pad.getDimensions(), pad.getOrientation() );
       m_spriteBatcher->queueSpriteInstance( pad.getHandle(), model );
   }
}

void DoodleJump::queueEnemySprite()
{
    if(m_score > 30)
    {
       for ( int i = 0; i < NinjaDoodlerBuffer.size(); i++)
       {
           if (NinjaDoodlerBuffer[i].getOrientation().position.y < (m_camera.getPosition().y-3.5))
           {
               auto nd = m_ninjaDoodlerSprite->getOrientation();
               nd.position.x = floor + int((range * rand())) / (RAND_MAX + 1.0);
               nd.position.y = NinjaDoodlerBuffer[i].getOrientation().position.y+20;
               NinjaDoodlerBuffer[i].setOrientation(nd);
           }
           glm::mat4 model = dimsAndOrientation2DToMat4( NinjaDoodlerBuffer[i].getDimensions(), NinjaDoodlerBuffer[i].getOrientation() );
           m_spriteBatcher->queueSpriteInstance( NinjaDoodlerBuffer[i].getHandle(), model );
       }
   }
}

void DoodleJump::queueBulletSprite()                //I know that the constant barrage of bullets is not how it is in the actual game but this is intentional
{
    int bulletDeletionFlag = 0;
    for ( int i = 0; i < DoodleBulletBuffer.size(); i++)
    {
        auto db = DoodleBulletBuffer[i].getOrientation();
        db.position.x = DoodleBulletBuffer[i].getOrientation().position.x;
        db.position.y += 0.2;
        DoodleBulletBuffer[i].setOrientation(db);
        if(DoodleBulletBuffer.size() != 0)
        {
            if (DoodleBulletBuffer[i].getOrientation().position.y > (m_camera.getPosition().y+3.5))
            {
                Swap(DoodleBulletBuffer[i], DoodleBulletBuffer[DoodleBulletBuffer.size()-1]);
                bulletDeletionFlag = 1;
            }
        }
        glm::mat4 model = dimsAndOrientation2DToMat4( DoodleBulletBuffer[i].getDimensions(), DoodleBulletBuffer[i].getOrientation() );
        m_spriteBatcher->queueSpriteInstance( DoodleBulletBuffer[i].getHandle(), model );
    }

    if(DoodleBulletBuffer.size() != 0)
    {
        if(bulletDeletionFlag == 1)
        {
            DoodleBulletBuffer.pop_back();
            bulletDeletionFlag = 0;
        }
    }
}

void DoodleJump::queueSprites()
{
    m_spriteBatcher->clearInstances();
    //queueBackgroundSprite();
    queueInitialJumpPadSprite();
    queueJumpPadSprite();
    queueDoodlerSprite();
    queueBulletSprite();
    queueEnemySprite();
}

void DoodleJump::queueDoodlerSprite()
{
    // A MultiSprite provides a list of sprites to draw at any one time
    // Draw each one of them independently
    // Consider making a MultiSprite batching function to generalize this one
    std::vector<SimpleSprite2D> doodlerSprites = m_doodlerSprite->getSpritesToDraw();
    for( int i = 0; i < doodlerSprites.size(); i++ )
    {
        SimpleSprite2D sprite = doodlerSprites[ i ];
        glm::mat4 model = dimsAndOrientation2DToMat4( sprite.getDimensions(), sprite.getOrientation() );
        m_spriteBatcher->queueSpriteInstance( sprite.getHandle(), model );
    }
}

void DoodleJump::Swap(SimpleSprite2D &first, SimpleSprite2D &second)
{
    SimpleSprite2D temp = first;
    first = second;
    second = temp;
}

// ----------------------------------------------------------------
// Graphics - Do not modify below this line
// ----------------------------------------------------------------

void DoodleJump::initializeGraphics()
{
    m_graphicsObject = std::make_unique<GraphicsObject>();
    m_graphicsObject->printGLVersionInfo();
    m_spriteBatcher = std::make_unique<SpriteBatcher>();
    loadGraphicsAssets();
}

void DoodleJump::loadGraphicsAssets()
{
    loadShaders();
    loadSprites();
}

void DoodleJump::loadShaders()
{
    ShaderFileRepository repo;
    std::ifstream vertFile( "shaders/sprite2d/sprite2d.vert" );
    std::ifstream fragFile( "shaders/sprite2d/sprite2d.frag" );
    repo.loadVertexShader( vertFile );
    repo.loadFragmentShader( fragFile );
    ShaderSourceData ssd = repo.getShaderSourceData()[0];
    m_shader = std::make_unique<Shader>( ssd.vertexSource, ssd.fragmentSource );
}

int DoodleJump::texPathToSpriteHandle( const std::string &path )
{
    TextureParams p;
    Tex2D tex = m_textureLoader->loadTex2D( path, p );
    int texHandle = m_textureCache.storeTexture( std::move( tex ) );
    return m_spriteBatcher->registerSprite( m_textureCache.getTexture( texHandle ) );
}

MultiSpriteData DoodleJump::multiSpriteFileDataToMultiSpriteData( const MultiSpriteFileData& fdata )
{
    MultiSpriteData data;

    for( auto it = fdata.states.begin(); it != fdata.states.end(); it++ )
    {
        std::string stateName = it->first;
        MultiSpriteStateFileData state = it->second;
        for( auto it2 = state.sprites.begin(); it2 != state.sprites.end(); it2++ )
        {
            std::string spriteName = it2->first;
            SimpleSprite2DFileData ssfileData = it2->second;
            data.states[ stateName ].sprites.emplace( spriteName, spriteFileDataToSpriteData( ssfileData ) );
        }
    }
    return data;
}

SimpleSprite2DData DoodleJump::spriteFileDataToSpriteData( const SimpleSprite2DFileData& d)
{
    SimpleSprite2DData data;
    data.dimensions = d.dimensions;
    data.offset = d.offset;
    data.handle = texPathToSpriteHandle( d.path );
    return data;
}

void DoodleJump::clearScreen()
{
    m_graphicsObject->enableDepth();
    m_graphicsObject->setClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    m_graphicsObject->clearColorBuffer();
    m_graphicsObject->clearDepthBuffer();
}

void DoodleJump::setBlendState()
{
    m_graphicsObject->enableBlend();
    m_graphicsObject->setBlendMode( GraphicsObject::BLEND_MODE::SRC_ALPHA, GraphicsObject::BLEND_MODE::ONE_MINUS_SRC_ALPHA );
}

void DoodleJump::drawSprites()
{
    //
    m_shader->bind();
    m_shader->applyUniform( ShaderVar( m_camera.getViewMatrix(), "viewMatrix" ) );
    m_shader->applyUniform( ShaderVar( m_camera.getProjectionMatrix(), "projectionMatrix" ) );

    m_spriteBatcher->drawSprites( m_shader.get(), "modelMatrix", "texMap" );

    m_shader->unbind();
}

void DoodleJump::resize( Rect2D<int> dims )
{
    glViewport( 0, 0, dims.width, dims.height );

    float ratio = static_cast<float>( dims.height ) / static_cast<float>( dims.width );
    m_camera.setHeight( ratio*m_gameWidth );
    m_camera.setWidth( m_gameWidth );
}
