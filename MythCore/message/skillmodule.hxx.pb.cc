// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: skillmodule.hxx

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "skillmodule.hxx.pb.h"

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

const ::google::protobuf::Descriptor* CUseSkillRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CUseSkillRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* CUseSkillResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CUseSkillResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* SKILL_MODULE_MSG_ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_skillmodule_2ehxx() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_skillmodule_2ehxx() {
  protobuf_AddDesc_skillmodule_2ehxx();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "skillmodule.hxx");
  GOOGLE_CHECK(file != NULL);
  CUseSkillRequest_descriptor_ = file->message_type(0);
  static const int CUseSkillRequest_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CUseSkillRequest, skillindex_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CUseSkillRequest, target_),
  };
  CUseSkillRequest_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CUseSkillRequest_descriptor_,
      CUseSkillRequest::internal_default_instance(),
      CUseSkillRequest_offsets_,
      -1,
      -1,
      -1,
      sizeof(CUseSkillRequest),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CUseSkillRequest, _internal_metadata_));
  CUseSkillResponse_descriptor_ = file->message_type(1);
  static const int CUseSkillResponse_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CUseSkillResponse, result_),
  };
  CUseSkillResponse_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CUseSkillResponse_descriptor_,
      CUseSkillResponse::internal_default_instance(),
      CUseSkillResponse_offsets_,
      -1,
      -1,
      -1,
      sizeof(CUseSkillResponse),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CUseSkillResponse, _internal_metadata_));
  SKILL_MODULE_MSG_ID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_skillmodule_2ehxx);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CUseSkillRequest_descriptor_, CUseSkillRequest::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CUseSkillResponse_descriptor_, CUseSkillResponse::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_skillmodule_2ehxx() {
  CUseSkillRequest_default_instance_.Shutdown();
  delete CUseSkillRequest_reflection_;
  CUseSkillResponse_default_instance_.Shutdown();
  delete CUseSkillResponse_reflection_;
}

void protobuf_InitDefaults_skillmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  CUseSkillRequest_default_instance_.DefaultConstruct();
  CUseSkillResponse_default_instance_.DefaultConstruct();
  CUseSkillRequest_default_instance_.get_mutable()->InitAsDefaultInstance();
  CUseSkillResponse_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_skillmodule_2ehxx_once_);
void protobuf_InitDefaults_skillmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_skillmodule_2ehxx_once_,
                 &protobuf_InitDefaults_skillmodule_2ehxx_impl);
}
void protobuf_AddDesc_skillmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_skillmodule_2ehxx();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017skillmodule.hxx\"6\n\020CUseSkillRequest\022\022\n"
    "\nSkillIndex\030\001 \001(\r\022\016\n\006Target\030\002 \003(\r\"#\n\021CUs"
    "eSkillResponse\022\016\n\006Result\030\001 \001(\r*o\n\023SKILL_"
    "MODULE_MSG_ID\022\031\n\025ID_SKILL_MODULE_ERROR\020\000"
    "\022\035\n\030ID_C2S_REQUEST_USE_SKILL\020\200(\022\036\n\031ID_S2"
    "C_RESPONSE_USE_SKILL\020\201(b\006proto3", 231);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "skillmodule.hxx", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_skillmodule_2ehxx);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_skillmodule_2ehxx_once_);
void protobuf_AddDesc_skillmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_skillmodule_2ehxx_once_,
                 &protobuf_AddDesc_skillmodule_2ehxx_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_skillmodule_2ehxx {
  StaticDescriptorInitializer_skillmodule_2ehxx() {
    protobuf_AddDesc_skillmodule_2ehxx();
  }
} static_descriptor_initializer_skillmodule_2ehxx_;
const ::google::protobuf::EnumDescriptor* SKILL_MODULE_MSG_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SKILL_MODULE_MSG_ID_descriptor_;
}
bool SKILL_MODULE_MSG_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 5120:
    case 5121:
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
const int CUseSkillRequest::kSkillIndexFieldNumber;
const int CUseSkillRequest::kTargetFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CUseSkillRequest::CUseSkillRequest()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_skillmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CUseSkillRequest)
}

void CUseSkillRequest::InitAsDefaultInstance() {
}

CUseSkillRequest::CUseSkillRequest(const CUseSkillRequest& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CUseSkillRequest)
}

void CUseSkillRequest::SharedCtor() {
  skillindex_ = 0u;
  _cached_size_ = 0;
}

