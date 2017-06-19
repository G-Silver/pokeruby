#include "global.h"
#include "gba/m4a_internal.h"
#include "battle.h"
#include "decompress.h"
#include "m4a.h"
#include "main.h"
#include "pokeball.h"
#include "pokemon.h"
#include "rom_8077ABC.h"
#include "songs.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "util.h"

extern struct MusicPlayerInfo gMPlay_BGM;
extern u16 gBattleTypeFlags;
extern u8 gBankTarget;
extern u8 gActiveBank;
extern u16 gBattlePartyID[];
extern u8 gObjectBankIDs[];
extern u8 gDoingBattleAnim;
extern u8 gHealthboxIDs[];

extern const struct SpriteSheet gUnknown_0820A92C[];
extern const struct SpritePalette gUnknown_0820A98C[];
extern const struct SpriteTemplate gSpriteTemplate_820AAB4[];
extern const u8 gUnknown_08D030D0[];

extern u32 ball_number_to_ball_processing_index(u16);  // not sure of return type
extern void sub_80786EC();
extern bool8 sub_8078718(struct Sprite *);
extern u8 sub_814086C(u8, u8, int, int, u8);
extern u8 sub_8141314(u8, u8, int, u8);

static void sub_8046464(u8);
static void sub_80466E8(struct Sprite *);
static void sub_80466F4(struct Sprite *);
static void sub_8046760(struct Sprite *);
static void sub_80467F8(struct Sprite *);
static void sub_804684C(struct Sprite *);
static void sub_8046944(struct Sprite *);
static void sub_8046984(struct Sprite *);
static void sub_8046C78(struct Sprite *);
static void sub_8046E7C(struct Sprite *);
static void sub_8046E9C(struct Sprite *);
static void sub_8046FBC(struct Sprite *);
static void sub_8047074(struct Sprite *);
static void sub_80470C4(struct Sprite *);
static void sub_8047230(struct Sprite *);
static void sub_8047254(struct Sprite *);
static void sub_80473D0(struct Sprite *);
static void sub_804748C(struct Sprite *);
static void sub_8047638(struct Sprite *);
static void sub_80476E0(struct Sprite *);
static void sub_8047754(struct Sprite *);
static void sub_804780C(struct Sprite *);
static void sub_8047830(struct Sprite *);
static void oamc_804BEB4(struct Sprite *);
static u16 sub_8047978(u8);

u8 sub_8046400(u16 a, u8 b)
{
    u8 taskId;

    gDoingBattleAnim = 1;
    ewram17810[gActiveBank].unk0_3 = 1;
    taskId = CreateTask(sub_8046464, 5);
    gTasks[taskId].data[1] = a;
    gTasks[taskId].data[2] = b;
    gTasks[taskId].data[3] = gActiveBank;
    return 0;
}

static void sub_8046464(u8 taskId)
{
    bool8 sp0 = FALSE;
    u16 r8;
    u8 r5;
    u16 ball;
    u8 r4;
    u8 spriteId;

    if (gTasks[taskId].data[0] == 0)
    {
        gTasks[taskId].data[0]++;
        return;
    }
    r8 = gTasks[taskId].data[2];
    r5 = gTasks[taskId].data[3];
    if (GetBankSide(r5) != 0)
        ball = GetMonData(&gEnemyParty[gBattlePartyID[r5]], MON_DATA_POKEBALL);
    else
        ball = GetMonData(&gPlayerParty[gBattlePartyID[r5]], MON_DATA_POKEBALL);
    r4 = ball_number_to_ball_processing_index(ball);
    sub_80478DC(r4);
    spriteId = CreateSprite(&gSpriteTemplate_820AAB4[r4], 32, 80, 0x1D);
    gSprites[spriteId].data0 = 0x80;
    gSprites[spriteId].data1 = 0;
    gSprites[spriteId].data7 = r8;
    switch (r8)
    {
    case 0xFF:
        gBankTarget = r5;
        gSprites[spriteId].pos1.x = 0x18;
        gSprites[spriteId].pos1.y = 0x44;
        gSprites[spriteId].callback = sub_8047074;
        break;
    case 0xFE:
        gSprites[spriteId].pos1.x = sub_8077ABC(r5, 0);
        gSprites[spriteId].pos1.y = sub_8077ABC(r5, 1) + 0x18;
        gBankTarget = r5;
        gSprites[spriteId].data0 = 0;
        gSprites[spriteId].callback = sub_8047254;
        break;
    default:
        gBankTarget = GetBankByPlayerAI(1);
        sp0 = TRUE;
        break;
    }
    gSprites[spriteId].data6 = gBankTarget;
    if (!sp0)
    {
        DestroyTask(taskId);
        return;
    }
    gSprites[spriteId].data0 = 0x22;
    gSprites[spriteId].data2 = sub_8077ABC(gBankTarget, 0);
    gSprites[spriteId].data4 = sub_8077ABC(gBankTarget, 1) - 16;
    gSprites[spriteId].data5 = -40;
    sub_80786EC(&gSprites[spriteId]);
    gSprites[spriteId].oam.affineParam = taskId;
    gTasks[taskId].data[4] = gBankTarget;
    gTasks[taskId].func = TaskDummy;
    PlaySE(SE_NAGERU);
}

