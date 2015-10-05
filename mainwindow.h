#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csv_parser.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnLoadCSV_clicked();
    void on_btnConvert_clicked();

private:
    Ui::MainWindow *ui;
    QString fileName;
};

#endif // MAINWINDOW_H
