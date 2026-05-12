#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// ============ 常量定义 ============
#define mapWidth 80
#define mapHeight 25
#define TRUE 1
#define FALSE 0

typedef int BOOL;

// ============ 结构体定义 ============
typedef struct {
    float x, y;
    int width, height;
    float vertSpeed;
    float horizSpeed;
    BOOL IsFly;
    char cType;         // 'B'=砖块, '?'=问号砖, '*'=过关砖, 'E'=敌人
} TObject;

// ============ 全局变量 ============
char map[mapHeight][mapWidth + 1];
TObject mario;
TObject *brick = NULL;
int brickLength = 0;
TObject *moving = NULL;
int movingLength = 0;
int level = 1;

// ============ 函数声明 ============
void setCur(int x, int y);
void InitObject(TObject *obj, int x, int y, int w, int h, char type);
void ClearMap();
void PutObjectOnMap(TObject obj);
void ShowMap();
BOOL IsCollision(TObject o1, TObject o2);
void VertMoveObject(TObject *obj);
void HorizonMoveObject(TObject *obj);
void HorizonMoveMap(int dx);
void MarioCollision();
void CreateLevel(int lvl);
TObject *GetNewMoving();

// ============ 设置光标位置 ============
void setCur(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// ============ 初始化对象 ============
void InitObject(TObject *obj, int x, int y, int w, int h, char type) {
    obj->x = x;
    obj->y = y;
    obj->width = w;
    obj->height = h;
    obj->vertSpeed = 0;
    obj->horizSpeed = 0;
    obj->IsFly = FALSE;
    obj->cType = type;
}

// ============ 清空地图 ============
void ClearMap() {
    for (int j = 0; j < mapHeight; j++) {
        for (int i = 0; i < mapWidth; i++) {
            map[j][i] = ' ';
        }
    }
}

// ============ 把对象画到地图上 ============
void PutObjectOnMap(TObject obj) {
    int x = (int)obj.x;
    int y = (int)obj.y;
    for (int j = 0; j < obj.height; j++) {
        for (int i = 0; i < obj.width; i++) {
            int mx = x + i;
            int my = y + j;
            if (mx >= 0 && mx < mapWidth && my >= 0 && my < mapHeight) {
                map[my][mx] = obj.cType;
            }
        }
    }
}

// ============ 显示地图 ============
void ShowMap() {
    map[mapHeight - 1][mapWidth - 1] = '\0';
    for (int j = 0; j < mapHeight; j++) {
        printf("%s\n", map[j]);
    }
}

// ============ AABB碰撞检测 ============
BOOL IsCollision(TObject o1, TObject o2) {
    return ((o1.x + o1.width) > o2.x) &&
           (o1.x < (o2.x + o2.width)) &&
           ((o1.y + o1.height) > o2.y) &&
           (o1.y < (o2.y + o2.height));
}

// ============ 垂直移动（重力+碰撞） ============
void VertMoveObject(TObject *obj) {
    obj->IsFly = TRUE;
    obj->vertSpeed += 0.05f;
    obj->y += obj->vertSpeed;

    // 检测与所有砖块的碰撞
    for (int i = 0; i < brickLength; i++) {
        if (IsCollision(*obj, brick[i])) {
            // 撞到了，回退
            obj->y -= obj->vertSpeed;
            obj->vertSpeed = 0;
            obj->IsFly = FALSE;

            // 检查是否碰到过关砖块 '*'
            if (brick[i].cType == '*') {
                level++;
                if (level > 3) level = 1;
                system("color 27");
                Sleep(500);
                CreateLevel(level);
            }
            break;
        }
    }

    // 掉落到底部 -> 重置关卡
    if (obj->y > mapHeight) {
        CreateLevel(level);
        Sleep(1000);
    }
}

// ============ 敌人水平移动 ============
void HorizonMoveObject(TObject *obj) {
    obj->x += obj->horizSpeed;

    // 检测与砖块的碰撞
    for (int i = 0; i < brickLength; i++) {
        if (IsCollision(*obj, brick[i])) {
            obj->x -= obj->horizSpeed;
            obj->horizSpeed = -obj->horizSpeed;
            return;
        }
    }

    // 检测会不会掉下去（脚下没砖）
    TObject tmp = *obj;
    tmp.y += 1;
    tmp.IsFly = TRUE;
    VertMoveObject(&tmp);
    if (tmp.IsFly == TRUE) {
        obj->x -= obj->horizSpeed;
        obj->horizSpeed = -obj->horizSpeed;
    }
}

// ============ 地图卷轴 ============
void HorizonMoveMap(int dx) {
    mario.x += dx;
    for (int i = 0; i < brickLength; i++) {
        brick[i].x -= dx;
    }
    for (int i = 0; i < movingLength; i++) {
        moving[i].x -= dx;
    }
}

// ============ 马里奥碰敌人 ============
void MarioCollision() {
    for (int i = 0; i < movingLength; i++) {
        if (IsCollision(mario, moving[i])) {
            if (mario.vertSpeed > 0) {
                // 踩死敌人
                moving[i].cType = ' ';
            } else {
                // 被敌人碰到 -> 重置
                CreateLevel(level);
                Sleep(1000);
            }
        }
    }
}

// ============ 获取新敌人槽位 ============
TObject *GetNewMoving() {
    movingLength++;
    moving = (TObject*)realloc(moving, sizeof(TObject) * movingLength);
    return &moving[movingLength - 1];
}

// ============ 创建关卡 ============
void CreateLevel(int lvl) {
    // 初始化马里奥
    InitObject(&mario, 39, 10, 3, 3, 'B');

    if (lvl == 1) {
        brickLength = 6;
        brick = (TObject*)realloc(brick, sizeof(TObject) * brickLength);
        InitObject(&brick[0], 20, 20, 40, 5, 'B');
        InitObject(&brick[1], 60, 15, 10, 10, 'B');
        InitObject(&brick[2], 80, 20, 20, 5, 'B');
        InitObject(&brick[3], 120, 15, 10, 10, '?');
        InitObject(&brick[4], 150, 20, 40, 5, '*');
        InitObject(&brick[5], 210, 15, 10, 10, 'B');

        movingLength = 1;
        moving = (TObject*)realloc(moving, sizeof(TObject) * movingLength);
        InitObject(&moving[0], 25, 10, 3, 2, 'E');
        moving[0].horizSpeed = 0.3f;
    }
    else if (lvl == 2) {
        brickLength = 7;
        brick = (TObject*)realloc(brick, sizeof(TObject) * brickLength);
        InitObject(&brick[0], 10, 22, 10, 3, 'B');
        InitObject(&brick[1], 30, 18, 20, 3, 'B');
        InitObject(&brick[2], 60, 15, 15, 3, 'B');
        InitObject(&brick[3], 90, 12, 30, 3, 'B');
        InitObject(&brick[4], 130, 18, 10, 3, '?');
        InitObject(&brick[5], 160, 20, 40, 3, 'B');
        InitObject(&brick[6], 220, 15, 10, 3, '*');

        movingLength = 2;
        moving = (TObject*)realloc(moving, sizeof(TObject) * movingLength);
        InitObject(&moving[0], 35, 10, 3, 2, 'E');
        moving[0].horizSpeed = 0.3f;
        InitObject(&moving[1], 95, 10, 3, 2, 'E');
        moving[1].horizSpeed = -0.3f;
    }
    else {
        brickLength = 5;
        brick = (TObject*)realloc(brick, sizeof(TObject) * brickLength);
        InitObject(&brick[0], 15, 20, 20, 3, 'B');
        InitObject(&brick[1], 50, 15, 10, 3, '?');
        InitObject(&brick[2], 80, 12, 30, 3, 'B');
        InitObject(&brick[3], 130, 18, 30, 3, 'B');
        InitObject(&brick[4], 190, 15, 10, 3, '*');

        movingLength = 2;
        moving = (TObject*)realloc(moving, sizeof(TObject) * movingLength);
        InitObject(&moving[0], 55, 10, 3, 2, 'E');
        moving[0].horizSpeed = 0.4f;
        InitObject(&moving[1], 135, 10, 3, 2, 'E');
        moving[1].horizSpeed = -0.4f;
    }
}

// ============ 主函数 ============
int main(int argc, char *argv[]) {
    int lvl = 1;
    int maxLevel = 3;

    // 解析命令行参数
    if (argc >= 2) {
        lvl = atoi(argv[1]);
    }
    if (argc >= 3) {
        maxLevel = atoi(argv[2]);
    }
    if (lvl < 1 || lvl > maxLevel) {
        lvl = 1;
    }

    CreateLevel(lvl);

    // 游戏主循环
    do {
        ClearMap();

        // 跳跃
        if ((mario.IsFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) {
            mario.vertSpeed = -1.0f;
        }
        // 左移
        if (GetKeyState('A') < 0) {
            HorizonMoveMap(-1);
        }
        // 右移
        if (GetKeyState('D') < 0) {
            HorizonMoveMap(1);
        }

        // 物理更新
        VertMoveObject(&mario);
        MarioCollision();

        // 更新敌人
        for (int i = 0; i < movingLength; i++) {
            VertMoveObject(&moving[i]);
            HorizonMoveObject(&moving[i]);
        }

        // 渲染
        for (int i = 0; i < brickLength; i++) {
            PutObjectOnMap(brick[i]);
        }
        for (int i = 0; i < movingLength; i++) {
            PutObjectOnMap(moving[i]);
        }
        PutObjectOnMap(mario);

        setCur(0, 0);
        ShowMap();

        Sleep(10);
    } while (GetKeyState(VK_ESCAPE) >= 0);

    // 释放内存
    free(brick);
    free(moving);

    return 0;
}