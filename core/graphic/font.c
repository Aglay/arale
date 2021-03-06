#include "font.h"

char* font_list[0xff];

char font_space[] = { 
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

char font_cap_a[]= { 
0x0,0x18,0x18,0x18,0x18,0x24,0x24,0x24,0x24,0x7e,0x42,0x42,0x42,0xe7,0x0,0x0};

char font_cap_b[]= { 
0x0,0xf0,0x48,0x44,0x44,0x44,0x48,0x78,0x44,0x42,0x42,0x42,0x44,0xf8,0x0,0x0};

char font_cap_c[]= { 
0x0,0x3a,0x46,0x42,0x82,0x80,0x80,0x80,0x80,80,0x82,0x42,0x44,0x38,0x0,0x0};

char font_cap_d[]= { 
0x0,0xf8,0x44,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0x44,0xf8,0x0,0x0};

char font_cap_e[]= { 
0x0,0xfe,0x42,0x42,0x40,0x40,0x44,0x7c,0x44,0x40,0x40,0x42,0x42,0xfe,0x0,0x0};

char font_cap_f[]= { 
0x0,0xfe,0x42,0x42,0x40,0x40,0x44,0x7c,0x44,0x44,0x40,0x40,0x40,0xf0,0x0,0x0};

char font_cap_g[]= { 
0x0,0x3a,0x46,0x42,0x82,0x80,0x80,0x9e,0x82,0x82,0x82,0x42,0x46,0x38,0x0,0x0};

char font_cap_h[]= { 
0x0,0xe7,0x42,0x42,0x42,0x42,0x42,0x7e,0x42,0x42,0x42,0x42,0x42,0xe7,0x0,0x0};

char font_cap_i[]= { 
0x0,0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x0,0x0};

char font_cap_j[]= { 
0x0,0x1f,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x84,0x48,0x30,0x0};

char font_cap_k[]= { 
0x0,0xe7,0x42,0x44,0x48,0x50,0x50,0x60,0x50,0x50,0x48,0x44,0x42,0xe7,0x0,0x0};

char font_cap_l[]= { 
0x0,0xf0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0x42,0xfe,0x0,0x0};

char font_cap_m[]= { 
0x0,0xc3,0x42,0x66,0x66,0x66,0x5a,0x5a,0x5a,0x42,0x42,0x42,0x42,0xe7,0x0,0x0};

char font_cap_n[]= { 
0x0,0xc7,0x42,0x62,0x62,0x52,0x52,0x52,0x4a,0x4a,0x4a,0x46,0x46,0xe2,0x0,0x0};

char font_cap_o[]= { 
0x0,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x0,0x0};

char font_cap_p[]= { 
0x0,0xf8,0x44,0x42,0x42,0x42,0x44,0x78,0x40,0x40,0x40,0x40,0x40,0xf0,0x0,0x0};

char font_cap_q[]= { 
0x0,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x92,0x8a,0x44,0x3a,0x0,0x0};

char font_cap_r[]= { 
0x0,0xfc,0x42,0x42,0x42,0x42,0x7c,0x44,0x42,0x42,0x42,0x42,0x42,0xe7,0x0,0x0};

char font_cap_s[]= { 
0x0,0x3a,0x46,0x82,0x82,0x80,0x40,0x38,0x4,0x2,0x82,0x82,0xc4,0xb8,0x0,0x0};

char font_cap_t[]= { 
0x0,0xfe,0x92,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x0,0x0};

char font_cap_u[]= { 
0x0,0xe7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x3c,0x0,0x0};

char font_cap_v[]= { 
0x0,0xe7,0x42,0x42,0x42,0x42,0x24,0x24,0x24,0x24,0x18,0x18,0x18,0x18,0x0,0x0};

char font_cap_w[]= { 
0x0,0xe7,0x42,0x42,0x42,0x5a,0x5a,0x5a,0x5a,0x24,0x24,0x24,0x24,0x24,0x0,0x0};

char font_cap_x[]= { 
0x0,0xe7,0x42,0x42,0x24,0x24,0x24,0x18,0x24,0x24,0x24,0x42,0x42,0xe7,0x0,0x0};

char font_cap_y[]= { 
0x0,0xee,0x44,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x7c,0x0,0x0};

char font_cap_z[]= { 
0x0,0xfe,0x84,0x84,0x8,0x8,0x10,0x10,0x20,0x20,0x40,0x42,0x82,0xfe,0x0,0x0};

char font_a[]= { 
0x0,0x0,0x0,0x0,0x0,0x70,0x8,0x4,0x3c,0x44,0x84,0x84,0x8c,0x76,0x0,0x0};

char font_b[]= { 
0xc0,0x40,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x64,0x58,0x0,0x0};

char font_c[]= { 
0x0,0x0,0x0,0x0,0x0,0x30,0x4c,0x84,0x84,0x80,0x80,0x82,0x44,0x38,0x0,0x0};

char font_d[]= { 
0xc,0x4,0x4,0x4,0x4,0x34,0x4c,0x84,0x84,0x84,0x84,0x84,0x4c,0x36,0x0,0x0};

char font_e[]= { 
0x0,0x0,0x0,0x0,0x0,0x38,0x44,0x82,0x82,0xfc,0x80,0x82,0x42,0x3c,0x0,0x0};

char font_f[]= { 
0xe,0x10,0x10,0x10,0x10,0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x0,0x0};

char font_g[]= { 
0x0,0x0,0x0,0x0,0x0,0x36,0x4c,0x84,0x84,0x84,0x84,0x4c,0x34,0x4,0x4,0x78};

char font_h[]= { 
0xc0,0x40,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x42,0xe3,0x0,0x0};

char font_i[]= { 
0x0,0x10,0x10,0x0,0x0,0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x0,0x0};

char font_j[]= { 
0x0,0x4,0x4,0x0,0x0,0xc,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x8,0x8,0x30};

char font_k[]= { 
0xc0,0x40,0x40,0x40,0x40,0x4e,0x44,0x48,0x50,0x60,0x50,0x48,0x44,0xe6,0x0,0x0};

char font_l[]= { 
0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x0,0x0};

char font_m[]= { 
0x0,0x0,0x0,0x0,0x0,0xf6,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xdb,0x0,0x0};

char font_n[]= { 
0x0,0x0,0x0,0x0,0x0,0xd8,0x64,0x42,0x42,0x42,0x42,0x42,0x42,0xe3,0x0,0x0};

char font_o[]= { 
0x0,0x0,0x0,0x0,0x0,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x0,0x0};

char font_p[]= { 
0x0,0x0,0x0,0x0,0x0,0xd8,0x64,0x42,0x42,0x42,0x42,0x42,0x64,0x58,0x40,0xe0};

char font_q[]= { 
0x0,0x0,0x0,0x0,0x0,0x34,0x4c,0x84,0x84,0x84,0x84,0x84,0x4c,0x34,0x4,0xe};

char font_r[]= { 
0x0,0x0,0x0,0x0,0x0,0xdc,0x62,0x42,0x40,0x40,0x40,0x40,0x40,0xe0,0x0,0x0};

char font_s[]= { 
0x0,0x0,0x0,0x0,0x0,0x7a,0x86,0x82,0xc0,0x38,0x6,0x82,0xc2,0xbc,0x0,0x0};

char font_t[]= { 
0x0,0x0,0x10,0x10,0x10,0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xe,0x0,0x0};

char font_u[]= { 
0x0,0x0,0x0,0x0,0x0,0xc6,0x42,0x42,0x42,0x42,0x42,0x42,0x46,0x3b,0x0,0x0};

char font_v[]= { 
0x0,0x0,0x0,0x0,0x0,0xe7,0x42,0x42,0x42,0x24,0x24,0x24,0x18,0x18,0x0,0x0};

char font_w[]= { 
0x0,0x0,0x0,0x0,0x0,0xe7,0x42,0x42,0x5a,0x5a,0x5a,0x24,0x24,0x24,0x0,0x0};

char font_x[]= { 
0x0,0x0,0x0,0x0,0x0,0xc6,0x44,0x28,0x28,0x10,0x28,0x28,0x44,0xc6,0x0,0x0};

char font_y[]= { 
0x0,0x0,0x0,0x0,0x0,0xe7,0x42,0x42,0x24,0x24,0x24,0x18,0x18,0x10,0x10,0x60};

char font_z[]= { 
0x0,0x0,0x0,0x0,0x0,0xfe,0x82,0x84,0x8,0x10,0x20,0x42,0x82,0xfe,0x0,0x0};
//char font_z[]= { 
//0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00,0x00,0x0,0x0,0x0,0x0,0x0};

char font_0[] = { 
0x0,0x18,0x24,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x24,0x18,0x0,0x0};

char font_1[] = { 
0x0,0x8,0x18,0x28,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x3e,0x0,0x0};

char font_2[] = { 
0x0,0x18,0x24,0x42,0x42,0x2,0x4,0x8,0x10,0x20,0x20,0x40,0x40,0x7e,0x0,0x0};

char font_3[] = { 
0x0,0x18,0x24,0x42,0x2,0x2,0x4,0x18,0x4,0x2,0x2,0x42,0x24,0x18,0x0,0x0};

char font_4[] = { 
0x0,0xc,0xc,0xc,0x14,0x14,0x14,0x24,0x24,0x44,0x7e,0x4,0x4,0x1e,0x0,0x0};

char font_5[] = { 
0x0,0x7c,0x40,0x40,0x40,0x58,0x64,0x2,0x2,0x2,0x2,0x42,0x24,0x18,0x0,0x0};

char font_6[] = { 
0x0,0x18,0x24,0x42,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x0,0x0};

char font_7[] = { 
0x0,0x7e,0x42,0x42,0x4,0x4,0x8,0x8,0x8,0x10,0x10,0x10,0x10,0x38,0x0,0x0};

char font_8[] = { 
0x0,0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x0,0x0};

char font_9[] = { 
0x0,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x26,0x1a,0x2,0x42,0x24,0x18,0x0,0x0};

void init_font()
{
    font_list[' '] = font_space;
    font_list['a'] = font_a;
    font_list['b'] = font_b;
    font_list['c'] = font_c;
    font_list['d'] = font_d;
    font_list['e'] = font_e;
    font_list['f'] = font_f;
    font_list['g'] = font_g;
    font_list['h'] = font_h;
    font_list['i'] = font_i;
    font_list['j'] = font_j;
    font_list['k'] = font_k;
    font_list['l'] = font_l;
    font_list['m'] = font_m;
    font_list['n'] = font_n;
    font_list['o'] = font_o;
    font_list['p'] = font_p;
    font_list['q'] = font_q;
    font_list['r'] = font_r;
    font_list['s'] = font_s;
    font_list['t'] = font_t;
    font_list['u'] = font_u;
    font_list['v'] = font_v;
    font_list['w'] = font_w;
    font_list['x'] = font_x;
    font_list['y'] = font_y;
    font_list['z'] = font_z;
    font_list['A'] = font_cap_a;
    font_list['B'] = font_cap_b;
    font_list['C'] = font_cap_c;
    font_list['D'] = font_cap_d;
    font_list['E'] = font_cap_e;
    font_list['F'] = font_cap_f;
    font_list['G'] = font_cap_g;
    font_list['H'] = font_cap_h;
    font_list['I'] = font_cap_i;
    font_list['J'] = font_cap_j;
    font_list['K'] = font_cap_k;
    font_list['L'] = font_cap_l;
    font_list['M'] = font_cap_m;
    font_list['N'] = font_cap_n;
    font_list['O'] = font_cap_o;
    font_list['P'] = font_cap_p;
    font_list['Q'] = font_cap_q;
    font_list['R'] = font_cap_r;
    font_list['S'] = font_cap_s;
    font_list['T'] = font_cap_t;
    font_list['U'] = font_cap_u;
    font_list['V'] = font_cap_v;
    font_list['W'] = font_cap_w;
    font_list['X'] = font_cap_x;
    font_list['Y'] = font_cap_y;
    font_list['Z'] = font_cap_z;

    font_list['0'] = font_0;
    font_list['1'] = font_1;
    font_list['2'] = font_2;
    font_list['3'] = font_3;
    font_list['4'] = font_4;
    font_list['5'] = font_5;
    font_list['6'] = font_6;
    font_list['7'] = font_7;
    font_list['8'] = font_8;
    font_list['9'] = font_9;
    //int cursor
    
}
