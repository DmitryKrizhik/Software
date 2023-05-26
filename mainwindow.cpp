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

/**
 * @brief Объект для работы с TCP-сокетом
 */
QTcpSocket MainWindow::socket;

/**
 * @brief Строковая переменная для хранения состояния системы
 */
QString state;

/**
 * @brief Строковая переменная для хранения единиц измерения температуры
 */
QString degrees;

/**
 * @brief Строковая переменная для хранения единиц измерения давления
 */
QString pressureUnit;

/**
 * @brief Строковая переменная для хранения текущей темы интерфейса
 */
QString theme;

/**
 * @brief Переменная для хранения типа давления (атмосферное или абсолютное)
 */
int MainWindow::pressureType;

/**
 * @brief Переменная для хранения типа температуры (Цельсий или Фаренгейт)
 */
int MainWindow::temperatureType;

/**
 * @brief Переменная для хранения значения температуры
 */
float temperature;

/**
 * @brief Переменная для хранения значения угла поворота
 */
int angle;

/**
 * @brief Переменная для хранения значения влажности
 */
int humidity;

/**
 * @brief Переменная для хранения значения давления
 */
int pressure;

/**
 * @brief Функция для отправки запроса на сервер каждые пять секунд
 */
void SendRequestEveryFiveSec();

void SetDegrees(QString &degrees, QString _degrees)
{
    /**
     * @brief Функция для установки значения температуры в градусах
     * @param degrees ссылка на строковую переменную для хранения единиц измерения температуры
     * @param _degrees новое значение температуры в градусах
     */
    degrees = _degrees;
}
void SetPressureUnit(QString &pressureUnit, QString _pressureUnit)
{
    /**
     * @brief Функция для установки единицы измерения давления
     * @param pressureUnit ссылка на строковую переменную для хранения единиц измерения давления
     * @param _pressureUnit новое значение единицы измерения давления
     */
    pressureUnit = _pressureUnit;
}
void SetTemperature(float &temperature, float _temperature)
{
    /**
     * @brief Функция для установки значения температуры
     * @param temperature ссылка на переменную для хранения значения температуры
     * @param _temperature новое значение температуры
     */
    temperature = _temperature;
}
void SetAngle(int &angle, int _angle)
{
    /**
     * @brief Функция для установки значения угла поворота
     * @param angle ссылка на переменную для хранения значения угла поворота
     * @param _angle новое значение угла поворота
     */
    angle = _angle;
}
void SetHumidity(int &humidity, int _humidity)
{
    /**
     * @brief Функция для установки значения влажности
     * @param humidity ссылка на переменную для хранения значения влажности
     * @param _humidity новое значение влажности
     */
    humidity = _humidity;
}
void SetPressure(int &pressure, int _pressure)
{
    /**
     * @brief Функция для установки значения давления
     * @param pressure ссылка на переменную для хранения значения давления
     * @param _pressure новое значение давления
     */
    pressure = _pressure;
}
void SetState(QString &state, QString _state)
{
    /**
     * @brief Функция для установки состояния системы
     * @param state ссылка на строковую переменную для хранения состояния системы
     * @param _state новое значение состояния системы
     */
    state = _state;
}
void SetTheme(QString &theme, QString _theme)
{
    /**
     * @brief Функция для установки текущей темы интерфейса
     * @param theme ссылка на строковую переменную для хранения текущей темы интерфейса
     * @param _theme новое значение темы интерфейса
     */
    theme = _theme;
}
void MainWindow::SetPressureType(int &pressureType, int _pressureType)
{
    /**
     * @brief Функция для установки типа давления (атмосферное или абсолютное)
     * @param pressureType ссылка на переменную для хранения единиц измерения давления
     * @param _pressureType новое значение типа давления
     */
    pressureType = _pressureType;
}
void MainWindow::SetTemperatureType(int &temperatureType, int _temperatureType)
{
    /**
     * @brief Функция для установки типа температуры (Цельсий или Фаренгейт)
     * @param temperatureType ссылка на переменную для хранения единиц измерения температуры
     * @param _temperatureType новое значение типа температуры
     */
    temperatureType = _temperatureType;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLight() //Changes the color theme of the interface to light
{
    /**
     * @brief Функция для установки светлой темы интерфейса
     * Изменяет цветовую схему интерфейса на светлую
     * @see SetTheme(), setColorButton_ON_OFF()
     */
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
    /**
     * @brief Функция для установки темной темы интерфейса
     * Изменяет цветовую схему интерфейса на темную
     * @see SetTheme(), setColorButton_ON_OFF()
     */
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

    /**
     * @brief Функция для включения кнопок интерфейса
     * Включает все кнопки интерфейса, которые были выключены ранее.
     */
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
    /**
     * @brief Функция для выключения кнопок интерфейса
     * Выключает все кнопки интерфейса.
     */
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
    /**
     * @brief Включает приложение и изменяет цвета в зависимости от выбранной темы.
     * Функция включает все кнопки интерфейса, изменяет текст на кнопке "Turn ON/OFF" на "Turn OFF"
     * и изменяет цвета текста и фона в соответствии с выбранной темой.
     * @see setColorButton_ON_OFF()
     */
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
}

void MainWindow::TurnOFF() //OFF the application and changes colors depending on the selected color theme
{
    /**
     * @brief Выключает приложение и изменяет цвета в зависимости от выбранной темы.
     * Функция выключает все кнопки интерфейса, изменяет текст на кнопке "Turn ON/OFF" на "Turn ON"
     * и изменяет цвета текста и фона в соответствии с выбранной темой.
     * @see setColorButton_ON_OFF()
     */
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

    /**
     * @brief Обрабатывает нажатие кнопки "Turn ON/OFF".
     * Функция изменяет цвета текста и фона в соответствии с выбранной темой.
     * Если на кнопке написано "Turn ON", то вызывается функция TurnON(), а если "Turn OFF", то вызывается функция TurnOFF().
     * @see setTheme(), TurnON(), TurnOFF()
     */
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
    /**
     * @brief Обрабатывает выбор пользователем разрешения экрана из выпадающего списка.
     * Функция изменяет размер окна приложения в соответствии с выбранным разрешением.
     * @param index Индекс выбранного элемента в выпадающем списке.
     */
       if(index == 0) { setFixedSize(800, 600); }
       else if(index ==1) { setFixedSize(1024,768); }
}

void MainWindow::on_ThemeBox_activated(int index) //Handles the color theme selection box
{
    /**
     * @brief Обрабатывает выбор пользователем цветовой темы из выпадающего списка.
     * Функция изменяет цветовую схему приложения в соответствии с выбранной темой.
     * @param index Индекс выбранного элемента в выпадающем списке.
     */
    if(index == 0) { setLight(); }
    else if(index == 1) { setDark(); }
}

void MainWindow::on_SendRequest_button_clicked() //Sends a request to the Simulator
{   
    /**
     * @brief Отправляет запрос на симулятор.
     * Функция вызывает метод SendRequest() для отправки запроса на симулятор.
     * @note Для работы функции необходимо, чтобы было установлено соединение с симулятором.
     * @see SendRequest()
     */
    SendRequest();
}

void MainWindow::startSendingRequests() //Sends requests to the Simulator every five seconds
{
    /**
     * @brief Начинает отправку запросов на симулятор каждые пять секунд.
     * Функция создает таймер, который каждые пять секунд вызывает метод SendRequest().
     * @note Для работы функции необходимо, чтобы было установлено соединение с симулятором.
     * @see SendRequest()
     */
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::SendRequest);
    timer->start(5000);
}

