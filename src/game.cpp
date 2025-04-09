#include <iostream>

#include <string>

#include <conio.h>
#include <windows.h>
#include <ctime>

#include "memory_pool.hpp"
#include "register_bank.hpp"

#define H_EDGE '-'
#define V_EDGE '|'

char SNAKE_HEAD = 'O';
char SNAKE_BODY = '0';

char* SOTOPED_MSG = (char*)"Game Over !";

constexpr int WIDTH = 40;
constexpr int HEIGHT = 21;

enum Direction { UP, RIGHT, DOWN, LEFT, ESC };


// MEMORY ADDRESS
constexpr size_t R_SCORE = 0;
constexpr size_t POS_X = 4;         // 0
constexpr size_t POS_Y = 8;         // 4
constexpr size_t DIR = 12;          // 8
constexpr size_t FRUIT_X = 16;      // 12
constexpr size_t FRUIT_Y = 20;      // 16
constexpr size_t LENGTH = 24;       // 20

// PAIRS x/y
constexpr size_t BODY_START = 100; 

// SET CANVA IN CONSOLE
constexpr int SCREEN_SIZE = WIDTH * HEIGHT;
char screen[SCREEN_SIZE];
char running = 1;

// CANVA ADJUSTMENT
void resizeConsole(int width, int height){
    HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );

    // SETS THE SIZE OF THE SCREEN BUFFER (NO SCROLL)
    COORD newSize;
    newSize.X = width;
    newSize.Y = height;
    SetConsoleScreenBufferSize(hOut, newSize);

    // SETS THE SCREEN SIZE
    SMALL_RECT displayArea = {
        0, 0,
        static_cast<SHORT>(width - 1),
        static_cast<SHORT>(height - 1)
    };
    SetConsoleWindowInfo(hOut, TRUE, &displayArea);
}

void PrintNumberToScreen(int x, int y, int value, char* screen, int screenWidth) {
    char str[12];
    int i = 10;
    str[11] = '\0';

    if (value == 0) {
        str[i--] = '0';
    } else {
        while (value > 0 && i >= 0) {
            str[i--] = '0' + (value % 10);
            value /= 10;
        }
    }

    int start = i + 1;
    for (int j = 0; str[start + j] != '\0'; ++j) {
        int index = (y * screenWidth) + x + j;
        screen[index] = str[start + j];
    }
}

void DrawScoreText(char* screen, int screenWidth, MemoryPool& mem ) {
    const char* text = "SCORE:";
    int x = 2;
    int y = 0;

    for (int i = 0; text[i] != '\0'; ++i) {
        int index = (y * screenWidth) + x + i;
        screen[index] = text[i];
    }

    PrintNumberToScreen(x + 7, y, mem.read( R_SCORE ), screen, screenWidth);
}


void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void placeFruit(MemoryPool& mem) {
    int fx = 1 + rand() % (WIDTH - 2);
    int fy = 2 + rand() % (HEIGHT - 3);
    mem.write(FRUIT_X, fx);
    mem.write(FRUIT_Y, fy);
}


void drawBorders() {
    // HORIZONTAL EDGES
    for (int i = 0; i < WIDTH; ++i) {
        screen[1 * WIDTH + i] = H_EDGE;
        screen[(HEIGHT - 1) * WIDTH + i] = H_EDGE;
    }

    // VERTICAL EDGES
    for (int i = 1; i < HEIGHT; ++i) {
        screen[i * WIDTH + 0] = V_EDGE;
        screen[i * WIDTH + (WIDTH - 1)] = V_EDGE;
    }

}


void input(MemoryPool& mem) {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
            case 'w':{
                mem.write(DIR, UP);
                SNAKE_HEAD = 'A';
                break;
            }
            case 'd':{
                mem.write(DIR, RIGHT);
                SNAKE_HEAD = '>';
                break;
            }
            case 's':{
                mem.write(DIR, DOWN);
                SNAKE_HEAD = 'V';
                break;
            }
            case 'a':{
                mem.write(DIR, LEFT);
                SNAKE_HEAD = '<';
                break;
            }
            case 'q': mem.write(DIR, ESC); break;
        }
    }
}

