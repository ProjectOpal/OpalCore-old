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

namespace testing {

  class SimpleModule : public core::OpalModule {
   public:
    SimpleModule() : core::Module(10) {};
    core::InputSharedVariable<testing::ProtoFoo> proto_foo;
    core::OutputSharedVariable<testing::ProtoBar> proto_bar;
   private:
    int SomeFunction(int some_var);
  };

} // namespace testing]

#endif
