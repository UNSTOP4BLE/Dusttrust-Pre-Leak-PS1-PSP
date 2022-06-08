	/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "battle.h"
#include "../main.h"
#include <stdlib.h>
#include <stdio.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <psputility.h>
#include "../glib2d.h"
#include "../psp/triWav.h"
#include "../psp/triLog.h"
#include "../psp/triMemory.h"
#include "../psp/triInput.h"
#include "../mutil.h"
#include "../fixed.h"

#include "../chars/dtsans1.h"

int timershit = 0;
int bump = 0;

static struct
{
	//Gfx_Tex tex_hud;
	struct
		fixed_t x, y, zoom;
		fixed_t tx, ty, tzoom, speed;
	} camera;
	struct
	{
		fixed_t x, y, xsize, ysize;
		fixed_t tx, ty, txsize, tysize;
		fixed_t speed;
	} box;
	struct
	{
		fixed_t x, y, angle;
		fixed_t speed;
	} soul;
} battle;

static void Soul(void)
{
	if (pad_state.held & PAD_UP)
		battle.soul.y -= battle.soul.speed;
	if (pad_state.held & PAD_DOWN)
		battle.soul.y += battle.soul.speed;
	if (pad_state.held & PAD_LEFT)
		battle.soul.x -= battle.soul.speed;
	if (pad_state.held & PAD_RIGHT)
		battle.soul.x += battle.soul.speed;
	
	if(battle.soul.y < battle.box.y + (-battle.box.ysize / 2) + 4)
		battle.soul.y = battle.box.y + (-battle.box.ysize / 2) + 4;
	if(battle.soul.y > battle.box.y + (battle.box.ysize / 2) - 4)
		battle.soul.y = battle.box.y + (battle.box.ysize / 2) - 4;
	if(battle.soul.x < battle.box.x + (-battle.box.xsize / 2) + 4)
		battle.soul.x = battle.box.x + (-battle.box.xsize / 2) + 4;
	if(battle.soul.x > battle.box.x + (battle.box.xsize / 2) - 3)
		battle.soul.x = battle.box.x + (battle.box.xsize / 2) - 3;
	
	
	RECT soul_src = {225, 0, 8, 8};
	RECT soul_dst = {
		battle.soul.x,
		battle.soul.y,
		8,
		8
	};
	Battle_DrawTexRotate(&battle.tex_hud, &soul_src, &soul_dst, 0, 4, 4);
}

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
	
	//battle.box.txsize = 200 + (smooth(timershit * 1.7) / 10);
	battle.box.x = (smooth((timershit + 64) * 1.7) / 50);
	battle.box.y = 54 + (smooth((timershit + 0) * 1.7) / 50);
	
	RECT box2_src = {248, 0, 2, 2};
	RECT box2_dst = {
		battle.box.x,
		battle.box.y,
		battle.box.xsize,
		battle.box.ysize
	};
	Battle_DrawTexRotate(&battle.tex_hud, &box2_src, &box2_dst, 0, box2_dst.w / 2, box2_dst.h / 2);

	RECT box_src = {248, 8, 2, 2};
	RECT box_dst = {
		battle.box.x,
		battle.box.y,
		battle.box.xsize + 4,
		battle.box.ysize + 4
	};
	Battle_DrawTexRotate(&battle.tex_hud, &box_src, &box_dst, 0, box_dst.w / 2, box_dst.h / 2);
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
	if (bump > 60)
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
	
	battle.soul.x = 0;
	battle.soul.y = 0;
	battle.soul.speed = 1;
}

void Battle_Tick(void)
{
	//Camera
	Battle_Camera_Tick();
	
	timershit += 2;
	Draw_Sans();
	Soul();
	Battle_Box();
}
	
void Battle_Unload(void)
{
	Gfx_SetClear(0, 0, 0);
}