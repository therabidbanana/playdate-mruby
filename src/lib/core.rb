
module Kernel
  LOADED = {}

  def __file_path(filename)
    parts = filename.split('/')
    parts.pop
    parts.join('/')
  end

  # Returns the source path (later could use to strip out part of __FILE__ that isn't used)
  def __project_source_path
    __FILE__.delete_suffix('/lib/core.rb')
  end

  def require(name, curr_name = $PD_FILENAME)
    name = "#{name}.mrb" unless name.end_with?(".mrb")
    fullname = if name.start_with?('/')
                  name.delete_prefix('/')
              elsif name.start_with?('./')
                name = name.delete_prefix('./')
                "#{__file_path(curr_name)}/#{name}"
              else
                "cartridge/#{name}"
              end
    if LOADED[fullname]
      Playdate::System.logToConsole("[require] #{fullname} was already required. Skipping")
    else
      LOADED[fullname] = true
      __pd_load(fullname)
    end
  end
end
