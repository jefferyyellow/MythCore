// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.hxx

#ifndef PROTOBUF_common_2ehxx__INCLUDED
#define PROTOBUF_common_2ehxx__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_common_2ehxx();
void protobuf_InitDefaults_common_2ehxx();
void protobuf_AssignDesc_common_2ehxx();
void protobuf_ShutdownFile_common_2ehxx();

class PBItemEquip;
class PBItemObject;

// ===================================================================

class PBItemObject : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBItemObject) */ {
 public:
  PBItemObject();
  virtual ~PBItemObject();

  PBItemObject(const PBItemObject& from);

  inline PBItemObject& operator=(const PBItemObject& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBItemObject& default_instance();

  static const PBItemObject* internal_default_instance();

  void Swap(PBItemObject* other);

  // implements Message ----------------------------------------------

  inline PBItemObject* New() const { return New(NULL); }

  PBItemObject* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBItemObject& from);
  void MergeFrom(const PBItemObject& from);
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
  void InternalSwap(PBItemObject* other);
  void UnsafeMergeFrom(const PBItemObject& from);
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

  // optional uint32 Number = 2;
  void clear_number();
  static const int kNumberFieldNumber = 2;
  ::google::protobuf::uint32 number() const;
  void set_number(::google::protobuf::uint32 value);

  // optional uint32 Index = 3;
  void clear_index();
  static const int kIndexFieldNumber = 3;
  ::google::protobuf::uint32 index() const;
  void set_index(::google::protobuf::uint32 value);

  // optional .PBItemEquip ItemEquip = 4;
  bool has_itemequip() const;
  void clear_itemequip();
  static const int kItemEquipFieldNumber = 4;
  const ::PBItemEquip& itemequip() const;
  ::PBItemEquip* mutable_itemequip();
  ::PBItemEquip* release_itemequip();
  void set_allocated_itemequip(::PBItemEquip* itemequip);

  // @@protoc_insertion_point(class_scope:PBItemObject)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::PBItemEquip* itemequip_;
  ::google::protobuf::uint32 itemid_;
  ::google::protobuf::uint32 number_;
  ::google::protobuf::uint32 index_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_common_2ehxx_impl();
  friend void  protobuf_AddDesc_common_2ehxx_impl();
  friend void protobuf_AssignDesc_common_2ehxx();
  friend void protobuf_ShutdownFile_common_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBItemObject> PBItemObject_default_instance_;

// -------------------------------------------------------------------

class PBItemEquip : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBItemEquip) */ {
 public:
  PBItemEquip();
  virtual ~PBItemEquip();

  PBItemEquip(const PBItemEquip& from);

  inline PBItemEquip& operator=(const PBItemEquip& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBItemEquip& default_instance();

  static const PBItemEquip* internal_default_instance();

  void Swap(PBItemEquip* other);

  // implements Message ----------------------------------------------

  inline PBItemEquip* New() const { return New(NULL); }

  PBItemEquip* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBItemEquip& from);
  void MergeFrom(const PBItemEquip& from);
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
  void InternalSwap(PBItemEquip* other);
  void UnsafeMergeFrom(const PBItemEquip& from);
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

  // optional uint32 Level = 4;
  void clear_level();
  static const int kLevelFieldNumber = 4;
  ::google::protobuf::uint32 level() const;
  void set_level(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:PBItemEquip)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 level_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_common_2ehxx_impl();
  friend void  protobuf_AddDesc_common_2ehxx_impl();
  friend void protobuf_AssignDesc_common_2ehxx();
  friend void protobuf_ShutdownFile_common_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBItemEquip> PBItemEquip_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// PBItemObject

// optional uint32 ItemID = 1;
inline void PBItemObject::clear_itemid() {
  itemid_ = 0u;
}
inline ::google::protobuf::uint32 PBItemObject::itemid() const {
  // @@protoc_insertion_point(field_get:PBItemObject.ItemID)
  return itemid_;
}
inline void PBItemObject::set_itemid(::google::protobuf::uint32 value) {
  
  itemid_ = value;
  // @@protoc_insertion_point(field_set:PBItemObject.ItemID)
}

// optional uint32 Number = 2;
inline void PBItemObject::clear_number() {
  number_ = 0u;
}
inline ::google::protobuf::uint32 PBItemObject::number() const {
  // @@protoc_insertion_point(field_get:PBItemObject.Number)
  return number_;
}
inline void PBItemObject::set_number(::google::protobuf::uint32 value) {
  
  number_ = value;
  // @@protoc_insertion_point(field_set:PBItemObject.Number)
}

// optional uint32 Index = 3;
inline void PBItemObject::clear_index() {
  index_ = 0u;
}
inline ::google::protobuf::uint32 PBItemObject::index() const {
  // @@protoc_insertion_point(field_get:PBItemObject.Index)
  return index_;
}
inline void PBItemObject::set_index(::google::protobuf::uint32 value) {
  
  index_ = value;
  // @@protoc_insertion_point(field_set:PBItemObject.Index)
}

// optional .PBItemEquip ItemEquip = 4;
inline bool PBItemObject::has_itemequip() const {
  return this != internal_default_instance() && itemequip_ != NULL;
}
inline void PBItemObject::clear_itemequip() {
  if (GetArenaNoVirtual() == NULL && itemequip_ != NULL) delete itemequip_;
  itemequip_ = NULL;
}
inline const ::PBItemEquip& PBItemObject::itemequip() const {
  // @@protoc_insertion_point(field_get:PBItemObject.ItemEquip)
  return itemequip_ != NULL ? *itemequip_
                         : *::PBItemEquip::internal_default_instance();
}
inline ::PBItemEquip* PBItemObject::mutable_itemequip() {
  
  if (itemequip_ == NULL) {
    itemequip_ = new ::PBItemEquip;
  }
  // @@protoc_insertion_point(field_mutable:PBItemObject.ItemEquip)
  return itemequip_;
}
inline ::PBItemEquip* PBItemObject::release_itemequip() {
  // @@protoc_insertion_point(field_release:PBItemObject.ItemEquip)
  
  ::PBItemEquip* temp = itemequip_;
  itemequip_ = NULL;
  return temp;
}
inline void PBItemObject::set_allocated_itemequip(::PBItemEquip* itemequip) {
  delete itemequip_;
  itemequip_ = itemequip;
  if (itemequip) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:PBItemObject.ItemEquip)
}

inline const PBItemObject* PBItemObject::internal_default_instance() {
  return &PBItemObject_default_instance_.get();
}
// -------------------------------------------------------------------

// PBItemEquip

// optional uint32 Level = 4;
inline void PBItemEquip::clear_level() {
  level_ = 0u;
}
inline ::google::protobuf::uint32 PBItemEquip::level() const {
  // @@protoc_insertion_point(field_get:PBItemEquip.Level)
  return level_;
}
inline void PBItemEquip::set_level(::google::protobuf::uint32 value) {
  
  level_ = value;
  // @@protoc_insertion_point(field_set:PBItemEquip.Level)
}

inline const PBItemEquip* PBItemEquip::internal_default_instance() {
  return &PBItemEquip_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_common_2ehxx__INCLUDED