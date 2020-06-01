file(GLOB_RECURSE files "src/*")

add_custom_target(data
        mkdir -p "${CMAKE_BINARY_DIR}/digit_recognizer/data"
        COMMAND
        cp ${files} "${CMAKE_BINARY_DIR}/digit_recognizer/data/"
        DEPENDS digit_recognizer
        SOURCES ${files}
)
