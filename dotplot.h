#include <gd.h>

/*
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
typedef struct {
	int width;
	int height;
	int **cells;
} dotplot;

dotplot *create_dotplot(char *seq1, char *seq2);
dotplot *clone_dotplot(dotplot *dp);
void destroy_dotplot(dotplot *dp);
dotplot *zero_dotplot(dotplot *dp);
dotplot *filter_dotplot(dotplot *dp, int matchLength);
gdImagePtr render_dotplot(dotplot *dp, int width, int height);
int write_image(gdImagePtr image, char *filename);
void print_dotplot(dotplot *dp);
