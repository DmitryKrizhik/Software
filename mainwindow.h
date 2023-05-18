#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <thread>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QTcpSocket socket;
    static int pressureType;
    static int temperatureType;
    QTimer* timer = new QTimer(this);

private slots:


    void setLight();
    void setDark();
    void setEnabledTrue();
    void setEnabledFalse();
    void TurnON();
    void TurnOFF();
    void on_Turn_ON_OFF_clicked();
    void on_Simulating_Start_Stop_clicked();
    void on_ResolutionBox_activated(int index);
    void on_ThemeBox_activated(int index);
    void SetPressureType(int &pressureType, int _pressureType);
    void on_SendRequest_button_clicked();
    void on_PressureBox_activated(int index);
    void SetTemperatureType(int &temperatureType, int _temperatureType);
    void on_TemperatureBox_activated(int index);
    void setColorButton_ON_OFF();
    void SendRequest();
    void startSendingRequests();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
