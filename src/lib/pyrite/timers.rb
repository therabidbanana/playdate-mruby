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

    def add_timer(timer)
      # TODO: probably not a good place for a scan
      @active << timer unless @active.include?(timer)
      timer
    end

    def reset(timer)
      timer.reset!
      add_timer(timer)
    end

    def once(duration, delay: 0, &on_end)
      add_timer SimpleTimer.new(duration:, &on_end)
    end

    def loop(duration, max_loops: 0, &on_end)
      add_timer LoopTimer.new(duration:, max_loops:, &on_end)
    end

    def animation(delay, min:, max:, loops: 0, &on_end)
      add_timer AnimationTimer.new(duration: delay, min:, max:, max_loops: loops, &on_end)
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
          maybe_call_expired
          expire_timer
        end
      end
      def maybe_call_expired
        if @on_expired
          @on_expired.call
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
      def reset!
        @alive = true
        @elapsed = 0
      end
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
      def reset!
        @loops = 0
        super
      end
    end
    class AnimationTimer < TimerState
      attr_accessor :value
      def initialize(min:, max:, max_loops: 0, **kwargs)
        @type = :animation
        @loops = 0
        @value = min
        @min = min
        @max = max
        @max_loops = max_loops
        super
      end
      def maybe_call_expired
        if @on_expired
          @on_expired.call(@value, (@value >= @max))
        end
      end
      def expire_timer
        if (@value < @max)
          @value += 1
          @alive = true
          @elapsed = @elapsed - @duration
        elsif @max_loops.zero?
          @alive = false
        elsif @max_loops > 0 && @loops < @max_loops
          @loops += 1
          @value = @min
          @alive = true
          @elapsed = @elapsed - @duration
        elsif @max_loops.negative?
          @value = @min
          @alive = true
          @elapsed = @elapsed - @duration
        else
          @alive = false
        end
      end
      def reset!
        @loops = 0
        @value = @min
        super
      end
    end
  end
end
