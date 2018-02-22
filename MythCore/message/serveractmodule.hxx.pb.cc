// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: serveractmodule.hxx

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "serveractmodule.hxx.pb.h"

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

const ::google::protobuf::Descriptor* CGetServerActRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CGetServerActRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* CGetServerActResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CGetServerActResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* SERVER_ACT_MODULE_MSG_ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_serveractmodule_2ehxx() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_serveractmodule_2ehxx() {
  protobuf_AddDesc_serveractmodule_2ehxx();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "serveractmodule.hxx");
  GOOGLE_CHECK(file != NULL);
  CGetServerActRequest_descriptor_ = file->message_type(0);
  static const int CGetServerActRequest_offsets_[1] = {
  };
  CGetServerActRequest_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CGetServerActRequest_descriptor_,
      CGetServerActRequest::internal_default_instance(),
      CGetServerActRequest_offsets_,
      -1,
      -1,
      -1,
      sizeof(CGetServerActRequest),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetServerActRequest, _internal_metadata_));
  CGetServerActResponse_descriptor_ = file->message_type(1);
  static const int CGetServerActResponse_offsets_[1] = {
  };
  CGetServerActResponse_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CGetServerActResponse_descriptor_,
      CGetServerActResponse::internal_default_instance(),
      CGetServerActResponse_offsets_,
      -1,
      -1,
      -1,
      sizeof(CGetServerActResponse),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetServerActResponse, _internal_metadata_));
  SERVER_ACT_MODULE_MSG_ID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_serveractmodule_2ehxx);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CGetServerActRequest_descriptor_, CGetServerActRequest::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CGetServerActResponse_descriptor_, CGetServerActResponse::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_serveractmodule_2ehxx() {
  CGetServerActRequest_default_instance_.Shutdown();
  delete CGetServerActRequest_reflection_;
  CGetServerActResponse_default_instance_.Shutdown();
  delete CGetServerActResponse_reflection_;
}

void protobuf_InitDefaults_serveractmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  CGetServerActRequest_default_instance_.DefaultConstruct();
  CGetServerActResponse_default_instance_.DefaultConstruct();
  CGetServerActRequest_default_instance_.get_mutable()->InitAsDefaultInstance();
  CGetServerActResponse_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_serveractmodule_2ehxx_once_);
void protobuf_InitDefaults_serveractmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_serveractmodule_2ehxx_once_,
                 &protobuf_InitDefaults_serveractmodule_2ehxx_impl);
}
void protobuf_AddDesc_serveractmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_serveractmodule_2ehxx();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\023serveractmodule.hxx\"\026\n\024CGetServerActRe"
    "quest\"\027\n\025CGetServerActResponse*\203\001\n\030SERVE"
    "R_ACT_MODULE_MSG_ID\022\036\n\032ID_SERVER_ACT_MOD"
    "ULE_ERROR\020\000\022\"\n\035ID_C2S_REQUEST_GET_SERVER"
    "_ACT\020\2008\022#\n\036ID_S2C_RESPONSE_GET_SERVER_AC"
    "T\020\2018b\006proto3", 212);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "serveractmodule.hxx", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_serveractmodule_2ehxx);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_serveractmodule_2ehxx_once_);
void protobuf_AddDesc_serveractmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_serveractmodule_2ehxx_once_,
                 &protobuf_AddDesc_serveractmodule_2ehxx_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_serveractmodule_2ehxx {
  StaticDescriptorInitializer_serveractmodule_2ehxx() {
    protobuf_AddDesc_serveractmodule_2ehxx();
  }
} static_descriptor_initializer_serveractmodule_2ehxx_;
const ::google::protobuf::EnumDescriptor* SERVER_ACT_MODULE_MSG_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SERVER_ACT_MODULE_MSG_ID_descriptor_;
}
bool SERVER_ACT_MODULE_MSG_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 7168:
    case 7169:
      return true;
    default:
      return false;
  }
}


namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD GOOGLE_ATTRIBUTE_NORETURN;
static void MergeFromFail(int line) {
  ::google::protobuf::internal::MergeFromFail(__FILE__, line);
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CGetServerActRequest::CGetServerActRequest()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_serveractmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CGetServerActRequest)
}

void CGetServerActRequest::InitAsDefaultInstance() {
}

CGetServerActRequest::CGetServerActRequest(const CGetServerActRequest& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CGetServerActRequest)
}

void CGetServerActRequest::SharedCtor() {
  _cached_size_ = 0;
}

CGetServerActRequest::~CGetServerActRequest() {
  // @@protoc_insertion_point(destructor:CGetServerActRequest)
  SharedDtor();
}

void CGetServerActRequest::SharedDtor() {
}

void CGetServerActRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CGetServerActRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CGetServerActRequest_descriptor_;
}

