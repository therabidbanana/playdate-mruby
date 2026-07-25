$TEXT_WIDTH = 86
$TEXT_HEIGHT = 16
$LCD_COLUMNS = 400
$LCD_ROWS = 240

require 'scenes/main_scene'

class HelloMruby < Pyrite::Cartridge
  def initialize
    @debug = true
    scenes.add_scene(:main, MainScene)
  end
end

Pyrite.load_cartridge(HelloMruby)
