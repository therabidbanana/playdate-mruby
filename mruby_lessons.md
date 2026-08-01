Documenting some lessons of mruby as I go

I have an X from SDK, how do I represent it in mruby?
======

Many parts to this:

1. A data type must be declared - this is a wrapper that tags the pointer with a type

```c
extern const mrb_data_type pd_sprite_type; // base.h

// sprite.c
// You must provide your plan to garbage collect value being wrapped
static void sprite_free(mrb_state *mrb, void *p){
    if (p) { g_pd->sprite->freeSprite((LCDSprite*)p); };
}
const mrb_data_type pd_sprite_type = { "Playdate::Sprite", sprite_free }; 
```

2. A boxing class in mruby is created and associated with the data type

```c
    struct RClass *sprite = mrb_define_class_under(mrb, pd, "Sprite", mrb->object_class);
    // mrb defined way to hint this class actually a C data type
    MRB_SET_INSTANCE_TT(sprite, MRB_TT_DATA);
```

3. When getting the pointer, wrap it in the box class for use in Ruby. Associate
   the ruby object pointer as necessary with the raw pointer because there is no
   easy way to take a C pointer and get back the _same_ Ruby object (you'd end
   creating a new instance boxing the same pointer) 
   
   Sprite class has a way to do this, otherwise a C wrapper of some kind might
   be needed - box the boxed value :D?

```c
static mrb_value pd_sprite_new(mrb_state *mrb, mrb_value self){
    LCDSprite *s = g_pd->sprite->newSprite(); // raw sprite we want to wrap
    
    // ... Make other calls with raw pointer as needed
    
    // Third argument here is a pointer to the class we want, self if we defined a method on the correct class, otherwise
    // we need to get the RClass* another way
    struct RData *d = mrb_data_object_alloc(mrb, mrb_class_ptr(self), s, &pd_sprite_type);
    
    // RData is a void* and can be set in userdata, mrb_value isn't necessarily sized as a void* 
    g_pd->sprite->setUserdata(s, d);
    
    // Cast an RData pointer to an mrb_value
    return mrb_obj_value(d);
}
```

4. (Associations of two C objects) one can get garbage collected if mruby loses
   track and C expects it to stick around - mruby doesn't see the object referenced
   anywhere so it will call the free function you defined. Keep the mruby view in
   sync with the C side by manually setting an ivar:

```c
    // in setImage C method
    
    // Save pointer to bitmap on the sprite boxed object
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@image"), img);
```


5. Define methods as needed to interact with the underlying object - you must
   unbox it with the data type defined earlier 
      
   This can be easier if you set up a helper to do the unboxing.

```c
static mrb_value pd_sprite_moveTo(mrb_state *mrb, mrb_value self){
    mrb_int x, y;
    mrb_get_args(mrb, "ii", &x, &y);
    // Use helper to unbox Sprite in ruby to LCDSprite pointer in C
    // pd_sprite_get is  `(LCDSprite*)mrb_data_get_ptr(mrb, self, &pd_sprite_type)`;
    LCDSprite* s = pd_sprite_get(mrb, self); 
    g_pd->sprite->moveTo(s, (float)x, (float)y);
    return self;
}


/// inside bind function where sprite class is declared
    mrb_define_method(mrb, sprite, "move_to", pd_sprite_moveTo, MRB_ARGS_REQ(2));
```


Defining kernel-level C methods
======================

Default execution stack - where you define a C method might matter, for example,
to create a load function that MRB can call to have C load another ruby file,
you need that new file to execute with top level context - the function must be
defined at the top level on the global object class rather than as part of a
module to get the correct context.
