#ifndef GAMERENDERWIDGET_H
#define GAMERENDERWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>

#include "game/input/qttobxinputmapper.h"
#include "engine/input/inputselector.h"
#include "game/input/inputmapper.h"

class DoodleJump;

namespace Ui {
class GameRenderWidget;
}

class GameRenderWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameRenderWidget(QWidget *parent = 0);
    ~GameRenderWidget();

    void registerGame( DoodleJump* );

protected:
    virtual void mousePressEvent( QMouseEvent* ) override;
    virtual void mouseMoveEvent( QMouseEvent* ) override;
    virtual void mouseReleaseEvent( QMouseEvent* ) override;

    virtual void keyPressEvent( QKeyEvent* ) override;
    virtual void keyReleaseEvent( QKeyEvent* ) override;

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    /*QtToBXInputMapper getQtToBXInputMapper() { return qbim; }
    InputSelector getInputSelector() { return is; }
    InputMapper getInputMapper() { return im; }
    PlayerCommands getPlayerCommands() { return pc; }*/

private:
    Ui::GameRenderWidget *ui;

    QtToBXInputMapper qbim;
    InputSelector is;
    InputMapper im;
    PlayerCommands tempPC;

    QTimer m_timer;
    DoodleJump *m_doodleJump;

    void osSpecificSetup();
    void setupMac();
    void setupWindows();
    void timerSetup();
};

#endif // GAMERENDERWIDGET_H
