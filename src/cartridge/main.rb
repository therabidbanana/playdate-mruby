$TEXT_WIDTH = 86
$TEXT_HEIGHT = 16
$LCD_COLUMNS = 400
$LCD_ROWS = 240

$name = "there"


# Verifying load process works
# Playdate.load("cartridge/other.mrb")
require 'other'
require './other'

class HelloMruby < Pyrite::Cartridge
  def initialize
    @x = (400 - $TEXT_WIDTH) / 2
    @y = (240 - $TEXT_HEIGHT) / 2
    @dt = 0
    @dx = 1
    @dy = 2
    @player = boot_player
    logger.info "Initialized game"
    @debug = true
    @timer = timers.once(3000)
  end

  def boot_player
    image_table = Playdate::Graphics.loadBitmapTable('images/pineapple-walk')
    @bitmap = image_table.get_bitmap(1)
    Playdate::Sprite.new.tap do |sprite|
      sprite.set_image(@bitmap)
      sprite.move_to(@x, @y)
    end
  end

  def update
    @x += @dx
    @y += @dy
    @dt += 1
    @player.move_to(@x, @y)
    @player.set_image(@bitmap)
    logger.info(memory_slots) if (@dt % 30).zero?
    logger.info(delta_time.to_s) if (@dt % 30).zero?
    if @timer&.dead?
      logger.info("Timer expired #{@timer.inspect}")
      @timer = nil
    end

    @dx = -@dx unless @x.between?(0, $LCD_COLUMNS - $TEXT_WIDTH)
    @dy = -@dy unless @y.between?(0, $LCD_ROWS - $TEXT_HEIGHT)
  end

  def draw
    msg = "Hello #{$name}"
    Playdate::Graphics.drawText(msg, msg.length, @x, @y)
  end
end

Pyrite.load_cartridge(HelloMruby)
