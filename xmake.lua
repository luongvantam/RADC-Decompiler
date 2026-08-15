add_rules("mode.debug", "mode.release")

set_languages("cxx17")

target("radc")
    set_kind("binary")
    add_files("src/*.cpp")
    
    -- Optimize for speed
    if is_mode("release") then
        set_optimize("fastest")
    end
