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

class PBGameEvent;
class PBGameEventParam;
class PBGlobalMail;
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

// -------------------------------------------------------------------

class PBGlobalMail : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBGlobalMail) */ {
 public:
  PBGlobalMail();
  virtual ~PBGlobalMail();

  PBGlobalMail(const PBGlobalMail& from);

  inline PBGlobalMail& operator=(const PBGlobalMail& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBGlobalMail& default_instance();

  static const PBGlobalMail* internal_default_instance();

  void Swap(PBGlobalMail* other);

  // implements Message ----------------------------------------------

  inline PBGlobalMail* New() const { return New(NULL); }

  PBGlobalMail* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBGlobalMail& from);
  void MergeFrom(const PBGlobalMail& from);
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
  void InternalSwap(PBGlobalMail* other);
  void UnsafeMergeFrom(const PBGlobalMail& from);
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

  // optional uint32 mail_id = 1;
  void clear_mail_id();
  static const int kMailIdFieldNumber = 1;
  ::google::protobuf::uint32 mail_id() const;
  void set_mail_id(::google::protobuf::uint32 value);

  // optional uint32 mail_type = 2;
  void clear_mail_type();
  static const int kMailTypeFieldNumber = 2;
  ::google::protobuf::uint32 mail_type() const;
  void set_mail_type(::google::protobuf::uint32 value);

  // optional uint32 create_time = 3;
  void clear_create_time();
  static const int kCreateTimeFieldNumber = 3;
  ::google::protobuf::uint32 create_time() const;
  void set_create_time(::google::protobuf::uint32 value);

  // optional string mail_title = 4;
  void clear_mail_title();
  static const int kMailTitleFieldNumber = 4;
  const ::std::string& mail_title() const;
  void set_mail_title(const ::std::string& value);
  void set_mail_title(const char* value);
  void set_mail_title(const char* value, size_t size);
  ::std::string* mutable_mail_title();
  ::std::string* release_mail_title();
  void set_allocated_mail_title(::std::string* mail_title);

  // optional string mail_body = 5;
  void clear_mail_body();
  static const int kMailBodyFieldNumber = 5;
  const ::std::string& mail_body() const;
  void set_mail_body(const ::std::string& value);
  void set_mail_body(const char* value);
  void set_mail_body(const char* value, size_t size);
  ::std::string* mutable_mail_body();
  ::std::string* release_mail_body();
  void set_allocated_mail_body(::std::string* mail_body);

  // optional .PBMailItemList mail_item = 6;
  bool has_mail_item() const;
  void clear_mail_item();
  static const int kMailItemFieldNumber = 6;
  const ::PBMailItemList& mail_item() const;
  ::PBMailItemList* mutable_mail_item();
  ::PBMailItemList* release_mail_item();
  void set_allocated_mail_item(::PBMailItemList* mail_item);

  // @@protoc_insertion_point(class_scope:PBGlobalMail)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr mail_title_;
  ::google::protobuf::internal::ArenaStringPtr mail_body_;
  ::PBMailItemList* mail_item_;
  ::google::protobuf::uint32 mail_id_;
  ::google::protobuf::uint32 mail_type_;
  ::google::protobuf::uint32 create_time_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_dbmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_dbmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_dbmodule_2ehxx();
  friend void protobuf_ShutdownFile_dbmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBGlobalMail> PBGlobalMail_default_instance_;

// -------------------------------------------------------------------

class PBGameEventParam : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBGameEventParam) */ {
 public:
  PBGameEventParam();
  virtual ~PBGameEventParam();

  PBGameEventParam(const PBGameEventParam& from);

  inline PBGameEventParam& operator=(const PBGameEventParam& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBGameEventParam& default_instance();

  static const PBGameEventParam* internal_default_instance();

  void Swap(PBGameEventParam* other);

  // implements Message ----------------------------------------------

  inline PBGameEventParam* New() const { return New(NULL); }

  PBGameEventParam* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBGameEventParam& from);
  void MergeFrom(const PBGameEventParam& from);
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
  void InternalSwap(PBGameEventParam* other);
  void UnsafeMergeFrom(const PBGameEventParam& from);
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

