
#include "ring_buffer.h"
typedef uint8_t Type;
int main()
{
	Type data[10] = { 0,1,2,3,4,5,6,7,8,9 };

	ring_buffer::RingBuffer<Type>* buf = new ring_buffer::RingBuffer<Type>;
	buf->init(5);
	buf->print_hex("init: ");

	buf->put(-1);
	buf->put(-2);
	buf->put(data, 2);
	buf->print_hex("add -1, -2, 0, 1: ");
	buf->put(data + 5, 1);
	buf->print_hex("add 5: ");

	Type a[5];
	buf->get(a); printf("get= %d\n", a[0]);
	buf->pop(a, 3); printf("pop= %d, %d ,%d\n", a[0], a[1], a[2]);

	buf->put(data + 6, 3);
	buf->print_hex("pop -1,-2,0. add 6,7,8: ");

	uint32_t n = buf->size(); printf("size= %d\n", n);
	try
	{
		buf->put(data, (5 - n) + 1);
	}
	catch (const ring_buffer::OverflowException& e)
	{
		std::cerr << e.what() << '\n';
	}

	delete buf;
	return 0;
}