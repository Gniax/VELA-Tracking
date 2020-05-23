#ifndef TESTECOURSE_H
#define TESTECOURSE_H

#include <QThread>
#include <string>
#include <iostream>
using namespace std;


class TesteCourse : public QThread
{
    Q_OBJECT

public:
    explicit TesteCourse();
    void run();

signals:
    void SendInformations(string,string,string,string,string);

};

#endif // TESTECOURSE_H
