; Do not change these
.macro gfx_width: 640
.macro gfx_height: 320

; You can change these
.macro gfx_scale: 1
.macro gfx_vsync: 0
.macro gfx_fullscreen: 0

.macro GFX_ENUM: 4
.macro GFX_INIT: 0
.macro GFX_SEND_SCREEN: 1

; Create the screen with default settings
init_gfx:
    li r3, GFX_INIT
    li r4, gfx_scale
    li r5, gfx_fullscreen
    sli r5, r5, 1
    addi r5, r5, gfx_vsync
    sc GFX_ENUM
    blr

start:
    bl init_gfx
start2:
    li r4, 255
    li r5, __FRAMEBUFFER_START__
    li r3, GFX_SEND_SCREEN
    li r6, 0
    li r7, 0
    loop:
        sb r4, 3(r5) ; Set transparency to 255 (meaning no transparency)
        sb r7, 0(r5) ; R
        sb r7, 1(r5) ; G
        sb r7, 2(r5) ; B
        addi r5, r5, 4 ; Move to next pixel
        addi r6, r6, 1 ; x++
        cmpi r6, gfx_width ; See if we have gone past the edge of the screen
        blt loop ; If we havent then loop again
        addi r7, r7, 1 ; y++
        li r6, 0 ; x = 0
        cmpi r7, gfx_height ; Did we reach the end of the screen?
        blt loop
        sc 4 ; We reached the end of the render. Go back to top and re-render screen
        b start2
    end 0