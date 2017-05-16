cmake_minimum_required(VERSION 3.0)

file(GLOB FILES "*")
foreach(item ${FILES})
  if(IS_DIRECTORY ${item})
    add_subdirectory(${item})
  endif(IS_DIRECTORY ${item})
endforeach(item)


