module Pyrite
  class Entity < Playdate::Sprite
    def self.image_table(string = nil)
      if string
        # Shared by all entities at definition time
        @image_table = Playdate::Graphics.loadBitmapTable(string)
      else
        @image_table
      end
    end

    def self.animation_state(name, positional_frames = nil, frames: nil, delay: 300, and_then: nil, default: false)
      animation_state = { delay: delay }
      animation_state[:next_transition] = and_then if and_then
      frames ||= positional_frames
      frames ||= 0
      if frames.is_a?(Range)
        animation_state[:start] = frames.min
        animation_state[:end] = frames.max
      elsif frames.is_a?(Numeric)
        animation_state[:start] = animation_state[:end] = frames
      end
      @default_state = name if default

      @states ||= {}
      @states[name] = animation_state
    end

    def self.states
      @states
    end

    def self.default_state
      @default_state || @states.keys.first
    end

    attr_accessor :cartridge
    def inputs
      @cartridge.inputs
    end

    # transition to the start of a new animation state.
    # Ignore if current state matches unless force also set.
    def transition_to!(new_state, force: false)
      return if new_state == @state && !force
      # cartridge.logger.info "curr state #{@state} - #{@animation_states[@state].inspect}"
      cartridge.timers.reset(@animation_states[@state])
      @animation_states[@state].pause!
      @state = new_state
      cartridge.timers.reset(@animation_states[new_state])
      @animation_states[new_state].unpause!
      cartridge.logger.info "new state #{@state} - #{@animation_states[@state].inspect}"
      set_image(@image_table.get_bitmap(@animation_states[new_state].value))
      mark_dirty!
    end

    # Internal hook to setup state machine or other bits
    def before_prepare
      @state = self.class.default_state
      @image_table = self.class.image_table if self.class.image_table
      if self.class.states
        # cartridge.logger.info "initializing animation states: #{self.class.states.inspect}"
        @animation_states = self.class.states.map do |name, state|
          if state[:next_transition]
            loops = 0
          else
            loops = -1
          end
          timer = cartridge.timers.animation(state[:delay], min: state[:start], max: state[:end], loops:) do |value, last_tick|
            if state[:next_transition] && last_tick
              transition_to!(state[:next_transition])
            else
              # cartridge.logger.info "animation #{name}: #{value} #{last_tick}?"
              set_image(@image_table.get_bitmap(value))
              mark_dirty!
            end
          end
          timer.pause!
          [name, timer]
        end.to_h
        # cartridge.logger.info "initialized animation states: #{@animation_states.inspect}"
        transition_to!(@state, force: true)
      end
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
        sprite.before_prepare
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
