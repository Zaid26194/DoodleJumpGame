#include "GL/glew.h"

#include "gamerenderwidget.h"
#include "ui_gamerenderwidget.h"

#include <QKeyEvent>
#include <QGLFormat>

#include "gamewindow.h"
#include "game/doodlejump.h"


GameRenderWidget::GameRenderWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::GameRenderWidget)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    timerSetup();
}

GameRenderWidget::~GameRenderWidget()
{
    delete ui;
}

void GameRenderWidget::registerGame(DoodleJump *doodleJump)
{
    m_doodleJump = doodleJump;
}


void GameRenderWidget::mousePressEvent( QMouseEvent* event )
{
}

void GameRenderWidget::mouseMoveEvent( QMouseEvent* )
{
}

void GameRenderWidget::mouseReleaseEvent( QMouseEvent* event )
{
}

void GameRenderWidget::keyPressEvent( QKeyEvent* key )
{
    is.pressKey(qbim.key(key->key()));
    tempPC = im.keysToCommands(is);
    m_doodleJump->setPlayerCommands(tempPC);
}

void GameRenderWidget::keyReleaseEvent( QKeyEvent* key )
{
    is.releaseKey(qbim.key(key->key()));
    tempPC = im.keysToCommands(is);
    m_doodleJump->setPlayerCommands(tempPC);
}

void GameRenderWidget::initializeGL()
{
    m_doodleJump->initializeGraphics();
}

void GameRenderWidget::paintGL()
{
    m_doodleJump->tick( 1.0f/60.0f );
    m_doodleJump->drawFrame();
    dynamic_cast<GameWindow*>(this->parentWidget())->setScore( m_doodleJump->getScore());
}

void GameRenderWidget::timerSetup()
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));

    if(format().swapInterval() == -1)
    {
        // VSync unavailable
        fprintf( stderr, "Swap Buffers at v_blank not available: refresh at approx 60fps.\n" );
        m_timer.setInterval(10);
    }
    else
    {
        // VSync available
        m_timer.setInterval(0);
    }
    m_timer.start();
}

void GameRenderWidget::resizeGL(int w, int h)
{
    m_doodleJump->resize( Rect2D<int>{w,h} );
}
