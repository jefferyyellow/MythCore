// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: dbmodule.hxx

#ifndef PROTOBUF_dbmodule_2ehxx__INCLUDED
#define PROTOBUF_dbmodule_2ehxx__INCLUDED

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
#include "common.hxx.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_dbmodule_2ehxx();
void protobuf_InitDefaults_dbmodule_2ehxx();
void protobuf_AssignDesc_dbmodule_2ehxx();
void protobuf_ShutdownFile_dbmodule_2ehxx();

class PBSavePlayer;

// ===================================================================

class PBSavePlayer : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBSavePlayer) */ {
 public:
  PBSavePlayer();
  virtual ~PBSavePlayer();

  PBSavePlayer(const PBSavePlayer& from);

  inline PBSavePlayer& operator=(const PBSavePlayer& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBSavePlayer& default_instance();

  static const PBSavePlayer* internal_default_instance();

  void Swap(PBSavePlayer* other);

  // implements Message ----------------------------------------------

  inline PBSavePlayer* New() const { return New(NULL); }

  PBSavePlayer* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBSavePlayer& from);
  void MergeFrom(const PBSavePlayer& from);
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
  void InternalSwap(PBSavePlayer* other);
  void UnsafeMergeFrom(const PBSavePlayer& from);
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

  // optional .PBItemList bag = 1;
  bool has_bag() const;
  void clear_bag();
  static const int kBagFieldNumber = 1;
  const ::PBItemList& bag() const;
  ::PBItemList* mutable_bag();
  ::PBItemList* release_bag();
  void set_allocated_bag(::PBItemList* bag);

  // optional .PBItemList equip = 2;
  bool has_equip() const;
  void clear_equip();
  static const int kEquipFieldNumber = 2;
  const ::PBItemList& equip() const;
  ::PBItemList* mutable_equip();
  ::PBItemList* release_equip();
  void set_allocated_equip(::PBItemList* equip);

  // optional .PBTaskList task = 3;
  bool has_task() const;
  void clear_task();
  static const int kTaskFieldNumber = 3;
  const ::PBTaskList& task() const;
  ::PBTaskList* mutable_task();
  ::PBTaskList* release_task();
  void set_allocated_task(::PBTaskList* task);

  // @@protoc_insertion_point(class_scope:PBSavePlayer)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::PBItemList* bag_;
  ::PBItemList* equip_;
  ::PBTaskList* task_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_dbmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_dbmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_dbmodule_2ehxx();
  friend void protobuf_ShutdownFile_dbmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBSavePlayer> PBSavePlayer_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// PBSavePlayer

// optional .PBItemList bag = 1;
inline bool PBSavePlayer::has_bag() const {
  return this != internal_default_instance() && bag_ != NULL;
}
inline void PBSavePlayer::clear_bag() {
  if (GetArenaNoVirtual() == NULL && bag_ != NULL) delete bag_;
  bag_ = NULL;
}
inline const ::PBItemList& PBSavePlayer::bag() const {
  // @@protoc_insertion_point(field_get:PBSavePlayer.bag)
  return bag_ != NULL ? *bag_
                         : *::PBItemList::internal_default_instance();
}
inline ::PBItemList* PBSavePlayer::mutable_bag() {
  
  if (bag_ == NULL) {
    bag_ = new ::PBItemList;
  }
  // @@protoc_insertion_point(field_mutable:PBSavePlayer.bag)
  return bag_;
}
inline ::PBItemList* PBSavePlayer::release_bag() {
  // @@protoc_insertion_point(field_release:PBSavePlayer.bag)
  
  ::PBItemList* temp = bag_;
  bag_ = NULL;
  return temp;
}
inline void PBSavePlayer::set_allocated_bag(::PBItemList* bag) {
  delete bag_;
  bag_ = bag;
  if (bag) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:PBSavePlayer.bag)
}

// optional .PBItemList equip = 2;
inline bool PBSavePlayer::has_equip() const {
  return this != internal_default_instance() && equip_ != NULL;
}
inline void PBSavePlayer::clear_equip() {
  if (GetArenaNoVirtual() == NULL && equip_ != NULL) delete equip_;
  equip_ = NULL;
}
inline const ::PBItemList& PBSavePlayer::equip() const {
  // @@protoc_insertion_point(field_get:PBSavePlayer.equip)
  return equip_ != NULL ? *equip_
                         : *::PBItemList::internal_default_instance();
}
inline ::PBItemList* PBSavePlayer::mutable_equip() {
  
  if (equip_ == NULL) {
    equip_ = new ::PBItemList;
  }
  // @@protoc_insertion_point(field_mutable:PBSavePlayer.equip)
  return equip_;
}
inline ::PBItemList* PBSavePlayer::release_equip() {
  // @@protoc_insertion_point(field_release:PBSavePlayer.equip)
  
  ::PBItemList* temp = equip_;
  equip_ = NULL;
  return temp;
}
inline void PBSavePlayer::set_allocated_equip(::PBItemList* equip) {
  delete equip_;
  equip_ = equip;
  if (equip) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:PBSavePlayer.equip)
}

// optional .PBTaskList task = 3;
inline bool PBSavePlayer::has_task() const {
  return this != internal_default_instance() && task_ != NULL;
}
inline void PBSavePlayer::clear_task() {
  if (GetArenaNoVirtual() == NULL && task_ != NULL) delete task_;
  task_ = NULL;
}
inline const ::PBTaskList& PBSavePlayer::task() const {
  // @@protoc_insertion_point(field_get:PBSavePlayer.task)
  return task_ != NULL ? *task_
                         : *::PBTaskList::internal_default_instance();
}
inline ::PBTaskList* PBSavePlayer::mutable_task() {
  
  if (task_ == NULL) {
    task_ = new ::PBTaskList;
  }
  // @@protoc_insertion_point(field_mutable:PBSavePlayer.task)
  return task_;
}
inline ::PBTaskList* PBSavePlayer::release_task() {
  // @@protoc_insertion_point(field_release:PBSavePlayer.task)
  
  ::PBTaskList* temp = task_;
  task_ = NULL;
  return temp;
}
inline void PBSavePlayer::set_allocated_task(::PBTaskList* task) {
  delete task_;
  task_ = task;
  if (task) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:PBSavePlayer.task)
}

inline const PBSavePlayer* PBSavePlayer::internal_default_instance() {
  return &PBSavePlayer_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_dbmodule_2ehxx__INCLUDED