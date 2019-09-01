project(src CXX)

set(target "${PROJECT_NAME}")
add_library("${target}" STATIC)
# source
target_sources("${target}" PRIVATE
  domain.cpp
  env.cpp
  interpret.cpp
  json.cpp
  map.cpp
  symbol.cpp
  type.cpp
  value.cpp
  )
include(EnsureSources)
ensure_sources("${target}")
target_include_directories("${target}" PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}")
# option
target_compile_features("${target}" PUBLIC
  cxx_std_17)
target_compile_options("${target}" PUBLIC
  -Wall -Wextra -pedantic)
# link
target_link_libraries("${target}"
  nlohmann-json
  llvm
  )