CUseSkillRequest::~CUseSkillRequest() {
  // @@protoc_insertion_point(destructor:CUseSkillRequest)
  SharedDtor();
}

void CUseSkillRequest::SharedDtor() {
}

void CUseSkillRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CUseSkillRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CUseSkillRequest_descriptor_;
}

const CUseSkillRequest& CUseSkillRequest::default_instance() {
  protobuf_InitDefaults_skillmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CUseSkillRequest> CUseSkillRequest_default_instance_;

CUseSkillRequest* CUseSkillRequest::New(::google::protobuf::Arena* arena) const {
  CUseSkillRequest* n = new CUseSkillRequest;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CUseSkillRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:CUseSkillRequest)
  skillindex_ = 0u;
  target_.Clear();
}

bool CUseSkillRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CUseSkillRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 SkillIndex = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &skillindex_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_Target;
        break;
      }

      // repeated uint32 Target = 2;
      case 2: {
        if (tag == 18) {
         parse_Target:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, this->mutable_target())));
        } else if (tag == 16) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 1, 18, input, this->mutable_target())));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:CUseSkillRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CUseSkillRequest)
  return false;
#undef DO_
}

void CUseSkillRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CUseSkillRequest)
  // optional uint32 SkillIndex = 1;
  if (this->skillindex() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->skillindex(), output);
  }

  // repeated uint32 Target = 2;
  if (this->target_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(2, ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(_target_cached_byte_size_);
  }
  for (int i = 0; i < this->target_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32NoTag(
      this->target(i), output);
  }

  // @@protoc_insertion_point(serialize_end:CUseSkillRequest)
}

::google::protobuf::uint8* CUseSkillRequest::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CUseSkillRequest)
  // optional uint32 SkillIndex = 1;
  if (this->skillindex() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->skillindex(), target);
  }

  // repeated uint32 Target = 2;
  if (this->target_size() > 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(
      2,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
      _target_cached_byte_size_, target);
  }
  for (int i = 0; i < this->target_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteUInt32NoTagToArray(this->target(i), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:CUseSkillRequest)
  return target;
}

size_t CUseSkillRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CUseSkillRequest)
  size_t total_size = 0;

  // optional uint32 SkillIndex = 1;
  if (this->skillindex() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->skillindex());
  }

  // repeated uint32 Target = 2;
  {
    size_t data_size = 0;
    unsigned int count = this->target_size();
    for (unsigned int i = 0; i < count; i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::
        UInt32Size(this->target(i));
    }
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(data_size);
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
    _target_cached_byte_size_ = cached_size;
    GOOGLE_SAFE_CONCURRENT_WRITES_END();
    total_size += data_size;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CUseSkillRequest::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CUseSkillRequest)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CUseSkillRequest* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CUseSkillRequest>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CUseSkillRequest)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CUseSkillRequest)
    UnsafeMergeFrom(*source);
  }
}

void CUseSkillRequest::MergeFrom(const CUseSkillRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CUseSkillRequest)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CUseSkillRequest::UnsafeMergeFrom(const CUseSkillRequest& from) {
  GOOGLE_DCHECK(&from != this);
  target_.UnsafeMergeFrom(from.target_);
  if (from.skillindex() != 0) {
    set_skillindex(from.skillindex());
  }
}

void CUseSkillRequest::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CUseSkillRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CUseSkillRequest::CopyFrom(const CUseSkillRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CUseSkillRequest)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CUseSkillRequest::IsInitialized() const {

  return true;
}

