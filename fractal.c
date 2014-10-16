#include "fractal.h"


extern pthread_mutex_t mutex;


void* render_image(void* fractal_params)
{
	bool has_color;
	int x, y;
	double x0, y0, cam_x, cam_y, zoom, const_real, const_imag;
	unsigned int iterations, color_pair, last_color_pair;
	unsigned int (*function)(double, double, double, double);
	
	const unsigned int gamma_length = strlen(GAMMA_ASCII);

	last_color_pair = 1;

	if (has_colors()) {
		start_color();
		init_color_pairs();
	}

	attron(A_BOLD);

	while (true) {
		if (!fractal_params) {
			return (void*)EFAULT;
		}

		cam_x = ((fractal_params_t*)fractal_params)->cam_x;
		cam_y = ((fractal_params_t*)fractal_params)->cam_y;
		zoom  = ((fractal_params_t*)fractal_params)->zoom;
		has_color = ((fractal_params_t*)fractal_params)->has_color;
		function = ((fractal_params_t*)fractal_params)->function;
		const_real = ((fractal_params_t*)fractal_params)->const_real;
		const_imag = ((fractal_params_t*)fractal_params)->const_imag;

		if (!((fractal_params_t*)fractal_params)->params_updated) {
			continue;
		}

		((fractal_params_t*)fractal_params)->params_updated = false;

		pthread_mutex_lock(&mutex);

		erase();

		for (y = 0; y < LINES; ++y) {
			y0 = (((LINES / 2) - y) / zoom) + cam_y;

			for (x = 0; x < COLS; ++x) {
				x0 = ((x - (COLS / 2)) / zoom) / 2 + cam_x;
				iterations = function(x0, y0, const_real, const_imag);		

				if (has_color) {
					color_pair = ((iterations * 128) / MAX_ITERATIONS) % NUMBER_OF_COLORS + 1;

					if (color_pair != last_color_pair) {
						attron(COLOR_PAIR(color_pair));
						last_color_pair = color_pair;
					}
				}
				
				mvaddch(y, x, (unsigned long)GAMMA_ASCII[iterations % gamma_length]);
			}
		}

		print_info(cam_x, cam_y, zoom, has_color, const_real, const_imag);
		move(LINES-1, COLS-1); /* Move blinking cursor to rigth-bottom */
		refresh();

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}


void* check_input(void* fractal_params)
{
	noecho();
	keypad(stdscr, true);

	unsigned int pressed_key;

	do {
		pressed_key = getch();

		pthread_mutex_lock(&mutex);

		switch(pressed_key) {
			case 'w': {
				if (((fractal_params_t*)fractal_params)->zoom < ZOOM_MAX) {
					((fractal_params_t*)fractal_params)->zoom *= ZOOM_STEP;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case 's': {
				if (((fractal_params_t*)fractal_params)->zoom > ZOOM_STEP * ZOOM_DEFAULT) {
					((fractal_params_t*)fractal_params)->zoom /= ZOOM_STEP;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case 'c': {
				if (has_colors()) {
					((fractal_params_t*)fractal_params)->has_color = !((fractal_params_t*)fractal_params)->has_color;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case KEY_UP:    {
				if (((fractal_params_t*)fractal_params)->cam_y < CAM_Y_MAX) {
					((fractal_params_t*)fractal_params)->cam_y += 1 / ((fractal_params_t*)fractal_params)->zoom;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case KEY_DOWN:  {
				if (((fractal_params_t*)fractal_params)->cam_y > CAM_Y_MIN) {
					((fractal_params_t*)fractal_params)->cam_y -= 1 / ((fractal_params_t*)fractal_params)->zoom;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case KEY_LEFT:  {
				if (((fractal_params_t*)fractal_params)->cam_x > CAM_X_MIN) {
					((fractal_params_t*)fractal_params)->cam_x -= 1 / ((fractal_params_t*)fractal_params)->zoom;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case KEY_RIGHT: {
				if (((fractal_params_t*)fractal_params)->cam_x < CAM_X_MAX) {
					((fractal_params_t*)fractal_params)->cam_x += 1 / ((fractal_params_t*)fractal_params)->zoom;
					((fractal_params_t*)fractal_params)->params_updated = true;
				}
				break;
			}
			case '1': {
				((fractal_params_t*)fractal_params)->const_real += CONST_STEP;
				((fractal_params_t*)fractal_params)->params_updated = true;
				break;
			}
			case '2': {
				((fractal_params_t*)fractal_params)->const_real -= CONST_STEP;
				((fractal_params_t*)fractal_params)->params_updated = true;
				break;
			}
			case '3': {
				((fractal_params_t*)fractal_params)->const_imag += CONST_STEP;
				((fractal_params_t*)fractal_params)->params_updated = true;
				break;
			}
			case '4': {
				((fractal_params_t*)fractal_params)->const_imag -= CONST_STEP;
				((fractal_params_t*)fractal_params)->params_updated = true;
				break;
			}
			case 'j': {
				((fractal_params_t*)fractal_params)->function = julia_set;
				((fractal_params_t*)fractal_params)->zoom = ZOOM_DEFAULT;
				((fractal_params_t*)fractal_params)->const_real = CONST_REAL_DEFAULT;
				((fractal_params_t*)fractal_params)->const_imag = CONST_IMAG_DEFAULT;
				((fractal_params_t*)fractal_params)->cam_x = CAM_X_DEFAULT;
				((fractal_params_t*)fractal_params)->cam_y = CAM_Y_DEFAULT;
				((fractal_params_t*)fractal_params)->params_updated = true;
				break;
			}
			case 'm': {
				((fractal_params_t*)fractal_params)->function = mandelbrot_set;
				((fractal_params_t*)fractal_params)->zoom = ZOOM_DEFAULT;
				((fractal_params_t*)fractal_params)->const_real = CONST_REAL_DEFAULT;
				((fractal_params_t*)fractal_params)->const_imag = CONST_IMAG_DEFAULT;
				((fractal_params_t*)fractal_params)->cam_x = CAM_X_DEFAULT;
				((fractal_params_t*)fractal_params)->cam_y = CAM_Y_DEFAULT;
				((fractal_params_t*)fractal_params)->params_updated = true;
				break;
			}
			case ERR: {
				return (void*)EIO;
				break;
			}
			default: {
				break;
			}
		}

		pthread_mutex_unlock(&mutex);
	} while (pressed_key != 'q');

	return NULL;
}


unsigned int mandelbrot_set(double x0, double y0, double const_real, double const_imag)
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


unsigned int julia_set(double x0, double y0, double const_real, double const_imag)
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

		new_x = x2 - y2 + const_real;
		new_y = 2.0 * x * y + const_imag;

		x = new_x;
		y = new_y;
	}

	return 0;
}


void init_color_pairs()
{
	init_pair(1, COLOR_WHITE,   COLOR_BLACK);
	init_pair(2, COLOR_GREEN,   COLOR_BLACK);
	init_pair(3, COLOR_WHITE,   COLOR_BLACK);
	init_pair(4, COLOR_RED,     COLOR_BLACK);
	init_pair(5, COLOR_YELLOW,  COLOR_BLACK);
	init_pair(6, COLOR_WHITE,   COLOR_BLACK);
	init_pair(7, COLOR_WHITE,   COLOR_BLACK);
}


void print_info(double cam_x, double cam_y, double zoom, bool has_color, double const_real, double const_imag)
{
	if (has_color) {
		attron(COLOR_PAIR(1));
	}

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 32; ++j) {
			if (i == 7 || j >= 30) {
				mvaddch(i, j, ':');
			} else {
				mvaddch(i, j, ' ');
			}
		}
	}

	move(1, 1);
	printw("Cam.Ox: %f", cam_x);
	move(2, 1);
	printw("Cam.Oy: %f", cam_y);
	move(3, 1);
	if (zoom >= ZOOM_MAX) {
		printw("Zoom: %0.1f (MAX)", zoom);
	} else {
		printw("Zoom: %0.1f", zoom);
	}
	move(4, 1);
	printw("Const R: %f", const_real);
	move(5, 1);
	printw("Const I: %f", const_imag);
}