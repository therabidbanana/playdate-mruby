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

static mrb_value
pd_system_getCurrentTimeMilliseconds(mrb_state *mrb, mrb_value self)
{
    unsigned int val = g_pd->system->getCurrentTimeMilliseconds();

    return mrb_fixnum_value((mrb_int) val);
}

static mrb_value
pd_system_getButtonState(mrb_state *mrb, mrb_value self)
{
    PDButtons down;
    g_pd->system->getButtonState(&down, NULL, NULL);

    return mrb_fixnum_value((mrb_int) down);
}

static mrb_value
pd_system_getButtonStatePressed(mrb_state *mrb, mrb_value self)
{
    PDButtons pressed;
    g_pd->system->getButtonState(NULL, &pressed, NULL);

    return mrb_fixnum_value((mrb_int) pressed);
}

static mrb_value
pd_system_getButtonStateReleased(mrb_state *mrb, mrb_value self)
{
    PDButtons released;
    g_pd->system->getButtonState(NULL, NULL, &released);

    return mrb_fixnum_value((mrb_int) released);
}

void rubybind_pd_system(mrb_state *mrb) {
    g_pd->system->logToConsole("Preparing Playdate->System SDK functions...");

    struct RClass *pd = mrb_module_get(mrb, "Playdate");
    struct RClass *system = mrb_define_module_under(mrb, pd, "System");
    mrb_define_module_function(mrb, system, "logToConsole", pd_system_logToConsole, MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, system, "drawFPS", pd_system_drawFPS, MRB_ARGS_REQ(2));
    mrb_define_module_function(mrb, system, "getCurrentTimeMilliseconds", pd_system_getCurrentTimeMilliseconds, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, system, "getButtonState", pd_system_getButtonState, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, system, "getButtonStatePressed", pd_system_getButtonStatePressed, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, system, "getButtonStateReleased", pd_system_getButtonStateReleased, MRB_ARGS_NONE());

    mrb_define_const(mrb, system, "UP", mrb_fixnum_value(kButtonUp));
    mrb_define_const(mrb, system, "DOWN", mrb_fixnum_value(kButtonDown));
    mrb_define_const(mrb, system, "LEFT", mrb_fixnum_value(kButtonLeft));
    mrb_define_const(mrb, system, "RIGHT", mrb_fixnum_value(kButtonRight));
    mrb_define_const(mrb, system, "BUTTON_A", mrb_fixnum_value(kButtonA));
    mrb_define_const(mrb, system, "BUTTON_B", mrb_fixnum_value(kButtonB));

    g_pd->system->logToConsole("Playdate->System SDK functions ready.");
}
