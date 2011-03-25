#include "../../global.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include "rs232_port.h"


rs232_port::rs232_port(const char* pathPortFile, WORD reading_interval_dsec):
        m_WaitingInterval(reading_interval_dsec),
        m_PortDescryptor (0)

{
    strcpy(m_PathToPortFile, pathPortFile);

    if (verbose) fprintf(stderr, "set rs232 interface: port=%s\n", m_PathToPortFile);
}

//---------------------------------------------------------------------------------------//
rs232_port::~rs232_port(){
//    delete timer_ReadingInterval;
}

//---------------------------------------------------------------------------------------//
errType rs232_port::open_port(bool listen_mode)
{
    errType result=err_ok;
    if (m_PortDescryptor > 0) return result;    //for preventing of repeated opening of port

    //----------------------O P E N I N G   T H E   P O R T

    if (verbose) fprintf(stderr, "rs232 port %s has been opened\n\n",m_PathToPortFile);
    m_PortDescryptor = open(m_PathToPortFile, O_RDWR | O_NOCTTY/*| O_NONBLOCK*/);
    if (m_PortDescryptor == -1){
          fprintf(stderr, "ERROR: open_port: Unable to open %s (if you use Linux - try to use ttyS0 or ttyS1)\n", m_PathToPortFile);
          result = err_error;
          int status;
          exit(status);
      }
    else{
//        fcntl(m_PortDescryptor, F_SETFL, 0);
        fprintf(stderr, "Port %s is open\n", m_PathToPortFile);
      if (verbose) fprintf(stderr, "Descryptor = %d\n", m_PortDescryptor);
    }

    if(!isatty(m_PortDescryptor)){
        perror("File of terminal is not valid");
        return err_error;
    }


    //-----------------C O N F I G U R I N G    S E R I A L    P O R T
    /*
     * Получение текущих опций для порта...
     */
    tcgetattr(m_PortDescryptor, &m_options);
    /*
     * -------Разрешение приемника и установка локального режима...
     */
    m_options.c_cflag |= (CLOCAL | CREAD);

    /*
     * --------------ЛОКАЛЬНЫЕ ОПЦИИ
     */
    //
    // No line processing:
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    //
    m_options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG );
    /*
     * ------------------ОПЦИИ ВЫВОДА
     */
    //
    // Output flags - Turn off output processing
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    //
//     m_options.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR /*| ONOEOT|*/ OFILL | OPOST);
    m_options.c_oflag = 0;
    /*
     * ------------------ОПЦИИ ВВОДА
     */

    // Input flags - Turn off input processing
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    //
    m_options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                     | INLCR | IGNCR | ICRNL | IXON);
    /*
     * Установка аппаратного управления потоком передаваемых данных
     */
//    m_options.c_cflag |= ~(CRTSCTS);
    /*
     * ------------------Установка таймаутов чтения
     */
    m_options.c_cc[VMIN]  = 0;
    m_options.c_cc[VTIME] = m_WaitingInterval;
    /*
     * Установка новых опций для порта...
     */

    if (tcsetattr(m_PortDescryptor, TCSAFLUSH, &m_options) < 0){
        perror("Setting new configuration of com-port");
    }

    return result;
}

//---------------------------------------------------------------------------------------//
errType rs232_port::flush()
{
    errType result=err_ok;
    if (tcsetattr(m_PortDescryptor, TCSAFLUSH, &m_options) < 0){
        perror("Setting new configuration of com-port");
        result = err_error;
    }
}

//---------------------------------------------------------------------------------------//
errType rs232_port::close_port()
{
    errType result=err_ok;

    if (close(m_PortDescryptor) != -1) if (verbose) fprintf(stderr, "rs232 port %s has been closed\n\n",m_PathToPortFile);
    else {
        result = err_error;
//        fprintf(stderr, "ERROR: close com-port %s\n", m_PathToPortFile);
        
    }
    return result;
}
//********************************************************************************
//**********************c o n f i g u r i n g     m e t h o d s*******************
//********************************************************************************
//---------------------------------------------------------------------------------------//
void rs232_port::setReadInterval(WORD dsec)
{
    m_WaitingInterval = dsec;
    if (verbose) fprintf(stderr, "Setting reading interval for rs232: %d dsec\n", m_WaitingInterval);
    m_options.c_cc[VTIME] = m_WaitingInterval;
    if (tcsetattr(m_PortDescryptor, TCSAFLUSH, &m_options) < 0){
        perror("Setting new reading interval of com-port");
    }
}

//---------------------------------------------------------------------------------------//
void rs232_port::setupConfiguration(Baudrate_type indexOfBaudRate, Databits_type DataBits, Stopbits_type StopBits, Parity_type stateOfChecking)
{
    fprintf(stderr, "setupConfiguration:: indexOfBaudRate = %d, DataBits = %d, StopBits %d, stateOfChecking = %d\n", indexOfBaudRate, DataBits, StopBits, stateOfChecking);
    setBaudRate(indexOfBaudRate);
    setQuantityOfDataBits(DataBits);
    setQuantityOfStopBits(StopBits);
    setParityCheck(stateOfChecking);

    //applying configuration
    if (tcsetattr(m_PortDescryptor, TCSAFLUSH, &m_options) < 0){
        perror("Setting new configuration of com-port");
    }
    fprintf(stderr, "Baudrate of line-in: %d\n", cfgetispeed(&m_options));
    fprintf(stderr, "Baudrate of line-out: %d\n", cfgetospeed(&m_options));
    fprintf(stderr, "Configuring serial port is completed\n");
}

