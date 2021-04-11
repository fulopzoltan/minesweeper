#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
class GameBoard : public QObject {
  Q_OBJECT
private:
  int boardRows;
  int boardColumns;
  int numberOfBombs;

public:
  explicit GameBoard(QObject *parent = nullptr);
  void calculateNumbersOnTiles();
  void makeBoard(int rows, int columns, int bombs);
  std::vector<std::vector<int>> board;
signals:
};

#endif // GAMEBOARD_H
