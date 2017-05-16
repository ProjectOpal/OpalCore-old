"""
# Creates C++ header from Nanopb proto H header.
#
# Author: Sam Rosenstein
# Date: 4/27/17
#
# Runs in CMake script during build to create HPP headers with correct
# namespaces for protobuf messages.
#
# Example: NanoPB compiles 'package core.sensors' to 'core_sensors_<message_name>'
#          This script takes 'core_sensors_<message_name>' and creates nested
#          namespaces: 'namespace core { namespace sensors { ... }}'
#
# Run: python3 nanopb_cpp.py <nanopb_file.pb.h>
"""
import glob
import re, sys, os

# TODO @smr277: Create default constructor for protos

# @arg header_name: NanoPB header name (*.pb.h)
# @arg deconstructed_structs: A list of dictionaries with
#   {'namespaces':[], 'msg_name':str} where the namespaces is an ordered list of
#   namespaces, and the msg_name is the message name.
# @return: The string to write to the new header file
def CreateHeader(header_name, deconstructed_structs):
  output_string = ['#include "{}"\n'.format(header_name)]
  for struct in deconstructed_structs:
    for i, namespace in enumerate(struct['namespaces']):
      output_string.append(" " * i + "namespace " + namespace + " {")
    struct_old_name = "_".join(struct['namespaces']) + "_" + struct['msg_name']
    output_string.append(" " * len(struct['namespaces']) + "  " +
      "typedef {} {};".format(struct_old_name, struct['msg_name']))
    for i in range(len(struct['namespaces']) - 1, -1, -1):
      output_string.append(" " * i + "}")

  output_string = "\n".join(output_string)
  return output_string

# @arg nanoPBvar_name: The NanoPB variable name to parse
# return: A dictionary with {'namespaces':[], msg_name:str} where the namespaces
#     is an ordered list of namespaces, and the msg_name is the message name.
def DeconsrtuctNanoPBVar(nanoPBvar_name):
  names = nanoPBvar_name.split('_')
  output = {}
  output['namespaces'] = [name.strip() for name in names[:-1]]
  output['msg_name'] = names[-1].strip()
  return output


# Start of script

pb_h_file = None
file_write_path = ""
if len(sys.argv) == 2:
  pb_h_file = sys.argv[1]
elif len(sys.argv) == 3:
  pb_h_file = sys.argv[1]
  file_write_path = sys.argv[2]
else:
  print("ERROR: No input file")
  sys.exit()

re_var_name = re.compile('typedef struct.*?{[\s\S]*?}([\s\S]*?);')
with open(os.path.join(file_write_path, pb_h_file), "r+") as f:
  data = "".join([line for line in f])
  structs = re.findall(re_var_name, data)
  deconstructed_structs = []
  for struct in structs:
    deconstructed_structs.append(DeconsrtuctNanoPBVar(struct))
  new_header_contents = CreateHeader(pb_h_file, deconstructed_structs)
  new_filename = pb_h_file + "pp"
  with open(new_filename, "w+") as new_f:
    new_f.write(new_header_contents)




