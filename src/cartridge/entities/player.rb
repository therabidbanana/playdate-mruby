class Player < Pyrite::Entity
  attr_reader :x, :y
  def prepare(x: 0, y: 0)
    @dx = 1
    @dy = 2
    @image_table = Playdate::Graphics.loadBitmapTable('images/pineapple-walk')
    @bitmap = @image_table.get_bitmap(1)
    set_size(32, 32)
    # Note - set_image() + draw will _crash_
    # set_image(@bitmap)
    @x = x
    @y = y
    move_to(x, y)
    @timer = @cartridge.timers.animation(300, min: 0, max: 3, loops: -1)
  end


  def update(scene)
    if scene.inputs.pressed?(:left)
      @x -= @dx
    elsif scene.inputs.pressed?(:right)
      @x += @dx
    end
    if scene.inputs.pressed?(:up)
      @y -= @dy
    elsif scene.inputs.pressed?(:down)
      @y += @dy
    else
      @y = @y
    end
    mark_dirty!
    move_to(@x, @y)
  end

  # Less performant this way
  def draw(x, y, w, h)
    Playdate::Graphics.drawBitmap(@image_table.get_bitmap(@timer.value), x, y)
  end
end