void MainWindow::on_Simulating_Start_Stop_clicked() //Handles pressing the button "Simulating_Start_Stop". ON/OFF the Simulator's interface
{
    /**
     * @brief Обрабатывает нажатие кнопки "Simulating_Start_Stop", включая/выключая интерфейс симулятора.
     * Функция устанавливает соединение с симулятором и отправляет ему команду "ON" или "OFF" в зависимости от текущего состояния кнопки.
     * Если соединение установлено успешно, функция меняет текст на кнопке на "Stop Simulating" или "Start Simulating" соответственно.
     * @note Для работы функции необходимо, чтобы симулятор был доступен по адресу 127.0.0.1:8080.
     */
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
    /**
     * @brief Отправляет запрос к симулятору и получает в ответ актуальные значения параметров кондиционера.
     * Функция устанавливает соединение с симулятором, отправляет ему команду "get_data" и получает в ответ строку, содержащую значения параметров.
     * Затем функция обрабатывает полученную строку и устанавливает соответствующие значения на интерфейсе программы.
     * @note Для работы функции необходимо, чтобы симулятор был доступен по адресу 127.0.0.1:8080.
     */
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
    /**
     * @brief Обрабатывает событие выбора единицы измерения давления в PressureBox.
     * Функция получает индекс выбранной единицы измерения и текущее значение давления. Затем, в зависимости от выбранной единицы,
     * функция пересчитывает значение давления и показывает её на интерфейсе программы.
     * @param index Индекс выбранной единицы измерения в PressureBox.
     * @see SetPressure(), SetPressureType()
     */
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
    /**
     * @brief Обрабатывает событие выбора единицы измерения температуры в TemperatureBox.
     * Функция получает индекс выбранной единицы измерения и текущее значение температуры. Затем, в зависимости от выбранной единицы,
     * функция пересчитывает значение температуры и показывает её на интерфейсе программы.
     * @param index Индекс выбранной единицы измерения в TemperatureBox.
     * @see SetTemperature(), SetTemperatureType()
     */
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
    /**
     * @brief Меняет цвет кнопки "Turn_ON_OFF" в зависимости от выбранной цветовой темы.
     * Функция получает текущую цветовую тему и текст на кнопке "Turn_ON_OFF" и
     * устанавливает соответствующий цвет фона для кнопки "Turn_ON_OFF".
     */
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
    /**
     * @brief Переопределение метода "event" для успешной работы программы на сенсорном экране.
     * Функция перехватывает события касания экрана и возвращает true.
     * @param event Указатель на объект события QEvent.
     * @return true, если событие было обработано, false - если нет.
     */
    if (event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchUpdate ||
        event->type() == QEvent::TouchEnd) {
        return true;
    }
    return QWidget::event(event);
}

bool MainWindow::touchEvent(QTouchEvent *event) //Redefining "touchEvent" for successful working of the program on the touch screen
{
    /**
     * @brief Переопределение метода "touchEvent" для успешной работы программы на сенсорном экране.
     * Функция обрабатывает события касания экрана и возвращает true.
     * @param event Указатель на объект события QTouchEvent.
     * @return true, если событие было обработано, false - если нет.
     */
    if (event->touchPointStates() & Qt::TouchPointPressed) {
    } else if (event->touchPointStates() & Qt::TouchPointMoved) {

    } else if (event->touchPointStates() & Qt::TouchPointReleased) {
    }
    return true;
}