const CGetServerActRequest& CGetServerActRequest::default_instance() {
  protobuf_InitDefaults_serveractmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CGetServerActRequest> CGetServerActRequest_default_instance_;

CGetServerActRequest* CGetServerActRequest::New(::google::protobuf::Arena* arena) const {
  CGetServerActRequest* n = new CGetServerActRequest;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CGetServerActRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:CGetServerActRequest)
}

bool CGetServerActRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CGetServerActRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0 ||
        ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
  }
success:
  // @@protoc_insertion_point(parse_success:CGetServerActRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CGetServerActRequest)
  return false;
#undef DO_
}

void CGetServerActRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CGetServerActRequest)
  // @@protoc_insertion_point(serialize_end:CGetServerActRequest)
}

::google::protobuf::uint8* CGetServerActRequest::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CGetServerActRequest)
  // @@protoc_insertion_point(serialize_to_array_end:CGetServerActRequest)
  return target;
}

size_t CGetServerActRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CGetServerActRequest)
  size_t total_size = 0;

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CGetServerActRequest::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CGetServerActRequest)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CGetServerActRequest* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CGetServerActRequest>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CGetServerActRequest)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CGetServerActRequest)
    UnsafeMergeFrom(*source);
  }
}

void CGetServerActRequest::MergeFrom(const CGetServerActRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CGetServerActRequest)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CGetServerActRequest::UnsafeMergeFrom(const CGetServerActRequest& from) {
  GOOGLE_DCHECK(&from != this);
}

void CGetServerActRequest::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CGetServerActRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CGetServerActRequest::CopyFrom(const CGetServerActRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CGetServerActRequest)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CGetServerActRequest::IsInitialized() const {

  return true;
}

void CGetServerActRequest::Swap(CGetServerActRequest* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CGetServerActRequest::InternalSwap(CGetServerActRequest* other) {
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CGetServerActRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CGetServerActRequest_descriptor_;
  metadata.reflection = CGetServerActRequest_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CGetServerActRequest

inline const CGetServerActRequest* CGetServerActRequest::internal_default_instance() {
  return &CGetServerActRequest_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CGetServerActResponse::CGetServerActResponse()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_serveractmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CGetServerActResponse)
}

void CGetServerActResponse::InitAsDefaultInstance() {
}

CGetServerActResponse::CGetServerActResponse(const CGetServerActResponse& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CGetServerActResponse)
}

void CGetServerActResponse::SharedCtor() {
  _cached_size_ = 0;
}

CGetServerActResponse::~CGetServerActResponse() {
  // @@protoc_insertion_point(destructor:CGetServerActResponse)
  SharedDtor();
}

void CGetServerActResponse::SharedDtor() {
}

void CGetServerActResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CGetServerActResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CGetServerActResponse_descriptor_;
}

const CGetServerActResponse& CGetServerActResponse::default_instance() {
  protobuf_InitDefaults_serveractmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CGetServerActResponse> CGetServerActResponse_default_instance_;

CGetServerActResponse* CGetServerActResponse::New(::google::protobuf::Arena* arena) const {
  CGetServerActResponse* n = new CGetServerActResponse;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CGetServerActResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:CGetServerActResponse)
}

bool CGetServerActResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CGetServerActResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0 ||
        ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
  }
success:
  // @@protoc_insertion_point(parse_success:CGetServerActResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CGetServerActResponse)
  return false;
#undef DO_
}

void CGetServerActResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CGetServerActResponse)
  // @@protoc_insertion_point(serialize_end:CGetServerActResponse)
}

::google::protobuf::uint8* CGetServerActResponse::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CGetServerActResponse)
  // @@protoc_insertion_point(serialize_to_array_end:CGetServerActResponse)
  return target;
}

size_t CGetServerActResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CGetServerActResponse)
  size_t total_size = 0;

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CGetServerActResponse::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CGetServerActResponse)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CGetServerActResponse* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CGetServerActResponse>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CGetServerActResponse)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CGetServerActResponse)
    UnsafeMergeFrom(*source);
  }
}

void CGetServerActResponse::MergeFrom(const CGetServerActResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CGetServerActResponse)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CGetServerActResponse::UnsafeMergeFrom(const CGetServerActResponse& from) {
  GOOGLE_DCHECK(&from != this);
}

void CGetServerActResponse::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CGetServerActResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CGetServerActResponse::CopyFrom(const CGetServerActResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CGetServerActResponse)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CGetServerActResponse::IsInitialized() const {

  return true;
}

void CGetServerActResponse::Swap(CGetServerActResponse* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CGetServerActResponse::InternalSwap(CGetServerActResponse* other) {
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CGetServerActResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CGetServerActResponse_descriptor_;
  metadata.reflection = CGetServerActResponse_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CGetServerActResponse

inline const CGetServerActResponse* CGetServerActResponse::internal_default_instance() {
  return &CGetServerActResponse_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)