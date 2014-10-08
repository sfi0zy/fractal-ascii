#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <pthread.h>


#define MAX_ITERATIONS 1024
#define ESCAPE_RADIUS  4.02

#define DEFAULT_CAM_X  0.0
#define DEFAULT_CAM_Y  0.0
#define DEFAULT_ZOOM   10

#define GAMMA_ASCII " .`-_':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q"
#define NUMBER_OF_COLORS 7


typedef struct {
	double cam_x;
	double cam_y;
	double zoom;
	unsigned int iterations;
} fractal_params_t;


void* render_image(void*);
void* check_input(void*);

unsigned int mandelbrot_set(double, double);
void init_color_pairs();
void print_char(int, int, unsigned long, int);