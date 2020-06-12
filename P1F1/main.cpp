#include <qdebug.h>
#include <iostream>
#include <string>
#include <parser.h>
#include <scanner.h>
#include <nodoast.h>
#include <prueba.h>
#include <qstring.h>

#include <stdio.h>
#include <ctime>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <mkdisk.h>
#include <fdisk.h>
#include <mount.h>

using namespace std;

extern int yyparse();
extern NodoAST *root;
extern int linea; // Linea del token
extern int columna; // Columna de los tokens
extern int yylineno;
QList<MOUNT> lmount = QList<MOUNT>();
char letra = 'a';

int leer_mkdisk(MKDISK *mk, string path){
    FILE *disco = fopen(path.c_str(), "rb");

    if(disco != NULL){
        fseek(disco, 0, SEEK_SET);
        fread(&(*mk), sizeof(MKDISK), 1, disco);
        fclose(disco);
        return 1;
    }else{
        return 0;
    }
}

void vermount(){
    for(int i=0; i<lmount.size(); i++){
        qDebug()<< lmount[i].name.c_str();
        qDebug()<< lmount[i].path.c_str();
        qDebug()<< lmount[i].id.c_str();
        qDebug()<<"";
    }
}

int escribir_mkdisk(MKDISK mk, string path){
    FILE *disco = fopen(path.c_str(),"rb+");
    fseek(disco,0,SEEK_SET);
    fwrite(&mk, sizeof(MKDISK), 1, disco);
    fclose(disco);
}

int escribir_logica(string path, FDISK nuevo){
    FILE *disco = fopen(path.c_str(),"rb+");
    fseek(disco, nuevo.start,SEEK_SET);
    fwrite(&nuevo, sizeof(FDISK), 1, disco);
    fclose(disco);
}

int leer_fdisk(FDISK *fd, int pos, string path){
    FILE *disco = fopen(path.c_str(), "rb");
    if(disco != NULL){
        fseek(disco, pos, SEEK_SET);
        fread(&(*fd), sizeof(FDISK), 1, disco);
        fclose(disco);
        return 1;
    }else{
        return 0;
    }
}

FDISK leer_logica(int p, string path){
    FILE *disco = fopen(path.c_str(), "rb");
    FDISK lg;

    if(disco != NULL){
        fseek(disco, p, SEEK_SET);
        fread(&lg, sizeof(FDISK), 1, disco);
        fclose(disco);
        return lg;
    }
}

QList<FDISK> burbuja(QList<FDISK> list_tmp){
    FDISK tmp;
    for(int i = 0; i < list_tmp.size(); i++){
        for (int j = 0;j <list_tmp.size()-1; j++) {
            if(list_tmp[j].size_total > list_tmp[j+1].size_total){
                tmp = list_tmp[j];
                list_tmp[j] =  list_tmp[j+1];
                list_tmp[j+1] = tmp;
            }
        }
    }

    return list_tmp;
}

int int_pos(MKDISK mk, int size, char fit){
    QList<FDISK> part = QList<FDISK>();
    int espacio = 0;
    for(int i=0; i<4; i++){
        FDISK temp = mk.pos_part[i];
        if(temp.estado == -1 && temp.size_total >= size){
            part.append(mk.pos_part[i]);
            espacio += temp.size_total;
        }else if(temp.estado == -1){
            espacio += temp.size_total;
        }
    }

    part = burbuja(part);
    int p =-1;
    if(part.size() != 0){
        if(fit == 'F'){
            p = 0;

        }else if(fit == 'B'){
            p = 0;

        }else if(fit == 'W'){
            p = part.size()-1;
        }

        for(int i=0; i<4; i++){
            string n1 = mk.pos_part[i].name;
            string n2 = part[p].name;
            if(n1 == n2){
                return i;
            }
        }

    }else if((mk.size_disp-espacio) >= size){
        for(int i=0; i<4; i++){
            if(mk.pos_part[i].estado == 0){
                return i;
            }
        }
    }

    return p;

}