void update(RegisterBank& regs, MemoryPool& mem) {
    int dir = mem.read(DIR);
    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);

    int oldX = x;
    int oldY = y;

    switch (dir) {
        case UP:    y--; break;
        case RIGHT: x++; break;
        case DOWN:  y++; break;
        case LEFT:  x--; break;
        case ESC: running = -1; break;
    }

    // SAVE THE SNAKE BODY ON  MEMORY POOL
    int length = mem.read(LENGTH);
    for (int i = length; i > 0; --i) {
        int prev_x = mem.read(BODY_START + (i - 1) * 8);
        int prev_y = mem.read(BODY_START + (i - 1) * 8 + 4);
        mem.write(BODY_START + i * 8, prev_x);
        mem.write(BODY_START + i * 8 + 4, prev_y);
    }
    mem.write(BODY_START, oldX);
    mem.write(BODY_START + 4, oldY);

    mem.write(POS_X, x);
    mem.write(POS_Y, y);
}


bool checkCollision(MemoryPool& mem) {
    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);
    

    if (x <= 0 || x >= WIDTH-1 || y <= 1 || y >= HEIGHT-1){ 
        SOTOPED_MSG = (char*)"bateu de cara na parede !";
        return true;
    }
    
    int length = mem.read(LENGTH);
    for (int i = 0; i < length; ++i) {
        int bx = mem.read(BODY_START + i * 8);
        int by = mem.read(BODY_START + i * 8 + 4);
        if (x == bx && y == by){
            SOTOPED_MSG = (char*)"Mordeu seu prorio rabo";
            return true;
        }
    }

    return false;
}

void checkFruit(MemoryPool& mem) {
    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);
    int fx = mem.read(FRUIT_X);
    int fy = mem.read(FRUIT_Y);

    if (x == fx && y == fy) {
        int len = mem.read(LENGTH);
        int score = mem.read(R_SCORE);
        mem.write(LENGTH, len + 1);
        mem.write(R_SCORE, score + 1);
        placeFruit(mem);
    }
}

// CLEAR THE BUFFER SCREEN
void clearScreenBuffer(char* buffer) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            buffer[y * WIDTH + x] = ' ';
        }
    }
}

// DRAW CANVAS
void drawBuffered(MemoryPool& mem, char* screen) {
    clearScreenBuffer(screen);

    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);
    int fx = mem.read(FRUIT_X);
    int fy = mem.read(FRUIT_Y);
    int length = mem.read(LENGTH);

    
    // HORIZONTAL EDGES
    drawBorders();
    
    // SNACK
    screen[fy * WIDTH + fx] = 'F';
    
    // HEAD SNAKE
    screen[y * WIDTH + x] = SNAKE_HEAD;
    
    // BODY SNAKE
    for (int i = 0; i < length; ++i) {
        int bx = mem.read(BODY_START + i * 8);
        int by = mem.read(BODY_START + i * 8 + 4);
        screen[by * WIDTH + bx] = SNAKE_BODY;
    }
    
    // DRAW SCORE
    DrawScoreText(screen, 40, mem );
    
    // DRAW ALL
    DWORD charsWritten;
    COORD coord = {0, 0};
    WriteConsoleOutputCharacterA(
        GetStdHandle(STD_OUTPUT_HANDLE),
        screen,
        WIDTH * HEIGHT,
        coord,
        &charsWritten
    );
}


int main() {
    
    srand( static_cast<unsigned int>( time(nullptr) ) ); 

    resizeConsole(WIDTH, HEIGHT);

    RegisterBank regs;
    MemoryPool mem;

    mem.write(POS_X, WIDTH / 2);
    mem.write(POS_Y, HEIGHT / 2);
    mem.write(DIR, RIGHT);
    mem.write(LENGTH, 0);
    placeFruit(mem);



    while (running > 0) {
        input(mem);
        update(regs, mem);        
        if (checkCollision(mem)) break;
        checkFruit(mem);
        drawBuffered( mem, screen);        
        Sleep(100);
    }

    gotoxy(0, HEIGHT + 2);
    std::cout << SOTOPED_MSG;

    return 0;
}
