// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: itemmodule.hxx

#ifndef PROTOBUF_itemmodule_2ehxx__INCLUDED
#define PROTOBUF_itemmodule_2ehxx__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_itemmodule_2ehxx();
void protobuf_InitDefaults_itemmodule_2ehxx();
void protobuf_AssignDesc_itemmodule_2ehxx();
void protobuf_ShutdownFile_itemmodule_2ehxx();

class CMessagePlayerDiamondUpdateNotify;
class CMessagePlayerMoneyUpdateNotify;

enum ITEM_MODULE_MSG_ID {
  ID_ITEM_MODULE_ERROR = 0,
  ID_S2C_NOTIYF_PLAYER_MONEY_UPDATE = 2048,
  ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE = 2049,
  ITEM_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  ITEM_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool ITEM_MODULE_MSG_ID_IsValid(int value);
const ITEM_MODULE_MSG_ID ITEM_MODULE_MSG_ID_MIN = ID_ITEM_MODULE_ERROR;
const ITEM_MODULE_MSG_ID ITEM_MODULE_MSG_ID_MAX = ID_S2C_NOTIYF_PLAYER_DIAMOND_UPDATE;
const int ITEM_MODULE_MSG_ID_ARRAYSIZE = ITEM_MODULE_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* ITEM_MODULE_MSG_ID_descriptor();
inline const ::std::string& ITEM_MODULE_MSG_ID_Name(ITEM_MODULE_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    ITEM_MODULE_MSG_ID_descriptor(), value);
}
inline bool ITEM_MODULE_MSG_ID_Parse(
    const ::std::string& name, ITEM_MODULE_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ITEM_MODULE_MSG_ID>(
    ITEM_MODULE_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CMessagePlayerMoneyUpdateNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessagePlayerMoneyUpdateNotify) */ {
 public:
  CMessagePlayerMoneyUpdateNotify();
  virtual ~CMessagePlayerMoneyUpdateNotify();

  CMessagePlayerMoneyUpdateNotify(const CMessagePlayerMoneyUpdateNotify& from);

  inline CMessagePlayerMoneyUpdateNotify& operator=(const CMessagePlayerMoneyUpdateNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessagePlayerMoneyUpdateNotify& default_instance();

  static const CMessagePlayerMoneyUpdateNotify* internal_default_instance();

  void Swap(CMessagePlayerMoneyUpdateNotify* other);

  // implements Message ----------------------------------------------

  inline CMessagePlayerMoneyUpdateNotify* New() const { return New(NULL); }

  CMessagePlayerMoneyUpdateNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessagePlayerMoneyUpdateNotify& from);
  void MergeFrom(const CMessagePlayerMoneyUpdateNotify& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CMessagePlayerMoneyUpdateNotify* other);
  void UnsafeMergeFrom(const CMessagePlayerMoneyUpdateNotify& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 Money = 1;
  void clear_money();
  static const int kMoneyFieldNumber = 1;
  ::google::protobuf::uint32 money() const;
  void set_money(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessagePlayerMoneyUpdateNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 money_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_itemmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_itemmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_itemmodule_2ehxx();
  friend void protobuf_ShutdownFile_itemmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessagePlayerMoneyUpdateNotify> CMessagePlayerMoneyUpdateNotify_default_instance_;

// -------------------------------------------------------------------

class CMessagePlayerDiamondUpdateNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessagePlayerDiamondUpdateNotify) */ {
 public:
  CMessagePlayerDiamondUpdateNotify();
  virtual ~CMessagePlayerDiamondUpdateNotify();

  CMessagePlayerDiamondUpdateNotify(const CMessagePlayerDiamondUpdateNotify& from);

  inline CMessagePlayerDiamondUpdateNotify& operator=(const CMessagePlayerDiamondUpdateNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessagePlayerDiamondUpdateNotify& default_instance();

  static const CMessagePlayerDiamondUpdateNotify* internal_default_instance();

  void Swap(CMessagePlayerDiamondUpdateNotify* other);

  // implements Message ----------------------------------------------

  inline CMessagePlayerDiamondUpdateNotify* New() const { return New(NULL); }

  CMessagePlayerDiamondUpdateNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessagePlayerDiamondUpdateNotify& from);
  void MergeFrom(const CMessagePlayerDiamondUpdateNotify& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CMessagePlayerDiamondUpdateNotify* other);
  void UnsafeMergeFrom(const CMessagePlayerDiamondUpdateNotify& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 Diamond = 2;
  void clear_diamond();
  static const int kDiamondFieldNumber = 2;
  ::google::protobuf::uint32 diamond() const;
  void set_diamond(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessagePlayerDiamondUpdateNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 diamond_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_itemmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_itemmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_itemmodule_2ehxx();
  friend void protobuf_ShutdownFile_itemmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessagePlayerDiamondUpdateNotify> CMessagePlayerDiamondUpdateNotify_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CMessagePlayerMoneyUpdateNotify

// optional uint32 Money = 1;
inline void CMessagePlayerMoneyUpdateNotify::clear_money() {
  money_ = 0u;
}
inline ::google::protobuf::uint32 CMessagePlayerMoneyUpdateNotify::money() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerMoneyUpdateNotify.Money)
  return money_;
}
inline void CMessagePlayerMoneyUpdateNotify::set_money(::google::protobuf::uint32 value) {
  
  money_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerMoneyUpdateNotify.Money)
}

inline const CMessagePlayerMoneyUpdateNotify* CMessagePlayerMoneyUpdateNotify::internal_default_instance() {
  return &CMessagePlayerMoneyUpdateNotify_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessagePlayerDiamondUpdateNotify

// optional uint32 Diamond = 2;
inline void CMessagePlayerDiamondUpdateNotify::clear_diamond() {
  diamond_ = 0u;
}
inline ::google::protobuf::uint32 CMessagePlayerDiamondUpdateNotify::diamond() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerDiamondUpdateNotify.Diamond)
  return diamond_;
}
inline void CMessagePlayerDiamondUpdateNotify::set_diamond(::google::protobuf::uint32 value) {
  
  diamond_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerDiamondUpdateNotify.Diamond)
}

inline const CMessagePlayerDiamondUpdateNotify* CMessagePlayerDiamondUpdateNotify::internal_default_instance() {
  return &CMessagePlayerDiamondUpdateNotify_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::ITEM_MODULE_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ITEM_MODULE_MSG_ID>() {
  return ::ITEM_MODULE_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_itemmodule_2ehxx__INCLUDED