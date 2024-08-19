#include "com_port.h"
int main()
{
	comm_port::SerialPort<void> port(comm_port::e_rate_9600, 8, comm_port::e_no_parity, comm_port::e_ones_stopbit);
	port.comm_open("COM3", 100, 0);

	char word[] = "Hello world\r\n";
	port.comm_write(word, sizeof(word));

	printf("time0= %u\n", port.get_last_event_ms());
	port.comm_read(word, sizeof(word));
	printf("time1= %u\n", port.get_last_event_ms());

	port.comm_close();
	return 0;
}