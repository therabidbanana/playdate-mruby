module Pyrite
  class Scene
    attr_reader :cartridge
    def initialize(cartridge)
      @cartridge = cartridge
      prepare
    end
    def scenes = @cartridge.scenes
    def logger = @cartridge.logger
    def timers = @cartridge.timers
    def inputs = @cartridge.inputs

    def prepare
    end

    def update
    end

    def draw
    end

    def cleanup
    end
  end

  class SceneManager
    attr_reader :active_scene
    attr_accessor :default_scene
    def initialize(cart)
      @scene_map = {}
      @active_scene = nil
      @cartridge = cart
    end

    def add_scene(name, klass)
      raise "Duplicate scene" if @scene_map.has_key?(name)
      @scene_map[name] = klass
      @default_scene ||= name
    end

    def enter_scene!(name = @default_scene)
      leave_scene! unless @active_scene.nil?
      @active_scene = @scene_map[name].new(@cartridge)
    end

    def leave_scene!
      # TODO: transitions?
      @active_scene.cleanup
    end
  end
end
