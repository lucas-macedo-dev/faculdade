#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>
HANDLE serial_abre(int porta, const long int BaudRate,
                   const char parity, const char data);
void serial_limpa(const int porta);
void serial_fecha(const int porta);
char serial_recebe(const int porta);
void serial_envia(const int porta, const int txchar);
void delay_ms(clock_t millis);
HANDLE hCom;
int main()
{
    clock_t timeout;
    unsigned char r, e, tecla;
    int porta, taxa;
    setlocale(LC_ALL, "Portuguese");
    printf(" MONITOR SERIAL \n\n");
    printf(" Número da porta serial: ");
    scanf("%d", &porta);
    printf(" Taxa de transmissão...: ");
    scanf("%d", &taxa);
    if (!serial_abre(porta, taxa, ‘8’, ‘N’))
    {
        getch();
        system("cls");
        exit(1);
    }
    timeout = clock() + 60000;
    printf("\n COMANDOS\n\n");
    printf(" [A].....Acende LED1 [B].....Apaga LED1\n");
    printf(" [C].....Acende LED2 [D].....Apaga LED2\n\n");
    while (1)
    {
        r = serial_recebe(porta);
        if (r)
            printf(" Recebido: %c\r", r);
        if (kbhit())
        {
            tecla = getch();
            if (tecla ==’*’)
            {
                serial_fecha(porta);
                exit(0);
            }
            printf(" Enviado : %c\r", tecla);
            serial_limpa(porta);
            serial_envia(porta, tecla);
            serial_limpa(porta);
        }
    }
}

/*********************************************************
 Manipulador para inicialização da porta serial (abertura)
**********************************************************/

HANDLE serial_abre(int porta, const long int BaudRate,
                   const char parity, const char data)
{
    BOOL bPortReady;
    DCB dcb;
    COMMTIMEOUTS CommTimeouts;
    char ComPortName[5];
    if (porta < 10)
    {
        ComPortName[0] =’C’;
        ComPortName[1] =’O’;
        ComPortName[2] =’M’;
        ComPortName[3] =’0’ + porta;
    }
    else
    {
        if (porta < 20)
        {
            ComPortName[0] =’C’;
            ComPortName[1] =’O’;
            ComPortName[2] =’M’;
            ComPortName[3] =’1’;
            ComPortName[4] =’0’ + porta - 10;
        }
        else
        {
            if (porta < 30)
            {
                ComPortName[0] =’C’;
                ComPortName[1] =’O’;
                ComPortName[2] =’M’;
                ComPortName[3] =’2’;
                ComPortName[4] =’0’ + porta - 20;
            }
        }
    }
    printf(" Abrindo porta %s\n", ComPortName);
    hCom = CreateFile(ComPortName, GENERIC_READ | GENERIC_WRITE,
                      0, NULL, OPEN_EXISTING, 0, NULL);
    if ((int)hCom <= 0)
    {
        printf("Falha");
        return 0;
    }
    bPortReady = SetupComm(hCom, 128, 128);
    if (!bPortReady)
    {
        printf("Falha");
        return 0;
    }
    bPortReady = GetCommState(hCom, &dcb);
    if (!bPortReady)
    {
        printf("Falha");
        return 0;
    }
    dcb.BaudRate = BaudRate;
    if (data == ‘7’)
        dcb.ByteSize = 7;
    else
        dcb.ByteSize = 8;
    if (parity == ‘E’)
        dcb.Parity = EVENPARITY;
    if (parity == ‘O’)
        dcb.Parity = ODDPARITY;
    else
        dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fAbortOnError = TRUE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fRtsControl = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    bPortReady = SetCommState(hCom, &dcb);
    if (!bPortReady)
    {
        printf("Falha");
        return 0;
    }
    bPortReady = GetCommTimeouts(hCom, &CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 5;
    CommTimeouts.ReadTotalTimeoutConstant = 5;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1;
    CommTimeouts.WriteTotalTimeoutConstant = 5;
    CommTimeouts.WriteTotalTimeoutMultiplier = 1;
    bPortReady = SetCommTimeouts(hCom, &CommTimeouts);
    if (!bPortReady)
    {
        printf("Falha");
        return 0;
    }
    printf(" COM%d conectada.\n", porta);
    return (hCom);
}

/*******************************************
 Funções para manipulação da porta serial
********************************************/
void serial_fecha(const int porta) { CloseHandle(hCom); }
char serial_recebe(const int porta)
{
    char rxchar;
    BOOL bReadRC;
    static DWORD iBytesRead;
    bReadRC = ReadFile(hCom, &rxchar, 1, &iBytesRead, NULL);
    if (iBytesRead)
    {
        return rxchar;
    }
    else
    {
        return 0;
    }
}
void serial_limpa(const int porta)
{
    while (serial_recebe(porta) != 0)
        ;
}
void serial_envia(const int porta, const int txchar)
{
    BOOL bWriteRC;
    static DWORD iBytesWritten;
    bWriteRC = WriteFile(hCom, &txchar, 1, &iBytesWritten, NULL);
    return;
}
void delay_ms(clock_t millis)
{
    clock_t endtime;
    endtime = millis + clock();
    while (endtime > clock())
    {
        ;
    }
}