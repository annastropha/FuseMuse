#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qcombobox.h>

#include "fmzipinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //Maps between the absolute path of a zip and the properties map of the same zip
    QMap<QString, FMZipInfo> packets;
    QMap<QString, FMZipInfo> driverModules;
    QMap<QString, FMZipInfo> controlModules;

private slots:
    void on_packetComboBox_activated(int index);

    void on_driverComboBox_activated(int index);

    void on_controlComboBox_activated(int index);

    void on_actionLoad_Instruments_triggered();

    void on_actionLoad_Driver_Modules_triggered();

    void on_actionLoad_Control_Modules_triggered();

    void on_addMelodicButton_clicked();

    void on_addHarmonicButton_clicked();

    void on_addSupportingButton_clicked();

    void on_removeInstrumentButton_clicked();

    void on_actionClear_Instruments_triggered();

    void on_actionClear_Driver_Modules_triggered();

    void on_actionClear_Control_Modules_triggered();

    void on_actionExit_triggered();

    void on_actionClear_Instrument_Hierarchy_triggered();

    void on_composeButton_clicked();

private:
    //load zips from a directory.  Args are
    //dir, name property name, combobox to load into, map between path and properties
    void loadZips(QString, QString, QComboBox*, QMap<QString, FMZipInfo>&);
    Ui::MainWindow *ui;

    const QString packetPath = "./res/default_packets";
    const QString driverModulePath = "./res/default_driver_modules";
    const QString controlModulePath = "./res/default_control_modules";
};

QString execute(QString zipPath, QString mode, QString input);

extern MainWindow* mw;

#endif // MAINWINDOW_H
