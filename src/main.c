//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "mruby.h"
#include "pd_api.h"
#include "bindings/rubybind_base.h"

// ARM compiling MRuby for device needs the following stubs for exception handlers
#if defined(__thumb__)
__attribute__((used)) void *__exidx_start[0] = {};
__attribute__((used)) void *__exidx_end[0] = {};
#endif

extern const uint8_t ruby_source[];
static int update(void* userdata);

const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;
mrb_state* ruby;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
    	pd->system->logToConsole("Loaded up.");
	    ruby = initRuby(pd);

		mrb_load_irep(ruby, ruby_source);
		ruby_report_any_exception(ruby);
		pd->system->logToConsole("Ruby libraries loaded.");

		load_mrb_file(ruby, "cartridge/game.mrb");
		pd->system->logToConsole("Ruby cartridge loaded.");

		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);

		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		pd->graphics->setFont(font);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}

	return 0;
}

static int update(void* userdata)
{
    PlaydateAPI* pd = userdata;
    /* Call with symbol (faster, no string lookup) */
    int snapshot = mrb_gc_arena_save(ruby);
    mrb_funcall_id(ruby, mrb_top_self(ruby), mrb_intern_lit(ruby, "game_update"), 0);
    if(ruby->exc){
        mrb_value m = mrb_funcall(ruby, mrb_obj_value(ruby->exc), "inspect", 0);
        // pd->system->logToConsole("MRB Error: %s", mrb_str_to_cstr(ruby, m));
        ruby->exc = NULL;
    }
    // mrb_value val = mrb_load_string(ruby, "game_update()");
    mrb_gc_arena_restore(ruby, snapshot);
    // Should redraw?
    return 1;
}