  // repeated uint32 Param = 1;
  int param_size() const;
  void clear_param();
  static const int kParamFieldNumber = 1;
  ::google::protobuf::uint32 param(int index) const;
  void set_param(int index, ::google::protobuf::uint32 value);
  void add_param(::google::protobuf::uint32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      param() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_param();

  // repeated string StrParam = 2;
  int strparam_size() const;
  void clear_strparam();
  static const int kStrParamFieldNumber = 2;
  const ::std::string& strparam(int index) const;
  ::std::string* mutable_strparam(int index);
  void set_strparam(int index, const ::std::string& value);
  void set_strparam(int index, const char* value);
  void set_strparam(int index, const char* value, size_t size);
  ::std::string* add_strparam();
  void add_strparam(const ::std::string& value);
  void add_strparam(const char* value);
  void add_strparam(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& strparam() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_strparam();

  // @@protoc_insertion_point(class_scope:PBGameEventParam)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > param_;
  mutable int _param_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::std::string> strparam_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_dbmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_dbmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_dbmodule_2ehxx();
  friend void protobuf_ShutdownFile_dbmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBGameEventParam> PBGameEventParam_default_instance_;

// -------------------------------------------------------------------

class PBGameEvent : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBGameEvent) */ {
 public:
  PBGameEvent();
  virtual ~PBGameEvent();

  PBGameEvent(const PBGameEvent& from);

  inline PBGameEvent& operator=(const PBGameEvent& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBGameEvent& default_instance();

  static const PBGameEvent* internal_default_instance();

  void Swap(PBGameEvent* other);

  // implements Message ----------------------------------------------

  inline PBGameEvent* New() const { return New(NULL); }

  PBGameEvent* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBGameEvent& from);
  void MergeFrom(const PBGameEvent& from);
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
  void InternalSwap(PBGameEvent* other);
  void UnsafeMergeFrom(const PBGameEvent& from);
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

  // optional uint32 event_id = 1;
  void clear_event_id();
  static const int kEventIdFieldNumber = 1;
  ::google::protobuf::uint32 event_id() const;
  void set_event_id(::google::protobuf::uint32 value);

  // optional uint32 event_type = 2;
  void clear_event_type();
  static const int kEventTypeFieldNumber = 2;
  ::google::protobuf::uint32 event_type() const;
  void set_event_type(::google::protobuf::uint32 value);

  // optional uint32 role_id = 3;
  void clear_role_id();
  static const int kRoleIdFieldNumber = 3;
  ::google::protobuf::uint32 role_id() const;
  void set_role_id(::google::protobuf::uint32 value);

  // optional uint32 create_time = 4;
  void clear_create_time();
  static const int kCreateTimeFieldNumber = 4;
  ::google::protobuf::uint32 create_time() const;
  void set_create_time(::google::protobuf::uint32 value);

  // optional .PBGameEventParam event_param = 5;
  bool has_event_param() const;
  void clear_event_param();
  static const int kEventParamFieldNumber = 5;
  const ::PBGameEventParam& event_param() const;
  ::PBGameEventParam* mutable_event_param();
  ::PBGameEventParam* release_event_param();
  void set_allocated_event_param(::PBGameEventParam* event_param);

  // @@protoc_insertion_point(class_scope:PBGameEvent)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::PBGameEventParam* event_param_;
  ::google::protobuf::uint32 event_id_;
  ::google::protobuf::uint32 event_type_;
  ::google::protobuf::uint32 role_id_;
  ::google::protobuf::uint32 create_time_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_dbmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_dbmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_dbmodule_2ehxx();
  friend void protobuf_ShutdownFile_dbmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBGameEvent> PBGameEvent_default_instance_;

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
// -------------------------------------------------------------------

// PBGlobalMail

// optional uint32 mail_id = 1;
inline void PBGlobalMail::clear_mail_id() {
  mail_id_ = 0u;
}
inline ::google::protobuf::uint32 PBGlobalMail::mail_id() const {
  // @@protoc_insertion_point(field_get:PBGlobalMail.mail_id)
  return mail_id_;
}
inline void PBGlobalMail::set_mail_id(::google::protobuf::uint32 value) {
  
  mail_id_ = value;
  // @@protoc_insertion_point(field_set:PBGlobalMail.mail_id)
}

// optional uint32 mail_type = 2;
inline void PBGlobalMail::clear_mail_type() {
  mail_type_ = 0u;
}
inline ::google::protobuf::uint32 PBGlobalMail::mail_type() const {
  // @@protoc_insertion_point(field_get:PBGlobalMail.mail_type)
  return mail_type_;
}
inline void PBGlobalMail::set_mail_type(::google::protobuf::uint32 value) {
  
  mail_type_ = value;
  // @@protoc_insertion_point(field_set:PBGlobalMail.mail_type)
}

// optional uint32 create_time = 3;
inline void PBGlobalMail::clear_create_time() {
  create_time_ = 0u;
}
inline ::google::protobuf::uint32 PBGlobalMail::create_time() const {
  // @@protoc_insertion_point(field_get:PBGlobalMail.create_time)
  return create_time_;
}
inline void PBGlobalMail::set_create_time(::google::protobuf::uint32 value) {
  
  create_time_ = value;
  // @@protoc_insertion_point(field_set:PBGlobalMail.create_time)
}

// optional string mail_title = 4;
inline void PBGlobalMail::clear_mail_title() {
  mail_title_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PBGlobalMail::mail_title() const {
  // @@protoc_insertion_point(field_get:PBGlobalMail.mail_title)
  return mail_title_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PBGlobalMail::set_mail_title(const ::std::string& value) {
  
  mail_title_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:PBGlobalMail.mail_title)
}
inline void PBGlobalMail::set_mail_title(const char* value) {
  
  mail_title_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PBGlobalMail.mail_title)
}
inline void PBGlobalMail::set_mail_title(const char* value, size_t size) {
  
  mail_title_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PBGlobalMail.mail_title)
}
inline ::std::string* PBGlobalMail::mutable_mail_title() {
  
  // @@protoc_insertion_point(field_mutable:PBGlobalMail.mail_title)
  return mail_title_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PBGlobalMail::release_mail_title() {
  // @@protoc_insertion_point(field_release:PBGlobalMail.mail_title)
  
  return mail_title_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PBGlobalMail::set_allocated_mail_title(::std::string* mail_title) {
  if (mail_title != NULL) {
    
  } else {
    
  }
  mail_title_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), mail_title);
  // @@protoc_insertion_point(field_set_allocated:PBGlobalMail.mail_title)
}

// optional string mail_body = 5;
inline void PBGlobalMail::clear_mail_body() {
  mail_body_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PBGlobalMail::mail_body() const {
  // @@protoc_insertion_point(field_get:PBGlobalMail.mail_body)
  return mail_body_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PBGlobalMail::set_mail_body(const ::std::string& value) {
  
  mail_body_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:PBGlobalMail.mail_body)
}
inline void PBGlobalMail::set_mail_body(const char* value) {
  
  mail_body_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PBGlobalMail.mail_body)
}
inline void PBGlobalMail::set_mail_body(const char* value, size_t size) {
  
  mail_body_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PBGlobalMail.mail_body)
}
inline ::std::string* PBGlobalMail::mutable_mail_body() {
  
  // @@protoc_insertion_point(field_mutable:PBGlobalMail.mail_body)
  return mail_body_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PBGlobalMail::release_mail_body() {
  // @@protoc_insertion_point(field_release:PBGlobalMail.mail_body)
  
  return mail_body_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PBGlobalMail::set_allocated_mail_body(::std::string* mail_body) {
  if (mail_body != NULL) {
    
  } else {
    
  }
  mail_body_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), mail_body);
  // @@protoc_insertion_point(field_set_allocated:PBGlobalMail.mail_body)
}

// optional .PBMailItemList mail_item = 6;
inline bool PBGlobalMail::has_mail_item() const {
  return this != internal_default_instance() && mail_item_ != NULL;
}
inline void PBGlobalMail::clear_mail_item() {
  if (GetArenaNoVirtual() == NULL && mail_item_ != NULL) delete mail_item_;
  mail_item_ = NULL;
}
inline const ::PBMailItemList& PBGlobalMail::mail_item() const {
  // @@protoc_insertion_point(field_get:PBGlobalMail.mail_item)
  return mail_item_ != NULL ? *mail_item_
                         : *::PBMailItemList::internal_default_instance();
}
inline ::PBMailItemList* PBGlobalMail::mutable_mail_item() {
  
  if (mail_item_ == NULL) {
    mail_item_ = new ::PBMailItemList;
  }
  // @@protoc_insertion_point(field_mutable:PBGlobalMail.mail_item)
  return mail_item_;
}
inline ::PBMailItemList* PBGlobalMail::release_mail_item() {
  // @@protoc_insertion_point(field_release:PBGlobalMail.mail_item)
  
  ::PBMailItemList* temp = mail_item_;
  mail_item_ = NULL;
  return temp;
}
inline void PBGlobalMail::set_allocated_mail_item(::PBMailItemList* mail_item) {
  delete mail_item_;
  mail_item_ = mail_item;
  if (mail_item) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:PBGlobalMail.mail_item)
}

