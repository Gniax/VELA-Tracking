#include "graphicmanager.h"

//Constructeur par défaut du graphic manager
GraphicManager::GraphicManager(VelaWindow* parent, int gx , int gy)
{
    //Création des différentes scene utiliser
    sceneBase = new QGraphicsScene(parent);
    sceneGlobal = new QGraphicsScene(parent);
    sceneCoureur = new QGraphicsScene(parent);
    sceneBalise = new QGraphicsScene(parent);

    //Modifie la taille des scenes
    sceneBase->setSceneRect(0,0,gx,gy);
    sceneGlobal->setSceneRect(0,0,gx,gy);
    sceneCoureur->setSceneRect(0,0,gx,gy);
    sceneBalise->setSceneRect(0,0,gx,gy);
    sceneSizeX = gx;
    sceneSizeY = gy;

    //Diagonal d'un point de coureur sur la scene
    diagPixelPosCoureur = 20;
    diagPixelPosBalise = 20;

    //Définition de la zone de course
    posZoneCourseHX = 20;
    posZoneCourseHY = 50;
    posZoneCourseBX = gx - posZoneCourseHX - 20;
    posZoneCourseBY = gy - posZoneCourseHY - 50;
    areaCoursePixelX = (float)(posZoneCourseBX);
    areaCoursePixelY = (float)(posZoneCourseBY);

    //Définition des timers
    _running = false;
    _timeElapsed = 0;
    _timerRunning = new QTimer(this);
    _timerGlobalView  = new QTimer(this);

    //Définition de la sélection de la vue
    isSelectedGlobalView = false;
    isSelectedCoureurView = false;

    //Connexion des signaux des timers
    connect(_timerRunning , SIGNAL(timeout()) , this , SLOT(UpdateTimer()));
    connect(_timerGlobalView , SIGNAL(timeout()) , this , SLOT(UpdateTimerGlobalView()));
}

//Timer qui met à jour l'affichage des timers
void GraphicManager::UpdateTimer()
{
    //Si la course est en cours
    if(_running)
    {
        //Si la vue est global alors met a jour le timer de la vue globale
        if(isSelectedGlobalView)
            updateTimeGlobalView();

        //Si la vue est est un coureur alors met a jour le timer de la vue du coureur sélectionner
        if(isSelectedCoureurView)
            updateTimeCoureurView();
    }
}

//Incrémentation de la durée de la course
void GraphicManager::UpdateTimerGlobalView()
{
    //Si la course est en cours on augmente de 1 secondes le temps de la course
    if(_running)
    {
        _timeElapsed++;
    }
}

//Mise à jour du timer dans l'affichage du coureur
void GraphicManager::updateTimeCoureurView()
{
    //Bloque l'accès à la vue le temps de modifier le temps afficher
    mutexCoureurView.lock();
    textTimeCoureur->setPlainText(GetTranslatedTime(_currentSelectedCoureur->_timeLastPoint));
    mutexCoureurView.unlock();
}

//Mise à jour du timer dans l'affichage global
void GraphicManager::updateTimeGlobalView()
{
   //Bloque l'accès le temps de modifier le temps afficher
    mutexGlobaleView.lock();
    textTime->setPlainText(GetTranslatedTime(_timeElapsed));
    mutexGlobaleView.unlock();
}

//Définie la vue de base à l'entrée de l'application
void GraphicManager::SetBaseView()
{
    isSelectedGlobalView = false;
    //Nétoie la vue de la scene de base
    sceneBase->clear();

    //Affiche du texte dans la scene
    QGraphicsTextItem* textWelcomePart1 = sceneBase->addText("VELA", QFont("Arial", 80) );
    textWelcomePart1->setDefaultTextColor(QColor(Qt::white));
    textWelcomePart1->moveBy(160,80);
    QGraphicsTextItem* textWelcomePart2 = sceneBase->addText("TRACKING", QFont("Arial", 80) );
    textWelcomePart2->setDefaultTextColor(QColor(Qt::white));
    textWelcomePart2->moveBy(40,190);
}