int exist_logicas(string path, string name){
    FILE *disco = fopen(path.c_str(), "rb");
    FDISK fs;

    if(disco != NULL){
        fseek(disco, sizeof(MKDISK), SEEK_SET);
        fread(&fs, sizeof(FDISK), 1, disco);

        while(fs.estado != 0){
            string n = fs.name;
            if(fs.estado == 1 && n == name){
                fclose(disco);
                return -1;

            }else if(fs.next == -1){
                fclose(disco);
                return 0;
            }

            fseek(disco, fs.next, SEEK_SET);
            fread(&fs, sizeof(FDISK), 1, disco);
        }
    }

    fclose(disco);
    return 0;
}

int desactivar_logicas(string path){
    FILE *disco = fopen(path.c_str(), "rb");
    FDISK fs;

    if(disco != NULL){
        fseek(disco, sizeof(MKDISK), SEEK_SET);
        fread(&fs, sizeof(FDISK), 1, disco);
        FDISK n = FDISK(0, ' ', ' ', "", 0, 0);
        n.estado = 0;

        while(fs.estado != 0){
            n.start = fs.start;
            escribir_logica(path, n);
            fseek(disco, fs.next, SEEK_SET);
            fread(&fs, sizeof(FDISK), 1, disco);
        }
    }

    fclose(disco);
    return 0;
}

int buscar_ext(MKDISK mk){
    for(int i=0; i<sizeof(mk.pos_part);i++){
        if(mk.pos_part[i].type == 'E'){
            return i;
        }
    }
    return -1;
}

void desactivar_l(string path, string name){
    FILE *disco = fopen(path.c_str(), "rb");
    FDISK lg;
    MKDISK mk;

    fseek(disco, 0, SEEK_SET);
    fread(&mk, sizeof(MKDISK), 1, disco);

    fseek(disco, sizeof(MKDISK), SEEK_SET);
    fread(&lg, sizeof(FDISK), 1, disco);

    while(true){
        string n = lg.name;
        if(n == name){
            lg.estado = -1;
            escribir_logica(path, lg);

            int e = buscar_ext(mk);
            mk.pos_part[e].size_disp += lg.size_total;
            escribir_mkdisk(mk, path);

            break;
        }
        if(lg.next == -1 || lg.estado == 0){
            break;
        }
        fseek(disco, lg.next, SEEK_SET);
        fread(&lg, sizeof(FDISK), 1, disco);
    }

    fclose(disco);

}

int exist_nombre(string path, string name){
    FILE *disco = fopen(path.c_str(), "rb");
    MKDISK mk;
    FDISK lg;
    fseek(disco, 0, SEEK_SET);
    fread(&mk, sizeof(MKDISK), 1, disco);

    for(int i=0; i<4; i++){
        string n = mk.pos_part[i].name;
        if(n == name && mk.pos_part[i].estado == 1){
            return i;
        }
    }

    fseek(disco, sizeof(MKDISK), SEEK_SET);
    fread(&lg, sizeof(FDISK), 1, disco);

    while(true){
        string n = lg.name;
        if(n == name && lg.estado == 1){
            return lg.start;
        }
        if(lg.estado == 0 || lg.next == -1){
            break;
        }
        fseek(disco, lg.next, SEEK_SET);
        fread(&lg, sizeof(FDISK), 1, disco);
    }

    fclose(disco);

    return -1;
}

int startFDISK(MKDISK mk, int b){
    int r = sizeof(MKDISK);
    for(int i=0; i<b; i++){
        if(mk.pos_part[i].estado != 0){
            r += mk.pos_part[i].size_total;
        }
    }
    return r;
}

int buscar_mount(string id){
    for(int i=0; i<lmount.size(); i++){
        if(lmount[i].id == id){
            return i;
        }
    }
    return -1;
}

