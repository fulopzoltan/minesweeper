#include "gameboard.h"
#include <QDebug>

GameBoard::GameBoard(QObject *parent) : QObject(parent) {}

void GameBoard::calculateNumbersOnTiles() {
  int rows[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int columns[] = {-1, -1, -1, 0, 1, 1, 1, 0};
  for (int i = 0; i < boardRows; ++i) {
    for (int j = 0; j < boardColumns; ++j) {
      if (board[i][j] == 9)
        continue;
      for (int k = 0; k < 8; k++) {
        if (i + rows[k] >= 0 && i + rows[k] < boardRows &&
            j + columns[k] >= 0 && j + columns[k] < boardColumns) {
          board[i][j] = board[i + rows[k]][j + columns[k]] == 9
                            ? board[i][j] += 1
                            : board[i][j];
        }
      }
    }
  }
}

void GameBoard::makeBoard(int rows, int columns, int bombs) {

  numberOfBombs = bombs;
  boardColumns = columns;
  boardRows = rows;
  board.resize(boardRows);
  for (auto &row : board) {
    row.resize(boardColumns);
  }
  for (auto &row : board) {
    fill(row.begin(), row.end(), 0);
  }

  int count = 0;
  int prob = (numberOfBombs * 100) / (boardColumns * boardRows);
  srand(time(nullptr));
  board[rand() % rows][rand() % columns] = 9;
  count++;
  QDebug debug(QtDebugMsg);
  while (count != numberOfBombs) {
    for (int i = 0; i < boardRows; ++i) {
      for (int j = 0; j < boardColumns; ++j) {
        if (count == numberOfBombs)
          break;
        if (rand() % 101 < prob) {
          board[i][j] = 9;
          count++;
        }
      }
    }
  }
  calculateNumbersOnTiles();
  for (int i = 0; i < boardRows; ++i) {
    for (int j = 0; j < boardColumns; ++j) {
      debug << board[i][j];
    }
    debug << "\n";
  }
}
