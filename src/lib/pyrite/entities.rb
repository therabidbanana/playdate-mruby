module Pyrite
  class Entity < Playdate::Sprite
    attr_accessor :cartridge
    def inputs
      @cartridge.inputs
    end

    def prepare(*args, **kwargs)
    end
  end

  class Entities
    def initialize(cartridge)
      @entity_map = {}
      @sprite_list = []
      @cartridge = cartridge
    end
    def register(name, klass)
      @entity_map[name] = klass
    end
    def create(name, *args, **kwargs)
      @entity_map[name].new.tap do |sprite|
        sprite.cartridge = @cartridge
        sprite.prepare(*args, **kwargs)
        @sprite_list << sprite
      end
    end

    def update_all
      @sprite_list.each{|sprite| sprite.update(@cartridge.active_scene) if sprite.respond_to?(:update) }
    end

    def draw_all
      Playdate::Sprite.drawSprites()
    end
  end
end
