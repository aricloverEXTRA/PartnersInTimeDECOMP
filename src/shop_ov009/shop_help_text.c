/*
 * Shop help text (overlay 9, 0x0207A844-0x0207A9A8).
 *
 * The help line at the bottom of the shop and the background it is drawn on.
 */

#include <game/shop_panels.h>
extern u8 data_ov009_0207ea3c[];
extern u8 data_ov009_0207ea44;
extern ShopText *data_ov009_0207ea30;
void func_0203b7a0(u32 value, void *destination, u32 size);
void func_0203b7b4(const void *source, void *destination, u32 size);

#define HELP_FONTS ((const u32 *const *)(&data_ov009_0207ea44))
#define HELP_PIXELS (*(u8 **)(data_ov009_0207ea3c + 0x6c))

/* The original fill calls load the clear value from a stack word. */
void ShopHelpText_InitBackground(void)
{
    void *characters = Overlay5DisplayBg_GetCharacterVram(DISPLAY_ENGINE_MAIN, 0);
    volatile u32 clear = 0;
    u16 *map;
    int i;
    func_0203b7a0(clear, characters, 24576);
    map = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_MAIN, 0);
    for (i = 0; i < 1024; ++i, ++map)
        *map = i;
}

void ShopHelpText_Draw(int kind, u16 entry)
{
    GameText text;
    /* C struct-return storage reproduces the native text-token stack slot. */
    GameTextToken token;
    volatile u32 clear;
    u8 *characters;
    const u8 *string = ShopText_GetString(data_ov009_0207ea30, kind, entry, 0);
    GameText_Init(&text, HELP_FONTS, HELP_PIXELS, string,
        16, 5, 1, 0, 1, 6, 31, 0, 255, 0);
    clear = 0;
    func_0203b7a0(clear, HELP_PIXELS, 5120);
    for (;;) {
        if (!*text.text)
            break;
        token = GameText_Next(&text, 0, 0);
    }
    characters = (u8 *)Overlay5DisplayBg_GetCharacterVram(DISPLAY_ENGINE_MAIN, 0);
    func_0203b7b4(HELP_PIXELS, characters + 20480, 4096);
}
