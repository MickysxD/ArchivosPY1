#ifndef MKDISK_H
#define MKDISK_H
#include <qstring.h>
#include <fdisk.h>


class MKDISK
{
public:
    int size;
    char fit;
    char unit;
    char name[16];
    char time[16];
    int random;

    int size_disp;
    int num_part;
    FDISK pos_part[4];
    int ext;

    MKDISK();
    MKDISK(int size, char fit, char unit, char name[16], char time[16], int random);

};

#endif // MKDISK_H
