#include "raylib.h"
#include <string.h>
#include <stdio.h>

#define TILE_SIZE 48
#define MAX_W 20
#define MAX_H 20
#define MAX_UNDO 1024

// =================== [ NEW: Button + Homepage System ] ===================
// Button structure (from the second code)
typedef struct {
    Rectangle rect;
    char text[64];
    Color normalColor;
    Color hoverColor;
    Color textColor;
    int id;
} Button;

// Handle a single button (draw + hover + click)
static int HandleButton(Button *btn, Vector2 mousePos, bool mouseClicked) {
    bool isHovered = CheckCollisionPointRec(mousePos, btn->rect);
    Color currentColor = isHovered ? btn->hoverColor : btn->normalColor;

    DrawRectangleRec(btn->rect, currentColor);
    DrawRectangleLinesEx(btn->rect, 2, btn->textColor);

    int textSize = 20;
    int textWidth = MeasureText(btn->text, textSize);
    int textX = (int)(btn->rect.x + (btn->rect.width - textWidth)/2);
    int textY = (int)(btn->rect.y + (btn->rect.height - textSize)/2);
    DrawText(btn->text, textX, textY, textSize, btn->textColor);

    if (isHovered && mouseClicked) return btn->id;
    return -1;
}

// Responsive homepage/menu (with background image)
// CHANGED SIGNATURE: now takes optional bg texture pointer + availability flag.
static int ShowMenuSimple(int currentLevel, int totalLevels, Texture2D *bg, bool hasBg) {
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();

    // ===== NEW: draw background picture (cover the screen, keep aspect) =====
    if (hasBg) {
        float swf = (float)sw, shf = (float)sh;
        float tw = (float)bg->width, th = (float)bg->height;
        float screenAR = swf / shf;
        float texAR    = tw  / th;

        Rectangle src;
        Rectangle dst = {0, 0, swf, shf};

        if (texAR > screenAR) {
            // texture wider than screen -> crop left/right
            float newW = th * screenAR;
            float xOff = (tw - newW) * 0.5f;
            src = (Rectangle){ xOff, 0, newW, th };
        } else {
            // texture taller than screen -> crop top/bottom
            float newH = tw / screenAR;
            float yOff = (th - newH) * 0.5f;
            src = (Rectangle){ 0, yOff, tw, newH };
        }
        DrawTexturePro(*bg, src, dst, (Vector2){0,0}, 0.0f, WHITE);

        // optional: subtle dark overlay for text readability
        DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.25f));
    } else {
        ClearBackground(DARKBLUE);
    }
    // ======================================================================

    // Title
    const char *title = "SOKOBAN";
    int titleSize = 48;
    int tw = MeasureText(title, titleSize);
    DrawText(title, (sw - tw)/2, (int)(sh*0.08f), titleSize, WHITE);

    const char *subtitle = "Enhanced Homepage";
    int subSize = 24;
    int subw = MeasureText(subtitle, subSize);
    DrawText(subtitle, (sw - subw)/2, (int)(sh*0.08f + 40), subSize, LIGHTGRAY);

    char lvlStr[64];
    snprintf(lvlStr, sizeof(lvlStr), "Current Level: %d/%d", currentLevel + 1, totalLevels);
    int lvlSize = 20;
    int lvlw = MeasureText(lvlStr, lvlSize);
    DrawText(lvlStr, (sw - lvlw)/2, (int)(sh*0.08f + 75), lvlSize, YELLOW);

    // Button area
    float btnW = (sw >= 800) ? 400 : sw*0.7f;
    float btnH = 50;
    float startX = (sw - btnW)/2.0f;
    float startY = sh*0.27f;
    float gap = 16;

    Button buttons[5];

    buttons[0] = (Button){
        .rect = {startX, startY + 0*(btnH+gap), btnW, btnH},
        .normalColor = DARKGREEN, .hoverColor = GREEN, .textColor = WHITE, .id = 1
    };
    strcpy(buttons[0].text, "Play Game");

    buttons[1] = (Button){
        .rect = {startX, startY + 1*(btnH+gap), btnW, btnH},
        .normalColor = DARKPURPLE, .hoverColor = PURPLE, .textColor = WHITE, .id = 2
    };
    strcpy(buttons[1].text, "Restart & Play");

    buttons[2] = (Button){
        .rect = {startX, startY + 2*(btnH+gap), btnW, btnH},
        .normalColor = MAROON, .hoverColor = RED, .textColor = WHITE, .id = 3
    };
    strcpy(buttons[2].text, "How to Play");

    buttons[3] = (Button){
        .rect = {startX, startY + 3*(btnH+gap), btnW, btnH},
        .normalColor = ORANGE, .hoverColor = GOLD, .textColor = WHITE, .id = 4
    };
    strcpy(buttons[3].text, "About Us");

    buttons[4] = (Button){
        .rect = {startX, startY + 4*(btnH+gap), btnW, btnH},
        .normalColor = GRAY, .hoverColor = LIGHTGRAY, .textColor = BLACK, .id = 5
    };
    strcpy(buttons[4].text, "Exit Game");

    int selected = -1;
    for (int i = 0; i < 5; i++) {
        int r = HandleButton(&buttons[i], mousePos, mouseClicked);
        if (r != -1) selected = r;
    }

    const char *hint1 = "Click buttons or use number keys (1-5)";
    int h1s = 18, h1w = MeasureText(hint1, h1s);
    DrawText(hint1, (sw - h1w)/2, (int)(startY + 5*(btnH+gap) + 10), h1s, YELLOW);

    const char *hint2 = "ESC: Exit Game";
    int h2s = 16, h2w = MeasureText(hint2, h2s);
    DrawText(hint2, (sw - h2w)/2, (int)(startY + 5*(btnH+gap) + 35), h2s, LIGHTGRAY);

    EndDrawing();

    if (IsKeyPressed(KEY_ONE))  return 1;
    if (IsKeyPressed(KEY_TWO))  return 2;
    if (IsKeyPressed(KEY_THREE))return 3;
    if (IsKeyPressed(KEY_FOUR)) return 4;
    if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_ESCAPE)) return 5;

    return selected;
}

