#include "splashkit.h"
#include "character.h"
#include "time.h"
#include "background.h"
//#include "camera.h"
//#include "collision.h"
//#include "test_functions.h"

#define SCREEN_BORDER 30
#define SCREEN_LIMIT_LEFT 0
#define SCREEN_RIGHT_LIMIT 1900

void reject_boundry(Fighter *player, float border, float value)
{
    sprite_set_x(player->get_fighter_sprite(), border + value);
    if(player->get_state() == "WalkLeft" || player->get_state() == "WalkRight")
        player->change_state(new WalkBlockState, "WalkBlock");
}

void test_left_edge(Fighter *player1, Fighter *player2, double left_edge, double right_edge)
{
    if (sprite_x(player1->get_fighter_sprite()) < left_edge && ((player1->get_state() == "WalkLeft" || player1->get_state() == "WalkRight") || player1->get_state() == "Jump"))
    {
        move_camera_by(sprite_x(player1->get_fighter_sprite()) - left_edge, 0);
        if(sprite_x(player2->get_fighter_sprite()) > right_edge)
            reject_boundry(player2, right_edge, -1);
    }
}

void test_right_edge(Fighter *player1, Fighter *player2, double left_edge, double right_edge)
{
    if(sprite_x(player1->get_fighter_sprite()) > right_edge && ((player1->get_state() == "WalkLeft" || player1->get_state() == "WalkRight") || player1->get_state() == "Jump"))
    {
        move_camera_by(sprite_x(player1->get_fighter_sprite()) - right_edge, 0);
        if(sprite_x(player2->get_fighter_sprite()) < left_edge)
            reject_boundry(player2, left_edge, -1);
    }
}

void test_left_boundry(Fighter *player1, Fighter *player2, double right_edge)
{
    if(sprite_x(player1->get_fighter_sprite()) < SCREEN_LIMIT_LEFT)
    {
        reject_boundry(player1, SCREEN_LIMIT_LEFT, 1);
        if(sprite_x(player2->get_fighter_sprite()) > right_edge)
            reject_boundry(player2, right_edge, -1);
    }
}

void test_right_boundry(Fighter *player1, Fighter *player2, double left_edge)
{
    if(sprite_x(player1->get_fighter_sprite()) > SCREEN_RIGHT_LIMIT)
    {
        reject_boundry(player1, SCREEN_RIGHT_LIMIT, -1);
        if(sprite_x(player2->get_fighter_sprite()) < left_edge)
            reject_boundry(player2, left_edge, 1);
    }
}

void update_camera_position(Fighter *player1, Fighter *player2)
{
    // Test edge of screen boundaries to adjust the camera
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER - 100;

    test_left_edge(player1, player2, left_edge, right_edge);
    test_left_edge(player2, player1, left_edge, right_edge);
    test_right_edge(player1, player2, left_edge, right_edge);
    test_right_edge(player2, player1, left_edge, right_edge);
    test_left_boundry(player1, player2, right_edge);
    test_left_boundry(player2, player1, right_edge);
    test_right_boundry(player1, player2, left_edge);
    test_right_boundry(player2, player1, left_edge);
}

class GameLoop;

class GameLoopState
{
    protected:
        GameLoop *game_screen;
        string state_type;
    
    public:
        virtual ~GameLoopState()
        {
            //write_line("GameLoop State Destruction:" + state_type);
        };

        void set_state(GameLoop *game_screen, string state_type)
        {
            this->game_screen = game_screen;
            this->state_type = state_type;
        };

        string get_state()
        {
            return this->state_type;
        };

        virtual void update() = 0;
};

class GameLoop
{
    protected:
        GameLoopState *state;
        Fighter *player1;
        Fighter *player2;
        Background *background;
        Time round_timer;
        int round_number = 1;
        string end_display_text = "";

    public:
        GameLoop(Fighter *player1, Fighter *player2, Background *background, GameLoopState *state) : state(nullptr)
        {
            set_camera_x(630);
            set_camera_y(200);
            this->player1 = player1;
            this->player2 = player2;
            this->background = background;
            this->change_state(state, "Initial");
        };

        ~GameLoop()
        {
            //write_line("GameLoop Destruction");
            delete state;
        };

        void change_state(GameLoopState *new_state, string state_type)
        {
            //write_line("Changing GameLoop State: " + state_type);
            if(this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, state_type);
        };

        void update()
        {
            this->state->update();
        };

        void sprite_face_direction(Fighter *player1, Fighter *player2, bool initial)
        {
            point_2d sprite1_center = center_point(player1->get_fighter_sprite());
            point_2d sprite2_center = center_point(player2->get_fighter_sprite());

            if(sprite1_center.x < sprite2_center.x)
            {
                player1->set_facing_left(true);
                player2->set_facing_left(false);
            }
            else
            {
                player1->set_facing_left(false);
                player2->set_facing_left(true);
            }

            if(player1->is_facing_left() != initial)
            {
                if(player1->get_state() == "Idle")
                    player1->change_state(new IdleState,"Idle");
                if(player2->get_state() == "Idle")
                    player2->change_state(new IdleState,"Idle");
            }
        };

