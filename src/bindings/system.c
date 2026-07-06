#include "rubybind_base.h"

static mrb_value
pd_system_logToConsole(mrb_state *mrb, mrb_value self)
{
    const char* a;
    mrb_get_args(mrb, "z", &a);

    g_pd->system->logToConsole(a);

    return mrb_nil_value();
}

static mrb_value
pd_system_drawFPS(mrb_state *mrb, mrb_value self)
{
    mrb_int a, b;
    mrb_get_args(mrb, "ii", &a, &b);

    g_pd->system->drawFPS(a, b);

    return mrb_nil_value();
}

void rubybind_pd_system(mrb_state *mrb) {
    g_pd->system->logToConsole("Preparing Playdate->System SDK functions...");

    struct RClass *pd = mrb_module_get(mrb, "Playdate");
    struct RClass *system = mrb_define_module_under(mrb, pd, "System");
    mrb_define_module_function(mrb, system, "logToConsole", pd_system_logToConsole, MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, system, "drawFPS", pd_system_drawFPS, MRB_ARGS_REQ(2));


    g_pd->system->logToConsole("Playdate->System SDK functions ready.");
}