void CUseSkillRequest::Swap(CUseSkillRequest* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CUseSkillRequest::InternalSwap(CUseSkillRequest* other) {
  std::swap(skillindex_, other->skillindex_);
  target_.UnsafeArenaSwap(&other->target_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CUseSkillRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CUseSkillRequest_descriptor_;
  metadata.reflection = CUseSkillRequest_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CUseSkillRequest

// optional uint32 SkillIndex = 1;
void CUseSkillRequest::clear_skillindex() {
  skillindex_ = 0u;
}
::google::protobuf::uint32 CUseSkillRequest::skillindex() const {
  // @@protoc_insertion_point(field_get:CUseSkillRequest.SkillIndex)
  return skillindex_;
}
void CUseSkillRequest::set_skillindex(::google::protobuf::uint32 value) {
  
  skillindex_ = value;
  // @@protoc_insertion_point(field_set:CUseSkillRequest.SkillIndex)
}

// repeated uint32 Target = 2;
int CUseSkillRequest::target_size() const {
  return target_.size();
}
void CUseSkillRequest::clear_target() {
  target_.Clear();
}
::google::protobuf::uint32 CUseSkillRequest::target(int index) const {
  // @@protoc_insertion_point(field_get:CUseSkillRequest.Target)
  return target_.Get(index);
}
void CUseSkillRequest::set_target(int index, ::google::protobuf::uint32 value) {
  target_.Set(index, value);
  // @@protoc_insertion_point(field_set:CUseSkillRequest.Target)
}
void CUseSkillRequest::add_target(::google::protobuf::uint32 value) {
  target_.Add(value);
  // @@protoc_insertion_point(field_add:CUseSkillRequest.Target)
}
const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
CUseSkillRequest::target() const {
  // @@protoc_insertion_point(field_list:CUseSkillRequest.Target)
  return target_;
}
::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
CUseSkillRequest::mutable_target() {
  // @@protoc_insertion_point(field_mutable_list:CUseSkillRequest.Target)
  return &target_;
}

inline const CUseSkillRequest* CUseSkillRequest::internal_default_instance() {
  return &CUseSkillRequest_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int CUseSkillResponse::kResultFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CUseSkillResponse::CUseSkillResponse()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_skillmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CUseSkillResponse)
}

void CUseSkillResponse::InitAsDefaultInstance() {
}

CUseSkillResponse::CUseSkillResponse(const CUseSkillResponse& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CUseSkillResponse)
}

void CUseSkillResponse::SharedCtor() {
  result_ = 0u;
  _cached_size_ = 0;
}

CUseSkillResponse::~CUseSkillResponse() {
  // @@protoc_insertion_point(destructor:CUseSkillResponse)
  SharedDtor();
}

void CUseSkillResponse::SharedDtor() {
}

void CUseSkillResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CUseSkillResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CUseSkillResponse_descriptor_;
}

const CUseSkillResponse& CUseSkillResponse::default_instance() {
  protobuf_InitDefaults_skillmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CUseSkillResponse> CUseSkillResponse_default_instance_;

CUseSkillResponse* CUseSkillResponse::New(::google::protobuf::Arena* arena) const {
  CUseSkillResponse* n = new CUseSkillResponse;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CUseSkillResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:CUseSkillResponse)
  result_ = 0u;
}

bool CUseSkillResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CUseSkillResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 Result = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &result_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:CUseSkillResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CUseSkillResponse)
  return false;
#undef DO_
}

void CUseSkillResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CUseSkillResponse)
  // optional uint32 Result = 1;
  if (this->result() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->result(), output);
  }

  // @@protoc_insertion_point(serialize_end:CUseSkillResponse)
}

::google::protobuf::uint8* CUseSkillResponse::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CUseSkillResponse)
  // optional uint32 Result = 1;
  if (this->result() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->result(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:CUseSkillResponse)
  return target;
}

size_t CUseSkillResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CUseSkillResponse)
  size_t total_size = 0;

  // optional uint32 Result = 1;
  if (this->result() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->result());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CUseSkillResponse::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CUseSkillResponse)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CUseSkillResponse* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CUseSkillResponse>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CUseSkillResponse)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CUseSkillResponse)
    UnsafeMergeFrom(*source);
  }
}

void CUseSkillResponse::MergeFrom(const CUseSkillResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CUseSkillResponse)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CUseSkillResponse::UnsafeMergeFrom(const CUseSkillResponse& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.result() != 0) {
    set_result(from.result());
  }
}

void CUseSkillResponse::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CUseSkillResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CUseSkillResponse::CopyFrom(const CUseSkillResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CUseSkillResponse)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CUseSkillResponse::IsInitialized() const {

  return true;
}

void CUseSkillResponse::Swap(CUseSkillResponse* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CUseSkillResponse::InternalSwap(CUseSkillResponse* other) {
  std::swap(result_, other->result_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CUseSkillResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CUseSkillResponse_descriptor_;
  metadata.reflection = CUseSkillResponse_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CUseSkillResponse

// optional uint32 Result = 1;
void CUseSkillResponse::clear_result() {
  result_ = 0u;
}
::google::protobuf::uint32 CUseSkillResponse::result() const {
  // @@protoc_insertion_point(field_get:CUseSkillResponse.Result)
  return result_;
}
void CUseSkillResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CUseSkillResponse.Result)
}

inline const CUseSkillResponse* CUseSkillResponse::internal_default_instance() {
  return &CUseSkillResponse_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
