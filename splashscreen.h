#include "splashkit.h"

class Splashscreen
{
    private:
        double alpha = 1.0;
        bool thoth_logo = true;
        bool games_logo = false;
        bool playing = true;

    public:
        Splashscreen()
        {
            create_timer("splash_screen");
            start_timer("splash_screen");
        };
        ~Splashscreen()
        {
            free_timer(timer_named("splash_screen"));
        };

        void show_thoth_logo()
        {
            unsigned int time_ticks = timer_ticks("splash_screen");

            if((time_ticks/1000) < 6)
                alpha -= 0.008;
            if((time_ticks/1000) > 4)
            {
                thoth_logo = false;
                games_logo = true;
                alpha = 1.0;
            }
                
            clear_screen(COLOR_BLACK);
            draw_bitmap("thothBrown", 235, 28, option_to_screen());
            draw_bitmap("thothWhite", 230, 30, option_to_screen());
        
            draw_text("Games", COLOR_BROWN, "thothFont", 70, 245, 343, option_to_screen());
            draw_text("Games", COLOR_WHITE, "thothFont", 70, 240, 345, option_to_screen());
            fill_rectangle(rgba_color(0.0,0.0,0.0,alpha),0,0,768,488,option_to_screen());
        };

        void show_game_logo()
        {
            unsigned int time_ticks = timer_ticks("splash_screen");

            if((time_ticks/1000) < 12)
                alpha -= 0.008;
            if((time_ticks/1000) > 9)
            {
                games_logo = false;
                alpha = 1.0;
            }
            draw_text("Lachlan Morgan  " + std::to_string(218144979), COLOR_WHITE, "normalFont", 30, 175, 210, option_to_screen());
            fill_rectangle(rgba_color(0.0,0.0,0.0,alpha),0,0,768,488,option_to_screen());
        };

        void update()
        {
            if(thoth_logo)
                show_thoth_logo();
            else if(games_logo)
                show_game_logo();
            else
                playing = false;
        };

        bool get_playing()
        {
            return this->playing;
        };

};