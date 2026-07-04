$TEXT_WIDTH = 86
$TEXT_HEIGHT = 16
$LCD_COLUMNS = 400
$LCD_ROWS = 240

$x = (400 - $TEXT_WIDTH) / 2
$y = (240 - $TEXT_HEIGHT) / 2
$dx = 1
$dy = 2
Playdate::System.logToConsole('Foobar')

def game_update()
  Playdate::Graphics.clear()
  msg = "Hello Ruby"
  Playdate::Graphics.drawText(msg, msg.length, $x, $y)
  $x += $dx;
  $y += $dy;


  $dx = -$dx unless $x.between?(0, $LCD_COLUMNS - $TEXT_WIDTH)
  $dy = -$dy unless $y.between?(0, $LCD_ROWS - $TEXT_HEIGHT)

  Playdate::System.drawFPS(0,0)
end
