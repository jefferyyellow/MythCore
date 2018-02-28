// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rankmodule.hxx

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "rankmodule.hxx.pb.h"

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

const ::google::protobuf::Descriptor* CGetRankInfoRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CGetRankInfoRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* PBRankRoleInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PBRankRoleInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* CGetRankInfoResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CGetRankInfoResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* RANK_MODULE_MSG_ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_rankmodule_2ehxx() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_rankmodule_2ehxx() {
  protobuf_AddDesc_rankmodule_2ehxx();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "rankmodule.hxx");
  GOOGLE_CHECK(file != NULL);
  CGetRankInfoRequest_descriptor_ = file->message_type(0);
  static const int CGetRankInfoRequest_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetRankInfoRequest, ranktype_),
  };
  CGetRankInfoRequest_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CGetRankInfoRequest_descriptor_,
      CGetRankInfoRequest::internal_default_instance(),
      CGetRankInfoRequest_offsets_,
      -1,
      -1,
      -1,
      sizeof(CGetRankInfoRequest),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetRankInfoRequest, _internal_metadata_));
  PBRankRoleInfo_descriptor_ = file->message_type(1);
  static const int PBRankRoleInfo_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PBRankRoleInfo, roleid_),
  };
  PBRankRoleInfo_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      PBRankRoleInfo_descriptor_,
      PBRankRoleInfo::internal_default_instance(),
      PBRankRoleInfo_offsets_,
      -1,
      -1,
      -1,
      sizeof(PBRankRoleInfo),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PBRankRoleInfo, _internal_metadata_));
  CGetRankInfoResponse_descriptor_ = file->message_type(2);
  static const int CGetRankInfoResponse_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetRankInfoResponse, ranktype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetRankInfoResponse, roleinfo_),
  };
  CGetRankInfoResponse_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CGetRankInfoResponse_descriptor_,
      CGetRankInfoResponse::internal_default_instance(),
      CGetRankInfoResponse_offsets_,
      -1,
      -1,
      -1,
      sizeof(CGetRankInfoResponse),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CGetRankInfoResponse, _internal_metadata_));
  RANK_MODULE_MSG_ID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_rankmodule_2ehxx);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CGetRankInfoRequest_descriptor_, CGetRankInfoRequest::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      PBRankRoleInfo_descriptor_, PBRankRoleInfo::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CGetRankInfoResponse_descriptor_, CGetRankInfoResponse::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_rankmodule_2ehxx() {
  CGetRankInfoRequest_default_instance_.Shutdown();
  delete CGetRankInfoRequest_reflection_;
  PBRankRoleInfo_default_instance_.Shutdown();
  delete PBRankRoleInfo_reflection_;
  CGetRankInfoResponse_default_instance_.Shutdown();
  delete CGetRankInfoResponse_reflection_;
}

void protobuf_InitDefaults_rankmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  CGetRankInfoRequest_default_instance_.DefaultConstruct();
  PBRankRoleInfo_default_instance_.DefaultConstruct();
  CGetRankInfoResponse_default_instance_.DefaultConstruct();
  CGetRankInfoRequest_default_instance_.get_mutable()->InitAsDefaultInstance();
  PBRankRoleInfo_default_instance_.get_mutable()->InitAsDefaultInstance();
  CGetRankInfoResponse_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_rankmodule_2ehxx_once_);
void protobuf_InitDefaults_rankmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_rankmodule_2ehxx_once_,
                 &protobuf_InitDefaults_rankmodule_2ehxx_impl);
}
void protobuf_AddDesc_rankmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_rankmodule_2ehxx();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016rankmodule.hxx\"\'\n\023CGetRankInfoRequest\022"
    "\020\n\010RankType\030\001 \001(\r\" \n\016PBRankRoleInfo\022\016\n\006R"
    "oleID\030\001 \001(\r\"K\n\024CGetRankInfoResponse\022\020\n\010R"
    "ankType\030\001 \001(\r\022!\n\010RoleInfo\030\002 \003(\0132\017.PBRank"
    "RoleInfo*u\n\022RANK_MODULE_MSG_ID\022\030\n\024ID_RAN"
    "K_MODULE_ERROR\020\000\022!\n\034ID_C2S_REQUEST_GET_R"
    "ANK_INFO\020\200H\022\"\n\035ID_S2C_RESPONSE_GET_RANK_"
    "INFO\020\201Hb\006proto3", 295);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "rankmodule.hxx", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_rankmodule_2ehxx);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_rankmodule_2ehxx_once_);
