#include "rubybind_base.h"

extern PlaydateAPI* g_pd = NULL;

mrb_value load_mrb_file(mrb_state* mrb, const char* path){
    FileStat st;

    if(g_pd->file->stat(path, &st)){
        g_pd->system->error("load_mrb_file failure: stat %s: %s", path, g_pd->file->geterr());
        return mrb_nil_value();
    }

    SDFile* f = g_pd->file->open(path, kFileRead);
    if(!f){
        g_pd->system->error("load_mrb_file failure: open %s: %s", path, g_pd->file->geterr());
        return mrb_nil_value();
    }

    uint8_t *buf  = g_pd->system->realloc(NULL, st.size);
    int got = g_pd->file->read(f, buf, st.size);
    g_pd->file->close(f);

    if(got != (int)st.size){
        g_pd->system->error("load_mrb_file failure: read %d/%u %s", got, st.size, path);
        g_pd->system->realloc(buf, 0);
        return mrb_nil_value();
    }

    mrb_value prev_value = mrb_gv_get(mrb, mrb_intern_lit(mrb, "$PD_FILENAME"));
    mrb_gv_set(mrb, mrb_intern_lit(mrb, "$PD_FILENAME"), mrb_str_new_cstr(mrb, path));
    int ai = mrb_gc_arena_save(mrb);
    mrb_value r = mrb_load_irep_buf(mrb, buf, st.size);
    g_pd->system->realloc(buf, 0);
    ruby_report_any_exception(mrb);
    mrb_gc_arena_restore(mrb, ai);
    mrb_gv_set(mrb, mrb_intern_lit(mrb, "$PD_FILENAME"), prev_value);
    return r;
}

static mrb_value
pd_load(mrb_state *mrb, mrb_value self)
{
    const char* filename;
    mrb_get_args(mrb, "z", &filename);
    load_mrb_file(mrb, filename);

    return mrb_nil_value();
}


// Define the memory management function
void* mrb_basic_alloc_func(void *p, size_t size) {
    return g_pd->system->realloc(p, size);
}

mrb_state* initRuby(PlaydateAPI* pd)
{
    // Set global state
    g_pd = pd;
   	mrb_state* ruby = mrb_open();

    /* Define Playdate */
    struct RClass *playdate = mrb_define_module(ruby, "Playdate");
    g_pd->system->logToConsole("Playdate class defined.");

    rubybind_pd_system(ruby);
    rubybind_pd_graphics(ruby);
    rubybind_pd_sprite(ruby);

    mrb_define_module_function(ruby, playdate, "load", pd_load, MRB_ARGS_REQ(1));

    return ruby;
}
