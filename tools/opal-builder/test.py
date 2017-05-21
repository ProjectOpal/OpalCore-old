import unittest, os, shutil, logging, sys
from tinydb import TinyDB, Query

import opalbuilder

def touch(fname, times=None):
    fhandle = open(fname, 'a')
    try:
        os.utime(fname, times)
    finally:
        fhandle.close()

# Create a specific file structure. Provided as lists of
#   tuples: name, underlying files
#   Example:
#     [("src", [("core", [("moduleA", ["module.opal", "module.cpp"])])])]
#     would create the following structure:
#     - src
#       - core
#         - moduleA
#           - module.opal
#           - module.cpp
def CreateFileStructure(base_list, base_dir):
  dirs = filter(lambda x : type(x) == tuple, base_list)
  files = filter(lambda x : type(x) == str, base_list)
  for file_to_make in files:
    touch(os.path.join(base_dir, file_to_make))
  for dir_to_make in dirs:
    new_base_dir = os.path.join(base_dir, dir_to_make[0])
    try:
      os.mkdir(new_base_dir)
    except:
      return
    CreateFileStructure(dir_to_make[1], new_base_dir)

def DestroyFileStructure(base_list, base_dir):
  dirs = filter(lambda x : type(x) == tuple, base_list)
  for dir_to_delete in dirs:
    shutil.rmtree(os.path.join(base_dir, dir_to_delete[0]))


class TestFindModuleRoots(unittest.TestCase):
  simple = [
    ("src",
      [("core",
        [("moduleA", ["module.opal", "module.cpp"]),
        ("moduleB", ["module.opal", "module.cpp"])]
      )]
    )]
  simple_check = ["test/src/core/moduleA", "test/src/core/moduleB"]

  simple_one_mod = [
    ("src",
      [("core",
        [("moduleA", ["module.opal", "module.cpp"]),
        ("moduleB", ["module.cpp"])]
      )]
    )]
  simple_one_mod_check = ["test/src/core/moduleA"]

  mod_in_mod = [
    ("src",
      ["module.opal", ("core",
        [("moduleA", ["module.opal", "module.cpp"]),
        ("moduleB", ["module.cpp"])]
      )]
    )]
  mod_in_mod_check = ["test/src", "test/src/core/moduleA"]

  mod_exception = [
    ("src",
      ["module.opal", ("core",
        [("moduleA", ["module.opal", "module_fake.opal"]),
        ("moduleB", ["module.cpp"])]
      )]
    )]

  def test_simple(self):
    CreateFileStructure(self.simple, "test")
    output = opalbuilder.FindModuleRoots("test")
    self.assertEqual(set(output), set(self.simple_check))
    DestroyFileStructure(self.simple, "test")

  def test_simple_one_mod(self):
    CreateFileStructure(self.simple_one_mod, "test")
    output = opalbuilder.FindModuleRoots("test")
    self.assertEqual(set(output), set(self.simple_one_mod_check))
    DestroyFileStructure(self.simple_one_mod, "test")

  def test_mod_in_mod(self):
    CreateFileStructure(self.mod_in_mod, "test")
    output = opalbuilder.FindModuleRoots("test")
    self.assertEqual(set(output), set(self.mod_in_mod_check))
    DestroyFileStructure(self.mod_in_mod, "test")

  def test_mod_in_exception(self):
    CreateFileStructure(self.mod_in_mod, "test")
    try:
      output = opalbuilder.FindModuleRoots("test")
      self.assertTrue(False)
    except:
      DestroyFileStructure(self.mod_in_mod, "test")
      self.assertTrue(True)

