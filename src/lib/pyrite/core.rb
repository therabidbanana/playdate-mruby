module Pyrite
  def self.load_cartridge(klass)
    @current = klass.new
    @current.prepare
    raise "Cartridge has no scenes" unless @current.scenes.default_scene
    # Load default scene
    @current.scenes.enter_scene!
  end

  def self.current_cartridge
    raise "No cartridge loaded" unless @current
  end

  def self.game_update(dt = 0)
    @current.delta_time = dt
    @current.before_update
    @current.scenes.active_scene&.update
    @current.after_update
    @current.before_draw
    @current.scenes.active_scene&.draw
    @current.after_draw
  end
end
