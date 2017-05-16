cmake_minimum_required(VERSION 3.0)

# TODO @smr277: Make this a file glob to auto add all sub folders
file(GLOB FILES "*")
foreach(item ${FILES})
  if(IS_DIRECTORY ${item})
    add_subdirectory(${item})
  endif(IS_DIRECTORY ${item})
endforeach(item)

