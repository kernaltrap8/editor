#define EDITOR_VERSION "0.2"
#define quitKey 'q'
static char rawTermBuff = '\0';

// function declaration
void enableRawMode();
void disableRawMode();
void printChar();
void die(const char* s);
char editorReadKey();
//void abFree(struct abuf* ab);

#define CTRL_KEY(k) ((k) & 0x1f)

typedef struct erow {
    int size;
    char* chars;
} erow;

struct editorConfig {
    int cx, cy;
    int rowoff;
    int coloff;
    int screenrows;
    int screencols;
    int numrows;
    erow* row;
    struct termios orig_termios;
};

static struct editorConfig E;

struct abuf {
    char* b;
    int len;
};

#define ABUF_INIT {NULL, 0}

enum editorKey {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};
