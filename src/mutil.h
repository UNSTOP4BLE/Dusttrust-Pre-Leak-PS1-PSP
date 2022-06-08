/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_MUTIL_H
#define PSXF_GUARD_MUTIL_H

#include "fixed.h"
//Point type
typedef struct
{
	short x, y;
} POINT;
//Math utility functions
int MUtil_Sin(int x);
int MUtil_Cos(int x);
int smooth(int x);
void MUtil_RotatePoint(POINT *p, int s, int c);
fixed_t MUtil_Pull(fixed_t a, fixed_t b, fixed_t t);

#endif
