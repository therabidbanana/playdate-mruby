module Pyrite
  class Logger
    def info(str)
      Playdate::System.logToConsole(str.to_s)
    end

    def warn(str)
      Playdate::System.logToConsole(str.to_s)
    end
  end
end