//Affiche la vue globale de la course
void GraphicManager::SetGlobalView(QList<Coureur*> allcoureur,QList<Balise*> allbalise)
{
    isSelectedCoureurView = false;

    //Vérouille l'accès a la vue global
    mutexGlobaleView.lock();

    //Nétoie la vue de la scene global
    sceneGlobal->clear();

    //On stocke les données des coureurs sous forme de liste
    QList<float> tempCoureurPosLongitude;
    QList<float> tempCoureurPoslatitude;
    for(int i=0;i < allcoureur.count();i++)
    {
       tempCoureurPosLongitude.append(allcoureur[i]->GetLastLongitude());
       tempCoureurPoslatitude.append(allcoureur[i]->GetLastLatitude());
    }

    //Affiche le nombnre de coureur
    QGraphicsTextItem* textCoureurNumber = sceneGlobal->addText("Coureurs: " + QString::number(allcoureur.count()), QFont("Arial", 20));
    textCoureurNumber->setDefaultTextColor(QColor(Qt::white));
    textCoureurNumber->moveBy(10,440);

    //Affiche le nombre de balise
    QGraphicsTextItem* textBaliseNumber = sceneGlobal->addText("Balises: " + QString::number(allbalise.count()), QFont("Arial", 20) );
    textBaliseNumber->setDefaultTextColor(QColor(Qt::white));
    textBaliseNumber->moveBy(500,440);

    //Affiche le nom de la vue
    QGraphicsTextItem* textViewInfo = sceneGlobal->addText("Vue globale de la course ", QFont("Arial", 20) );
    textViewInfo->setDefaultTextColor(QColor(Qt::white));
    textViewInfo->moveBy(0,0);

    //Affichage temps d'éxécution dans la vue globale
    textTime = sceneGlobal->addText(GetTranslatedTime(_timeElapsed), QFont("Arial", 20) );
    textTime->setDefaultTextColor(QColor(Qt::white));
    textTime->moveBy(520,0);


    //Selectionne la vue global pour que le timer puisse mettre a jour le compteur
    isSelectedGlobalView = true ;

    //Si le nombre total d'élements est inférieur à 2 alors on affiche simplement l'élément dans la scene
    if(allbalise.count() == 1 && allcoureur.count() == 0)
    {
        sceneGlobal->addRect(posZoneCourseHX, posZoneCourseHY, diagPixelPosBalise, diagPixelPosBalise, QPen(Qt::red), QBrush(Qt::red));
    }
    else if(allbalise.count() == 0 && allcoureur.count() == 1)
    {
        sceneGlobal->addEllipse(posZoneCourseHX, posZoneCourseHY, diagPixelPosCoureur, diagPixelPosCoureur, QPen(Qt::green), QBrush(Qt::blue));
    }
    else
    {
        //Calibre la vue de la scene en fonction de tout les point
        CalibrateView(tempCoureurPosLongitude,tempCoureurPoslatitude,allbalise);

        //On affiche les coureurs dans la scene global
        for(int i=0;i < allcoureur.count();i++)
        {
           int posX = posZoneCourseHX + GetXFromLongitude(allcoureur[i]->GetLastLongitude()) - (diagPixelPosCoureur/2);
           int posY = posZoneCourseHY + GetYFromLatitude(allcoureur[i]->GetLastLatitude()) - (diagPixelPosCoureur/2);
           sceneGlobal->addEllipse(posX, posY, diagPixelPosCoureur, diagPixelPosCoureur, QPen(Qt::green), QBrush(Qt::blue));
        }

        //On affiche les balises dans la scene global
        for(int i=0;i < allbalise.count();i++)
        {
           int posX = posZoneCourseHX + GetXFromLongitude(allbalise[i]->GetLastLongitude()) - (diagPixelPosBalise/2);
           int posY = posZoneCourseHY + GetYFromLatitude(allbalise[i]->GetLastLatitude()) - (diagPixelPosBalise/2);
           sceneGlobal->addRect(posX, posY, diagPixelPosBalise, diagPixelPosBalise, QPen(Qt::red), QBrush(Qt::red));
        }
    }
    mutexGlobaleView.unlock();
}

