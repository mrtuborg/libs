#ifndef RS232_PORT_H
#define RS232_PORT_H

#include "../comm_interface.h"
#include "../../my_types.h"
#include <termios.h>

//this enums need for accordance between values on form and factual parameters of com-ports
enum Baudrate_type
{
    BAUD2400   = 0,
    BAUD4800   = 1,
    BAUD9600   = 2,
    BAUD38400  = 3,
    BAUD57600  = 4,
    BAUD115200 = 5,
};

enum Databits_type
{
    DB5 = 0,
    DB6 = 1,
    DB7 = 2,
    DB8 = 3
};

enum Stopbits_type
{
    SB1 = 0,
    SB2 = 1,
};

enum Parity_type
{
    NO_PARITY   = 0,
    ODD_PARITY  = 1,
    EVEN_PARITY = 2
};


/**************************class*************************/
class rs232_port: public comm_interface
{
    char   m_PathToPortFile[20];
    struct termios m_options;
    WORD   m_WaitingInterval;   // time of waiting for data in port, in 0,1 sec
    int    m_PortDescryptor;   /* Файловый дескриптор для порта */

private:
    BYTE  codeBaudRate(DWORD BaudRate);
    DWORD decodeBaudRate(BYTE Code);


public:
    rs232_port(const char* pathPortFile, WORD reading_interval_dsec);
    ~rs232_port();

    errType open_port(bool listen_mode=false);
    errType close_port();
    errType flush(void);
    void    setupConfiguration(Baudrate_type indexOfBaudRate, Databits_type DataBits, Stopbits_type StopBits, Parity_type stateOfChecking);

    errType sendByte(BYTE in);
    errType readByte(BYTE* out);
    errType sendData(BYTE* data, size_t len);
    errType readData(BYTE* data, size_t& lengthOfReceived);

    void setReadInterval(WORD dsec);
    WORD getReadInterval() {  return m_WaitingInterval;  }
    int  getFD() {  return m_PortDescryptor;  }
    connection_type getInterfaceType() {return RS232;}

 private:
    void setBaudRate(Baudrate_type indexOfBaudRate);
    void setQuantityOfDataBits(Databits_type Quantity);
    void setQuantityOfStopBits(Stopbits_type Quantity);
    void setParityCheck(Parity_type stateOfChecking);
};

#endif // RS232_PORT_H
