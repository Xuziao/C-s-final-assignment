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
int mine[MAPSIZE + 2][MAPSIZE + 2];
bool show[MAPSIZE + 2][MAPSIZE + 2];
int mine_num[MAPSIZE + 2][MAPSIZE + 2];

typedef struct point {
    int x;
    int y;
}point;

typedef struct queue {
    point data;
    struct queue * next;
}queue;

void creat_map();
void play_game();
void clear_screen();
int draw();
void span(point now);
void end_game();
bool check(point *cli);
queue * push(queue * p, point now);
queue * pop(queue * top);
queue * init_queue();

int main() {
    printf("Press Enter\n");
    getchar();
    while (play) {        
        creat_map();
        play_game();
        end_game();
    }
    return 0;
}

void end_game() {
    printf("One more round: Y/N\n");
    while (true) {
        char key = getchar();
        if (key == 'Y') {
            play = 1;
            return;
        }
        if (key == 'N') {
            play = 0;
            return;
        }
    }
}

bool check(point * cli){
    if(cli->x > MAPSIZE || cli->x <= 0 || cli->y > MAPSIZE || cli->y <= 0) { 
        clear_screen();
        printf("Invalid coordinates\n");
        draw();
        scanf("%d%d", &cli->x, &cli->y);
        return 0;
    }
    return 1;
}

void creat_map() {
    for (int i = 0; i < MAPSIZE + 2; i++)
        for (int j = 0; j < MAPSIZE + 2; j++)
            mine[i][j] = 0, show[i][j] = 0, mine_num[i][j] = 0;
    for (int i = 0; i < MAPSIZE + 2; i++) {
        mine_num[0][i] = mine[0][i] = 2;
        mine_num[i][0] = mine[i][0] = 2;
        mine_num[MAPSIZE + 1][i] = mine[MAPSIZE + 1][i] = 2;
        mine_num[i][MAPSIZE + 1] = mine[i][MAPSIZE + 1] = 2;
    }
    srand(time(NULL));
    clear_screen();
    draw();
    point first_click;
    scanf("%d%d", &first_click.x, &first_click.y);
    while(!check(&first_click));
    int counter = 0;
    while (counter < MINENUM) {
        int pos = rand() % (MAPSIZE * MAPSIZE);
        if (mine[pos / MAPSIZE + 1][pos % MAPSIZE + 1] || (pos / MAPSIZE + 1 == first_click.x && pos % MAPSIZE + 1 == first_click.y))
            continue;
        mine[pos / MAPSIZE + 1][pos % MAPSIZE + 1] = true;
        counter++;
    }
    for (int i = 1; i < MAPSIZE + 2; i++)
        for (int j = 1; j < MAPSIZE + 2; j++)
            for (int a = -1; a <= 1; a++)
                for (int b = -1; b <= 1; b++)
                    mine_num[i][j] += (mine[i + a][j + b] == 1 ? 1 : 0);
    span(first_click);
}

void play_game() {
    while (true) {
        clear_screen();
        if(draw() == MINENUM) {
            printf("You win!\n");
            return;
        }
        point click;
        scanf("%d%d", &click.x, &click.y);
        while(!check(&click));
        if (mine[click.x][click.y]) {
            clear_screen();
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
                            printf("x ");
                        else
                            printf("* ");
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

int draw() {
    int counter = 0;
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
                printf("* "), counter++;
        }
        printf("\n");
    }
    printf("Input coordinate x y:");
    return counter;
}

queue * init_queue() {
    queue * init = (queue*)malloc(sizeof(queue));
    init->next = NULL;
    return init;
}

queue * push(queue * p, point now) {
    queue * temp = (queue*)malloc(sizeof(queue));
    temp->data = now;
    temp->next = NULL;
    p->next = temp;
    p = temp;
    return p;
}

queue * pop(queue * top) {
    if (top->next == NULL)
        return NULL;
    queue * temp = top;
    top = top->next;
    free(temp);
    return top;
}

void span(point now) {
    queue * top = init_queue();
    queue * p = top;
    p = push(p, now);
    while(top->next) {
        show[p->data.x][p->data.y] = true;
        top = pop(top);
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++) {
                if (show[top->data.x + i][top->data.y + j])
                    continue;
                if ((!mine[top->data.x + i][top->data.y + j])) {
                    if (mine_num[top->data.x + i][top->data.y + j]) {
                        show[top->data.x + i][top->data.y + j] = true;
                        continue;
                    }
                    point temp;
                    temp.x = top->data.x + i;
                    temp.y = top->data.y + j;
                    p = push(p, temp);
                }
            }
    }    
}