#ifndef GRAPHICMANAGER_H
#define GRAPHICMANAGER_H

#include <velawindow.h>
#include <QGraphicsScene>
#include <QMutex>

class VelaWindow;
class Coureur;

class GraphicManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphicManager(VelaWindow* parent,int gx,int gy);
    void SetBaseView();
    void SetGlobalView(QList<Coureur*> allcoureur,QList<Balise*> allbalise);
    void SetCoureurView(Coureur* coureurSelected, QList<Balise*> allbalise);
    void SetBaliseView(Balise* baliseSelected, QList<Coureur*> allcoureur, QList<Balise*> allbalise);
    QGraphicsScene* sceneBase;
    QGraphicsScene* sceneGlobal;
    QGraphicsScene* sceneCoureur;
    QGraphicsScene* sceneBalise;
    QTimer* _timerRunning;
    QTimer* _timerGlobalView;
    bool _running;
    int _timeElapsed;
    QGraphicsTextItem* textTime;
    QGraphicsTextItem* textTimeCoureur;
    bool isSelectedGlobalView;
    bool isSelectedCoureurView;

private:
    QString _timeGlobalView;
    void updateTimeGlobalView();
    void updateTimeCoureurView();
    QMutex mutexGlobaleView;
    QMutex mutexCoureurView;
    QString GetTranslatedTime(int timeMs);
    Coureur* _currentSelectedCoureur;
    int sceneSizeX;
    int sceneSizeY;
    float areaCoursePixelX;
    float areaCoursePixelY;
    void CalibrateView(QList<float> allcoordlongitude, QList<float> alcoordlatitude, QList<Balise*> allbalise);
    void CalibrateBaliseView(QList<Balise*> allbalise);
    float minLongitude;
    float minLatitude;
    float maxLongitude;
    float maxLatitude;
    float dimCourseLong;
    float dimCourseLat;
    int GetXFromLongitude(float longitude);
    int GetYFromLatitude(float latitude);


    int diagPixelPosCoureur;
    int diagPixelPosBalise;
    int posZoneCourseHX;
    int posZoneCourseHY;
    int posZoneCourseBX;
    int posZoneCourseBY;

private slots:
    void UpdateTimer();
    void UpdateTimerGlobalView();
};

#endif // GRAPHICMANAGER_H
