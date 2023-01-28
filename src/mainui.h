#ifndef MAINUI_H
#define MAINUI_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

class Cell;
class MainUI : public QDialog
{
    Q_OBJECT

    QVBoxLayout* layout;
    QGridLayout* boardLayout;
    QLabel *h[8 * 10], *v1[9 * 4], *v2[9 * 4], *l[8], *chu, *han;
    Cell* cells[9 * 10];
signals:
    void setup(Cell**);
public slots:
    void onFind(const QString opp_name) {
        emit setup(cells);
    }
public:
    MainUI();
};


#endif // MAINUI_H
