/*
// Simple test opal module. Does not compile, just for parsing.
// Author: Sam Rosenstein (samr37l@gmail.com)
// Date: 5/18/17
//
*/
#ifndef OPAL_MODULE_H
#define OPAL_MODULE_H

#include "some/path/to/opal/header.hpp"
#include "some/path/to/a/proto.pb.hpp"
#include "some/path/to/another/proto.pb.hpp"
#include "some/path/to/another/protofoo.pb.hpp"

namespace core {
  namespace complex {

    class ComplexModule : public OpalModule {
     public:
      ComplexModule(const char *name, int dec_hz) : core::Module(name, dec_hz) {};
      InputSharedVariable<core::complex::ProtoFoo> proto_foo;
      OutputSharedVariable<core::complex::ProtoBar> proto_bar;
      OutputSharedVariable<core::ProtoBar> proto_foobar;
     private:
      int SomeFunction(int some_var);
    };

    class NoInputModule : public OpalModule {
     public:
      NoInputModule() : core::Module(10) {};
      OutputSharedVariable<core::ProtoBar> proto_bar;
     private:
      int SomeFunction(int some_var);
    };

    class NoOutputModule : public OpalModule {
     public:
      NoInputModule() : core::Module(10) {};
      InputSharedVariable<core::complex::ProtoFoo> proto_foo;
      InputSharedVariable<core::complex::ProtoFoo> proto_foobar;
     private:
      int SomeFunction(int some_var);
    };

    class NoInputOrOutput : public OpalModule {
     public:
      NoInputModule() : core::Module(10) {};
     private:
      int SomeFunction(int some_var);
    };
  } // namespace complex
} // namespace core

#endif
