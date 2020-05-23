#ifndef OPTIONINFORMATION_H
#define OPTIONINFORMATION_H

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <optionwindow.h>
#include <velawindow.h>

namespace Ui {
class OptionInformation;
}

class OptionWindow;

class OptionInformation : public QDialog
{
Q_OBJECT
public:
   explicit OptionInformation(OptionWindow* optionwindow, QWidget *parent = 0);
    ~OptionInformation();
    void OpenWindow(QString mode);
    void SetInformationSuccess();
    void SetInformationError();
    void SetInformationWarning();
    void SetInformationProcess();

private:
    Ui::OptionInformation *ui;
    OptionWindow* optionWindow;
    QString modeOption;
    QFont infoHeaderTextStyle;
    QFont infoMessageTextStyle;
    QFont infoBtnStyle;

private slots:
    void warning_btn_next();
    void warning_btn_return();
    void success_btn_next();
    void failure_btn_next();

};

#endif // OPTIONINFORMATION_H


