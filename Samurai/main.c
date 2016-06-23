#include <allegro.h>
#include <time.h>

int max(int a, int b)
{
    if(a >= b)
        return a;
    else
        return b;
}
int min(int a, int b)
{
    if(a <= b)
        return a;
    else
        return b;
}
int pixel_perfect_colision(int x1, int y1, BITMAP* objt1 , int x2, int y2, BITMAP* objt2)
{
    int i, j;
    int colisao = FALSE;

    if(!((x1 > x2 + objt2->w) || (y1 > y2 + objt2->h) || (x2 > x1 + objt1->w) || (y2 > y1 + objt1->h)))
    {
        int cima = max(y1, y2);
        int baixo = min(y1+objt1->h, y2+objt2->h);
        int esquerda = max(x1, x2);
        int direita = min(x1+objt1->w, x2+objt2->w);

        for(i=cima; i<baixo && !colisao; i++)
        {
            for(j=esquerda; j<direita && !colisao; j++)
            {
                if(getpixel(objt1, j-x1, i-y1) != makecol(255, 0, 255) && getpixel(objt2, j-x2, i-y2) != makecol(255, 0, 255));
                colisao = TRUE;
            }
        }
    }
  return colisao;
}


volatile int exit_game;
void close_game()
{
    exit_game = TRUE;
}
END_OF_FUNCTION(close_game)

volatile int milisegundos;

void mse_counter()
{
    milisegundos++;
}
END_OF_FUNCTION(mse_counter)

volatile int ticks;

void tick_counter()
{
    ticks++;
}
END_OF_FUNCTION(tick_counter)

volatile int timer;

void incrementa_timer()
{
    timer++;
}
END_OF_FUNCTION(incrementa_timer)

volatile int exit_game = FALSE;
int screen_state;
enum {GMENU, SGAME, GOVER};

void game_menu()
{
    ///sons
    SAMPLE* musica_menu = load_sample("sons/musicas/musica_menu.wav");
    SAMPLE* cursor = load_sample("sons/efeitos/cursor.wav");
    SAMPLE* select = load_sample("sons/efeitos/select.wav");

    ///bitmaps
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* fundo_menu = load_bitmap("sprites/fundo_menu.bmp", NULL);
    BITMAP* iniciar = load_bitmap("sprites/iniciar.bmp", NULL);
    BITMAP* sair = load_bitmap ("sprites/sair.bmp", NULL);
    BITMAP* seta = load_bitmap("sprites/seta.bmp", NULL);

    ///variaveis
    int num_frames = 4;
    int frame_atual = 0;
    int tempo_troca = 100;
    float seta_x = 380;
    float seta_y = 220;
    int a;
    int exit_screen = FALSE;

    play_sample(musica_menu, 255, 128, 1000, TRUE);

    while(!exit_game && !exit_screen)
        {
            while (ticks > 0 && !exit_screen && !exit_game)
            {
            //IMPUT
            keyboard_input();

            if(key[KEY_ESC])
                exit_game = TRUE;
            if(apertou(KEY_DOWN))
            {
                if(seta_y < 301)
                {
                    play_sample(cursor, 255, 128, 1000, FALSE);
                    seta_y = seta_y + 130;
                }
            }
            if(apertou(KEY_UP))
            {
                if(seta_y > 301)
                {
                    play_sample(cursor, 255, 128, 1000, FALSE);
                    seta_y = seta_y - 130;
                }
            }
            if(key[KEY_ENTER] && seta_y == 220)
            {
               play_sample(select, 255, 128, 1000, FALSE);
               exit_screen = TRUE;
               screen_state = SGAME;

            }
            if(key[KEY_ENTER] && seta_y == 350)
            {
                close_game();
            }

        //UPDATE

            frame_atual = (milisegundos / tempo_troca) % num_frames;

        //DRAW

            draw_sprite(buffer, fundo_menu, 0, 0);
            draw_sprite(buffer, iniciar, 450, 180);
            draw_sprite(buffer, sair, 450, 300);
            draw_sprite(buffer, seta, seta_x, seta_y);
            draw_sprite(screen, buffer, 0, 0);

            ticks--;

            }
        }

        stop_sample(musica_menu);

    //FINALIZAÇÃO

    destroy_bitmap(buffer);
    destroy_bitmap(fundo_menu);
    destroy_bitmap(seta);
    destroy_bitmap(iniciar);
    destroy_bitmap(sair);
}
END_OF_FUNCTION(game_menu)
void init()
{
    allegro_init();
    install_timer();
    install_keyboard();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    set_window_title("Samurai");
    set_close_button_callback(close_game);

    LOCK_VARIABLE(exit_game);
    LOCK_FUNCTION(close_game);

    ticks = 0;
    LOCK_FUNCTION(tick_counter);
    LOCK_VARIABLE(ticks);
    install_int_ex(tick_counter, BPS_TO_TIMER(60));

    milisegundos = 0;
    LOCK_FUNCTION(mse_counter);
    LOCK_VARIABLE(milisegundos);
    install_int_ex(mse_counter, MSEC_TO_TIMER(1));
    screen_state = GMENU;
}
END_OF_FUNCTION(init)

