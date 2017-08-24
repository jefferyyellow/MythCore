// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: itemmodule.hxx

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "itemmodule.hxx.pb.h"

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

const ::google::protobuf::Descriptor* CMessagePlayerMoneyUpdateNotify_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CMessagePlayerMoneyUpdateNotify_reflection_ = NULL;
const ::google::protobuf::Descriptor* CMessagePlayerDiamondUpdateNotify_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CMessagePlayerDiamondUpdateNotify_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* ITEM_MODULE_MSG_ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_itemmodule_2ehxx() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_itemmodule_2ehxx() {
  protobuf_AddDesc_itemmodule_2ehxx();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "itemmodule.hxx");
  GOOGLE_CHECK(file != NULL);
  CMessagePlayerMoneyUpdateNotify_descriptor_ = file->message_type(0);
  static const int CMessagePlayerMoneyUpdateNotify_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CMessagePlayerMoneyUpdateNotify, money_),
  };
  CMessagePlayerMoneyUpdateNotify_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CMessagePlayerMoneyUpdateNotify_descriptor_,
      CMessagePlayerMoneyUpdateNotify::internal_default_instance(),
      CMessagePlayerMoneyUpdateNotify_offsets_,
      -1,
      -1,
      -1,
      sizeof(CMessagePlayerMoneyUpdateNotify),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CMessagePlayerMoneyUpdateNotify, _internal_metadata_));
  CMessagePlayerDiamondUpdateNotify_descriptor_ = file->message_type(1);
  static const int CMessagePlayerDiamondUpdateNotify_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CMessagePlayerDiamondUpdateNotify, diamond_),
  };
  CMessagePlayerDiamondUpdateNotify_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      CMessagePlayerDiamondUpdateNotify_descriptor_,
      CMessagePlayerDiamondUpdateNotify::internal_default_instance(),
      CMessagePlayerDiamondUpdateNotify_offsets_,
      -1,
      -1,
      -1,
      sizeof(CMessagePlayerDiamondUpdateNotify),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CMessagePlayerDiamondUpdateNotify, _internal_metadata_));
  ITEM_MODULE_MSG_ID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_itemmodule_2ehxx);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CMessagePlayerMoneyUpdateNotify_descriptor_, CMessagePlayerMoneyUpdateNotify::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      CMessagePlayerDiamondUpdateNotify_descriptor_, CMessagePlayerDiamondUpdateNotify::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_itemmodule_2ehxx() {
  CMessagePlayerMoneyUpdateNotify_default_instance_.Shutdown();
  delete CMessagePlayerMoneyUpdateNotify_reflection_;
  CMessagePlayerDiamondUpdateNotify_default_instance_.Shutdown();
  delete CMessagePlayerDiamondUpdateNotify_reflection_;
}

void protobuf_InitDefaults_itemmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  CMessagePlayerMoneyUpdateNotify_default_instance_.DefaultConstruct();
  CMessagePlayerDiamondUpdateNotify_default_instance_.DefaultConstruct();
  CMessagePlayerMoneyUpdateNotify_default_instance_.get_mutable()->InitAsDefaultInstance();
  CMessagePlayerDiamondUpdateNotify_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_itemmodule_2ehxx_once_);
void protobuf_InitDefaults_itemmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_itemmodule_2ehxx_once_,
                 &protobuf_InitDefaults_itemmodule_2ehxx_impl);
}
void protobuf_AddDesc_itemmodule_2ehxx_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_itemmodule_2ehxx();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016itemmodule.hxx\"0\n\037CMessagePlayerMoneyU"
    "pdateNotify\022\r\n\005Money\030\001 \001(\r\"4\n!CMessagePl"
    "ayerDiamondUpdateNotify\022\017\n\007Diamond\030\002 \001(\r"
    "*\200\001\n\022ITEM_MODULE_MSG_ID\022\030\n\024ID_ITEM_MODUL"
    "E_ERROR\020\000\022&\n!ID_S2C_NOTIYF_PLAYER_MONEY_"
    "UPDATE\020\200\020\022(\n#ID_S2C_NOTIYF_PLAYER_DIAMON"
    "D_UPDATE\020\201\020b\006proto3", 259);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "itemmodule.hxx", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_itemmodule_2ehxx);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_itemmodule_2ehxx_once_);
