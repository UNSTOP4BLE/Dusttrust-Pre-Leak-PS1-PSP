/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "battle.h"

#include "../mem.h"
#include "../main.h"
#include "../timer.h"
#include "../io.h"
#include "../gfx.h"
#include "../audio.h"
#include "../pad.h"
#include "../archive.h"
#include "../mutil.h"
#include "../random.h"

#include "../loadscr.h"

#include "../chars/dtsans1.h"

u8 timershit = 0;
u8 bump = 0;

static struct
{
	Gfx_Tex tex_hud, tex_dtsans;
	struct
	{
		fixed_t x, y, zoom;
		fixed_t tx, ty, tzoom, speed;
	} camera;
	struct
	{
		fixed_t x, y, xsize, ysize;
		fixed_t tx, ty, txsize, tysize;
		fixed_t speed;
	} box;
	
} battle;

static void Battle_Box(void)
{
	fixed_t dx = battle.box.tx - battle.box.x;
	fixed_t dy = battle.box.ty - battle.box.y;
	fixed_t dxsize = battle.box.txsize - battle.box.xsize;
	fixed_t dysize = battle.box.tysize - battle.box.ysize;
	
	battle.box.x += FIXED_MUL(dx, battle.box.speed);
	battle.box.y += FIXED_MUL(dy, battle.box.speed);
	battle.box.xsize += FIXED_MUL(dxsize, battle.box.speed);
	battle.box.ysize += FIXED_MUL(dysize, battle.box.speed);
	
	battle.box.txsize = 200 + (smooth(timershit * 1.7) / 10);
	
	RECT box2_src = {248, 0, 2, 2};
	RECT box2_dst = {
		battle.box.x,
		battle.box.y,
		battle.box.xsize,
		battle.box.ysize
	};
	Battle_DrawTexRotate(&battle.tex_hud, &box2_src, &box2_dst, smooth(timershit) / 50, box2_dst.w / 2, box2_dst.h / 2);

	RECT box_src = {248, 8, 2, 2};
	RECT box_dst = {
		battle.box.x,
		battle.box.y,
		battle.box.xsize + 4,
		battle.box.ysize + 4
	};
	Battle_DrawTexRotate(&battle.tex_hud, &box_src, &box_dst, smooth(timershit) / 50, box_dst.w / 2, box_dst.h / 2);
}

static void Battle_Camera_Tick(void)
{
	fixed_t dx = battle.camera.tx - battle.camera.x;
	fixed_t dy = battle.camera.ty - battle.camera.y;
	fixed_t dz = battle.camera.tzoom - battle.camera.zoom;
	
	battle.camera.x += FIXED_MUL(dx, battle.camera.speed);
	battle.camera.y += FIXED_MUL(dy, battle.camera.speed);
	battle.camera.zoom += FIXED_MUL(dz, battle.camera.speed);
	
	bump += 1;
	if (bump > 64)
	{
		battle.camera.zoom += FIXED_DEC(1,20);
		bump = 0;
	}
}

void Battle_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT *dst)
{
	Battle_DrawTexRotate(tex, src, dst, 0, 0, 0);
}

void Battle_DrawTexRotate(Gfx_Tex *tex, const RECT *src, const RECT *dst, u8 angle, u8 hx, u8 hy)
{
	fixed_t xz = dst->x + battle.camera.x;
	fixed_t yz = dst->y + battle.camera.y;
	fixed_t wz = dst->w;
	fixed_t hz = dst->h;
	
	RECT sdst = {
		SCREEN_WIDTH2 + FIXED_MUL(xz, battle.camera.zoom),
		SCREEN_HEIGHT2 + FIXED_MUL(yz, battle.camera.zoom),
		FIXED_MUL(wz, battle.camera.zoom),
		FIXED_MUL(hz, battle.camera.zoom)
	};
	Gfx_DrawTexRotate(tex, src, &sdst, angle, FIXED_MUL(hx, battle.camera.zoom), FIXED_MUL(hy, battle.camera.zoom));
}

void Battle_Load(void)
{
	Gfx_SetClear(50, 50, 50);
	
	Gfx_LoadTex(&battle.tex_hud, IO_Read("\\SPRITES\\BATTLE.TIM;1"), GFX_LOADTEX_FREE);
	Load_Sans();
	
	Audio_LoadMus("\\MUSIC\\PHASE1.MUS;1");
	Audio_PlayMus(true);
	Audio_SetVolume(0, 0x3FFF, 0x0000);
	Audio_SetVolume(1, 0x0000, 0x3FFF);
	
	battle.camera.x = 0;
	battle.camera.y = 0;
	battle.camera.zoom = FIXED_DEC(1,1);
	battle.camera.tx = 0;
	battle.camera.ty = 0;
	battle.camera.tzoom = FIXED_DEC(1,1);
	battle.camera.speed = FIXED_UNIT / 24;
	
	battle.box.x = 0;
	battle.box.y = 0;
	battle.box.xsize = 60;
	battle.box.ysize = 60;
	battle.box.tx = 0;
	battle.box.ty = 64;
	battle.box.txsize = 316;
	battle.box.tysize = 60;
	battle.box.speed = FIXED_UNIT / 32;
}

void Battle_Tick(void)
{
	//Camera
	Battle_Camera_Tick();
	
	timershit += 2;
	Draw_Sans();
	Battle_Box();
}
	
void Battle_Unload(void)
{
	Gfx_SetClear(0, 0, 0);
}