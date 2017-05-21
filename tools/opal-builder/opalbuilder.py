

# Ask user to pick modules
#   Find all modules
# Attempt to auto connect modules
#   Modules must have inputs and outputs
#   Graph to connect inputs and outputs
#   (Note: Need some way to have no input and no output, aka test scripts)
# Create CMakeLists.txt file
#   Have a template CMakeLists.txt file that can be substituded by python script
# Create Main file
#   Have template main file that can be sub by python script
#   Need to know locations of module files for includes
# Compile
#   run cmake
#   Need to have way to compile modules on their own (?maybe?)


# Module datasctructure
#   Will use TinyDB to store information as follows:
#     name, path, inputs, outputs
#
# Need to be able to build a graph connecting inputs to outputs

# Find all modules
#   Recursively search directories for modules starting from src
#   Store module information in tinydb
#   This should be done for each compile time

import os, re, json
import CppHeaderParser

MODULE_FILE = "module.opal"
MODULE_CLASS_NAME = "OpalModule"
INPUT_SHARED_VAR_REGEX = re.compile("InputSharedVariable<(.*?)>")
OUTPUT_SHARED_VAR_REGEX = re.compile("OutputSharedVariable<(.*?)>")

class OpalModule:
  protos = set([])
  def __init__(self, name="", path="", inputs=None, outputs=None):
    self.inputs = []
    self.outputs = []
    self.name = name
    self.path = path
    if (inputs):
      self.inputs = inputs
      OpalModule.protos.update(inputs)
    else:
      self.inputs = []
    if (outputs):
      self.outputs = outputs
      OpalModule.protos.update(outputs)
    else:
      self.outputs = []

  def __bool__(self):
    return bool(self.path)
  def __str__(self):
    return """Module Path: {}
    Module Name: {}
    Module Inputs: {}
    Module Outputs {}""".format(self.path, self.name, self.inputs, self.outputs);
  def __eq__(self, other):
    if(not type(other) == type(self)):
      return False
    if(not other.name == self.name):
      return False
    if (not other.path == self.path):
      return False
    if (not set(other.inputs) == set(self.inputs)):
      return False
    if (not set(other.outputs) == set(self.outputs)):
      return False
    return True
  def __hash__(self):
    return hash(self.path+self.name)

  def AddInput(self, proto_name):
    self.inputs.append(proto_name)
    OpalModule.protos.add(proto_name)

  def AddOutput(self, proto_name):
    self.outputs.append(proto_name)
    OpalModule.protos.add(proto_name)

  def ClearAllProtos():
    protos.clear()

  def RemoveProto(proto_name):
    protos.discard(proto_name)

# Finds all the module's root directories. Starts search from base_dir.
def FindModuleRoots(base_dir):
  r = re.compile(MODULE_FILE)
  dirs = [base_dir]
  module_roots = []
  while (len(dirs) > 0):
    for root, subdirs, files in os.walk(dirs.pop()):
      dirs += subdirs
      opal_files = list(filter(r.match, files))
      if (len(opal_files) > 1):
        raise Exception("Too many .opal files located in directory {}".format(root))
      elif (len(opal_files) == 1):
        module_roots.append(root)
  return module_roots

# Parses a module file. File is simple JSON with the name of the project and
#   header files to include.
def CreateModuleFile(module_name, header_files, location):
  data = {"name": module_name, "header_files": header_files}
  with open(os.path.join(location, "module.opal"), "w+") as opal_file:
    json.dump(data, opal_file)

def ParseModule(file_dir):
  # Load JSON data from .opal file
  json_data = open(os.path.join(file_dir, MODULE_FILE)).read()
  data = json.loads(json_data)
  for hdr in data['header_files']:
    hdr_path = os.path.join(file_dir, hdr)
    with open(hdr_path, 'r') as hdr_file:
      ModuleFromHeader(hdr_file)

# Given a header file, find the modules and create the module structure.
# If the header file can't be parsed, returns None
def ModuleFromHeader(filename):
  def filter_for_module(x):
    inherit_list = x[1].get('inherits')
    for item in inherit_list:
      if (MODULE_CLASS_NAME in item['class']):
        return True
  cpp_hdr = None
  try:
    cpp_hdr = CppHeaderParser.CppHeader(filename)
  except:
    return None

  opal_modules = list(filter(filter_for_module, list(cpp_hdr.classes.items())))

  modules = []
  for module in opal_modules:
    cur_opal_module = OpalModule()
    cur_opal_module.path = filename
    cur_opal_module.name = "{}::{}".format(module[1].get('namespace'), module[0])
    variables = None
    try:
      variables = module[1].get('properties').get('public')
    except:
      continue

    for variable in variables:
      input_proto = INPUT_SHARED_VAR_REGEX.search(variable.get('type'))
      if (input_proto):
        cur_opal_module.AddInput((input_proto[1], variable.get('name')))
        continue
      output_proto = OUTPUT_SHARED_VAR_REGEX.search(variable.get('type'))
      if (output_proto):
        cur_opal_module.AddOutput((output_proto[1], variable.get('name')))
        continue
    modules.append(cur_opal_module)
  return modules






