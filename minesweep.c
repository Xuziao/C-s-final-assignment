#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef __WIN32__
sys = 0;
#elif __linux__
sys = 1;
#elif __APPLE__
sys = 2;
#else
printf("操作系统未知\n");
exit(1);
#endif

#define MINENUM 10
#define MAPSIZE 9

bool play = 1;
bool mine[MAPSIZE + 2][MAPSIZE + 2];
bool show[MAPSIZE + 2][MAPSIZE + 2];
int mine_num[MAPSIZE + 2][MAPSIZE + 2];

typedef struct point {
    int x;
    int y;
}point;
typedef struct stack {
    point data;
    struct stack * next;
}stack;

void creat_map();
void play_game();
void clear_screen();
void draw();
void span(point now);
bool check(point *cli);
stack * push(stack * p, point now);
stack * pop(stack * p);

int main() {
    printf("Press Enter\n");
    getchar();
    while (play) {        
        creat_map();
        play_game();
        printf("One more round: Y/N\n");
        while (true) {
            char key = getchar();
            if (key == 'Y')
                break;
            if (key == 'N') {
                play = 0;
                break;
            }
        }
    }
    return 0;
}

bool check(point * cli){
    if(cli->x > MAPSIZE || cli->x < 0 || cli->y > MAPSIZE || cli->y < 0) {
        scanf("%d%d", &cli->x, &cli->y);
        return 0;
    }
    return 1;
}

void creat_map() {
    for (int i = 0; i <= MAPSIZE + 2; i++)
        for (int j = 0; j <= MAPSIZE + 2; j++)
            mine[i][j] = 0, show[i][j] = 0, mine_num[i][j] = 0;
    for (int i = 0; i <= MAPSIZE; i++)
        show[0][i] = 1, show[i][0] = 1, show[MAPSIZE + 2][i] = 1, show[i][MAPSIZE + 2] = 1;
    srand(time(NULL));
    clear_screen();
    draw();
    point first_click;
    scanf("%d%d", &first_click.x, &first_click.y);
    while(!check(&first_click));
    int counter = 0;
    while (counter < MINENUM) {
        int pos = rand() % (MAPSIZE * MAPSIZE) + 1;
        if (mine[pos / MAPSIZE][pos % MAPSIZE] || (pos / MAPSIZE == first_click.x && pos % MAPSIZE == first_click.y))
            continue;
        mine[pos / MAPSIZE][pos % MAPSIZE] = true;
        counter++;
    }
    for (int i = 0; i <= MAPSIZE + 2; i++)
        for (int j = 0; j <= MAPSIZE + 2; j++)
            for (int a = -1; a <= 1; a++)
                for (int b = -1; b <= 1; b++)
                    mine_num[i][j] += mine[i + a][j + b];
    span(first_click);
}

void play_game() {
    while (true) {
        clear_screen();
        draw();
        point click;
        scanf("%d%d", &click.x, &click.y);
        while(!check(&click));
        if (mine[click.x][click.y]) {
            printf("Game Over\n");
            printf("0  ");
            for (int i = 1; i <= MAPSIZE; i++)
                printf("%d ", i);
            printf("\n\n");
            for (int i = 1; i <= MAPSIZE; i++) {
                printf("%d  ", i);
                for (int j = 1; j <= MAPSIZE; j++) {
                    if (show [i][j])
                        printf("%d ", mine_num[i][j]);
                    else
                        if (mine[i][j]) 
                            printf("x  ");
                        else
                            printf("*  ");
                }
                printf("\n");
            }
            return;
        }
        span(click);
    }
}

void clear_screen() {
    switch (sys) {
    case 0:
        system("cls");
        break;
    case 1:
        system("clear");
        break;
    case 2:
        system("clear");
        break;
    default:
        break;
    }
}
void draw() {
    printf("0  ");
    for (int i = 1; i <= MAPSIZE; i++)
        printf("%d ", i);
    printf("\n\n");
    for (int i = 1; i <= MAPSIZE; i++) {
        printf("%d  ", i);
        for (int j = 1; j <= MAPSIZE; j++) {
            if (show [i][j])
                printf("%d ", mine_num[i][j]);
            else
                printf("* ");
        }
        printf("\n");
    }
    printf("Input coordinate x y：");
}

stack * push(stack * p, point now) {
    stack * temp = (stack*)malloc(sizeof(stack));
    temp->data = now;
    temp->next = p;
    p = temp;
    return p;
}

stack * pop(stack *p) {
    if (p) {
        stack *temp = p;
        p = p->next;
        free(temp);
    }
    return p;
}

void span(point now) {
    stack * p =NULL;
    p = push(p, now);
    while(p) {
        show[p->data.x][p->data.y] = true;
        stack q = *p;
        p = pop(p);
        for (int i = -1; i < 1; i++)
            for (int j = -1; j < 1; j++)
                if (!mine[q.data.x + i][q.data.y + j]){
                    point temp = q.data;
                    temp.x = q.data.x + i;
                    temp.y = q.data.y + j;
                    p = push(p, temp);
                }
    }    
}