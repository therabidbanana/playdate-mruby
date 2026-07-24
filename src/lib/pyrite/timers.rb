module Pyrite
  class Timers
    def initialize
      @active = []
    end

    # ticks in milliseconds
    def tick(dt)
      @active.each do |timer_state|
        handle_timer(timer_state, dt)
      end
      @active.select!(&:alive?)
    end

    def handle_timer(timer_state, dt)
      timer_state.elapsed += dt
      if timer_state.elapsed >= timer_state.duration
        timer_state.alive = false
      end
      timer_state
    end

    def once(duration, delay: 0)
      SimpleTimer.new(duration:).tap do |me|
        @active << me
      end
    end

    class TimerState
      attr_accessor :type, :duration, :elapsed, :alive
      def initialize(duration:, elapsed: 0, alive: true)
        @alive = alive
        @elapsed = elapsed
        @duration = duration
        @type = :simple
      end

      def alive? = @alive
      def on?    = @alive
      def off?   = @alive
      def dead?  = !@alive
    end
    class SimpleTimer < TimerState
      def initialize(**kwargs)
        @type = :simple
        super
      end
    end
  end
end
