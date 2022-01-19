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

void Salarios::guardar()
{
    // Abrir cuadro de diálogo para seleccionar ubicación y nombre del archivo.
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de salarios (*.slr)");

    // Crear un objeto QFile
    QFile archivo(nombreArchivo);
    // Abrirlo para escritura
    if(archivo.open(QFile::WriteOnly | QFile::Truncate)){
        // Crear un 'stream' de texto
        QTextStream salida(&archivo);
        // Enviar los datos del resultado a la salida
        salida << ui->outResultado->toPlainText();
        salida.operator<<("\n--------------------\n");
        salida.operator<<("Total\n");
        salida.operator<<("Salario bruto: "+ QString::number(m_controlador->getSalarioBrutototal())+"\n");
        salida.operator<<("IESS: "+QString::number(m_controlador->getTotaliees())+"\n");
        salida.operator<<("Salario Neto: "+QString::number(m_controlador->getSalarioNetototal())+"\n");
        salida.operator<<("\n--------------------\n");
        // Mostrar 5 segundo que todo fue bien
        ui->statusbar->showMessage("Datos almacenados en " + nombreArchivo, 5000);
    }else {
        // Mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(this,
                             "Guardar datos",
                             "No se pudo guardar el archivo");
    }
    // Cerrar el archivo
    archivo.close();

}

void Salarios::abrir()
{
    // Abrir cuadro de diálogo para seleccionar ubicación y nombre del archivo.
    QString nombreArchivo = QFileDialog::getOpenFileName(this,
                                                         "Abrir archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de salarios (*.slr)");

    // Crear un objeto QFile
    QFile archivo(nombreArchivo);
    // Abrirlo para lectura
    if(archivo.open(QFile::ReadOnly)){
        // Crear un 'stream' de texto
        QTextStream entrada(&archivo);
        // Leer todo el contenido del archivo
        QString datos="";
        QString nombre;

        while(nombre!="--------------------" && entrada.atEnd()==false){

            nombre=entrada.readLine();
            if(nombre=="--------------------"){

            }else{
                datos+=nombre+"\n";
            }
        }
        // Cargar el contenido al área de texto
        ui->outResultado->clear();
        ui->outTotalBruto->clear();
        ui->outTotalIESS->clear();
        ui->outTotalNeto->clear();
        // imprimimos todo lo que va antes de los resultados totales
        ui->outResultado->setPlainText(datos);
        //leemos la linea "Total:"
        entrada.readLine();
        // impresion y asignacion de el total del salario bruto
        QString a;
        a=entrada.readLine();
        a.remove(0,14);
        m_controlador->setSalarioBrutototal(a.toDouble());
        ui->outTotalBruto->setText(a);
        //impresion y asignacion del total IESS
        QString b;
        b=entrada.readLine();
        b.remove(0,6);
        m_controlador->setTotaliees(b.toDouble());
        ui->outTotalIESS->setText(b);
        //impresion y asignacion del Total Neto
        QString c;
        c=entrada.readLine();
        c.remove(0,13);
        m_controlador->setSalarioNetototal(c.toDouble());
        ui->outTotalNeto->setText(c);
        // Mostrar 5 segundo que todo fue bien
        ui->statusbar->showMessage("Datos leidos desde " + nombreArchivo, 5000);
    }else {
        // Mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(this,
                             "Abrir datos",
                             "No se pudo abrir el archivo");
    }
    // Cerrar el archivo
    archivo.close();
}


void Salarios::on_actionCalcular_triggered()
{
    calcular();
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

    // Validar datos correctos
    if (nombre == "" || horas == 0){
        /*
        QMessageBox msgBox;
        msgBox.setText("El nombre o el número de horas está vacío");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        */
        QMessageBox::warning(this,"Advertencia","El nombre o el número de horas está vacío");
        return;
    }

    // Agregar obrero al controlador
    m_controlador->agregarObrero(nombre, horas, jornada);
    // Calcular
    if (m_controlador->calcularSalario()){
        // muestra los resultados de los calculos del obrero
        ui->outResultado->appendPlainText(m_controlador->obrero()->toString());
        // muestra los resultados totales de los obreros
        ui->outTotalBruto->setText(QString::number(m_controlador->getSalarioBrutototal(),'t',2)+"\n");
        ui->outTotalNeto->setText(QString::number(m_controlador->getSalarioNetototal(),'t',2)+"\n");
        ui->outTotalIESS->setText(QString::number(m_controlador->getTotaliees(),'t',2)+"\n");
        // limpiar la interfaz
        limpiar();
        // Mostrar mensaje por 5 segundos en la barra de estado
        ui->statusbar->showMessage("calculos procesados para " + nombre, 5000);
    }else {
        QMessageBox::critical(
                    this,
                    "Error",
                    "Error al calcular el salario.");
    }
}


void Salarios::on_actionGuardar_triggered()
{
    guardar();
}


void Salarios::on_actionNuevo_triggered()
{
    limpiar();
    ui->outTotalBruto->clear();
    ui->outTotalNeto->clear();
    ui->outTotalIESS->clear();
    ui->outResultado->clear();
}


void Salarios::on_actionAbrir_triggered()
{
    abrir();
}


void Salarios::on_actionAcerca_de_Salarios_triggered()
{
    // Crear un objeto de la ventana que queremos invocar
    Acerca *dialogo = new Acerca(this);
    // Enviar parámetro a la ventana
    dialogo->setVersion(VERSION);
    // Mostrar la ventana (diálogo) MODAL
    dialogo->exec();
    // Obtener datos de la ventana
    qDebug() << dialogo->valor();
}
