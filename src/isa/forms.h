#pragma once
#include "types.h"



typedef enum{
	FORM_I,
	FORM_D,
	FORM_DS,
	FORM_B,
	FORM_SC,
	FORM_X,
	FORM_XL,
	FORM_XFX,
	FORM_XFL,
	FORM_XS,
	FORM_XO,
	FORM_A,
	FORM_M,
	FORM_MD,
	FORM_MDS,
}FORM_MODE_t;


//In order to ignore bitfeild warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked-bitfield-compat"




#define FORCE_PACK __attribute__ ((packed))

typedef struct FORCE_PACK{
	union{
		struct{
			u32 padding : 26;
    		u8 opcode : 6;
		};
    	u32 full;
    };
}GENERIC_FORM_t;

typedef struct FORCE_PACK{
    u8 LK : 1;
    u8 AA : 1;
	u32 LI : 24;
    u8 opcode : 6;
}I_FORM_t;

typedef struct FORCE_PACK{
    u8 LK : 1;
	u8 AA : 1;
	u16 BD : 12;
	u8 BI : 6;
    u8 BO : 6;
    u8 opcode : 6;
}B_FORM_t;

typedef struct FORCE_PACK{
    u8 padding3 : 1;
	u8 always_one : 1;
	u8 padding2 : 2;
	u8 LEV : 7;
	u32 padding : 14;
    u8 opcode : 6;
}SC_FORM_t;

typedef struct FORCE_PACK{
    union FORCE_PACK{
        u32 D : 16;
        s32 SI : 16;
        u32 UI : 16;
    };    

	u8 RA : 5;
	
    union FORCE_PACK{
	    u8 RT : 5;
	    u8 RS : 5;
	    u8 TO : 5;
	    u8 FRT : 5;
	    u8 FRS : 5;
	    struct FORCE_PACK{
			u8 L : 1;
	        u8 BF_padding : 1;
	        u8 BF : 3;
	    };
	};

    u8 opcode : 6;
}D_FORM_t;

typedef struct FORCE_PACK{
	u8 XO : 2;
    u32 DS : 14;
	u8 RA : 5;
    union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
    };
	u8 opcode : 6;
}DS_FORM_t;


typedef struct FORCE_PACK{
	u8 Rc : 1;
    
    u32 XO : 10;

    union FORCE_PACK{
	    u8 RB : 5;
	    u8 NB : 5;
	    u8 SH : 5;
	    u8 FRB : 5;
	    u8 U : 4;
	};

	union FORCE_PACK{
        u8 RA : 5;
        u8 FRA : 5;
        u8 SR : 4;
        struct FORCE_PACK{
            u8 RA_padding : 2;
            u8 BFA : 3;
        };
    };

	union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
        u8 TO : 5;
        u8 FRT : 5;
        u8 FRS : 5;
        u8 TH : 2;
        struct FORCE_PACK{
            union{
                struct{
                    u8 L : 1;
                    u8 BF_padding : 1;
                };
                u8 BIG_L : 2;
            };
            u8 BF : 3;
        };
    };

    u8 opcode : 6;
}X_FORM_t;

typedef struct FORCE_PACK{
	u8 LK : 1;
    u32 XO : 10;
    
    union FORCE_PACK{
        u8 BB : 5;
        u8 BH : 2;
    };

	union FORCE_PACK{
        u8 BA : 5;
        u8 BI : 5;
        struct FORCE_PACK{
        	u8 BFA_padding : 2;
        	u8 BFA : 3;
        };
    };

	union FORCE_PACK{
        u8 BT : 5;
        u8 BO : 5;
        struct{
			u8 BF_padding : 2;
        	u8 BF : 3;
        };
    };

    u8 opcode : 6;
}XL_FORM_t;


typedef struct FORCE_PACK{
	u8 padding : 1;

	u32 XO : 10;

	union FORCE_PACK{
        u16 spr : 10;
        u16 tbr : 10;
        struct FORCE_PACK{
        	u8 FXM_padding : 1;
            u8 FXM : 8;
            u8 always_1_or_0 : 1;
        };
    };
    
	union FORCE_PACK{
        u8 RT : 5;
        u8 RS : 5;
    };
    
    u8 opcode : 6;
}XFX_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;

	u32 XO : 10;
	u8 FRB : 5;
	u8 padding2 : 1;
	u8 FLM : 8;
    u8 padding1 : 1;

    u8 opcode : 6;
}XFL_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;
	u8 sh1 : 1;
	u16 XO : 9;
	u8 sh : 5;
	u8 RA : 5;
	u8 RS : 5;
    u8 opcode : 6;
}XS_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;
	u16 XO : 9;
	u8 OE : 1;
	u8 RB : 5;
	u8 RA : 5;
	u8 RT : 5;
    u8 opcode : 6;
}XO_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;
	u8 XO : 4;
	u8 FRC : 5;
	u8 FRB : 5;
	u8 FRA : 5;
	u8 FRT : 5;
    u8 opcode : 6;
}A_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;
	u8 ME : 4;
	u8 MB : 5;
	union FORCE_PACK{
        u8 RB : 5;
        u8 SH : 5;
    };
	u8 RA : 5;
	u8 RS : 5;
	u8 opcode : 6;
}M_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;

	u8 sh1 : 1;
	u8 XO : 4;
	union FORCE_PACK {
        u8 mb : 5;
        u8 me : 5;
    };
    u8 sh : 5;
	u8 RA : 5;
	u8 RS : 5;
	
    u8 opcode : 6;
}MD_FORM_t;

typedef struct FORCE_PACK{
    u8 Rc : 1;

	u8 XO : 5;
	union FORCE_PACK {
        u8 mb : 5;
        u8 me : 5;
    };
	u8 sh : 5;
	u8 RA : 5;
	u8 RS : 5;
	
    u8 opcode : 6;
}MDS_FORM_t;

#pragma GCC diagnostic pop