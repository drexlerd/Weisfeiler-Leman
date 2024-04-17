find_package(pybind11 CONFIG REQUIRED PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
# Set result variables
find_package(pybind11)

pybind11_add_module(_pykwl MODULE)
target_sources(_pykwl
    PRIVATE
        main.cpp
        pykwl/bindings.cpp)
target_link_libraries(_pykwl
    PRIVATE
        pybind11::module
        wl::core)
target_link_options(_pykwl PRIVATE -static-libstdc++)
target_compile_definitions(_pykwl PUBLIC WL_VERSION_INFO="${WL_VERSION_INFO}")
