#include "splashkit.h"
class SplashScreen;

class SplashScreenState
{
    protected:
    SplashScreen *splash_screen;
    string state_type;
    
    public:
        virtual ~SplashScreenState()
        {
            write_line("SplashScreen State Destruction:" + state_type);
        };

        void set_state(SplashScreen *splash_screen, string state_type)
        {
            this->splash_screen = splash_screen;
            this->state_type = state_type;
        };

        string get_state()
        {
            return this->state_type;
        };

        virtual void update() = 0;
};

class SplashScreen
{
    protected:
        SplashScreenState *state;

    public:
        SplashScreen(SplashScreenState *state, string state_type) : state(nullptr)
        {
            create_timer("splash_screen");
            start_timer("splash_screen");
            this->change_state(state, state_type);
        };

        ~SplashScreen()
        {
            write_line("Splash Screen Destruction: " + state->get_state());
            free_timer(timer_named("splash_screen"));
            delete state;
        };

        void change_state(SplashScreenState *new_state, string state_type)
        {
            write_line("Changing SplashScreen State: " + state_type);
            if(this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, state_type);
        };

        void update()
        {
            this->state->update();
        };

        string get_state()
        {
            return this->state->get_state();
        };
};

class ThothLogoState : public SplashScreenState
{
    private:
        double alpha = 1.0;

    public:
        ThothLogoState(){};

        ~ThothLogoState(){};

        void update() override;
};

class PersonalLogoState : public SplashScreenState
{
    private:
        double alpha = 1.0;

    public:
        PersonalLogoState(){};

        ~PersonalLogoState(){};

        void update() override;
};

class CompleteState : public SplashScreenState
{
    public:
        CompleteState(){};

        ~CompleteState(){};

        void update() override;
};

void ThothLogoState::update()
{
    unsigned int time_ticks = timer_ticks("splash_screen");

    if((time_ticks/1000) < 5)
    {
        alpha -= 0.008;
        clear_screen(COLOR_BLACK);
        draw_bitmap("thothBrown", 235, 28, option_to_screen());
        draw_bitmap("thothWhite", 230, 30, option_to_screen());

        draw_text("Games", COLOR_BROWN, "thothFont", 70, 245, 343, option_to_screen());
        draw_text("Games", COLOR_WHITE, "thothFont", 70, 240, 345, option_to_screen());
        fill_rectangle(rgba_color(0.0,0.0,0.0,alpha),0,0,768,488,option_to_screen());

    }     
    else
        this->splash_screen->change_state(new PersonalLogoState, "PersonalLogo");
}

void PersonalLogoState::update()
{
    unsigned int time_ticks = timer_ticks("splash_screen");

    if((time_ticks/1000) < 10)
    {
        alpha -= 0.008;
        draw_text("Lachlan Morgan  " + std::to_string(218144979), COLOR_WHITE, "normalFont", 30, 175, 210, option_to_screen());
        fill_rectangle(rgba_color(0.0,0.0,0.0,alpha),0,0,768,488,option_to_screen());
    }
    else
        this->splash_screen->change_state(new CompleteState, "Complete");
}

void CompleteState::update()
{
    fill_rectangle(COLOR_WHITE,0,0,768,488,option_to_screen());
}