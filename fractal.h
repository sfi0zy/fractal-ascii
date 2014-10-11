#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <ncurses.h>
#include <pthread.h>
#include <math.h>


#define MAX_ITERATIONS 1024
#define ESCAPE_RADIUS  4.02

#define CAM_X_DEFAULT  0.0
#define CAM_X_MIN     -3.0
#define CAM_X_MAX      3.0
#define CAM_Y_DEFAULT  0.0
#define CAM_Y_MIN     -3.0
#define CAM_Y_MAX      3.0
#define ZOOM_DEFAULT   10
#define ZOOM_STEP      1.2
#define ZOOM_MIN       1

#define GAMMA_ASCII " .`-_':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q"
#define NUMBER_OF_COLORS 7


#define DOUBLE_COMPARSION_DELTA 0.00000001


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
void print_info(double, double, double);