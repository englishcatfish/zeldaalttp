// TODO: Try to get this file to match using the old compiler

#include "gba/gba.h"
#include "global.h"

struct UnknownStruct1
{
    u8 unk0;
    u8 filler1[4];
    u8 unk5;
};

#define unk3000000 (*(struct UnknownStruct1 *)0x3000000)

#define SOFT_RESET_KEY_COMBO (A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON)

void AgbMain(void)
{
    s32 var;

  reset:
    initialize_gba();
    sub_08134684();
    sub_08067AE0();
    switch (sub_08068108())
    {
      case -1:
        break;
      case 0:
        break;
      case 1:
        sub_0803072C();
        break;
    }
    unk3000000.unk5 = 1;
    gUnknown_0202A8C0 = 1;
    gUnknown_03000030 = gUnknown_0844DD74;
    gUnknown_03000BA4 = gUnknown_0854F7E8;
    gUnknown_030059A0 = &gUnknown_030004A0;
    sub_0800BED8();
    sub_0800B85C(1);
    sub_0800D6F4();
    sub_0800D744();
    sub_0803668C();
    sub_0800C03C();
    gUnknown_03000F40 = 0x1234567;

    // copy some functions into RAM
    CpuCopy32((void *)sub_08008FA4, gUnknown_03000530, sizeof(gUnknown_03000530));
    CpuCopy32((void *)sub_080007C4, gUnknown_03000E40, sizeof(gUnknown_03000E40));
    CpuCopy32((void *)sub_080008A8, gUnknown_03005AE0, sizeof(gUnknown_03005AE0));
    CpuCopy32((void *)sub_0800023C, gUnknown_03006740, sizeof(gUnknown_03006740));
    gUnknown_03006C70 = gUnknown_03006740;
    while (1)
    {
        if ((gUnknown_03000BF0 & 0x180) != 0x100)
            VBlankIntrWait();
        else
            IntrWait(TRUE, INTR_FLAG_SERIAL);
        rom2_read_keys();
        gUnknown_03000BF0 = sub_0800C54C(gUnknown_03000FF8);
        if (is_reset_key_combo_pressed())
            goto reset;
        gUnknown_03000948.unk0++;
        sub_08036864();
        gUnknown_0842731C[gUnknown_03006D10]();
        sub_080368A0();
        sub_0800D788();
        sub_0800C118();
        sub_0802C940();
        sub_0800B524();
    }
}

void initialize_gba(void)
{
    main_disable_interrupts();
    REG_WAITCNT = 0x4014;

    // Clear memory regions
    DmaFill32(3, 0,                 (void *)EWRAM,          EWRAM_SIZE);
    DmaFill32(3, 0,                 (void *)(IWRAM + 0x10), IWRAM_SIZE - 0x210);
    DmaFill32(3, 0,                 (void *)VRAM,           VRAM_SIZE);
    DmaFill16(3, RGB_WHITE,         (void *)PLTT,           PLTT_SIZE);
    DmaFill32(3, 0xA0,              gUnknown_03005E20,      OAM_SIZE * 2);
    DmaCopy32(3, gUnknown_03005E20, (void *)OAM,            OAM_SIZE * 2);  // Why are we writing into unused memory?

    sub_0800B6BC();
    sub_080679CC(1, gUnknown_03000B94);
    REG_IE = INTR_FLAG_VBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_GAMEPAK;
    REG_DISPSTAT = DISPSTAT_VBLANK_INTR | DISPSTAT_VCOUNT_INTR;
    REG_IME = 1;
}

// Why can't I get this short thing to match?
#ifdef NONMATCHING
bool32 is_reset_key_combo_pressed(void)
{
    return ((gHeldKeys & SOFT_RESET_KEY_COMBO) == SOFT_RESET_KEY_COMBO);
}
#else
__attribute__((naked))
bool32 is_reset_key_combo_pressed(void)
{
    asm("mov r2, #0\n\
	ldr r0, _0800B2E0  @ =gHeldKeys\n\
	ldrh r1, [r0]\n\
	mov r0, #15\n\
	and r0, r0, r1\n\
	cmp r0, #15\n\
	bne _0800B2DC\n\
	mov r2, #1\n\
_0800B2DC:\n\
	add r0, r2, #0\n\
	bx lr\n\
_0800B2E0:\n\
	.4byte gHeldKeys");
}
#endif