void protobuf_AddDesc_rankmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_rankmodule_2ehxx_once_,
                 &protobuf_AddDesc_rankmodule_2ehxx_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_rankmodule_2ehxx {
  StaticDescriptorInitializer_rankmodule_2ehxx() {
    protobuf_AddDesc_rankmodule_2ehxx();
  }
} static_descriptor_initializer_rankmodule_2ehxx_;
const ::google::protobuf::EnumDescriptor* RANK_MODULE_MSG_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RANK_MODULE_MSG_ID_descriptor_;
}
bool RANK_MODULE_MSG_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 9216:
    case 9217:
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
const int CGetRankInfoRequest::kRankTypeFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CGetRankInfoRequest::CGetRankInfoRequest()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_rankmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CGetRankInfoRequest)
}

void CGetRankInfoRequest::InitAsDefaultInstance() {
}

CGetRankInfoRequest::CGetRankInfoRequest(const CGetRankInfoRequest& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CGetRankInfoRequest)
}

void CGetRankInfoRequest::SharedCtor() {
  ranktype_ = 0u;
  _cached_size_ = 0;
}

CGetRankInfoRequest::~CGetRankInfoRequest() {
  // @@protoc_insertion_point(destructor:CGetRankInfoRequest)
  SharedDtor();
}

void CGetRankInfoRequest::SharedDtor() {
}

void CGetRankInfoRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CGetRankInfoRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CGetRankInfoRequest_descriptor_;
}

const CGetRankInfoRequest& CGetRankInfoRequest::default_instance() {
  protobuf_InitDefaults_rankmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CGetRankInfoRequest> CGetRankInfoRequest_default_instance_;

CGetRankInfoRequest* CGetRankInfoRequest::New(::google::protobuf::Arena* arena) const {
  CGetRankInfoRequest* n = new CGetRankInfoRequest;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CGetRankInfoRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:CGetRankInfoRequest)
  ranktype_ = 0u;
}

bool CGetRankInfoRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CGetRankInfoRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 RankType = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &ranktype_)));
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
  // @@protoc_insertion_point(parse_success:CGetRankInfoRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CGetRankInfoRequest)
  return false;
#undef DO_
}

void CGetRankInfoRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CGetRankInfoRequest)
  // optional uint32 RankType = 1;
  if (this->ranktype() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->ranktype(), output);
  }

  // @@protoc_insertion_point(serialize_end:CGetRankInfoRequest)
}

::google::protobuf::uint8* CGetRankInfoRequest::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CGetRankInfoRequest)
  // optional uint32 RankType = 1;
  if (this->ranktype() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->ranktype(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:CGetRankInfoRequest)
  return target;
}

size_t CGetRankInfoRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CGetRankInfoRequest)
  size_t total_size = 0;

  // optional uint32 RankType = 1;
  if (this->ranktype() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->ranktype());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CGetRankInfoRequest::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CGetRankInfoRequest)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CGetRankInfoRequest* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CGetRankInfoRequest>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CGetRankInfoRequest)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CGetRankInfoRequest)
    UnsafeMergeFrom(*source);
  }
}

void CGetRankInfoRequest::MergeFrom(const CGetRankInfoRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CGetRankInfoRequest)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CGetRankInfoRequest::UnsafeMergeFrom(const CGetRankInfoRequest& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.ranktype() != 0) {
    set_ranktype(from.ranktype());
  }
}