int pos_logica(FDISK fd, string path, char fit, int size){
    FILE *disco = fopen(path.c_str(), "rb");
    FDISK ld;
    QList<FDISK> llog = QList<FDISK>();
    int espacio = 0;
    int ultima = sizeof(MKDISK);

    if(disco != NULL){
        fseek(disco, sizeof(MKDISK), SEEK_SET);
        fread(&ld, sizeof(FDISK), 1, disco);

        while(ld.estado != 0){
            if(ld.estado == -1 && ld.size_total >= size){
                llog.append(ld);
                espacio += ld.size_total;
            }else if(ld.estado == -1){
                espacio += ld.size_total;
            }

            if(ld.next == -1){
                ultima = ld.start+sizeof(FDISK);
                break;
            }

            fseek(disco, ld.next, SEEK_SET);
            fread(&ld, sizeof(FDISK), 1, disco);
        }
        fclose(disco);

        llog = burbuja(llog);
        int p =-1;
        if(llog.size() != 0){
            if(fit == 'F'){
                p = 0;
            }else if(fit == 'B'){
                p = 0;
            }else if(fit == 'W'){
                p = llog.size()-1;
            }

            return llog[p].start;

        }else if((fd.size_disp-espacio) >= size){
            return ultima;
        }else{
            return p;
        }

    }
    fclose(disco);
    return -1;

}

int real_size(int size, char unit){
    if(unit == 'K'){
        return size*1024;
    }else{
        return size*1024*1024;
    }
}

int confirmar(int n){
    string r;
    if(n == 1){
        qDebug()<<"Eliminar disco? [s/n]";
    }else if(n == 2){
        qDebug()<<"Eliminar particion? [s/n]";
    }
    cin >> r;

    if(r == "s" || r == "S"){
        return 1;
    }else if(r == "n" || r == "N"){
        return 0;
    }else{
        qDebug()<<"Intente de nuevo\n";
        return confirmar(n);
    }

}

void carpeta(QString path){
    QStringList lista = path.split("/");

    QString fpath = "";

    for(int i=1; i<lista.size()-1; i++){
        fpath = fpath+"/"+lista[i];
        string spath = fpath.toStdString();
        char p[200];

        for(int j=0; j<200; j++){
            p[j] = spath[j];
        }

        //qDebug()<<p;
        mkdir(p,0777);
    }

}

void comando_mkdisk(NodoAST nodo){
    time_t ttemp = time(0);
    tm *tmtime = localtime(&ttemp);
    QString qtime= QString::number(tmtime->tm_mday)+"/"+QString::number(tmtime->tm_mon+1)+"/"+QString::number(tmtime->tm_year+1900)+"  "+  QString::number(tmtime->tm_hour)+":"+QString::number(tmtime->tm_min);

    string stime = qtime.toStdString();

    char time[16];
    int random = rand();
    char name[16] = "";

    int size = 0;
    char fit = 'F';
    char unit = 'M';
    string path = "";

    string raid = "";

    for(int i=0; i<16; i++) {
        time[i] = stime[i];
    }

    for(int i=0; i < nodo.param.size(); i++){
        NodoAST temp = nodo.param.at(i);

        if(temp.tipo == "SIZE"){
            size = temp.valor.toInt();

        }else if(temp.tipo == "FIT"){
            string ftemp = temp.valor.toUpper().toStdString();
            if(ftemp != "BF" && ftemp != "FF" && ftemp != "WF"){
                fit = ' ';
            }else{
                fit = ftemp[0];
            }

        }else if(temp.tipo == "UNIT"){
            string utemp = temp.valor.toUpper().toStdString();
            if(utemp != "K" && utemp != "M"){
                unit = ' ';
            }else{
                unit = utemp[0];
            }

        }else if(temp.tipo == "PATH"){
            temp.valor = temp.valor.replace("\"","");
            temp.valor = temp.valor.replace("/home","/home/micky");
            carpeta(temp.valor);
            path = temp.valor.toStdString();

            QStringList lista = temp.valor.split("/");
            QString qname = lista.value(lista.length()-1);
            string sname = qname.toStdString();
            for(int i=0; i<16; i++){
                name[i] = sname[i];
            }

            QString qraid = temp.valor.replace(".disk","_raid.disk");
            raid = qraid.toStdString();

        }
    }

    if(size != 0 && path != "" && fit != ' ' && unit != ' '){
        FILE *archivo=fopen(path.c_str(),"wb");

        if(archivo != NULL){
            int tm = real_size(size, unit);

            //mkdisk
            fseek(archivo, tm, SEEK_SET);
            fwrite("", 1, 1, archivo);

            fclose(archivo);

            MKDISK mk = MKDISK(tm, fit, unit, name, time, random);

            archivo=fopen(path.c_str(),"rb+");
            fseek(archivo,0,SEEK_SET);
            fwrite(&mk, sizeof(MKDISK), 1, archivo);
            fclose(archivo);
            qDebug()<<"MKDISK:"<< name<< " creado";

            //raid del mkdisk
            FILE *archivor=fopen(raid.c_str(),"wb");
            fseek(archivor, tm, SEEK_SET);
            fwrite("", 1, 1, archivor);
            fclose(archivor);

        }else{
            qDebug()<<"Error al crear archivo, comando MKDISK (path no exite)";
        }

    }else{
        if(size == 0){
            qDebug()<<"Falta parametro Size en comando MKDISK";
        }
        if(path == ""){
            qDebug()<<"Falta parametro Path en comando MKDISK";
        }
        if(fit == ' '){
            qDebug()<<"Parametro Fit no valido en comando MKDISK";
        }
        if(unit == ' '){
            qDebug()<<"Parametro Unit no valido en comando MKDISK";
        }
    }

}

