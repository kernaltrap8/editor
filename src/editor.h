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
    int screenrows;
    int screencols;
    struct termios orig_termios;
};

static struct editorConfig E;