void objc_0804ABD4(struct Sprite *sprite)
{
    if (sub_8078718(sprite))
    {
        u8 taskId = sprite->oam.affineParam;
        u8 r5 = gTasks[taskId].data[4];
        u8 r8 = gTasks[taskId].data[2];
        u32 r4;  // not sure of this type

        StartSpriteAnim(sprite, 1);
        sprite->affineAnimPaused = TRUE;
        sprite->pos1.x += sprite->pos2.x;
        sprite->pos1.y += sprite->pos2.y;
        sprite->pos2.x = 0;
        sprite->pos2.y = 0;
        sprite->data5 = 0;
        r4 = ball_number_to_ball_processing_index(sub_8047978(r5));
        sub_814086C(sprite->pos1.x, sprite->pos1.y - 5, 1, 0x1C, r4);
        sprite->data0 = sub_8141314(0, r5, 14, r4);
        sprite->data6 = r5;
        sprite->data7 = r8;
        DestroyTask(taskId);
        sprite->callback = sub_80466E8;
    }
}

static void sub_80466E8(struct Sprite *sprite)
{
    sprite->callback = sub_80466F4;
}

static void sub_80466F4(struct Sprite *sprite)
{
    sprite->data5++;
    if (sprite->data5 == 10)
    {
        sprite->data5 = 0;
        sprite->callback = sub_8046760;
        StartSpriteAffineAnim(&gSprites[gObjectBankIDs[sprite->data6]], 2);
        AnimateSprite(&gSprites[gObjectBankIDs[sprite->data6]]);
        gSprites[gObjectBankIDs[sprite->data6]].data1 = 0;
    }
}

static void sub_8046760(struct Sprite *sprite)
{
    sprite->data5++;
    if (sprite->data5 == 11)
        PlaySE(SE_SUIKOMU);
    if (gSprites[gObjectBankIDs[sprite->data6]].affineAnimEnded)
    {
        StartSpriteAnim(sprite, 2);
        gSprites[gObjectBankIDs[sprite->data6]].invisible = TRUE;
        sprite->data5 = 0;
        sprite->callback = sub_80467F8;
    }
    else
    {
        gSprites[gObjectBankIDs[sprite->data6]].data1 += 0x60;
        gSprites[gObjectBankIDs[sprite->data6]].pos2.y = -gSprites[gObjectBankIDs[sprite->data6]].data1 >> 8;
    }
}

static void sub_80467F8(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        sprite->data5++;
        if (sprite->data5 == 1)
        {
            sprite->data3 = 0;
            sprite->data4 = 32;
            sprite->data5 = 0;
            sprite->pos1.y += Cos(0, 32);
            sprite->pos2.y = -Cos(0, sprite->data4);
            sprite->callback = sub_804684C;
        }
    }
}