void comando_rmdisk(NodoAST nodo){
    string path = "";
    for(int i=0; i<nodo.param.length(); i++){
        NodoAST temp = nodo.param[i];
        if(temp.tipo == "PATH"){
            temp.valor = temp.valor.replace("\"","");
            temp.valor = temp.valor.replace("/home","/home/micky");
            path = temp.valor.toStdString();
        }
    }

    if(path != ""){
        FILE* archivo = fopen(path.c_str(), "rb");
        if(archivo != NULL){
            if(confirmar(1) == 1){
                remove(path.c_str());
                qDebug()<<"Disco eliminado";
            }else{
                qDebug()<<"Eliminacion cancelada";
            }
        }else{
            qDebug()<<"Error en comando RMDISK (path no existe)";
        }
    }
}

void comando_fdisk(NodoAST nodo){
    int size = 0;
    char unit = 'K';
    string path = "";
    char type = 'P';
    char fit = 'W';
    char del[16] = "";
    char name[16] = "";
    int add = 0;

    for(int i=0; i < nodo.param.size(); i++){
        NodoAST temp = nodo.param.at(i);
        //qDebug()<<temp.tipo;
        //qDebug()<<temp.valor;
        if(temp.tipo == "SIZE"){
            size = temp.valor.toInt();

        }else if(temp.tipo == "FIT"){
            string ftemp = temp.valor.toUpper().toStdString();
            if(ftemp != "BF" && ftemp != "FF" && ftemp != "WF"){
                fit = ' ';
            }else{
                fit = ftemp[0];
            }

        }else if(temp.tipo == "UNIT"){
            string utemp = temp.valor.toUpper().toStdString();
            if(utemp != "B" && utemp != "K" && utemp != "M"){
                unit = ' ';
            }else{
                unit = utemp[0];
            }
        }else if(temp.tipo == "PATH"){
            temp.valor = temp.valor.replace("\"","");
            temp.valor = temp.valor.replace("/home","/home/micky");
            path = temp.valor.toStdString();
        }else if(temp.tipo == "TYPE"){
            string ttemp = temp.valor.toUpper().toStdString();
            if(ttemp != "P" && ttemp != "E" && ttemp != "L"){
                type = ' ';
            }else{
                type = ttemp[0];
            }
        }else if(temp.tipo == "DELETE"){
            string dtemp = temp.valor.toUpper().toStdString();
            if(dtemp != "FAST" && dtemp != "FULL"){
                dtemp = "";
            }
            for(int i=0; i<16; i++){
                del[i] = dtemp[i];

            }
        }else if(temp.tipo == "NAME"){
            temp.valor = temp.valor.replace("\"","");
            string ntemp = temp.valor.toStdString();
            for(int i=0; i<16; i++){
                name[i] = ntemp[i];
            }
        }else if(temp.tipo == "ADD"){
            add = temp.valor.toInt();
        }

    }

    if(path != ""){
        MKDISK mk;
        if(leer_mkdisk(&mk, path) == 1){

            if(strncmp(name, "", 16) != 0 && add != 0 && unit != ' '){
                //add
                add = real_size(add, unit);
                int p = exist_nombre(path, name);
                if(p == -1){
                    qDebug()<<"No existe particion (E,P,L), comando FDISK";
                }else if(p > 5){
                    FDISK lg = leer_logica(p, path);

                    if(add > 0){
                        if(lg.size_total-lg.size >= add){
                            lg.size += add;
                            lg.size_disp += add;
                            escribir_logica(path, lg);
                            qDebug()<<"Espasio agregado, comando FDISK";
                        }else{
                            qDebug()<<"No existe espasio suficiente para agregar, comando FDISK";
                        }

                    }else{
                        if(lg.size+add >= 0){
                            lg.size += add;
                            lg.size_disp += add;
                            escribir_logica(path, lg);
                            qDebug()<<"Espasio agregado, comando FDISK";
                        }else{
                            qDebug()<<"No existe espasio suficiente para quitar, comando FDISK";
                        }
                    }

                }else if(p >= 0){
                    if(add > 0){
                        if(mk.pos_part[p].size_total-mk.pos_part[p].size >= add){
                            mk.pos_part[p].size += add;
                            mk.pos_part[p].size_disp += add;
                            escribir_mkdisk(mk,path);
                            qDebug()<<"Espasio agregado, comando FDISK";
                        }else{
                            qDebug()<<"No existe espasio suficiente para agregar, comando FDISK";
                        }
                    }else{
                        if(mk.pos_part[p].size+add >= 0 && mk.pos_part[p].size_disp+add>=0){
                            mk.pos_part[p].size += add;
                            mk.pos_part[p].size_disp += add;
                            escribir_mkdisk(mk,path);
                            qDebug()<<"Espacio quitado, comando FDISK";
                        }else{
                            qDebug()<<"No existe espasio suficiente para quitar, comando FDISK";
                        }
                    }

                }

            }else if(strncmp(name, "", 16) != 0 && strncmp(del, "", 16) != 0){
                //delete
                int e = exist_nombre(path, name);
                if(e == -1){
                    qDebug()<<"No existe particion (E,P,L) con ese nombre, comando FDISK";
                }else{
                    if(confirmar(2) == 1){
                        if(e > 4){
                            desactivar_l(path, name);
                            qDebug()<<"Particion logica eliminada, comando FDISK";
                        }else if(e >= 0){
                            mk.pos_part[e].estado = -1;
                            mk.size_disp += mk.pos_part[e].size_total;
                            if(mk.pos_part[e].type == 'E'){
                                mk.ext = 0;
                                desactivar_logicas(path);
                            }
                            mk.num_part--;
                            escribir_mkdisk(mk, path);
                            qDebug()<<"Particion eliminada, comando FDISK";
                        }
                    }else{
                        qDebug()<<"Eliminacion cancelada, comando FDISK";
                    }
                }


            }else if(size != 0 && strncmp(name, "", 16) != 0 && unit != ' ' && type != ' ' && fit != ' '){
                //particion extendidas o primarias
                int tm = real_size(size, unit);

                if(type == 'P' || type == 'E'){
                    if(tm > mk.size_disp){
                        qDebug()<<"No hay espacio suficiente para la particion, comando FDISK";
                    }else if(exist_nombre(path, name) != -1){
                        qDebug()<<"Existe particion o logica con el mismo nombre, comando FDISK";
                    }else if(mk.num_part == 4){
                        qDebug()<<"Ya existen 4 particiones, comando FDISK";
                    }else if(type == 'E' && mk.ext == 1){
                        qDebug()<<"Existe particion extendida, comando FDISK";
                    }else {
                        int b = int_pos(mk, tm, fit);
                        if(b == -1){
                            qDebug()<<"No hay espacio que cumpla, comando FDISK";
                        }else{
                            FDISK ant = mk.pos_part[b];
                            int start = startFDISK(mk, b);
                            if(ant.estado == 0){
                                FDISK fd = FDISK(tm, type, fit, name, start);
                                mk.size_disp -= tm;
                                if(type == 'E'){
                                    mk.ext = 1;
                                }
                                mk.num_part++;
                                mk.pos_part[b] = fd;
                                escribir_mkdisk(mk, path);
                                qDebug()<<"Particion creada, comando FDISK";
                            }else{
                                FDISK fd = FDISK(tm, type, fit, name, ant.start);
                                fd.size_total = ant.size_total;
                                mk.size_disp -= tm;
                                if(type == 'E'){
                                    mk.ext = 1;
                                }
                                mk.num_part++;
                                mk.pos_part[b] = fd;
                                escribir_mkdisk(mk, path);
                                qDebug()<<"Particion creada, comando FDISK";
                            }
                        }
                    }

                }else if(type == 'L'){
                    //particiones logicas
                    if(mk.ext == 0){
                        qDebug()<<"No existe particion extendida para una Logica, comando FDISK";
                    }else if(exist_nombre(path, name) != -1){
                        qDebug()<<"Existe particion o logica con el mismo nombre, comando FDISK";
                    }else{
                        int ext = buscar_ext(mk);
                        FDISK fd = mk.pos_part[ext];
                        if(fd.size_disp < tm){
                            qDebug()<<"No hay espacio suficiente para la logica, comando FDISK";
                        }else{
                            int lp = pos_logica(fd, path, fit, tm);
                            if(lp != -1){
                                FDISK log = leer_logica(lp, path);

                                FDISK nuevo = FDISK(tm, type, fit, name, lp, log.next);

                                if(lp > sizeof(MKDISK)){
                                    FDISK anterior = leer_logica(lp-sizeof(FDISK), path);
                                    anterior.next = lp;
                                    escribir_logica(path, anterior);
                                }

                                if(nuevo.next == 0){
                                   nuevo.next = -1;
                                }else{
                                    nuevo.size_total = log.size_total;
                                }

                                escribir_logica(path, nuevo);

                                fd.size_disp -= tm;
                                mk.pos_part[ext] = fd;
                                escribir_mkdisk(mk, path);
                                qDebug()<<"Logica creada, comando FDISK";\


                            }else{
                                qDebug()<<"Error al encontrar espacio, comando FDISK";
                            }
                        }

                    }
                }else{
                    qDebug()<<"ERROR desconocido, comando FDISK";
                }

            }

        }else{
            qDebug()<<"Direccion de Path no existe, comando FDISK";
        }

    }else{
        qDebug()<<"Error Disco no existe, comando FDISK";
    }



}