//Permet l'affichage du tracer de la course du coureur.
void GraphicManager::SetCoureurView(Coureur* coureurSelected, QList<Balise*> allbalise)
{
    isSelectedGlobalView = false;

    //Bloque l'accès a la vue le temps de la modifier
    mutexCoureurView.lock();

    //Nétoie la vue de la scene coureur
    sceneCoureur->clear();

    //Définition  des styles pour les tracés dans la scene
    QBrush greenBrush(Qt::blue);
    QPen outlinePen(Qt::red);
    outlinePen.setWidth(2);

    //Affiche des informations du coureur dans la scene
    //Nom du coureur
    QGraphicsTextItem* textCoureurName = sceneCoureur->addText(coureurSelected->GetName(), QFont("Arial", 20) );
    textCoureurName->setDefaultTextColor(QColor(Qt::white));
    textCoureurName->moveBy(10,0);
    //Position du coureur
    QGraphicsTextItem* textGpsPos = sceneCoureur->addText(QString::number(coureurSelected->GetLastLongitude()) + " / " + QString::number(coureurSelected->GetLastLatitude()), QFont("Arial", 20) );
    textGpsPos->setDefaultTextColor(QColor(Qt::white));
    textGpsPos->moveBy(10,440);
    //Vitesse du coureur
    QGraphicsTextItem* textCoureurSpeed = sceneCoureur->addText(QString::number(coureurSelected->GetLastVitesse()) + " Km/h", QFont("Arial", 20) );
    textCoureurSpeed->setDefaultTextColor(QColor(Qt::white));
    textCoureurSpeed->moveBy(525,440);

    //Affichage temps d'éxécution dans la vue coureur
    textTimeCoureur = sceneCoureur->addText(GetTranslatedTime(coureurSelected->_timeLastPoint), QFont("Arial", 20) );
    textTimeCoureur->setDefaultTextColor(QColor(Qt::white));
    textTimeCoureur->moveBy(520,0);

    //Garde un pointeur vers le coureur séletionner pour récupèrer le temps du dernier point reçue
    _currentSelectedCoureur = coureurSelected;

    //Définie la vue comme sélectionner pour mettre à jour le timer
    isSelectedCoureurView = true;

    //Calibre la vue de la scene en fonction de tout les point du coureur
    CalibrateView(coureurSelected->allCoordLongitude,coureurSelected->allCoordLatitude,allbalise);

    // Si le nombre de coureur est supérieur a 1 ou le nombre de balise est supériueur a 0
    // Alors on affiche les points du coureur calibrer (Nécessite au moins 2 points balise compris pour le calibrage sinon on prend le point 0)
    if(coureurSelected->allCoordLongitude.count() > 1 || allbalise.count() > 0)
    {
        int lastPosX = -1;
        int lastPosY = -1;

        //Pour chaque position du coureur
        for(int i=0;i < coureurSelected->allCoordLongitude.count()-1;i++)
        {
           // On récupère sa position en coordonées X et Y
           int posX = posZoneCourseHX + GetXFromLongitude(coureurSelected->allCoordLongitude[i]) - (diagPixelPosCoureur/2);
           int posY = posZoneCourseHY + GetYFromLatitude(coureurSelected->allCoordLatitude[i]) - (diagPixelPosCoureur/2);

           // Si il existe déjà un point on éffectue le tracer sinon on affiche simplement le point
           if(lastPosX != -1)
           {
               sceneCoureur->addLine(lastPosX+(diagPixelPosCoureur/2),lastPosY+(diagPixelPosCoureur/2), posX+(diagPixelPosCoureur/2), posY+(diagPixelPosCoureur/2),QPen(Qt::blue));
               sceneCoureur->addEllipse(posX,posY, diagPixelPosCoureur, diagPixelPosCoureur,QPen(Qt::white), greenBrush);
           }
           else
           {
               sceneCoureur->addEllipse(posX,posY, diagPixelPosCoureur, diagPixelPosCoureur,QPen(Qt::white), greenBrush);
           }
           lastPosX = posX;
           lastPosY = posY;
        }

        int posX = posZoneCourseHX + GetXFromLongitude(coureurSelected->allCoordLongitude[coureurSelected->allCoordLongitude.count()-1]) - (diagPixelPosCoureur/2);
        int posY = posZoneCourseHY + GetYFromLatitude(coureurSelected->allCoordLatitude[coureurSelected->allCoordLongitude.count()-1]) - (diagPixelPosCoureur/2);

        //Si on a plus de 1 point alors on fait les tracer de la course
        if(coureurSelected->allCoordLongitude.count() > 1)
        {
            sceneCoureur->addLine(lastPosX+(diagPixelPosCoureur/2),lastPosY+(diagPixelPosCoureur/2), posX+(diagPixelPosCoureur/2), posY+(diagPixelPosCoureur/2),QPen(Qt::blue));
        }

        sceneCoureur->addEllipse(posX,posY, diagPixelPosCoureur, diagPixelPosCoureur,QPen(Qt::white), QBrush(Qt::white));

        //Si il éxiste des balises alors on les affiches
        if(allbalise.count() > 0)
        {
            for(int i=0;i < allbalise.count();i++)
            {
               int posX = posZoneCourseHX + GetXFromLongitude(allbalise[i]->GetLastLongitude()) - (diagPixelPosBalise/2);
               int posY = posZoneCourseHY + GetYFromLatitude(allbalise[i]->GetLastLatitude()) - (diagPixelPosBalise/2);
               sceneCoureur->addRect(posX, posY, diagPixelPosBalise, diagPixelPosBalise, QPen(Qt::red), QBrush(Qt::red));
            }
        }
    }
    else if(coureurSelected->allCoordLongitude.count() == 1)
    {
        sceneCoureur->addEllipse(posZoneCourseHX,posZoneCourseHY, diagPixelPosCoureur, diagPixelPosCoureur,QPen(Qt::white), QBrush(Qt::white));
    }

    //Libère l'accès a la vue
    mutexCoureurView.unlock();
}

