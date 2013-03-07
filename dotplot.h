typedef enum {
	UL, // upper left
	UR  // upper right
} direction;

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

/* Private */
dotplot *_dotplot_allocate(int width, int height);
void _filter_left_diagonal(dotplot *original, dotplot *filtered, int matchLength);
void _filter_right_diagonal(dotplot *original, dotplot *filtered, int matchLength);
void _set_match(dotplot *filtered, int x, int y, direction dir, int length);
