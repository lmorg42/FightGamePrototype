#include "splashkit.h"
#include "lifebar.h"
#include "playerinput.h"
#include "collision.h"
#include "test_functions.h"
//#include "camera.h"
class Fighter;

class FighterState
{
    protected:
        Fighter *fighter;
        string state_type;
    
    public:
        virtual ~FighterState()
        {
            //write_line("Fighter State Destruction:" + state_type);
        };

        void set_state(Fighter *fighter, string state_type)
        {
            this->fighter = fighter;
            this->state_type = state_type;
        };

        string get_state()
        {
            return this->state_type;
        };

        virtual void update() = 0;
        virtual void get_input() = 0;
};

class Fighter
{
    protected:
        FighterState *state;
        Fighter *opponent;
        sprite fighter_sprite;
        point_2d start_position;
        Lifebar lifebar;
        string name;
        string bundlename;
        bool facing_left;
        bool player1;
        int round_win = 0;

    public:
        key_code right_key;
        key_code left_key;
        key_code jump_key;
        key_code crouch_key;
        key_code punch_key;
        key_code kick_key;
        key_code block_key;

        Fighter(FighterState *state, bool player1, double start_x, double start_y):state(nullptr)
        {
            //Inject FighterType
            this->opponent = opponent;
            this->player1 = player1;
            if(player1)
                facing_left = true;
            else
                facing_left = false;

            start_position.x = start_x;
            start_position.y = start_y;
            
            fighter_sprite = create_sprite("RyuBmp", "RyuTest");
            name = "Ryu";
            
            Lifebar lifebar(player1,name);
            this->lifebar = lifebar;
            sprite_setup();

            //Re-do input
            if(player1)
            {
                right_key = RIGHT_KEY;
                left_key = LEFT_KEY;
                jump_key = UP_KEY;
                crouch_key = DOWN_KEY;
                punch_key = A_KEY;
                kick_key = S_KEY;
                block_key = D_KEY;
            }
            else
            {
                right_key = K_KEY;
                left_key = H_KEY;
                jump_key = U_KEY;
                crouch_key = J_KEY;
                punch_key = B_KEY;
                kick_key = N_KEY;
                block_key = M_KEY;
            }

            this->change_state(state, "Idle");
        };
        ~Fighter(){};

        void change_state(FighterState *new_state, string state_type)
        {
            //write_line("Changing Fighter State: " + state_type);
            if(this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, state_type);
        };

        void update()
        {
            this->state->update();
        };

        void get_input()
        {
            this->state->get_input();
        };

        void sprite_setup()
        {
            if(player1)
                facing_left = true;
            sprite_set_position(fighter_sprite, start_position);
            sprite_set_scale(fighter_sprite, 2.8);
        };

        void draw_lifebar()
        {
            this->lifebar.draw();

            if(player1 && round_win >= 1)
                draw_bitmap("coin",320,45,option_to_screen());
            else if(!player1 && round_win >= 1)
                draw_bitmap("coin",425,45,option_to_screen());
        };

        string get_name()
        {
            return this->name;
        };

        bool is_facing_left()
        {
            return this->facing_left;
        };

        void set_facing_left(bool facing_left)
        {
            this->facing_left = facing_left;
        };

        bool is_player1()
        {
            return this->player1;
        };

        sprite get_fighter_sprite()
        {
            return this->fighter_sprite;
        };

        string get_state()
        {
            return this->state->get_state();
        };

        point_2d get_start_position()
        {
            return this->start_position;
        };

        Fighter* get_opponent()
        {
            return this->opponent;
        };

        void set_opponent(Fighter *opponent)
        {
            this->opponent = opponent;
        };

        Lifebar get_lifebar()
        {
            return this->lifebar;
        };

        void set_lifebar(Lifebar lifebar)
        {
            this->lifebar = lifebar;
        };

        void add_round_win()
        {
            this->round_win += 1;
        };

        int get_round_win()
        {
            return this->round_win;
        };

        void reset()
        {
            sprite_setup();
            lifebar.reset();
        };
};

class IdleState : public FighterState
{
    private:
        bool once = false;

    public:
        IdleState(){};

        ~IdleState(){};

        void update() override;
        void get_input() override;
};