// Simple "How to Play" (from the last code, adapted)
static int ShowHowToPlay() {
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();
    ClearBackground(DARKPURPLE);

    const char *t = "HOW TO PLAY SOKOBAN";
    int ts = 28, tw = MeasureText(t, ts);
    DrawText(t, (sw - tw)/2, (int)(sh*0.08f), ts, WHITE);

    int x0 = (int)(sw*0.12f), y0 = (int)(sh*0.20f);
    DrawText("OBJECTIVE:", x0, y0, 20, YELLOW);
    DrawText("Push all boxes onto target positions", x0+20, y0+28, 18, WHITE);

    y0 += 80;
    DrawText("CONTROLS:", x0, y0, 20, YELLOW);
    DrawText("Arrow Keys - Move player", x0+20, y0+28, 18, WHITE);
    DrawText("U - Undo last move",      x0+20, y0+56, 18, WHITE);
    DrawText("R - Restart level",       x0+20, y0+84, 18, WHITE);
    DrawText("M - Return to menu",      x0+20, y0+112,18, WHITE);
    DrawText("ESC - Exit game",         x0+20, y0+140,18, WHITE);

    // Back button
    Button back = {
        .rect = { (float)(sw/2 - 100), (float)(sh*0.78f), 200, 50 },
        .normalColor = MAROON, .hoverColor = RED, .textColor = WHITE, .id = 1
    };
    strcpy(back.text, "Back to Menu");

    int result = HandleButton(&back, mousePos, mouseClicked);

    EndDrawing();

    if (result == 1 || IsKeyPressed(KEY_ESCAPE) || GetKeyPressed() != 0) return 1;
    return 0;
}

// Simple "About Us" (from the last code, adapted)
static int ShowAboutUs() {
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();
    ClearBackground(MAROON);

    const char *t = "ABOUT SOKOBAN REIMAGINED";
    int ts = 32, tw = MeasureText(t, ts);
    DrawText(t, (sw - tw)/2, (int)(sh*0.08f), ts, WHITE);

    DrawText("Classic puzzle game redesigned", (int)(sw*0.28f), (int)(sh*0.25f), 20, WHITE);
    DrawText("This simple build shows:",        (int)(sw*0.28f), (int)(sh*0.30f), 20, YELLOW);
    DrawText("- Homepage/menu with buttons",    (int)(sw*0.30f), (int)(sh*0.35f), 18, WHITE);
    DrawText("- Undo & Restart",                (int)(sw*0.30f), (int)(sh*0.39f), 18, WHITE);
    DrawText("- PNG textures or colored tiles", (int)(sw*0.30f), (int)(sh*0.43f), 18, WHITE);
    DrawText("- Resizable window + centering",  (int)(sw*0.30f), (int)(sh*0.47f), 18, WHITE);

    DrawText("Built with Raylib",               (int)(sw*0.30f), (int)(sh*0.56f), 18, LIGHTGRAY);

    Button back = {
        .rect = { (float)(sw/2 - 100), (float)(sh*0.78f), 200, 50 },
        .normalColor = DARKGRAY, .hoverColor = GRAY, .textColor = WHITE, .id = 1
    };
    strcpy(back.text, "Back to Menu");

    int result = HandleButton(&back, mousePos, mouseClicked);

    EndDrawing();

    if (result == 1 || IsKeyPressed(KEY_ESCAPE) || GetKeyPressed() != 0) return 1;
    return 0;
}
// ========================================================================