void CGetRankInfoRequest::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CGetRankInfoRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CGetRankInfoRequest::CopyFrom(const CGetRankInfoRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CGetRankInfoRequest)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CGetRankInfoRequest::IsInitialized() const {

  return true;
}

void CGetRankInfoRequest::Swap(CGetRankInfoRequest* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CGetRankInfoRequest::InternalSwap(CGetRankInfoRequest* other) {
  std::swap(ranktype_, other->ranktype_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CGetRankInfoRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CGetRankInfoRequest_descriptor_;
  metadata.reflection = CGetRankInfoRequest_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CGetRankInfoRequest

// optional uint32 RankType = 1;
void CGetRankInfoRequest::clear_ranktype() {
  ranktype_ = 0u;
}
::google::protobuf::uint32 CGetRankInfoRequest::ranktype() const {
  // @@protoc_insertion_point(field_get:CGetRankInfoRequest.RankType)
  return ranktype_;
}
void CGetRankInfoRequest::set_ranktype(::google::protobuf::uint32 value) {
  
  ranktype_ = value;
  // @@protoc_insertion_point(field_set:CGetRankInfoRequest.RankType)
}

inline const CGetRankInfoRequest* CGetRankInfoRequest::internal_default_instance() {
  return &CGetRankInfoRequest_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int PBRankRoleInfo::kRoleIDFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

PBRankRoleInfo::PBRankRoleInfo()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_rankmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:PBRankRoleInfo)
}

void PBRankRoleInfo::InitAsDefaultInstance() {
}

PBRankRoleInfo::PBRankRoleInfo(const PBRankRoleInfo& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:PBRankRoleInfo)
}

void PBRankRoleInfo::SharedCtor() {
  roleid_ = 0u;
  _cached_size_ = 0;
}

PBRankRoleInfo::~PBRankRoleInfo() {
  // @@protoc_insertion_point(destructor:PBRankRoleInfo)
  SharedDtor();
}

void PBRankRoleInfo::SharedDtor() {
}

void PBRankRoleInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PBRankRoleInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PBRankRoleInfo_descriptor_;
}

const PBRankRoleInfo& PBRankRoleInfo::default_instance() {
  protobuf_InitDefaults_rankmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<PBRankRoleInfo> PBRankRoleInfo_default_instance_;

PBRankRoleInfo* PBRankRoleInfo::New(::google::protobuf::Arena* arena) const {
  PBRankRoleInfo* n = new PBRankRoleInfo;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void PBRankRoleInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:PBRankRoleInfo)
  roleid_ = 0u;
}

bool PBRankRoleInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:PBRankRoleInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 RoleID = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &roleid_)));
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
  // @@protoc_insertion_point(parse_success:PBRankRoleInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:PBRankRoleInfo)
  return false;
#undef DO_
}

void PBRankRoleInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:PBRankRoleInfo)
  // optional uint32 RoleID = 1;
  if (this->roleid() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->roleid(), output);
  }

  // @@protoc_insertion_point(serialize_end:PBRankRoleInfo)
}

::google::protobuf::uint8* PBRankRoleInfo::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:PBRankRoleInfo)
  // optional uint32 RoleID = 1;
  if (this->roleid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->roleid(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:PBRankRoleInfo)
  return target;
}

size_t PBRankRoleInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:PBRankRoleInfo)
  size_t total_size = 0;

  // optional uint32 RoleID = 1;
  if (this->roleid() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->roleid());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PBRankRoleInfo::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:PBRankRoleInfo)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const PBRankRoleInfo* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const PBRankRoleInfo>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:PBRankRoleInfo)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:PBRankRoleInfo)
    UnsafeMergeFrom(*source);
  }
}

void PBRankRoleInfo::MergeFrom(const PBRankRoleInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:PBRankRoleInfo)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void PBRankRoleInfo::UnsafeMergeFrom(const PBRankRoleInfo& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.roleid() != 0) {
    set_roleid(from.roleid());
  }
}

