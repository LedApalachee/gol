#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

int fieldSizeX;
int fieldSizeY;
bool** field;

int CursorPosX;
int CursorPosY;

void init_field(int sx, int sy)
{
    fieldSizeX = sx;
    fieldSizeY = sy;
    field = new bool*[sx];
    for (int x = 0; x < sx; ++x)
        field[x] = new bool[sy];
    for (int x = 0; x < sx; ++x)
        for (int y = 0; y < sy; ++y)
            field[x][y] = false;
}

bool getCell(int posX, int posY)
{
    if (posX < 0 && posY >= 0 && posY < fieldSizeY) return getCell(fieldSizeX+posX, posY);
    if (posX >= 0 && posX < fieldSizeX && posY < 0) return getCell(posX, fieldSizeY+posY);
    if (posX >= fieldSizeX && posY >= 0 && posY < fieldSizeY) return getCell(posX-fieldSizeX, posY);
    if (posX >= 0 && posX < fieldSizeX && posY >= fieldSizeY) return getCell(posX, posY-fieldSizeY);
    if (posX < 0 && posY < 0) return getCell(fieldSizeX+posX, fieldSizeY+posY);
    if (posX >= fieldSizeX && posY >= fieldSizeY) return getCell(posX-fieldSizeX, posY-fieldSizeY);
    if (posX >= fieldSizeX && posY < 0) return getCell(posX-fieldSizeX, posY+fieldSizeY);
    if (posX < 0 && posY >= fieldSizeY) return getCell(posX+fieldSizeX, posY-fieldSizeY);
    return field[posX][posY];
}

int getAround(int posX, int posY)
{
    int result = 0;
    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y)
            if (getCell(posX+x, posY+y) && !(x == 0 && y == 0)) ++result;
    return result;
}

int rendersizeX;
int rendersizeY;
char* render;

void init_render()
{
    rendersizeX = fieldSizeX + 3;
    rendersizeY = fieldSizeY + 2;
    render = new char[rendersizeX * rendersizeX + 1];

    for (int x = 0; x < rendersizeX-1; ++x)
        render[x] = render[rendersizeX * rendersizeY - x - 2] = '_';
    for (int y = 0; y < rendersizeY; ++y)
    {
        render[y * rendersizeX] = render[y * rendersizeX + rendersizeX-2] = '|';
        render[y * rendersizeX + rendersizeX-1] = '\n';
    }
    render[0] = render[rendersizeX-2] = ' ';
    render[rendersizeX * rendersizeX] = '\0';
}

void update_render()
{
    for (int x = 0; x < fieldSizeX; ++x)
        for (int y = 0; y < fieldSizeY; ++y)
            field[x][y] ? render[(y+1) * rendersizeX + (x+1)] = 'O' : render[(y+1) * rendersizeX + (x+1)] = ' ';
    render[(CursorPosY+1) * rendersizeX + (CursorPosX+1)] = '?';
}

void Logic()
{
    bool nextgen[fieldSizeX][fieldSizeY];
    for (int x = 0; x < fieldSizeX; ++x)
        for (int y = 0; y < fieldSizeY; ++y)
            if ((!field[x][y] && getAround(x,y) == 3) || (field[x][y] && (getAround(x,y) == 3 || getAround(x,y) == 2))) 
                nextgen[x][y] = true;
            else nextgen[x][y] = false;
    for (int x = 0; x < fieldSizeX; ++x)
        for (int y = 0; y < fieldSizeY; ++y)
            field[x][y] = nextgen[x][y];
}

void clear()
{
    for (int i = 0; i < fieldSizeX; ++i)
        for (int j = 0; j < fieldSizeY; ++j)
            field[i][j] = false;
}

void showInfo()
{
    system("cls");
    std::cout << "wasd - move cursor\n" <<
                  "j - put alive cell\n" <<
                  "k - erase alive cell\n" <<
                  "c - clear field\n" <<
                  "p - pause/run\n" <<
                  "n - next turn (during the pause)\n" <<
                  "q - quit\n";
    getch();
    system("cls");
}

int main(int argc, char** argv)
{
    system("cls");
    bool pause = true;

    if (argc >= 3)
    {
        init_field(std::atoi(argv[1]), std::atoi(argv[2]));
    }
    else
    {
        init_field(40, 40);
    }

    CursorPosX = fieldSizeX/2;
    CursorPosY = fieldSizeY/2;

    init_render();
    while(true)
    {
        update_render();
        std::cout << render;
        if (pause) std::cout << "\nPAUSED.\n";
        if (kbhit() || pause)
        {
            switch (getch())
            {
                case 's':
                    if (CursorPosY < fieldSizeY-1) ++CursorPosY;
                    else CursorPosY = 0;
                    break;
                case 'w':
                    if (CursorPosY > 0) --CursorPosY;
                    else CursorPosY = fieldSizeY-1;
                    break;
                case 'a':
                    if (CursorPosX > 0) --CursorPosX;
                    else CursorPosX = fieldSizeX-1;
                    break;
                case 'd':
                    if (CursorPosX < fieldSizeX-1) ++CursorPosX;
                    else CursorPosX = 0;
                    break;
                case 'j':
                    field[CursorPosX][CursorPosY] = true;
                    break;
                case 'p':
                    pause = !pause;
                    break;
                case 'c':
                    clear();
                    break;
                case 'k':
                    field[CursorPosX][CursorPosY] = false;
                    break;
                case 'n':
                    if (pause) Logic();
                    break;
                case 'i':
                    showInfo();
                    break;
                case 'q':
                    return 0;
            }
        }
        if (!pause) Logic();
        if (!pause) Sleep(75);
        system("cls");
    }

    delete[] render;
    for (int x = 0; x < fieldSizeX; ++x)
        delete[] field[x];
    delete[] field;
    return 0;
}