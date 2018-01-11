#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>

#include <memory>

class DoodleJump;
class GameRenderWidget;

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    void setScore(int score);

private:
    Ui::GameWindow *ui;

    std::unique_ptr<DoodleJump> m_doodleJump;
    std::unique_ptr<GameRenderWidget> m_gameRenderWidget;

    void initGameRenderWidget();
    void setGLFormat();

private slots:
    void startButtonPressed();
    void restartButtonPressed();
    void pauseButtonPressed();
};

#endif // GAMEWINDOW_H
