#include "velawindow.h"
#include "ui_velawindow.h"

//250 MAX -> représente le partage de la plage d'adresse IP classe C
#define MAX_COUREUR 200
#define MAX_BALISE 50

//Constructeur par défaut de l'interface principal
VelaWindow::VelaWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::VelaWindow)
{
    //enregistre le meta type pour le signal qui transmet les données du serveur à l'ihm (Plus propre ?)
    qRegisterMetaType<string>();

    //Démarrage de l'interface principal
    ui->setupUi(this);

    //Item selectionner null
    CurrentViewItemId = 0;

    option_win = new OptionWindow(this);

    //Modifie le background du graphicview
    ui->graphicsView->setStyleSheet("background-color: black;");

    //Permet de slide la liste des coureurs
    QScroller::grabGesture(ui->ListCoureur,QScroller::LeftMouseButtonGesture);

    //set le style des bouton coureur et balise
    itemTexteStyle.setFamily("Helvetica [Cronyx]");
    itemTexteStyle.setStyleHint(QFont::SansSerif);
    itemTexteStyle.setCapitalization(QFont::AllUppercase);
    itemTexteStyle.setPointSize(15);
    itemTexteStyle.setWeight(30);

    //Change le style du bouton option
    QFont texteBtnOption;
    texteBtnOption.setFamily("Helvetica [Cronyx]");
    texteBtnOption.setStyleHint(QFont::SansSerif);
    texteBtnOption.setCapitalization(QFont::AllUppercase);
    texteBtnOption.setPointSize(15);
    texteBtnOption.setWeight(30);
    ui->optionButton->setFont(texteBtnOption);
    ui->optionButton->setStyleSheet("background-color: #353535; color: white;border: 2px solid #FFFFFF;");

    //Connecte le bouton option a sont évènement
    connect(ui->optionButton, SIGNAL (clicked()), this, SLOT (optionButton_Clicked()));


    int gx = ui->graphicsView->width();
    int gy = ui->graphicsView->height();

    graphicMng = new GraphicManager(this,gx,gy);

    serveurWifi = new ServerWifi();
    connect(serveurWifi, SIGNAL(ReceptionInformations(string,string,string,string,string)), this, SLOT(ServeurInformation(string,string,string,string,string)));

    sqlManager = new SQLManager(0);


    //Affiche le texte d'accueil dans le graphic
    ui->graphicsView->setScene(graphicMng->sceneBase);
    graphicMng->SetBaseView();

    //Creation d'une thread pour test la course
    TesteCourse* threadTest = new TesteCourse();
    connect(threadTest, SIGNAL(SendInformations(string,string,string,string,string)), this, SLOT(ServeurInformation(string,string,string,string,string)));

    threadTest->start();

    //Démarage de la bdd et des serveur wifi et xbee
    sqlManager->startLocalDatabase("localhost", "admin", "marius", "bddmer");
   // serveurWifi->startServer();
   // serveurWifi->setSQL(sqlManager);

}

//Création d'une nouvelle course
void VelaWindow::ResetCourse()
{
    ui->ListCoureur->clear(); //Clean la liste des coureurs de l'interface
    lesBalises.clear();       //Clean la liste des balises connecter
    lesCoureurs.clear();      //Clean la liste des coureurs connecter
    int gx = ui->graphicsView->width();
    int gy = ui->graphicsView->height();
    graphicMng = new GraphicManager(this,gx,gy);
    graphicMng->_running = false;
    //graphicMng->_timeElapsed = 0;
    ui->graphicsView->setScene(graphicMng->sceneBase); //Affichage du graphique de base
    graphicMng->SetBaseView();
}

//Exportation de la BDD mer vers la BDD terre
bool VelaWindow::ExportBDD()
{
    if(sqlManager->exportDatabaseLocalToDistant())
        return true;

    return false;
}

