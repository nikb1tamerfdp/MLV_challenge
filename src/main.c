#define _DEFAULT_SOURCE

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../includes/util/misc.h"
#include "../includes/util/error.h"
#include "../includes/game.h"
#include "../includes/interface.h"
#include "../includes/acquisition.h"
#include "../includes/util/str.h"
#include "../includes/util/list.h"
#include "../includes/util/matrix.h"
#include "../includes/ui/drawables/drawable.h"
#include "../includes/ui/scene.h"
#include "../includes/ui/drawables/shapes.h"
#include "../includes/ui/drawables/ellipse.h"
#include "../includes/ui/drawables/sprite.h"

/* void set_space(Space *adrr); */
/* void initialise_space(Space *S); */

int main(int argc, char** argv)
{

    Scene scene = SCENE_NEW(String_new("My Scene"));
    
    TreeNode root = TREE_ROOT(SCENE_TREE(scene));
    SceneNode root_node = SCENE_NODE_NEW(String_new("My Root"), root);
    TREE_NODE_CONTENT(root) = root_node;

    TreeNode root2 = NEW(_TreeNode);
    root2 -> children = LIST_NEW();
    root2 -> container = SCENE_TREE(scene);
    root2 -> parent = root;
    ListItem item = LIST_APPEND(TREE_NODE_CHILDREN(root), root2);
    SceneNode node = SCENE_NODE_NEW(String_new("My Scene Node"), root2);
    SCENE_NODE_CONTAINER(node) = LIST_ITEM_CONTENT_AS(item, TreeNode);
    TREE_NODE_CONTENT(SCENE_NODE_CONTAINER(node)) = node;

    MLV_create_window("Test", NULL, 800, 600);

    /*MLV_Image* texture = MLV_load_image("img/starship_green.png");
    Rect rect = { { 0., 0. }, { MLV_get_image_width(texture), MLV_get_image_height(texture) } };
    Drawable d = _Sprite_as_drawable(texture, rect);*/
    Vector2 radius = { 100, 30 };
    ShapeDisplay display = { FALSE, MLV_COLOR_RED, { 1, MLV_COLOR_RED, BORDER_OUTSIDE } };
    Drawable d = _Ellipse_as_drawable(radius, display);
    d -> node = node;
    LIST_APPEND(SCENE_NODE_DRAWABLES(node), d);

    double px[4] = { -100., 100., 100., -100. };
    double py[4] = { -30., -30., 30., 30. };

    double time_last_frame, absolute_time, to_wait;
    time_last_frame = get_absolute_time();
    int a = 90;
    int delta = 4;

    /*TRANSFORM_SET_ANCHOR_XY(SCENE_NODE_LOCAL_TRANSFORM(node), 0, 0);*/
    /*TRANSFORM_SET_SCALE_XY_FROM_XY(SCENE_NODE_LOCAL_TRANSFORM(node), 1, 2, -100, -100);*/
    TRANSFORM_SET_SCALE_XY(SCENE_NODE_LOCAL_TRANSFORM(node), 1, 1);

    while (TRUE)
    {

        a += delta;
        if (a >= 360) a = 0;

        MLV_clear_window(MLV_COLOR_BLACK);
        
        /* board */
        TRANSFORM_SET_POS_XY(SCENE_NODE_LOCAL_TRANSFORM(root_node), 400, 300);
        TRANSFORM_SET_SCALE_XY(SCENE_NODE_LOCAL_TRANSFORM(root_node), 1, 1);
        TRANSFORM_SET_ROT(SCENE_NODE_LOCAL_TRANSFORM(node), a);
        
        Vector2 movement = VEC_NIL;
        if (MLV_get_keyboard_state(MLV_KEYBOARD_z) == MLV_PRESSED)
            movement.y += -1;
        if (MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED)
            movement.y += 1;
        if (MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED)
            movement.x += -1;
        if (MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED)
            movement.x += 1;
        if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED)
        {
            delta++;
            if (delta > 100) delta = 100;
        }
        if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) != MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_RELEASED)
        {
            if (delta > 4) delta--;
            if (delta < 4) delta++;
        }
        if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED)
        {
            delta--;
            if (delta < -100) delta = -100;
        }
        if (MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED)
            delta = 4;
        
        VECTOR_NORMALIZE(movement);
        TRANSFORM_MOVE_XY(SCENE_NODE_LOCAL_TRANSFORM(node), movement.x, movement.y);

        Transform global = SCENE_NODE_GLOBAL_TRANSFORM(node);
        Point pp[4] = {
            TRANSFORM_APPLY_XY(global, px[0], py[0]),
            TRANSFORM_APPLY_XY(global, px[1], py[1]),
            TRANSFORM_APPLY_XY(global, px[2], py[2]),
            TRANSFORM_APPLY_XY(global, px[3], py[3])
        };
        int ppx[4] = { pp[0].x, pp[1].x, pp[2].x, pp[3].x };
        int ppy[4] = { pp[0].y, pp[1].y, pp[2].y, pp[3].y };
        MLV_draw_polygon(ppx, ppy, 4, MLV_COLOR_WHITE);
        TRANSFORM_FREE(global);
        SCENE_DRAW(scene);

        /* mini-map */
        TRANSFORM_SET_POS_XY(SCENE_NODE_LOCAL_TRANSFORM(root_node), 600, 100);
        TRANSFORM_SET_SCALE_XY(SCENE_NODE_LOCAL_TRANSFORM(root_node), .1, .1);
        SCENE_DRAW(scene);

        MLV_draw_line(400, 290, 400, 310, MLV_COLOR_WHITE);
        MLV_draw_line(390, 300, 410, 300, MLV_COLOR_WHITE);

        absolute_time = get_absolute_time();
        to_wait = absolute_time - time_last_frame;
        if (to_wait < (1.0 / NB_FRAME_PER_SEC))
            MLV_wait_milliseconds((int)(((1.0 / (double)NB_FRAME_PER_SEC) - to_wait) * 1000));

        MLV_actualise_window();
        MLV_wait_milliseconds(50);

    }

    MLV_free_window();

    return 0;

}

