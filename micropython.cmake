# Create an INTERFACE library for our C module.
add_library(usermod_ucrypto INTERFACE)

# Add our source files to the lib
target_sources(usermod_ucrypto INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/moducrypto.c
    ${CMAKE_CURRENT_LIST_DIR}/tomsfastmath/tfm_mpi.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_ucrypto INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_ucrypto INTERFACE
    MICROPY_PY_UCRYPTO=1
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_ucrypto)