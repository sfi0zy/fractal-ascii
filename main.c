#include "fractal.h"


int main(void)
{
	pthread_t render_thread_id;
	pthread_t input_thread_id;
	fractal_params_t params;

	params.cam_x = DEFAULT_CAM_X;
	params.cam_y = DEFAULT_CAM_Y;
	params.zoom  = DEFAULT_ZOOM;

	initscr();
	noecho();
	keypad(stdscr, true);

	pthread_create(&render_thread_id, NULL, &render_image, &params);
	pthread_create(&input_thread_id, NULL, &check_input, &params);

	pthread_join(input_thread_id, NULL);

	endwin();
	exit(0);
}