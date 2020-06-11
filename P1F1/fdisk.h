#ifndef FDISK_H
#define FDISK_H


class FDISK
{
public:
    int estado;
    char type;
    char fit;
    char name[16];
    int size;
    int start;
    int next;

    int size_total;
    int size_disp;

    FDISK();
    FDISK(int size, char type, char fit, char name[16], int start);
    FDISK(int size, char type, char fit, char name[16], int start, int next);
};

#endif // FDISK_H
