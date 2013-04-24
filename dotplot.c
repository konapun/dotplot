#include "dotplot.h"
#include <stdlib.h>
#include <string.h>

/*** Private ***/
typedef enum {
	UL, // upper left
	UR  // upper right
} direction;

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

/*
* Build diagonals extending from the upper right to lower left
* TODO: Consolidate code
*/
void _filter_left_diagonals(dotplot *original, dotplot *filtered, int matchLength) {
	int stretch, x;
	
	x = original->width-1;
	while (x >= 0) { // upper right (rows)
		stretch = 0;
		int y = 0;
		int x2 = x;
		while (x2 >= 0) {
			if (original->cells[x2][y] == 1) {
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					_set_match(filtered, x2+1, y-1, UR, stretch);
				}
				
				stretch = 0;
			}
			
			x2--;
			y++;
		}
		
		if (stretch >= matchLength) {
			_set_match(filtered, x2+1, y-1, UR, stretch);
		}
		x--;
	}
	
	int y = 1;
	while (y < original->height) { // lower right (columns)
		stretch = 0;
		int x = original->width-1;
		int y2 = y;
		while (y2 < original->height && x > 0) {
			if (original->cells[x][y2] == 1) {
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					_set_match(filtered, x+1, y2-1, UR, stretch);
				}
				
				stretch = 0;
			}
			
			y2++;
			x--;
		}
		
		if (stretch >= matchLength) {
			_set_match(filtered, x+1, y2-1, UR, stretch);
		}
		
		y++;
	}
}

/*
* Build diagonals extending from the upper left to lower right
* TODO: Consolidate code
*/
void _filter_right_diagonals(dotplot *original, dotplot *filtered, int matchLength) {
	int stretch, x;
	
	x = 0;
	while (x < original->width) { // upper right (rows)
		stretch = 0;
		int y = 0;
		int x2 = x;
		while (x2 < original->width) {
			if (original->cells[x2][y] == 1) {
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					_set_match(filtered, x2-1, y-1, UL, stretch);
				}
				
				stretch = 0;
			}
			
			x2++;
			y++;
		}
		
		if (stretch >= matchLength) {
			_set_match(filtered, x2-1, y-1, UL, stretch);
		}
		x++;
	}
	
	int y = 1;
	while (y < original->height) { // lower left (columns)
		stretch = 0;
		int x = 0;
		int y2 = y;
		while (y2 < original->height && x < original->width) {
			if (original->cells[x][y2] == 1) {
				stretch++;
			}
			else {
				if (stretch >= matchLength) {
					_set_match(filtered, x-1, y2-1, UL, stretch);
				}
				
				stretch = 0;
			}
			
			y2++;
			x++;
		}
		
		if (stretch >= matchLength) {
			_set_match(filtered, x-1, y2-1, UL, stretch);
		}
		y++;
	}
}

void _set_match(dotplot *filtered, int x, int y, direction dir, int length) {
	filtered->cells[x][y] = 1;
	length--;
	switch(dir) {
		case UL: // build from (x, y) diagonally to upper left
			while (length > 0) {
				filtered->cells[--x][--y] = 1;
				length--;
			}
			break;
		case UR: // build from (x, y) diagonally to upper right
			length++;
			while (length > 0) {
				filtered->cells[x++][y--] = 1;
				length--;
			}
			break;
		default:
			return;
	}
}

/*** Public ***/
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
	
	_filter_left_diagonals(dp, zeroed, matchLength);
	_filter_right_diagonals(dp, zeroed, matchLength);
	return zeroed;
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
	/* don't scale up */
	if (width > dp->width) {
		width = dp->width;
	}
	if (height > dp->height) {
		height = dp->height;
	}
	
	double min_width = 1.0;
	double min_height = 1.0;
	double cell_width = (double) width / (double) dp->width;
	double cell_height = (double) height / (double) dp->height;
	double render_width = cell_width;
	double render_height = cell_height;
	if (render_width < min_width) {
		render_width = min_width;
	}
	if (render_height < min_height) {
		render_height = min_height;
	}
	
	gdImagePtr image = gdImageCreate(width, height);
	int background_color = gdImageColorAllocate(image, 232, 232, 232);
	int match_color = gdImageColorAllocate(image, 0, 0, 0);
	
	int x, y;
	double pixel_x = 0.0;
	double pixel_y = 0.0;
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