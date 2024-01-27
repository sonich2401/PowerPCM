#pragma once
#include "types.h"

#define FORCE_PACK __attribute__ ((packed))

typedef struct FORCE_PACK{
    u8 opcode : 6;
}GENERIC_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    u32 LI : 24;
    u8 AA : 1;
    u8 LK : 1;
}I_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    u8 BO : 6;
    u8 BI : 6;
    u16 BD : 12;
    u8 AA : 1;
    u8 LK : 1;
}B_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    u32 padding : 14;
    u8 LEV : 7;
    u8 padding2 : 2;
    u8 always_one : 1;
    u8 padding3 : 1;
}SC_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
        u8 TO : 5;
        u8 FRT : 5;
        u8 FRS : 5;
        struct FORCE_PACK{
            u8 BF : 3;
            u8 L : 1;
        };
    };
    
    u8 RA : 5;
    
    union FORCE_PACK{
        u32 D : 16;
        s32 SI : 16;
        u32 UI : 16;
    };    
}D_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
    };
    
    u8 RA : 5;
    
    u32 DS : 14;
    u8 XO : 2;
}DS_FORM_t;


typedef struct FORCE_PACK{
    u8 opcode : 6;
    union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
        u8 TO : 5;
        u8 FRT : 5;
        u8 FRS : 5;
        struct FORCE_PACK{
            u8 BF : 3;
            
            union{
                struct{
                    u8 BF_padding : 1;
                    u8 L : 1;
                };
                u8 BIG_L : 2;
            };
        };
        struct FORCE_PACK{
            u8 TH_padding : 1;
            u8 TH : 4;
        };
    };
    
    union{
        u8 RA : 5;
        u8 FRA : 5;
        u8 BFA : 3;
        struct FORCE_PACK{
            u8 RA_padding : 1;
            u8 SR : 4;
        };
    };
    
    union{
        u8 RB : 5;
        u8 NB : 5;
        u8 SH : 5;
        u8 FRB : 5;
        u8 U : 4;
    };

    u32 XO : 10;

    u8 Rc : 1;
}X_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    union{
        u8 BT : 5;
        u8 BO : 5;
        u8 BF : 3;
    };

    union{
        u8 BA : 5;
        u8 BI : 5;
        u8 BFA : 3;
    };

    union{
        u8 BB : 5;
        struct FORCE_PACK{
            u8 BH_padding : 3;
            u8 BH : 2; 
        };
    };
    

    u32 XO : 10;
    u8 LK : 1;
}XL_FORM_t;


typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
    };

    union FORCE_PACK{
        u16 spr : 10;
        u16 tbr : 10;
        struct FORCE_PACK{
            union FORCE_PACK {
                u8 always_1 : 1;
                u8 always_0 : 1;
            };
            u8 FXM : 8;
        };
    };


    u32 XO : 10;
}XFX_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 padding1 : 1;
    u8 FLM : 8;
    u8 padding2 : 1;
    u8 FRB : 5;

    u32 XO : 10;

    u8 Rc : 1;
}XFL_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 RS : 5;
    u8 RA : 5;
    u8 sh : 5;


    u16 XO : 9;

    u8 sh : 1;
    u8 Rc : 1;
}XS_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 RT : 5;
    u8 RA : 5;
    u8 RB : 5;

    u8 OE : 1;
    u16 XO : 9;

    u8 Rc : 1;
}XO_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 FRT : 5;
    u8 FRA : 5;
    u8 FRB : 5;
    u8 FRC : 5;

    u8 XO : 4;
    u8 Rc : 1;
}A_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 RS : 5;
    u8 RA : 5;
    union FORCE_PACK{
        u8 RB : 5;
        u8 SH : 5;
    };
    u8 MB : 5;
    u8 ME : 4;
    u8 Rc : 1;
}M_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 RS : 5;
    u8 RA : 5;
    u8 sh : 5;
    union FORCE_PACK {
        u8 mb : 5;
        u8 me : 5;
    };
    u8 XO : 4;
    u8 sh : 1;
    u8 Rc : 1;
}MD_FORM_t;

typedef struct FORCE_PACK{
    u8 opcode : 6;
    
    u8 RS : 5;
    u8 RA : 5;
    u8 sh : 5;
    union FORCE_PACK {
        u8 mb : 5;
        u8 me : 5;
    };
    u8 XO : 5;
    u8 Rc : 1;
}MDS_FORM_t;

