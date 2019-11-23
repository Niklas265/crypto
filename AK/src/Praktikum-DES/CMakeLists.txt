# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-DES
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i des-getopt.ggo -F des-getopt
        DEPENDS des-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable( des des-main DESCipher BlockCipher
        ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.c
        )
add_executable( desTest desTest DESCipher BlockCipher catch.hpp)


