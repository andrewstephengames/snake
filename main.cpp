#include "snake.h"

int main () {
     int factor = 100;
     Vector2 w = {
          .x = factor*16,
          .y = factor*9,
     };
     srand(clock());
     SetConfigFlags(FLAG_WINDOW_RESIZABLE);
     SetTraceLogLevel(LOG_ERROR);
     InitWindow (w.x, w.y, "Snake");
     Game *game = new Game (Menu, w, {0}, 0);
     SetTargetFPS(60);
     while (!WindowShouldClose()) {
          w.x = GetScreenWidth();
          w.y = GetScreenHeight();
          game->set_canvas (w);
          BeginDrawing();
               game->state_machine();
          EndDrawing();
          if (IsKeyPressed (KEY_Q)) {
               break;
          }
     }
     CloseWindow();
     return 0;
}