class WalkState : public FighterState
{
    private:
        bool once = false;
        bool left_direction;

    public:
        WalkState(bool left_direction)
        {
            this->left_direction = left_direction;
        };

        ~WalkState(){};

        void update() override;
        void get_input() override;
};

class CrouchState : public FighterState
{
    private:
        bool once = false;

    public:
        CrouchState(){};

        ~CrouchState(){};

        void update() override;
        void get_input() override;
};

class JumpState : public FighterState
{
    private:
        bool once = false;

    public:
        JumpState(){};

        ~JumpState(){};

        void update() override;
        void get_input() override;
};

class BlockState : public FighterState
{
    private:
        bool once = false;

    public:
        BlockState(){};

        ~BlockState(){};

        void update() override;
        void get_input() override;
};

class PunchState : public FighterState
{
    private:
        bool once = false;

    public:
        PunchState(){};

        ~PunchState(){};

        void update() override;
        void get_input() override;
};

class KickState : public FighterState
{
    private:
        bool once = false;

    public:
        KickState(){};

        ~KickState(){};

        void update() override;
        void get_input() override;
};

class HitState : public FighterState
{
    private:
        bool once = false;

    public:
        HitState(){};

        ~HitState(){};

        void update() override;
        void get_input() override;
};

class KoState : public FighterState
{
    private:
        bool once = false;

    public:
        KoState(){};

        ~KoState(){};

        void update() override;
        void get_input() override;
};

class VictoryState : public FighterState
{
    private:
        bool once = false;

    public:
        VictoryState(){};

        ~VictoryState(){};

        void update() override;
        void get_input() override;
};

class TimeOverFighterState : public FighterState
{
    private:
        bool once = false;

    public:
        TimeOverFighterState(){};

        ~TimeOverFighterState(){};

        void update() override;
        void get_input() override;
};

class WalkBlockState : public FighterState
{
    private:
        bool once = false;

    public:
        WalkBlockState(){};

        ~WalkBlockState(){};

        void update() override;
        void get_input() override;
};

void test_walking_collision(Fighter *fighter1, Fighter *fighter2)
{
    rectangle rect = make_hitbox(fighter1->get_fighter_sprite(), 0, -48, 105, -30);
    bool rectangle_collision = sprite_rectangle_collision(fighter2->get_fighter_sprite(), rect);

    if(rectangle_collision)
    {
        fighter1->change_state(new WalkBlockState,"WalkBlock");
        if(fighter1->is_facing_left())
            sprite_set_x(fighter1->get_fighter_sprite(), sprite_x(fighter1->get_fighter_sprite()) - 2);
        else
            sprite_set_x(fighter1->get_fighter_sprite(), sprite_x(fighter1->get_fighter_sprite()) + 2);
    }
    //draw_hitboxes(rect, rect);
}

void test_idle_collision(Fighter *fighter1, Fighter *fighter2)
{
    rectangle rect = make_hitbox(fighter1->get_fighter_sprite(), 0, -48, 105, -30);
    bool rectangle_collision = sprite_rectangle_collision(fighter2->get_fighter_sprite(), rect);

    if(rectangle_collision)
    {
        if(fighter2->get_state() == "Idle")
        {
            if(fighter1->is_facing_left())
                sprite_set_x(fighter1->get_fighter_sprite(), sprite_x(fighter1->get_fighter_sprite()) - 2);
            else
                sprite_set_x(fighter1->get_fighter_sprite(), sprite_x(fighter1->get_fighter_sprite()) + 2);
        }
    }
    //draw_hitboxes(rect, rect);
}

void attack_knockback(Fighter *fighter, double amount)
{
    if(fighter->is_facing_left())
        sprite_set_x(fighter->get_fighter_sprite(), sprite_x(fighter->get_fighter_sprite()) - amount);
    else
        sprite_set_x(fighter->get_fighter_sprite(), sprite_x(fighter->get_fighter_sprite()) + amount);
}

void attack_damage(Fighter *fighter, int range1, int range2)
{
    int random = rand() % range1 + range2;
    write_line(std::to_string(random));

    Lifebar damage = fighter->get_lifebar();
    damage.damage(random);
    fighter->set_lifebar(damage); 
}

