// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mapmodule.hxx

#ifndef PROTOBUF_mapmodule_2ehxx__INCLUDED
#define PROTOBUF_mapmodule_2ehxx__INCLUDED

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
void protobuf_AddDesc_mapmodule_2ehxx();
void protobuf_InitDefaults_mapmodule_2ehxx();
void protobuf_AssignDesc_mapmodule_2ehxx();
void protobuf_ShutdownFile_mapmodule_2ehxx();

class CMessageCreateNPCListNotify;
class CMessageCreatePlayerListNotify;
class CMessageEntityMoveNotify;
class CMessagePlayerSceneInfo;
class PBNpcSceneInfo;

enum MAP_MODULE_MSG_ID {
  ID_MAP_MODULE_ERROR = 0,
  ID_S2C_NOTIYF_ENTITY_MOVE = 3072,
  ID_S2C_NOTIYF_CREATE_PLAYER_LIST = 3073,
  ID_S2C_NOTIYF_CREATE_NPC_LIST = 3074,
  MAP_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  MAP_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool MAP_MODULE_MSG_ID_IsValid(int value);
const MAP_MODULE_MSG_ID MAP_MODULE_MSG_ID_MIN = ID_MAP_MODULE_ERROR;
const MAP_MODULE_MSG_ID MAP_MODULE_MSG_ID_MAX = ID_S2C_NOTIYF_CREATE_NPC_LIST;
const int MAP_MODULE_MSG_ID_ARRAYSIZE = MAP_MODULE_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* MAP_MODULE_MSG_ID_descriptor();
inline const ::std::string& MAP_MODULE_MSG_ID_Name(MAP_MODULE_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    MAP_MODULE_MSG_ID_descriptor(), value);
}
inline bool MAP_MODULE_MSG_ID_Parse(
    const ::std::string& name, MAP_MODULE_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MAP_MODULE_MSG_ID>(
    MAP_MODULE_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CMessageEntityMoveNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageEntityMoveNotify) */ {
 public:
  CMessageEntityMoveNotify();
  virtual ~CMessageEntityMoveNotify();

  CMessageEntityMoveNotify(const CMessageEntityMoveNotify& from);

  inline CMessageEntityMoveNotify& operator=(const CMessageEntityMoveNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageEntityMoveNotify& default_instance();

  static const CMessageEntityMoveNotify* internal_default_instance();

  void Swap(CMessageEntityMoveNotify* other);

  // implements Message ----------------------------------------------

  inline CMessageEntityMoveNotify* New() const { return New(NULL); }

  CMessageEntityMoveNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageEntityMoveNotify& from);
  void MergeFrom(const CMessageEntityMoveNotify& from);
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
  void InternalSwap(CMessageEntityMoveNotify* other);
  void UnsafeMergeFrom(const CMessageEntityMoveNotify& from);
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

  // optional uint32 EntityID = 1;
  void clear_entityid();
  static const int kEntityIDFieldNumber = 1;
  ::google::protobuf::uint32 entityid() const;
  void set_entityid(::google::protobuf::uint32 value);

  // optional uint32 DesPosX = 2;
  void clear_desposx();
  static const int kDesPosXFieldNumber = 2;
  ::google::protobuf::uint32 desposx() const;
  void set_desposx(::google::protobuf::uint32 value);

  // optional uint32 DesPosY = 3;
  void clear_desposy();
  static const int kDesPosYFieldNumber = 3;
  ::google::protobuf::uint32 desposy() const;
  void set_desposy(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessageEntityMoveNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 entityid_;
  ::google::protobuf::uint32 desposx_;
  ::google::protobuf::uint32 desposy_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_mapmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_mapmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_mapmodule_2ehxx();
  friend void protobuf_ShutdownFile_mapmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageEntityMoveNotify> CMessageEntityMoveNotify_default_instance_;

// -------------------------------------------------------------------

class CMessagePlayerSceneInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessagePlayerSceneInfo) */ {
 public:
  CMessagePlayerSceneInfo();
  virtual ~CMessagePlayerSceneInfo();

  CMessagePlayerSceneInfo(const CMessagePlayerSceneInfo& from);

  inline CMessagePlayerSceneInfo& operator=(const CMessagePlayerSceneInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessagePlayerSceneInfo& default_instance();

  static const CMessagePlayerSceneInfo* internal_default_instance();

  void Swap(CMessagePlayerSceneInfo* other);

  // implements Message ----------------------------------------------

  inline CMessagePlayerSceneInfo* New() const { return New(NULL); }

  CMessagePlayerSceneInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessagePlayerSceneInfo& from);
  void MergeFrom(const CMessagePlayerSceneInfo& from);
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
  void InternalSwap(CMessagePlayerSceneInfo* other);
  void UnsafeMergeFrom(const CMessagePlayerSceneInfo& from);
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

  // optional uint32 EntityID = 1;
  void clear_entityid();
  static const int kEntityIDFieldNumber = 1;
  ::google::protobuf::uint32 entityid() const;
  void set_entityid(::google::protobuf::uint32 value);

  // optional uint32 PosX = 2;
  void clear_posx();
  static const int kPosXFieldNumber = 2;
  ::google::protobuf::uint32 posx() const;
  void set_posx(::google::protobuf::uint32 value);

  // optional uint32 PosY = 3;
  void clear_posy();
  static const int kPosYFieldNumber = 3;
  ::google::protobuf::uint32 posy() const;
  void set_posy(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessagePlayerSceneInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 entityid_;
  ::google::protobuf::uint32 posx_;
  ::google::protobuf::uint32 posy_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_mapmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_mapmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_mapmodule_2ehxx();
  friend void protobuf_ShutdownFile_mapmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessagePlayerSceneInfo> CMessagePlayerSceneInfo_default_instance_;

// -------------------------------------------------------------------

class CMessageCreatePlayerListNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageCreatePlayerListNotify) */ {
 public:
  CMessageCreatePlayerListNotify();
  virtual ~CMessageCreatePlayerListNotify();

  CMessageCreatePlayerListNotify(const CMessageCreatePlayerListNotify& from);

  inline CMessageCreatePlayerListNotify& operator=(const CMessageCreatePlayerListNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageCreatePlayerListNotify& default_instance();

  static const CMessageCreatePlayerListNotify* internal_default_instance();

  void Swap(CMessageCreatePlayerListNotify* other);

  // implements Message ----------------------------------------------

  inline CMessageCreatePlayerListNotify* New() const { return New(NULL); }

  CMessageCreatePlayerListNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageCreatePlayerListNotify& from);
  void MergeFrom(const CMessageCreatePlayerListNotify& from);
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
  void InternalSwap(CMessageCreatePlayerListNotify* other);
  void UnsafeMergeFrom(const CMessageCreatePlayerListNotify& from);
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

  // @@protoc_insertion_point(class_scope:CMessageCreatePlayerListNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_mapmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_mapmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_mapmodule_2ehxx();
  friend void protobuf_ShutdownFile_mapmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageCreatePlayerListNotify> CMessageCreatePlayerListNotify_default_instance_;

// -------------------------------------------------------------------

class PBNpcSceneInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBNpcSceneInfo) */ {
 public:
  PBNpcSceneInfo();
  virtual ~PBNpcSceneInfo();

  PBNpcSceneInfo(const PBNpcSceneInfo& from);

  inline PBNpcSceneInfo& operator=(const PBNpcSceneInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBNpcSceneInfo& default_instance();

  static const PBNpcSceneInfo* internal_default_instance();

  void Swap(PBNpcSceneInfo* other);

  // implements Message ----------------------------------------------

  inline PBNpcSceneInfo* New() const { return New(NULL); }

  PBNpcSceneInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBNpcSceneInfo& from);
  void MergeFrom(const PBNpcSceneInfo& from);
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
  void InternalSwap(PBNpcSceneInfo* other);
  void UnsafeMergeFrom(const PBNpcSceneInfo& from);
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

  // optional uint32 EntityID = 1;
  void clear_entityid();
  static const int kEntityIDFieldNumber = 1;
  ::google::protobuf::uint32 entityid() const;
  void set_entityid(::google::protobuf::uint32 value);

  // optional uint32 TempID = 2;
  void clear_tempid();
  static const int kTempIDFieldNumber = 2;
  ::google::protobuf::uint32 tempid() const;
  void set_tempid(::google::protobuf::uint32 value);

  // optional uint32 PosX = 3;
  void clear_posx();
  static const int kPosXFieldNumber = 3;
  ::google::protobuf::uint32 posx() const;
  void set_posx(::google::protobuf::uint32 value);

  // optional uint32 PosY = 4;
  void clear_posy();
  static const int kPosYFieldNumber = 4;
  ::google::protobuf::uint32 posy() const;
  void set_posy(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:PBNpcSceneInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 entityid_;
  ::google::protobuf::uint32 tempid_;
  ::google::protobuf::uint32 posx_;
  ::google::protobuf::uint32 posy_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_mapmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_mapmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_mapmodule_2ehxx();
  friend void protobuf_ShutdownFile_mapmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBNpcSceneInfo> PBNpcSceneInfo_default_instance_;

// -------------------------------------------------------------------

class CMessageCreateNPCListNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageCreateNPCListNotify) */ {
 public:
  CMessageCreateNPCListNotify();
  virtual ~CMessageCreateNPCListNotify();

  CMessageCreateNPCListNotify(const CMessageCreateNPCListNotify& from);

  inline CMessageCreateNPCListNotify& operator=(const CMessageCreateNPCListNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageCreateNPCListNotify& default_instance();

  static const CMessageCreateNPCListNotify* internal_default_instance();

  void Swap(CMessageCreateNPCListNotify* other);

  // implements Message ----------------------------------------------

  inline CMessageCreateNPCListNotify* New() const { return New(NULL); }

  CMessageCreateNPCListNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageCreateNPCListNotify& from);
  void MergeFrom(const CMessageCreateNPCListNotify& from);
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
  void InternalSwap(CMessageCreateNPCListNotify* other);
  void UnsafeMergeFrom(const CMessageCreateNPCListNotify& from);
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

  // repeated .PBNpcSceneInfo NPCInfo = 1;
  int npcinfo_size() const;
  void clear_npcinfo();
  static const int kNPCInfoFieldNumber = 1;
  const ::PBNpcSceneInfo& npcinfo(int index) const;
  ::PBNpcSceneInfo* mutable_npcinfo(int index);
  ::PBNpcSceneInfo* add_npcinfo();
  ::google::protobuf::RepeatedPtrField< ::PBNpcSceneInfo >*
      mutable_npcinfo();
  const ::google::protobuf::RepeatedPtrField< ::PBNpcSceneInfo >&
      npcinfo() const;

  // @@protoc_insertion_point(class_scope:CMessageCreateNPCListNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::PBNpcSceneInfo > npcinfo_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_mapmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_mapmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_mapmodule_2ehxx();
  friend void protobuf_ShutdownFile_mapmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageCreateNPCListNotify> CMessageCreateNPCListNotify_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CMessageEntityMoveNotify

// optional uint32 EntityID = 1;
inline void CMessageEntityMoveNotify::clear_entityid() {
  entityid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageEntityMoveNotify::entityid() const {
  // @@protoc_insertion_point(field_get:CMessageEntityMoveNotify.EntityID)
  return entityid_;
}
inline void CMessageEntityMoveNotify::set_entityid(::google::protobuf::uint32 value) {
  
  entityid_ = value;
  // @@protoc_insertion_point(field_set:CMessageEntityMoveNotify.EntityID)
}

// optional uint32 DesPosX = 2;
inline void CMessageEntityMoveNotify::clear_desposx() {
  desposx_ = 0u;
}
inline ::google::protobuf::uint32 CMessageEntityMoveNotify::desposx() const {
  // @@protoc_insertion_point(field_get:CMessageEntityMoveNotify.DesPosX)
  return desposx_;
}
inline void CMessageEntityMoveNotify::set_desposx(::google::protobuf::uint32 value) {
  
  desposx_ = value;
  // @@protoc_insertion_point(field_set:CMessageEntityMoveNotify.DesPosX)
}

// optional uint32 DesPosY = 3;
inline void CMessageEntityMoveNotify::clear_desposy() {
  desposy_ = 0u;
}
inline ::google::protobuf::uint32 CMessageEntityMoveNotify::desposy() const {
  // @@protoc_insertion_point(field_get:CMessageEntityMoveNotify.DesPosY)
  return desposy_;
}
inline void CMessageEntityMoveNotify::set_desposy(::google::protobuf::uint32 value) {
  
  desposy_ = value;
  // @@protoc_insertion_point(field_set:CMessageEntityMoveNotify.DesPosY)
}

inline const CMessageEntityMoveNotify* CMessageEntityMoveNotify::internal_default_instance() {
  return &CMessageEntityMoveNotify_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessagePlayerSceneInfo

// optional uint32 EntityID = 1;
inline void CMessagePlayerSceneInfo::clear_entityid() {
  entityid_ = 0u;
}
inline ::google::protobuf::uint32 CMessagePlayerSceneInfo::entityid() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerSceneInfo.EntityID)
  return entityid_;
}
inline void CMessagePlayerSceneInfo::set_entityid(::google::protobuf::uint32 value) {
  
  entityid_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerSceneInfo.EntityID)
}

// optional uint32 PosX = 2;
inline void CMessagePlayerSceneInfo::clear_posx() {
  posx_ = 0u;
}
inline ::google::protobuf::uint32 CMessagePlayerSceneInfo::posx() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerSceneInfo.PosX)
  return posx_;
}
inline void CMessagePlayerSceneInfo::set_posx(::google::protobuf::uint32 value) {
  
  posx_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerSceneInfo.PosX)
}

// optional uint32 PosY = 3;
inline void CMessagePlayerSceneInfo::clear_posy() {
  posy_ = 0u;
}
inline ::google::protobuf::uint32 CMessagePlayerSceneInfo::posy() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerSceneInfo.PosY)
  return posy_;
}
inline void CMessagePlayerSceneInfo::set_posy(::google::protobuf::uint32 value) {
  
  posy_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerSceneInfo.PosY)
}