int comando_mount(NodoAST nodo){
    string path = "";
    string name = "";

    for(int i=0; i < nodo.param.size(); i++){
        NodoAST temp = nodo.param.at(i);

        if(temp.tipo == "PATH"){
            temp.valor = temp.valor.replace("\"","");
            temp.valor = temp.valor.replace("/home","/home/micky");
            path = temp.valor.toStdString();

        }else if(temp.tipo == "NAME"){
            temp.valor = temp.valor.replace("\"","");
            name = temp.valor.toStdString();
        }
    }

    if(path != "" && name != ""){
        MKDISK mk;
        if(leer_mkdisk(&mk, path) == 1){
            int e = exist_nombre(path, name);
            if(e == -1 || e > 3){
                qDebug()<<"No existe particion con ese nombre, comando MOUNT";
            }else{
                string nombre = "";
                bool n = false;
                for(int i=0; i<lmount.size(); i++){
                    if(name == lmount[i].name && path == lmount[i].path){
                        n = true;
                    }else if(path == lmount[i].path){
                        nombre = lmount[i].id;
                    }
                }
                MOUNT mt;
                string nm;
                if(n){
                    qDebug()<<"Ya existe Mount con mismo name y path, comando MOUNT";
                }else {
                    if(nombre != ""){
                        int i = (int)nombre[nombre.length()-1] - 47;
                        nm += nombre[0];
                        nm += nombre[1];
                        nm += nombre[2];
                        nm += to_string(i);
                        mt = MOUNT(path, name, nm);
                    }else{
                        string nm = "vd";
                        nm += letra;
                        nm += "1";
                        mt = MOUNT(path, name, nm);
                        letra++;
                    }
                    lmount.append(mt);
                    qDebug()<<"Mount con exito, comando MOUNT";
                }

                vermount();
            }
        }else{
            qDebug()<<"Path no existe, comando MOUNT";
        }

    }else{
        qDebug()<<"Falta parametro path o name, comando MOUNT";
    }

}

