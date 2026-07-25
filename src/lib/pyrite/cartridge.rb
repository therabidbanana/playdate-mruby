module Pyrite
  class Cartridge
    attr_accessor :delta_time
    attr_accessor :debug

    def logger
      @logger ||= Logger.new
    end

    def timers
      @timers ||= Timers.new
    end

    def inputs
      @inputs ||= Inputs.new
    end

    def scenes
      @scenes ||= SceneManager.new(self)
    end

    def prepare
    end

    #### Internal hooks

    def before_update
      timers.tick((delta_time * 1000).floor)
      # Update timers
    end

    def after_update
      # Not sure anything goes here
    end

    def before_draw
      Playdate::Sprite.drawSprites()
    end

    def after_draw
      if debug
        Playdate::System.drawFPS(0,0)
        Playdate::Graphics.drawText(memory_slots, memory_slots.length, 0, 10)
      end
    end

    def memory_slots
      objects = ObjectSpace.count_objects
      "Slots: #{objects[:FREE]} free / #{objects[:TOTAL]}"
    end
  end
end
