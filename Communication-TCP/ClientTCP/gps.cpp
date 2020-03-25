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
// Linux headers
#include <fcntl.h> // fichier de contrôle O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

using namespace std;
GPS::GPS(QObject* parent) : QThread(parent)
{
    latitude = new char();
    longitude = new char();
    dLatitude = new char();
    dLongitude = new char();
    vitesse = new char();
    *latitude = '\r';
    *longitude = '\r';
    *dLatitude = '\r';
    *dLongitude = '\r';
    *vitesse = '\r';

    logger.info("Initialisation du GPS");
}

void GPS::run()
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
                if (response[0]=='$'&&response[1]=='G'&&response[2]=='P'&&response[3]=='R'&&response[4]=='M'&&response[5]=='C' && strlen(response)>=60)
                {//vérification de la bonne trame récupérée
                    int a =0;
                    char d[] = ",";
                    char *p = strtok(response, d);
                    while(p != NULL)/*trie de la trame*/
                    {
                        a++;
                        if (a==4)
                        {
                            strcpy(latitude, p);
                            logger.info(p);
                        }
                        if (a==5)
                        {
                            strcpy(dLatitude, p);
                            logger.info(p);
                        }
                        if (a==6)
                        {
                            strcpy(longitude, p);
                            logger.info(p);
                        }
                        if (a==7)
                        {
                            strcpy(dLongitude, p);
                            logger.info(p);
                        }
                        if (a==8)
                        {
                             double z = atoi(p);
                             z=z*1.852; // passage des noeuds au km/h
                             char vit[]="";
                             sprintf(vit,"%lf",z);
                             strcpy(vitesse, vit);
                        }

                            p = strtok(NULL, d);
                        }
                }
                //faire repartir le programme du début si mauvaise trame

                }
            logger.info("[GPS] Tour de boucle finit...");
            sleep(1);
        }
}
