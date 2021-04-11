#include "tile.h"
#include <QGridLayout>
Tile::Tile(QWidget *parent) : QPushButton(parent), revealed(false) {}

void Tile::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::RightButton) {
    emit rightClicked();
  }
  if (e->button() == Qt::LeftButton) {
    emit leftClicked();
  }
}

void Tile::revealSelf(int num) {
  if (revealed) {
    return;
  }
  if (num == 9) {
    setDisabled(true);
    setStyleSheet("background-color:red");
    setIcon(QIcon(":/resources/icons/bomb.png"));

    return;
  }
  if (num != 0)
    setText(QString::number(num));
  setStyleSheet("font:bold;background-color:#561996;color:#ffe4f2");
  setDisabled(true);
}

void Tile::resetSelf() {
  flagged = false;
  revealed = false;
  setText("");
  setStyleSheet("border:none;background-color:#00076f;margin:0;");
  setIcon(QIcon());
  setEnabled(true);
}
