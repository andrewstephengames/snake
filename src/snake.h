#ifndef SNAKE_H_
#define SNAKE_H_

#include <ctime>
#include <vector>
#include "menu.h"

using std::vector;

typedef enum {
     Title,
     Button1,
     Button2,
     Score,
} ui_element;

class Snake {
     private:
          Rectangle rect;
          Texture2D texture;
          float speed;
          bool keys[512];
     public:
          Snake (Rectangle rect, char *file_name, float speed) {
               this->rect = rect;
               texture = LoadTexture (file_name);
               this->speed = speed;
          }
          Texture2D get_texture () {
               return texture;
          }
          ~Snake() {
               if (texture.id > 0) {
                    UnloadTexture(texture);
               }
          }
          Rectangle get_rect () {
               return rect;
          }
          float get_speed () {
               return speed;
          }
          void set_speed (float speed) {
               this->speed = speed;
          }
          void unload_keys ();
          void handle_keys (Vector2 canvas);
};

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
               if (background.id > 0) {
                    UnloadTexture (background);
               }
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
          void state_machine (Snake *snake);
          void mouse_on_element (Element *e, GameState current_state, GameState prev_state);
          void draw_button (Element *e, bool texture, bool center, Vector2 canvas, GameState current_state, GameState prev_state);
          void print_game_state();
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
               if (texture.id > 0) {
                    UnloadTexture (texture);
               }
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
     rect.x = (float)GetRandomValue (margin, canvas2.x);
     rect.y = (float)GetRandomValue (margin, canvas2.y);
     rect.width = margin;
     rect.height = margin;
}

void Snake::unload_keys () {
     for (int i = 0; i < 512; ++i) {
          Snake::keys[i] = false;
     }
}

void Snake::handle_keys (Vector2 canvas) {
     Vector2 dir = {1, 1};
     float margin = canvas.x/60;
     if (IsKeyPressed (KEY_W)) {
          Snake::unload_keys();
          Snake::keys[KEY_W] = true;
     }
     if (IsKeyPressed (KEY_A)) {
          Snake::unload_keys();
          Snake::keys[KEY_A] = true;
     }
     if (IsKeyPressed (KEY_S)) {
          Snake::unload_keys();
          Snake::keys[KEY_S] = true;
     }
     if (IsKeyPressed (KEY_D)) {
          Snake::unload_keys();
          Snake::keys[KEY_D] = true;
     }
     if (rect.x > canvas.x-margin) {
          rect.x = margin;
     }
     if (rect.x < margin) {
          rect.x = canvas.x-margin;
     }
     if (rect.y > canvas.y-margin) {
          rect.y = margin;
     }
     if (rect.y < margin) {
          rect.y = canvas.y-margin;
     }
     if (Snake::keys[KEY_W]) {
          rect.y -= margin*Snake::speed;
     }
     if (Snake::keys[KEY_A]) {
          rect.x -= margin*Snake::speed;
     }
     if (Snake::keys[KEY_S]) {
          rect.y += margin*Snake::speed;
     }
     if (Snake::keys[KEY_D]) {
          rect.x += margin*Snake::speed;
     }
}

void Game::print_game_state() {
     switch (Game::get_game_state()) {
          case 0: {
               printf ("Menu\n");
          } break;
          case 1: {
               printf ("Start\n");
          } break;
          case 2: {
               printf ("End\n");
          } break;
          default:
               printf ("Unknown\n");
     }
}

void Game::mouse_on_element (Element *e, GameState current_state, GameState prev_state) {
     Vector2 mouse = GetMousePosition();
     if (CheckCollisionPointRec (mouse, e->box)) {
          e->fg = LIME;
          e->bg = WHITE;
          if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT)) {
               Game::set_game_state (current_state);
          }
     } else {
          e->fg = BLACK;
          e->bg = WHITE;
     }
}

void Game::draw_button (Element *e, bool texture, bool center, Vector2 canvas, GameState current_state, GameState prev_state) {
     if (center) {
          center_element (e, canvas);
     }
     e->box.width = MeasureText(e->label, e->font_size);
     e->box.height = e->font_size;
     if (texture) {
          DrawTextureRec (e->texture, e->box, rec_to_v(e->box), e->bg);
     } else {
          float thick = e->box.x/100;
          Rectangle temp = {
               .x = e->box.x - thick*3,
               .y = e->box.y - thick*3,
               .width = e->box.width + thick*5,
               .height = e->box.height + thick*5,
          };
          DrawRectangleLinesEx (temp, thick, e->fg);
     }
     DrawText (e->label, e->box.x, e->box.y, e->font_size, e->fg);
     mouse_on_element (e, current_state, prev_state);
}

void Game::state_machine (Snake *snake) {
     bool use_texture, center;
     Vector2 w = Game::get_canvas();
     switch (Game::get_game_state()) {
          case Menu: {
               draw_background (w, 255, Game::get_background());
               use_texture = false;
               center = true;
               Vector2 canvas = {
                    .x = w.x,
                    .y = w.y/2,
               };
               draw_label (&Game::e.at(Title), use_texture, center, canvas);

               use_texture = false;
               center = true;
               canvas.y = w.y;
               Game::draw_button (&Game::e.at(Button1), use_texture, center, canvas, Start, Menu);

               use_texture = false;
               center = true;
               canvas.y = w.y + w.y/2;
               Game::draw_button (&Game::e.at(Button2), use_texture, center, canvas, End, Menu);
               if (IsKeyPressed (KEY_Q)) {
                    Game::set_game_state (End);
               }
          } break;
          case Start: {
               draw_background (w, 255, Game::get_background());
               if (IsKeyPressed (KEY_Q)) {
                    Game::set_game_state (End);
               }
               center = false;
               use_texture = false;
               sprintf (Game::e.at(Score).label, "Score: %d", Game::get_score());
               draw_label (&Game::e.at(Score), center, use_texture, w);
               if (snake->get_texture().id > 0) {
                    DrawTextureRec (snake->get_texture(), snake->get_rect(), rec_to_v(snake->get_rect()), WHITE);
               } else {
                    DrawRectangleRec (snake->get_rect(), GREEN);
               }
               snake->handle_keys(w);
               if (IsKeyPressed (KEY_R)) {
                    Game::set_game_state (Menu);
               }
          } break;
          case End: {
               draw_background (canvas, 255, Game::get_background());
               exit(EXIT_SUCCESS);
          } break;
          default: {
               fprintf (stderr, "Unknown game state\n");
               exit(EXIT_FAILURE);
          }
     }
}