//Traitement des informations envoyer par le serveur
void VelaWindow::ServeurInformation(string ip, string longitude, string latitude, string vitesse, string timestamp)
{
  //  qDebug() << QString::fromStdString(ip) << "   " << QString::fromStdString(longitude) << "   " <<  QString::fromStdString(latitude)  << "   " << QString::fromStdString(vitesse)  << "   " << QString::fromStdString(timestamp);

    //Sauvegarde des infos dans la base de données local
    sqlManager->saveDataInformations(ip, longitude, latitude, vitesse, timestamp);

    //Traitement des informations reçus coureur ou balise et ajout/update si existant ou non
    float receptLongitude = stof(longitude);
    float receptLatitude = stof(latitude);
    int receptVitesse = stoi(vitesse);
    int receptTime = stoi(timestamp);

    QString receptIp = QString::fromStdString(ip);

    //Recupere l'identifiant du coureur grâce a l'ip
    int deviceId = TranslateIpToNumber(receptIp);

    //Dans le cas ou l'a conversion a réussie l'identifiant a bien été récupéré
    if(deviceId != 0)
    {
        //Si il s'agit d'un coureur
        if(deviceId <= MAX_COUREUR)
        {
            //Verification de chaque ID dans les listes pour savoir si il s'agit d'un ajout ou d'une modification
            if(lesCoureurs.count() > 0)
            {
                //Pour chaque coureur
                for(int i=0;i < lesCoureurs.count();i++)
                {
                    //Vérifie si le coureur existe déjà
                    if(lesCoureurs[i]->GetId() == deviceId)
                    {
                        lesCoureurs[i]->UpdateInformation(receptLongitude, receptLatitude , receptVitesse);

                        //Si il est actuellement visionner alors le mettre a jour
                        if(deviceId == CurrentViewItemId)
                        {
                            ui->graphicsView->setScene(graphicMng->sceneCoureur);
                            graphicMng->SetCoureurView(lesCoureurs[i],lesBalises);
                        }
                        else
                        {
                            //Si aucune vue est afficher alors on affiche la vue global
                            //Autrement si la vue sélectionner est une balise on actualise la vue de la balise
                            if(CurrentViewItemId == 0)
                            {
                                //Affiche la vue globale
                                ui->graphicsView->setScene(graphicMng->sceneGlobal);
                                graphicMng->SetGlobalView(lesCoureurs,lesBalises);
                            }
                            else if(CurrentViewItemId > MAX_COUREUR)
                            {
                                ui->graphicsView->setScene(graphicMng->sceneBalise);
                                graphicMng->SetBaliseView(lesBalises[CurrentViewItemId-MAX_COUREUR-1],lesCoureurs,lesBalises);
                            }
                        }
                        return;
                    }

                }


            }
            //Sinon on créer un nouveau coureur
            if (lesCoureurs.count() > 0)
            {
                //Pour chaque coureur
                for(int i=0;i < lesCoureurs.count();i++)
                {
                    //Si sont identifiant est inférieur a l'identifiant du coureur selecttionner
                    if(deviceId < lesCoureurs[i]->GetId())
                    {
                        //Ajout un nouveau coureur a la liste et connecte sont bouton a l'évènement qui affiche la course du coureur
                        lesCoureurs.insert(i,new Coureur((QString)"COUREUR " + QString::number(deviceId) , deviceId, receptLongitude, receptLatitude , receptVitesse));
                        connect(lesCoureurs[i], SIGNAL(CoureurSelected(Coureur*)), this, SLOT(SelecteCoureurView(Coureur*)));

                        //Création du bouton dans la liste des coureurs
                        QListWidgetItem* tmpItem = new QListWidgetItem();
                        tmpItem->setSizeHint(QSize(150,50));  //Set size of button
                        ui->ListCoureur->insertItem(i,tmpItem);
                        lesCoureurs[i]->itemButton->setFont(itemTexteStyle);
                        lesCoureurs[i]->itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
                        ui->ListCoureur->setItemWidget(tmpItem, lesCoureurs[i]->itemButton);

                        //Affiche sur le graphic la vue global
                        if(CurrentViewItemId == 0)
                        {
                            ui->graphicsView->setScene(graphicMng->sceneGlobal);
                            graphicMng->SetGlobalView(lesCoureurs,lesBalises);
                        }

                        //On quitte la boucle
                        break;
                    }
                    else if(i+1 == lesCoureurs.count())
                    {
                        //Ajout un nouveau coureur a la liste et connecte sont bouton a l'évènement qui affiche la course du coureur
                        lesCoureurs.append(new Coureur((QString)"COUREUR " + QString::number(deviceId) , deviceId, receptLongitude, receptLatitude , receptVitesse));
                        connect(lesCoureurs.last(), SIGNAL(CoureurSelected(Coureur*)), this, SLOT(SelecteCoureurView(Coureur*)));

                        //Crétaion du bouton dans la liste des coureurs
                        QListWidgetItem* tmpItem = new QListWidgetItem();
                        tmpItem->setSizeHint(QSize(150,50));  //Set size of button
                        ui->ListCoureur->insertItem(lesCoureurs.count()-1,tmpItem);
                        lesCoureurs[i+1]->itemButton->setFont(itemTexteStyle);
                        lesCoureurs[i+1]->itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
                        ui->ListCoureur->setItemWidget(tmpItem, lesCoureurs[i+1]->itemButton);

                        //Affiche sur le graphic la vue global
                        ui->graphicsView->setScene(graphicMng->sceneGlobal);
                        graphicMng->SetGlobalView(lesCoureurs,lesBalises);

                        break;
                    }
                }
            }
            else
            {
                //Ajoute le coureur a la liste
                lesCoureurs.append(new Coureur((QString)"COUREUR " + QString::number(deviceId) , deviceId, receptLongitude, receptLatitude , receptVitesse));
                connect(lesCoureurs.last(), SIGNAL(CoureurSelected(Coureur*)), this, SLOT(SelecteCoureurView(Coureur*)));

                //Ajoute sont bouton dans la liste des coureurs et balise
                QListWidgetItem* tmpItem = new QListWidgetItem();
                tmpItem->setSizeHint(QSize(150,50));  //Set size of button
                ui->ListCoureur->insertItem(0,tmpItem);
                lesCoureurs[lesCoureurs.count()-1]->itemButton->setFont(itemTexteStyle);
                lesCoureurs[lesCoureurs.count()-1]->itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
                ui->ListCoureur->setItemWidget(tmpItem, lesCoureurs[lesCoureurs.count()-1]->itemButton);

                //Affiche sur le graphic la vue global
                ui->graphicsView->setScene(graphicMng->sceneGlobal);
                graphicMng->SetGlobalView(lesCoureurs,lesBalises);

                //Démarrage de la course
                if(!graphicMng->_running)
                {
                    graphicMng->_running = true;
                    graphicMng->_timerRunning->start(100);
                    graphicMng->_timerGlobalView->start(1000);
                }
            }
        }
        else
        {
            //Modifie l'index de début des balises pour les ajouter après les coureurs dans la liste
            int indexBalise = lesCoureurs.count();

            //Verification de chaque ID dans les listes pour savoir si il s'agit d'un ajout ou d'une modification
            if(lesBalises.count() > 0)
            {
                for(int i=0;i < lesBalises.count();i++)
                {
                    //Check si le coureur existe déjà
                    if(lesBalises[i]->GetId() == deviceId)
                    {
                        lesBalises[i]->UpdateInformation(receptLongitude, receptLatitude);

                        return;
                    }
                }
            }

            //Sinon on créer une nouvelle balise
            if (lesBalises.count() > 0)
            {
                for(int i=0;i < lesBalises.count();i++)
                {
                    if(deviceId < lesBalises[i]->GetId())
                    {
                        lesBalises.insert(i,new Balise((QString)"BALISE " + QString::number(deviceId-200) , deviceId, receptLongitude, receptLatitude));
                        connect(lesBalises[i], SIGNAL(BaliseSelected(Balise*)), this, SLOT(SelecteBaliseView(Balise*)));

                        QListWidgetItem* tmpItem = new QListWidgetItem();
                        tmpItem->setSizeHint(QSize(150,50));  //Set size of button
                        ui->ListCoureur->insertItem(indexBalise+i,tmpItem);
                        lesBalises[i]->itemButton->setFont(itemTexteStyle);
                        lesBalises[i]->itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
                        ui->ListCoureur->setItemWidget(tmpItem, lesBalises[i]->itemButton);

                        //Affiche sur le graphic la vue global
                        ui->graphicsView->setScene(graphicMng->sceneGlobal);
                        graphicMng->SetGlobalView(lesCoureurs,lesBalises);

                        break;
                    }
                    else if(i+1 == lesBalises.count())
                    {
                        lesBalises.append(new Balise((QString)"BALISE " + QString::number(deviceId-200) , deviceId, receptLongitude, receptLatitude));
                        connect(lesBalises.last(), SIGNAL(BaliseSelected(Balise*)), this, SLOT(SelecteBaliseView(Balise*)));

                        QListWidgetItem* tmpItem = new QListWidgetItem();
                        tmpItem->setSizeHint(QSize(150,50));  //Set size of button
                        if(indexBalise == -1)
                            ui->ListCoureur->insertItem(i+2,tmpItem);
                        else
                            ui->ListCoureur->insertItem(indexBalise+i+2,tmpItem);

                        lesBalises[i+1]->itemButton->setFont(itemTexteStyle);
                        lesBalises[i+1]->itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
                        ui->ListCoureur->setItemWidget(tmpItem, lesBalises[i+1]->itemButton);

                        //Affiche sur le graphicView la vue global
                        ui->graphicsView->setScene(graphicMng->sceneGlobal);
                        graphicMng->SetGlobalView(lesCoureurs,lesBalises);

                        break;
                    }
                }
            }
            else
            {
                lesBalises.append(new Balise((QString)"BALISE " + QString::number(deviceId-200) , deviceId, receptLongitude, receptLatitude));
                connect(lesBalises.last(), SIGNAL(BaliseSelected(Balise*)), this, SLOT(SelecteBaliseView(Balise*)));

                QListWidgetItem* tmpItem = new QListWidgetItem();
                tmpItem->setSizeHint(QSize(150,50));  //Set size of button
                ui->ListCoureur->insertItem(indexBalise+lesBalises.count(),tmpItem);
                lesBalises[lesBalises.count()-1]->itemButton->setFont(itemTexteStyle);
                lesBalises[lesBalises.count()-1]->itemButton->setStyleSheet("background-color: #282828; color: white;border: 2px solid #000000;");
                ui->ListCoureur->setItemWidget(tmpItem, lesBalises[lesBalises.count()-1]->itemButton);

                //Affiche sur le graphic la vue global
                ui->graphicsView->setScene(graphicMng->sceneGlobal);
                graphicMng->SetGlobalView(lesCoureurs,lesBalises);

                //Démarrage de la course
                if(!graphicMng->_running)
                {
                    graphicMng->_running = true;
                    graphicMng->_timerRunning->start(100);
                    graphicMng->_timerGlobalView->start(1000);
                }
            }
        }
    }
}

