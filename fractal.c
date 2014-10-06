#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>


#define MAX_ITERATIONS 1024
#define ESCAPE_RADIUS  4.02

const char* letters = " .`-_':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q";

unsigned int mandelbrot_set(double x0, double y0);


int main(void)
{
	double x, y, x0, y0, cam_x, cam_y, zoom;
	unsigned int iterations, pressed_key;
	const unsigned int letters_length = strlen(letters);

	cam_x = cam_y = 0.0;
	zoom = 10.0;

	initscr();
	noecho();
	keypad(stdscr, true);

	do {
		for (int y = 0; y < LINES; ++y) {
			y0 = (((LINES / 2) - y) / zoom) + cam_y;

			for (x = 0; x < COLS; ++x) {
				x0 = ((x - (COLS / 2)) / zoom) / 2 + cam_x;
				iterations = mandelbrot_set(x0, y0);

				mvaddch(y, x, (unsigned long)letters[iterations % letters_length] | A_BOLD);
			}
		}

		refresh();
		pressed_key = getch();

		switch(pressed_key) {
			case 'w': zoom *= 1.2; break;
			case 's': zoom /= 1.2; break;
			case KEY_UP:    cam_y += 1 / zoom; break;
			case KEY_DOWN:  cam_y -= 1 / zoom; break;
			case KEY_LEFT:  cam_x -= 1 / zoom; break;
			case KEY_RIGHT: cam_x += 1 / zoom; break;
		}

	} while (pressed_key != 'q');


	endwin();
	exit(0);
}


unsigned int mandelbrot_set(double x0, double y0)
{
	double x, y, x2, y2, new_x, new_y;

	x = x0;
	y = y0;

	for (unsigned int i = 0; i < MAX_ITERATIONS; ++i) {
		x2 = x * x;
		y2 = y * y;

		if (ESCAPE_RADIUS < x2 + y2) {
			return i;
		}

		new_x = x2 - y2 + x0;
		new_y = 2.0 * x * y + y0;

		x = new_x;
		y = new_y;
	}

	return 0;
}
