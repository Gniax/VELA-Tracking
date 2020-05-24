#include "gps.h"
#include <QCoreApplication>
#include <QMessageLogger>
// C library headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iterator>
#include <map>
#include <iomanip>
#include <sstream>
// Linux headers
#include <fcntl.h> // fichier de contrôle O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

using namespace std;

static inline float randomFloat(float a, float b);

GPS::GPS(QObject* parent) : QThread(parent)
{
    _gpsSimulation = false;
    _mode = "COUREUR"; // Par défaut le mode sera COUREUR...
    _latitude = "\r";
    _longitude = "\r";
    _dLatitude = "\r";
    _dLongitude = "\r";
    _vitesse = "\r";
    _timestamp = 0;

    logger.info("Initialisation du GPS");
}

GPS::~GPS(){}

void GPS::run()
{
    if (_gpsSimulation)
    {
        SimulationDeCourse(1);
       /* while (true)
        {
            float tempLatitude = 45 + randomFloat(0.1, 2);
            float tempLongitude = -0.95 + randomFloat(0.01, 0.1);
            float tempVitesse = 0.00000;
            if (_mode == "COUREUR")
                tempVitesse = randomFloat(0, 20);

            ostringstream ssLatitude, ssLongitude, ssVitesse;
            ssLatitude << tempLatitude;
            ssLongitude << tempLongitude;
            ssVitesse << tempVitesse;

            string latitude(ssLatitude.str());
            string longitude(ssLongitude.str());
            string vitesse(ssVitesse.str());

            _latitude = latitude.c_str();
            _longitude = longitude.c_str();
            _dLatitude = 'W';
            _dLongitude = 'Z';
            _vitesse = vitesse.c_str();
            _timestamp = time(0);
            sleep(5);
        }*/
    }
    else
    {
        // Ouverture du port USB
        int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );

        // Termios pour tty
        struct termios tty;
        struct termios tty_old;
        memset (&tty, 0, sizeof tty);

        /* Erreur */
        if ( tcgetattr ( USB, &tty ) != 0 ) {
        cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
        }

        /* Sauvegarde les paramètres */
        tty_old = tty;

        /* baud rate */
        cfsetospeed (&tty, (speed_t)B4800);
        cfsetispeed (&tty, (speed_t)B4800);

        /* paramètres */
        tty.c_cflag 	&=  ~PARENB;        	// Make 8n1
        tty.c_cflag 	&=  ~CSTOPB;
        tty.c_cflag 	&=  ~CSIZE;
        tty.c_cflag 	|=  CS8;

        tty.c_cflag 	&=  ~CRTSCTS;       	//pas de controle de flux
        tty.c_cc[VMIN]   =  1;              	// read doesn't block
        tty.c_cc[VTIME]  =  5;              	// 0.5 seconds read timeout
        tty.c_cflag 	|=  CREAD | CLOCAL; 	// turn on READ & ignore ctrl lines

        /* Make raw */
        cfmakeraw(&tty);

        /* Application des paramètres*/
        tcflush( USB, TCIFLUSH );
        if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
            cout << "Error " << errno << " from tcsetattr" << endl;
        }

        while(true)
        {
            int n = 0,
            spot = 0;
            char buf = '\0';

            /* récupération de la réponse*/
            char response[1024];
            string test(response);

            memset(response, '\0', sizeof response);

            do {
                n = read( USB, &buf, 1 );
                sprintf( &response[spot], "%c", buf );
                spot += n;
            } while( buf != '\r' && n > 0);

            if (n < 0) {
                cout << "Erreur de lecture: " << strerror(errno) << endl;
                sleep(1);
            }
            else if (n == 0) {
                cout << "pas de trame!" << endl;
            }
            else
            {
                logger.info(response);
                //vérification de la bonne trame récupérée
                if (response[1]=='$'&& response[2]=='G'&&response[3]=='P'&&response[4]=='R'&&response[5]=='M'&&response[6]=='C'&& strlen(response)>=60)
                {
                    int a =0;
                    char d[] = ",";
                    char *p = strtok(response, d);
                    while(p != NULL)/*trie de la trame*/
                    {
                        a++;
                        if (a==4)
                        {
                            string tmp(p);
                            _latitude = tmp;
                        }
                        else if (a==5)
                        {
                            string tmp(p);
                            _dLatitude = tmp;
                        }
                        else if (a==6)
                        {
                            string tmp(p);
                            _longitude = tmp;
                        }
                        else if (a==7)
                        {
                            string tmp(p);
                            _dLongitude = tmp;
                        }
                        else if (a==8)
                        {
                             double z = atoi(p);
                             z=z*1.852; // passage des noeuds au km/h
                             char vit[]="";
                             sprintf(vit,"%lf",z);
                             string tmp(vit);
                             _vitesse = tmp;
                             // On définit ici le timestamp... on suppose que toutes les précédentes valeurs sont déjà remplies...
                             _timestamp = time(0);
                        }
                        p = strtok(NULL, d);
                    }
                }
            }
            //faire repartir le programme du début si mauvaise trame
            sleep(1);
        }
    }
}

