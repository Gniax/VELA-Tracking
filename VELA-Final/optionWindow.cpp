#include "optionwindow.h"
#include "ui_optionwindow.h"

//Constructeur par défaut de l'interface de paramètre
OptionWindow::OptionWindow(VelaWindow* mainwindow, QWidget *parent) : QDialog(parent), ui(new Ui::OptionWindow)
{
    //Instancie la fenêtre
    ui->setupUi(this);

    //Sauvegarde un pointeur vers l'interface principal
    mainWindow = mainwindow;

    //Instanciation de la fenêtre d'affichage
    optionInfo = new OptionInformation(this);

    //Définition des flags qui modifie le comportement de l'interface (pas de bordure, pleine écran)
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);

    //this->showFullScreen();
    ui->label->setPixmap(QPixmap("logo.png"));

    //Définie le style des boutons
    QFont styleBtn;
    styleBtn.setFamily("Helvetica [Cronyx]");
    styleBtn.setStyleHint(QFont::SansSerif);
    styleBtn.setCapitalization(QFont::AllUppercase);
    styleBtn.setPointSize(14);
    styleBtn.setWeight(20);

    //Atribue le style aux boutons de la fenêtre
    ui->btn_back->setFont(styleBtn);
    ui->btn_back->setStyleSheet("background-color: #353535; color: white;border: 2px solid #FFFFFF;");
    ui->btn_reset->setFont(styleBtn);
    ui->btn_reset->setStyleSheet("background-color: #353535; color: white;border: 2px solid #FFFFFF;");
    ui->btn_save_bdd->setFont(styleBtn);
    ui->btn_save_bdd->setStyleSheet("background-color: #353535; color: white;border: 2px solid #FFFFFF;");
}

//Déstructeur par défaut de l'interface
OptionWindow::~OptionWindow()
{
    delete ui;
}

//Evènement lors de l'appuie sur le bouton Retour
void OptionWindow::on_btn_back_clicked()
{
    ///Ferme le fenêtre
    this->close();
}

//Evènement lors de l'appuie sur le bouton Réinitialisé la course
void OptionWindow::on_btn_reset_clicked()
{
    ///Désactive le controle de la fenêtre paramètre
    this->setDisabled(true);

    //Ouvre la fenêtre d'information en mode réinitialisation
    optionInfo->OpenWindow("RESET");
    optionInfo->SetInformationWarning();
}

//Evenement lors de l'appuie sur le bouton Exporter la course
void OptionWindow::on_btn_save_bdd_clicked()
{
    ///Désactive le controle de la fenêtre paramètre
    this->setDisabled(true);

    //Ouvre la fenêtre d'information en mode exportation de course
    optionInfo->OpenWindow("BDD");
    optionInfo->SetInformationWarning();
}

//Evenement temporaire le temps du dévellopement pour fermer le programme
void OptionWindow::on_btn_exit_clicked()
{
    exit(0);
}

