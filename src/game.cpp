#include <iostream>

#include <string> // Controle do console

#include <conio.h>
#include <windows.h>
#include <ctime>

#include "memory_pool.hpp"
#include "register_bank.hpp"

constexpr int WIDTH = 80;
constexpr int HEIGHT = 40;

enum Direction { UP, RIGHT, DOWN, LEFT };

// Endereços na memória
constexpr size_t POS_X = 0;
constexpr size_t POS_Y = 4;
constexpr size_t DIR = 8;
constexpr size_t FRUIT_X = 12;
constexpr size_t FRUIT_Y = 16;
constexpr size_t LENGTH = 20;
constexpr size_t BODY_START = 100; // pares x/y a partir daqui

// Configura o pseudo canva no sonsole
// constexpr int SCREEN_SIZE = (WIDTH + 1) * HEIGHT; old
constexpr int SCREEN_SIZE = WIDTH * HEIGHT;
char screen[SCREEN_SIZE];

// Canva ajustment
void resizeConsole(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Define o tamanho do buffer de tela (sem scroll)
    COORD newSize;
    newSize.X = width;
    newSize.Y = height;
    SetConsoleScreenBufferSize(hOut, newSize);

    // Define o tamanho da janela
    SMALL_RECT displayArea = {
        0, 0,
        static_cast<SHORT>(width - 1),
        static_cast<SHORT>(height - 1)
    };
    SetConsoleWindowInfo(hOut, TRUE, &displayArea);
}


void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void placeFruit(MemoryPool& mem) {
    // int fx = rand() % WIDTH;
    // int fy = rand() % HEIGHT;
    int fx = 1 + rand() % (WIDTH - 2);  // de 1 até WIDTH - 2
    int fy = 1 + rand() % (HEIGHT - 2); // de 1 até HEIGHT - 2
    mem.write(FRUIT_X, fx);
    mem.write(FRUIT_Y, fy);
}

void drawBorders() {
    for (int x = 0; x <= WIDTH; ++x) {
        gotoxy(x, 0); std::cout << "#";
        gotoxy(x, HEIGHT); std::cout << "#";
    }
    for (int y = 0; y <= HEIGHT; ++y) {
        gotoxy(0, y); std::cout << "#";
        gotoxy(WIDTH, y); std::cout << "#";
    }
}

void input(MemoryPool& mem) {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
            case 'w': mem.write(DIR, UP); break;
            case 'd': mem.write(DIR, RIGHT); break;
            case 's': mem.write(DIR, DOWN); break;
            case 'a': mem.write(DIR, LEFT); break;
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
    }

    // salvar corpo atual no pool
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

void draw(MemoryPool& mem) {
    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);
    int fx = mem.read(FRUIT_X);
    int fy = mem.read(FRUIT_Y);
    int length = mem.read(LENGTH);

    system("cls");
    drawBorders();

    // desenha fruta
    gotoxy(fx, fy); std::cout << "F";

    // desenha cabeça
    gotoxy(x, y); std::cout << "O";

    // desenha corpo
    for (int i = 0; i < length; ++i) {
        int bx = mem.read(BODY_START + i * 8);
        int by = mem.read(BODY_START + i * 8 + 4);
        gotoxy(bx, by); std::cout << "o";
    }
}

bool checkCollision(MemoryPool& mem) {
    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);

    if (x <= 0 || x >= WIDTH || y <= 0 || y >= HEIGHT)
        return true;

    int length = mem.read(LENGTH);
    for (int i = 0; i < length; ++i) {
        int bx = mem.read(BODY_START + i * 8);
        int by = mem.read(BODY_START + i * 8 + 4);
        if (x == bx && y == by) return true;
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
        mem.write(LENGTH, len + 1);
        placeFruit(mem);
    }
}

// Gerencia o canva no console
void clearScreenBuffer(char* buffer) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            buffer[y * WIDTH + x] = '.';
        }
    }
}

// Draw convas
void drawBuffered(MemoryPool& mem, char* screen) {
    clearScreenBuffer(screen);

    int x = mem.read(POS_X);
    int y = mem.read(POS_Y);
    int fx = mem.read(FRUIT_X);
    int fy = mem.read(FRUIT_Y);
    int length = mem.read(LENGTH);

    // bordas horizontais
    for (int i = 0; i < WIDTH; ++i) {
        screen[0 * WIDTH + i] = '#';
        screen[(HEIGHT - 1) * WIDTH + i] = '#';
    }

    // bordas verticais
    for (int i = 0; i < HEIGHT; ++i) {
        screen[i * WIDTH + 0] = '#';
        screen[i * WIDTH + (WIDTH - 1)] = '#';
    }

    // fruta
    screen[fy * WIDTH + fx] = 'F';

    // cabeça da cobra
    screen[y * WIDTH + x] = 'O';

    // corpo da cobra
    for (int i = 0; i < length; ++i) {
        int bx = mem.read(BODY_START + i * 8);
        int by = mem.read(BODY_START + i * 8 + 4);
        screen[by * WIDTH + bx] = 'o';
    }

    // desenha tudo de uma vez
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
    
    srand(static_cast<unsigned int>(time(nullptr))); // <--- aqui!

    resizeConsole(WIDTH, HEIGHT);

    RegisterBank regs;
    MemoryPool mem;

    mem.write(POS_X, WIDTH / 2);
    mem.write(POS_Y, HEIGHT / 2);
    mem.write(DIR, RIGHT);
    mem.write(LENGTH, 0);
    placeFruit(mem);

    while (true) {
        input(mem);
        update(regs, mem);
        if (checkCollision(mem)) break;
        checkFruit(mem);
        // draw(mem);
        drawBuffered( mem, screen);
        Sleep(100);
    }

    gotoxy(0, HEIGHT + 2);
    std::cout << "Game Over!";

    return 0;
}
