// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.hxx

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "message.hxx.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::EnumDescriptor* MESSAGE_MODULE_ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_message_2ehxx() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_message_2ehxx() {
  protobuf_AddDesc_message_2ehxx();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "message.hxx");
  GOOGLE_CHECK(file != NULL);
  MESSAGE_MODULE_ID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_message_2ehxx);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_message_2ehxx() {
}

void protobuf_InitDefaults_message_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_message_2ehxx_once_);
void protobuf_InitDefaults_message_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_message_2ehxx_once_,
                 &protobuf_InitDefaults_message_2ehxx_impl);
}
void protobuf_AddDesc_message_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_message_2ehxx();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013message.hxx*\263\001\n\021MESSAGE_MODULE_ID\022\030\n\024M"
    "ESSAGE_MODULE_LOGIN\020\000\022\034\n\027MESSAGE_MODULE_"
    "PROPERTY\020\200\010\022\030\n\023MESSAGE_MODULE_ITEM\020\200\020\022\027\n"
    "\022MESSAGE_MODULE_MAP\020\200\030\022\030\n\023MESSAGE_MODULE"
    "_TASK\020\200 \022\031\n\024MESSAGE_MODULE_SKILL\020\200(b\006pro"
    "to3", 203);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "message.hxx", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_message_2ehxx);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_message_2ehxx_once_);
void protobuf_AddDesc_message_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_message_2ehxx_once_,
                 &protobuf_AddDesc_message_2ehxx_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_message_2ehxx {
  StaticDescriptorInitializer_message_2ehxx() {
    protobuf_AddDesc_message_2ehxx();
  }
} static_descriptor_initializer_message_2ehxx_;
const ::google::protobuf::EnumDescriptor* MESSAGE_MODULE_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MESSAGE_MODULE_ID_descriptor_;
}
bool MESSAGE_MODULE_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 1024:
    case 2048:
    case 3072:
    case 4096:
    case 5120:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
