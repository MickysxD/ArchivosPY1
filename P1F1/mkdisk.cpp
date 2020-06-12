#include "mkdisk.h"
#include "fdisk.h"

MKDISK::MKDISK(){

}

MKDISK::MKDISK(int size, char fit, char unit, char name[16], char time[16], int random)
{
    this->size = size;
    this->fit = fit;
    this->unit = unit;

    for(int i=0; i<16; i++){
        this->name[i] = name[i];
        this->time[i] = time[i];
    }

    this->random = random;
    this->size_disp = size;
    this->ext = 0;
    this->num_part = 0;

    this->pos_part[0] = FDISK();
    this->pos_part[1] = FDISK();
    this->pos_part[2] = FDISK();
    this->pos_part[3] = FDISK();
}

