module Pyrite
  class Cartridge
    def logger
      @logger ||= Logger.new
    end

    def draw
    end

    def update
    end
  end
end
