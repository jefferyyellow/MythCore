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

class CDiamondUpdateNotify;
class CInsertItemNotify;
class CMoneyUpdateNotify;

enum ITEM_MODULE_MSG_ID {
  ID_ITEM_MODULE_ERROR = 0,
  ID_S2C_NOTIYF_MONEY_UPDATE = 2048,
  ID_S2C_NOTIYF_DIAMOND_UPDATE = 2049,
  ITEM_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  ITEM_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool ITEM_MODULE_MSG_ID_IsValid(int value);
const ITEM_MODULE_MSG_ID ITEM_MODULE_MSG_ID_MIN = ID_ITEM_MODULE_ERROR;
const ITEM_MODULE_MSG_ID ITEM_MODULE_MSG_ID_MAX = ID_S2C_NOTIYF_DIAMOND_UPDATE;
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

class CMoneyUpdateNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMoneyUpdateNotify) */ {
 public:
  CMoneyUpdateNotify();
  virtual ~CMoneyUpdateNotify();

  CMoneyUpdateNotify(const CMoneyUpdateNotify& from);

  inline CMoneyUpdateNotify& operator=(const CMoneyUpdateNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMoneyUpdateNotify& default_instance();

  static const CMoneyUpdateNotify* internal_default_instance();

  void Swap(CMoneyUpdateNotify* other);

  // implements Message ----------------------------------------------

  inline CMoneyUpdateNotify* New() const { return New(NULL); }

  CMoneyUpdateNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMoneyUpdateNotify& from);
  void MergeFrom(const CMoneyUpdateNotify& from);
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
  void InternalSwap(CMoneyUpdateNotify* other);
  void UnsafeMergeFrom(const CMoneyUpdateNotify& from);
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

  // @@protoc_insertion_point(class_scope:CMoneyUpdateNotify)
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
extern ::google::protobuf::internal::ExplicitlyConstructed<CMoneyUpdateNotify> CMoneyUpdateNotify_default_instance_;

// -------------------------------------------------------------------

class CDiamondUpdateNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CDiamondUpdateNotify) */ {
 public:
  CDiamondUpdateNotify();
  virtual ~CDiamondUpdateNotify();

  CDiamondUpdateNotify(const CDiamondUpdateNotify& from);

  inline CDiamondUpdateNotify& operator=(const CDiamondUpdateNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CDiamondUpdateNotify& default_instance();

  static const CDiamondUpdateNotify* internal_default_instance();

  void Swap(CDiamondUpdateNotify* other);

  // implements Message ----------------------------------------------

  inline CDiamondUpdateNotify* New() const { return New(NULL); }

  CDiamondUpdateNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CDiamondUpdateNotify& from);
  void MergeFrom(const CDiamondUpdateNotify& from);
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
  void InternalSwap(CDiamondUpdateNotify* other);
  void UnsafeMergeFrom(const CDiamondUpdateNotify& from);
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

  // optional uint32 Diamond = 1;
  void clear_diamond();
  static const int kDiamondFieldNumber = 1;
  ::google::protobuf::uint32 diamond() const;
  void set_diamond(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CDiamondUpdateNotify)
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
extern ::google::protobuf::internal::ExplicitlyConstructed<CDiamondUpdateNotify> CDiamondUpdateNotify_default_instance_;

// -------------------------------------------------------------------

class CInsertItemNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CInsertItemNotify) */ {
 public:
  CInsertItemNotify();
  virtual ~CInsertItemNotify();

  CInsertItemNotify(const CInsertItemNotify& from);

  inline CInsertItemNotify& operator=(const CInsertItemNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CInsertItemNotify& default_instance();

  static const CInsertItemNotify* internal_default_instance();

  void Swap(CInsertItemNotify* other);

  // implements Message ----------------------------------------------

  inline CInsertItemNotify* New() const { return New(NULL); }

  CInsertItemNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CInsertItemNotify& from);
  void MergeFrom(const CInsertItemNotify& from);
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
  void InternalSwap(CInsertItemNotify* other);
  void UnsafeMergeFrom(const CInsertItemNotify& from);
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

