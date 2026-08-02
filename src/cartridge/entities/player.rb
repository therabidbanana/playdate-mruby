class Player < Pyrite::Entity
  image_table 'images/pineapple-walk'
  animation_state :standing, 0..0, delay: 2300, and_then: :blinking
  animation_state :blinking, 1..2, and_then: :pace
  animation_state :pace, 3..4, delay: 500, and_then: :standing
  animation_state :walking, 3..4

  attr_reader :x, :y
  def prepare(x: 0, y: 0)
    @dx = 1
    @dy = 2
    # @bitmap = @image_table.get_bitmap(1)
    set_size(32, 32)
    # Note - set_image() + draw will _crash_
    # set_image(@bitmap)
    @x = x
    @y = y
    move_to(x, y)
    # @timer = @cartridge.timers.animation(300, min: 0, max: 3, loops: -1)
  end


  def update(scene)
    dx = if scene.inputs.pressed?(:left)
           -@dx
         elsif scene.inputs.pressed?(:right)
           @dx
         else
           0
         end
    dy = if scene.inputs.pressed?(:up)
           -@dy
         elsif scene.inputs.pressed?(:down)
           @dy
         else
           0
         end
    motion = (dx.abs + dy.abs).positive?
    if motion && @state != :walking
      transition_to!(:walking)
    elsif !motion && @state == :walking
      transition_to!(:standing)
    end
    @x = @x + dx
    @y = @y + dy
    move_to(@x, @y)
  end

  # Less performant this way
  # def draw(x, y, w, h)
  #   Playdate::Graphics.drawBitmap(@image_table.get_bitmap(@timer.value), x, y)
  # end
end
