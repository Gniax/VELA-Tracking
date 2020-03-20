/*
qt_gpv : version 2 -> récuperation des trames nmea sur le gps + triage uniquement gprmc avec commentaire et ajustement
Dev:Bertal Bryan
*/


#include <QCoreApplication>
// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

using namespace std;

int main(int argc, char *argv[])
{
    bool var=true;
    int i=0;
    QCoreApplication a(argc, argv);
    while(var=true){
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 ) {
       std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)B4800);
    cfsetispeed (&tty, (speed_t)B4800);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
//    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( USB, TCIFLUSH );
    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
       std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    int n = 0,
    spot = 0;
    char buf = '\0';

    /* Whole response*/
    char response[1024];
    memset(response, '\0', sizeof response);
    do {
        n = read( USB, &buf, 1 );
        sprintf( &response[spot], "%c", buf );
        spot += n;
    } while( buf != '\r' && n > 0);

    if (n < 0) {
        std::cout << "Error reading: " << strerror(errno) << std::endl;
    }
    else if (n == 0) {
        std::cout << "Read nothing!" << std::endl;
    }
    else {
            if (response[0]=='$'&&response[1]=='G'&&response[2]=='P'&&response[3]=='R'&&response[4]=='M'&&response[5]=='C'){

            std::cout << "Response: " << response << std::endl;
            var = false;
            int a =0;
            int len = strlen(response);
              char d[] = ",";
              char *p = strtok(response, d);
              while(p != NULL)
              {
                 a++;
                 if (a==4)
                 {
                    printf("latitude :%s\n", p);
                 }
                 if (a==5)
                 {
                    printf(" direction latitude :%s\n", p);
                 }
                 if (a==6)
                 {
                    printf("longitude :%s\n", p);
                 }
                 if (a==7)
                 {
                    printf(" Direction longitude :%s\n", p);
                 }
                 if (a==8)
                 {
                    printf("vitesse :%s\n", p);
                 }
               p = strtok(NULL, d);
              }


        /*
         * Faire le trie de la trame
        */

            }
            else {
                var=true;
                i++;
                std::cout<<"Je relance pour trouver la bonne trame:"<< i << std::endl;
                sleep(1);
            //faire repartir le programme du début si mauvaise trame
            }
            }
        }

 return a.exec();
}
