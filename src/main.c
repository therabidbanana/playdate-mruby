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

__attribute__((used)) void *__exidx_start[0] = {};
__attribute__((used)) void *__exidx_end[0] = {};
extern const uint8_t ruby_source[];

static int update(void* userdata);
static void initRuby(PlaydateAPI* pd);

const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;
mrb_state* ruby;
PlaydateAPI* mrb_pd = NULL;

static mrb_value
pd_system_logToConsole(mrb_state *mrb, mrb_value self)
{
    const char* a;
    mrb_get_args(mrb, "z", &a);
    if (mrb_pd) {
        mrb_pd->system->logToConsole(a);
    }
    return mrb_nil_value();
}

static mrb_value
pd_system_drawFPS(mrb_state *mrb, mrb_value self)
{
    mrb_int a, b;
    mrb_get_args(mrb, "ii", &a, &b);
    if (mrb_pd) {
        mrb_pd->system->drawFPS(a, b);
    }
    return mrb_nil_value();
}


static mrb_value
pd_graphics_drawText(mrb_state *mrb, mrb_value self)
{
    const char* text;
    mrb_int len,x,y;
    mrb_get_args(mrb, "ziii", &text, &len, &x, &y);
    if (mrb_pd) {
        mrb_pd->graphics->drawText(text, len, kASCIIEncoding, x, y);
    }
    return mrb_nil_value();
}

static mrb_value
pd_graphics_clear(mrb_state *mrb, mrb_value self)
{
    // TODO: Allow color
    if (mrb_pd) {
        mrb_pd->graphics->clear(kColorWhite);
    }
    return mrb_nil_value();
}


// Define the memory management function
void* mrb_basic_alloc_func(void *p, size_t size) {
    return mrb_pd->system->realloc(p, size);
}

static void
initRuby(PlaydateAPI* pd)
{
    // Set global state
    mrb_pd = pd;
   	ruby = mrb_open();

    /* Define Playdate */
    struct RClass *playdate = mrb_define_module(ruby, "Playdate");
    struct RClass *system = mrb_define_module_under(ruby, playdate, "System");
    struct RClass *graphics = mrb_define_module_under(ruby, playdate, "Graphics");
    mrb_pd->system->logToConsole("Class structure defined.");

    mrb_define_module_function(ruby, system, "logToConsole", pd_system_logToConsole, MRB_ARGS_REQ(1));
    mrb_define_module_function(ruby, system, "drawFPS", pd_system_drawFPS, MRB_ARGS_REQ(2));
    mrb_define_module_function(ruby, graphics, "drawText", pd_graphics_drawText, MRB_ARGS_REQ(4));
    mrb_define_module_function(ruby, graphics, "clear", pd_graphics_clear, MRB_ARGS_NONE());
    mrb_pd->system->logToConsole("SDK functions defined.");

    mrb_load_irep(ruby, ruby_source);
    if(ruby->exc){
        mrb_value m = mrb_funcall(ruby, mrb_obj_value(ruby->exc), "inspect", 0);
        pd->system->logToConsole("MRB Error: %s", mrb_str_to_cstr(ruby, m));
        ruby->exc = NULL;
    }

    mrb_pd->system->logToConsole("Ruby source loaded.");
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
    	pd->system->logToConsole("Loaded up.");
	    initRuby(pd);

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
