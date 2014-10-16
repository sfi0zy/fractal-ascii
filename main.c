#include "fractal.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char** argv)
{
	pthread_t render_thread_id;
	pthread_t input_thread_id;
	fractal_params_t params;
	int next_option, err_flag;

	err_flag = 0;

	while((next_option = getopt(argc, argv, ":h")) != -1) {
		switch (next_option) {
			case 'h':
				printf(HELP_STRING);
				exit(EXIT_SUCCESS);
			case '?':
				fprintf(stderr, "Unrecognised option: -%c\n", optopt);
				err_flag++;
				break;
			default:
				break;
		}
	}

	while (optind < argc) {
		fprintf(stderr, "invalid argument %s\n", optarg);
		optind++;
		err_flag++;
	}

	if (err_flag) {
		exit(EINVAL);
	}

	params.const_real = CONST_REAL_DEFAULT;
	params.const_imag = CONST_IMAG_DEFAULT;
	params.function = FRACTAL_FUNCTION_DEFAULT;
	params.cam_x = CAM_X_DEFAULT;
	params.cam_y = CAM_Y_DEFAULT;
	params.zoom  = ZOOM_DEFAULT;
	params.has_color = HAS_COLOR_DEFAULT;
	params.params_updated = true;

	initscr();

	pthread_create(&render_thread_id, NULL, &render_image, &params);
	pthread_create(&input_thread_id, NULL, &check_input, &params);

	pthread_join(input_thread_id, NULL);
	pthread_cancel(render_thread_id);

	endwin();
	exit(EXIT_SUCCESS);
}