// Tile types enumeration (original)
typedef enum {FLOOR, WALL, TARGET, BOX, BOX_ON_TARGET, PLAYER, PLAYER_ON_TARGET} Tile;

// Sample level (original)
const char *levelMap[] = {
        " ########",
        "##  #   #",
        "#  $.   #",
        "#  .$.$ #",
        "##.$ $.##",
        "# $.$.@ #",
        "#   .$  #",
        "##  #  ##",
        " ####### ",
        NULL // Null-terminated array
};

// Move record for undo stack (original)
typedef struct {
    int px, py;             // player position before move
    int boxMoved;           // 1 if a box was pushed
    int boxX, boxY;         // original box position (if pushed)
    int boxDestX, boxDestY; // new box position (if pushed)
} MoveRecord;

// Level data (original)
typedef struct {
    Tile grid[MAX_H][MAX_W];
    int w, h;
    int px, py;
    int moveCount;

    // undo buffer (legacy, kept)
    Tile undoGrid[MAX_H][MAX_W];
    int undoPx, undoPy;
    int undoMoveCount;

    // new: undo stack
    MoveRecord undoStack[MAX_UNDO];
    int undoTop;
} Level;

// Init level (original)
void LoadLevel(Level *lvl, const char **src) {
    memset(lvl, 0, sizeof(Level));
    lvl->moveCount = 0;
    lvl->undoTop = 0;

    int rows = 0;
    int len = (int)strlen(src[0]);
    while (src[rows]) rows++;
    if (rows > MAX_H) rows = MAX_H;
    if (len  > MAX_W) len  = MAX_W;

    lvl->h = rows;
    lvl->w = len;

    for (int r=0; r<rows; r++) {
        for (int x=0; x<len; x++) {
            char c = src[r][x];
            switch(c){
                case '#': lvl->grid[r][x] =     WALL; break;
                case '.': lvl->grid[r][x] =     TARGET; break;
                case '$': lvl->grid[r][x] =     BOX; break;
                case '@': lvl->grid[r][x] =     PLAYER; lvl->px = x; lvl->py = r; break;
                case '*': lvl->grid[r][x] =     BOX_ON_TARGET; break;
                case '+': lvl->grid[r][x] =     PLAYER_ON_TARGET; lvl->px = x; lvl->py = r; break;
                default : lvl->grid[r][x] =     FLOOR; break;
            }
        }
    }
}

// Push move to undo stack (original)
void PushUndo(Level *lvl, int px, int py, int boxMoved, int boxX, int boxY, int boxDestX, int boxDestY) {
  if (lvl->undoTop < MAX_UNDO) {
        MoveRecord *m = &lvl->undoStack[lvl->undoTop++];
        m->px = px; m->py = py;
        m->boxMoved = boxMoved;
        m->boxX = boxX; m->boxY = boxY;
        m->boxDestX = boxDestX; m->boxDestY = boxDestY;
    }
}

// Revert last move using undo stack (original)
void UndoMove(Level *lvl) {
    if (lvl->undoTop == 0) return;

    MoveRecord m = lvl->undoStack[--lvl->undoTop];

    // Restore current player tile to floor/target
    if (lvl->grid[lvl->py][lvl->px] == PLAYER_ON_TARGET) lvl->grid[lvl->py][lvl->px] = TARGET;
    else lvl->grid[lvl->py][lvl->px] = FLOOR;

    // Restore player position
    lvl->px = m.px; lvl->py = m.py;

    // Set player tile back
    if (lvl->grid[m.py][m.px] == TARGET) lvl->grid[m.py][m.px] = PLAYER_ON_TARGET;
    else lvl->grid[m.py][m.px] = PLAYER;

    // If a box was moved in that move, restore it
    if (m.boxMoved) {
        Tile *from = &lvl->grid[m.boxDestY][m.boxDestX];
        Tile *to   = &lvl->grid[m.boxY][m.boxX];

        if (*from == BOX_ON_TARGET) *from = TARGET;
        else *from = FLOOR;

        if (*to == TARGET) *to = BOX_ON_TARGET;
        else *to = BOX;
    }

    if (lvl->moveCount > 0) lvl->moveCount--;
}

