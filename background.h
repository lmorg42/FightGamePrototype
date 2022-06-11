#include "splashkit.h"

class Background;

class BackgroundType
{
    protected:
        Background *background;
        string bundlename;
    
    public:
        virtual ~BackgroundType()
        {
            write_line("Background Type Destruction");
        };

        void set_background(Background *background)
        {
            this->background = background;
        };

        virtual void draw_background() = 0;
};

class Background
{
    private:
        BackgroundType *background;
        string type;

    public:
        Background(BackgroundType *background, string type) : background(nullptr)
        {
            this->change_background(background, type);
        };

        ~Background()
        {
            write_line("Background Destruction: " + type);
            delete background;
        };

        void change_background(BackgroundType *background, string type)
        {
            write_line("Changing Background To: " + type);
            if(this->background != nullptr)
                delete this->background;
            this->background = background;
            this->background->set_background(this);
        };

        void draw_background()
        {
            this->background->draw_background();
        };
};

class CityBackground : public BackgroundType
{
    public:
        CityBackground()
        {
            this->bundlename = "citybackground";
            if(!has_resource_bundle(bundlename))
            {
                load_resource_bundle(bundlename, "background_city.txt");
                write_line("Background Loaded: " + bundlename); 
            }
        };

        ~CityBackground()
        {
            free_resource_bundle(bundlename);
            write_line("Background Resources Free: " + bundlename); 
        };

        void draw_background() override;
};

void CityBackground::draw_background()
{
    point_2d cam = camera_position();
    draw_bitmap("farground",(cam.x - 100) - (0.5*(cam.x)),100,option_scale_bmp(1,1));
    draw_bitmap("background",(cam.x - 100) - (0.8*(cam.x)),0,option_scale_bmp(1,0.8));
    draw_bitmap("foreground",(cam.x + 400) - (1.6*(cam.x)),0,option_scale_bmp(1.5,1));
}