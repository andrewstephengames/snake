#ifndef SNAKE_H_
#define SNAKE_H_

#include <ctime>
#include <vector>

using std::vector;

#include "menu.h"

class Game {
     private:
          GameState game_state;
          Vector2 canvas;
          Texture2D background;
          int score;
     public:
          vector<Element> e;
          Game (GameState game_state, Vector2 canvas, Texture2D background, int score) {
               this->game_state = game_state;
               this->canvas = canvas;
               this->background = background;
               this->score = score;
          }
          ~Game() {
               UnloadTexture (background);
               e.clear();
          }
          GameState get_game_state () {
               return game_state;
          }
          void set_game_state (GameState game_state) {
               this->game_state = game_state;
          }
          Vector2 get_canvas () {
               return canvas;
          }
          void set_canvas (Vector2 canvas) {
               this->canvas = canvas;
          }
          int get_score () {
               return score;
          }
          void set_score (int score) {
               this->score = score;
          }
          Texture2D get_background() {
               return background;
          }
          void state_machine ();
};

class Snake {
     private:
          Rectangle rect;
          Texture2D texture;
     public:
          Snake (Rectangle rect, char *file_name) {
               this->rect = rect;
               //DrawRectangleRec (rect, GREEN);
               Vector2 pos = {rect.x, rect.y};
               texture = LoadTexture (file_name);
          }
          Texture2D get_texture () {
               return texture;
          }
          ~Snake() {
               UnloadTexture(texture);
          }
          Rectangle get_rect () {
               return rect;
          }
          void handle_keys (Vector2 canvas);
};

class Fruit {
     private:
          Rectangle rect;
          Texture2D texture;
          int points;
     public:
          Fruit (char *file_name) {
               texture = LoadTexture (file_name);
               points = 0;
          }
          ~Fruit() {
               UnloadTexture (texture);
          }
          Rectangle get_rect() {
               return rect;
          }
          void set_rect() {
               this->rect = rect;
          }
          Texture2D get_texture () {
               return texture;
          }
          void set_points (int points) {
               this->points = points;
          }
          int get_points () {
               return points;
          }
          void generate (Vector2 canvas);
};

#endif // SNAKE_H_

void Fruit::generate(Vector2 canvas) {
     float margin = canvas.x/60;
     Vector2 canvas2 = {
          .x = canvas.x-margin,
          .y = canvas.y-margin,
     };
     rect.x = rand()%(int)canvas2.x;
     rect.y = rand()%(int)canvas2.y;
}

void Snake::handle_keys (Vector2 canvas) {
     float margin = canvas.x/60;
     if (IsKeyPressed (KEY_W) || IsKeyPressed (KEY_UP)) {
          rect.y -= margin;
          printf ("%d was pressed\n", KEY_W);
     }
     if (IsKeyPressed (KEY_A) || IsKeyPressed (KEY_LEFT)) {
          rect.x -= margin;
          printf ("%d was pressed\n", KEY_A);
     }
     if (IsKeyPressed (KEY_S) || IsKeyPressed (KEY_DOWN)) {
          rect.y += margin;
          printf ("%d was pressed\n", KEY_S);
     }
     if (IsKeyPressed (KEY_D) || IsKeyPressed (KEY_RIGHT)) {
          rect.x += margin;
          printf ("%d was pressed\n", KEY_D);
     }
}

void Game::state_machine () {
     switch (Game::get_game_state()) {
          case Menu: {
               ClearBackground (RAYWHITE);
               draw_background (Game::get_canvas(), 255, Game::get_background());
               Vector2 canvas = {
                    .x = Game::get_canvas().x,
                    .y = Game::get_canvas().y/2,
               };
               Element e1 = {
                    .box = {
                         .x = canvas.x,
                         .y = canvas.y,
                         .width = 100,
                         .height = 100,
                    },
                    .font_size = canvas.x/15,
                    .bg = WHITE,
                    .fg = BLACK,
                    .texture = {0},
               };
               strcpy (e1.label, "Snake");
               e.push_back (e1);
               bool use_texture = false;
               bool center = true;
               draw_label (&e.back(), use_texture, center, canvas);
               /*
               canvas.y = Game::get_canvas().y + Game::get_canvas().y/6;
               strcpy (e->label, "Play");
               Game::set_game_state (draw_button (e, use_texture, true, canvas, Start, Menu));
               canvas.y = Game::get_canvas().y + Game::get_canvas().y/6;
               strcpy (e->label, "Quit");
               Game::set_game_state (draw_button (e, use_texture, true, canvas, End, Menu));
               */
          } break;
          case Start: {
               /*
               Vector2 canvas = Game::get_canvas();
               draw_background (canvas, 255, Game::get_background());
               strcpy (e.label, "Score");
               draw_label (&e.back(), false, false, canvas);
               e->box = {
                    .x = rand()%(int)canvas.x,
                    .y = rand()%(int)canvas.y,
                    .width = 32,
                    .height = 32,
               };
               Snake *snake = new Snake (e->box, NULL);
               if (snake->get_texture().id > 0) {
                    DrawTextureRec (snake->get_texture(), e->box, rec_to_v(e->box), WHITE);
               } else {
                    DrawRectangleRec (e->box, WHITE);
               }
               snake->handle_keys(canvas);
               */
               if (IsKeyPressed (KEY_Q)) {
                    Game::set_game_state (End);
               }
          } break;
          case End: {
               //CloseWindow();
               //exit(EXIT_SUCCESS);
          } break;
          default: {
               fprintf (stderr, "Unknown game state\n");
               exit(EXIT_FAILURE);
          }
     }
}
