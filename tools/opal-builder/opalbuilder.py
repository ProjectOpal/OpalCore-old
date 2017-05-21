

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

import os, re, json, sys, pprint
import CppHeaderParser
from tinydb import TinyDB, Query
from graphviz import Digraph #NOTE: Need to have GraphViz installed!!

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
  def __dict__(self):
    return {'path': self.path, 'name': self.name, 'inputs': self.inputs,
            'outputs': self.outputs}

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
  modules = []
  for hdr in data['header_files']:
    hdr_path = os.path.join(file_dir, hdr)
    with open(hdr_path, 'r') as hdr_file:
      found_modules = ModuleFromHeader(hdr_file)
      if (found_modules):
        modules += found_modules

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

# Creates a graph that connect the nodes with the same input and output types
# The graph is a dict with the key is
#   (OpalModule, output_type:str) -> [OpalModule...]
def CalculateConnections(opal_modules, tinydb=None):
  if (tinydb is None):
    return
  for mod in opal_modules:
    ModuleInsertDB(mod, tinydb)
  graph = {}
  for mod in DBAllModules(tinydb):
    opal_mod = DBtoModule(mod)
    for (output_type , _) in opal_mod.outputs:
      # Create a new node with this module and output type
      # Add an edge from this node to the modules that have an input of this
      #   output_type
      graph[(opal_mod, output_type)] = DBFindModulesWithInputType(output_type,
                                                                  tinydb)
  # TODO: Graph connects all inputs and outputs of the same type. User input
  #     will be required in order to differentiate between
  return graph

# Creates a database storeable object (easy to search) from an opal module
# Updates dictionary
def ModuleInsertDB(module, db):
  db_dict = {}
  db_dict['db_inputs'] = [x[0] for x in module.inputs]
  db_dict['db_outputs'] = [x[0] for x in module.outputs]
  db_dict.update(module.__dict__())
  db.insert(db_dict)

# Read in a tinydb dict and create an opal module from it
# returns an OpalModule object
def DBtoModule(db_readin):
  mod_dict = db_readin.copy()
  mod_dict['inputs'] = [tuple(x) for x in mod_dict['inputs']]
  mod_dict['outputs'] = [tuple(x) for x in mod_dict['outputs']]
  mod_dict.pop('db_inputs')
  mod_dict.pop('db_outputs')
  return OpalModule(**mod_dict)

# Find all modules that have the input type of this output type
# For example, if you have moduleA with output_type proto_foo, you
# can use this to find all modules with input_type proto_foo.
# Returns a list of OpalModule objects
def DBFindModulesWithInputType(output_type, db):
  db_modules = Query()
  opal_mods = []
  for mod in db.search(db_modules.db_inputs.any([output_type])):
    opal_mods.append(DBtoModule(mod))
  return opal_mods

# Return a list of all the modules.
def DBAllModules(db):
  return db.all()

# Visualize a graph
def VisualizeGraph(graph, filename):
  dot = Digraph(name='Your current OpalPilot configuration')
  dot.attr(rankdir='LR', size='16,10')
  opal_mods = set([x[0] for x in graph.keys()])
  for mod in opal_mods:
    dot.node(str(hash(mod)), mod.name)
  for ((node , output_type), input_node_list)  in graph.items():
    for input_node in input_node_list:
      dot.edge(str(hash(node)),  str(hash(input_node)), label = output_type)
  dot.render(filename, view=True)








