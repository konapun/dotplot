#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include "dotplot.h"

/*
* Create a dotplot from 2 nucleotide strings and write results as an image to a file
*
* author: Bremen Braun, 2013 for FlyExpress
*/
int main(int argc, char **argv) {
	if (argc != 3) {
		return 1;
	}
	
	int minMatch = 1;
	dotplot *dp = create_dotplot(argv[1], argv[2]);
	dotplot *filtered = filter_dotplot(dp, minMatch);
	
	print_dotplot(dp);
	printf("---------\n");
	print_dotplot(filtered);
	gdImagePtr image = render_dotplot(filtered, 2000, 2000);
	int did_write = write_image(image, "test.png");
	gdImageDestroy(image);
	if (!did_write) {
		fprintf(stderr, "Can't create test.png\n");
		return 2;
	}
	
	return 0;
}

dotplot *create_dotplot(char *seq1, char *seq2) {
	dotplot *dp = _dotplot_allocate(strlen(seq1), strlen(seq2));
	int y, x;
	for (y = 0; y < dp->height; y++) {
		for (x = 0; x < dp->width; x++) {
			if (seq1[x] == seq2[y]) {
				dp->cells[x][y] = 1;
			}
			else {
				dp->cells[x][y] = 0;
			}
		}
	}
	
	return dp;
}

dotplot *_dotplot_allocate(int width, int height) {
	int i;
	int **cells;
	cells = (int**) malloc(width * sizeof(int*));
	for (i = 0; i < width; i++) {
		cells[i] = (int*) malloc(height * sizeof(int));
	}
	
	dotplot *dp = (dotplot *) malloc(sizeof(dotplot));
	dp->width = width;
	dp->height = height;
	dp->cells = cells;
	
	return dp;
}

dotplot *zero_dotplot(dotplot *dp) {
	dotplot *zeroed = _dotplot_allocate(dp->width, dp->height);
	int y, x;
	for (y = 0; y < zeroed->height; y++) {
		for (x = 0; x < zeroed->width; x++) {
			zeroed->cells[x][y] = 0;
		}
	}
	
	return zeroed;
}

dotplot *clone_dotplot(dotplot *dp) {
	dotplot *clone = _dotplot_allocate(dp->width, dp->height);
	int **cells = dp->cells;
	int **cloneCells = clone->cells;
	
	int y, x;
	for (y = 0; y < dp->height; y++) {
		for (x = 0; x < dp->width; x++) {
			cloneCells[x][y] = cells[x][y];
		}
	}
	
	return clone;
}

void destroy_dotplot(dotplot *dp) {
	int **cells = dp->cells;
	//TODO
}

dotplot *filter_dotplot(dotplot *dp, int matchLength) {
	dotplot *zeroed = zero_dotplot(dp);
	int **cells = dp->cells;
	
	//_filter_left_diagonals(dp, zeroed, matchLength);
	_filter_right_diagonals(dp, zeroed, matchLength);
	return zeroed;
}

// Build diagonals extending from the upper right to lower left
void _filter_left_diagonals(dotplot *original, dotplot *filtered, int matchLength) {
	int stretch, x;
}

//TODO: Consolidate code
void _filter_right_diagonals(dotplot *original, dotplot *filtered, int matchLength) {
	int stretch, x;
	
	x = 0;
	while (x < original->width) { // upper right
		stretch = 0;
		int y = 0;
		int x2 = x;
		while (x2 < original->width) {
			//printf("On cell (%d, %d)\n", x2, y);
			if (original->cells[x2][y] == 1) {
				printf("MATCH at (%d, %d)\n", x2, y);
				stretch++;
			}
			else {
				printf("No match at (%d, %d)\n", x2, y);
				if (stretch >= matchLength) {
					printf("Setting match of length %d from (%d, %d)\n", stretch, x2-1, y-1);
					_set_match(filtered, x2-1, y-1, UL, stretch);
				}
				
				stretch = 0;
			}
			
			x2++;
			y++;
		}
		
		if (stretch >= matchLength) {
			printf("Setting match of length %d from (%d, %d)\n", stretch, x2-1, y-1);
			_set_match(filtered, x2-1, y-1, UL, stretch);
		}
		x++;
	}
	
	stretch = 0;
	x = 1;
	while (x < original->width) { // lower left
		int y = 0;
		int x2 = x;
		while (x2 < original->width) {
			if (original->cells[x2][y] == 1) { //FIXME: order?
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					//_set_match(filtered, x2, y, UL, stretch); //FIXME: order?
				}
			}
			
			x2++;
			y++;
		}
		x++;
	}
	
	//printf("RETURNING\n");
}

void _set_match(dotplot *filtered, int x, int y, direction dir, int length) {
	printf("Got match of length %d at (%d, %d)\n", length, x, y);
	printf("Setting (%d, %d) with %d remaining\n", x, y, length);
	filtered->cells[x][y] = 1;
	length--;
	switch(dir) {
		case UL: // build from (x, y) diagonally to upper left
			while (length > 0) {
				printf("Setting (%d, %d) with %d remaining\n", x-1, y-1, length);
				filtered->cells[--x][--y] = 1;
				length--;
			}
			break;
		case UR: // build from (x, y) diagonally to upper right
			while (length-- > 0) {
				filtered->cells[x++][y--] = 1;
			}
			break;
		default:
			return;
	}
}

void print_dotplot(dotplot *dp) {
	int y, x;
	for (y = 0; y < dp->height; y++) {
		for (x = 0; x < dp->width; x++) {
			int cell = dp->cells[x][y];
			printf("%i", cell);
		}
		printf("\n");
	}
}

gdImagePtr render_dotplot(dotplot *dp, int width, int height) {
	gdImagePtr image = gdImageCreate(width, height);
	int background_color = gdImageColorAllocate(image, 232, 232, 232);
	int match_color = gdImageColorAllocate(image, 0, 0, 0);
	
	int min_width = 1;
	int min_height = 1;
	int cell_width = width / dp->width;
	int cell_height = height / dp->height;
	int render_width = cell_width;
	int render_height = cell_height;
	if (render_width < min_width) {
		render_width = min_width;
	}
	if (render_height < min_height) {
		render_height = min_height;
	}
	
	int x, y;
	int pixel_x = 0;
	int pixel_y = 0;
	for (y = 0; y < dp->height; y++) {
		pixel_x = 0;
		for (x = 0; x < dp->width; x++) { 
			if (dp->cells[x][y] == 1) { // match
				gdImageFilledRectangle(image, pixel_x, pixel_y, pixel_x + render_width, pixel_y + render_height, match_color);
			}
			
			pixel_x += cell_width;
		}
		
		pixel_y += cell_height;
	}
	
	return image;
}

int write_image(gdImagePtr image, char *filename) {
	FILE *out = fopen(filename, "wb");
	if (!out) {
		return 0;
	}
	gdImagePng(image, out);
	fclose(out);
	return 1;
}