void GPS::setMode(string mode)
{
    _mode = mode;
    qDebug() << "Mode:" << mode.c_str() << " activé !";
};

void GPS::simulateGPS(bool value)
{
    if (value)
        qDebug() << "*** Simulation du GPS activée ! ***";
    else
        qDebug() << "*** Simulation du GPS désactivée ! ***";

    _gpsSimulation = value;
}

void GPS::SimulationDeCourse(int identifier)
{
    QList<float> allCoordLongitude;
    QList<float> allCoordLatitude;
    if (_mode == "COUREUR")
    {
        if(identifier == 1)
        {
            //Données pour le coureur 1
            allCoordLongitude.append(-1.572542);
            allCoordLatitude.append(46.370376);
            allCoordLongitude.append(-1.576639);
            allCoordLatitude.append(46.368283);
            allCoordLongitude.append(-1.579673);
            allCoordLatitude.append(46.366189);
            allCoordLongitude.append(-1.58374);
            allCoordLatitude.append(46.364116);
            allCoordLongitude.append(-1.580948);
            allCoordLatitude.append(46.361351);
            allCoordLongitude.append(-1.577792);
            allCoordLatitude.append(46.359131);
            allCoordLongitude.append(-1.572967);
            allCoordLatitude.append(46.357917);
            allCoordLongitude.append(-1.566109);
            allCoordLatitude.append(46.356995);
            allCoordLongitude.append(-1.563803);
            allCoordLatitude.append(46.359257);
            allCoordLongitude.append(-1.565714);
            allCoordLatitude.append(46.363089);
            allCoordLongitude.append(-1.567596);
            allCoordLatitude.append(46.366168);
            allCoordLongitude.append(-1.566738);
            allCoordLatitude.append(46.369988);
            allCoordLongitude.append(-1.57047);
            allCoordLatitude.append(46.371726);
        }
        if(identifier == 2)
        {
            //Données pour le coureur 2
            allCoordLongitude.append(-1.570804);
            allCoordLatitude.append(46.371035);
            allCoordLongitude.append(-1.574810);
            allCoordLatitude.append(46.369151);
            allCoordLongitude.append(-1.578694);
            allCoordLatitude.append(46.366889);
            allCoordLongitude.append(-1.582578);
            allCoordLatitude.append(46.364502);
            allCoordLongitude.append(-1.581972);
            allCoordLatitude.append(46.361612);
            allCoordLongitude.append(-1.579969);
            allCoordLatitude.append(46.359183);
            allCoordLongitude.append(-1.573930);
            allCoordLatitude.append(46.357947);
            allCoordLongitude.append(-1.568741);
            allCoordLatitude.append(46.357633);
            allCoordLongitude.append(-1.566192);
            allCoordLatitude.append(46.358890);
            allCoordLongitude.append(-1.566222);
            allCoordLatitude.append(46.363350);
            allCoordLongitude.append(-1.566677);
            allCoordLatitude.append(46.366156);
            allCoordLongitude.append(-1.566829);
            allCoordLatitude.append(46.368962);
            allCoordLongitude.append(-1.568377);
            allCoordLatitude.append(46.371538);
            allCoordLongitude.append(-1.570774);
            allCoordLatitude.append(46.372208);
        }
    }

    if (_mode == "BALISE")
    {
        if(identifier == 1)
        {
            allCoordLongitude.append(-1.570175);
            allCoordLatitude.append(46.369162);
        }
        if(identifier == 2)
        {
            allCoordLongitude.append(-1.580311);
            allCoordLatitude.append(46.362608);
        }
        if(identifier == 3)
        {
            allCoordLongitude.append(-1.56796);
            allCoordLatitude.append(46.358147);
        }
    }





    for(int i = 0;i < allCoordLongitude.count();i++)
    {
        float tempLatitude = allCoordLatitude[i];
        float tempLongitude = allCoordLongitude[i];;
        float tempVitesse = 0.00000;
        if (_mode == "COUREUR")
            tempVitesse = randomFloat(0, 20);

        ostringstream ssLatitude, ssLongitude, ssVitesse;
        ssLatitude << tempLatitude;
        ssLongitude << tempLongitude;
        ssVitesse << tempVitesse;

        string latitude(ssLatitude.str());
        string longitude(ssLongitude.str());
        string vitesse(ssVitesse.str());

        _latitude = latitude.c_str();
        _longitude = longitude.c_str();
        _dLatitude = 'W';
        _dLongitude = 'Z';
        _vitesse = vitesse.c_str();
        _timestamp = time(0);
        sleep(5);
    }
}

string GPS::getMode() { return _mode; };
string GPS::getLatitude() { return _latitude; };
string GPS::getLongitude() { return _longitude; };
string GPS::getDlatitude() { return _dLatitude; };
string GPS::getDlongitude() { return _dLongitude; };
string GPS::getVitesse() { return _vitesse; };

time_t GPS::getTimestamp() { return _timestamp; };

// Fonction utilisée pour la simulation de GPS
float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