//---------------------------------------------------------------------------------------//
void rs232_port::setBaudRate(Baudrate_type indexOfBaudRate)
{
    switch (indexOfBaudRate){
        case BAUD2400:
            cfsetispeed(&m_options, B2400);
            cfsetospeed(&m_options, B2400);
        break;
        case BAUD4800:
            cfsetispeed(&m_options, B4800);
            cfsetospeed(&m_options, B4800);
        break;
        case BAUD9600:
            cfsetispeed(&m_options, B9600);
            cfsetospeed(&m_options, B9600);
        break;
        case BAUD38400:
            cfsetispeed(&m_options, B38400);
            cfsetospeed(&m_options, B38400);
        break;
        case BAUD57600:
            cfsetispeed(&m_options, B57600);
            cfsetospeed(&m_options, B57600);
        break;
        case BAUD115200:
            cfsetispeed(&m_options, B115200);
            cfsetospeed(&m_options, B115200);
        break;
        default: fprintf(stderr, "Error of configuring com-port: value of baudrate is not valid!\n");
    }
}

//---------------------------------------------------------------------------------------//
void rs232_port::setQuantityOfDataBits(Databits_type Quantity)
{
    m_options.c_cflag &= ~(CSIZE | PARENB); /* Маскирование битов размера символов */
    switch (Quantity){
    case DB5:
        m_options.c_cflag |= CS5;
        break;
    case DB6:
        m_options.c_cflag |= CS6;
        break;
    case DB7:
        m_options.c_cflag |= CS7;
        break;
    case DB8:
        m_options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Error of configuring of com-port: quantity of data bits is out the range of valid values: %d\n\n", Quantity);
    }
}

//---------------------------------------------------------------------------------------//
void rs232_port::setQuantityOfStopBits(Stopbits_type Quantity)
{
    //there "0" means 1 stop-bit and "1" means 2 stop-bits
    switch (Quantity){
    case SB1:
        m_options.c_cflag &= ~CSTOPB;
        break;
    case SB2:
        m_options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr, "Error of configuring of com-port: quantity of stop bits is out the range of valid values  - it is (%d) (0 - is one bit; 1 - is two)", Quantity);
    }
}

//---------------------------------------------------------------------------------------//
void rs232_port::setParityCheck(Parity_type stateOfChecking)
{
    switch (stateOfChecking){
    case NO_PARITY:     // Отсутствие проверки на четность (8N1)
        m_options.c_cflag &= ~PARENB;
        //        m_options.c_cflag &= ~CSTOPB;
        //        m_options.c_cflag &= ~CSIZE;
        //        m_options.c_cflag |= CS8;
        //        m_options.c_iflag |= ~ISTRIP; //with this works incorrectly
        break;
    case ODD_PARITY:// Проверка на нечетность (7O1):
        m_options.c_cflag |= PARENB;
        m_options.c_cflag |= PARODD;
        //                m_options.c_cflag &= ~CSTOPB;
        //                m_options.c_cflag &= ~CSIZE;
        //                m_options.c_cflag |= CS7;
        //        m_options.c_iflag |= INPCK;
        break;
    case EVEN_PARITY:// Проверка на четность (7E1):
        m_options.c_cflag |= PARENB;
        m_options.c_cflag &= ~PARODD;
        //                m_options.c_cflag &= ~CSTOPB;
        //                m_options.c_cflag &= ~CSIZE;
        //                m_options.c_cflag |= CS7;
        //         m_options.c_iflag |= INPCK;
        break;
    default:
        fprintf(stderr, "Error of opening com-port: parity check is out the range of valid values\n");
        break;
    }
}

//**************************************************************************************
//**********************d a t a - t r a n s p o r t     m e t h o d s*******************
//**************************************************************************************
//---------------------------------------------------------------------------------------//
errType rs232_port::sendByte(BYTE in)
{
    errType result=err_ok;
    return result;
}

//---------------------------------------------------------------------------------------//
errType rs232_port::readByte(BYTE* out)
{
    errType result=err_ok;
    int n = read(m_PortDescryptor, out, 1);
    if (n == 0){
        result = err_port_is_empty;
    }
    if (n == -1) {
        if ((errno == EINTR) or (errno == EAGAIN)) result = err_port_is_empty;
                else {
                    fprintf(stderr, "ERROR: reading byte from port %s, errno =%d\n", m_PathToPortFile, errno);
                    perror("Read failed");
                    if (errno == 84){
                        fprintf(stderr, "Данный COM-порт уже занят или не существует. Измените входной com-порт в ПС Конфигурирования.\n");
                        exit(errno);
                        fprintf(stderr, "%f\n", 5/0);
                    }
                    result = err_error;
                }
            }
//    if (n > 0) qDebug("Readed %d bytes", n);
    return result;
}

