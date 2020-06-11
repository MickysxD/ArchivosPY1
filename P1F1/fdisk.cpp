#include "fdisk.h"



FDISK::FDISK()
{
    this->estado = 0;
    this->size = 0;
}

FDISK::FDISK(int size, char type, char fit, char name[16], int start)
{
    this->estado = 1;
    this->size = size;
    this->type = type;
    this->fit = fit;

    for(int i=0; i<16; i++){
        this->name[i] = name[i];
    }

    this->size_total = size;
    this->size_disp = size;
    this->start = start;
}

FDISK::FDISK(int size, char type, char fit, char name[16], int start, int next)
{
    this->estado = 1;
    this->size = size;
    this->type = type;
    this->fit = fit;

    for(int i=0; i<16; i++){
        this->name[i] = name[i];
    }

    this->size_total = size;
    this->size_disp = size;
    this->start = start;
    this->next = next;
}
