#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <velawindow.h>
#include "optioninformation.h"

namespace Ui {
class OptionWindow;
}

class VelaWindow;
class OptionInformation;

class OptionWindow : public QDialog
{
    Q_OBJECT

public:
     explicit OptionWindow(VelaWindow* mainwindow,QWidget *parent = 0);
     VelaWindow* mainWindow;
    ~OptionWindow();

private:
    Ui::OptionWindow *ui;
    OptionInformation* optionInfo;

signals:
    void StartExportDatabase();

private slots:
    void on_btn_back_clicked();
    void on_btn_reset_clicked();
    void on_btn_save_bdd_clicked();
    void on_btn_exit_clicked();

};

#endif // DIALOG_H
