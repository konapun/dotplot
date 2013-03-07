#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	dotplot *filtered = filter_dotplot(dp, minMatch);
	
	//destroy_dotplot(dp);
	
	//render_dotplot(filtered);
	//destroy_dotplot(filtered);
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

dotplot *filter_dotplot(dotplot *dp, int matchLength) {
	dotplot *zeroed = zero_dotplot(dp);
	int **cells = dp->cells;
	
	int i, j;
	int width = dp->width;
	int height = dp->height;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			if (cells[i][j] == 1) {
				_filter_dotplot_rec(dp, zeroed, i, j, matchLength, 0, 1);
				_filter_dotplot_rec(dp, zeroed, i, j, matchLength, 0, 0);
			}
		}
	}
}

void _filter_dotplot_rec(dotplot *dp, stretch *as, int x, int y, int matchLength, int runningTotal, int goLeft) {
	int **cells = dp->cells;
	if (goLeft) {
		x--;
	}
	else {
		x++;
	}
	y++;
	
	if (x > 0 && x < dp->width && y < dp->height) {
		if (cells[x][y] == 1) { // has match
			_filter_dotplot_rec(dp, as, x, y, matchLength, runningTotal++, goLeft); // continue match
		}
		else {
			if (runningTotal >= matchLength) { // no match, but met minimum length requirement
				
			}
		}
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

char *render_dotplot(dotplot *dp) {

}

stretch *create_stretch(int initialSize) {
	stretch *as = (stretch*) malloc(initialSize * sizeof(alignment)); //FIXME
	as->length = 0;
	as->maxLength = initialSize;
	
	return as;
}

void append_alignment(stretch *as, alignment al) {
	if (as->length == as->maxLength) {
		_expand_stretch(as, 20);
	}
	
	as->alignments[as->length] = al;
	as->length++;
}

void _expand_stretch(stretch *as, int expansionSize) {
	int newSize = as->length + expansionSize;
	alignment *stretched = memcpy
}