void protobuf_AddDesc_itemmodule_2ehxx() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_itemmodule_2ehxx_once_,
                 &protobuf_AddDesc_itemmodule_2ehxx_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_itemmodule_2ehxx {
  StaticDescriptorInitializer_itemmodule_2ehxx() {
    protobuf_AddDesc_itemmodule_2ehxx();
  }
} static_descriptor_initializer_itemmodule_2ehxx_;
const ::google::protobuf::EnumDescriptor* ITEM_MODULE_MSG_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ITEM_MODULE_MSG_ID_descriptor_;
}
bool ITEM_MODULE_MSG_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 2048:
    case 2049:
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
const int CMessagePlayerMoneyUpdateNotify::kMoneyFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CMessagePlayerMoneyUpdateNotify::CMessagePlayerMoneyUpdateNotify()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_itemmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CMessagePlayerMoneyUpdateNotify)
}

void CMessagePlayerMoneyUpdateNotify::InitAsDefaultInstance() {
}

CMessagePlayerMoneyUpdateNotify::CMessagePlayerMoneyUpdateNotify(const CMessagePlayerMoneyUpdateNotify& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CMessagePlayerMoneyUpdateNotify)
}

void CMessagePlayerMoneyUpdateNotify::SharedCtor() {
  money_ = 0u;
  _cached_size_ = 0;
}

CMessagePlayerMoneyUpdateNotify::~CMessagePlayerMoneyUpdateNotify() {
  // @@protoc_insertion_point(destructor:CMessagePlayerMoneyUpdateNotify)
  SharedDtor();
}

void CMessagePlayerMoneyUpdateNotify::SharedDtor() {
}

void CMessagePlayerMoneyUpdateNotify::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CMessagePlayerMoneyUpdateNotify::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CMessagePlayerMoneyUpdateNotify_descriptor_;
}

const CMessagePlayerMoneyUpdateNotify& CMessagePlayerMoneyUpdateNotify::default_instance() {
  protobuf_InitDefaults_itemmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CMessagePlayerMoneyUpdateNotify> CMessagePlayerMoneyUpdateNotify_default_instance_;

CMessagePlayerMoneyUpdateNotify* CMessagePlayerMoneyUpdateNotify::New(::google::protobuf::Arena* arena) const {
  CMessagePlayerMoneyUpdateNotify* n = new CMessagePlayerMoneyUpdateNotify;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CMessagePlayerMoneyUpdateNotify::Clear() {
// @@protoc_insertion_point(message_clear_start:CMessagePlayerMoneyUpdateNotify)
  money_ = 0u;
}

bool CMessagePlayerMoneyUpdateNotify::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CMessagePlayerMoneyUpdateNotify)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 Money = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &money_)));
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
  // @@protoc_insertion_point(parse_success:CMessagePlayerMoneyUpdateNotify)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CMessagePlayerMoneyUpdateNotify)
  return false;
#undef DO_
}

void CMessagePlayerMoneyUpdateNotify::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CMessagePlayerMoneyUpdateNotify)
  // optional uint32 Money = 1;
  if (this->money() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->money(), output);
  }

  // @@protoc_insertion_point(serialize_end:CMessagePlayerMoneyUpdateNotify)
}

::google::protobuf::uint8* CMessagePlayerMoneyUpdateNotify::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CMessagePlayerMoneyUpdateNotify)
  // optional uint32 Money = 1;
  if (this->money() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->money(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:CMessagePlayerMoneyUpdateNotify)
  return target;
}

