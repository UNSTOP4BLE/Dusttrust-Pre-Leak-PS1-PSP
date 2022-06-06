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

int timershit = 0;

static struct
{
	Gfx_Tex tex_dtsans;
	struct
	{
		fixed_t x, y, zoom;
	} camera;
} battle;


static void Battle_Camera_Tick(void)
{
	int aa = smooth(timershit);
	battle.camera.zoom = FIXED_DEC(2,1) + (FIXED_DEC(1,1000) * smooth(timershit));
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
	Gfx_SetClear(0, 0, 0);
	
	Gfx_LoadTex(&battle.tex_dtsans, IO_Read("\\SPRITES\\DTSANS.TIM;1"), GFX_LOADTEX_FREE);
	
	Audio_LoadMus("\\MUSIC\\PHASE1.MUS;1");
	Audio_PlayMus(true);
	Audio_SetVolume(0, 0x3FFF, 0x0000);
	Audio_SetVolume(1, 0x0000, 0x3FFF);
	
	battle.camera.x = 0;
	battle.camera.y = 0;
	battle.camera.zoom = FIXED_DEC(1,1);
}

void Battle_Tick(void)
{
	//Camera
	Battle_Camera_Tick();
	
	timershit += 2;
	
	int sansposx = 0;
	int sansposy = 0;
	
	
	//Hoodies stems
	RECT hoodstrems_src = {63, 114, 19, 16};
	RECT hoodstrems_dst = {
		sansposx + 5,
		sansposy - 26 - (smooth(timershit) / 250),
		19,
		16
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &hoodstrems_src, &hoodstrems_dst, -smooth((timershit * 1.5) + 80) / 200, 14, 1);
	
	//Torso
	RECT torso_src = {36, 77, 53, 36};
	RECT torso_dst = {
		sansposx - 7,
		sansposy - 19 - (smooth(timershit) / 250),
		53,
		36
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &torso_src, &torso_dst, 0, torso_dst.w / 2, torso_dst.h);
	
	//Head
	RECT head_src = {81, 135, 36, 32};
	RECT head_dst = {
		sansposx,
		sansposy - 44 - (smooth(timershit) / 250),
		36,
		32
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &head_src, &head_dst, smooth(timershit + 25) / 240, head_dst.w / 2, head_dst.h);
	
	//Left Arm
	RECT leftarm_src = {7, 80, 28, 33};
	RECT leftarm_dst = {
		sansposx - 25,
		sansposy - 48 - (smooth(timershit) / 250),
		28,
		33
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &leftarm_src, &leftarm_dst, -smooth(timershit + 50) / 200, 18, 0);
	
	//Right Arm
	RECT rightarm_src = {1, 138, 79, 49};
	RECT rightarm_dst = {
		sansposx + 13,
		sansposy - 46 - (smooth(timershit) / 250),
		79,
		49
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &rightarm_src, &rightarm_dst, -smooth(timershit + 50) / 200, 39, 29);
	
	//Legs
	RECT legs_src = {1, 114, 61, 23};
	RECT legs_dst = {
		sansposx,
		sansposy,
		61,
		23 + (smooth(timershit) / 250)
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &legs_src, &legs_dst, 0, legs_dst.w / 2, legs_dst.h);
	
	//Hoodies back
	RECT hoodsback_src = {90, 114, 71, 20};
	RECT hoodsback_dst = {
		(sansposx - 20) - (smooth(timershit + 100) / 300),
		sansposy - 26 - (smooth(timershit) / 250),
		71 + (smooth(timershit + 100) / 300),
		20
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &hoodsback_src, &hoodsback_dst, -smooth((timershit + 100) * 2) / 300, 30, 1);

	//cape
	RECT cape_src = {90, 77, 61, 36};
	RECT cape_dst = {
		(sansposx - 25) - (smooth(timershit + 100) / 200),
		sansposy - 48 - (smooth(timershit) / 250),
		61 + (smooth(timershit + 100) / 200),
		36
	};
	Battle_DrawTexRotate(&battle.tex_dtsans, &cape_src, &cape_dst, smooth((timershit + 25) * 1.2) / 200, 38, 1);
}

void Battle_Unload(void)
{
	Gfx_SetClear(0, 0, 0);
}