function(ensure_sources)
  file(GLOB glob_files *.cpp)
  list(TRANSFORM glob_files REPLACE "^.*/" "")
  foreach(file IN LISTS glob_files)
    if(NOT file IN_LIST ARGV)
      list(APPEND missing_files "${file}")
    endif()
  endforeach()
  if(missing_files)
    list(JOIN missing_files " " missings)
    message(FATAL_ERROR "missing sources to add: ${missings}")
  endif()
endfunction()