static void sub_804684C(struct Sprite *sprite)
{
    bool8 r5 = FALSE;

    switch (sprite->data3 & 0xFF)
    {
    case 0:
        sprite->pos2.y = -Cos(sprite->data5, sprite->data4);
        sprite->data5 += 4 + (sprite->data3 >> 8);
        if (sprite->data5 >= 64)
        {
            sprite->data4 -= 10;
            sprite->data3 += 0x101;
            if (sprite->data3 >> 8 == 4)
                r5 = TRUE;
            switch (sprite->data3 >> 8)
            {
            case 1:
                PlaySE(SE_KON);
                break;
            case 2:
                PlaySE(SE_KON2);
                break;
            case 3:
                PlaySE(SE_KON3);
                break;
            default:
                PlaySE(SE_KON4);
                break;
            }
        }
        break;
    case 1:
        sprite->pos2.y = -Cos(sprite->data5, sprite->data4);
        sprite->data5 -= 4 + (sprite->data3 >> 8);
        if (sprite->data5 <= 0)
        {
            sprite->data5 = 0;
            sprite->data3 &= 0xFF00;
        }
        break;
    }
    if (r5)
    {
        sprite->data3 = 0;
        sprite->pos1.y += Cos(64, 32);
        sprite->pos2.y = 0;
        if (sprite->data7 == 0)
        {
            sprite->callback = sub_8046C78;
        }
        else
        {
            sprite->callback = sub_8046944;
            sprite->data4 = 1;
            sprite->data5 = 0;
        }
    }
}

static void sub_8046944(struct Sprite *sprite)
{
    sprite->data3++;
    if (sprite->data3 == 31)
    {
        sprite->data3 = 0;
        sprite->affineAnimPaused = TRUE;
        StartSpriteAffineAnim(sprite, 1);
        sprite->callback = sub_8046984;
        PlaySE(SE_BOWA);
    }
}

static void sub_8046984(struct Sprite *sprite)
{
    switch (sprite->data3 & 0xFF)
    {
    case 0:
    case 2:
        sprite->pos2.x += sprite->data4;
        sprite->data5 += sprite->data4;
        sprite->affineAnimPaused = FALSE;
        if (sprite->data5 > 3 || sprite->data5 < -3)
        {
            sprite->data3++;
            sprite->data5 = 0;
        }
        break;
    case 1:
        sprite->data5++;
        if (sprite->data5 == 1)
        {
            sprite->data5 = 0;
            sprite->data4 = -sprite->data4;
            sprite->data3++;
            sprite->affineAnimPaused = FALSE;
            if (sprite->data4 < 0)
                ChangeSpriteAffineAnim(sprite, 2);
            else
                ChangeSpriteAffineAnim(sprite, 1);
        }
        else
        {
            sprite->affineAnimPaused = TRUE;
        }
        break;
    case 3:
        sprite->data3 += 0x100;
        if (sprite->data3 >> 8 == sprite->data7)
        {
            sprite->callback = sub_8046C78;
        }
        else
        {
            if (sprite->data7 == 4 && sprite->data3 >> 8 == 3)
            {
                sprite->callback = sub_8046E7C;
                sprite->affineAnimPaused = TRUE;
            }
            else
            {
                sprite->data3++;
                sprite->affineAnimPaused = TRUE;
            }
        }
        break;
    case 4:
    default:
        sprite->data5++;
        if (sprite->data5 == 31)
        {
            sprite->data5 = 0;
            sprite->data3 &= 0xFF00;
            StartSpriteAffineAnim(sprite, 3);
            if (sprite->data4 < 0)
                StartSpriteAffineAnim(sprite, 2);
            else
                StartSpriteAffineAnim(sprite, 1);
            PlaySE(SE_BOWA);
        }
        break;
    }
}

static void sub_8046AD0(u8 taskId)
{
    u8 r6 = gTasks[taskId].data[2];
    u8 r3 = gTasks[taskId].data[1];
    u16 species = gTasks[taskId].data[0];

    switch (gTasks[taskId].data[15])
    {
    case 0:
    default:
        if (gTasks[taskId].data[8] < 3)
            gTasks[taskId].data[8]++;
        else
            gTasks[taskId].data[15] = r6 + 1;
        break;
    case 1:
        PlayCry1(species, r3);
        DestroyTask(taskId);
        break;
    case 2:
        StopCryAndClearCrySongs();
        gTasks[taskId].data[10] = 3;
        gTasks[taskId].data[15] = 20;
        break;
    case 20:
        if (gTasks[taskId].data[10] != 0)
        {
            gTasks[taskId].data[10]--;
            break;
        }
        PlayCry4(species, r3, 1);
        DestroyTask(taskId);
        break;
    case 3:
        gTasks[taskId].data[10] = 6;
        gTasks[taskId].data[15] = 30;
        break;
    case 30:
        if (gTasks[taskId].data[10] != 0)
        {
            gTasks[taskId].data[10]--;
            break;
        }
        gTasks[taskId].data[15]++;
        // fall through
    case 31:
        if (!IsCryPlayingOrClearCrySongs())
        {
            StopCryAndClearCrySongs();
            gTasks[taskId].data[10] = 3;
            gTasks[taskId].data[15]++;
        }
        break;
    case 32:
        if (gTasks[taskId].data[10] != 0)
        {
            gTasks[taskId].data[10]--;
            break;
        }
        PlayCry4(species, r3, 0);
        DestroyTask(taskId);
        break;
    }
}

