#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <quazip/JlCompress.h>

#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <qsysinfo.h>


#include <iostream>

#include <utilities.h>
#include <packet_part.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadZips(packetPath, "packet_name", ui->packetComboBox, packets);
    on_packetComboBox_activated(0); // force update of description box
    loadZips(driverModulePath, "driver_name", ui->driverComboBox, driverModules);
    on_driverComboBox_activated(0); // force update of description box
    loadZips(controlModulePath, "control_name", ui->controlComboBox, controlModules);
    on_controlComboBox_activated(0); // force update of description box
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadZips(QString path, QString namePropName, QComboBox *selectorBox, QMap<QString, FMZipInfo>& map) {
    QString startDir = QDir::currentPath();
    std::cout << "Current Dir: " << QDir::currentPath().toStdString() << std::endl;
    QDir::setCurrent(path);
    std::cout << "New Dir: " << QDir::currentPath().toStdString() << std::endl;
    QFileInfoList files = QDir::current().entryInfoList();
    std::cout << "Count: " << files.length() << std::endl;

    for(QFileInfo f : files) {
        std::cout << "File: " << f.completeBaseName().toStdString() << '.' << f.suffix().toStdString() << std::endl;
        if(f.suffix() == "jar" || f.suffix() == "zip") {
            FMZipInfo i(f.absoluteFilePath());
            QString key = i["ZipFilePath"];
            int matchIndex = map.size();
            if(map.find(key) != map.end()) {
                matchIndex = selectorBox->findData(key);
                selectorBox->removeItem(matchIndex);
            }
            selectorBox->insertItem(matchIndex, i[namePropName], QVariant(key));
            map.insert(key, i);
        }
    }
    QDir::setCurrent(startDir);
}

void MainWindow::on_packetComboBox_activated(int index)
{
    (void)index; //make compiler think it's used

    QString fpath = ui->packetComboBox->currentData().toString();
    std::cout << "Packet Selected: " << fpath.toStdString() << std::endl;

    FMZipInfo info = packets[fpath];

    ui->packetDescBox->setText(info["packet_description"]);

    ui->addMelodicButton->setDisabled(info["can_play_melody"] != "true");
    ui->addHarmonicButton->setDisabled(info["can_play_harmony"] != "true");
    ui->addSupportingButton->setDisabled(info["can_play_support"] != "true");
}

void MainWindow::on_driverComboBox_activated(int index)
{
    (void)index; //make compiler think it's used

    QString fpath = ui->driverComboBox->currentData().toString();
    std::cout << "Driver Module Selected: " << fpath.toStdString() << std::endl;

    FMZipInfo info = driverModules[fpath];

    ui->driverModuleDescBox->setText(info["driver_description"]);
}


void MainWindow::on_controlComboBox_activated(int index)
{
    (void)index; //make compiler think it's used

    QString fpath = ui->controlComboBox->currentData().toString();
    std::cout << "Control Module Selected: " << fpath.toStdString() << std::endl;

    FMZipInfo info = controlModules[fpath];

    ui->controlModuleDescBox->setText(info["control_description"]);
}

void MainWindow::on_actionLoad_Instruments_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::DontUseNativeDialog
                                                    | QFileDialog::DontResolveSymlinks);
    loadZips(dir, "packet_name", ui->packetComboBox, packets);
    on_packetComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionLoad_Driver_Modules_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::DontUseNativeDialog
                                                    | QFileDialog::DontResolveSymlinks);
    loadZips(dir, "driver_name", ui->driverComboBox, driverModules);
    on_driverComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionLoad_Control_Modules_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::DontUseNativeDialog
                                                    | QFileDialog::DontResolveSymlinks);
    loadZips(dir, "control_name", ui->controlComboBox, controlModules);
    on_controlComboBox_activated(0); // force update of description box
}

