#include "com_port.h"
#include <conio.h>

int main(int argc, char** argv)
{
    const char* port_name = argc > 1 ? argv[1] : "COM1";

	comm_port::SerialPort<void> port(comm_port::e_rate_9600, 8, comm_port::e_no_parity, comm_port::e_ones_stopbit);    
    port.comm_open(port_name, 100, 0);

    char send[64];
    sprintf(send,"Hello world from %s\r\n",port_name);
    port.comm_write(send, sizeof(send));

    char rcv[64];
	printf("time0= %u\n", port.get_last_event_ms());
    printf("Listening %s...\n", port_name);
    do
    {
        if(port.comm_read(rcv, sizeof(rcv)))
        {
            printf("rcv from: %s\n",rcv);
            break;
        }
        if (kbhit())
        {
            char ch = getch();
            if (ch == 27) //esc
            {
                break;
            }
        }
    }while(true);
	printf("time1= %u\n", port.get_last_event_ms());

	port.comm_close();
	return 0;
}