static void sub_8046C78(struct Sprite *sprite)
{
    u8 r5 = sprite->data6;
    u32 r4;  // not sure of this type

    StartSpriteAnim(sprite, 1);
    r4 = ball_number_to_ball_processing_index(sub_8047978(r5));
    sub_814086C(sprite->pos1.x, sprite->pos1.y - 5, 1, 0x1C, r4);
    sprite->data0 = sub_8141314(1, sprite->data6, 14, r4);
    sprite->callback = sub_8046E9C;
    if (gMain.inBattle)
    {
        struct Pokemon *pkmn;
        u16 species;
        s8 r8;
        u16 r4_2;
        u8 taskId;

        if (GetBankSide(r5) != 0)
        {
            pkmn = &gEnemyParty[gBattlePartyID[r5]];
            r8 = 25;
        }
        else
        {
            pkmn = &gPlayerParty[gBattlePartyID[r5]];
            r8 = -25;
        }
        species = GetMonData(pkmn, MON_DATA_SPECIES);
        if ((r5 == GetBankByPlayerAI(0) || r5 == GetBankByPlayerAI(1))
         && IsDoubleBattle() && ewram17840.unk9_0)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                if (IsBGMPlaying())
                    m4aMPlayStop(&gMPlay_BGM);
            }
            else
            {
                m4aMPlayVolumeControl(&gMPlay_BGM, 0xFFFF, 128);
            }
        }
        if (!IsDoubleBattle() || !ewram17840.unk9_0)
            r4_2 = 0;
        else if (r5 == GetBankByPlayerAI(0) || r5 == GetBankByPlayerAI(1))
            r4_2 = 1;
        else
            r4_2 = 2;
        taskId = CreateTask(sub_8046AD0, 3);
        gTasks[taskId].data[0] = species;
        gTasks[taskId].data[1] = r8;
        gTasks[taskId].data[2] = r4_2;
        gTasks[taskId].data[15] = 0;
    }
    StartSpriteAffineAnim(&gSprites[gObjectBankIDs[sprite->data6]], 1);
    AnimateSprite(&gSprites[gObjectBankIDs[sprite->data6]]);
    gSprites[gObjectBankIDs[sprite->data6]].data1 = 0x1000;
}

static void sub_8046E7C(struct Sprite *sprite)
{
    sprite->animPaused = TRUE;
    sprite->callback = sub_8046FBC;
    sprite->data3 = 0;
    sprite->data4 = 0;
    sprite->data5 = 0;
}

static void sub_8046E9C(struct Sprite *sprite)
{
    bool8 r7 = FALSE;
    u8 r4 = sprite->data6;

    gSprites[gObjectBankIDs[r4]].invisible = FALSE;
    if (sprite->animEnded)
        sprite->invisible = TRUE;
    if (gSprites[gObjectBankIDs[r4]].affineAnimEnded)
    {
        StartSpriteAffineAnim(&gSprites[gObjectBankIDs[r4]], 0);
        r7 = TRUE;
    }
    else
    {
        gSprites[gObjectBankIDs[r4]].data1 -= 288;
        gSprites[gObjectBankIDs[r4]].pos2.y = gSprites[gObjectBankIDs[r4]].data1 >> 8;
    }
    if (sprite->animEnded && r7)
    {
        s32 i;
        u32 r3;

        gSprites[gObjectBankIDs[r4]].pos2.y = 0;
        gDoingBattleAnim = 0;
        ewram17810[r4].unk0_3 = 0;
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
        for (r3 = 0, i = 0; i < 4; i++)
        {
            if (ewram17810[i].unk0_3 == 0)
                r3++;
        }
        if (r3 == 4)
        {
            for (i = 0; i < 12; i++)
                sub_804794C(i);
        }
    }
}

