#include "rubybind_base.h"

static mrb_value
pd_graphics_drawText(mrb_state *mrb, mrb_value self)
{
    const char* text;
    mrb_int len,x,y;
    mrb_get_args(mrb, "ziii", &text, &len, &x, &y);
    g_pd->graphics->drawText(text, len, kASCIIEncoding, x, y);
    return mrb_nil_value();
}

static mrb_value
pd_graphics_clear(mrb_state *mrb, mrb_value self)
{
    // TODO: Allow color
    g_pd->graphics->clear(kColorWhite);
    return mrb_nil_value();
}


void rubybind_pd_graphics(mrb_state *mrb) {
    g_pd->system->logToConsole("Preparing Playdate->Graphics SDK functions...");

    struct RClass *pd = mrb_module_get(mrb, "Playdate");
    struct RClass *graphics = mrb_define_module_under(mrb, pd, "Graphics");
    mrb_define_module_function(mrb, graphics, "drawText", pd_graphics_drawText, MRB_ARGS_REQ(4));
    mrb_define_module_function(mrb, graphics, "clear", pd_graphics_clear, MRB_ARGS_NONE());

    g_pd->system->logToConsole("Playdate->Graphics SDK functions prepared");
}
