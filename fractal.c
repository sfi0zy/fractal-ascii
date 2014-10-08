#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <pthread.h>


#define MAX_ITERATIONS 1024
#define ESCAPE_RADIUS  4.02

#define GAMMA_ASCII " .`-_':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q"
#define NUMBER_OF_COLORS 7


typedef struct {
	double cam_x;
	double cam_y;
	double zoom;
	unsigned int iterations;
} fractal_params_t;


void* render_image(void*);
unsigned int mandelbrot_set(double, double);
void init_color_pairs();
void print_char(int, int, unsigned long, int);


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


void* render_image(void* fractal_params)
{
	int x, y;
	double x0, y0, cam_x, cam_y, zoom;
	unsigned int iterations;

	const unsigned int gamma_length = strlen(GAMMA_ASCII);

	start_color();
	init_color_pairs();

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
	attron(COLOR_PAIR(color_pair));
	mvaddch(y, x, char_param);
	attroff(COLOR_PAIR(color_pair));
}
