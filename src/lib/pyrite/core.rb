module Pyrite
  def self.load_cartridge(klass)
    @current = klass.new
    raise "Cartridge cannot draw" unless @current.respond_to?(:draw)
    raise "Cartridge cannot update" unless @current.respond_to?(:update)
  end

  def self.current_cartridge
    raise "No cartridge loaded" unless @current
  end

  def self.game_update(dt = 0)
    @current.delta_time = dt
    @current.before_update
    @current.update
    @current.after_update
    @current.before_draw
    @current.draw
    @current.after_draw
  end
end
