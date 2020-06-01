file(GLOB_RECURSE files "src/*.cpp")

add_library(engine STATIC ${files})
target_include_directories(engine PUBLIC include)

target_link_libraries(engine PUBLIC core)