size_t CMessagePlayerMoneyUpdateNotify::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CMessagePlayerMoneyUpdateNotify)
  size_t total_size = 0;

  // optional uint32 Money = 1;
  if (this->money() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->money());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CMessagePlayerMoneyUpdateNotify::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CMessagePlayerMoneyUpdateNotify)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CMessagePlayerMoneyUpdateNotify* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CMessagePlayerMoneyUpdateNotify>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CMessagePlayerMoneyUpdateNotify)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CMessagePlayerMoneyUpdateNotify)
    UnsafeMergeFrom(*source);
  }
}

void CMessagePlayerMoneyUpdateNotify::MergeFrom(const CMessagePlayerMoneyUpdateNotify& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CMessagePlayerMoneyUpdateNotify)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CMessagePlayerMoneyUpdateNotify::UnsafeMergeFrom(const CMessagePlayerMoneyUpdateNotify& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.money() != 0) {
    set_money(from.money());
  }
}

void CMessagePlayerMoneyUpdateNotify::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CMessagePlayerMoneyUpdateNotify)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CMessagePlayerMoneyUpdateNotify::CopyFrom(const CMessagePlayerMoneyUpdateNotify& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CMessagePlayerMoneyUpdateNotify)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CMessagePlayerMoneyUpdateNotify::IsInitialized() const {

  return true;
}

void CMessagePlayerMoneyUpdateNotify::Swap(CMessagePlayerMoneyUpdateNotify* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CMessagePlayerMoneyUpdateNotify::InternalSwap(CMessagePlayerMoneyUpdateNotify* other) {
  std::swap(money_, other->money_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CMessagePlayerMoneyUpdateNotify::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CMessagePlayerMoneyUpdateNotify_descriptor_;
  metadata.reflection = CMessagePlayerMoneyUpdateNotify_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CMessagePlayerMoneyUpdateNotify

// optional uint32 Money = 1;
void CMessagePlayerMoneyUpdateNotify::clear_money() {
  money_ = 0u;
}
::google::protobuf::uint32 CMessagePlayerMoneyUpdateNotify::money() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerMoneyUpdateNotify.Money)
  return money_;
}
void CMessagePlayerMoneyUpdateNotify::set_money(::google::protobuf::uint32 value) {
  
  money_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerMoneyUpdateNotify.Money)
}

inline const CMessagePlayerMoneyUpdateNotify* CMessagePlayerMoneyUpdateNotify::internal_default_instance() {
  return &CMessagePlayerMoneyUpdateNotify_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int CMessagePlayerDiamondUpdateNotify::kDiamondFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CMessagePlayerDiamondUpdateNotify::CMessagePlayerDiamondUpdateNotify()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_itemmodule_2ehxx();
  SharedCtor();
  // @@protoc_insertion_point(constructor:CMessagePlayerDiamondUpdateNotify)
}

void CMessagePlayerDiamondUpdateNotify::InitAsDefaultInstance() {
}

CMessagePlayerDiamondUpdateNotify::CMessagePlayerDiamondUpdateNotify(const CMessagePlayerDiamondUpdateNotify& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:CMessagePlayerDiamondUpdateNotify)
}

void CMessagePlayerDiamondUpdateNotify::SharedCtor() {
  diamond_ = 0u;
  _cached_size_ = 0;
}

CMessagePlayerDiamondUpdateNotify::~CMessagePlayerDiamondUpdateNotify() {
  // @@protoc_insertion_point(destructor:CMessagePlayerDiamondUpdateNotify)
  SharedDtor();
}

void CMessagePlayerDiamondUpdateNotify::SharedDtor() {
}

void CMessagePlayerDiamondUpdateNotify::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CMessagePlayerDiamondUpdateNotify::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CMessagePlayerDiamondUpdateNotify_descriptor_;
}

