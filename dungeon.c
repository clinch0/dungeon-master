#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

struct termios oldt;
void reset() { tcsetattr(0, TCSANOW, &oldt); }

void clear() { printf("\033[2J\033[H"); fflush(stdout); }
void move(int x, int y) { printf("\033[%d;%dH", y, x); fflush(stdout); }
void color(int c) { printf("\033[%dm", c); fflush(stdout); }

int px = 5, py = 5, hp = 100, gold = 0, level = 1;
int exit_x, exit_y, monster_x, monster_y, treasure_x, treasure_y;
int map_w = 20, map_h = 15;
char map[20][15];

void gen_level() {
    for (int y = 0; y < map_h; y++)
        for (int x = 0; x < map_w; x++)
            map[x][y] = (x == 0 || x == map_w-1 || y == 0 || y == map_h-1) ? '#' : '.';
    
    exit_x = rand() % (map_w - 2) + 1;
    exit_y = rand() % (map_h - 2) + 1;
    monster_x = rand() % (map_w - 2) + 1;
    monster_y = rand() % (map_h - 2) + 1;
    treasure_x = rand() % (map_w - 2) + 1;
    treasure_y = rand() % (map_h - 2) + 1;
    
    px = map_w / 2; py = map_h / 2;
}

void draw() {
    clear();
    color(33);
    printf("═══ DUNGEON MASTER ═══ Level: %d  HP: %d  Gold: %d\n\n", level, hp, gold);
    color(0);
    
    for (int y = 0; y < map_h; y++) {
        for (int x = 0; x < map_w; x++) {
            if (x == px && y == py) { color(32); printf("@"); color(0); }
            else if (x == exit_x && y == exit_y) { color(36); printf("E"); color(0); }
            else if (x == monster_x && y == monster_y) { color(31); printf("M"); color(0); }
            else if (x == treasure_x && y == treasure_y) { color(33); printf("$"); color(0); }
            else printf("%c", map[x][y]);
        }
        printf("\n");
    }
    
    color(37);
    printf("\n[WASD] Move  [Q]uit  Find exit (E), avoid monsters (M), get gold ($)\n");
    color(0);
    fflush(stdout);
}

int main() {
    srand(time(0));
    tcgetattr(0, &oldt);
    atexit(reset);
    
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    
    gen_level();
    
    while (hp > 0) {
        draw();
        
        char c = getchar();
        int nx = px, ny = py;
        
        if (c == 'q') break;
        if (c == 'w' || c == 'k') ny--;
        if (c == 's' || c == 'j') ny++;
        if (c == 'a' || c == 'h') nx--;
        if (c == 'd' || c == 'l') nx++;
        
        if (map[nx][ny] == '#') continue;
        
        px = nx; py = ny;
        
        if (px == treasure_x && py == treasure_y) {
            gold += 100;
            treasure_x = rand() % (map_w - 2) + 1;
            treasure_y = rand() % (map_h - 2) + 1;
        }
        
        if (px == monster_x && py == monster_y) {
            hp -= 20;
            monster_x = rand() % (map_w - 2) + 1;
            monster_y = rand() % (map_h - 2) + 1;
        }
        
        if (px == exit_x && py == exit_y) {
            level++;
            gen_level();
        }
        
        if (monster_x != px || monster_y != py) {
            if (rand() % 3 == 0) {
                if (monster_x < px) monster_x++;
                if (monster_x > px) monster_x--;
                if (monster_y < py) monster_y++;
                if (monster_y > py) monster_y--;
            }
        }
    }
    
    clear();
    color(31);
    printf("\n\n   GAME OVER!\n\n");
    color(37);
    printf("   Final Level: %d\n", level);
    printf("   Total Gold: %d\n\n", gold);
    color(0);
    
    return 0;
}
