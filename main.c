#include "fractal.h"


int main(void)
{
	unsigned int pressed_key;
	fractal_params_t params;
	pthread_t render_thread_id;

	params.cam_x = 0.0;
	params.cam_y = 0.0;
	params.zoom  = 10.0;

	initscr();
	noecho();
	keypad(stdscr, true);

	pthread_create(&render_thread_id, NULL, &render_image, &params);

	do {
		pressed_key = getch();

		switch(pressed_key) {
			case 'w': params.zoom *= 1.2; break;
			case 's': params.zoom /= 1.2; break;
			case KEY_UP:    params.cam_y += 1 / params.zoom; break;
			case KEY_DOWN:  params.cam_y -= 1 / params.zoom; break;
			case KEY_LEFT:  params.cam_x -= 1 / params.zoom; break;
			case KEY_RIGHT: params.cam_x += 1 / params.zoom; break;
		}

	} while (pressed_key != 'q');


	endwin();
	clear();
	exit(0);
}