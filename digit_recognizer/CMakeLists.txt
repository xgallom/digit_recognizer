file(GLOB_RECURSE files "src/*")

add_executable(digit_recognizer ${files})

target_link_libraries(digit_recognizer PRIVATE data_loader engine)
