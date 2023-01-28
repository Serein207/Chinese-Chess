#ifndef CELL_H
#define CELL_H

#include "piece.h"

#include <QPushButton>
#include <QTimer>

class Cell : public QPushButton {
    Q_OBJECT

    static Cell *chosen;
private:
    inline void delayedFade() {
        QTimer::singleShot(500, this, &Cell::fade);
    }
    inline void setBackground(const QColor& color) {
        QPalette style(palette());
        style.setColor(QPalette::Button, color);
        setPalette(style);
    }
    inline void setTextColor(const QColor& color) {
        QPalette style(palette());
        style.setColor(QPalette::ButtonText, color);
        setPalette(style);
    }
private slots:
    void fade() {
        setBackground(Qt::white);
    }
public:
    const int x, y;
    Cell(int x, int y);

    inline void change(Piece const* piece = nullptr) {
        if (!piece) {
            setText("+");
            setTextColor(Qt::black);
            fade();
            return;
        }
        setText(QString(piece->text()));
        if (piece->side())
            setTextColor(Qt::red);
        else
            setTextColor(Qt::black);
    }
    inline void select() {
        if (chosen)
            chosen->fade();
        setBackground(Qt::blue);
        chosen = this;
    }
    inline void wrongMove() {
        setBackground(Qt::red);
        delayedFade();
    }
    inline void fineMove() {
        setBackground(Qt::green);
        delayedFade();
        chosen->fade();
        chosen = nullptr;
    }
signals:
    void click(int x, int y);
};

#endif // CELL_H
