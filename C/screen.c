#include "screen.h"

/**
 * Generate a random number
 */
int randint(int min, int max) {
  return min + (rand() % (max - min));
}

/**
 * Find the average color of a block
 * @param i index to write colour to in values
 * @param min_x minimum x value for a pixel
 * @param max_x maximum x value for a pixel
 * @param min_y minimum y value for a pixel
 * @param max_y maximum y value for a pixel
 * @param values array of bytes to write color to
 * @param d The X display to query
 * @param image The snapshot of the display to query
 * @param pixels_to_process The number of pixels to use for an average
 * @param brightness A percentage to scale the color by
 */
void fillRGB(int i, int min_x, int max_x, int min_y, int max_y, unsigned char values[], 
			 Display* d, XImage* image, int pixels_to_process, int brightness) {
	int total[3] = {0, 0, 0};
	int x, y;
	XColor c;

	for (int j = 0; j < pixels_to_process; j++) {
		x = randint(min_x, max_x);
		y = randint(min_y, max_y);

		c.pixel = XGetPixel(image, x, y);
		XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

		total[0] += c.red/256;
		total[1] += c.green/256;
		total[2] += c.blue/256;
	}

        values[i] = 'L';
	for (int j = 1; j < 4; j ++) 
		values[i+j] = total[j-1] / pixels_to_process * brightness / 100;
}

/**
 * Get the RGB colors for the display
 * @param d the X display to get the colors from
 * @param values the array to write the colors to
 * @param t a random seed
 * @param cnf config
 */
void get_colors(Display *d, unsigned char *values, unsigned t, struct config *cnf) {
	XImage *image;

	srand(t); // Initialising random

	//image = XGetImage(d, RootWindow(d, DefaultScreen(d)), 0, 0, cnf->horizontal_pixel_count, cnf->vertical_pixel_count, AllPlanes, ZPixmap);
	image = XGetImage(d, DefaultRootWindow(d), 0, 0, cnf->horizontal_pixel_count, cnf->vertical_pixel_count, AllPlanes, ZPixmap);

	//Filling the left side
	for (int i = 0; i < cnf->leds_on_side; i++) {
		fillRGB(4 * (cnf->leds_on_side - i - 1),
				0, cnf->pixels_per_led_side,
				cnf->vertical_pixel_gap + (i * cnf->pixels_per_led_side), cnf->vertical_pixel_gap + (i+1) * cnf->pixels_per_led_side,
				values, d, image, cnf->pixels_to_process, cnf->brightness * cnf->left_brightness_factor / 100);
	}

	//Filling the top side
	for (int i = 0; i < cnf->leds_on_top; i++) {
		fillRGB(cnf->leds_on_side * 4 + 4 * i,
				cnf->horizontal_pixel_gap + i * cnf->pixels_per_led_top, cnf->horizontal_pixel_gap + (i+1) * cnf->pixels_per_led_top,
				0, cnf->pixels_per_led_top,
				values, d, image, cnf->pixels_to_process, cnf->brightness * cnf->top_brightness_factor / 100);
	}

	//Filling the right side
	for (int i = 0; i < cnf->leds_on_side; i++) {
		fillRGB((cnf->leds_on_side + cnf->leds_on_top + i) * 4,
				cnf->horizontal_pixel_count - cnf->pixels_per_led_side, cnf->horizontal_pixel_count - 1,
				cnf->vertical_pixel_gap + i * cnf->pixels_per_led_side, cnf->vertical_pixel_gap + (i+1) * cnf->pixels_per_led_side,
				values, d, image, cnf->pixels_to_process, cnf->brightness * cnf->right_brightness_factor / 100);
	}

	XDestroyImage(image);
}

void get_colors_rainbow(Display *d, unsigned char *values, unsigned t, struct config *cnf) {
    int total = 2* (cnf->leds_on_side + cnf->leds_on_top);
    for (int k = 0; k < total; k++) {
        values[4*k + 2] = 'L';
        values[4*k + 3] = 254;
        values[4*k + 4] = 45;
        values[4*k + 5] = 0;
    }
}