// Check if level is solved (original)
bool LevelSolved(Level *lvl) {
    for (int y = 0; y < lvl->h; y++) {
        for (int x = 0; x < lvl->w; x++) {
            if (lvl->grid[y][x] == BOX) return false;
        }
    }
    return true;
}

// TryMove (original)
void TryMove(Level *lvl, int dx, int dy) {
    int nx = lvl->px + dx, ny = lvl->py + dy;

    // Check bounds
    if (nx < 0 || ny < 0 || nx >= lvl->w || ny >= lvl->h) return;
    // Check for walls
    Tile dest = lvl->grid[ny][nx];
    if (dest == WALL) return;

    // Handle box pushing
    if (dest == BOX || dest == BOX_ON_TARGET) { // pushing a box
        int nnx = nx + dx, nny = ny + dy;
        if (nnx < 0 || nny < 0 || nnx >= lvl->w || nny >= lvl->h) return;
        Tile next = lvl->grid[nny][nnx];
        if (next != FLOOR && next != TARGET) return; // can't push

        // RECORD UNDO
        PushUndo(lvl, lvl->px, lvl->py, 1, nx, ny, nnx, nny);

        // move box
        lvl->grid[nny][nnx] = (next == TARGET) ? BOX_ON_TARGET : BOX;
        lvl->grid[ny][nx] = (dest == BOX_ON_TARGET) ? TARGET : FLOOR;
    } 
    else if (dest == FLOOR || dest == TARGET) {
        // RECORD UNDO: normal move (no box moved)
        PushUndo(lvl, lvl->px, lvl->py, 0, 0, 0, 0, 0);
    } else return; // blocked

    Tile oldPlayerTile = lvl->grid[lvl->py][lvl->px];
    lvl->grid[lvl->py][lvl->px] = (oldPlayerTile == PLAYER_ON_TARGET) ? TARGET : FLOOR;
    lvl->px = nx;
    lvl->py = ny;
    Tile underlying = lvl->grid[ny][nx]; // what is there now
    lvl->grid[ny][nx] = (underlying == TARGET) ? PLAYER_ON_TARGET : PLAYER; // store the right state
    lvl->moveCount++;
}

