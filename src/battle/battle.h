/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_BATTLE_H
#define PSXF_GUARD_BATTLE_H

#include "../gfx.h"
#include "../io.h"
#include "../pad.h"
#include "../fixed.h"


void Battle_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT *dst);
void Battle_DrawTexRotate(Gfx_Tex *tex, const RECT *src, const RECT *dst, u8 angle, u8 hx, u8 hy);

//Battle functions
void Battle_Load(void);
void Battle_Unload(void);
void Battle_Tick(void);

#endif