void MainWindow::on_addMelodicButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QString jarfile = ui->packetComboBox->currentData().toString();
    QTreeWidgetItem* next = new QTreeWidgetItem(tree, Qt::UserRole);
    tree->setCurrentItem(next);
    next->setText(0, packets[jarfile]["packet_name"]);
    next->setData(0, Qt::ToolTipRole, "melodic");
    next->setData(0, Qt::UserRole, QVariant(jarfile));
}

void MainWindow::on_addHarmonicButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* cur = tree->currentItem();
    QString jarfile = ui->packetComboBox->currentData().toString();
    if(!cur) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please select a Packet in the composition tree before adding a Harmonic Packet");
        msgBox.exec();
    } else {
        QTreeWidgetItem* next = new QTreeWidgetItem(cur, Qt::UserRole);
        tree->setCurrentItem(next);
        next->setText(0, packets[jarfile]["packet_name"]);
        next->setData(0, Qt::ToolTipRole, "harmonic");
        next->setData(0, Qt::UserRole, QVariant(jarfile));
    }
    tree->expandAll();
}

void MainWindow::on_addSupportingButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* cur = tree->currentItem();
    QString jarfile = ui->packetComboBox->currentData().toString();
    if(!cur) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please select a Packet in the composition tree before adding a Supporting Packet");
        msgBox.exec();
    } else {
        QTreeWidgetItem* next = new QTreeWidgetItem(cur, Qt::UserRole);
        next->setText(0, packets[jarfile]["packet_name"]);
        next->setData(0, Qt::ToolTipRole, "supporting");
        next->setData(0, Qt::UserRole, QVariant(jarfile));
    }
    tree->expandAll();
}

void MainWindow::on_removeInstrumentButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* cur = tree->currentItem();
    if(!cur) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please select a Packet in the composition tree");
        msgBox.exec();
    } else {
        int index = tree->indexOfTopLevelItem(cur);
        //qt tree items delete children when deleted, thankfully
        if(index != -1) {
            delete tree->takeTopLevelItem(index);
        } else {
            cur->parent()->removeChild(cur);
            delete cur;
        }
    }
}

