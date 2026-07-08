module Pyrite
  class Logger
    def info(str)
      Playdate::System.logToConsole(str)
    end
    def warn(str)
      Playdate::System.logToConsole(str)
    end
  end
end