void test_punch_collision(Fighter *fighter1, Fighter *fighter2)
{
    if(fighter2->get_state() != "Hit" && fighter2->get_state() != "Jump")
    {
        rectangle rect;
        if(fighter1->is_facing_left())
            rect = make_hitbox(fighter1->get_fighter_sprite(), 50, -48, -30, -30);
        else
            rect = make_hitbox(fighter1->get_fighter_sprite(), -50, -48, -30, -30);

        bool rectangle_collision = sprite_rectangle_collision(fighter2->get_fighter_sprite(), rect);

        if(rectangle_collision)
        {
            if(fighter2->get_state() == "Block")
            {               
                attack_knockback(fighter2, 0.5);
                attack_damage(fighter2, 2, 1);
            }
            else
            {
                attack_knockback(fighter2, 12);
                fighter2->change_state(new HitState, "Hit");
                attack_damage(fighter2, 6, 2);
            }
        }
        //draw_hitboxes(rect, rect);
    }
}

void test_kick_collision(Fighter *fighter1, Fighter *fighter2)
{
    if(fighter2->get_state() != "Hit" && fighter2->get_state() != "Jump")
    {
        rectangle rect;
        if(fighter1->is_facing_left())
            rect = make_hitbox(fighter1->get_fighter_sprite(), 50, -48, -30, -30);
        else
            rect = make_hitbox(fighter1->get_fighter_sprite(), -50, -48, -30, -30);

        bool rectangle_collision = sprite_rectangle_collision(fighter2->get_fighter_sprite(), rect);

        if(rectangle_collision)
        {
            if(fighter2->get_state() == "Block")
            {                
                attack_knockback(fighter2, 3);
                attack_damage(fighter2, 3, 1);
            }
            else
            {
                attack_knockback(fighter2, 18);
                fighter2->change_state(new HitState, "Hit");
                attack_damage(fighter2, 8, 6);
            }
        }
        //draw_hitboxes(rect, rect);
    }
}

void sprite_update_routine_continuous(sprite fighter_sprite)
{
    draw_sprite(fighter_sprite);
    if(sprite_animation_has_ended(fighter_sprite))
        sprite_replay_animation(fighter_sprite);
    update_sprite(fighter_sprite);
}

void sprite_update_routine_once(sprite fighter_sprite, Fighter *fighter)
{
    draw_sprite(fighter_sprite);
    if(sprite_animation_has_ended(fighter_sprite))
        fighter->change_state(new IdleState, "Idle");
    update_sprite(fighter_sprite);
}

void animation_routine(bool facing_left, string left_anim, string right_anim, Fighter *fighter)
{
    if(facing_left)
        sprite_start_animation(fighter->get_fighter_sprite(), left_anim);
    else
        sprite_start_animation(fighter->get_fighter_sprite(), right_anim);
}

void IdleState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        sprite_set_dx(fighter_sprite, 0);
        sprite_set_y(fighter->get_fighter_sprite(), fighter->get_start_position().y);
        animation_routine(this->fighter->is_facing_left(), "IdleLeft", "IdleRight", fighter);
        once = true;
    }
    test_idle_collision(this->fighter, this->fighter->get_opponent());
    sprite_update_routine_continuous(fighter_sprite);
}

void IdleState::get_input()
{
    if(key_down(fighter->left_key))
        this->fighter->change_state(new WalkState(true), "WalkLeft");
    else if(key_down(fighter->right_key))
        this->fighter->change_state(new WalkState(false), "WalkRight");
    else if(key_down(fighter->crouch_key))
        this->fighter->change_state(new CrouchState, "Crouch");
    else if(key_typed(fighter->jump_key))
        this->fighter->change_state(new JumpState, "Jump");
    else if(key_typed(fighter->block_key))
        this->fighter->change_state(new BlockState, "Block");
    else if(key_typed(fighter->punch_key))
        this->fighter->change_state(new PunchState, "Punch");
    else if(key_typed(fighter->kick_key))
        this->fighter->change_state(new KickState, "Kick");
}

