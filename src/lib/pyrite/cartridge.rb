module Pyrite
  class Cartridge
    def logger
      @logger ||= Logger.new
    end

    def draw
    end

    def update
    end

    def memory_slots
      objects = ObjectSpace.count_objects
      "Slots: #{objects[:FREE]} free / #{objects[:TOTAL]}"
    end
  end
end
