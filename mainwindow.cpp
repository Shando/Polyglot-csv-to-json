#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <fstream>

#include <wchar.h>
#ifdef __APPLE__
        #include <sys/uio.h>
//#else
//        #include <sys/io.h>
#endif
#include <fcntl.h>

#ifndef _O_U16TEXT
  #define _O_U16TEXT 0x20000
#endif

using namespace std;

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

void MainWindow::on_btnLoadCSV_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.csv)"));

    if ( fileName != "" ) {
        ui->btnConvert->setEnabled(true);
        ui->txtFile->setText(fileName);
        ui->txtConversion->setText("Press the \"Convert to JSON\" Button to create a JSON file with\nthe same name, and in the same location, as the selected file.");
    }
}

void MainWindow::on_btnConvert_clicked()
{
    static QString myArray[35][1500];
    static QString myArray2[35][1500];

    // const char * filename = argv[1];

    /* Declare the variables to be used */
    QByteArray ba = fileName.toLatin1();
    const char *filename = ba.data();

    const char field_terminator = ',';
    const char line_terminator  = '\n';
    const char enclosure_char   = '"';

    csv_parser file_parser;

    /* Define how many records we're gonna skip. This could be used to skip the column definitions. */
    file_parser.set_skip_lines(1);

    /* Specify the file to parse */
    file_parser.init(filename);

    /* Here we tell the parser how to parse the file */
    file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_NONE);
    file_parser.set_field_term_char(field_terminator);
    file_parser.set_line_term_char(line_terminator);

    long row_count = 1;
    long row_width = 0;

    /* Check to see if there are more records, then grab each row one at a time */
    while(file_parser.has_more_rows()) {
        long i = 0;

        /* Get the record */
        csv_row row = file_parser.get_row();
        row_width = row.size();
        QString strTmp;

        for (i = 0; i < row.size(); i++) {
            if (row_count < 4 || row_count > 5) {
                strTmp = QString::fromStdString(row[i]);
                strTmp.simplified();

                if (i > 1) {
                    if ( strTmp.isEmpty() && row_count > 5 ) {
                        myArray[i][row_count] = "No translation currently available.";
                    } else {
                        strTmp.remove(QRegExp("\""));
                        myArray[i][row_count] = strTmp;             // LANGUAGE_CODE - enGB etc.
                    }
                } else if (i == 0) {
                    myArray2[1][row_count] = strTmp;
                } else if (i == 1) {
                    strTmp.remove(QRegExp("\""));
                    myArray2[2][row_count] = strTmp;
                }
            }
        }

        row_count++;
    }

    QString strOut = fileName.left(fileName.length() - 3);
    strOut = strOut + "json";

    ofstream myfile (strOut.toUtf8());

    if (myfile.is_open()) {
        strOut = "{ \"polyglot\": [\n";
        myfile << strOut.toStdString();

        for (int x = 2; x < row_width; x++) {
            strOut = "\t{\t\"lang_code\": \"" + myArray[x][1].toUtf8() + "\",\n";
            strOut.simplified();
            myfile << strOut.toStdString();
            strOut = "\t\t\"lang_name_en\": \"" + myArray[x][2].toUtf8() + "\",\n";
            strOut.simplified();
            myfile << strOut.toStdString();
            strOut = "\t\t\"lang_direction\": \"" + myArray[x][3].toUtf8() + "\",\n";
            strOut.simplified();
            myfile << strOut.toStdString();
            strOut = "\t\t\"codes\": [\n";
            myfile << strOut.toStdString();

            for (int y = 6; y < row_count; y++) {
                strOut = "\t\t\t{\t\"code\": \"" + myArray2[1][y].toUtf8() + "\",\n";
                strOut.simplified();
                myfile << strOut.toStdString();
                strOut = "\t\t\t\t\"comment\": \"" + myArray2[2][y].toUtf8() + "\",\n";
                strOut.simplified();
                myfile << strOut.toStdString();
                strOut = "\t\t\t\t\"translation\": \"" + myArray[x][y].toUtf8() + "\"\n";
                strOut.simplified();
                myfile << strOut.toStdString();

                if ( y == row_count - 1 && x != row_width - 1) {
                    strOut = "\t\t\t}\n";
                    myfile << strOut.toStdString();
                    strOut = "\t\t]\n";
                    myfile << strOut.toStdString();
                    strOut = "\t},\n";
                    myfile << strOut.toStdString();
                } else if (y == row_count - 1 && x == row_width - 1) {
                    strOut = "\t\t\t}\n";
                    myfile << strOut.toStdString();
                    strOut = "\t\t]\n";
                    myfile << strOut.toStdString();
                    strOut = "\t}\n";
                    myfile << strOut.toStdString();
                    strOut = "\t]\n";
                    myfile << strOut.toStdString();
                } else {
                    strOut = "\t\t\t},\n";
                    myfile << strOut.toStdString();
                }
            }
            myfile.flush();
        }

        strOut = "}";
        myfile << strOut.toStdString();
        myfile.close();

        ui->btnConvert->setEnabled(false);
        ui->txtFile->setText("Conversion Successful. Please select another file to convert.");
        ui->txtConversion->setText("");
    } else {
        cout << "Unable to open file";
    }
}
