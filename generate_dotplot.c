#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include "dotplot.h"

/*
* Create a dotplot from 2 nucleotide strings and write results as an image to a file
*/
int main(int argc, char **argv) {
	if (argc != 3) {
		return 1;
	}
	
	int minMatch = 5;
	dotplot *dp = create_dotplot(argv[1], argv[2]);
	//dotplot *filtered = filter_dotplot(dp, minMatch);
	
	gdImagePtr image = render_dotplot(dp, 2000, 2000);
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
	int i, j;
	for (i = 0; i < dp->width; i++) {
		for (j = 0; j < dp->height; j++) {
			if (seq1[i] == seq2[j]) {
				dp->cells[i][j] = 1;
			}
			else {
				dp->cells[i][j] = 0;
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
	int i, j;
	for (i = 0; i < zeroed->width; i++) {
		for (j = 0; j < zeroed->height; j++) {
			zeroed->cells[i][j] = 0;
		}
	}
	
	return zeroed;
}

dotplot *clone_dotplot(dotplot *dp) {
	dotplot *clone = _dotplot_allocate(dp->width, dp->height);
	int **cells = dp->cells;
	int **cloneCells = clone->cells;
	
	int i, j;
	for (i = 0; i < dp->width; i++) {
		for (j = 0; j < dp->height; j++) {
			cloneCells[i][j] = cells[i][j];
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
	
	_filter_left_diagonal(dp, zeroed, matchLength);
	_filter_right_diagonal(dp, zeroed, matchLength);
}

// Build diagonals extending from the upper right to lower left
void _filter_left_diagonal(dotplot *original, dotplot *filtered, int matchLength) {
	int stretch, x, y;
	
	stretch = 0;
	x = original->width - 1;
	y = 0;
	while (x > 0) { // build diagonals along top
		if (y < original->height) {
			if (original->cells[x][y] == 1) {
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					_set_match(filtered, x, y, UL, stretch);
				}
			}
		}
	}
	
	stretch = 0;
	x = 0;
	y = 0;
	//while (y < original->height) { // build diagonals along side
	//}
}

void _filter_right_diagonal(dotplot *original, dotplot *filtered, int matchLength) {
	int stretch = 0;
	int i, j;
	for (i = 0; i < original->width; i++) {
		for (j = i; j < original->height; j++) {
			if (original->cells[i][j] == 1) {
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					_set_match(filtered, i, j, UR, stretch);
				}
			}
		}
	}
}

void _set_match(dotplot *filtered, int x, int y, direction dir, int length) {
	filtered->cells[x][y] = 1;
	switch(dir) {
		case UL: // build from (x, y) diagonally to upper left
			while (length-- > 0) {
				filtered->cells[x--][y--] = 1;
			}
			break;
		case UR: // build from (x, y) diagonally to upper right
			while (length-- > 0) {
				filtered->cells[x++][y--] = 1;
			}
			break;
		default:
			// ignore
	}
}

void print_dotplot(dotplot *dp) {
	int i, j;
	for (i = 0; i < dp->width; i++) {
		for (j = 0; j < dp->height; j++) {
			int cell = dp->cells[i][j];
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
