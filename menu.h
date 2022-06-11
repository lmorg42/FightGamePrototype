#include "splashkit.h"

enum selection
{
    VSPLAYER,
    VSCOMPUTER,
    CREDITS,
    QUIT
};

class Menu
{
    private:
        int select = 0;
        font text_font;
        font text_font2;
        int menu_selection = -1;

    public:
        Menu()
        {
            create_timer("menu_time");
            start_timer("menu_time");
        };
        ~Menu()
        {
            free_timer(timer_named("menu_time"));
        };

        void update()
        {
            clear_screen(COLOR_BLACK);
            draw_text("Menu Screen", COLOR_WHITE, "fancyFont", 65, 200, 30, option_to_screen());
            draw_text("VS Player", COLOR_WHITE, "normalFont", 58, 10, 250, option_to_screen());
            draw_text("VS Computer", COLOR_WHITE, "normalFont", 58, 10, 300, option_to_screen());
            draw_text("Credits", COLOR_WHITE, "normalFont", 58, 10, 350, option_to_screen());
            draw_text("Quit", COLOR_WHITE, "normalFont", 58, 10, 400, option_to_screen());
            input();
            selection();
            make_selection(select);
        };

        void flashing_text(string text, int y_pos)
        {
            unsigned int time_ticks = timer_ticks("menu_time");
            if((time_ticks/650)%2 == 0)
                draw_text(text, COLOR_RED, "normalFont", 58, 10, y_pos, option_to_screen());
            else
                draw_text(text, COLOR_YELLOW, "normalFont", 58, 10, y_pos, option_to_screen());
        };

        void selection()
        {
            switch(this->select)
            {
                case 0: flashing_text("VS Player", 250);
                        break;
                case 1: flashing_text("VS Computer", 300);
                        break;
                case 2: flashing_text("Credits", 350);
                        break;
                case 3: flashing_text("Quit", 400);
                        break;
            }
        };

        void input()
        {
            if(key_typed(UP_KEY))
                this->select -= 1;
            if(key_typed(DOWN_KEY))
                this->select += 1;

            if(select < 0)
                this->select = 0;
            if(select > 3)
                this->select = 3;
        };

        void make_selection(int select)
        {
            if(key_typed(SPACE_KEY))
            {
                switch(select)
                {
                    case 0: menu_selection = VSPLAYER;
                        break;
                    case 1: menu_selection = VSCOMPUTER;
                        break;
                    case 2: menu_selection = CREDITS;
                        break;
                    case 3: menu_selection = QUIT;
                        break;
                }
            }
        };

        int get_selection()
        {
            return this->menu_selection;
        };
};