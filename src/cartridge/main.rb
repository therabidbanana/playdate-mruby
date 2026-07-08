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
    @dx = 1
    @dy = 2
    @player = boot_player
    logger.info "Initialized game"
  end

  def boot_player
    Playdate::Sprite.new.tap do |sprite|
      sprite.set_image(Playdate::Graphics.loadBitmap('images/player'))
      sprite.move_to(@x, @y)
    end
  end

  def update
    @x += @dx
    @y += @dy
    @player.move_to(@x, @y)

    @dx = -@dx unless @x.between?(0, $LCD_COLUMNS - $TEXT_WIDTH)
    @dy = -@dy unless @y.between?(0, $LCD_ROWS - $TEXT_HEIGHT)
  end

  def draw
    Playdate::Graphics.clear()
    Playdate::Sprite.drawSprites()
    msg = "Hello #{$name}"
    Playdate::Graphics.drawText(msg, msg.length, @x, @y)
    Playdate::System.drawFPS(0,0)
  end
end

Pyrite.load_cartridge(HelloMruby)
