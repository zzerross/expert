#include "pr.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

int dy[4] = { -1, 0, 1,  0 };
int dx[4] = {  0, 1, 0, -1 };

enum State {
    FREE,
    VISITED,
    FIXED,
} gState[5][5];

template <int S>
struct Board {
    int (*buf)[S];

    void init(int b[S][S]) {
        buf = b;

        dump();
    }

    void dump(const char* s = nullptr) {
#ifdef _pl_
        _pr(1, "%10s: \n", s ? s : __func__);

        for (int i = 0; i < S; i++) {
            _pr(1, "%10s: ", "");

            for (int j = 0; j < S; j++)
                _pr(1, "%d ", buf[i][j]);

            _pr(1, "\n");
        }
#endif
    }
};

Board<5> gBoard;
Board<3> gPattern;
Board<5> gOrigin;

struct Point {
    int y, x;

    void set(int y, int x) {
        this->y = y;
        this->x = x;
    }
};

struct Path {
#define POINTS (25 * 25)
    Point buf[POINTS];
    int size;

    void init() {
        size = 0;
    }

    void add(int y, int x) {
        if (POINTS <= size)
            _pr(0, "overflow");

        buf[size++].set(y, x);
    }

    void dump() {
#ifdef _pl_
        _pr(1, "%10s: size=%d ", __func__, size);

        for (int i = 0; i < size; i++)
            _pr(1, "(%d,%d) ", buf[i].y, buf[i].x);
#endif
    }
};

Path gPath;

extern bool swap(int dir);

int rng(int i, int j) {
    return 0 <= i && i < 5 && 0 <= j && j < 5;
}

void next(int& y, int& x) {
    y = 1, x = 1;

    for (int i = 0; i < 3; i++, y++) {
        for (int j = 0; j < 3; j++, x++) {
            if (gBoard.buf[y][x] != gPattern.buf[i][j]) {
                _pl(1, "%d %d", y, x);

                return;
            }

            gState[y][x] = FIXED;
        }
    }
}

int find(int a, int b, int y, int x, int& u, int& v) {
    if (0 <= a && gBoard.buf[a][b] == gBoard.buf[y][x])
        return 1;

    for (int d = 0; d < 4; d++) {
        int i = y + dy[d];
        int j = x + dx[d];

        if (!rng(i, j))
            continue;

        enum State s = gState[i][j];

        if (s != FREE)
            continue;

        gState[i][j] = VISITED;

        if (find(a, b, i, j, u, v)) {
            gPath.add(i, j);

            return 1;
        }

        gState[i][j] = s;
    }

    return 0;
}

void move(int y, int x, int u, int v) {
    if (y < u) {
        for (; y <= u; y++)
            swap(UP);
    } else {
        for (; u <= y; y--)
            swap(DOWN);
    }

    if (x < v) {
        for (; x < v; x++)
            swap(RIGHT);
    } else {
        for (; v < x; x--)
            swap(LEFT);
    }

    gOrigin.dump(__func__);
}

void solve(int board[5][5], int pattern[3][3], int callCntLimit) {
#ifdef _pl_
    extern int orgBoard[5][5];

    gOrigin.init(orgBoard);
#endif

    gBoard.init(board);
    gPattern.init(pattern);

    int y, x, u, v;

    next(y, x);

    move(4, 4, y, x);

    if (!find(y, x, y, x, u, v)) {
        _pl(0, "not found");
        return;
    }

    if (!find(-1, -1, u, v, y, x)) {
        _pl(0, "not found");
        return;
    }

    gPath.dump();

    _pl(0, "end of solve");
}
