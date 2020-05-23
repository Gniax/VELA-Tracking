#include "optioninformation.h"
#include "ui_optioninformation.h"

//Constructeur par défaut de la fenêtre d'information
OptionInformation::OptionInformation(OptionWindow* optionwindow,QWidget *parent)  : QDialog(parent), ui(new Ui::OptionInformation)
{
    //Initialisation de la fenêtre
    ui->setupUi(this);

    //Enregistre un pointeur vers la fenêtre des paramètres
    optionWindow = optionwindow;

    //Définition du comportement de la fenêtre et de sont style
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
    this->setGeometry(QRect(QPoint(200,140),QPoint(600,340)));
    this->setStyleSheet("border-color: rgb(255,255,255); border-width : 3px; border-style: solid;");

    //Style pour le texte d'entête de la fenêtre
    infoHeaderTextStyle.setFamily("Verdana");
    infoHeaderTextStyle.setStyleHint(QFont::SansSerif);
    infoHeaderTextStyle.setCapitalization(QFont::AllUppercase);
    infoHeaderTextStyle.setPointSize(20);
    infoHeaderTextStyle.setWeight(50);

    //Style pour le texte du message de la fenêtre
    infoMessageTextStyle.setFamily("Verdana");
    infoMessageTextStyle.setStyleHint(QFont::SansSerif);
    infoMessageTextStyle.setPointSize(14);
    infoMessageTextStyle.setWeight(25);

    //Style de base pour les boutons de la fenêtre
    infoBtnStyle.setFamily("Verdana");
    infoBtnStyle.setStyleHint(QFont::SansSerif);
    infoBtnStyle.setPointSize(14);
    infoBtnStyle.setWeight(20);
}

//Déstructeur par défaut
OptionInformation::~OptionInformation()
{
    delete ui;
}

//Permet l'affichage de la fenêtre en enregistrant le mode choisie
void OptionInformation::OpenWindow(QString mode)
{
    modeOption = mode;
    this->show();
}

//Fenêtre qui affiche la demande de confirmation
void OptionInformation::SetInformationWarning()
{
    QString textType, textMessage;

    //Selon le mode modifie le texte
    if(modeOption == "BDD")
    {
        textType = "attention";
        textMessage = "La courses va être exporté en ligne, souhaiter vous continuer ?";
    }
    if(modeOption == "RESET")
    {
        textType = "attention";
        textMessage = "L'affichage de la course va être réinitialiser, souhaiter vous continuer ?";
    }

    //Active les bouton nécessaire a se type d'interface
    ui->btn_return->show();
    ui->btn_next->show();

    //Définie le message d'entête afficher et sont style
    ui->lbl_infoType->setText(textType);
    ui->lbl_infoType->setFont(infoHeaderTextStyle);
    ui->lbl_infoType->setAlignment(Qt::AlignCenter);
    ui->lbl_infoType->setStyleSheet("color:orange;border-style: none;");

    //Définie le message afficher et sont style
    ui->lbl_infoMessage->setText(textMessage);
    ui->lbl_infoMessage->setFont(infoMessageTextStyle);
    ui->lbl_infoMessage->setAlignment(Qt::AlignCenter);
    ui->lbl_infoMessage->setStyleSheet("color:white;border-style: none;");

    //Définie le texte et le style du bouton suivant
    ui->btn_next->setText("Suivant");
    ui->btn_next->setFont(infoBtnStyle);
    ui->btn_next->setStyleSheet("background-color: #353535; color: white; border: 2px solid #FFFFFF;border-radius: 5px; border-color: orange;");

    //Définie le texte et le style du bouton retour
    ui->btn_return->setText("Retour");
    ui->btn_return->setFont(infoBtnStyle);
    ui->btn_return->setStyleSheet("background-color: #353535; color: white; border: 2px solid #FFFFFF;border-radius: 5px; border-color: orange;");

    //Définie le style de la fenêtre
    this->setStyleSheet("border-color: orange; border-width : 3px; border-style: solid;");

    //Connecte les boutons a leur évènement
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(warning_btn_next()));
    connect(ui->btn_return, SIGNAL(clicked()), this, SLOT(warning_btn_return()));
}

//Fenêtre qui affiche que le transfère s'est bien dérouler
void OptionInformation::SetInformationSuccess()
{
    //Masque le bouton retour et affiche le bouton suivant
    ui->btn_return->hide();
    ui->btn_next->show();

    //Définie le message d'entête afficher et sont style
    ui->lbl_infoType->setText("succès");
    ui->lbl_infoType->setFont(infoHeaderTextStyle);
    ui->lbl_infoType->setAlignment(Qt::AlignCenter);
    ui->lbl_infoType->setStyleSheet("color:green;border-style: none;");

    //Définie le message afficher et sont style
    ui->lbl_infoMessage->setText("Le transfert de la course a bien été éffectuer !");
    ui->lbl_infoMessage->setFont(infoMessageTextStyle);
    ui->lbl_infoMessage->setAlignment(Qt::AlignCenter);
    ui->lbl_infoMessage->setStyleSheet("color:white;border-style: none;");

    //Définie le texte et le style du bouton suivant
    ui->btn_next->setText("Fermer");
    ui->btn_next->setFont(infoBtnStyle);
    ui->btn_next->setStyleSheet("background-color: #353535; color: white;border: 2px solid #FFFFFF;border-radius: 5px; border-color: green;");

    //Définie le style de la fenêtre
    this->setStyleSheet("border-color: green; border-width : 3px; border-style: solid;");

    //Connecte le bouton à sont évènement
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(success_btn_next()));
}

