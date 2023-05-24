#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setLight();
    SetPressureType(pressureType, 0);
    SetTemperatureType(temperatureType, 0);
    TurnOFF();
    //showFullScreen();
    startSendingRequests();

}

QTcpSocket MainWindow::socket;
QString state;
QString degrees;
QString pressureUnit;
QString theme;

int MainWindow::pressureType;
int MainWindow::temperatureType;
float temperature;
int angle;
int humidity;
int pressure;

void SendRequestEveryFiveSec();
void SetDegrees(QString &degrees, QString _degrees) { degrees = _degrees; }
void SetPressureUnit(QString &pressureUnit, QString _pressureUnit) { pressureUnit = _pressureUnit; }
void SetTemperature(float &temperature, float _temperature) { temperature = _temperature; }
void SetAngle(int &angle, int _angle) { angle = _angle; }
void SetHumidity(int &humidity, int _humidity) { humidity = _humidity; }
void SetPressure(int &pressure, int _pressure) { pressure = _pressure; }
void SetState(QString &state, QString _state) { state = _state; }
void SetTheme(QString &theme, QString _theme) { theme = _theme; }
void MainWindow::SetPressureType(int &pressureType, int _pressureType) { pressureType = _pressureType; }
void MainWindow::SetTemperatureType(int &temperatureType, int _temperatureType) { temperatureType = _temperatureType; }
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLight() //Changes the color theme of the interface to light
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(180, 186, 207));
    pal.setColor(QPalette::Button, QColor(157, 164, 199));
    pal.setColor(QPalette::ButtonText, QColor(252, 255, 252));
    pal.setColor(QPalette::WindowText, QColor(252, 255, 252));
    setPalette(pal);
    SetTheme(theme, "Light");
    setColorButton_ON_OFF();
}

void MainWindow::setDark() //Changes the color theme of the interface to dark
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(99, 107, 133));
    pal.setColor(QPalette::Button, QColor(99, 107, 133));
    pal.setColor(QPalette::ButtonText, QColor(224, 231, 255));
    pal.setColor(QPalette::WindowText, QColor(224, 231, 255));
    setPalette(pal);
    SetTheme(theme, "Dark");
    setColorButton_ON_OFF();

}

void MainWindow::setEnabledTrue() //ON buttons
{
    ui->PressureBox->setEnabled(true);
    ui->ResolutionBox->setEnabled(true);
    ui->ThemeBox->setEnabled(true);
    ui->TemperatureBox->setEnabled(true);
    ui->Simulating_Start_Stop->setEnabled(true);
    ui->PressureBox->setEnabled(true);
    ui->SendRequest_button->setEnabled(true);
}

void MainWindow::setEnabledFalse() //OFF buttons
{
    ui->PressureBox->setEnabled(false);
    ui->ResolutionBox->setEnabled(false);
    ui->ThemeBox->setEnabled(false);
    ui->TemperatureBox->setEnabled(false);
    ui->Simulating_Start_Stop->setEnabled(false);
    ui->PressureBox->setEnabled(false);
    ui->SendRequest_button->setEnabled(false);
}

void MainWindow::TurnON() //ON the application and changes colors depending on the selected color theme
{
    setEnabledTrue();
    ui->Turn_ON_OFF->setText("Turn OFF");
    QPalette pal1 = palette();

    if(theme == "Light")
    {
        pal1.setColor(QPalette::ButtonText, QColor(252, 255, 252));
        pal1.setColor(QPalette::WindowText, QColor(252, 255, 252));
        setPalette(pal1);
    }
    else if(theme == "Dark")
    {
        pal1.setColor(QPalette::ButtonText, QColor(224, 231, 255));
        pal1.setColor(QPalette::WindowText, QColor(224, 231, 255));
        setPalette(pal1);
    }
    setColorButton_ON_OFF();
    //GetInfo();
}

void MainWindow::TurnOFF() //OFF the application and changes colors depending on the selected color theme
{
    setEnabledFalse();
    ui->Turn_ON_OFF->setText("Turn ON");
    QPalette pal1 = palette();
    if(theme == "Light")
    {
        pal1.setColor(QPalette::ButtonText, QColor(157, 164, 199));
        pal1.setColor(QPalette::WindowText, QColor(180, 186, 207));
        setPalette(pal1);
    }
    else if(theme == "Dark")
    {
        pal1.setColor(QPalette::ButtonText, QColor(99, 107, 133));
        pal1.setColor(QPalette::WindowText, QColor(99, 107, 133));
        setPalette(pal1);
    }
    setColorButton_ON_OFF();

}

