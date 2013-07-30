#include <stdio.h>
#include <unistd.h>
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
	/* Process opts */
	int minMatch = 5;
	int width = 2000;
	int height = 2000;
	int c;
	while ((c = getopt(argc, argv, "n:w:h:")) != -1) {
		switch (c) {
			case 'n':
				minMatch = atoi(optarg);
				break;
			case 'w':
				width = atoi(optarg);
				break;
			case 'h':
				height = atoi(optarg);
				break;
			default:
				return 1;
		}
	}
	
	/* Process args */
	int oi;
	int i = 0;
	char *seq1;
	char *seq2;
	char *filename;
	for (oi = optind; oi < argc; oi++) {
		if (i == 0) {
			seq1 = argv[oi];
		}
		else if (i == 1) {
			seq2 = argv[oi];
		}
		else if (i == 2) {
			filename = argv[oi];
		}
		else {
			fprintf(stderr, "Wrong number of arguments\nUsage: %s sequence1 sequence2 filename\n", argv[0]);
			return 1;
		}
		
		i++;
	}
	
	dotplot *dp = create_dotplot(seq1, seq2);
	dotplot *filtered = filter_dotplot(dp, minMatch);
	destroy_dotplot(dp);
	
	gdImagePtr image = render_dotplot(filtered, width, height);
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
