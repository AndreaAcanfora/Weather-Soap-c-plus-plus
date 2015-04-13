#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QString>

#include "soapGlobalWeatherSoapProxy.h"
//*
//*
//* @author Andrea Acanfora
//*
//*
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() // evento click del bottone
{
    try{
    QString city = ui->textEdit->toPlainText();// prendo il input la città
    QString nation = ui->textEdit2->toPlainText();// prendo in input la nazione

    GlobalWeatherSoapProxy service;
    _ns1__GetWeather* meteo = new _ns1__GetWeather(); // dichiaro un oggetto di tipo _ns1__GetWeather
    meteo->CityName = new std::string(city.toUtf8().constData()); // faccio capire al web service la città
    meteo->CountryName = new std::string(nation.toUtf8().constData()); // faccio capire al web service la nazione

    _ns1__GetWeatherResponse response;

    if (service.GetWeather(meteo, response) == SOAP_OK) { // se è andato a buon fine
        QString xml = QString::fromStdString(*(response.GetWeatherResult));
        bool flag = true;
        if(ui->checkBox->isChecked()){ // se voglio il file in xml setto il testo e metto flag a false
            ui->meteo->setText(xml);
            flag = false;
        }
        if(flag){ // se non lo voglio xml agisco sul testo
        QString prova;
        QString tipe;
        int count = 2; // contatore (parte da 2 perchè deve essere divisibile per 2)
        for(int i = 0; i<xml.length()-1;i++){ // scorro tutti i caratteri della stringa
            if(xml[i] == '>' && xml[i+1]!= '<'){ // appena trova la fine di un tag
                //che non riinizi subito con un'altro
                i++;
                for(int k = 0; k < xml.length(); k++,i++){ // continuo a incrementare i ma incremento anche k
                    if(xml[i]=='<') // se trova la fine di un tag esce dal for
                        break;
                    prova[k] = xml[i]; // se siamo all'inter
                }
                switch(count){ // do alla stringa tipe indipendentemente dall'indice una stringa
                    case 4:
                        tipe = "City: ";
                        break;
                    case 6:
                        tipe = "Day: ";
                        break;
                    case 8:
                        tipe = "Wind: ";
                        break;
                    case 10:
                        tipe = "Visibility: ";
                        break;
                    case 12:
                        tipe = "Temperature: ";
                        break;
                    case 14:
                        tipe = "DewPoint: ";
                        break;
                    case 16:
                        tipe = "RelativeHumidity: ";
                        break;
                    case 18:
                        tipe = "Pressure: ";
                        break;
                }

                if(count % 2 == 0 && count != 20) // inserisco solo le righe che mi servono
                    ui->meteo->append(tipe+prova+"\n"); // aggiungo la stringa alla textfield
                count++;
                for(int j = 0; j<prova.length();j++)
                    prova[j] = ' '; // resetto la stringa
            }
        }
    }
    } else { // se non è andato a buon fine mostra il motivo
        ui->meteo->setText(QString::fromStdString(service.soap_fault_string()));
    }
    } catch (std::exception &e) {
        ui->meteo->setText("Compilation error");
    }
}
