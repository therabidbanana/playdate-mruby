#include "rubybind_base.h"

static void sprite_free(mrb_state *mrb, void *p){
    if (p) g_pd->sprite->freeSprite((LCDSprite*)p);
}
// Boxed value for Playdate SDK LCDSprite
const mrb_data_type pd_sprite_type = { "Playdate::Sprite", sprite_free };

static mrb_value pd_sprite_new(mrb_state *mrb, mrb_value self){
    LCDSprite *s = g_pd->sprite->newSprite();
    g_pd->sprite->addSprite(s);
    struct RData *d = mrb_data_object_alloc(mrb, mrb_class_ptr(self), s, &pd_sprite_type);
    return mrb_obj_value(d);
}

static mrb_value pd_sprite_moveTo(mrb_state *mrb, mrb_value self){
    mrb_int x, y;
    mrb_get_args(mrb, "ii", &x, &y);
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    LCDSprite* s = pd_sprite_get(mrb, self);
    g_pd->sprite->moveTo(s, (float)x, (float)y);
    return self;
}

void rubybind_pd_sprite(mrb_state *mrb) {
    struct RClass *pd = mrb_module_get(mrb, "Playdate");
    struct RClass *sprite = mrb_define_class_under(mrb, pd, "Sprite", &pd_sprite_type);
    // mrb defined way to hint this class is C backed
    MRB_SET_INSTANCE_TT(sprite, MRB_TT_DATA);
    mrb_define_class_method(mrb, sprite, "new", pd_sprite_new, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "move_to", pd_sprite_moveTo, MRB_ARGS_REQ(2));
}
