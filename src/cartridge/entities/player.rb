class Player < Pyrite::Entity
  attr_reader :x, :y
  def prepare(x: 0, y: 0)
    @dx = 1
    @dy = 2
    @image_table = Playdate::Graphics.loadBitmapTable('images/pineapple-walk')
    @bitmap = @image_table.get_bitmap(1)
    set_image(@bitmap)
    @x = x
    @y = y
    move_to(x, y)
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
    move_to(@x, @y)
  end

  # def draw
    # Playdate::Graphics.drawBitmap(@bitmap, @x, @y)
  # end
end