class TestParseHppFile(unittest.TestCase):
  simple_module_file = "resources/simple_module.hpp"
  complex_module_file = "resources/complex_module.hpp"

  def test_simple(self):
    simple_module = opalbuilder.OpalModule(
                      path=self.simple_module_file,
                      name="testing::SimpleModule",
                      inputs=[("testing::ProtoFoo", "proto_foo")],
                      outputs=[("testing::ProtoBar", "proto_bar")])
    output = opalbuilder.ModuleFromHeader(self.simple_module_file)
    self.assertEqual(len(output), 1)
    self.assertEqual(output[0], simple_module)

  def test_complex(self):
    modules = set([opalbuilder.OpalModule(
                    path=self.complex_module_file,
                    name="core::complex::ComplexModule",
                    inputs=[("core::complex::ProtoFoo", "proto_foo")],
                    outputs=[("core::complex::ProtoBar", "proto_bar"),
                             ("core::ProtoBar", "proto_foobar")]),
              opalbuilder.OpalModule(
                path=self.complex_module_file,
                name="core::complex::NoInputModule",
                inputs=[],
                outputs=[("core::ProtoBar", "proto_bar")]),
              opalbuilder.OpalModule(
                path=self.complex_module_file,
                name="core::complex::NoOutputModule",
                inputs=[("core::complex::ProtoFoo", "proto_foo"),
                        ("core::complex::ProtoFoo", "proto_foobar")],
                outputs=[]),
              opalbuilder.OpalModule(
                path=self.complex_module_file,
                name="core::complex::NoInputOrOutput",
                inputs=[],
                outputs=[])
              ])
    output = opalbuilder.ModuleFromHeader(self.complex_module_file)
    self.assertEqual(len(output), len(modules))
    self.assertEqual(set(output), modules)

class TestCalculateConnections(unittest.TestCase):
  complex_module_file = "resources/complex_module.hpp"
  def test_simple(self):
    # This should create the graph
    # ModuleA ---(core::complex::ProtoBar)---> ModuleB
    # ModuleB ---(core::complex::ProtoFoo)---> ModuleA
    modules = [opalbuilder.OpalModule(
                    path=self.complex_module_file,
                    name="core::complex::ModuleA",
                    inputs=[("core::complex::ProtoFoo", "proto_foo")],
                    outputs=[("core::complex::ProtoBar", "proto_bar")]),
              opalbuilder.OpalModule(
                path=self.complex_module_file,
                name="core::complex::ModuleB",
                inputs=[("core::complex::ProtoBar", "proto_bar")],
                outputs=[("core::complex::ProtoFoo", "proto_foo")])]
    correct_graph = {
      (modules[0], "core::complex::ProtoBar"): [modules[1]],
      (modules[1], "core::complex::ProtoFoo"): [modules[0]],
    }
    db = TinyDB('test/db.json')
    db.purge()
    graph = opalbuilder.CalculateConnections(modules, tinydb=db)
    db.close()
    self.assertEqual(graph, correct_graph)
    opalbuilder.VisualizeGraph(graph, "test/test_simple.gv")

  def test_complex(self):
    # This should create the graph
    # ModuleA ---(core::complex::ProtoBar)---> ModuleB
    # ModuleA ---(core::complex::ProtoFoo)---> ModuleB, ModuleC
    # ModuleB ---(core::ProtoFoo)---> ModuleC
    # ModuleC ---(core::complex::ProtoBar)---> ModuleB
    modules = [opalbuilder.OpalModule(
                    path=self.complex_module_file,
                    name="core::complex::ModuleA",
                    inputs=[],
                    outputs=[("core::complex::ProtoBar", "proto_bar"),
                             ("core::complex::ProtoFoo", "proto_foo")]),
              opalbuilder.OpalModule(
                path=self.complex_module_file,
                name="core::complex::ModuleB",
                inputs=[("core::complex::ProtoBar", "proto_bar"),
                        ("core::complex::ProtoFoo", "proto_foo")],
                outputs=[("core::ProtoFoo", "proto_foobar")]),
              opalbuilder.OpalModule(
                path=self.complex_module_file,
                name="core::complex::ModuleC",
                inputs=[("core::complex::ProtoFoo", "proto_foobar"),
                        ("core::ProtoFoo", "proto_bar")],
                outputs=[("core::complex::ProtoBar", "proto_foo")]),
              ]
    correct_graph = {
      (modules[0], "core::complex::ProtoBar"): [modules[1]],
      (modules[0], "core::complex::ProtoFoo"): [modules[1], modules[2]],
      (modules[1], "core::ProtoFoo"): [modules[2]],
      (modules[2], "core::complex::ProtoBar"): [modules[1]],
    }
    db = TinyDB('test/db.json')
    db.purge()
    graph = opalbuilder.CalculateConnections(modules, tinydb=db)
    db.close()
    self.assertEqual(graph, correct_graph)
    opalbuilder.VisualizeGraph(graph, "test/test_complex.gv")

if __name__ == '__main__':
  unittest.main()