int comando_unmount(NodoAST nodo){
    string id = "";

    for(int i=0; i < nodo.param.size(); i++){
        NodoAST temp = nodo.param.at(i);
        if(temp.tipo == "ID"){
            temp.valor = temp.valor.replace("\"","");
            id = temp.valor.toStdString();
        }
    }

    if(id != ""){
        int um = -1;
        for(int i=0; i<lmount.size(); i++){
            if(id == lmount[i].id){
                um = i;
                break;
            }
        }

        if(um == -1){
            qDebug()<<"Id de Mount no existe, comando UNMOUNT";
        }else {
            lmount.removeAt(um);
            qDebug()<<"Unmount con exito, comando UNMOUNT";
        }

        vermount();

    }else{
        qDebug()<<"Falta parametro name, comando UNMOUNT";
    }

}

int comando_rep(NodoAST nodo){
    string id = "";
    string path = "";
    string name = "";

    for(int i=0; i < nodo.param.size(); i++){
        NodoAST temp = nodo.param.at(i);
        if(temp.tipo == "ID"){
            temp.valor = temp.valor.replace("\"","");
            id = temp.valor.toStdString();
        }else if(temp.tipo == "NAME"){
            temp.valor = temp.valor.replace("\"","");
            if(temp.valor.toUpper() == "MBR" || temp.valor.toUpper() == "DISK"){
                name = temp.valor.toUpper().toStdString();
            }
        }else if(temp.tipo == "PATH"){
            temp.valor = temp.valor.replace("\"","");
            temp.valor = temp.valor.replace("/home","/home/micky");
            path = temp.valor.toStdString();
        }
    }

    if(id == ""){
        qDebug()<<"Falta id, comando REP";
    }else if(name == ""){
        qDebug()<<"Falta name, comando REP";
    } else if(path == ""){
        qDebug()<<"Falta path, comando REP";
    }else{
        int m = buscar_mount(id);

        if(m == -1){
            qDebug()<<"Id de Mount no existe, comando REP";
        }else {
            MOUNT mt = lmount[m];
            MKDISK mk;
            if(leer_mkdisk(&mk, mt.path) == -1){
                qDebug()<<"Error con el path, comando REP";
            }else{
                if(name == "MBR"){
                    ofstream archivo;
                    archivo.open(path+".dot",ios::out);



                }else{

                }
            }
        }

        vermount();

    }

}