////this is a variant for send data by single bytes with intervals
//errType rs232_port::sendData(BYTE* data, size_t len)
//{
//  if (verbose) qDebug("->>->>->>->>->>rs232_port::sendData");
//    errType result = err_ok;
//    size_t total_sended = 0;
//    do{
//        QCoreApplication::processEvents();
//        ssize_t sended = 0;
//        if (!timer_ReadingInterval->isActive()) {
//            sended = write(m_PortDescryptor, data+total_sended, 1);
//            timer_ReadingInterval->start();
//        }
//        else {
//            sended = 0;
//        }
//        if (sended == -1)
//        {
//            qDebug("\nERROR: sending data via rs232_port");
//          if (verbose) qDebug("Port = %s, len = %d", m_PathToPortFile, len);
//            result = err_error;
//        }
//        total_sended += sended;
//    }while((total_sended < len)&&(result != err_error));
//
//    if ( (result == err_ok)&&(verbose) ){
//        fprintf(stderr, "send packet: [");
//        for (size_t i = 0; i < len; i++) fprintf(stderr, "%.2X ", data[i]);
//        fprintf(stderr, "]\n");
//    }
//
//  if (verbose) qDebug("<<-<<-<<-<<-<<-rs232_port::sendData");
//  if (verbose) qDebug("");
//    return result;
//}


//---------------------------------------------------------------------------------------//
//this is a variant for send data by whole block
errType rs232_port::sendData(BYTE* data, size_t len)
{
  if (verbose) fprintf(stderr, "->>->>->>->>->>rs232_port::sendData\n");
    errType result = err_ok;
    size_t total_sended = 0;
    do{
        ssize_t sended = 0;
        sended = write(m_PortDescryptor, data+total_sended, len-total_sended);
        if (sended == -1)
        {
            fprintf(stderr, "\nERROR: sending data via rs232_port\n");
          if (verbose) fprintf(stderr, "Port = %s, len = %d\n", m_PathToPortFile, len);
            result = err_error;
        }
        total_sended += sended;
    }while((total_sended < len)&&(result != err_error));

    if ( (result == err_ok)&&(verbose) ){
        fprintf(stderr, "send packet: [");
        for (size_t i = 0; i < len; i++) fprintf(stderr, "%.2X ", data[i]);
        fprintf(stderr, "]\n");
    }

  if (verbose) fprintf(stderr, "<<-<<-<<-<<-<<-rs232_port::sendData\n");
  if (verbose) fprintf(stderr, "\n");
    return result;
}


//---------------------------------------------------------------------------------------//
//this method returning data and it's length, which have been read from com-port if
//timer with interval is not finished. If there nothing to read and interval has finished,
//function returnes err_port_is_empty
errType rs232_port::readData(BYTE* data, size_t& lengthOfReceived)
{
  if (verbose) fprintf(stderr, "->->->->->->rs232_port::readData\n");
    errType result=err_ok;
    WORD received = 0;
    do{
       result = readByte(data+received);
       if (result == err_ok) received++;
    }while (result == err_ok);
    if ((received)&&(result == err_port_is_empty)){
        result = err_ok;
        lengthOfReceived = received;
        if (verbose){
            fprintf(stderr, "%d bytes has been recieved: [", received);
            for (size_t i = 0; i < received; i++) fprintf(stderr, "%.2X ", data[i]);
            fprintf(stderr, "]\n");
        }
    }
    if (result == err_port_is_empty){
        if (verbose) fprintf(stderr, "COM port is empty\n");
    }

  if (verbose) fprintf(stderr, "<-<-<-<-<-<-rs232_port::readData\n");
  if (verbose) fprintf(stderr, "\n");
    return result;
}
//****************************************************************************
//**********************a u x i l i a r y     m e t h o d s*******************
//****************************************************************************
//---------------------------------------------------------------------------------------//
BYTE rs232_port::codeBaudRate(DWORD BaudRate)
{
    switch (BaudRate){
    case 2400:
        return 0;
        break;
    case 4800:
        return 1;
        break;
    case 9600:
        return 2;
        break;
    case 38400:
        return 3;
        break;
    case 57600:
        return 4;
        break;
    case 115200:
        return 5;
        break;
    default: fprintf(stderr, "Error: unavailible value of baudrate!\n");
    }
    return 0;
}

//---------------------------------------------------------------------------------------//
DWORD rs232_port::decodeBaudRate(BYTE CodeOfBaudrate)
{
    switch (CodeOfBaudrate){
    case 0:
        return 2400;
        break;
    case 1:
        return 4800;
        break;
    case 2:
        return 9600;
        break;
    case 3:
        return 38400;
        break;
    case 4:
        return 57600;
        break;
    case 5:
        return 115200;
        break;
    default: fprintf(stderr, "Error: unavailible code of baudrate!\n");
    }
    return 0;
}
