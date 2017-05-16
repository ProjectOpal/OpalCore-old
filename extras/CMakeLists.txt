# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)


function(GENERATE_CPP_PROTO HDRS)
  if (NOT ARGN)
    return()
  endif(NOT ARGN)

  if(NOT DEFINED GIT_ROOT)
    set(GIT_ROOT "${CMAKE_BINARY_DIR}/..")
  endif(NOT DEFINED GIT_ROOT)
  set(TOOLS_DIR ${GIT_ROOT}/tools)

  list(REMOVE_DUPLICATES ARGN)
  foreach(item ${ARGN})
    string(REGEX MATCH ".*\\.pb\\.h" item ${item})
    if(item)
      string(CONCAT hpp_proto_hdr "${item}" "pp")
      set(output_hdr "${hpp_proto_hdr}")
      add_custom_command(
        OUTPUT ${output_hdr}
        COMMAND python3 ${TOOLS_DIR}/nanopb_cpp.py ${item}
        DEPENDS ${item}
        COMMENT "Building HPP Headers for ${hpp_proto_hdr}"
      )
      list(APPEND ${HDRS} "${output_hdr}")
    endif(item)
  endforeach(item)
  set_source_files_properties(${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)

endfunction(GENERATE_CPP_PROTO)
