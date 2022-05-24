#include "splashkit.h"
#include "menu.h"
#include "gamescreen.h"
#include "splashscreen.h"

Character character_selection(bool facing_left, double start_x, double start_y, bool player1, bool human_player, string name)
{
    if(name == "Ryu")
    {
        Ryu player(facing_left, start_x, start_y, player1, human_player);
        return player;
    }
}

Background background_selection(string name)
{
    if(name == "City")
    {
        CityBackground background;
        return background;
    }
}

int main()
{
    load_resource_bundle("bundle", "animation_bundle.txt");
    set_camera_x(630);
    set_camera_y(200);
    open_window("Test", 768, 488);
    window_toggle_border("Test");

    Splashscreen splash_screen;
    GameScreen game_screen;
    Character player1;
    Character player2;
    Background background;
    Menu menu_screen;
    
    bool splashscreen = true;
    bool quit = false;
    bool menu = false;
    bool game = false;

    bool load_game = true;
    bool victory = false;

    while(!key_typed(ESCAPE_KEY))
    {
        if(splashscreen)
        {
            splash_screen.update();
            splashscreen = splash_screen.get_playing();
            if(!splashscreen)
                menu = true;
        }
        else if(menu)
        {
            if(!has_timer("menu_time"))
            {
                create_timer("menu_time");
                start_timer("menu_time");
            }
            menu_screen.update();
            quit = menu_screen.get_quit();
            if(quit)
                menu = false;
            game = menu_screen.get_game_start();
            if(game)
                menu = false;
        }
        else if(game)
        {
            if(load_game)
            {
                player1 = character_selection(true, camera_x() + 150, camera_y() + 280, true, true, "Ryu");
                player2 = character_selection(false, camera_x() + 543, camera_y() + 280, false, true, "Ryu");
                background = background_selection("City");
                GameScreen load(player1, player2, background);
                game_screen = load;
                load_game = false;
            }
            game_screen.update();
            victory = game_screen.get_victory();
            if(victory)
            {
                game = false;
                menu_screen.set_game_start(false);
                menu = true;
                game_screen.full_reset();
            }
        }
        else if(quit)
        {
            break;
            //exit(0);
        }

        process_events();
        refresh_screen(60);
    }

    return 0;
}