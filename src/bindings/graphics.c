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

static void bitmap_free(mrb_state *mrb, void *p){
    if (p) g_pd->graphics->freeBitmap((LCDBitmap*)p);
}
// Boxed value for Playdate SDK LCDBitmap
const mrb_data_type pd_bitmap_type = { "Playdate::Bitmap", bitmap_free };
struct RClass* bitmap;

static mrb_value
pd_graphics_loadBitmap(mrb_state *mrb, mrb_value self)
{
    const char *outErr = NULL;
    const char* path;
    mrb_get_args(mrb, "z", &path);
	LCDBitmap *img = g_pd->graphics->loadBitmap(path, &outErr);
	struct RData *d = mrb_data_object_alloc(mrb, bitmap, img, &pd_bitmap_type);

	if ( outErr != NULL ) {
		g_pd->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
	}
	return mrb_obj_value(d);
}

void rubybind_pd_graphics(mrb_state *mrb) {
    g_pd->system->logToConsole("Preparing Playdate->Graphics SDK functions...");

    struct RClass *pd = mrb_module_get(mrb, "Playdate");
    struct RClass *graphics = mrb_define_module_under(mrb, pd, "Graphics");
    bitmap = mrb_define_class_under(mrb, pd, "Bitmap", mrb->object_class);
    // mrb defined way to hint this class is C backed
    MRB_SET_INSTANCE_TT(bitmap, MRB_TT_DATA);

    mrb_define_module_function(mrb, graphics, "drawText", pd_graphics_drawText, MRB_ARGS_REQ(4));
    mrb_define_module_function(mrb, graphics, "loadBitmap", pd_graphics_loadBitmap, MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, graphics, "clear", pd_graphics_clear, MRB_ARGS_NONE());

    g_pd->system->logToConsole("Playdate->Graphics SDK functions prepared");
}
