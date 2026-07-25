class MainScene < Pyrite::Scene
  def prepare
    # entities.add(:player, Player)
    @x = (400 - $TEXT_WIDTH) / 2
    @y = (240 - $TEXT_HEIGHT) / 2
    @dx = 1
    @dy = 2
    @player = boot_player
    logger.info "Initialized game"
    timers.once(3000) do
      logger.info "Expired!"
    end
    timers.loop(1000, max_loops: 5) do
      logger.info(cartridge.memory_slots)
      logger.info(cartridge.delta_time.to_s)
    end
  end

  def boot_player
    # entities.create(:player)

    image_table = Playdate::Graphics.loadBitmapTable('images/pineapple-walk')
    @bitmap = image_table.get_bitmap(1)
    Playdate::Sprite.new.tap do |sprite|
      sprite.set_image(@bitmap)
      sprite.move_to(@x, @y)
    end
  end

  def update
    if inputs.pressed?(:left)
      @x -= @dx
    elsif inputs.pressed?(:right)
      @x += @dx
    end
    if inputs.pressed?(:up)
      @y -= @dy
    elsif inputs.pressed?(:down)
      @y += @dy
    else
      @y = @y
    end
    @player.move_to(@x, @y)
    @player.set_image(@bitmap)
    # @dx = -@dx unless @x.between?(0, $LCD_COLUMNS - $TEXT_WIDTH)
    # @dy = -@dy unless @y.between?(0, $LCD_ROWS - $TEXT_HEIGHT)
  end

  def draw
    msg = "Hello there"
    Playdate::Graphics.drawText(msg, msg.length, @x, @y)
  end
end