const CMessagePlayerDiamondUpdateNotify& CMessagePlayerDiamondUpdateNotify::default_instance() {
  protobuf_InitDefaults_itemmodule_2ehxx();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<CMessagePlayerDiamondUpdateNotify> CMessagePlayerDiamondUpdateNotify_default_instance_;

CMessagePlayerDiamondUpdateNotify* CMessagePlayerDiamondUpdateNotify::New(::google::protobuf::Arena* arena) const {
  CMessagePlayerDiamondUpdateNotify* n = new CMessagePlayerDiamondUpdateNotify;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CMessagePlayerDiamondUpdateNotify::Clear() {
// @@protoc_insertion_point(message_clear_start:CMessagePlayerDiamondUpdateNotify)
  diamond_ = 0u;
}

bool CMessagePlayerDiamondUpdateNotify::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:CMessagePlayerDiamondUpdateNotify)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 Diamond = 2;
      case 2: {
        if (tag == 16) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &diamond_)));
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
  // @@protoc_insertion_point(parse_success:CMessagePlayerDiamondUpdateNotify)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:CMessagePlayerDiamondUpdateNotify)
  return false;
#undef DO_
}

void CMessagePlayerDiamondUpdateNotify::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:CMessagePlayerDiamondUpdateNotify)
  // optional uint32 Diamond = 2;
  if (this->diamond() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->diamond(), output);
  }

  // @@protoc_insertion_point(serialize_end:CMessagePlayerDiamondUpdateNotify)
}

::google::protobuf::uint8* CMessagePlayerDiamondUpdateNotify::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:CMessagePlayerDiamondUpdateNotify)
  // optional uint32 Diamond = 2;
  if (this->diamond() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->diamond(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:CMessagePlayerDiamondUpdateNotify)
  return target;
}

size_t CMessagePlayerDiamondUpdateNotify::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CMessagePlayerDiamondUpdateNotify)
  size_t total_size = 0;

  // optional uint32 Diamond = 2;
  if (this->diamond() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->diamond());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CMessagePlayerDiamondUpdateNotify::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CMessagePlayerDiamondUpdateNotify)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const CMessagePlayerDiamondUpdateNotify* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CMessagePlayerDiamondUpdateNotify>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CMessagePlayerDiamondUpdateNotify)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CMessagePlayerDiamondUpdateNotify)
    UnsafeMergeFrom(*source);
  }
}

void CMessagePlayerDiamondUpdateNotify::MergeFrom(const CMessagePlayerDiamondUpdateNotify& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CMessagePlayerDiamondUpdateNotify)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void CMessagePlayerDiamondUpdateNotify::UnsafeMergeFrom(const CMessagePlayerDiamondUpdateNotify& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.diamond() != 0) {
    set_diamond(from.diamond());
  }
}

void CMessagePlayerDiamondUpdateNotify::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CMessagePlayerDiamondUpdateNotify)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CMessagePlayerDiamondUpdateNotify::CopyFrom(const CMessagePlayerDiamondUpdateNotify& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CMessagePlayerDiamondUpdateNotify)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool CMessagePlayerDiamondUpdateNotify::IsInitialized() const {

  return true;
}

void CMessagePlayerDiamondUpdateNotify::Swap(CMessagePlayerDiamondUpdateNotify* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CMessagePlayerDiamondUpdateNotify::InternalSwap(CMessagePlayerDiamondUpdateNotify* other) {
  std::swap(diamond_, other->diamond_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CMessagePlayerDiamondUpdateNotify::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CMessagePlayerDiamondUpdateNotify_descriptor_;
  metadata.reflection = CMessagePlayerDiamondUpdateNotify_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// CMessagePlayerDiamondUpdateNotify

// optional uint32 Diamond = 2;
void CMessagePlayerDiamondUpdateNotify::clear_diamond() {
  diamond_ = 0u;
}
::google::protobuf::uint32 CMessagePlayerDiamondUpdateNotify::diamond() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerDiamondUpdateNotify.Diamond)
  return diamond_;
}
void CMessagePlayerDiamondUpdateNotify::set_diamond(::google::protobuf::uint32 value) {
  
  diamond_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerDiamondUpdateNotify.Diamond)
}

inline const CMessagePlayerDiamondUpdateNotify* CMessagePlayerDiamondUpdateNotify::internal_default_instance() {
  return &CMessagePlayerDiamondUpdateNotify_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)