void MainWindow::on_Turn_ON_OFF_clicked() //Handles the ON/OFF button
{
    setColorButton_ON_OFF();
    if(ui->Turn_ON_OFF->text() == "Turn ON")
    {
        TurnON();
    }
    else if(ui->Turn_ON_OFF->text() == "Turn OFF")
    {
        TurnOFF();
    }
}

void MainWindow::on_ResolutionBox_activated(int index) //Handles the screen resolution selection box
{
       if(index == 0) { setFixedSize(800, 600); }
       else if(index ==1) { setFixedSize(1024,768); }
}

void MainWindow::on_ThemeBox_activated(int index) //Handles the color theme selection box
{
    if(index == 0) { setLight(); }
    else if(index == 1) { setDark(); }
}

void MainWindow::on_SendRequest_button_clicked() //Sends a request to the Simulator
{
    SendRequest();
}

void MainWindow::startSendingRequests() //Sends requests to the Simulator every five seconds
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::SendRequest);
    timer->start(5000);
}

void MainWindow::on_Simulating_Start_Stop_clicked() //Handles pressing the button "Simulating_Start_Stop". ON/OFF the Simulator's interface
{
    socket.connectToHost("127.0.0.1", 8080);
    if(ui->Simulating_Start_Stop->text() == "Start Simulating")
    {
        if (socket.waitForConnected())
        {
            socket.write("ON");
            socket.flush();
        }
        ui->Simulating_Start_Stop->setText("Stop Simulating");
    }
    else if(ui->Simulating_Start_Stop->text() == "Stop Simulating")
    {
        if (socket.waitForConnected())
        {
            socket.write("OFF");
            socket.flush();
        }
        ui->Simulating_Start_Stop->setText("Start Simulating");
    }
}


void MainWindow::SendRequest() //Sends a request to the Simulator and receives
{                              //in response the actual values of the air conditioner parameters
    socket.connectToHost("127.0.0.1", 8080);

    if (socket.waitForConnected()) {
        socket.write("get_data");
        socket.flush();
        socket.waitForBytesWritten();
        socket.waitForReadyRead();
        QString response = socket.readAll();
        QStringList data = response.split(",");

        SetAngle(angle, data.at(2).toInt());
        SetHumidity(humidity, data.at(3).toInt());
        SetState(state, data.at(4));
        SetDegrees(degrees, data.at(5));
        SetPressureUnit(pressureUnit, data.at(6));
        ui->HumidityLabel->setText("Humidity: " + QString::number(humidity) + "%");
        ui->SimulatorLabel->setText("Simulator: " + state);

        if(angle < 0) { ui->AngleLabel->setText("Angle of rotation: " + QString::number(-angle) + " left"); }
        else if(angle > 0) { ui->AngleLabel->setText("Angle of rotatiton: " + QString::number(angle) + " right"); }
        else { ui->AngleLabel->setText("Angle of rotation: " + QString::number(angle)); }


        if(degrees == "Celsius" && ui->TemperatureBox->currentIndex() == 0)
        {
               SetTemperature(temperature, data.at(0).toInt());
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " celsius");
        }
        else if(degrees == "Celsius" && ui->TemperatureBox->currentIndex() == 1)
        {
               SetTemperature(temperature, (data.at(0).toInt() * 9 / 5) + 32);
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " fharenheit");
        }
        else if(degrees == "Celsius" && ui->TemperatureBox->currentIndex() == 2)
        {
               SetTemperature(temperature, data.at(0).toInt() + 273);
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " kelvin");
        }
        else if(degrees == "Fharenheit" && ui->TemperatureBox->currentIndex() == 0)
        {
               SetTemperature(temperature, (data.at(0).toInt() - 32) * 5 / 9);
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " celsius");
        }
        else if(degrees == "Fharenheit" && ui->TemperatureBox->currentIndex() == 1)
        {
               SetTemperature(temperature, data.at(0).toInt());
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " fharenheit");
        }
        else if(degrees == "Fharenheit" && ui->TemperatureBox->currentIndex() == 2)
        {
               SetTemperature(temperature, (data.at(0).toInt() - 32) * 5 / 9 + 273);
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " kelvin");
        }
        else if(degrees == "Kelvin" && ui->TemperatureBox->currentIndex() == 0)
        {
               SetTemperature(temperature, data.at(0).toInt() - 273);
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " celsius");
        }
        else if(degrees == "Kelvin" && ui->TemperatureBox->currentIndex() == 1)
        {
               SetTemperature(temperature, (data.at(0).toInt() - 273) * 9 / 5 + 32);
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " fharenheit");
        }
        else if(degrees == "Kelvin" && ui->TemperatureBox->currentIndex() == 2)
        {
               SetTemperature(temperature, data.at(0).toInt());
               ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " kelvin");
        }

        if(pressureUnit == "Kilopascals" && ui->PressureBox->currentIndex() == 0)
        {
            SetPressure(pressure, data.at(1).toInt());
            ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " kilopascals");
        }
        else if(pressureUnit == "Kilopascals" && ui->PressureBox->currentIndex() == 1)
        {
            SetPressure(pressure, data.at(1).toInt() * 7.5006156);
            ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " mm. of mercury");
        }
        else if(pressureUnit == "mm. of mercury" && ui->PressureBox->currentIndex() == 0)
        {
            SetPressure(pressure, data.at(1).toInt() / 7.5006156);
            ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " kilopascals");
        }
        else if(pressureUnit == "mm. of mercury" && ui->PressureBox->currentIndex() == 1)
        {
            SetPressure(pressure, data.at(1).toInt());
            ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " mm. of mercury");
        }

    socket.close();
        }
}