inline const PBGlobalMail* PBGlobalMail::internal_default_instance() {
  return &PBGlobalMail_default_instance_.get();
}
// -------------------------------------------------------------------

// PBGameEventParam

// repeated uint32 Param = 1;
inline int PBGameEventParam::param_size() const {
  return param_.size();
}
inline void PBGameEventParam::clear_param() {
  param_.Clear();
}
inline ::google::protobuf::uint32 PBGameEventParam::param(int index) const {
  // @@protoc_insertion_point(field_get:PBGameEventParam.Param)
  return param_.Get(index);
}
inline void PBGameEventParam::set_param(int index, ::google::protobuf::uint32 value) {
  param_.Set(index, value);
  // @@protoc_insertion_point(field_set:PBGameEventParam.Param)
}
inline void PBGameEventParam::add_param(::google::protobuf::uint32 value) {
  param_.Add(value);
  // @@protoc_insertion_point(field_add:PBGameEventParam.Param)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
PBGameEventParam::param() const {
  // @@protoc_insertion_point(field_list:PBGameEventParam.Param)
  return param_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
PBGameEventParam::mutable_param() {
  // @@protoc_insertion_point(field_mutable_list:PBGameEventParam.Param)
  return &param_;
}

// repeated string StrParam = 2;
inline int PBGameEventParam::strparam_size() const {
  return strparam_.size();
}
inline void PBGameEventParam::clear_strparam() {
  strparam_.Clear();
}
inline const ::std::string& PBGameEventParam::strparam(int index) const {
  // @@protoc_insertion_point(field_get:PBGameEventParam.StrParam)
  return strparam_.Get(index);
}
inline ::std::string* PBGameEventParam::mutable_strparam(int index) {
  // @@protoc_insertion_point(field_mutable:PBGameEventParam.StrParam)
  return strparam_.Mutable(index);
}
inline void PBGameEventParam::set_strparam(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:PBGameEventParam.StrParam)
  strparam_.Mutable(index)->assign(value);
}
inline void PBGameEventParam::set_strparam(int index, const char* value) {
  strparam_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:PBGameEventParam.StrParam)
}
inline void PBGameEventParam::set_strparam(int index, const char* value, size_t size) {
  strparam_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:PBGameEventParam.StrParam)
}
inline ::std::string* PBGameEventParam::add_strparam() {
  // @@protoc_insertion_point(field_add_mutable:PBGameEventParam.StrParam)
  return strparam_.Add();
}
inline void PBGameEventParam::add_strparam(const ::std::string& value) {
  strparam_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:PBGameEventParam.StrParam)
}
inline void PBGameEventParam::add_strparam(const char* value) {
  strparam_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:PBGameEventParam.StrParam)
}
inline void PBGameEventParam::add_strparam(const char* value, size_t size) {
  strparam_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:PBGameEventParam.StrParam)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
