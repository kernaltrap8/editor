#define EDITOR_VERSION "0.1"
#define quitKey 'q'
static char rawTermBuff = '\0';

// function declaration
void enableRawMode();
void disableRawMode();
void printChar();
void die(const char* s);
char editorReadKey();

#define CTRL_KEY(k) ((k) & 0x1f)

struct editorConfig {
    int cx, cy;
    int screenrows;
    int screencols;
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
    ARROW_DOWN
};