void MainWindow::on_PressureBox_activated(int index) //Handles the selection of the unit of measure in the PressureBox
{

    if(index == 0 && pressureType == 0)
    {
        SetPressure(pressure, pressure);
        ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " kilopascals");
    }
    else if(index == 0 && pressureType == 1)
    {
        SetPressure(pressure, pressure / 7.5006156);
        ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " kilopascals");
    }
    else if(index == 1 && pressureType == 0)
    {
        SetPressure(pressure, pressure * 7.5006156);
        ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " mm. of mercury");
    }
    else if(index == 1 && pressureType == 1)
    {
        SetPressure(pressure, pressure);
        ui->PressureLabel->setText("Pressure: " + QString::number(pressure) + " mm. of mercury");
    }
    SetPressureType(pressureType, index);
}


void MainWindow::on_TemperatureBox_activated(int index) //Handles the selection of the unit of measure in the TemperatureBox
{
    if(index == 0 && temperatureType == 0)
    {
           SetTemperature(temperature, temperature);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " celsius");
    }
    else if(index == 0 && temperatureType == 1)
    {
           SetTemperature(temperature, (temperature -32) * 5 /9);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " celsius");
    }
    else if(index == 0 && temperatureType == 2)
    {
           SetTemperature(temperature, temperature - 273);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " celsius");
    }
    else if(index == 1 && temperatureType == 0)
    {
           SetTemperature(temperature, (temperature * 9 / 5) + 32);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " fharenheit");
    }
    else if(index == 1 && temperatureType == 1)
    {
           SetTemperature(temperature, temperature);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " fharenheit");
    }
    else if(index == 1 && temperatureType == 2)
    {
           SetTemperature(temperature, (temperature - 273) * 9 / 5 + 32);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " fharenheit");
    }
    else if(index == 2 && temperatureType == 0)
    {
           SetTemperature(temperature, temperature + 273);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " kelvin");
    }
    else if(index == 2 && temperatureType == 1)
    {
           SetTemperature(temperature, (temperature - 32) * 5 / 9 + 273);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " kelvin");
    }
    else if(index == 0 && temperatureType == 0)
    {
           SetTemperature(temperature, temperature);
           ui->TemperatureLabel->setText("Temperature: " + QString::number(temperature) + " kelvin");
    }

    SetTemperatureType(temperatureType,index);
}

void MainWindow::setColorButton_ON_OFF() //Changes the color of the "Turn_ON_OFF" button depending on the selected color theme
{
    QPalette pal = palette();
    if(ui->Turn_ON_OFF->text() == "Turn OFF" && theme == "Light")
    {
        ui->Turn_ON_OFF->setStyleSheet(QString("background-color: rgb(168, 180, 224)"));
    }
    else if(ui->Turn_ON_OFF->text() == "Turn OFF" && theme == "Dark")
    {
        ui->Turn_ON_OFF->setStyleSheet(QString("background-color: rgb(132, 143, 181)"));
    }
    else if(ui->Turn_ON_OFF->text() == "Turn ON" && theme == "Light")
    {
        ui->Turn_ON_OFF->setStyleSheet(QString("background-color: rgb(214, 66, 66)"));
    }
    else if(ui->Turn_ON_OFF->text() == "Turn ON" && theme == "Dark")
    {
        ui->Turn_ON_OFF->setStyleSheet(QString("background-color: rgb(189, 58, 58)"));
    }
}

bool MainWindow::event(QEvent *event) //Redefining "event" for successful working of the program on the touch screen
{
    if (event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchUpdate ||
        event->type() == QEvent::TouchEnd) {
        return true;
    }
    return QWidget::event(event);
}

bool MainWindow::touchEvent(QTouchEvent *event) //Redefining "touchEvent" for successful working of the program on the touch screen
{
    if (event->touchPointStates() & Qt::TouchPointPressed) {
    } else if (event->touchPointStates() & Qt::TouchPointMoved) {

    } else if (event->touchPointStates() & Qt::TouchPointReleased) {

    }
    return true;
}

