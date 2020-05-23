#include "testecourse.h"

TesteCourse::TesteCourse()
{

}

void TesteCourse::run()
{

    this->sleep(2);

    //Coureur 1
    QList<float> allCoordLongitudeCoureur1;
    QList<float> allCoordLatitudeCoureur1;
    //Coureur 2
    QList<float> allCoordLongitudeCoureur2;
    QList<float> allCoordLatitudeCoureur2;
    //Coureur 3
    QList<float> allCoordLongitudeCoureur3;
    QList<float> allCoordLatitudeCoureur3;
    //Coureur 4
    QList<float> allCoordLongitudeCoureur4;
    QList<float> allCoordLatitudeCoureur4;
    //Coureur 5
    QList<float> allCoordLongitudeCoureur5;
    QList<float> allCoordLatitudeCoureur5;
    //Coureur 6
    QList<float> allCoordLongitudeCoureur6;
    QList<float> allCoordLatitudeCoureur6;

    float coordLongitudeBalise1;
    float coordLatitudeBalise1;
    float coordLongitudeBalise2;
    float coordLatitudeBalise2;
    float coordLongitudeBalise3;
    float coordLatitudeBalise3;

    //Données pour le coureur 1
    allCoordLongitudeCoureur1.append(-1.572542);
    allCoordLatitudeCoureur1.append(46.370376);
    allCoordLongitudeCoureur1.append(-1.576639);
    allCoordLatitudeCoureur1.append(46.368283);
    allCoordLongitudeCoureur1.append(-1.579673);
    allCoordLatitudeCoureur1.append(46.366189);
    allCoordLongitudeCoureur1.append(-1.58374);
    allCoordLatitudeCoureur1.append(46.364116);
    allCoordLongitudeCoureur1.append(-1.580948);
    allCoordLatitudeCoureur1.append(46.361351);
    allCoordLongitudeCoureur1.append(-1.577792);
    allCoordLatitudeCoureur1.append(46.359131);
    allCoordLongitudeCoureur1.append(-1.572967);
    allCoordLatitudeCoureur1.append(46.357917);
    allCoordLongitudeCoureur1.append(-1.566109);
    allCoordLatitudeCoureur1.append(46.356995);
    allCoordLongitudeCoureur1.append(-1.563803);
    allCoordLatitudeCoureur1.append(46.359257);
    allCoordLongitudeCoureur1.append(-1.565714);
    allCoordLatitudeCoureur1.append(46.363089);
    allCoordLongitudeCoureur1.append(-1.567596);
    allCoordLatitudeCoureur1.append(46.366168);
    allCoordLongitudeCoureur1.append(-1.566738);
    allCoordLatitudeCoureur1.append(46.369988);
    allCoordLongitudeCoureur1.append(-1.57047);
    allCoordLatitudeCoureur1.append(46.371726);

    //Données pour le coureur 2
    allCoordLongitudeCoureur2.append(-1.570804);
    allCoordLatitudeCoureur2.append(46.371035);
    allCoordLongitudeCoureur2.append(-1.574810);
    allCoordLatitudeCoureur2.append(46.369151);
    allCoordLongitudeCoureur2.append(-1.578694);
    allCoordLatitudeCoureur2.append(46.366889);
    allCoordLongitudeCoureur2.append(-1.582578);
    allCoordLatitudeCoureur2.append(46.364502);
    allCoordLongitudeCoureur2.append(-1.581972);
    allCoordLatitudeCoureur2.append(46.361612);
    allCoordLongitudeCoureur2.append(-1.579969);
    allCoordLatitudeCoureur2.append(46.359183);
    allCoordLongitudeCoureur2.append(-1.573930);
    allCoordLatitudeCoureur2.append(46.357947);
    allCoordLongitudeCoureur2.append(-1.568741);
    allCoordLatitudeCoureur2.append(46.357633);
    allCoordLongitudeCoureur2.append(-1.566192);
    allCoordLatitudeCoureur2.append(46.358890);
    allCoordLongitudeCoureur2.append(-1.566222);
    allCoordLatitudeCoureur2.append(46.363350);
    allCoordLongitudeCoureur2.append(-1.566677);
    allCoordLatitudeCoureur2.append(46.366156);
    allCoordLongitudeCoureur2.append(-1.566829);
    allCoordLatitudeCoureur2.append(46.368962);
    allCoordLongitudeCoureur2.append(-1.568377);
    allCoordLatitudeCoureur2.append(46.371538);
    allCoordLongitudeCoureur2.append(-1.570774);
    allCoordLatitudeCoureur2.append(46.372208);

    //Données pour la balise 1
    coordLongitudeBalise1 = -1.570175;
    coordLatitudeBalise1 = 46.369162;

    //Données pour la balise 2
    coordLongitudeBalise2 = -1.580311;
    coordLatitudeBalise2 = 46.362608;

    //Données pour la balise 3
    coordLongitudeBalise3 = -1.567960;
    coordLatitudeBalise3 = 46.358147;


    emit SendInformations("192.168.0.201", QString::number(coordLongitudeBalise1).toStdString(), QString::number(coordLatitudeBalise1).toStdString(),"0","5222222");
    this->sleep(1);
    emit SendInformations("192.168.0.202", QString::number(coordLongitudeBalise2).toStdString(), QString::number(coordLatitudeBalise2).toStdString(),"0","5555555");
    this->sleep(1);
    emit SendInformations("192.168.0.203", QString::number(coordLongitudeBalise3).toStdString(), QString::number(coordLatitudeBalise3).toStdString(),"0","5555555");
    this->sleep(1);

    for(int i = 0;i < allCoordLongitudeCoureur1.count();i++)
    {
        this->sleep(1);
        emit SendInformations("192.168.0.1", QString::number(allCoordLongitudeCoureur1[i]).toStdString(), QString::number(allCoordLatitudeCoureur1[i]).toStdString(),"70","5555555");
        this->sleep(1);
        emit SendInformations("192.168.0.2", QString::number(allCoordLongitudeCoureur2[i]).toStdString(), QString::number(allCoordLatitudeCoureur2[i]).toStdString(),"99","5555555");
        this->sleep(4);
    }

}
