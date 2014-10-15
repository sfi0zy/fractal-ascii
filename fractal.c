#include "fractal.h"


extern pthread_mutex_t mutex;


void* render_image(void* fractal_params)
{
	bool has_color;
	int x, y;
	double x0, y0, cam_x, cam_y, zoom, last_cam_x, last_cam_y, last_zoom;
	unsigned int iterations, color_pair, last_color_pair;
	
	const unsigned int gamma_length = strlen(GAMMA_ASCII);

	last_color_pair = 1;
	last_cam_x = last_cam_y = last_zoom = 1.0;

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

		if (fabs(last_cam_x - cam_x) < DOUBLE_COMPARSION_DELTA &&
			fabs(last_cam_y - cam_y) < DOUBLE_COMPARSION_DELTA &&
			fabs(last_zoom - zoom) < DOUBLE_COMPARSION_DELTA) {
			goto __render_image_next_iter;
		}

		pthread_mutex_lock(&mutex);

		erase();

		for (y = 0; y < LINES; ++y) {
			y0 = (((LINES / 2) - y) / zoom) + cam_y;

			for (x = 0; x < COLS; ++x) {
				x0 = ((x - (COLS / 2)) / zoom) / 2 + cam_x;
				iterations = mandelbrot_set(x0, y0);		

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

		print_info(cam_x, cam_y, zoom, has_color);
		move(LINES-1, COLS-1); /* Move blinking cursor to rigth-bottom */
		refresh();

		pthread_mutex_unlock(&mutex);

		__render_image_next_iter:

		last_cam_x = cam_x;
		last_cam_y = cam_y;
		last_zoom = zoom;
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
				}
				break;
			}
			case 's': {
				if (((fractal_params_t*)fractal_params)->zoom > ZOOM_STEP * ZOOM_DEFAULT) {
					((fractal_params_t*)fractal_params)->zoom /= ZOOM_STEP;
				}
				break;
			}
			case 'c': {
				if (has_colors()) {
					((fractal_params_t*)fractal_params)->has_color = !((fractal_params_t*)fractal_params)->has_color;
				}
				break;
			}
			case KEY_UP:    {
				if (((fractal_params_t*)fractal_params)->cam_y < CAM_Y_MAX) {
					((fractal_params_t*)fractal_params)->cam_y += 1 / ((fractal_params_t*)fractal_params)->zoom;
				}
				break;
			}
			case KEY_DOWN:  {
				if (((fractal_params_t*)fractal_params)->cam_y > CAM_Y_MIN) {
					((fractal_params_t*)fractal_params)->cam_y -= 1 / ((fractal_params_t*)fractal_params)->zoom;
				}
				break;
			}
			case KEY_LEFT:  {
				if (((fractal_params_t*)fractal_params)->cam_x > CAM_X_MIN) {
					((fractal_params_t*)fractal_params)->cam_x -= 1 / ((fractal_params_t*)fractal_params)->zoom;
				}
				break;
			}
			case KEY_RIGHT: {
				if (((fractal_params_t*)fractal_params)->cam_x < CAM_X_MAX) {
					((fractal_params_t*)fractal_params)->cam_x += 1 / ((fractal_params_t*)fractal_params)->zoom;
				}
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
	init_pair(2, COLOR_GREEN,   COLOR_BLACK);
	init_pair(3, COLOR_WHITE,   COLOR_BLACK);
	init_pair(4, COLOR_RED,     COLOR_BLACK);
	init_pair(5, COLOR_YELLOW,  COLOR_BLACK);
	init_pair(6, COLOR_WHITE,   COLOR_BLACK);
	init_pair(7, COLOR_WHITE,   COLOR_BLACK);
}


void print_info(double cam_x, double cam_y, double zoom, bool has_color)
{
	if (has_color) {
		attron(COLOR_PAIR(1));
	}

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 32; ++j) {
			if (i == 5 || j >= 30) {
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
}