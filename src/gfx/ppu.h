#ifndef DRIVER_H
#define DRIVER_H


#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif
#include "types.h"

typedef struct{
    bool _free;
    unsigned x;
    unsigned y;
    ptr_t spr_data;
}OAM_obj;

#define OAM_SIZE 10

typedef struct{
    OAM_obj objs[OAM_SIZE];
}OAM_t;



#include <stdint.h>

typedef union{
    struct{
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    unsigned char rgb[3];
    uint32_t full_int;
}pixel;

typedef enum{
    PIXEL,
    TEXT
}GFX_type;

//void ppu_driver_init(unsigned char scale, bool vsync, bool fullscreen);

//void ppu_driver_text(unsigned int x, unsigned int y, char* __restrict str, pixel pix, unsigned char scale);

//void ppu_driver_pixel(unsigned int x, unsigned int y, pixel pix);

void ppu_process_ext_device_command(void);

#ifdef __cplusplus
};
#endif


#endif