int p(){
    FILE *disco = fopen("/home/micky/archivos/fase1/Disco1.disk", "rb");
    MKDISK mk;
    FDISK fd;
    if(disco != NULL){
        fseek(disco, 0, SEEK_SET);
        fread(&mk, sizeof(MKDISK), 1, disco);

        qDebug()<<"MKDISK";
        qDebug()<<mk.name;
        qDebug()<<mk.size;
        qDebug()<<mk.size_disp;
        qDebug()<<mk.time;
        qDebug()<<"";

        qDebug()<<"PARTICIONES";
        for(int i=0; i<4; i++){
            if(mk.pos_part[i].estado != 0){
                fd = mk.pos_part[i];
                qDebug()<<fd.estado;
                qDebug()<<fd.name;
                qDebug()<<fd.size_total;
                qDebug()<<fd.size;
                qDebug()<<fd.size_disp;
                qDebug()<<fd.type;
                qDebug()<<"";
            }
        }

        fseek(disco, sizeof(MKDISK), SEEK_SET);
        fread(&fd, sizeof(FDISK), 1, disco);

        qDebug()<<"LOGICAS";
        if(fd.estado != 0){
            qDebug()<<fd.estado;
            qDebug()<<fd.name;
            qDebug()<<fd.next;
            qDebug()<<fd.size_total;
            qDebug()<<fd.size;
            qDebug()<<fd.size_disp;
            qDebug()<<fd.type;
            qDebug()<<"";

            while(fd.next != -1){
                fseek(disco, fd.next, SEEK_SET);
                fread(&fd, sizeof(FDISK), 1, disco);

                qDebug()<<fd.estado;
                qDebug()<<fd.name;
                qDebug()<<fd.next;
                qDebug()<<fd.size_total;
                qDebug()<<fd.size;
                qDebug()<<fd.size_disp;
                qDebug()<<fd.type;
                qDebug()<<"";
            }
        }

        fclose(disco);

    }else{
        qDebug()<<"error ver";

    }
}

