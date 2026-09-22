# 把 stb 包成一個 target：使用者只要 link 它就能 include header，
# implementation 由 stb_impl.cpp 提供。
add_library(stb STATIC stb_impl.cpp)
add_library(stb::stb ALIAS stb)

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 第三方程式碼，不吃本專案的 warning 設定。
if(MSVC)
    target_compile_options(stb PRIVATE /w)
else()
    target_compile_options(stb PRIVATE -w)
endif()
