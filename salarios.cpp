#include "salarios.h"
#include "ui_salarios.h"

Salarios::Salarios(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Salarios)
{
    ui->setupUi(this);
    m_controlador = new Controlador();
}

Salarios::~Salarios()
{
    delete ui;
}


void Salarios::on_cmdCalcular_clicked()
{
    calcular();
}

void Salarios::limpiar()
{
    ui->inNombre->setText("");
    ui->inHoras->setValue(0);
    ui->inMatutino->setChecked(true);
    ui->inNombre->setFocus();

}

void Salarios::calcular()
{
    // Obteber datos de la GUI
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if (ui->inMatutino->isChecked()){
        jornada = TipoJornada::Matutina;
    } else if (ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    } else {
        jornada = TipoJornada::Nocturna;
    }
    //Validar datos correctos
    if(nombre=="" || horas==0){
        /*QMessageBox msgBox;
        msgBox.setText("El nombre o el numero de horas esta vacio");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec(); */

        QMessageBox::warning(this,"Advertencia","EL nombre o el numero de horas esta vacio");
        return;
    }

    // Agregar obrero al controlador
    m_controlador->agregarObrero(nombre, horas, jornada);
    // Calcular
    if (m_controlador->calcularSalario()){
        //muestra los resultados de los calculos del obrero
        ui->outResultado->appendPlainText(m_controlador->obrero()->toString());
        //limpiar la interfaz
        limpiar();
        //Mostrar mensaje por 5 segundos en la barra de estado
        ui->statusbar->showMessage("Calculos procesados para " + nombre,5000);
    }
    else{
        QMessageBox::critical(this,
                              "Error",
                              "Error al calcular el salario.");
    }
}

void Salarios::guardar()
{
    //Abrir cuadro de dialogo para seleccionar ubicacion y nombre del archivo.
    QString  nombreArchivo = QFileDialog::getSaveFileName(this,
                                                          "Guardar datos",
                                                          QDir::home().absolutePath(),
                                                          "Archivos de texto (*.txt)");

    qDebug() <<nombreArchivo;
    //Crear un objet QFile
    QFile archivo(nombreArchivo);
    if(archivo.open(QFile::WriteOnly | QFile::Truncate)){
        //Crear un strem de texto
        QTextStream salida(&archivo);
        //Enviar los datos de datos del resultado a la salida
        salida << ui->outResultado->toPlainText();
        //Mostrar 5 segundos que todo esta bien
        ui->statusbar->showMessage("datos almacenados en " + nombreArchivo,5000);
    }else{
        QMessageBox::warning(this,
                             "Guardar datos",
                             "No se pudo guardar los datos");
    }
    archivo.close();
}

void Salarios::on_actionNuevo_triggered()
{

    limpiar();
        ui->outResultado->clear();
}


void Salarios::on_actionGuardar_triggered()
{
    guardar();
}