PBGameEventParam::strparam() const {
  // @@protoc_insertion_point(field_list:PBGameEventParam.StrParam)
  return strparam_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
PBGameEventParam::mutable_strparam() {
  // @@protoc_insertion_point(field_mutable_list:PBGameEventParam.StrParam)
  return &strparam_;
}

inline const PBGameEventParam* PBGameEventParam::internal_default_instance() {
  return &PBGameEventParam_default_instance_.get();
}
// -------------------------------------------------------------------

// PBGameEvent

// optional uint32 event_id = 1;
inline void PBGameEvent::clear_event_id() {
  event_id_ = 0u;
}
inline ::google::protobuf::uint32 PBGameEvent::event_id() const {
  // @@protoc_insertion_point(field_get:PBGameEvent.event_id)
  return event_id_;
}
inline void PBGameEvent::set_event_id(::google::protobuf::uint32 value) {
  
  event_id_ = value;
  // @@protoc_insertion_point(field_set:PBGameEvent.event_id)
}

// optional uint32 event_type = 2;
inline void PBGameEvent::clear_event_type() {
  event_type_ = 0u;
}
inline ::google::protobuf::uint32 PBGameEvent::event_type() const {
  // @@protoc_insertion_point(field_get:PBGameEvent.event_type)
  return event_type_;
}
inline void PBGameEvent::set_event_type(::google::protobuf::uint32 value) {
  
  event_type_ = value;
  // @@protoc_insertion_point(field_set:PBGameEvent.event_type)
}

// optional uint32 role_id = 3;
inline void PBGameEvent::clear_role_id() {
  role_id_ = 0u;
}
inline ::google::protobuf::uint32 PBGameEvent::role_id() const {
  // @@protoc_insertion_point(field_get:PBGameEvent.role_id)
  return role_id_;
}
inline void PBGameEvent::set_role_id(::google::protobuf::uint32 value) {
  
  role_id_ = value;
  // @@protoc_insertion_point(field_set:PBGameEvent.role_id)
}

// optional uint32 create_time = 4;
inline void PBGameEvent::clear_create_time() {
  create_time_ = 0u;
}
inline ::google::protobuf::uint32 PBGameEvent::create_time() const {
  // @@protoc_insertion_point(field_get:PBGameEvent.create_time)
  return create_time_;
}
inline void PBGameEvent::set_create_time(::google::protobuf::uint32 value) {
  
  create_time_ = value;
  // @@protoc_insertion_point(field_set:PBGameEvent.create_time)
}

// optional .PBGameEventParam event_param = 5;
inline bool PBGameEvent::has_event_param() const {
  return this != internal_default_instance() && event_param_ != NULL;
}
inline void PBGameEvent::clear_event_param() {
  if (GetArenaNoVirtual() == NULL && event_param_ != NULL) delete event_param_;
  event_param_ = NULL;
}
inline const ::PBGameEventParam& PBGameEvent::event_param() const {
  // @@protoc_insertion_point(field_get:PBGameEvent.event_param)
  return event_param_ != NULL ? *event_param_
                         : *::PBGameEventParam::internal_default_instance();
}
inline ::PBGameEventParam* PBGameEvent::mutable_event_param() {
  
  if (event_param_ == NULL) {
    event_param_ = new ::PBGameEventParam;
  }
  // @@protoc_insertion_point(field_mutable:PBGameEvent.event_param)
  return event_param_;
}
inline ::PBGameEventParam* PBGameEvent::release_event_param() {
  // @@protoc_insertion_point(field_release:PBGameEvent.event_param)
  
  ::PBGameEventParam* temp = event_param_;
  event_param_ = NULL;
  return temp;
}
inline void PBGameEvent::set_allocated_event_param(::PBGameEventParam* event_param) {
  delete event_param_;
  event_param_ = event_param;
  if (event_param) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:PBGameEvent.event_param)
}

inline const PBGameEvent* PBGameEvent::internal_default_instance() {
  return &PBGameEvent_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_dbmodule_2ehxx__INCLUDED
