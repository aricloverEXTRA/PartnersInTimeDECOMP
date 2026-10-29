/*
 * Shop sub-screen text (overlay 9, 0x0207BBE0-0x0207BD84).
 *
 * Initializes the text drawn on the shop's lower screen.
 */

#include <game/shop_panels.h>
#include <game/shop_graphics.h>
extern u8 data_ov009_0207ea3c[];
extern u8 data_ov009_0207ea44;
extern ShopText *data_ov009_0207ea30;
void func_0203b7a0(u32 value, void *destination, u32 size);
void func_0203b7b4(const void *source, void *destination, u32 size);
#define FONTS ((const u32 *const *)(&data_ov009_0207ea44))
#define PIXELS (*(u8 **)(data_ov009_0207ea3c + 0x70))

void ShopSubscreenText_Init(void)
{
    GameTextToken token;
    /* Both original clear calls read their value from a stack word. */
    volatile u32 clear_background, clear_text;
    u16 entries[4][3] = {
        {40, 41, 41}, {42, 43, 43}, {44, 45, 45}, {46, 46, 46}
    };
    GameText text;
    u16 *map;
    int i;
    void *characters;
    const u8 *string;
    characters = Overlay5DisplayBg_GetCharacterVram(DISPLAY_ENGINE_SUB, 0);
    clear_background = 0;
    func_0203b7a0(clear_background, characters, 24576);
    map = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_SUB, 0);
    for (i = 0; i < 1024; ++i, ++map) *map = i;
    Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, 1);
    string = ShopText_GetString(data_ov009_0207ea30, 2,
        entries[data_ov009_0207ea3c[0x97]][data_ov009_0207ea3c[0x98]], 0);
    GameText_Init(&text, FONTS, PIXELS, string, 56, 2, 1, 4, 5, 6, 31, 0, 255, 0);
    clear_text = 0;
    func_0203b7a0(clear_text, PIXELS, 10240);
    for (;;) {
        if (!*text.text) break;
        token = GameText_Next(&text, 0, 0);
    }
    characters = Overlay5DisplayBg_GetCharacterVram(DISPLAY_ENGINE_SUB, 0);
    func_0203b7b4(PIXELS, (u8 *)characters + 6144, 10240);
}