//Permet l'affichage des données de la balise séléctionner en gardant la vue sur les coureurs et des autres balises.
void GraphicManager::SetBaliseView(Balise* selectedbalise, QList<Coureur*> allcoureur, QList<Balise*> allbalise)
{
    isSelectedCoureurView = false;
    isSelectedGlobalView = false;

    //Nétoie la vue de la scene balise
    sceneBalise->clear();

    //On stocke les données des coureurs sous forme de liste
    QList<float> tempCoureurPosLongitude;
    QList<float> tempCoureurPoslatitude;
    for(int i=0;i < allcoureur.count();i++)
    {
       tempCoureurPosLongitude.append(allcoureur[i]->GetLastLongitude());
       tempCoureurPoslatitude.append(allcoureur[i]->GetLastLatitude());
    }

    //Affiche des informations de la balise dans la scene
    QGraphicsTextItem* textBaliseName = sceneBalise->addText(selectedbalise->GetName(), QFont("Arial", 20) );
    textBaliseName->setDefaultTextColor(QColor(Qt::white));
    textBaliseName->moveBy(10,0);
    QGraphicsTextItem* textGpsPos = sceneBalise->addText(QString::number(selectedbalise->GetLastLongitude()) + " / " + QString::number(selectedbalise->GetLastLatitude()), QFont("Arial", 20) );
    textGpsPos->setDefaultTextColor(QColor(Qt::white));
    textGpsPos->moveBy(10,440);

    //Si le nombre d'éléments est inférieur à 2 alors on affiche la balise au point 0
    if(allbalise.count() == 1 && allcoureur.count() == 0)
    {
        sceneBalise->addRect(posZoneCourseHX,posZoneCourseHY, diagPixelPosBalise, diagPixelPosBalise, QPen(Qt::white), QBrush(Qt::white));
    }
    else
    {
        //Calibre la vue en fonction de tout les points de la scene
        CalibrateView(tempCoureurPosLongitude,tempCoureurPoslatitude,allbalise);

        //Affiche les coureurs
        for(int i=0;i < allcoureur.count();i++)
        {
           int posX = posZoneCourseHX + GetXFromLongitude(allcoureur[i]->GetLastLongitude()) - (diagPixelPosCoureur/2);
           int posY = posZoneCourseHY + GetYFromLatitude(allcoureur[i]->GetLastLatitude()) - (diagPixelPosCoureur/2);
           sceneBalise->addEllipse(posX, posY, diagPixelPosCoureur, diagPixelPosCoureur, QPen(Qt::green), QBrush(Qt::blue));
        }

        //Affiche la balise séléctionner en blanc et les autres en rouge
       for(int i=0;i < allbalise.count();i++)
       {
          int posX = posZoneCourseHX + GetXFromLongitude(allbalise[i]->GetLastLongitude()) - (diagPixelPosBalise/2);
          int posY = posZoneCourseHY + GetYFromLatitude(allbalise[i]->GetLastLatitude()) - (diagPixelPosBalise/2);
          if(allbalise[i]->GetId() == selectedbalise->GetId())
          {
              sceneBalise->addRect(posX, posY, diagPixelPosBalise, diagPixelPosBalise, QPen(Qt::white), QBrush(Qt::white));
          }
          else
          {
              sceneBalise->addRect(posX, posY, diagPixelPosBalise, diagPixelPosBalise, QPen(Qt::red), QBrush(Qt::red));
          }
       }
    }
}

