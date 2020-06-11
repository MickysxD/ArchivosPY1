#include "nodoast.h"
#include <qstring.h>
#include <qlist.h>
#include <qdebug.h>

NodoAST::NodoAST()
{
}

NodoAST::NodoAST(QString tipo, QString valor, int fila, int columna)
{
    this->tipo = tipo;
    this->valor = valor;
    this->fila = fila;
    this->columna = columna;
    this->param = QList<NodoAST>();
}

NodoAST::NodoAST(QString tipo, QString valor, int fila, int columna, QList<NodoAST> nuevo)
{
    this->tipo = tipo;
    this->valor = valor;
    this->fila = fila;
    this->columna = columna;
    this->param = nuevo;
}

void NodoAST::add(NodoAST nuevo)
{
    this->param.append(nuevo);
}

void NodoAST::recorrer(NodoAST root)
{
    qDebug()<<"Tipo: "<<root.tipo.toUtf8().constData()<<"Valor: "<<root.valor.toUtf8().constData();

    if(root.param.length() > 0){
        for(int i=0;i<root.param.length();i++){
            qDebug()<<i;
            this->recorrer(root.param[i]);
        }
    }
}