//Fenêtre qui affiche qu'une erreur à eu lieux lors du transfert
void OptionInformation::SetInformationError()
{
    //Masque le bouton retour et affiche le bouton suivant
    ui->btn_return->hide();
    ui->btn_next->show();

    //Définie le message d'entête afficher et sont style
    ui->lbl_infoType->setText("échec");
    ui->lbl_infoType->setFont(infoHeaderTextStyle);
    ui->lbl_infoType->setAlignment(Qt::AlignCenter);
    ui->lbl_infoType->setStyleSheet("color:red;border-style: none;");

    //Définie le message afficher et sont style
    ui->lbl_infoMessage->setText("Un problème est survenue lors du transfert de la course en ligne !");
    ui->lbl_infoMessage->setFont(infoMessageTextStyle);
    ui->lbl_infoMessage->setAlignment(Qt::AlignCenter);
    ui->lbl_infoMessage->setStyleSheet("color:white;border-style: none;");

    //Définie le texte et le style du bouton suivant
    ui->btn_next->setText("Fermer");
    ui->btn_next->setFont(infoBtnStyle);
    ui->btn_next->setStyleSheet("background-color: #353535; color: white;border: 2px solid #FFFFFF;border-radius: 5px; border-color: red;");

    //Définie le style de la fenêtre
    this->setStyleSheet("border-color: red; border-width : 3px; border-style: solid;");

    //Connecte le bouton à sont évènement
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(failure_btn_next()));
}

//Fenêtre qui s'affiche le temps du transfert de la bdd
void OptionInformation::SetInformationProcess()
{
    //Masque les 2 boutons
    ui->btn_return->hide();
    ui->btn_next->hide();

    //Définie le message d'entête afficher et sont style
    ui->lbl_infoType->setText("Transfère");
    ui->lbl_infoType->setFont(infoHeaderTextStyle);
    ui->lbl_infoType->setAlignment(Qt::AlignCenter);
    ui->lbl_infoType->setStyleSheet("color:orange;border-style: none;");

    //Définie le message afficher et sont style
    ui->lbl_infoMessage->setText("Transfère en cours, veuillez patienter...");
    ui->lbl_infoMessage->setFont(infoMessageTextStyle);
    ui->lbl_infoMessage->setAlignment(Qt::AlignCenter);
    ui->lbl_infoMessage->setStyleSheet("color:white;border-style: none;");

    //Définie le style de la fenêtre
    this->setStyleSheet("border-color: orange; border-width : 3px; border-style: solid;");
}

// Lors de l'appuie sur le bouton suivant dans la fenêtre d'avertissement
void OptionInformation::warning_btn_next()
{
    //Déconnecte les boutons de leurs évènement
    ui->btn_next->disconnect();
    ui->btn_return->disconnect();

    //Transfère la base de données mer vers la base de données terre
    if(modeOption == "BDD")
    {
        //Affiche l'interface de transfert
        SetInformationProcess();

        //Si le transfert à réussie on affiche la fenêtre succès
        //Autrement celle d'échecs
        if(optionWindow->mainWindow->ExportBDD() == true)
            SetInformationSuccess();
        else
            SetInformationError();
    }

    //Réinisialisation de l'affichage de la course
    if(modeOption == "RESET")
    {
        optionWindow->mainWindow->ResetCourse();
        optionWindow->setDisabled(false);
        this->close();
    }
}

// Lors de l'appuie sur le bouton retour dans la fenêtre d'avertissement
void OptionInformation::warning_btn_return()
{
    //Déconnecte les boutons de leurs évènement
    ui->btn_next->disconnect();
    ui->btn_return->disconnect();

    //On réactive la fenêtre des paramètres
    optionWindow->setDisabled(false);

    //Ferme la fenêtre
    this->close();
}

// Lors de l'appuie sur le bouton suivant dans la fenêtre de succès de transfert
void OptionInformation::success_btn_next()
{
    //Déconnecte le bouton de sont évènement
    ui->btn_next->disconnect();

    //On réactive la fenêtre des paramètres
    optionWindow->setDisabled(false);

    //Ferme la fenêtre
    this->close();
}

// Lors de l'appuie sur le bouton suivant dans la fenêtre d'échec de transfert
void OptionInformation::failure_btn_next()
{
    //Déconnecte le bouton de sont évènement
    ui->btn_next->disconnect();

    //On réactive la fenêtre des paramètres
    optionWindow->setDisabled(false);

    //Ferme la fenêtre
    this->close();
}
