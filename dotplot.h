#include <gd.h>

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
