/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "../battle/battle.h"
#include "../mutil.h"

static struct
{
	Gfx_Tex tex_dtsans1;
	u8 animcount;
} dtsans1;

void Load_Sans(void)
{
	dtsans1.animcount = 0;
	Gfx_LoadTex(&dtsans1.tex_dtsans1, IO_Read("\\SPRITES\\DTSANS.TIM;1"), GFX_LOADTEX_FREE);
}

void Draw_Sans(void)
{
	dtsans1.animcount += 2;
	u8 sansposx = 0;
	u8 sansposy = 0;
	
	//Hoodies stems
	RECT hoodstrems_src = {63, 114, 19, 16};
	RECT hoodstrems_dst = {
		sansposx + 5,
		sansposy - 26 - (smooth(dtsans1.animcount) / 250),
		19,
		16
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &hoodstrems_src, &hoodstrems_dst, -smooth((dtsans1.animcount * 1.5) + 80) / 200, 14, 1);
	
	//Torso
	RECT torso_src = {36, 77, 53, 36};
	RECT torso_dst = {
		sansposx - 7,
		sansposy - 19 - (smooth(dtsans1.animcount) / 250),
		53,
		36
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &torso_src, &torso_dst, 0, torso_dst.w / 2, torso_dst.h);
	
	//Head
	RECT head_src = {81, 135, 36, 32};
	RECT head_dst = {
		sansposx,
		sansposy - 44 - (smooth(dtsans1.animcount) / 250),
		36,
		32
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &head_src, &head_dst, smooth(dtsans1.animcount + 25) / 240, head_dst.w / 2, head_dst.h);
	
	//Left Arm
	RECT leftarm_src = {7, 80, 28, 33};
	RECT leftarm_dst = {
		sansposx - 25,
		sansposy - 48 - (smooth(dtsans1.animcount) / 250),
		28,
		33
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &leftarm_src, &leftarm_dst, -smooth(dtsans1.animcount + 50) / 200, 18, 0);
	
	//Right Arm
	RECT rightarm_src = {1, 138, 79, 49};
	RECT rightarm_dst = {
		sansposx + 13,
		sansposy - 46 - (smooth(dtsans1.animcount) / 250),
		79,
		49
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &rightarm_src, &rightarm_dst, -smooth(dtsans1.animcount + 50) / 200, 39, 29);
	
	//Legs
	RECT legs_src = {1, 114, 61, 23};
	RECT legs_dst = {
		sansposx,
		sansposy,
		61,
		23 + (smooth(dtsans1.animcount) / 250)
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &legs_src, &legs_dst, 0, legs_dst.w / 2, legs_dst.h);
	
	//Hoodies back
	RECT hoodsback_src = {90, 114, 71, 20};
	RECT hoodsback_dst = {
		(sansposx - 20) - (smooth(dtsans1.animcount + 100) / 300),
		sansposy - 26 - (smooth(dtsans1.animcount) / 250),
		71 + (smooth(dtsans1.animcount + 100) / 300),
		20
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &hoodsback_src, &hoodsback_dst, -smooth((dtsans1.animcount + 100) * 2) / 300, 30, 1);

	//cape
	RECT cape_src = {90, 77, 61, 36};
	RECT cape_dst = {
		(sansposx - 25) - (smooth(dtsans1.animcount + 100) / 200),
		sansposy - 48 - (smooth(dtsans1.animcount) / 250),
		61 + (smooth(dtsans1.animcount + 100) / 200),
		36
	};
	Battle_DrawTexRotate(&dtsans1.tex_dtsans1, &cape_src, &cape_dst, smooth((dtsans1.animcount + 25) * 1.2) / 200, 38, 1);
}