#include "splashkit.h"
#include "menu.h"
#include "gamescreen.h"
#include "splashscreen.h"

class Screen;
class ScreenType
{
    protected:
    Screen *screen;
    string screen_type;
    
    public:
        virtual ~ScreenType()
        {
            //write_line("Screen State Destruction: " + screen_type);
        };

        void set_screen(Screen *screen, string screen_type)
        {
            this->screen = screen;
            this->screen_type = screen_type;
        };

        string get_type()
        {
            return this->screen_type;
        };

        virtual void update() = 0;
};
class Screen
{
    private:
        ScreenType *screen;

    public:
        Screen(ScreenType *screen, string screen_type) : screen(nullptr)
        {
            this->change_screen(screen, screen_type);
        };

        ~Screen()
        {
            //write_line("Screen Destruction: " + screen->get_type());
            delete screen;
        };

        void change_screen(ScreenType *new_screen, string type)
        {
            //write_line("Changing Screen: " + type);
            if(this->screen != nullptr)
                delete this->screen;
            this->screen = new_screen;
            this->screen->set_screen(this, type);
        };

        void update()
        {
            this->screen->update();
        };
};

class SplashScreenScreen : public ScreenType
{
    private:
        SplashScreen *splash_screen;

    public:
        SplashScreenScreen() : splash_screen(nullptr)
        {
            SplashScreen *initial_screen = new SplashScreen(new ThothLogoState, "ThothLogo");
            splash_screen = initial_screen;
        };

        ~SplashScreenScreen(){};

        void update() override;
};

class MenuScreen : public ScreenType
{
    private:
        Menu menu;
    public:
        MenuScreen(){};

        ~MenuScreen(){};

        void update() override;
};

class GameScreen : public ScreenType
{
    private:
        GameLoop *game;
    public:
        GameScreen() : game(nullptr)
        {
            Fighter *fighter1 = new Fighter(new IdleState, true, 630 + 150, 200 + 280);
            Fighter *fighter2 = new Fighter(new IdleState, false, 630 + 543, 200 + 280);
            fighter1->set_opponent(fighter2);
            fighter2->set_opponent(fighter1);
            Background *background = new Background(new CityBackground, "City");
            GameLoop *game = new GameLoop(fighter1, fighter2, background, new PreGameState);
            this->game = game;
        };

        ~GameScreen(){};

        void update() override;
};

void SplashScreenScreen::update()
{
    splash_screen->update();

    if(splash_screen->get_state() == "Complete")
    {
        this->screen->change_screen(new MenuScreen, "MenuScreen");
    }
}

void MenuScreen::update()
{
    menu.update();
    switch(menu.get_selection())
    {
        case VSPLAYER: this->screen->change_screen(new GameScreen, "GameScreen");
            break;
        case VSCOMPUTER: write_line("VS Computer selected");
            break;
        case CREDITS: write_line("Credits Selected");
            break;
        case QUIT: exit(0);
            break;
        default:
            break;
    }
}

void GameScreen::update()
{
    game->update();
    if(game->get_state() == "ResetRound" && (game->get_player1()->get_round_win() == 2 || game->get_player2()->get_round_win() == 2))
        this->screen->change_screen(new MenuScreen, "MenuScreen");
}

void sprite_face_direction2(Fighter *fighter1, Fighter *fighter2, bool initial)
{
    point_2d sprite1_center = center_point(fighter1->get_fighter_sprite());
    point_2d sprite2_center = center_point(fighter2->get_fighter_sprite());

    if(sprite1_center.x < sprite2_center.x)
    {
        fighter1->set_facing_left(true);
        fighter2->set_facing_left(false);
    }
    else
    {
        fighter1->set_facing_left(false);
        fighter2->set_facing_left(true);
    }

    if(fighter1->is_facing_left() != initial)
    {
        if(fighter1->get_state() == "Idle")
            fighter1->change_state(new IdleState,"Idle");
        if(fighter2->get_state() == "Idle")
            fighter2->change_state(new IdleState,"Idle");
    }
};

int main()
{
    load_resource_bundle("bundle", "animation_bundle.txt");
    load_resource_bundle("ryu", "fighter_ryu.txt");
    open_window("Test", 768, 488);
    Screen *screen = new Screen(new SplashScreenScreen, "SplashScreen");

    while(!key_typed(ESCAPE_KEY))
    {
        screen->update();
        process_events();
        refresh_screen(60);
    }

    delete screen;
    free_resource_bundle("bundle");

    return 0;
}