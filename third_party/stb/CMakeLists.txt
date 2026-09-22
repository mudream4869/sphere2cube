add_library(stb STATIC stb_impl.cpp)
add_library(stb::stb ALIAS stb)

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(stb PRIVATE /w)
else()
    target_compile_options(stb PRIVATE -w)
endif()
