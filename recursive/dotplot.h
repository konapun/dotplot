typedef struct {
	int width;
	int height;
	int **cells;
} dotplot;

dotplot *_dotplot_allocate(int width, int height);
dotplot *create_dotplot(char *seq1, char *seq2);
dotplot *clone_dotplot(dotplot *dp);
void destroy_dotplot(dotplot *dp);
dotplot *zero_dotplot(dotplot *dp);
dotplot *filter_dotplot(dotplot *dp, int matchLength);
void _filter_dotplot_rec(dotplot *dp, dotplot *zeroed, int x, int y, int matchLength, int runningTotal, int goLeft);
char *render_dotplot(dotplot *dp);
void print_dotplot(dotplot *dp);

/////////////////////////////////////
typedef struct {
	int x;
	int y;
} alignment;

typedef struct {
	int length;
	int maxLength;
	alignment *alignments;
} stretch;

stretch *create_stretch(int initialSize);
void append_alignment(stretch *as, alignment *al);
void _expand_stretch(stretch *as, int expansionSize);