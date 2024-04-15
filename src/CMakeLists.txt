file(GLOB_RECURSE WL_SRC_FILES
   "*.cpp" "**/*.cpp")
file(GLOB_RECURSE WL_PRIVATE_HEADER_FILES
    "*.hpp" "**/*.hpp")
file(GLOB_RECURSE WL_PUBLIC_HEADER_FILES
    "../include/*.hpp" "../include/**/*.hpp")

add_library(core STATIC ${WL_SRC_FILES} ${WL_PRIVATE_HEADER_FILES} ${WL_PUBLIC_HEADER_FILES})
set_target_properties(core PROPERTIES OUTPUT_NAME wl_core)

# Create an alias for simpler reference
add_library(wl::core ALIAS core)

target_link_options(core PRIVATE -static-libstdc++)

# Use include depending on building or using from installed location
target_include_directories(core
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS core
    EXPORT wlcoreTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT wlcoreTargets
    NAMESPACE wl::
    COMPONENT core
    FILE "wlcoreTargets.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wl"
)

# Generate build tree export file
export(EXPORT wlcoreTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/wlcoreTargets.cmake"
    NAMESPACE wl::
)