//Permet de calibrer la vue de la course en fonctions des points des balises et des coureurs
void GraphicManager::CalibrateView(QList<float> allcoordlongitude, QList<float> alcoordlatitude, QList<Balise*> allbalise) //Soit CalibrateView(QList<float>& allcoordlongitude, QList<float>& alcoordlatitude) pour ne pas recréer la liste => plus optimiser
{
    //On ajoute la position des balises
    for(int i=0;i < allbalise.count();i++)
    {
        allcoordlongitude.append(allbalise[i]->GetLastLongitude());
        alcoordlatitude.append(allbalise[i]->GetLastLatitude());
    }

    // Si on dispose de point
    if(allcoordlongitude.count() > 0 && alcoordlatitude.count() > 0)
    {
        //Recup min et max longitude
        minLongitude = allcoordlongitude[0];
        maxLongitude = allcoordlongitude[0];
        for(int i=0;i < allcoordlongitude.count();i++)
        {
            //min
            if(minLongitude > allcoordlongitude[i])
                minLongitude = allcoordlongitude[i];
            //max
            if(maxLongitude < allcoordlongitude[i])
                maxLongitude = allcoordlongitude[i];
        }

        //Recup min et max latitude
        minLatitude = alcoordlatitude[0];
        maxLatitude = alcoordlatitude[0];
        for(int i=0;i < alcoordlatitude.count();i++)
        {
            //min
            if(minLatitude > alcoordlatitude[i])
                minLatitude = alcoordlatitude[i];
            //max
            if(maxLatitude < alcoordlatitude[i])
                maxLatitude = alcoordlatitude[i];
        }

        //Reglage des dimension de la course
        dimCourseLong = maxLongitude - minLongitude;
        dimCourseLat = maxLatitude - minLatitude;
    }
}

//Permet de translater les coordonées longitude en coordonées X en pixel
int GraphicManager::GetXFromLongitude(float longitude)
{
    return (longitude - minLongitude) / (dimCourseLong / areaCoursePixelX);
}

//Permet de translater les coordonées latitude en coordonées Y en pixel
int GraphicManager::GetYFromLatitude(float latitude)
{
    return (latitude - minLatitude) / (dimCourseLat / areaCoursePixelY);
}

//Permet de translater le nombre de secondes reçue en argument au format hh:mm:ss pour l'affichage
QString GraphicManager::GetTranslatedTime(int timeMs)
{
    int totalNumberOfSeconds = timeMs;
    int seconds = totalNumberOfSeconds % 60;
    int minutes = (totalNumberOfSeconds / 60) % 60;
    int hours = (totalNumberOfSeconds / 60 / 60);

    if(hours >= 0 && hours <= 99)
        if(minutes >= 0 && minutes <= 59)
            if(seconds >= 0 && seconds <= 59)
                return QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

    return QString("00:00:00");
}







