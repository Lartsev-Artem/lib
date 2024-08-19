#ifndef RUNTIME_EVENTS_H
#define RUNTIME_EVENTS_H

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

//! @brief запустить функцию echo ввода с клавиатуры
inline void echo_keyboard()
{
	printf("press ESC to exit.\n");
	printf("press SPACE to clear screen.\n\n");
	bool is_run = true; char ch;
	do
	{		
		if (kbhit())
		{
			ch = getch();
			switch (ch)
			{
			case 13:
				printf("\r\n");
				break;
			case 27:
				is_run = false;
				break;
			case 32:
				system("cls");
				printf("press ESC to exit.\n");
				printf("press SPACE to clear screen.\n\n");
				break;
			default:
				printf("%c", ch);
				break;
			}
		}

	} while (is_run);
}


#endif //RUNTIME_EVENTS_H
