#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameboard.h"
#include "tile.h"
#include <QMainWindow>
#include <QObject>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  GameBoard gameBoard;

private:
  Ui::MainWindow *ui;
  void initBoard(int rows, int cols, int bombs);
  void revealAdjacent(int x, int y);
  void revealAllBombs();
  struct Settings {
    int rows;
    int cols;
    int bombs;
  };
  Settings settings;
  void deleteTilesOnBoard();
  QTimer *timer;
  int time = 0;
  int numberRevealed;
  int flagCounter = 0;

public slots:
  void onLeftClicked();
  void onRightClicked();
  void restartGame();
signals:
  void revealedChanged(int);
  void flagChanged(int);
};
#endif // MAINWINDOW_H