void main_update_bg_regs(void)
{
    struct GPURegConfig *gpuConfig = &gUnknown_03000F50;

    REG_DISPCNT = gpuConfig->dispCnt;

    REG_BG0CNT = gpuConfig->bgConfig[0].bgcnt;
    REG_BG1CNT = gpuConfig->bgConfig[1].bgcnt;
    REG_BG2CNT = gpuConfig->bgConfig[2].bgcnt;
    REG_BG3CNT = gpuConfig->bgConfig[3].bgcnt;

    REG_BG0HOFS = gpuConfig->bgConfig[0].hofs;
    REG_BG0VOFS = gpuConfig->bgConfig[0].vofs;

    REG_BG1HOFS = gpuConfig->bgConfig[1].hofs;
    REG_BG1VOFS = gpuConfig->bgConfig[1].vofs;

    REG_BG2HOFS = gpuConfig->bgConfig[2].hofs;
    REG_BG2VOFS = gpuConfig->bgConfig[2].vofs;

    REG_BG3HOFS = gpuConfig->bgConfig[3].hofs;
    REG_BG3VOFS = gpuConfig->bgConfig[3].vofs;

    REG_BG2PA = gpuConfig->bg2pa;
    REG_BG2PB = gpuConfig->bg2pb;
    REG_BG2PC = gpuConfig->bg2pc;
    REG_BG2PD = gpuConfig->bg2pd;
    REG_BG2X_L = gpuConfig->bg2xl;
    REG_BG2X_H = gpuConfig->bg2xh;
    REG_BG2Y_L = gpuConfig->bg2yl;
    REG_BG2Y_H = gpuConfig->bg2yh;

    REG_BG3PA = gpuConfig->bg3pa;
    REG_BG3PB = gpuConfig->bg3pb;
    REG_BG3PC = gpuConfig->bg3pc;
    REG_BG3PD = gpuConfig->bg3pd;
    REG_BG3X_L = gpuConfig->bg3xl;
    REG_BG3X_H = gpuConfig->bg3xh;
    REG_BG3Y_L = gpuConfig->bg3yl;
    REG_BG3Y_H = gpuConfig->bg3yh;

    REG_WIN0H = gpuConfig->win0h;
    REG_WIN1H = gpuConfig->win1h;
    REG_WIN0V = gpuConfig->win0v;
    REG_WIN1V = gpuConfig->win1v;
    REG_WININ = gpuConfig->winin;
    REG_WINOUT = gpuConfig->winout;

    REG_MOSAIC = gpuConfig->mosaic;

    REG_BLDCNT = gpuConfig->bldcnt;
    REG_BLDALPHA = gpuConfig->bldalpha;
    REG_BLDY = gpuConfig->bldy;
}

void sub_0800B430(void)
{
    if (gUnknown_03000F50.bgConfig[0].unk6 != 0 && gUnknown_03000F50.bgConfig[0].src != NULL)
        sub_0800B4E0(&gUnknown_03000F50.bgConfig[0]);
    if (gUnknown_03000F50.bgConfig[1].unk6 != 0 && gUnknown_03000F50.bgConfig[1].src != NULL)
        sub_0800B4E0(&gUnknown_03000F50.bgConfig[1]);
    if (gUnknown_03000F50.bgConfig[2].unk6 != 0 && gUnknown_03000F50.bgConfig[2].src != NULL)
        sub_0800B4E0(&gUnknown_03000F50.bgConfig[2]);
    if (gUnknown_03000F50.bgConfig[3].unk6 != 0 && gUnknown_03000F50.bgConfig[3].src != NULL)
        sub_0800B4E0(&gUnknown_03000F50.bgConfig[3]);

    if (gUnknown_03000954[gUnknown_03000204 ^ 1] != 0 && (gUnknown_03000F50.dispCnt & 0x1000))
    {
        CpuFastCopy(gUnknown_03005E20 + (gUnknown_03000204 ^ 1) * 0x400, (void *)OAM, 0x400);
        gUnknown_03000954[gUnknown_03000204 ^ 1] = 0;
    }
}