        string get_state()
        {
            return this->state->get_state();
        };

        Background* get_background()
        {
            return this->background;
        };

        Fighter* get_player1()
        {
            return this->player1;
        };

        void set_player1(Fighter *player)
        {
            this->player1 = player;
        }

        Fighter* get_player2()
        {
            return this->player2;
        };

        void set_player2(Fighter *player)
        {
            this->player2 = player;
        }

        Time get_timer()
        {
            return this->round_timer;
        };

        void set_timer(Time timer)
        {
            this->round_timer = timer;
        }

        int get_round_number()
        {
            return this->round_number;
        };

        void set_round_number(int value)
        {
            this->round_number = value;
        };

        string get_end_display_text()
        {
            return this->end_display_text;
        };

        void set_end_display_text(string text)
        {
            this->end_display_text = text;
        };
};

class PreGameState : public GameLoopState
{
    private:
        double round_alpha = 0.1;
        double fight_alpha = 0.1;
        double fade_alpha = 1.0;
        bool finished = false;

    public:
        PreGameState(){};

        ~PreGameState(){};

        void update() override;

        void pre_game_show_text()
        {
            if(round_alpha < 3)
            {
                round_alpha += 0.02;
                color text_color = rgba_color(255.0,255.0,255.0,round_alpha);
                draw_text("Round " + to_string(this->game_screen->get_round_number()), text_color, "normalFont", 65, 285, 130, option_to_screen());
            }
            else
            {
                if(fight_alpha < 2.5)
                {
                    fight_alpha += 0.02;
                    color text_color = rgba_color(255.0,255.0,255.0,fight_alpha);
                    draw_text("Fight", text_color, "normalFont", 65, 310, 130, option_to_screen());
                }
                else if(fight_alpha > 2.5 && fight_alpha < 3)
                {
                    fight_alpha += 0.02;
                    if(fade_alpha > 0)
                        fade_alpha -= 0.02;
                    color text_color = rgba_color(255.0,0.0,0.0,fade_alpha);
                    draw_text("Fight", text_color, "normalFont", 65, 310, 130, option_to_screen());
                }
                else
                    finished = true;
            }
        };
};

class GameState : public GameLoopState
{
    private:
        Fighter *player1;
        Fighter *player2;
        Background *background;
        Time round_timer;
        bool once = false;

    public:
        GameState(){};

        ~GameState(){};

        void update() override;

        bool test_time_over(Fighter *player1, Fighter *player2, Time round_timer)
        {
            bool time_over = false;

            if(round_timer.get_time() == "00")
            {
                time_over = true;
            }

            return time_over;
        };

        bool test_double_ko(Fighter *player1, Fighter *player2, Time round_timer)
        {
            bool double_ko = false;

            if(player1->get_lifebar().get_health() <= 0 && player2->get_lifebar().get_health() <= 0)
            {
                double_ko = true;
            }

            return double_ko;
        };

        bool test_player_win(Fighter *player1, Fighter *player2, Time round_timer)
        {
            bool player_win = false;

            if(player2->get_lifebar().get_health() <= 0)
            {
                player_win = true;
            }
            return player_win;
        };

        void game_over_test(Fighter *player1, Fighter *player2, Time round_timer);
};

class RoundEndState : public GameLoopState
{
    private:
        double round_over_timer = 0.0;

    public:
        RoundEndState(){};

        ~RoundEndState(){};

        void update() override;
};

class TimeOverState : public GameLoopState
{
    public:
        TimeOverState(){};

        ~TimeOverState(){};

        void update() override;
};

class DoubleKoState : public GameLoopState
{
    public:
        DoubleKoState(){};

        ~DoubleKoState(){};

        void update() override;
};

class PlayerWinState : public GameLoopState
{
    private:
        bool player_1_win;

    public:
        PlayerWinState(bool player_1_win)
        {
            this->player_1_win = player_1_win;
        };

        ~PlayerWinState(){};

        void update() override;
};

class ResetRoundState : public GameLoopState
{
    public:
        ResetRoundState(){};

        ~ResetRoundState(){};

        void update() override;
};

void PreGameState::update()
{
    clear_screen(COLOR_GRAY);
    this->game_screen->get_background()->draw_background();
    this->game_screen->get_player1()->update();
    this->game_screen->get_player2()->update();
    this->game_screen->get_player1()->draw_lifebar();
    this->game_screen->get_player2()->draw_lifebar();
    this->game_screen->get_timer().stop_time();
    this->game_screen->get_timer().update();
    pre_game_show_text();

    if(finished)
    {
        this->game_screen->get_timer().start_time();
        this->game_screen->change_state(new GameState, "Game");
    }
}

