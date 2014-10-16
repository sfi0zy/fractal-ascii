#ifndef FRACTAL_H_INCLUDED
#define FRACTAL_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <ncurses.h>
#include <pthread.h>
#include <math.h>
#include <getopt.h>


#define MAX_ITERATIONS 1024
#define ESCAPE_RADIUS  4.02

#define CONST_REAL_DEFAULT 0.0
#define CONST_IMAG_DEFAULT 0.0
#define CONST_STEP 0.001

#define FRACTAL_FUNCTION_DEFAULT mandelbrot_set

#define CAM_X_DEFAULT  0.0
#define CAM_X_MIN     -3.0
#define CAM_X_MAX      3.0
#define CAM_Y_DEFAULT  0.0
#define CAM_Y_MIN     -3.0
#define CAM_Y_MAX      3.0
#define ZOOM_DEFAULT   10
#define ZOOM_STEP      1.2
#define ZOOM_MIN       1
#define ZOOM_MAX       10000000
#define HAS_COLOR_DEFAULT false

#define GAMMA_ASCII " .`-_':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q"
#define NUMBER_OF_COLORS 7


#define DOUBLE_COMPARSION_DELTA 0.00000001


#define HELP_STRING "-------------------------------------------------\n\
Fractal generator with ASCII graphics for Linux\n\
-------------------------------------------------\n\
* 'W' and 'S' for zoom\n\
* Arrow keys for moving picture\n\
* 'C' to color on/off\n\
* '1' and '2' to change real const\n\
* '3' and '4' to change imaginary const\n\
* 'J' - switch to Julia Set\n\
* 'M' - switch to Mandelbrot Set\n"


typedef struct {
	double const_real;
	double const_imag;
	double cam_x;
	double cam_y;
	double zoom;
	unsigned int iterations;
	bool has_color;
	bool params_updated;
	unsigned int (*function)(double, double, double, double);
} fractal_params_t;


void* render_image(void*);
void* check_input(void*);

unsigned int mandelbrot_set(double, double, double, double);
unsigned int julia_set(double, double, double, double);

void init_color_pairs();
void print_info(double, double, double, bool, double, double);

#endif /* FRACTAL_H_INCLUDED */