static void sub_8046FBC(struct Sprite *sprite)
{
    u8 r7 = sprite->data6;

    sprite->data4++;
    if (sprite->data4 == 40)
    {
        return;
    }
    else if (sprite->data4 == 95)
    {
        gDoingBattleAnim = 0;
        m4aMPlayAllStop();
        PlaySE(BGM_FANFA5);
    }
    else if (sprite->data4 == 315)
    {
        FreeOamMatrix(gSprites[gObjectBankIDs[sprite->data6]].oam.matrixNum);
        DestroySprite(&gSprites[gObjectBankIDs[sprite->data6]]);
        DestroySpriteAndFreeResources(sprite);
        if (gMain.inBattle)
            ewram17810[r7].unk0_3 = 0;
    }
}

static void sub_8047074(struct Sprite *sprite)
{
    sprite->data0 = 25;
    sprite->data2 = sub_8077ABC(sprite->data6, 2);
    sprite->data4 = sub_8077ABC(sprite->data6, 3) + 0x18;
    sprite->data5 = -30;
    sprite->oam.affineParam = sprite->data6;
    sub_80786EC(sprite);
    sprite->callback = sub_80470C4;
}

#define HIBYTE(x) (((x) >> 8) & 0xFF)

static void sub_80470C4(struct Sprite *sprite)
{
    u32 r6;
    u32 r7;

    if (HIBYTE(sprite->data7) >= 35 && HIBYTE(sprite->data7) < 80)
    {
        s16 r4;

        if ((sprite->oam.affineParam & 0xFF00) == 0)
        {
            r6 = sprite->data1 & 1;
            r7 = sprite->data2 & 1;
            sprite->data1 = ((sprite->data1 / 3) & ~1) | r6;
            sprite->data2 = ((sprite->data2 / 3) & ~1) | r7;
            StartSpriteAffineAnim(sprite, 4);
        }
        r4 = sprite->data0;
        sub_8078B5C(sprite);
        sprite->data7 += sprite->data6 / 3;
        sprite->pos2.y += Sin(HIBYTE(sprite->data7), sprite->data5);
        sprite->oam.affineParam += 0x100;
        if ((sprite->oam.affineParam >> 8) % 3 != 0)
            sprite->data0 = r4;
        else
            sprite->data0 = r4 - 1;
        if (HIBYTE(sprite->data7) >= 80)
        {
            r6 = sprite->data1 & 1;
            r7 = sprite->data2 & 1;
            sprite->data1 = ((sprite->data1 * 3) & ~1) | r6;
            sprite->data2 = ((sprite->data2 * 3) & ~1) | r7;
        }
    }
    else
    {
        if (sub_8078718(sprite))
        {
            sprite->pos1.x += sprite->pos2.x;
            sprite->pos1.y += sprite->pos2.y;
            sprite->pos2.y = 0;
            sprite->pos2.x = 0;
            sprite->data6 = sprite->oam.affineParam & 0xFF;
            sprite->data0 = 0;
            if (IsDoubleBattle() && ewram17840.unk9_0
             && sprite->data6 == GetBankByPlayerAI(2))
                sprite->callback = sub_8047230;
            else
                sprite->callback = sub_8046C78;
            StartSpriteAffineAnim(sprite, 0);
        }
    }
}

static void sub_8047230(struct Sprite *sprite)
{
    if (sprite->data0++ > 0x18)
    {
        sprite->data0 = 0;
        sprite->callback = sub_8046C78;
    }
}

static void sub_8047254(struct Sprite *sprite)
{
    sprite->data0++;
    if (sprite->data0 > 15)
    {
        sprite->data0 = 0;
        if (IsDoubleBattle() && ewram17840.unk9_0
         && sprite->data6 == GetBankByPlayerAI(3))
            sprite->callback = sub_8047230;
        else
            sprite->callback = sub_8046C78;
    }
}

static u8 sub_80472B0(u8 a, u8 b, u8 c, u8 d)
{
    return sub_814086C(a, b, c, d, 0);
}

static u8 sub_80472D8(u8 a, u8 b, u32 c)
{
    return sub_8141314(a, b, c, 0);
}