void PBRankRoleInfo::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:PBRankRoleInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PBRankRoleInfo::CopyFrom(const PBRankRoleInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:PBRankRoleInfo)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool PBRankRoleInfo::IsInitialized() const {

  return true;
}

void PBRankRoleInfo::Swap(PBRankRoleInfo* other) {
  if (other == this) return;
  InternalSwap(other);
}
void PBRankRoleInfo::InternalSwap(PBRankRoleInfo* other) {
  std::swap(roleid_, other->roleid_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata PBRankRoleInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PBRankRoleInfo_descriptor_;
  metadata.reflection = PBRankRoleInfo_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// PBRankRoleInfo

// optional uint32 RoleID = 1;
void PBRankRoleInfo::clear_roleid() {
  roleid_ = 0u;
}
::google::protobuf::uint32 PBRankRoleInfo::roleid() const {
  // @@protoc_insertion_point(field_get:PBRankRoleInfo.RoleID)
  return roleid_;
}
void PBRankRoleInfo::set_roleid(::google::protobuf::uint32 value) {
  
  roleid_ = value;
  // @@protoc_insertion_point(field_set:PBRankRoleInfo.RoleID)
}

inline const PBRankRoleInfo* PBRankRoleInfo::internal_default_instance() {
  return &PBRankRoleInfo_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int CGetRankInfoResponse::kRankTypeFieldNumber;
const int CGetRankInfoResponse::kRoleInfoFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CGetRankInfoResponse::CGetRankInfoResponse()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_rankmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CGetRankInfoResponse)
}

void CGetRankInfoResponse::InitAsDefaultInstance() {
}

CGetRankInfoResponse::CGetRankInfoResponse(const CGetRankInfoResponse& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CGetRankInfoResponse)
}

void CGetRankInfoResponse::SharedCtor() {
  ranktype_ = 0u;
  _cached_size_ = 0;
}

CGetRankInfoResponse::~CGetRankInfoResponse() {
  // @@protoc_insertion_point(destructor:CGetRankInfoResponse)
  SharedDtor();
}

void CGetRankInfoResponse::SharedDtor() {
}

void CGetRankInfoResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CGetRankInfoResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CGetRankInfoResponse_descriptor_;
}

const CGetRankInfoResponse& CGetRankInfoResponse::default_instance() {
  protobuf_InitDefaults_rankmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CGetRankInfoResponse> CGetRankInfoResponse_default_instance_;

CGetRankInfoResponse* CGetRankInfoResponse::New(::google::protobuf::Arena* arena) const {
  CGetRankInfoResponse* n = new CGetRankInfoResponse;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CGetRankInfoResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:CGetRankInfoResponse)
  ranktype_ = 0u;
  roleinfo_.Clear();
}

bool CGetRankInfoResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CGetRankInfoResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 RankType = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &ranktype_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_RoleInfo;
        break;
      }

      // repeated .PBRankRoleInfo RoleInfo = 2;
      case 2: {
        if (tag == 18) {
         parse_RoleInfo:
          DO_(input->IncrementRecursionDepth());
         parse_loop_RoleInfo:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtualNoRecursionDepth(
                input, add_roleinfo()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_loop_RoleInfo;
        input->UnsafeDecrementRecursionDepth();
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
  // @@protoc_insertion_point(parse_success:CGetRankInfoResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CGetRankInfoResponse)
  return false;
#undef DO_
}

void CGetRankInfoResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CGetRankInfoResponse)
  // optional uint32 RankType = 1;
  if (this->ranktype() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->ranktype(), output);
  }

  // repeated .PBRankRoleInfo RoleInfo = 2;
  for (unsigned int i = 0, n = this->roleinfo_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->roleinfo(i), output);
  }

  // @@protoc_insertion_point(serialize_end:CGetRankInfoResponse)
}

::google::protobuf::uint8* CGetRankInfoResponse::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CGetRankInfoResponse)
  // optional uint32 RankType = 1;
  if (this->ranktype() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->ranktype(), target);
  }

  // repeated .PBRankRoleInfo RoleInfo = 2;
  for (unsigned int i = 0, n = this->roleinfo_size(); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        2, this->roleinfo(i), false, target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:CGetRankInfoResponse)
  return target;
}

