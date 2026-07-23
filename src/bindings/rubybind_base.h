#include "mruby.h"
#include "pd_api.h"

extern PlaydateAPI *g_pd;
extern const mrb_data_type pd_sprite_type;
extern const mrb_data_type pd_bitmap_type;
extern const mrb_data_type pd_bitmap_table_type;
// Convert mrb wrapper to Playdate Sprite
static inline LCDSprite* pd_sprite_get(mrb_state *mrb, mrb_value v){
    return (LCDSprite*)mrb_data_get_ptr(mrb, v, &pd_sprite_type);
}
// Convert mrb wrapper to Playdate Bitmap
static inline LCDBitmap* pd_bitmap_get(mrb_state *mrb, mrb_value v){
    return (LCDBitmap*)mrb_data_get_ptr(mrb, v, &pd_bitmap_type);
}
// Convert mrb wrapper to Playdate BitmapTable
static inline LCDBitmapTable* pd_bitmap_table_get(mrb_state *mrb, mrb_value v){
    return (LCDBitmapTable*)mrb_data_get_ptr(mrb, v, &pd_bitmap_table_type);
}


static inline void ruby_report_any_exception(mrb_state *ruby){
    if(ruby->exc){
        mrb_value m = mrb_funcall(ruby, mrb_obj_value(ruby->exc), "inspect", 0);
        g_pd->system->logToConsole("MRB Error: %s", mrb_str_to_cstr(ruby, m));
        ruby->exc = NULL;
    }
}

mrb_value load_mrb_file(mrb_state* mrb, const char* path);
mrb_state* initRuby(PlaydateAPI *pd);
void rubybind_pd_system(mrb_state *mrb);
void rubybind_pd_graphics(mrb_state *mrb);
void rubybind_pd_sprite(mrb_state *mrb);
