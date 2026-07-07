$TEXT_WIDTH = 86
$TEXT_HEIGHT = 16
$LCD_COLUMNS = 400
$LCD_ROWS = 240

$x = (400 - $TEXT_WIDTH) / 2
$y = (240 - $TEXT_HEIGHT) / 2
$dx = 1
$dy = 2
$name = "there"

$sprite = Playdate::Sprite.new
$image = Playdate::Graphics.loadBitmap('images/player')
$sprite.set_image($image)
$sprite.move_to($x, $y)

# Verifying load process works
# Playdate.load("cartridge/other.mrb")
require 'other'
require './other'

def game_update()
  Playdate::Graphics.clear()
  Playdate::Sprite.drawSprites()
  msg = "Hello #{$name}"
  # other_image = $sprite.get_image
  begin
    Playdate::System.logToConsole("image = #{$image.inspect}")
  rescue => e
    Playdate::System.logToConsole("raised #{e.inspect}")
  end


  # if other_image == image
  # else
  #   Playdate::System.logToConsole("image != other_image")
  # end
  Playdate::Graphics.drawText(msg, msg.length, $x, $y)
  $x += $dx
  $y += $dy
  $sprite.move_to($x, $y)

  $dx = -$dx unless $x.between?(0, $LCD_COLUMNS - $TEXT_WIDTH)
  $dy = -$dy unless $y.between?(0, $LCD_ROWS - $TEXT_HEIGHT)

  Playdate::System.drawFPS(0,0)
end