void GameState::game_over_test(Fighter *player1, Fighter *player2, Time round_timer)
{
    game_screen->set_player1(player1);
    game_screen->set_player2(player2);
    game_screen->set_timer(round_timer);

    if(test_time_over(player1, player2, round_timer))
        this->game_screen->change_state(new TimeOverState, "TimeOver");
    else if(test_double_ko(player1, player2, round_timer))
        this->game_screen->change_state(new DoubleKoState, "DoubleKo");
    else if(test_player_win(player1, player2, round_timer))
    {
        player1->add_round_win();
        game_screen->set_player1(player1);
        this->game_screen->change_state(new PlayerWinState(true), "Player1Win");           
    }
    else if(test_player_win(player2, player1, round_timer))
    {
        player2->add_round_win();
        game_screen->set_player2(player2);
        this->game_screen->change_state(new PlayerWinState(false), "Player2Win");
    }
};

void GameState::update()
{
    if(!once)
    {
        this->player1 = game_screen->get_player1();
        this->player2 = game_screen->get_player2();
        this->background = game_screen->get_background();
        this->round_timer = game_screen->get_timer();
        once = true;
    }

    //Draw Screen
    clear_screen(COLOR_GRAY);
    background->draw_background();

    //Player Inputs
    player1->get_input();
    player2->get_input();
    
    //Update Sprites
    game_screen->sprite_face_direction(player1, player2, player1->is_facing_left());
    player1->update();
    player2->update();

    //Draw Lifebars
    player1->draw_lifebar();
    player2->draw_lifebar();

    //Update camera
    update_camera_position(player1, player2);

    //Test End Conditions
    game_over_test(player1, player2, round_timer);

    //Update timer
    round_timer.update();
}

void TimeOverState::update()
{
    this->game_screen->set_end_display_text("Time Over");
    this->game_screen->get_player1()->change_state(new TimeOverFighterState, "TimeOver");
    this->game_screen->get_player2()->change_state(new TimeOverFighterState, "TimeOver");
    this->game_screen->get_player1()->draw_lifebar();
    this->game_screen->get_player2()->draw_lifebar();
    this->game_screen->get_timer().stop_time();
    this->game_screen->change_state(new RoundEndState, "RoundEnd");
}

void DoubleKoState::update()
{
    this->game_screen->set_end_display_text("Double KO!!!");
    this->game_screen->get_player1()->change_state(new KoState, "Ko");
    this->game_screen->get_player2()->change_state(new KoState, "Ko");
    this->game_screen->get_player1()->draw_lifebar();
    this->game_screen->get_player2()->draw_lifebar();
    this->game_screen->get_timer().stop_time();
    this->game_screen->change_state(new RoundEndState, "RoundEnd");
}

void PlayerWinState::update()
{
    string name;
    if(player_1_win)
    {
        this->game_screen->get_player1()->change_state(new VictoryState, "Victory");
        this->game_screen->get_player2()->change_state(new KoState, "Ko");
        name = this->game_screen->get_player1()->get_name();
    }
    else
    {
        this->game_screen->get_player2()->change_state(new VictoryState, "Victory");
        this->game_screen->get_player1()->change_state(new KoState, "Ko");
        name = this->game_screen->get_player2()->get_name();
    }

    this->game_screen->set_end_display_text(name + " Wins!");
    this->game_screen->get_player1()->draw_lifebar();
    this->game_screen->get_player2()->draw_lifebar();
    this->game_screen->get_timer().stop_time();
    this->game_screen->change_state(new RoundEndState, "RoundEnd");
}

void RoundEndState::update()
{
    clear_screen(COLOR_GRAY);
    this->game_screen->get_background()->draw_background();
    this->game_screen->get_player1()->update();
    this->game_screen->get_player2()->update();
    this->game_screen->get_player1()->draw_lifebar();
    this->game_screen->get_player2()->draw_lifebar();
    this->game_screen->get_timer().update();
    draw_text(this->game_screen->get_end_display_text(), COLOR_WHITE, "normalFont", 65, 238, 123, option_to_screen());

    if(round_over_timer > 5)
        this->game_screen->change_state(new ResetRoundState, "ResetRound");

    round_over_timer += 0.02;
}

void ResetRoundState::update()
{
    set_camera_x(630);
    set_camera_y(200);

    this->game_screen->get_player1()->change_state(new IdleState, "Idle");
    this->game_screen->get_player2()->change_state(new IdleState, "Idle");

    Fighter *player1 = this->game_screen->get_player1();
    player1->reset();
    game_screen->set_player1(player1);

    Fighter *player2 = this->game_screen->get_player2();
    player2->reset();
    game_screen->set_player2(player2);

    Time timer = this->game_screen->get_timer();
    timer.reset();
    game_screen->set_timer(timer);
    game_screen->sprite_face_direction(player1, player2, player1->is_facing_left());

    this->game_screen->get_player1()->draw_lifebar();
    this->game_screen->get_player2()->draw_lifebar();
    this->game_screen->set_round_number(this->game_screen->get_round_number() + 1);
    this->game_screen->change_state(new PreGameState, "PreGame");
}