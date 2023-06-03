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
printf("Unknown system.\n");
exit(1);
#endif

bool play = 1;
int mapsize;
int minenum;
int arr_size;

typedef struct point {
    int x;
    int y;
    int z;
}point;

typedef struct queue {
    point data;
    struct queue * next;
}queue;

void creat_map(int *mine, int *mine_num, int *flags, bool *show);
void play_game(int *mine, int *mine_num, bool *show, bool *flags);
void clear_screen();
int draw(int *mine_num, bool *show, bool *flags);
void span(point now, int *mine, int *mine_num, bool *show, bool *flags);
void end_game();
bool check(point *cli, int *mine_num, bool *show, bool *flags);
queue * push(queue * p, point now);
queue * pop(queue * top);
queue * init_queue();

int main() {
    printf("Press Enter\n");
    getchar();
    printf("Please enter mapsize n*n and the number of mines\n n = ");
    scanf("%d", &mapsize);
    arr_size = mapsize + 2;
    printf("the number of mine = ");
    scanf("%d",& minenum);
    int mine[arr_size][arr_size];
    bool show[arr_size][arr_size];
    int mine_num[arr_size][arr_size];
    bool flags[arr_size][arr_size];
    while (play) {        
        creat_map(mine, mine_num, flags, show);
        play_game(mine, mine_num, show, flags);
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

bool check(point * cli,int *mine_num, bool *show, bool *flags){
    if(cli->z != 1 || cli->z != 0) {
        clear_screen();
        printf("Please choose whether set a flag.\n");
        draw(mine_num, show, flags);
        scanf("%d%d%d", &cli->x, &cli->y, &cli->z);
        return 0;
    }
    if(cli->x > mapsize || cli->x <= 0 || cli->y > mapsize || cli->y <= 0) { 
        clear_screen();
        printf("Invalid coordinates\n");
        draw(mine_num, show, flags);
        scanf("%d%d%d", &cli->x, &cli->y, &cli->z);
        return 0;
    }
    else if(show[arr_size * cli->x + cli->y]){
        clear_screen();
        printf("You have already clicked that area.\n");
        draw(mine_num, show, flags);
        scanf("%d%d%d", &cli->x, &cli->y, &cli->z);
        return 0;
    }
    return 1;
}

void creat_map(int *mine, int *mine_num, int *flags, bool *show) {
    for (int i = 0; i < arr_size; i++)
        for (int j = 0; j < arr_size; j++)
            mine[i * arr_size + j] = 0, show[i * arr_size + j] = 0, mine_num[i * arr_size + j] = 0, flags[i * arr_size + j] = 0;
    for (int i = 0; i < arr_size; i++) {
        mine_num[i] = mine[i] = 2;
        mine_num[i * arr_size] = mine[i * arr_size] = 2;
        mine_num[(mapsize + 1) * arr_size + i] = mine[(mapsize + 1) * arr_size + i] = 2;
        mine_num[i * arr_size + mapsize + 1] = mine[i * arr_size + mapsize + 1] = 2;
    }
    srand(time(NULL));
    clear_screen();
    draw(mine_num, show, flags);
    point first_click;
    scanf("%d%d", &first_click.x, &first_click.y);
    while(!check(&first_click, mine_num, show, flags));
    int counter = 0;
    while (counter < minenum) {
        int pos = rand() % (mapsize * mapsize);
        if (mine[(pos / mapsize + 1) * arr_size + pos % mapsize + 1] || (pos / mapsize + 1 == first_click.x && pos % mapsize + 1 == first_click.y))
            continue;
        mine[(pos / mapsize + 1) * arr_size + pos % mapsize + 1] = true;
        counter++;
    }
    for (int i = 1; i < arr_size; i++)
        for (int j = 1; j < arr_size; j++)
            for (int a = -1; a <= 1; a++)
                for (int b = -1; b <= 1; b++)
                    mine_num[i * arr_size + j] += (mine[(i + a) * arr_size + j + b] == 1 ? 1 : 0);
    span(first_click, mine, mine_num, show, flags);
}

void play_game(int *mine, int *mine_num, bool *show, bool *flags) {
    while (true) {
        clear_screen();
        if(draw(mine_num, show, flags) == minenum) {
            printf("You win!\n");
            return;
        }
        point click;
        scanf("%d%d%d", &click.x, &click.y, &click.z);
        while(!check(&click, mine_num, show, flags));
        flags[click.x * arr_size + click.y] = click.z;
        if (mine[click.x * arr_size + click.y]) {
            clear_screen();
            printf("Game Over\n");
            printf("0  ");
            for (int i = 1; i <= mapsize; i++)
                printf("%d ", i);
            printf("\n\n");
            for (int i = 1; i <= mapsize; i++) {
                printf("%d  ", i);
                for (int j = 1; j <= mapsize; j++) {
                    if (show [i * arr_size + j])
                        printf("%d ", mine_num[i * arr_size + j]);
                    else
                        if (mine[i * arr_size + j]) 
                            printf("x ");
                        else
                            printf("* ");
                }
                printf("\n");
            }
            return;
        }
        span(click, mine, mine_num, show, flags);
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

int draw(int *mine_num, bool *show, bool *flags) {
    int counter = 0;
    printf("0  ");
    for (int i = 1; i <= mapsize; i++)
        printf("%d ", i);
    printf("\n\n");
    for (int i = 1; i <= mapsize; i++) {
        printf("%d  ", i);
        for (int j = 1; j <= mapsize; j++) {
            if (show [i * arr_size + j])
                printf("%d ", mine_num[i * arr_size + j]);
            else if (flags[i * arr_size + j])
                printf("! ");
            else
                printf("* "), counter++;
        }
        printf("\n");
    }
    printf("x y stand for coordinates, z is whether put a flag. 1 for set a flag, 0 for not.\n");
    printf("Input x y z:");
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

void span(point now, int *mine, int *mine_num, bool *show, bool *flags) {
    queue * top = init_queue();
    queue * p = top;
    p = push(p, now);
    while(top->next) {
        show[p->data.x * arr_size + p->data.y] = true;
        top = pop(top);
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++) {
                if (show[(top->data.x + i) * arr_size + top->data.y + j])
                    continue;
                if (flags[(top->data.x + i) * arr_size + top->data.y + j])
                if ((!mine[(top->data.x + i) * arr_size + top->data.y + j])) {
                    if (mine_num[(top->data.x + i) * arr_size + top->data.y + j]) {
                        show[(top->data.x + i) * arr_size + top->data.y + j] = true;
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