int main(void) {
    // === (MINIMAL CHANGE) make window resizable ===
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    // Initialization
    InitWindow(640, 480, "Sokoban - PNG Textures (with Homepage)");
    SetTargetFPS(60);

    /* Load PNG textures - ensure these files exist in "assets/" */
    Texture2D tex[7];
    tex[0] = LoadTexture("assets/wall.png");
    tex[1] = LoadTexture("assets/floor.png");
    tex[2] = LoadTexture("assets/target.png");
    tex[3] = LoadTexture("assets/box.png");
    tex[4] = LoadTexture("assets/box_on_target.png");
    tex[5] = LoadTexture("assets/player.png");
    tex[6] = LoadTexture("assets/player.png"); // used for on-target player

    bool texturesOk=true;
    for (int i = 0; i < 7; i++){
        if (tex[i].id==0) texturesOk=false;
    }

    // ===== NEW: load homepage background image =====
    Texture2D menuBg = LoadTexture("assets/background.png");
    bool menuBgOk = (menuBg.id != 0);
    // ===============================================

    Level lvl; 
    LoadLevel(&lvl, levelMap);

    // === [NEW] Simple game mode state for homepage system ===
    // 0 = menu, 1 = playing, 3 = (unused here), 4 = how-to, 5 = about
    int gameMode = 0;
    int currentLevel = 0;   // single level
    int totalLevels  = 1;

    // Main game loop
    while (!WindowShouldClose()) {

        if (gameMode == 0) {
            // Homepage/menu (pass background)
            int choice = ShowMenuSimple(currentLevel, totalLevels, &menuBg, menuBgOk);
            if (choice == 1) {
                // Play
                LoadLevel(&lvl, levelMap);
                gameMode = 1;
            } else if (choice == 2) {
                // Restart & Play
                LoadLevel(&lvl, levelMap);
                gameMode = 1;
            } else if (choice == 3) {
                // How to Play
                gameMode = 4;
            } else if (choice == 4) {
                // About Us
                gameMode = 5;
            } else if (choice == 5) {
                break; // Exit
            }
            continue;
        }

        if (gameMode == 4) {
            // How to Play screen
            int back = ShowHowToPlay();
            if (back) gameMode = 0;
            continue;
        }

        if (gameMode == 5) {
            // About screen
            int back = ShowAboutUs();
            if (back) gameMode = 0;
            continue;
        }

        // =============== Playing mode (original logic kept) ===============
        // Input controls
        if (IsKeyPressed(KEY_R)) LoadLevel(&lvl, levelMap);
        if (IsKeyPressed(KEY_U)) { UndoMove(&lvl); }
        if (IsKeyPressed(KEY_UP))    TryMove(&lvl, 0, -1);
        if (IsKeyPressed(KEY_DOWN))  TryMove(&lvl, 0, 1);
        if (IsKeyPressed(KEY_LEFT))  TryMove(&lvl, -1, 0);
        if (IsKeyPressed(KEY_RIGHT)) TryMove(&lvl, 1, 0);
        if (IsKeyPressed(KEY_M))     gameMode = 0;    // [NEW] return to menu
        if (IsKeyPressed(KEY_ESCAPE)) break;          // exit game

        // === (MINIMAL ADD) compute dynamic tile size + centered offsets ===
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        // Leave room for your existing top text area (approx 100 px) and bottom (approx 60 px)
        int topMargin = 80;
        int sideMargin = 150; // to mimic your original 150 x-offset look
        int bottomMargin = 60;

        int maxBoardW = screenW - sideMargin*2;
        int maxBoardH = screenH - (topMargin + bottomMargin);

        // Fit tiles to window
        int tileSize = maxBoardW / (lvl.w > 0 ? lvl.w : 1);
        int tileSizeH = maxBoardH / (lvl.h > 0 ? lvl.h : 1);
        if (tileSizeH < tileSize) tileSize = tileSizeH;
        if (tileSize < 8) tileSize = 8; // clamp for visibility

        // Center board using your style (base at 150,80 in original)
        int boardW = tileSize * lvl.w;
        int boardH = tileSize * lvl.h;
        int offsetX = (screenW - boardW)/2;     // center horizontally
        int offsetY = topMargin + (maxBoardH - boardH)/2; // roughly keep the top area

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the level title and instructions (kept as-is)
        DrawText("SOKOBAN - PNG Textures", 170, 20, 20, DARKBLUE);
        DrawText("Arrow keys to move | R to restart | U to undo | M menu | ESC quit", 70, 50, 16, GRAY);
        // Show move count
        char moveStr[32];
        snprintf(moveStr, sizeof(moveStr), "Moves: %d", lvl.moveCount);
        DrawText(moveStr, 70, 70, 18, MAROON);

        // Draw level tiles (only changed TILE_SIZE -> tileSize and DrawTexture -> DrawTexturePro)
        for (int y=0; y<lvl.h;y++) {
            for (int x=0;x<lvl.w;x++) {
                int sx = offsetX + x * tileSize;
                int sy = offsetY + y * tileSize;
                Tile t=lvl.grid[y][x];

                if (texturesOk) {
                    Texture2D *ptx = NULL;
                    switch (t) {
                        case WALL:             ptx = &tex[0]; break;
                        case FLOOR:            ptx = &tex[1]; break;
                        case TARGET:           ptx = &tex[2]; break;
                        case BOX:              ptx = &tex[3]; break;
                        case BOX_ON_TARGET:    ptx = &tex[4]; break;
                        case PLAYER:           ptx = &tex[5]; break;
                        case PLAYER_ON_TARGET: ptx = &tex[6]; break;
                        default: break;
                    }
                    if (ptx) {
                        Rectangle src = {0, 0, (float)ptx->width, (float)ptx->height};
                        Rectangle dst = {(float)sx, (float)sy, (float)tileSize, (float)tileSize};
                        DrawTexturePro(*ptx, src, dst, (Vector2){0,0}, 0.0f, WHITE);
                    } else {
                        DrawRectangle(sx, sy, tileSize, tileSize, LIGHTGRAY);
                    }
                } else {
                    Color c;
                    switch (t) {
                        case WALL: c = DARKGRAY; break;
                        case FLOOR: c = LIGHTGRAY; break;
                        case TARGET: c = RED; break;
                        case BOX: c = ORANGE; break;
                        case BOX_ON_TARGET: c = GREEN; break;
                        case PLAYER: c = BLUE; break;
                        case PLAYER_ON_TARGET: c = SKYBLUE; break;
                        default: c = LIGHTGRAY; break;
                    }
                    DrawRectangle(sx, sy, tileSize, tileSize, c);
                }
                //DrawRectangleLines(sx, sy, tileSize, tileSize, BLACK);
            }
        }

        // Check if level solved (kept)
        if (LevelSolved(&lvl))
            DrawText("LEVEL CLEARED!", 230, 420, 24, GREEN);

        EndDrawing();
    }

    // Cleanup textures
    for (int i = 0; i < 7; i++) if (tex[i].id) UnloadTexture(tex[i]);
    // NEW: unload menu background
    if (menuBg.id) UnloadTexture(menuBg);

    CloseWindow();
    return 0;
}
