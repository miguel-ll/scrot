#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "display_X11.h"
#include "image_X11.h"

#define QUALITY 75

void help_show_usage(void) {
	fprintf(stdout,
			"Usage : scrot [OPTION] \n"
			"-h, --help                display this help and exit\n"
			"Default quality: 75. You can change it in scrot.c.\n"
			"-s  interactively choose a rectnagle with the mouse\n"
			"-w  interactively choose a window with the mouse\n\n"
			"This program is free software.\n"
			"Copyright Tom Gilbert 2000\n"
			"Copyleft Miguel 'mkn' 2022\n");
	exit(EXIT_SUCCESS);
}

static Image scrot_shoot(int sel, int win)
{
	if (sel || win) {
		struct Area *area;
		if (sel)
			area = display_select_area();
		else
			area = display_select_window();
		if (area == NULL) {
			fprintf(stderr, "Couldn't select area\n");
			exit(EXIT_FAILURE);
		}
		return image_from_area(area);
	}
	return image_from_screen();
}

int main(int argc, char **argv)
{
      int select = 0;
      int win = 0;

      if (argc > 1) {
      switch(argv[1][1]) {
	case 's':
		select = 1;
		break;
	case 'w':
		win = 1;
		break;
	default:
		help_show_usage();
		break;
	}
        }
        else
	     help_show_usage();
	if(!display_init()) {
		fprintf(stderr, "Can't open display\n");
	        exit(EXIT_FAILURE);
	}
	image_init();

	Image image = scrot_shoot(select, win);
	if (image == NULL) {
		fprintf(stderr, "no image grabbed\n");
		exit(EXIT_FAILURE);
        }
	image_set_quality(image, QUALITY);

	time_t t;
	char path_image[40];

	time(&t);
	struct tm *info = localtime(&t);
	strftime(path_image, 40, "%Y-%m-%d-%H%M%S_scrot.png", info);

	int error = image_save(image, path_image);
	if (error) {
		fprintf(stderr, "Saving to file %s failed\n", path_image);
		exit(EXIT_FAILURE);
	}
}

