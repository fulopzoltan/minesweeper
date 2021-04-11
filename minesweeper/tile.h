#ifndef TILE_H
#define TILE_H

#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QWidget>
class Tile : public QPushButton {
  Q_OBJECT
public:
  explicit Tile(QWidget *parent = nullptr);
  int number = 0;
  bool revealed;
  bool flagged;
  void mousePressEvent(QMouseEvent *) override;
  int x, y;
  void revealSelf(int num);
  void resetSelf();
signals:
  void rightClicked();
  void leftClicked();
public slots:
};

#endif // TILE_H