//Fonctions utiliser pour la translation de l'ip en identifiant
int VelaWindow::TranslateIpToNumber(QString ip)
{
    //Recupération de la fin de l'adresse IP -> (X)  0.0.0.X
    QString partIp;
    QString endOfIp;
    int numberEndIp;

    partIp = ip.mid(ip.length()-3,ip.length());

    //Dans le cas X.X.X.1
    if(partIp[1] == '.')
    {
        endOfIp = ip.mid(ip.length()-1,ip.length());
    }
    else
    {
        //Dans le cas X.X.X.11
        if(partIp[0] == '.')
        {
            endOfIp = ip.mid(ip.length()-2,ip.length());
        }
        else
        {
            //Dans le cas X.X.X.111
            endOfIp = partIp;
        }
    }
    numberEndIp = endOfIp.toInt();
    return numberEndIp;
}

//Permet l'affichage du coureur lors de sa selection dans la liste
void VelaWindow::SelecteCoureurView(Coureur* selectedcoureur)
{
    //Si un item est déjà observer traitement avant affichage autrement on l'affiche normalement
    if(CurrentViewItemId != 0)
    {
        //Si il s'agit déjà du coureur selectionner alors on le désactive et on remet la vue global
        if(CurrentViewItemId == selectedcoureur->GetId())
        {
            selectedcoureur->DisableSelection();
            CurrentViewItemId = 0;
            ui->graphicsView->setScene(graphicMng->sceneGlobal);
            graphicMng->SetGlobalView(lesCoureurs,lesBalises);
        }
        else
        {
        //Autrement si un item est déjà en visualisation on le désactive et on met le nouveau
            for(int i=0;i < lesCoureurs.count();i++)
            {
                if(lesCoureurs[i]->GetId() == CurrentViewItemId)
                {
                    lesCoureurs[i]->DisableSelection();
                    break;
                }
            }
            for(int i=0;i < lesBalises.count();i++)
            {
                if(lesBalises[i]->GetId() == CurrentViewItemId)
                {
                    lesBalises[i]->DisableSelection();
                    break;
                }
            }
            ui->graphicsView->setScene(graphicMng->sceneCoureur);
            graphicMng->SetCoureurView(selectedcoureur,lesBalises);
            CurrentViewItemId = selectedcoureur->GetId();;
        }
    }
    else
    {
        ui->graphicsView->setScene(graphicMng->sceneCoureur);
        graphicMng->SetCoureurView(selectedcoureur,lesBalises);
        CurrentViewItemId = selectedcoureur->GetId();
    }
}

