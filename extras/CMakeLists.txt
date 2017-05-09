# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

function(GENERATE_CPP_PROTO PROTOS)
  set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools)
  set(PROTO_HDRS)
  foreach(item ${PROTOS})
    string(REPLACE item "proto" "pb.h" ${item})
    set(PROTO_HDRS ${PROTO_HDRS} ${item})
  endforeach(item ${PROTOS})

  foreach(proto_hdr ${ONLY_PROTO_HDRS})
    string(CONCAT hpp_proto_hdr "${proto_hdr}" "pp")
    add_custom_command(
      OUTPUT ${hpp_proto_hdr}
      COMMAND python3 ${TOOLS_DIR}/nanopb_cpp.py ${proto_hdr}
      DEPENDS ${proto_hdr}
      COMMENT "Building HPP Headers for ${proto_hdr}"
    )
    set(PROTO_CPP_HDRS ${PROTO_CPP_HDRS} ${hpp_proto_hdr})
  endforeach()
endfunction()
