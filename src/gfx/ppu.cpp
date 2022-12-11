#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "vector.h"

#include <pthread.h>
#include "../bus.h"
#include "ppu.h"

static olc::PixelGameEngine* pge = nullptr;
static volatile bool sys_ready = false;

volatile bool command_waiting = false;
pthread_mutex_t mutex;

static volatile vector commands_waiting; //GFX_Command

struct driver_settings{
    unsigned int w;
    unsigned int h;
    unsigned char scale;
    bool vsync : 1;
    bool full : 1;
};

typedef struct{
    GFX_type t;
    union{
        driver_settings GFX_SETTINGS;
        struct{
            unsigned int x;
            unsigned int y;
            pixel pix;
        }GFX_PIXEL;
        struct{
            unsigned int x;
            unsigned int y;
            char* text;
            pixel color;
            unsigned char scale;
        }GFX_TEXT;
    };
}GFX_Command;


static volatile pixel* fb[2]; //FrameBuffer
static volatile unsigned char fb_index = 0;

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "PPCM";
        pge = this;
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
        pthread_mutex_lock(&mutex);
        fb[0] = (pixel*)malloc(sizeof(pixel) * this->GetWindowSize().x * this->GetWindowSize().y);
        fb[1] = (pixel*)malloc(sizeof(pixel) * this->GetWindowSize().x * this->GetWindowSize().y);
        sys_ready = true;
        pthread_mutex_unlock(&mutex);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels

        if(command_waiting){
            this->Clear(olc::BLACK);
            pthread_mutex_lock(&mutex);
            for(unsigned int x = 0; x < this->GetDrawTargetWidth(); x++){
                for(unsigned int y = 0; y < this->GetDrawTargetHeight(); y++){
                    Draw({x, y}, *(olc::Pixel*)&fb[fb_index^1][y * this->GetDrawTargetWidth() + x]);
                }
            }
            command_waiting = false;
            pthread_mutex_unlock(&mutex);
        }

        
		return true;
	}

    bool OnUserDestroy() override{
        pthread_mutex_lock(&mutex);
        free((void*)fb[0]);
        fb[0] = NULL;
        free((void*)fb[1]);
        fb[1] = NULL;
        pthread_mutex_unlock(&mutex);
        exit(EXIT_SUCCESS);
        return true;
    }
};

static olc::Sprite* spr[10] = {nullptr};

static driver_settings settings;
static pthread_t thread;

static void* driver_thread(void* args){
    (void)args;

    Example demo;
	if (demo.Construct(settings.w, settings.h, settings.scale, settings.scale, settings.full, settings.vsync))
		demo.Start();
    return args;
}

void ppu_driver_init(unsigned char scale, bool vsync, bool fullscreen){
    if(pge != NULL){
        fprintf(stderr, "WARN: PPU was already created but was created again! Skipping command ...\n");
        return;
    }
    vector_init((vector*)&commands_waiting, sizeof(GFX_Command), NULL);
    pthread_mutex_init(&mutex, NULL);

    settings.w = SCREEN_W;
    settings.h = SCREEN_H;
    settings.scale = scale;
    settings.vsync = vsync;
    settings.full = fullscreen;
    pthread_create(&thread, NULL, driver_thread, (void*)&settings); //TODO: Hanle thread creation errors

    while(!sys_ready){} //wait until commands are ready to be processed
}


void ppu_send_screen(){
    pthread_mutex_lock(&mutex);
    if(fb[fb_index] == NULL) {pthread_mutex_unlock(&mutex);return;};
    memcpy((pixel*)fb[fb_index], &bus.FRAMEBUFFER[0], FRAMEBUFFER_SIZE);
    command_waiting = true;
    fb_index ^= 1;
    pthread_mutex_unlock(&mutex);
}



#include "../cpu.h"

void ppu_spr(){

}


void ppu_process_ext_device_command(void){
    switch(cpu.ru[3]){
        case 0:
            ppu_driver_init(cpu.ru[4], cpu.ru[5] & 0b1, cpu.ru[5] & 0b10);
        break;

        case 1:
            ppu_send_screen();
        break;

        case 2:
            //Futre spot for ppu_spr for OAM
        break;

        default:
            fprintf(stderr, "ERR: Invalid GFX Command [%llu]! PC = %llX\n", cpu.ru[3], cpu.pc);
            exit(EXIT_FAILURE);
        break;
    }
}