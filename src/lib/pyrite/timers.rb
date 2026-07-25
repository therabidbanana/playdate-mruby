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
      @active.reject!(&:expired?)
    end

    def handle_timer(timer_state, dt)
      timer_state.advance(dt)
      timer_state
    end

    def once(duration, delay: 0, &on_end)
      SimpleTimer.new(duration:, &on_end).tap do |me|
        @active << me
      end
    end

    def loop(duration, max_loops: 0, &on_end)
      LoopTimer.new(duration:, max_loops:, &on_end).tap do |me|
        @active << me
      end
    end

    class TimerState
      attr_accessor :type, :duration, :elapsed, :alive
      def initialize(duration:, elapsed: 0, alive: true, &on_expired)
        @alive = alive
        @elapsed = elapsed
        @duration = duration
        if block_given?
          @on_expired = on_expired
        end
      end

      def advance(dt)
        unless paused?
          add_time(dt)
          check_expired
        end
      end
      def add_time(dt) = @elapsed += dt
      def check_expired
        if expired?
          if @on_expired
            @on_expired.call
          end
          expire_timer
        end
      end
      def expire_timer
        @alive = false
      end
      def pause!   = @paused = true
      def unpause! = @paused = false
      def resume!  = @paused = false

      def alive?   = @alive
      def on?      = @alive
      def expired? = @elapsed >= duration
      def off?     = !@alive
      def dead?    = !@alive
      def paused?  = @paused
    end
    class SimpleTimer < TimerState
      def initialize(**kwargs)
        @type = :simple
        super
      end
    end
    class LoopTimer < TimerState
      def initialize(max_loops: 0, **kwargs)
        @type = :loop
        @loops = 0
        @max_loops = max_loops
        super
      end
      def expire_timer
        @loops += 1
        if @max_loops > 0 && @loops < @max_loops
          @alive = true
          @elapsed = @elapsed - @duration
        else
          @alive = false
        end
      end
    end
  end
end
