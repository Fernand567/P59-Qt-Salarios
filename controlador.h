#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <QObject>

#include "obrero.h"

#define MATITUNO 2.65
#define VESPERTINO 2.73
#define NOCTURNO 2.87

#define HORA_EXTRA 50
#define IESS 9.5

class Controlador : public QObject
{
    Q_OBJECT
public:
    explicit Controlador(QObject *parent = nullptr);
    bool agregarObrero(QString nombre, int horas, TipoJornada jornada);
    bool calcularSalario();
    Obrero *obrero() const;


    double getSalarioNetototal() const;

    double getTotaliees() const;

    double getSalarioBrutototal() const;

    void setSalarioBrutototal(double newSalarioBrutototal);

    void setSalarioNetototal(double newSalarioNetototal);

    void setTotaliees(double newTotaliees);

signals:

private:
    Obrero *m_obrero;
    double salarioBrutototal=0;
    double salarioNetototal=0;
    double totaliees=0;

};

#endif // CONTROLADOR_H
