#include "fractal.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main(void)
{
	pthread_t render_thread_id;
	pthread_t input_thread_id;
	fractal_params_t params;

	void* input_thread_ret_status;

	params.cam_x = CAM_X_DEFAULT;
	params.cam_y = CAM_Y_DEFAULT;
	params.zoom  = ZOOM_DEFAULT;

	initscr();

	pthread_create(&render_thread_id, NULL, &render_image, &params);
	pthread_create(&input_thread_id, NULL, &check_input, &params);

	pthread_join(input_thread_id, &input_thread_ret_status);
	pthread_cancel(render_thread_id);

	endwin();
	exit(0);
}