void MainWindow::on_actionClear_Instruments_triggered()
{
    ui->packetComboBox->clear();
    loadZips(packetPath, "packet_name", ui->packetComboBox, packets);
    on_packetComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionClear_Driver_Modules_triggered()
{
    ui->packetComboBox->clear();
    loadZips(driverModulePath, "driver_name", ui->driverComboBox, driverModules);
    on_driverComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionClear_Control_Modules_triggered()
{
    ui->packetComboBox->clear();
    loadZips(controlModulePath, "control_name", ui->controlComboBox, controlModules);
    on_controlComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionClear_Instrument_Hierarchy_triggered()
{
    ui->compositionTree->clear();
}

std::string executeStd(std::string zipPath, std::string mode, std::string input) {
    QString a = execute(QString::fromStdString(zipPath),
                   QString::fromStdString(mode),
                   QString::fromStdString(input)
                   );
    return a.toStdString();
}

void buildTreeHelper(PacketPart* node, QTreeWidgetItem* w, PacketPart* parent) {
    FMZipInfo z = mw->packets[w->data(0, Qt::UserRole).toString()];
    node->set_parent(parent);
    node->set_packet_path(z["ZipFilePath"].toStdString());
    node->set_mode(w->data(0, Qt::ToolTipRole).toString().toStdString());

    for(int i = 0; i < w->childCount(); i++) {
        PacketPart* newPP = new PacketPart;
        buildTreeHelper(newPP, w->child(i), node);
        node->append_child(newPP);
    }
}


void buildTree(PacketPart* rootItem, QTreeWidget* uiTree) {
    //FOR NOW only support single melodic line
    //int numMelodic = uiTree.topLevelItemCount();
    buildTreeHelper(rootItem, uiTree->topLevelItem(0), NULL);
}

void MainWindow::on_composeButton_clicked()
{
    PacketPart* tree = new PacketPart();
    buildTree(tree, ui->compositionTree);
    executeShell(executeStd, tree, ui->driverComboBox->currentData().toString().toStdString(), ui->controlComboBox->currentData().toString().toStdString());
}

QString execute(QString zipPath, QString mode, QString input){

    QProcess *process = new QProcess(mw);
    if(mode == "play") {
        QString program = "python";
        process->start(program, QStringList() << "./res/player/player.py");

        process->waitForStarted(-1);
        if(input.length() > 0){
            std::cout << "sending input" << std::endl;
            process->write(input.toStdString().c_str(), input.length());
            process->write(" \n" , 2);
            process->waitForBytesWritten(-1);
        }
        process->closeWriteChannel();
        process->waitForReadyRead(-1);
        process->waitForFinished(-1);
        QString output = QString(process->readAllStandardOutput());
        QString err = QString(process->readAllStandardError());

        delete process;
        std::cout << "-----out-----" << std::endl;
        std::cout << output.toStdString() << std::endl;
        std::cout << "-----err-----" << std::endl;
        std::cout << err.toStdString() << std::endl;
        std::cout << "-----end-----" << std::endl;
        return output;
    } else {

        std::cout << "=======\n INPUT\n=======\n" << input.toStdString() << "\n=======\n" << std::endl;

        QProcess *process = new QProcess(mw);
        FMZipInfo zipInfo;
        std::cout << "Path: " << zipPath.toStdString() << std::endl;
        if(mode == "driver") {
            zipInfo = mw->driverModules[zipPath];
        } else if(mode == "control" || mode == "finalcontrol") {
            zipInfo = mw->controlModules[zipPath];
        } else {
            zipInfo = mw->packets[zipPath];
        }
        QString execType = zipInfo["exec_type"];
        std::cout << "Exec type: " << execType.toStdString() << std::endl;
        if (execType == "exe") {
            JlCompress::extractDir(zipPath, "./temp");
            QString program = "./temp/";
            if(QSysInfo::kernelType() == "linux") {
                program += zipInfo["linux_bin"];
            } else if (QSysInfo::productType() == "windows") {
                program += zipInfo["windows_bin"];
            } else {
                program += zipInfo["osx_bin"];
            }
            process->start(program, QStringList());
        }else if (execType == "python") {
            QString startDir = QDir::currentPath();
            std::cout << "Current Dir: " << QDir::currentPath().toStdString() << std::endl;
            QDir::setCurrent("./temp");
            std::cout << "New Dir: " << QDir::currentPath().toStdString() << std::endl;
            QString program = "python";
            JlCompress::extractDir(zipPath, ".");
            process->start(program, QStringList() << "./__main__.py");
            QDir::setCurrent(startDir);
        }else {// if(execType == "java") {
            QString program = "java";
            process->start(program, QStringList() << "-jar" << zipPath);
        }
        process->waitForStarted(-1);
        if(mode.length() > 0){
            std::cout << "Mode: " << mode.toStdString() << std::endl;
            process->write(mode.toStdString().c_str(), mode.length());
            process->write(" \n" , 2);
            process->waitForBytesWritten(-1);
        }
        if(input.length() > 0){
            std::cout << "sending input" << std::endl;
            process->write(input.toStdString().c_str(), input.length());
            process->write(" \n" , 2);
            process->waitForBytesWritten(-1);
        }
        process->closeWriteChannel();
        process->waitForReadyRead(-1);
        process->waitForFinished(-1);
        QString output = QString(process->readAllStandardOutput());
        QString err = QString(process->readAllStandardError());

        delete process;
        std::cout << "-----out-----" << std::endl;
        std::cout << output.toStdString() << std::endl;
        std::cout << "-----err-----" << std::endl;
        std::cout << err.toStdString() << std::endl;
        std::cout << "-----end-----" << std::endl;
        return output;
    }
}
