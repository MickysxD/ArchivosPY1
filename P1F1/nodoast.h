#ifndef NODOAST_H
#define NODOAST_H
#include <qstring.h>
#include <qlist.h>

class NodoAST
{
public:
    int fila;
    int columna;
    QString tipo;
    QString valor;
    QList<NodoAST> param;

    NodoAST();
    NodoAST(QString tipo, QString valor, int fila, int columna);
    NodoAST(QString tipo, QString valor, int fila, int columna, QList<NodoAST> nuevo);
    void add(NodoAST nuevo);
    void recorrer(NodoAST root);
};

#endif // NODOAST_H