void CreatePokeballSprite(u8 a, u8 b, u8 x, u8 y, u8 e, u8 f, u8 g, u32 h)
{
    u8 spriteId;

    LoadCompressedObjectPic(&gUnknown_0820A92C[0]);
    LoadCompressedObjectPalette(&gUnknown_0820A98C[0]);
    spriteId = CreateSprite(&gSpriteTemplate_820AAB4[0], x, y, f);
    gSprites[spriteId].data0 = a;
    gSprites[spriteId].data5 = gSprites[a].pos1.x;
    gSprites[spriteId].data6 = gSprites[a].pos1.y;
    gSprites[a].pos1.x = x;
    gSprites[a].pos1.y = y;
    gSprites[spriteId].data1 = g;
    gSprites[spriteId].data2 = b;
    gSprites[spriteId].data3 = h;
    gSprites[spriteId].data4 = h >> 16;
    gSprites[spriteId].oam.priority = e;
    gSprites[spriteId].callback = sub_80473D0;
    gSprites[a].invisible = TRUE;
}

static void sub_80473D0(struct Sprite *sprite)
{
    if (sprite->data1 == 0)
    {
        u8 r5;
        u8 r7 = sprite->data0;
        u8 r8 = sprite->data2;
        u32 r4 = (u16)sprite->data3 | ((u16)sprite->data4 << 16);

        if (sprite->subpriority != 0)
            r5 = sprite->subpriority - 1;
        else
            r5 = 0;
        StartSpriteAnim(sprite, 1);
        sub_80472B0(sprite->pos1.x, sprite->pos1.y - 5, sprite->oam.priority, r5);
        sprite->data1 = sub_80472D8(1, r8, r4);
        sprite->callback = sub_804748C;
        gSprites[r7].invisible = FALSE;
        StartSpriteAffineAnim(&gSprites[r7], 1);
        AnimateSprite(&gSprites[r7]);
        gSprites[r7].data1 = 0x1000;
        sprite->data7 = 0;
    }
    else
    {
        sprite->data1--;
    }
}

static void sub_804748C(struct Sprite *sprite)
{
    bool8 r12 = FALSE;
    bool8 r6 = FALSE;
    u8 r3 = sprite->data0;
    u16 var1;
    u16 var2;

    if (sprite->animEnded)
        sprite->invisible = TRUE;
    if (gSprites[r3].affineAnimEnded)
    {
        StartSpriteAffineAnim(&gSprites[r3], 0);
        r12 = TRUE;
    }
    var1 = (sprite->data5 - sprite->pos1.x) * sprite->data7 / 128 + sprite->pos1.x;
    var2 = (sprite->data6 - sprite->pos1.y) * sprite->data7 / 128 + sprite->pos1.y;
    gSprites[r3].pos1.x = var1;
    gSprites[r3].pos1.y = var2;
    if (sprite->data7 < 128)
    {
        s16 sine = -(gSineTable[(u8)sprite->data7] / 8);

        sprite->data7 += 4;
        gSprites[r3].pos2.x = sine;
        gSprites[r3].pos2.y = sine;
    }
    else
    {
        gSprites[r3].pos1.x = sprite->data5;
        gSprites[r3].pos1.y = sprite->data6;
        gSprites[r3].pos2.x = 0;
        gSprites[r3].pos2.y = 0;
        r6 = TRUE;
    }
    if (sprite->animEnded && r12 && r6)
        DestroySpriteAndFreeResources(sprite);
}

u8 sub_8047580(u8 a, u8 b, u8 x, u8 y, u8 e, u8 f, u8 g, u32 h)
{
    u8 spriteId;

    LoadCompressedObjectPic(&gUnknown_0820A92C[0]);
    LoadCompressedObjectPalette(&gUnknown_0820A98C[0]);
    spriteId = CreateSprite(&gSpriteTemplate_820AAB4[0], x, y, f);
    gSprites[spriteId].data0 = a;
    gSprites[spriteId].data1 = g;
    gSprites[spriteId].data2 = b;
    gSprites[spriteId].data3 = h;
    gSprites[spriteId].data4 = h >> 16;
    gSprites[spriteId].oam.priority = e;
    gSprites[spriteId].callback = sub_8047638;
    return spriteId;
}

static void sub_8047638(struct Sprite *sprite)
{
    if (sprite->data1 == 0)
    {
        u8 r6;
        u8 r7 = sprite->data0;
        u8 r8 = sprite->data2;
        u32 r5 = (u16)sprite->data3 | ((u16)sprite->data4 << 16);

        if (sprite->subpriority != 0)
            r6 = sprite->subpriority - 1;
        else
            r6 = 0;
        StartSpriteAnim(sprite, 1);
        sub_80472B0(sprite->pos1.x, sprite->pos1.y - 5, sprite->oam.priority, r6);
        sprite->data1 = sub_80472D8(1, r8, r5);
        sprite->callback = sub_80476E0;
        StartSpriteAffineAnim(&gSprites[r7], 2);
        AnimateSprite(&gSprites[r7]);
        gSprites[r7].data1 = 0;
    }
    else
    {
        sprite->data1--;
    }
}

