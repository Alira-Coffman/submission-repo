#include <avr/io.h>
#include <asf.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();

	/* Insert application code here, after the board has been initialized. */
	DDRC = 0x00;
	DDRB = 0xFF;
	while(1)
	{
		PORTB = PINC
	}
}