void WalkState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();

    if(!once)
    {
        animation_routine(this->fighter->is_facing_left(), "WalkLeft", "WalkRight", fighter);

        if(left_direction)
            sprite_set_dx(fighter_sprite, -4);
        else
            sprite_set_dx(fighter_sprite, 4);
        
        once = true;
    }

    test_walking_collision(this->fighter, this->fighter->get_opponent());
    sprite_update_routine_continuous(fighter_sprite);
    
}

void WalkState::get_input()
{
    if(key_typed(fighter->jump_key))
        this->fighter->change_state(new JumpState, "Jump");
    else if(key_released(fighter->left_key) || key_released(fighter->right_key))
        this->fighter->change_state(new IdleState, "Idle");
}

void CrouchState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        animation_routine(this->fighter->is_facing_left(), "CrouchLeft", "CrouchRight", fighter);
        once = true;
    }

    sprite_update_routine_continuous(fighter_sprite);
}

void CrouchState::get_input()
{
    if(key_released(fighter->crouch_key))
        this->fighter->change_state(new IdleState, "Idle");
}

void JumpState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        animation_routine(this->fighter->is_facing_left(), "JumpLeft", "JumpRight", fighter);
        once = true;
    }

    sprite_update_routine_once(fighter_sprite, this->fighter);
}

void JumpState::get_input()
{}

void BlockState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        animation_routine(this->fighter->is_facing_left(), "BlockLeft", "BlockRight", fighter);
        once = true;
    }

    sprite_update_routine_continuous(fighter_sprite);
}

void BlockState::get_input()
{
    if(key_released(fighter->block_key))
        this->fighter->change_state(new IdleState, "Idle");
}

void PunchState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        animation_routine(this->fighter->is_facing_left(), "PunchLeft", "PunchRight", fighter);
        test_punch_collision(this->fighter, this->fighter->get_opponent());
        once = true;
    }
    
    sprite_update_routine_once(fighter_sprite, this->fighter);
}

void PunchState::get_input()
{}

void KickState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        animation_routine(this->fighter->is_facing_left(), "KickLeft", "KickRight", fighter);
        test_kick_collision(this->fighter, this->fighter->get_opponent());
        once = true;
    }

    sprite_update_routine_once(fighter_sprite, this->fighter);
}

void KickState::get_input()
{}

void HitState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        sprite_set_dx(fighter_sprite, 0);
        int random = rand() % 2;
        if(random == 0)
            animation_routine(this->fighter->is_facing_left(), "HitLeft1", "HitRight1", fighter);
        else
            animation_routine(this->fighter->is_facing_left(), "HitLeft2", "HitRight2", fighter);

        once = true;
    }

    sprite_update_routine_once(fighter_sprite, this->fighter);
}

void HitState::get_input()
{}

void KoState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        sprite_set_dx(fighter_sprite,0);
        animation_routine(this->fighter->is_facing_left(), "KoLeft", "KoRight", fighter);
        once = true;
    }

    draw_sprite(fighter_sprite);
    update_sprite(fighter_sprite);
}

void KoState::get_input()
{}

void VictoryState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        sprite_start_animation(fighter_sprite, "Victory");
        once = true;
    }

    draw_sprite(fighter_sprite);
    update_sprite(fighter_sprite);
}

void VictoryState::get_input()
{}

void TimeOverFighterState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        sprite_start_animation(fighter_sprite, "Victory");
        once = true;
    }

    draw_sprite(fighter_sprite);
    update_sprite(fighter_sprite);
}

void TimeOverFighterState::get_input()
{}

void WalkBlockState::update()
{
    sprite fighter_sprite = this->fighter->get_fighter_sprite();
    if(!once)
    {
        sprite_set_dx(fighter_sprite, 0);
        once = true;
    }
    test_walking_collision(this->fighter, this->fighter->get_opponent());
    sprite_update_routine_continuous(fighter_sprite);
}

void WalkBlockState::get_input()
{
    if(key_typed(fighter->jump_key))
    {
        if(this->fighter->is_facing_left())
            sprite_set_dx(this->fighter->get_fighter_sprite(), 4);
        else
            sprite_set_dx(this->fighter->get_fighter_sprite(), -4);
        this->fighter->change_state(new JumpState, "Jump");
    }
    else if(key_released(fighter->left_key) || key_released(fighter->right_key))
        this->fighter->change_state(new IdleState, "Idle");
}