import glob
import re, sys

# Read the build directory
# Find all *.pb.h files
# Read over those files and find all of the functions within the files
# Create the hpp file with the correct classes

def CreateHeader(header_name, deconstructed_structs):
  output_string = ['#include "{}"\n'.format(header_name)]
  for struct in deconstructed_structs:
    for i, namespace in enumerate(struct['namespaces']):
      output_string.append(" " * i + "namespace " + namespace + " {")
    struct_old_name = "_".join(struct['namespaces']) + "_" + struct['var_name']
    output_string.append(" " * len(struct['namespaces']) + "  " +
      "typedef {} {};".format(struct_old_name, struct['var_name']))
    for i in range(len(struct['namespaces']) - 1, -1, -1):
      output_string.append(" " * i + "}")

  output_string = "\n".join(output_string)
  return output_string

def DeconsrtuctNanoPBVar(nanoPBvar_name):
  names = nanoPBvar_name.split('_')
  output = {}
  output['namespaces'] = [name.strip() for name in names[:-1]]
  output['var_name'] = names[-1].strip()
  return output


# Start of script

pb_h_file = []
if len(sys.argv) > 1:
  pb_h_file = sys.argv[1]
else:
  print("ERROR: No input files")

re_var_name = re.compile('typedef struct.*?{[\s\S]*?}([\s\S]*?);')
with open(pb_h_file, "r+") as f:
  data = "".join([line for line in f])
  structs = re.findall(re_var_name, data)
  deconstructed_structs = []
  for struct in structs:
    deconstructed_structs.append(DeconsrtuctNanoPBVar(struct))
  new_header_contents = CreateHeader(pb_h_file, deconstructed_structs)
  new_filename = pb_h_file + "pp"
  with open(new_filename, "w+") as new_f:
    new_f.write(new_header_contents)