void sub_0800B4E0(struct BGConfig *bgConfig)
{
    u16 bgcnt = bgConfig->bgcnt;
    u32 size = gUnknown_08427354[bgcnt >> 14];
    u32 offset = (0x800 * (bgcnt >> 8)) & 0xF800;
    void *dest = (void *)(VRAM + offset);

    u32 r3 = bgConfig->unk6;
    if (r3 != 0xFFFF)
        bgConfig->unk6--;

    CpuFastCopy(bgConfig->src, dest, size);
}

void sub_0800B524(void)
{
    while (gUnknown_0300050C != 0)
        ;
    gUnknown_03000204 ^= 1;
    gUnknown_03005E04 = gUnknown_03005E60 + gUnknown_03000204 * 0x400;
    gUnknown_03005E08 = gUnknown_03000E34;
    gUnknown_03000E34 = gUnknown_03006630 + gUnknown_03000204 * 0x80;
    gUnknown_0201EF14 = gUnknown_0200B310;
    gUnknown_02002480 = gUnknown_0201EDE0;
    gUnknown_0201EDE0 = 0;
    gUnknown_0200B310 = 0;
    CpuCopy32(gUnknown_030059A0, &gUnknown_03000F50, sizeof(gUnknown_03000F50));
    gUnknown_030059A0->bgConfig[3].unk6 = 0;
    gUnknown_030059A0->bgConfig[2].unk6 = 0;
    gUnknown_030059A0->bgConfig[1].unk6 = 0;
    gUnknown_030059A0->bgConfig[0].unk6 = 0;
    gUnknown_0300050C = 1;
}

void sub_0800B5EC(u32 a)
{
    gUnknown_03006D10 = a;
    gUnknown_03005050 = gUnknown_03000BFC = 0;
}

void main_disable_interrupts(void)
{
    REG_IME = 0;
    REG_IE = 0;
    REG_DISPSTAT = 0;
    REG_IF = 0;
    REG_IME = 0;  // again?

    DmaStop(0);
    DmaStop(1);
    DmaStop(2);
    DmaStop(3);

    REG_DISPCNT = 0x80;
}

void sub_0800B69C(void)
{
    sub_0800C4FC();
    sub_0800D24C(0x80070000);
    main_disable_interrupts();
    sub_0806940C();
}

void sub_0800B6B8(void)
{
}

void sub_0800B6BC(void)
{
    DmaCopy32(3, (void *)interrupt_main, gUnknown_030059B0, 0x120);
    gUnknown_03007FFC = gUnknown_030059B0;
    DmaCopy32(3, gUnknown_0842735C, &gUnknown_03000B70, 0x34);
}

void sub_0800B700(void)
{
    sub_0800C75C();
    sub_08134028();
    if (gUnknown_03000948.unk2 == 0 && gUnknown_0300050C != 0)
    {
        if (gUnknown_02000028[gUnknown_03000204 ^ 1] != 0)
        {
            void *src;
            void *dest;
            
            gUnknown_02000028[gUnknown_03000204 ^ 1] = 0;
            src = gUnknown_02000440 + (gUnknown_03000204 ^ 1) * 0x1000;
            dest = (void *)(VRAM + (gUnknown_0201F020->unk0 & 0x1F00) * 8);
            CpuFastCopy(src, dest, 0x1000);
        }
        sub_0800B430();
        main_update_bg_regs();
        sub_0800C8E4();
        sub_0800B9E4();
        gUnknown_0300050C = 0;
    }
    REG_IME = 0;
    gUnknown_03007FF8 |= 1;
    REG_IME = 1;
}

