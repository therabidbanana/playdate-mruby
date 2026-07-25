module Pyrite
  class Inputs
    MAP = {
      a: Playdate::System::BUTTON_A,
      b: Playdate::System::BUTTON_B,
      up: Playdate::System::UP,
      down: Playdate::System::DOWN,
      left: Playdate::System::LEFT,
      right: Playdate::System::RIGHT,
    }
    def to_button(symbol)
      MAP[symbol]
    end

    def buttons_pressed = Playdate::System.getButtonState
    def buttons_just_pressed =  Playdate::System.getButtonStatePressed
    def buttons_just_released =  Playdate::System.getButtonStateReleased

    def pressed?(symbol)
      (buttons_pressed & to_button(symbol)) != 0
    end

    def just_pressed?(symbol)
      (buttons_just_pressed & to_button(symbol)) != 0
    end

    def just_released?(symbol)
      (buttons_just_released & to_button(symbol)) != 0
    end
  end
end
