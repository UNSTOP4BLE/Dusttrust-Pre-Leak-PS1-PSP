/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <psputility.h>
#include "glib2d.h"
#include "psp/triWav.h"
#include "psp/triLog.h"
#include "psp/triMemory.h"
#include "psp/triInput.h"
#include "battle/battle.h"

PSP_MODULE_INFO("Flappy Bird PSP", 0, 1, 0);

int exit_callback(int arg1, int arg2, void* common) {
    sceKernelExitGame();
    return 0;
}

int callbackthread(SceSize args, void* argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();
    return 0;
}

void setupcallbacks() {
    int thid =sceKernelCreateThread("update_thread", callbackthread, 0x11, 0xFA0, 0, NULL );
    if (thid >= 0){
        sceKernelStartThread(thid, 0, NULL);
    }
}
//Game loop
GameLoop gameloop;

//Error handler
char error_msg[0x200];

void ErrorLock(void)
{
	while (1)
	{
		return 0;
	}
}

//Entry point
int main(int argc, char **argv)
{
	//Remember arguments

	//Initialize system
	//PSX_Init();
	
	//Mem_Init((void*)malloc_heap, sizeof(malloc_heap));
	
    setupcallbacks();
	triLogInit();
	triMemoryInit();
	triWavInit();
	triInputInit();
	
	//Start game
	gameloop = GameLoop_Battle;
	Battle_Load();
	
	//Game loop
	while (1)
	{
		//Prepare frame
		triInputUpdate();

		//Tick and draw game
		switch (gameloop)
		{
			case GameLoop_Battle:
				Battle_Tick();
				break;
		}
		
		//Flip gfx buffers
		//Gfx_Flip();
	}
	
	//Deinitialize system
	return 0;
}
