#include "fractal.h"


void* render_image(void* fractal_params)
{
	int x, y;
	double x0, y0, cam_x, cam_y, zoom;
	unsigned int iterations;

	const unsigned int gamma_length = strlen(GAMMA_ASCII);

	if (has_colors()) {
		start_color();
		init_color_pairs();
	}

	while (true) {
		cam_x = ((fractal_params_t*)fractal_params)->cam_x;
		cam_y = ((fractal_params_t*)fractal_params)->cam_y;
		zoom  = ((fractal_params_t*)fractal_params)->zoom;

		for (y = 0; y < LINES; ++y) {
			y0 = (((LINES / 2) - y) / zoom) + cam_y;

			for (x = 0; x < COLS; ++x) {
				x0 = ((x - (COLS / 2)) / zoom) / 2 + cam_x;
				iterations = mandelbrot_set(x0, y0);

				print_char(x, y, (unsigned long)GAMMA_ASCII[iterations % gamma_length] | A_BOLD,
					(int)(((iterations * 128) / MAX_ITERATIONS) % NUMBER_OF_COLORS + 1));
			}
		}

		refresh();
	}

	return NULL;
}


void* check_input(void* fractal_params)
{
	unsigned int pressed_key;

	do {
		pressed_key = getch();

		switch(pressed_key) {
			case 'w': ((fractal_params_t*)fractal_params)->zoom *= 1.2; break;
			case 's': ((fractal_params_t*)fractal_params)->zoom /= 1.2; break;
			case KEY_UP:    ((fractal_params_t*)fractal_params)->cam_y += 1 / ((fractal_params_t*)fractal_params)->zoom; break;
			case KEY_DOWN:  ((fractal_params_t*)fractal_params)->cam_y -= 1 / ((fractal_params_t*)fractal_params)->zoom; break;
			case KEY_LEFT:  ((fractal_params_t*)fractal_params)->cam_x -= 1 / ((fractal_params_t*)fractal_params)->zoom; break;
			case KEY_RIGHT: ((fractal_params_t*)fractal_params)->cam_x += 1 / ((fractal_params_t*)fractal_params)->zoom; break;
		}
	} while (pressed_key != 'q');

	return NULL;
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


void init_color_pairs()
{
	init_pair(1, COLOR_WHITE,   COLOR_BLACK);
	init_pair(2, COLOR_BLUE,    COLOR_BLACK);
	init_pair(3, COLOR_CYAN,    COLOR_BLACK);
	init_pair(4, COLOR_GREEN,   COLOR_BLACK);
	init_pair(5, COLOR_YELLOW,  COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_RED,     COLOR_BLACK);
}


void print_char(int x, int y, unsigned long char_param, int color_pair)
{
	if (has_colors()) {
		attron(COLOR_PAIR(color_pair));
	}
	
	mvaddch(y, x, char_param);
}