inline const CMessagePlayerSceneInfo* CMessagePlayerSceneInfo::internal_default_instance() {
  return &CMessagePlayerSceneInfo_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageCreatePlayerListNotify

inline const CMessageCreatePlayerListNotify* CMessageCreatePlayerListNotify::internal_default_instance() {
  return &CMessageCreatePlayerListNotify_default_instance_.get();
}
// -------------------------------------------------------------------

// PBNpcSceneInfo

// optional uint32 EntityID = 1;
inline void PBNpcSceneInfo::clear_entityid() {
  entityid_ = 0u;
}
inline ::google::protobuf::uint32 PBNpcSceneInfo::entityid() const {
  // @@protoc_insertion_point(field_get:PBNpcSceneInfo.EntityID)
  return entityid_;
}
inline void PBNpcSceneInfo::set_entityid(::google::protobuf::uint32 value) {
  
  entityid_ = value;
  // @@protoc_insertion_point(field_set:PBNpcSceneInfo.EntityID)
}

// optional uint32 TempID = 2;
inline void PBNpcSceneInfo::clear_tempid() {
  tempid_ = 0u;
}
inline ::google::protobuf::uint32 PBNpcSceneInfo::tempid() const {
  // @@protoc_insertion_point(field_get:PBNpcSceneInfo.TempID)
  return tempid_;
}
inline void PBNpcSceneInfo::set_tempid(::google::protobuf::uint32 value) {
  
  tempid_ = value;
  // @@protoc_insertion_point(field_set:PBNpcSceneInfo.TempID)
}

// optional uint32 PosX = 3;
inline void PBNpcSceneInfo::clear_posx() {
  posx_ = 0u;
}
inline ::google::protobuf::uint32 PBNpcSceneInfo::posx() const {
  // @@protoc_insertion_point(field_get:PBNpcSceneInfo.PosX)
  return posx_;
}
inline void PBNpcSceneInfo::set_posx(::google::protobuf::uint32 value) {
  
  posx_ = value;
  // @@protoc_insertion_point(field_set:PBNpcSceneInfo.PosX)
}

// optional uint32 PosY = 4;
inline void PBNpcSceneInfo::clear_posy() {
  posy_ = 0u;
}
inline ::google::protobuf::uint32 PBNpcSceneInfo::posy() const {
  // @@protoc_insertion_point(field_get:PBNpcSceneInfo.PosY)
  return posy_;
}
inline void PBNpcSceneInfo::set_posy(::google::protobuf::uint32 value) {
  
  posy_ = value;
  // @@protoc_insertion_point(field_set:PBNpcSceneInfo.PosY)
}

inline const PBNpcSceneInfo* PBNpcSceneInfo::internal_default_instance() {
  return &PBNpcSceneInfo_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageCreateNPCListNotify

// repeated .PBNpcSceneInfo NPCInfo = 1;
inline int CMessageCreateNPCListNotify::npcinfo_size() const {
  return npcinfo_.size();
}
inline void CMessageCreateNPCListNotify::clear_npcinfo() {
  npcinfo_.Clear();
}
inline const ::PBNpcSceneInfo& CMessageCreateNPCListNotify::npcinfo(int index) const {
  // @@protoc_insertion_point(field_get:CMessageCreateNPCListNotify.NPCInfo)
  return npcinfo_.Get(index);
}
inline ::PBNpcSceneInfo* CMessageCreateNPCListNotify::mutable_npcinfo(int index) {
  // @@protoc_insertion_point(field_mutable:CMessageCreateNPCListNotify.NPCInfo)
  return npcinfo_.Mutable(index);
}
inline ::PBNpcSceneInfo* CMessageCreateNPCListNotify::add_npcinfo() {
  // @@protoc_insertion_point(field_add:CMessageCreateNPCListNotify.NPCInfo)
  return npcinfo_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::PBNpcSceneInfo >*
CMessageCreateNPCListNotify::mutable_npcinfo() {
  // @@protoc_insertion_point(field_mutable_list:CMessageCreateNPCListNotify.NPCInfo)
  return &npcinfo_;
}
inline const ::google::protobuf::RepeatedPtrField< ::PBNpcSceneInfo >&
CMessageCreateNPCListNotify::npcinfo() const {
  // @@protoc_insertion_point(field_list:CMessageCreateNPCListNotify.NPCInfo)
  return npcinfo_;
}

inline const CMessageCreateNPCListNotify* CMessageCreateNPCListNotify::internal_default_instance() {
  return &CMessageCreateNPCListNotify_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::MAP_MODULE_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::MAP_MODULE_MSG_ID>() {
  return ::MAP_MODULE_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_mapmodule_2ehxx__INCLUDED
