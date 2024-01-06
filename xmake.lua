
add_rules("mode.debug")


target("demo")
    set_kind("binary")

    add_includedirs("src/")
    add_files("src/*.cpp")

    set_targetdir("bin")