void start_game()
{
    ///SONS
    SAMPLE* musica_fase1 = load_sample("sons/musicas/musica_fase1.wav");
    SAMPLE* select = load_sample("sons/efeitos/select.wav");
    SAMPLE* attacksd = load_sample("sons/efeitos/attack.wav");
    SAMPLE* dano = load_sample("sons/efeitos/dano.wav");

    ///bitmaps
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* fundo_floresta = load_bitmap("sprites/fundo_floresta.bmp", NULL);
    BITMAP* terra = load_bitmap("sprites/terra.bmp", NULL);
    BITMAP* agua = load_bitmap("sprites/agua.bmp", NULL);
    BITMAP* madeira = load_bitmap("sprites/madeira.bmp", NULL);
    BITMAP* pedra = load_bitmap("sprites/pedra.bmp", NULL);
    BITMAP* pedra2 = load_bitmap("sprites/pedra2.bmp", NULL);
    BITMAP* heart = load_bitmap("sprites/life.bmp", NULL);
    BITMAP* samurai = load_bitmap("sprites/samurai.bmp", NULL);
    BITMAP* samurai_aux2 = load_bitmap("sprites/samurai2.bmp", NULL);
    BITMAP* samurai_aux = load_bitmap("sprites/samurai.bmp", NULL);
    BITMAP* samu = load_bitmap("sprites/samu.bmp", NULL);
    BITMAP* samu2 = load_bitmap("sprites/samu2.bmp", NULL);
    BITMAP* bird1 = load_bitmap("sprites/bird1.bmp", NULL);
    BITMAP* life[3] = {TRUE, TRUE, TRUE};
    BITMAP* mapa[16] = {3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3};

    ///VAR

    int num_frames = 5;
    int num_frames_bird = 5;
    int frame_atual = 0;
    int frame_atual_bird = 0;
    int tempo_troca = 100;
    int frame_w = samu->w / num_frames;
    int frame_w_bird = bird1->w/ num_frames_bird;
    int i;
    int exit_screen = FALSE;
    int aux_sprite = 0;
    int samu_x = 400;
    int samu_y = 340;
    int bird1_x = 0;
    int bird1_y = 310;
    int k;
    int cont=0;
    int pulo = FALSE;
    int vel = 1;
    int cont2 = 0;
    int pts = 0;

    play_sample(musica_fase1, 255, 128, 1000, TRUE);

    ///LOOP
     while(!exit_game && !exit_screen)
        {
            while (ticks > 0 && !exit_game && !exit_screen)
            {
                 //IMPUT
                   keyboard_input();
                      if(segurou(KEY_RIGHT))
                   {
                       samurai = samu;
                       aux_sprite = frame_w * frame_atual;
                       samu_x++;
                   }
                        else if(soltou(KEY_RIGHT))
                   {
                       samurai = samurai_aux;
                       aux_sprite = 0;
                   }
                       else if(segurou(KEY_LEFT))
                   {
                       samurai = samu2;
                       aux_sprite = frame_w * frame_atual;
                       samu_x--;
                   }
                       else if(soltou(KEY_LEFT))
                   {
                       samurai = samurai_aux2;
                       aux_sprite = 0;
                   }
                      else if(apertou(KEY_UP))
                      {
                          if(samu_y==340)
                          {
                              play_sample(attacksd, 255, 128, 1000, FALSE);
                              pulo = TRUE;
                          }
                      }
                   if(key[KEY_TAB])
                   {
                       play_sample(select, 255, 128, 1000, FALSE);
                       exit_screen = TRUE;
                       screen_state = GMENU;
                   }
                   if(key[KEY_ESC])
                      exit_game = TRUE;

                   ///UPDATE
                  frame_atual = (milisegundos / tempo_troca) % num_frames;
                  frame_atual_bird = (milisegundos / tempo_troca) % num_frames_bird;

                  if(bird1_x == samu_x)
                  {
                    play_sample(dano, 255,128, 1000, FALSE);
                    cont++;
                    pts = pts - 10;
                  }
                  if(cont >= 3)
                  {
                      exit_screen = TRUE;
                      screen_state = GOVER;
                  }
                  bird1_x = bird1_x + vel;

                  if(bird1_x>800)
                  {
                      bird1_x = 0;
                      cont2++;
                      pts = pts + 10;
                  }

                  if(cont2==10)
                  {
                      vel++;
                      cont2=0;
                  }

                  if(vel==5)
                    vel=1;
                    if(pulo==TRUE && samu_y>170)
                    {
                        samu_y = samu_y-2;

                    } else pulo = FALSE;
                     if(pulo==FALSE && samu_y<340)
                    {
                        samu_y = samu_y+2;
                    }

                 ///DRAW
                 int life_y = 20;
                 int life_x = 20;
                 draw_sprite(buffer, fundo_floresta, 0, 0);
                 for(k=cont; k<3; k++)
                 {
                     if(life[k] == TRUE)
                     {
                         draw_sprite(buffer, heart, life_x, life_y);
                         life_x = life_x + 50;
                     }
                 }
                 int mapa_x = 0;
                 for(i=0; i<16; i++)
                      {
                          if(mapa[i] == 1)
                              draw_sprite(buffer, terra, mapa_x, 500);
                               if(mapa[i] == 2)
                                 draw_sprite(buffer, pedra, mapa_x, 500);
                                   if(mapa[i] == 3)
                                      draw_sprite(buffer, agua, mapa_x, 550);
                                         if(mapa[i] == 4)
                                            draw_sprite(buffer, pedra2, mapa_x, 400);
                                            mapa_x = mapa_x+50;
                      }
                      if(samu_x<150 || samu_x>600)
                      {
                          samu_y++;
                      }
                      if(samu_y>600)
                      {
                          exit_screen = TRUE;
                          screen_state = GOVER;
                      }
                      masked_blit(samurai, buffer, aux_sprite, 0, samu_x, samu_y, frame_w, samurai->h);
                      masked_blit(bird1, buffer, frame_w_bird * frame_atual_bird, 0, bird1_x, bird1_y, frame_w_bird, bird1->h);

                      draw_sprite(screen, buffer, 0, 0);
             }
        }
        stop_sample(musica_fase1);

    ///final

    destroy_bitmap(bird1);
    destroy_bitmap(samu2);
    destroy_bitmap(samu);
    destroy_bitmap(buffer);
    destroy_bitmap(terra);
    destroy_bitmap(pedra);
    destroy_bitmap(agua);
    destroy_bitmap(madeira);
    destroy_bitmap(samurai);
    destroy_bitmap(fundo_floresta);
    destroy_bitmap(pedra2);
}
END_OF_FUNCTION(start_game)

void gameover()
{
    BITMAP* gameover_img = load_bitmap("sprites/gameover.bmp", NULL);
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    SAMPLE* gameovertheme = load_sample("sons/musicas/gameovertheme.wav");
    SAMPLE* select = load_sample("sons/efeitos/select.wav");
    play_sample(gameovertheme, 255, 128, 1000, TRUE);

    int exit_screen = FALSE;
    while(!exit_game && !exit_screen)
    {
        keyboard_input();
        if(key[KEY_ENTER])
                   {
                       play_sample(select, 255, 128, 1000, FALSE);
                       exit_screen = TRUE;
                       screen_state = GMENU;
                   }

           draw_sprite(buffer, gameover_img, 0, 0);
           draw_sprite(screen, buffer, 0, 0);
    }
    stop_sample(gameovertheme);

     destroy_bitmap(gameover_img);
     destroy_bitmap(buffer);
}

int main()
{
    init();
    while(!exit_game)
    {
        if(screen_state == GMENU)
            game_menu();
        else if(screen_state == SGAME)
            start_game();
        else if(screen_state == GOVER)
                gameover();
    }
    return 0;
}

END_OF_MAIN()