  // optional uint32 ItemID = 1;
  void clear_itemid();
  static const int kItemIDFieldNumber = 1;
  ::google::protobuf::uint32 itemid() const;
  void set_itemid(::google::protobuf::uint32 value);

  // optional uint32 Index = 2;
  void clear_index();
  static const int kIndexFieldNumber = 2;
  ::google::protobuf::uint32 index() const;
  void set_index(::google::protobuf::uint32 value);

  // optional uint32 Number = 3;
  void clear_number();
  static const int kNumberFieldNumber = 3;
  ::google::protobuf::uint32 number() const;
  void set_number(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CInsertItemNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 itemid_;
  ::google::protobuf::uint32 index_;
  ::google::protobuf::uint32 number_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_itemmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_itemmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_itemmodule_2ehxx();
  friend void protobuf_ShutdownFile_itemmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CInsertItemNotify> CInsertItemNotify_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CMoneyUpdateNotify

// optional uint32 Money = 1;
inline void CMoneyUpdateNotify::clear_money() {
  money_ = 0u;
}
inline ::google::protobuf::uint32 CMoneyUpdateNotify::money() const {
  // @@protoc_insertion_point(field_get:CMoneyUpdateNotify.Money)
  return money_;
}
inline void CMoneyUpdateNotify::set_money(::google::protobuf::uint32 value) {
  
  money_ = value;
  // @@protoc_insertion_point(field_set:CMoneyUpdateNotify.Money)
}

inline const CMoneyUpdateNotify* CMoneyUpdateNotify::internal_default_instance() {
  return &CMoneyUpdateNotify_default_instance_.get();
}
// -------------------------------------------------------------------

// CDiamondUpdateNotify

// optional uint32 Diamond = 1;
inline void CDiamondUpdateNotify::clear_diamond() {
  diamond_ = 0u;
}
inline ::google::protobuf::uint32 CDiamondUpdateNotify::diamond() const {
  // @@protoc_insertion_point(field_get:CDiamondUpdateNotify.Diamond)
  return diamond_;
}
inline void CDiamondUpdateNotify::set_diamond(::google::protobuf::uint32 value) {
  
  diamond_ = value;
  // @@protoc_insertion_point(field_set:CDiamondUpdateNotify.Diamond)
}

inline const CDiamondUpdateNotify* CDiamondUpdateNotify::internal_default_instance() {
  return &CDiamondUpdateNotify_default_instance_.get();
}
// -------------------------------------------------------------------

// CInsertItemNotify

// optional uint32 ItemID = 1;
inline void CInsertItemNotify::clear_itemid() {
  itemid_ = 0u;
}
inline ::google::protobuf::uint32 CInsertItemNotify::itemid() const {
  // @@protoc_insertion_point(field_get:CInsertItemNotify.ItemID)
  return itemid_;
}
inline void CInsertItemNotify::set_itemid(::google::protobuf::uint32 value) {
  
  itemid_ = value;
  // @@protoc_insertion_point(field_set:CInsertItemNotify.ItemID)
}

// optional uint32 Index = 2;
inline void CInsertItemNotify::clear_index() {
  index_ = 0u;
}
inline ::google::protobuf::uint32 CInsertItemNotify::index() const {
  // @@protoc_insertion_point(field_get:CInsertItemNotify.Index)
  return index_;
}
inline void CInsertItemNotify::set_index(::google::protobuf::uint32 value) {
  
  index_ = value;
  // @@protoc_insertion_point(field_set:CInsertItemNotify.Index)
}

// optional uint32 Number = 3;
inline void CInsertItemNotify::clear_number() {
  number_ = 0u;
}
inline ::google::protobuf::uint32 CInsertItemNotify::number() const {
  // @@protoc_insertion_point(field_get:CInsertItemNotify.Number)
  return number_;
}
inline void CInsertItemNotify::set_number(::google::protobuf::uint32 value) {
  
  number_ = value;
  // @@protoc_insertion_point(field_set:CInsertItemNotify.Number)
}

inline const CInsertItemNotify* CInsertItemNotify::internal_default_instance() {
  return &CInsertItemNotify_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

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