size_t CGetRankInfoResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CGetRankInfoResponse)
  size_t total_size = 0;

  // optional uint32 RankType = 1;
  if (this->ranktype() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->ranktype());
  }

  // repeated .PBRankRoleInfo RoleInfo = 2;
  {
    unsigned int count = this->roleinfo_size();
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->roleinfo(i));
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CGetRankInfoResponse::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CGetRankInfoResponse)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CGetRankInfoResponse* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CGetRankInfoResponse>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CGetRankInfoResponse)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CGetRankInfoResponse)
    UnsafeMergeFrom(*source);
  }
}

void CGetRankInfoResponse::MergeFrom(const CGetRankInfoResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CGetRankInfoResponse)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CGetRankInfoResponse::UnsafeMergeFrom(const CGetRankInfoResponse& from) {
  GOOGLE_DCHECK(&from != this);
  roleinfo_.MergeFrom(from.roleinfo_);
  if (from.ranktype() != 0) {
    set_ranktype(from.ranktype());
  }
}

void CGetRankInfoResponse::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CGetRankInfoResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CGetRankInfoResponse::CopyFrom(const CGetRankInfoResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CGetRankInfoResponse)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CGetRankInfoResponse::IsInitialized() const {

  return true;
}

void CGetRankInfoResponse::Swap(CGetRankInfoResponse* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CGetRankInfoResponse::InternalSwap(CGetRankInfoResponse* other) {
  std::swap(ranktype_, other->ranktype_);
  roleinfo_.UnsafeArenaSwap(&other->roleinfo_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CGetRankInfoResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CGetRankInfoResponse_descriptor_;
  metadata.reflection = CGetRankInfoResponse_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CGetRankInfoResponse

// optional uint32 RankType = 1;
void CGetRankInfoResponse::clear_ranktype() {
  ranktype_ = 0u;
}
::google::protobuf::uint32 CGetRankInfoResponse::ranktype() const {
  // @@protoc_insertion_point(field_get:CGetRankInfoResponse.RankType)
  return ranktype_;
}
void CGetRankInfoResponse::set_ranktype(::google::protobuf::uint32 value) {
  
  ranktype_ = value;
  // @@protoc_insertion_point(field_set:CGetRankInfoResponse.RankType)
}

// repeated .PBRankRoleInfo RoleInfo = 2;
int CGetRankInfoResponse::roleinfo_size() const {
  return roleinfo_.size();
}
void CGetRankInfoResponse::clear_roleinfo() {
  roleinfo_.Clear();
}
const ::PBRankRoleInfo& CGetRankInfoResponse::roleinfo(int index) const {
  // @@protoc_insertion_point(field_get:CGetRankInfoResponse.RoleInfo)
  return roleinfo_.Get(index);
}
::PBRankRoleInfo* CGetRankInfoResponse::mutable_roleinfo(int index) {
  // @@protoc_insertion_point(field_mutable:CGetRankInfoResponse.RoleInfo)
  return roleinfo_.Mutable(index);
}
::PBRankRoleInfo* CGetRankInfoResponse::add_roleinfo() {
  // @@protoc_insertion_point(field_add:CGetRankInfoResponse.RoleInfo)
  return roleinfo_.Add();
}
::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo >*
CGetRankInfoResponse::mutable_roleinfo() {
  // @@protoc_insertion_point(field_mutable_list:CGetRankInfoResponse.RoleInfo)
  return &roleinfo_;
}
const ::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo >&
CGetRankInfoResponse::roleinfo() const {
  // @@protoc_insertion_point(field_list:CGetRankInfoResponse.RoleInfo)
  return roleinfo_;
}

inline const CGetRankInfoResponse* CGetRankInfoResponse::internal_default_instance() {
  return &CGetRankInfoResponse_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