int ejecutar_exec(){

    for(int i=0; i < root->param.length(); i++){
        NodoAST temp = root->param[i];
        if(temp.valor == "MKDISK"){
            comando_mkdisk(temp);

        }else if(temp.valor == "RMDISK"){
            comando_rmdisk(temp);

        }else if(temp.valor == "FDISK"){
            comando_fdisk(temp);

        }else if(temp.valor == "MOUNT"){
            comando_mount(temp);

        }else if(temp.valor == "UNMOUNT"){
            comando_unmount(temp);

        }else if(temp.valor == "REP"){


        }else if(temp.tipo == "COMENTARIO"){
            temp.valor = temp.valor.replace("#","");
            qDebug()<<"Comentario--->"<<temp.valor.toUtf8().constData();
            }
        qDebug()<<"-------------------------------------------------------------------------";
    }

}

int comando_exec(NodoAST nodo){
    string path = "";

    for(int i=0; i < nodo.param.size(); i++){
        NodoAST temp = nodo.param.at(i);
        if(temp.tipo == "PATH"){
            temp.valor = temp.valor.replace("\"","");
            temp.valor = temp.valor.replace("/home","/home/micky");
            path = temp.valor.toStdString();
        }
    }

    if(path != ""){
        ifstream archivo;
        archivo.open(path,ios::in);
        if(archivo.fail())
        {
            qDebug()<<"el archivo no existe";
            return 0;
        }else{
            QString dato = "";
            string cadena = "";
            int i=1;
            while (!archivo.eof()) {
                getline(archivo, cadena);
                dato = cadena.c_str();
                dato = dato.replace("\r","");
                if(dato != ""){
                    yy_scan_string(dato.toUtf8().constData());
                    if(yyparse() == 0){
                        qDebug()<<"LINEA-"+i;
                        qDebug()<<"-------------------------------------------------------------------------";
                        ejecutar_exec();
                        qDebug()<<"-------------------------------------------------------------------------";
                    }
                }
            }
            archivo.close();

        }

    }else{
        qDebug()<<"Falta parametro path, comando EXEC";
    }
}

int ejecutar(){

    for(int i=0; i < root->param.length(); i++){
        NodoAST temp = root->param[i];
        qDebug()<<"-------------------------------------------------------------------------";
        if(temp.valor == "MKDISK"){
            comando_mkdisk(temp);

        }else if(temp.valor == "RMDISK"){
            comando_rmdisk(temp);

        }else if(temp.valor == "FDISK"){
            comando_fdisk(temp);

        }else if(temp.valor == "MOUNT"){
            comando_mount(temp);

        }else if(temp.valor == "UNMOUNT"){
            comando_unmount(temp);

        }else if(temp.valor == "REP"){
            comando_rep(temp);

        }else if(temp.valor == "EXEC"){
            comando_exec(temp);
            return 0;
        }else if(temp.tipo == "COMENTARIO"){
            temp.valor = temp.valor.replace("#","");
            qDebug()<<"Comentario--->"<<temp.valor.toUtf8().constData();
        }
        qDebug()<<"-------------------------------------------------------------------------";
    }

}

int main()
{
    qDebug()<<"     Comando --> ";
    QString dato="";
    //desactivar_logicas("/home/micky/archivos/fase1/Disco1.disk");
    p();

    while(dato == ""){
        QTextStream qtin(stdin);
        dato = qtin.readLine();

        YY_BUFFER_STATE bf = yy_scan_string(dato.toUtf8().constData());

        if(yyparse() == 0){

            ejecutar();

        }

    }
    main();
    return 1;
}