static void sub_80476E0(struct Sprite *sprite)
{
    u8 r1;

    sprite->data5++;
    if (sprite->data5 == 11)
        PlaySE(SE_SUIKOMU);
    r1 = sprite->data0;
    if (gSprites[r1].affineAnimEnded)
    {
        StartSpriteAnim(sprite, 2);
        gSprites[r1].invisible = TRUE;
        sprite->data5 = 0;
        sprite->callback = sub_8047754;
    }
    else
    {
        gSprites[r1].data1 += 96;
        gSprites[r1].pos2.y = -gSprites[r1].data1 >> 8;
    }
}

static void sub_8047754(struct Sprite *sprite)
{
    if (sprite->animEnded)
        sprite->callback = SpriteCallbackDummy;
}

void obj_delete_and_free_associated_resources_(struct Sprite *sprite)
{
    DestroySpriteAndFreeResources(sprite);
}

void sub_804777C(u8 a)
{
    struct Sprite *sprite = &gSprites[gHealthboxIDs[a]];

    sprite->data0 = 5;
    sprite->data1 = 0;
    sprite->pos2.x = 0x73;
    sprite->pos2.y = 0;
    sprite->callback = sub_8047830;
    if (GetBankSide(a) != 0)
    {
        sprite->data0 = -sprite->data0;
        sprite->data1 = -sprite->data1;
        sprite->pos2.x = -sprite->pos2.x;
        sprite->pos2.y = -sprite->pos2.y;
    }
    gSprites[sprite->data5].callback(&gSprites[sprite->data5]);
    if (GetBankIdentity(a) == 2)
        sprite->callback = sub_804780C;
}

static void sub_804780C(struct Sprite *sprite)
{
    sprite->data1++;
    if (sprite->data1 == 20)
    {
        sprite->data1 = 0;
        sprite->callback = sub_8047830;
    }
}

static void sub_8047830(struct Sprite *sprite)
{
    sprite->pos2.x -= sprite->data0;
    sprite->pos2.y -= sprite->data1;
    if (sprite->pos2.x == 0 && sprite->pos2.y == 0)
        sprite->callback = SpriteCallbackDummy;
}

void sub_8047858(u8 a)
{
    u8 spriteId;

    spriteId = CreateInvisibleSpriteWithCallback(oamc_804BEB4);
    gSprites[spriteId].data0 = 1;
    gSprites[spriteId].data1 = gHealthboxIDs[a];
    gSprites[spriteId].callback = oamc_804BEB4;
}

static void oamc_804BEB4(struct Sprite *sprite)
{
    u8 r1 = sprite->data1;

    gSprites[r1].pos2.y = sprite->data0;
    sprite->data0 = -sprite->data0;
    sprite->data2++;
    if (sprite->data2 == 21)
    {
        gSprites[r1].pos2.x = 0;
        gSprites[r1].pos2.y = 0;
        DestroySprite(sprite);
    }
}

void sub_80478DC(u8 a)
{
    u16 var;

    if (GetSpriteTileStartByTag(gUnknown_0820A92C[a].tag) == 0xFFFF)
    {
        LoadCompressedObjectPic(&gUnknown_0820A92C[a]);
        LoadCompressedObjectPalette(&gUnknown_0820A98C[a]);
    }
    switch (a)
    {
    case 6:
    case 10:
    case 11:
        break;
    default:
        var = GetSpriteTileStartByTag(gUnknown_0820A92C[a].tag);
        LZDecompressVram(gUnknown_08D030D0, (void *)(VRAM + 0x10100 + var * 32));
        break;
    }
}

void sub_804794C(u8 a)
{
    FreeSpriteTilesByTag(gUnknown_0820A92C[a].tag);
    FreeSpritePaletteByTag(gUnknown_0820A98C[a].tag);
}

static u16 sub_8047978(u8 a)
{
    if (GetBankSide(a) == 0)
        return GetMonData(&gPlayerParty[gBattlePartyID[a]], MON_DATA_POKEBALL);
    else
        return GetMonData(&gEnemyParty[gBattlePartyID[a]], MON_DATA_POKEBALL);
}