//Permet l'affichage d'une balise lors de sa selection dans la liste
void VelaWindow::SelecteBaliseView(Balise* selectedbalise)
{
    //Si un item est déjà observer traitement avant affichage autrement on l'affiche normalement
    if(CurrentViewItemId != 0)
    {
        //Si il s'agit déjà du coureur selectionner alors on le désactive et on remet la vue global
        if(CurrentViewItemId == selectedbalise->GetId())
        {
            selectedbalise->DisableSelection();
            CurrentViewItemId = 0;
            ui->graphicsView->setScene(graphicMng->sceneGlobal);
            graphicMng->SetGlobalView(lesCoureurs,lesBalises);
        }
        else
        {
        //Autrement si un item est déjà en visualisation on le désactive et on met le nouveau
            for(int i=0;i < lesBalises.count();i++)
            {
                if(lesBalises[i]->GetId() == CurrentViewItemId)
                {
                    lesBalises[i]->DisableSelection();
                    break;
                }
            }
            for(int i=0;i < lesCoureurs.count();i++)
            {
                if(lesCoureurs[i]->GetId() == CurrentViewItemId)
                {
                    lesCoureurs[i]->DisableSelection();
                    break;
                }
            }
            //Affiche la scene de la balise
            ui->graphicsView->setScene(graphicMng->sceneBalise);
            graphicMng->SetBaliseView(selectedbalise,lesCoureurs,lesBalises);
            CurrentViewItemId = selectedbalise->GetId();;
        }
    }
    else
    {
        ui->graphicsView->setScene(graphicMng->sceneBalise);
        graphicMng->SetBaliseView(selectedbalise,lesCoureurs,lesBalises);
        CurrentViewItemId = selectedbalise->GetId();
    }
}

//Execution a l'appuie sur le bouton option
void VelaWindow::optionButton_Clicked()
{
    option_win->showFullScreen();
}


VelaWindow::~VelaWindow()
{
    delete ui;
}
