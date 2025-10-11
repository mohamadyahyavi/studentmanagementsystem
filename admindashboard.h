#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDashboard(QWidget *parent = nullptr);
    ~AdminDashboard();

private:
    Ui::AdminDashboard *ui;
};

#endif // ADMINDASHBOARD_H
