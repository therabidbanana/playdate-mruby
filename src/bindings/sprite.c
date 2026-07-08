#include "rubybind_base.h"

static void sprite_free(mrb_state *mrb, void *p){
    if (p) {
        g_pd->sprite->freeSprite((LCDSprite*)p);
    };
}
// Boxed value for Playdate SDK LCDSprite
const mrb_data_type pd_sprite_type = { "Playdate::Sprite", sprite_free };

static mrb_value pd_sprite_new(mrb_state *mrb, mrb_value self){
    LCDSprite *s = g_pd->sprite->newSprite();
    g_pd->sprite->addSprite(s);
    struct RData *d = mrb_data_object_alloc(mrb, mrb_class_ptr(self), s, &pd_sprite_type);
    // RData is void* and can be set in userdata, mrb_value isn't necessarily that type
    g_pd->sprite->setUserdata(s, d);
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

static mrb_value pd_sprite_setImage(mrb_state *mrb, mrb_value self){
    mrb_value bitmap;
    mrb_get_args(mrb, "o", &bitmap);
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    LCDSprite* s = pd_sprite_get(mrb, self);
    // Save pointer to bitmap on the sprite boxed object
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@image"), bitmap);

    LCDBitmap* b = pd_bitmap_get(mrb, bitmap);

    // TODO: allow flippage
    g_pd->sprite->setImage(s, b, kBitmapUnflipped);
    return self;
}

static mrb_value pd_sprite_getImage(mrb_state *mrb, mrb_value self){
    // Use boxed pointer instead
    return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@image"));
}

static mrb_value pd_sprite_drawSprites(mrb_state *mrb, mrb_value self){
    g_pd->sprite->drawSprites();
    return self;
}

void rubybind_pd_sprite(mrb_state *mrb) {
    struct RClass *pd = mrb_module_get(mrb, "Playdate");
    struct RClass *sprite = mrb_define_class_under(mrb, pd, "Sprite", mrb->object_class);
    // mrb defined way to hint this class is C backed
    MRB_SET_INSTANCE_TT(sprite, MRB_TT_DATA);
    mrb_define_class_method(mrb, sprite, "new", pd_sprite_new, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, sprite, "drawSprites", pd_sprite_drawSprites, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "move_to", pd_sprite_moveTo, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, sprite, "set_image", pd_sprite_setImage, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "get_image", pd_sprite_getImage, MRB_ARGS_NONE());
}