int main2(int argc, char** argv)
{

    ParamInterface gp;
    double time_last_frame;
    double absolute_time;
    double to_wait;
    double program_busy;
    int winner = -1;
    int selection;
    Space S;
    
    INIT_RANDOM();

    /* auto-load available plugins */
    LIST_OF(Plugin) plugins = Plugin_load_all("plugins/");
    if (LIST_IS_EMPTY(plugins))
        THROW_AND_KILL("No plugin found.", "Cannot start the challenge.");

    /* first part interface... */
    initialize_graphical_paramters(&gp);
    MLV_init_audio();
    create_graphical_window(&gp);
    
    MLV_Music* main_theme = MLV_load_music("assets/audio/MainTheme.ogg");
    MLV_play_music(main_theme, 1.0, -1);

    /* initialise_space(S); */
    draw_introduction(&gp);
    selection = 0;
    S.starships = NULL;
    S.missiles = NULL;
    S.explosions = NULL;
    S.deads = NULL;
    while (!selection)
        selection = draw_selection_space(&gp, plugins, &S);
    
    /* Second part of interface : the game */
    initialize_plugins(S.starships);
    S.frame_number = 0;
    time_last_frame = get_absolute_time();
    program_busy = 0;
    while (winner == -1)
    {
        save_frame_information(S.starships, S.missiles, S.frame_number);
        absolute_time = get_absolute_time();
        to_wait = absolute_time - time_last_frame;
        if (to_wait < (1.0 / NB_FRAME_PER_SEC))
            MLV_wait_milliseconds((int)(((1.0 / (double)NB_FRAME_PER_SEC) - to_wait) * 1000));
        time_last_frame = get_absolute_time();
        program_busy += ((to_wait) / (1.0 / (double)NB_FRAME_PER_SEC)) * 100;
        if (S.frame_number % NB_FRAME_PER_SEC == 0)
        {
            S.busy = (int)(program_busy / NB_FRAME_PER_SEC);
            program_busy = 0;
        }
        draw_space(&gp, &S);
        /* This key function resolve the move and collisions of all
       elements of the space... */
        winner = end_frame_resolution(&S);
        /* Ok for the next frame. */
        S.frame_number++;
    }

    display_win(winner);
    MLV_wait_seconds(5);

    MLV_free_music(main_theme);
    MLV_free_audio();
    free_graphical_window(&gp);

    clean_plugins(S.starships);
    clean_plugins(S.deads);

    remove("plugins/.info_frame.txt");

    free_space(&S);
    printf("END OK\n");

    return 0;
    
}
