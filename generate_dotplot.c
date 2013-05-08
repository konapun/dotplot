#include <stdio.h>
#include "dotplot.h"

/*
 * Create a dotplot from 2 nucleotide strings and write results as an image to a file
 *
 * (C) Copyright 2013 FlyExpress (http://www.flyexpress.net/)
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Author:
 *     Bremen Braun (konapun)
 */
int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Wrong number of arguments\nUsage: %s sequence1 sequence2 filename\n", argv[0]);
		return 1;
	}
	
	char *seq1 = argv[1];
	char *seq2 = argv[2];
	char *filename = argv[3];
	
	int minMatch = 5;
	dotplot *dp = create_dotplot(seq1, seq2);
	dotplot *filtered = filter_dotplot(dp, minMatch);
	destroy_dotplot(dp);
	
	gdImagePtr image = render_dotplot(filtered, 2000, 2000);
	int did_write = write_image(image, filename);
	gdImageDestroy(image);
	if (!did_write) {
		fprintf(stderr, "Can't create %s\n", filename);
		return 2;
	}
	
	destroy_dotplot(filtered);
	return 0;
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
