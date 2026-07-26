#include "rubybind_base.h"

static void sprite_free(mrb_state *mrb, void *p){
    if (p) {
        g_pd->sprite->removeSprite((LCDSprite*)p);
        g_pd->sprite->freeSprite((LCDSprite*)p);
    };
}
// Boxed value for Playdate SDK LCDSprite
const mrb_data_type pd_sprite_type = { "Playdate::Sprite", sprite_free };
static void
sprite_draw_trampoline(LCDSprite* s, PDRect bounds, PDRect drawrect){
    mrb_state* mrb = ruby;
    mrb_value obj = mrb_obj_value(g_pd->sprite->getUserdata(s));
    int ai = mrb_gc_arena_save(mrb);

    // we might want to push down the cliprect - the bounds technically are
    // acheivable from x/y / width/height on sprite
    mrb_value argv[4] = {
        mrb_float_value(mrb, bounds.x), mrb_float_value(mrb, bounds.y),
        mrb_float_value(mrb, bounds.width), mrb_float_value(mrb, bounds.height),
    };
    mrb_funcall_argv(mrb, obj, g_sym_draw, 4, argv);
    if(mrb->exc) {
        ruby_report_any_exception(mrb);
        // Clear so failures stop
        g_pd->sprite->setDrawFunction(s, NULL);
    }
    mrb_gc_arena_restore(mrb, ai);
}

static mrb_value pd_sprite_initialize(mrb_state *mrb, mrb_value self){
    LCDSprite *s = g_pd->sprite->newSprite();
    mrb_data_init(self, s, &pd_sprite_type);
    // RData is void* and can be set in userdata, mrb_value isn't necessarily that type
    g_pd->sprite->setUserdata(s, mrb_ptr(self));
    g_pd->sprite->addSprite(s);

    if (mrb_respond_to(mrb, self, g_sym_draw)) {
        g_pd->sprite->setDrawFunction(s, sprite_draw_trampoline);
    }

    return self;
}


static mrb_value pd_sprite_moveTo(mrb_state *mrb, mrb_value self){
    mrb_int x, y;
    mrb_get_args(mrb, "ii", &x, &y);
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    LCDSprite* s = pd_sprite_get(mrb, self);
    g_pd->sprite->moveTo(s, (float)x, (float)y);
    return self;
}

static mrb_value pd_sprite_setSize(mrb_state *mrb, mrb_value self){
    mrb_int w, h;
    mrb_get_args(mrb, "ii", &w, &h);
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    LCDSprite* s = pd_sprite_get(mrb, self);
    g_pd->sprite->setSize(s, (float)w, (float)h);
    return self;
}
static mrb_value pd_sprite_markDirty(mrb_state *mrb, mrb_value self){
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    LCDSprite* s = pd_sprite_get(mrb, self);
    g_pd->sprite->markDirty(s);
    return self;
}


static mrb_value pd_sprite_setImage(mrb_state *mrb, mrb_value self){
    mrb_value bitmap;
    mrb_get_args(mrb, "o", &bitmap);
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    LCDSprite* s = pd_sprite_get(mrb, self);
    // Save pointer to bitmap on the sprite boxed object to avoid collection
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
    mrb_define_class_method(mrb, sprite, "drawSprites", pd_sprite_drawSprites, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "initialize", pd_sprite_initialize, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "move_to", pd_sprite_moveTo, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, sprite, "set_size", pd_sprite_setSize, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, sprite, "mark_dirty!", pd_sprite_markDirty, MRB_ARGS_NONE());
    // mrb_define_method(mrb, sprite, "set_bounds", pd_sprite_setBounds, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, sprite, "set_image", pd_sprite_setImage, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "get_image", pd_sprite_getImage, MRB_ARGS_NONE());
}
