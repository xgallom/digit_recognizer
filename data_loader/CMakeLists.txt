file(GLOB_RECURSE files "src/*.cpp")

add_library(data_loader STATIC ${files})
target_include_directories(data_loader PUBLIC include)

target_link_libraries(data_loader PUBLIC core)
target_link_libraries(data_loader PRIVATE tbb)
