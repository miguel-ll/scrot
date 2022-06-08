#include <X11/Xlib.h>
#include <Imlib2.h>
#include <string.h>
#include "display_X11.h"
#include "image_X11.h"

void image_X11_init(void)
{
	Visual *vis = DefaultVisual(disp, XScreenNumberOfScreen(scr));
	Colormap cm = DefaultColormap(disp, XScreenNumberOfScreen(scr));

	imlib_context_set_display(disp);
	imlib_context_set_visual(vis);
	imlib_context_set_colormap(cm);
	imlib_context_set_color_modifier(NULL);
	imlib_context_set_operation(IMLIB_OP_COPY);
}

Image image_X11_from_screen(void)
{
	imlib_context_set_drawable(scr->root);
	return imlib_create_image_from_drawable(0, 0, 0, scr->width, scr->height, 1);
}

Image image_X11_from_area(struct Area *area)
{
	imlib_context_set_drawable(scr->root);
	return imlib_create_image_from_drawable(0, area->x, area->y, area->width, area->height, 1);
}

void image_init(void) { image_X11_init(); }

Image image_from_screen(void) { return image_X11_from_screen(); }

Image image_from_area(struct Area *area) { return image_X11_from_area(area); }

int image_width(Image image) {
	imlib_context_set_image(image);
	return imlib_image_get_width();
}

int image_height(Image image) {
	imlib_context_set_image(image);
	return imlib_image_get_height();
}

int image_save(Image image, char *filename) {
	printf("%s\n", filename);
	char *tmp = strrchr(filename, '.');
	if (tmp) {
		imlib_image_set_format(tmp + 1);
	}

	Imlib_Load_Error err;
	imlib_context_set_image(image);
	imlib_save_image_with_error_return(filename, &err);

	return err != IMLIB_LOAD_ERROR_NONE;
}

Image image_scale(Image image, int cwidth, int cheight, int twidth, int theight) {
	imlib_context_set_image(image);
	imlib_context_set_anti_alias(1);
	return imlib_create_cropped_scaled_image(0, 0, cwidth, cheight, twidth, theight);
}

void image_set_quality(Image image, int quality) {
	imlib_context_set_image(image);
	imlib_image_attach_data_value("quality", NULL, quality, NULL);
}

