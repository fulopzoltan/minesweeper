#include "mainwindow.h"
#include "gameboard.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->gridLayout->setSpacing(3);
  ui->restart->setIconSize(QSize(20, 20));
  ui->restart->setIcon(QIcon(":/resources/icons/restart.png"));
  connect(ui->restart, &QPushButton::clicked, this, &MainWindow::restartGame);
  settings.cols = 9;
  settings.rows = 9;
  settings.bombs = 10;
  setFixedSize(250, 310);
  timer = new QTimer();
  time = 0;
  connect(this, &MainWindow::flagChanged,
          [&](int newVal) { ui->bombs->display(settings.bombs - newVal); });
  connect(timer, &QTimer::timeout, [&]() { ui->time->display(++time); });
  connect(this, &MainWindow::revealedChanged, [&](int newVal) {
    qDebug() << newVal;
    if (newVal == ((settings.rows * settings.cols) - settings.bombs)) {
      timer->stop();
    }
  });
  connect(ui->actionBeginner, &QAction::triggered, [&]() {
    deleteTilesOnBoard();
    setFixedSize(250, 310);
    settings.cols = 9;
    settings.rows = 9;
    settings.bombs = 10;
    timer->stop();
    time = 0;
    ui->time->display(time);
    initBoard(settings.rows, settings.cols, settings.bombs);
  });
  connect(ui->actionIntermediate, &QAction::triggered, [&]() {
    deleteTilesOnBoard();

    settings.cols = 16;
    settings.rows = 16;
    settings.bombs = 40;
    setFixedSize(420, 480);
    timer->stop();
    time = 0;
    ui->time->display(time);
    initBoard(settings.rows, settings.cols, settings.bombs);
  });
  connect(ui->actionExpert, &QAction::triggered, [&]() {
    deleteTilesOnBoard();
    settings.cols = 30;
    settings.rows = 16;
    settings.bombs = 99;
    setFixedSize(750, 480);
    timer->stop();
    time = 0;
    ui->time->display(time);
    initBoard(settings.rows, settings.cols, settings.bombs);
  });
  initBoard(settings.rows, settings.cols, settings.bombs);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initBoard(int rows, int cols, int bombs) {
  ui->bombs->display(bombs);
  numberRevealed = 0;
  gameBoard.makeBoard(rows, cols, bombs);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Tile *tile = new Tile;
      tile->setFixedSize(21, 21);
      tile->x = i;
      tile->y = j;
      ui->gridLayout->addWidget(tile, i, j);
      connect(tile, &Tile::leftClicked, this, &MainWindow::onLeftClicked);
      connect(tile, &Tile::rightClicked, this, &MainWindow::onRightClicked);
    }
  }
}

void MainWindow::revealAdjacent(int x, int y) {
  int rows[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int columns[] = {-1, -1, -1, 0, 1, 1, 1, 0};

  for (int k = 0; k < 8; ++k) {
    if (x + rows[k] >= 0 && x + rows[k] < settings.rows &&
        y + columns[k] >= 0 && y + columns[k] < settings.cols) {
      Tile *tile = qobject_cast<Tile *>(
          ui->gridLayout->itemAtPosition(x + rows[k], y + columns[k])
              ->widget());
      if (tile->revealed)
        continue;
      ;
      if (gameBoard.board[tile->x][tile->y] == 9) {
        continue;
      }
      tile->revealSelf(gameBoard.board[tile->x][tile->y]);
      tile->revealed = true;
      emit revealedChanged(++numberRevealed);

      if (gameBoard.board[tile->x][tile->y] == 0) {
        revealAdjacent(tile->x, tile->y);
      }
    }
  }
}

void MainWindow::revealAllBombs() {
  timer->stop();

  for (int i = 0; i < settings.rows; i++) {
    for (int j = 0; j < settings.cols; j++) {
      Tile *tile =
          qobject_cast<Tile *>(ui->gridLayout->itemAtPosition(i, j)->widget());
      if (gameBoard.board[i][j] == 9) {
        tile->revealSelf(9);
        continue;
      }
      tile->setDisabled(true);
    }
  }
}

void MainWindow::deleteTilesOnBoard() {
  for (int i = 0; i < settings.rows; i++) {
    for (int j = 0; j < settings.cols; j++) {
      delete ui->gridLayout->itemAtPosition(i, j)->widget();
    }
  }
}

void MainWindow::onLeftClicked() {
  if (!timer->isActive()) {
    timer->start(1000);
  }
  auto *tile = qobject_cast<Tile *>(sender());
  if (!tile->flagged) {

    if (gameBoard.board[tile->x][tile->y] == 9) {
      revealAllBombs();
    }
    tile->revealSelf(gameBoard.board[tile->x][tile->y]);
    tile->revealed = true;
    emit revealedChanged(++numberRevealed);

    if (gameBoard.board[tile->x][tile->y] != 0) {
      return;
    }
    revealAdjacent(tile->x, tile->y);
  }
}

void MainWindow::onRightClicked() {
  auto *tile = qobject_cast<Tile *>(sender());
  if (tile->flagged) {
    tile->setIcon(QIcon());
    tile->flagged = false;
    emit flagChanged(--flagCounter);
  } else {
    if (flagCounter == settings.bombs)
      return;
    tile->setIcon(QIcon(":/resources/icons/flag.png"));
    tile->flagged = true;
    emit flagChanged(++flagCounter);
  }
}

void MainWindow::restartGame() {
  time = 0;
  ui->time->display(time);
  timer->stop();
  numberRevealed = 0;
  emit flagChanged(flagCounter = 0);
  gameBoard.makeBoard(settings.rows, settings.cols, settings.bombs);
  for (int i = 0; i < settings.rows; i++) {
    for (int j = 0; j < settings.cols; j++) {
      Tile *tile =
          qobject_cast<Tile *>(ui->gridLayout->itemAtPosition(i, j)->widget());
      tile->resetSelf();
    }
  }
}
