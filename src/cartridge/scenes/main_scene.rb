require 'entities/player'

class MainScene < Pyrite::Scene
  def prepare
    entities.register(:player, Player)
    @x = (400 - $TEXT_WIDTH) / 2
    @y = (240 - $TEXT_HEIGHT) / 2
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
    @player = entities.create(:player, x: @x, y: @y)
  end

  def update
    @x, @y = @player.x, @player.y
  end

  def draw
    msg = "Hello there"
    Playdate::Graphics.drawText(msg, msg.length, @x, @y)
  end
end
