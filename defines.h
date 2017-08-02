#ifndef DEFINES_H
#define DEFINES_H


#define CURRENT_VER "0.51"

#define LinuxBoard // раздефайнить если мы ходим скомпилировать на плату
#define MYD

#ifdef LinuxBoard
#define comportname "/dev/ttyO1" // com port for MYD board
#define pathtofile  "/opt/"
#define pathtosystemoptions  "/opt/systemoptions.txt"
#define pathtoarchive  "/opt/archive.txt"
#define pathtooptions  "/opt/options.txt"
#define pathtolog  "/opt/Log.txt"
#define uartsleep DelayMsec(50);
#define threadsleep DelayMsec(100);
#define longsleep DelayMsec(1000);
#endif

#ifndef LinuxBoard
#define comportname "COM3"
#define uartsleep Sleep(50);
#define longsleep Sleep(1000);
#define pathtofile  "C:/Work/"
#define pathtosystemoptions  "C:/Work/systemoptions.txt"
#define pathtooptions  "C:/Work/options.txt"
#define pathtolog  "C:/Work/Log.txt"
#define pathtoarchive  "C:/Work/archive.txt"
#endif

#define BADCRCCODE -9999
#define CONNECTERRORCODE -9998

#define ConnectionErrorMessage "ош.соед"
#define CRCErrorMessage "ош.crc"
#define ObryvErrorMessage "обрыв"

#define NaNMessage "nan"

#define MultiThread // если убрать , то приложение будет однопоточное (пока многопоточное предпочтительнее по скорости и т.п.)

#define ValuesUpdateTimer 100
#define GraphicsUpdateTimer 250
#define ArchiveUpdateTimer 5000
#define DateLabelUpdateTimer 1000
#define XRange 300
#define YRange 180

#endif // DEFINES_H