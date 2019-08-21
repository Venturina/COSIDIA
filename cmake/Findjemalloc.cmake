find_program(jemalloc_CONFIG_PROGRAM NAMES jemalloc-config)
if(jemalloc_CONFIG_PROGRAM)
    execute_process(COMMAND ${jemalloc_CONFIG_PROGRAM} --includedir OUTPUT_VARIABLE jemalloc_hint_includedir)
    execute_process(COMMAND ${jemalloc_CONFIG_PROGRAM} --libdir OUTPUT_VARIABLE jemalloc_hint_libdir)
endif()

find_path(jemalloc_INCLUDE_DIR NAMES jemalloc/jemalloc.h
    HINTS "${jemalloc_hint_includedir}" DOC "jemalloc include directory")
find_library(jemalloc_LIBRARY NAMES jemalloc
    HINTS "${jemalloc_hint_libdir}" DOC "jemalloc library")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(jemalloc REQUIRED_VARS jemalloc_INCLUDE_DIR jemalloc_LIBRARY)

if(jemalloc_FOUND AND NOT TARGET jemalloc::jemalloc)
    add_library(jemalloc::jemalloc UNKNOWN IMPORTED)
    set_target_properties(jemalloc::jemalloc PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${jemalloc_INCLUDE_DIR}"
        IMPORTED_LOCATION "${jemalloc_LIBRARY}"
        IMPORTED_LINK_INTERFACE_LANGUAGES "C")
endif()

set(jemalloc_INCLUDE_DIRS "${jemalloc_INCLUDE_DIR}")
mark_as_advanced(jemalloc_CONFIG_PROGRAM jemalloc_INCLUDE_DIR jemalloc_LIBRARY)
