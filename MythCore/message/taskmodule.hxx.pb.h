// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: taskmodule.hxx

#ifndef PROTOBUF_taskmodule_2ehxx__INCLUDED
#define PROTOBUF_taskmodule_2ehxx__INCLUDED

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
void protobuf_AddDesc_taskmodule_2ehxx();
void protobuf_InitDefaults_taskmodule_2ehxx();
void protobuf_AssignDesc_taskmodule_2ehxx();
void protobuf_ShutdownFile_taskmodule_2ehxx();

class CAbortTaskRequest;
class CAbortTaskResponse;
class CAcceptTaskRequest;
class CAcceptTaskResponse;
class CSubmitTaskRequest;
class CSubmitTaskResponse;
class CUpdateTaskProcessNotify;

enum TASK_MODULE_MSG_ID {
  ID_TASK_MODULE_ERROR = 0,
  ID_C2S_REQUEST_ACCEPT_TASK = 4096,
  ID_S2C_RESPONSE_ACCEPT_TASK = 4097,
  ID_C2S_REQUEST_SUBMIT_TASK = 4098,
  ID_S2C_RESPONSE_SUBMIT_TASK = 4099,
  ID_C2S_REQUEST_ABORT_TASK = 4100,
  ID_S2C_RESPONSE_ABORT_TASK = 4101,
  ID_S2C_NOTIFY_UPDATE_TASK_PROCESS = 4102,
  TASK_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  TASK_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool TASK_MODULE_MSG_ID_IsValid(int value);
const TASK_MODULE_MSG_ID TASK_MODULE_MSG_ID_MIN = ID_TASK_MODULE_ERROR;
const TASK_MODULE_MSG_ID TASK_MODULE_MSG_ID_MAX = ID_S2C_NOTIFY_UPDATE_TASK_PROCESS;
const int TASK_MODULE_MSG_ID_ARRAYSIZE = TASK_MODULE_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* TASK_MODULE_MSG_ID_descriptor();
inline const ::std::string& TASK_MODULE_MSG_ID_Name(TASK_MODULE_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    TASK_MODULE_MSG_ID_descriptor(), value);
}
inline bool TASK_MODULE_MSG_ID_Parse(
    const ::std::string& name, TASK_MODULE_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<TASK_MODULE_MSG_ID>(
    TASK_MODULE_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CAcceptTaskRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CAcceptTaskRequest) */ {
 public:
  CAcceptTaskRequest();
  virtual ~CAcceptTaskRequest();

  CAcceptTaskRequest(const CAcceptTaskRequest& from);

  inline CAcceptTaskRequest& operator=(const CAcceptTaskRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CAcceptTaskRequest& default_instance();

  static const CAcceptTaskRequest* internal_default_instance();

  void Swap(CAcceptTaskRequest* other);

  // implements Message ----------------------------------------------

  inline CAcceptTaskRequest* New() const { return New(NULL); }

  CAcceptTaskRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CAcceptTaskRequest& from);
  void MergeFrom(const CAcceptTaskRequest& from);
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
  void InternalSwap(CAcceptTaskRequest* other);
  void UnsafeMergeFrom(const CAcceptTaskRequest& from);
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

  // optional uint32 TaskID = 1;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 1;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // optional uint32 NpcEntityID = 2;
  void clear_npcentityid();
  static const int kNpcEntityIDFieldNumber = 2;
  ::google::protobuf::uint32 npcentityid() const;
  void set_npcentityid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CAcceptTaskRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 taskid_;
  ::google::protobuf::uint32 npcentityid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CAcceptTaskRequest> CAcceptTaskRequest_default_instance_;

// -------------------------------------------------------------------

class CAcceptTaskResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CAcceptTaskResponse) */ {
 public:
  CAcceptTaskResponse();
  virtual ~CAcceptTaskResponse();

  CAcceptTaskResponse(const CAcceptTaskResponse& from);

  inline CAcceptTaskResponse& operator=(const CAcceptTaskResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CAcceptTaskResponse& default_instance();

  static const CAcceptTaskResponse* internal_default_instance();

  void Swap(CAcceptTaskResponse* other);

  // implements Message ----------------------------------------------

  inline CAcceptTaskResponse* New() const { return New(NULL); }

  CAcceptTaskResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CAcceptTaskResponse& from);
  void MergeFrom(const CAcceptTaskResponse& from);
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
  void InternalSwap(CAcceptTaskResponse* other);
  void UnsafeMergeFrom(const CAcceptTaskResponse& from);
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

  // optional uint32 Result = 1;
  void clear_result();
  static const int kResultFieldNumber = 1;
  ::google::protobuf::uint32 result() const;
  void set_result(::google::protobuf::uint32 value);

  // optional uint32 TaskID = 2;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 2;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CAcceptTaskResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  ::google::protobuf::uint32 taskid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CAcceptTaskResponse> CAcceptTaskResponse_default_instance_;

// -------------------------------------------------------------------

class CSubmitTaskRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CSubmitTaskRequest) */ {
 public:
  CSubmitTaskRequest();
  virtual ~CSubmitTaskRequest();

  CSubmitTaskRequest(const CSubmitTaskRequest& from);

  inline CSubmitTaskRequest& operator=(const CSubmitTaskRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CSubmitTaskRequest& default_instance();

  static const CSubmitTaskRequest* internal_default_instance();

  void Swap(CSubmitTaskRequest* other);

  // implements Message ----------------------------------------------

  inline CSubmitTaskRequest* New() const { return New(NULL); }

  CSubmitTaskRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CSubmitTaskRequest& from);
  void MergeFrom(const CSubmitTaskRequest& from);
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
  void InternalSwap(CSubmitTaskRequest* other);
  void UnsafeMergeFrom(const CSubmitTaskRequest& from);
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

  // optional uint32 TaskID = 1;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 1;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // optional uint32 ItemIndex = 2;
  void clear_itemindex();
  static const int kItemIndexFieldNumber = 2;
  ::google::protobuf::uint32 itemindex() const;
  void set_itemindex(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CSubmitTaskRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 taskid_;
  ::google::protobuf::uint32 itemindex_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CSubmitTaskRequest> CSubmitTaskRequest_default_instance_;

// -------------------------------------------------------------------

class CSubmitTaskResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CSubmitTaskResponse) */ {
 public:
  CSubmitTaskResponse();
  virtual ~CSubmitTaskResponse();

  CSubmitTaskResponse(const CSubmitTaskResponse& from);

  inline CSubmitTaskResponse& operator=(const CSubmitTaskResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CSubmitTaskResponse& default_instance();

  static const CSubmitTaskResponse* internal_default_instance();

  void Swap(CSubmitTaskResponse* other);

  // implements Message ----------------------------------------------

  inline CSubmitTaskResponse* New() const { return New(NULL); }

  CSubmitTaskResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CSubmitTaskResponse& from);
  void MergeFrom(const CSubmitTaskResponse& from);
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
  void InternalSwap(CSubmitTaskResponse* other);
  void UnsafeMergeFrom(const CSubmitTaskResponse& from);
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

  // optional uint32 Result = 1;
  void clear_result();
  static const int kResultFieldNumber = 1;
  ::google::protobuf::uint32 result() const;
  void set_result(::google::protobuf::uint32 value);

  // optional uint32 TaskID = 2;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 2;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CSubmitTaskResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  ::google::protobuf::uint32 taskid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CSubmitTaskResponse> CSubmitTaskResponse_default_instance_;

// -------------------------------------------------------------------

class CAbortTaskRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CAbortTaskRequest) */ {
 public:
  CAbortTaskRequest();
  virtual ~CAbortTaskRequest();

  CAbortTaskRequest(const CAbortTaskRequest& from);

  inline CAbortTaskRequest& operator=(const CAbortTaskRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CAbortTaskRequest& default_instance();

  static const CAbortTaskRequest* internal_default_instance();

  void Swap(CAbortTaskRequest* other);

  // implements Message ----------------------------------------------

  inline CAbortTaskRequest* New() const { return New(NULL); }

  CAbortTaskRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CAbortTaskRequest& from);
  void MergeFrom(const CAbortTaskRequest& from);
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
  void InternalSwap(CAbortTaskRequest* other);
  void UnsafeMergeFrom(const CAbortTaskRequest& from);
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

  // optional uint32 TaskID = 1;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 1;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CAbortTaskRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 taskid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CAbortTaskRequest> CAbortTaskRequest_default_instance_;

// -------------------------------------------------------------------

class CAbortTaskResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CAbortTaskResponse) */ {
 public:
  CAbortTaskResponse();
  virtual ~CAbortTaskResponse();

  CAbortTaskResponse(const CAbortTaskResponse& from);

  inline CAbortTaskResponse& operator=(const CAbortTaskResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CAbortTaskResponse& default_instance();

  static const CAbortTaskResponse* internal_default_instance();

  void Swap(CAbortTaskResponse* other);

  // implements Message ----------------------------------------------

  inline CAbortTaskResponse* New() const { return New(NULL); }

  CAbortTaskResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CAbortTaskResponse& from);
  void MergeFrom(const CAbortTaskResponse& from);
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
  void InternalSwap(CAbortTaskResponse* other);
  void UnsafeMergeFrom(const CAbortTaskResponse& from);
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

  // optional uint32 Result = 1;
  void clear_result();
  static const int kResultFieldNumber = 1;
  ::google::protobuf::uint32 result() const;
  void set_result(::google::protobuf::uint32 value);

  // optional uint32 TaskID = 2;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 2;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CAbortTaskResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  ::google::protobuf::uint32 taskid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CAbortTaskResponse> CAbortTaskResponse_default_instance_;

// -------------------------------------------------------------------

class CUpdateTaskProcessNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CUpdateTaskProcessNotify) */ {
 public:
  CUpdateTaskProcessNotify();
  virtual ~CUpdateTaskProcessNotify();

  CUpdateTaskProcessNotify(const CUpdateTaskProcessNotify& from);

  inline CUpdateTaskProcessNotify& operator=(const CUpdateTaskProcessNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CUpdateTaskProcessNotify& default_instance();

  static const CUpdateTaskProcessNotify* internal_default_instance();

  void Swap(CUpdateTaskProcessNotify* other);

  // implements Message ----------------------------------------------

  inline CUpdateTaskProcessNotify* New() const { return New(NULL); }

  CUpdateTaskProcessNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CUpdateTaskProcessNotify& from);
  void MergeFrom(const CUpdateTaskProcessNotify& from);
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
  void InternalSwap(CUpdateTaskProcessNotify* other);
  void UnsafeMergeFrom(const CUpdateTaskProcessNotify& from);
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

  // optional uint32 TaskID = 1;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 1;
  ::google::protobuf::uint32 taskid() const;
  void set_taskid(::google::protobuf::uint32 value);

  // optional uint32 Param1 = 2;
  void clear_param1();
  static const int kParam1FieldNumber = 2;
  ::google::protobuf::uint32 param1() const;
  void set_param1(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CUpdateTaskProcessNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 taskid_;
  ::google::protobuf::uint32 param1_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_taskmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_taskmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_taskmodule_2ehxx();
  friend void protobuf_ShutdownFile_taskmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CUpdateTaskProcessNotify> CUpdateTaskProcessNotify_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CAcceptTaskRequest

// optional uint32 TaskID = 1;
inline void CAcceptTaskRequest::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CAcceptTaskRequest::taskid() const {
  // @@protoc_insertion_point(field_get:CAcceptTaskRequest.TaskID)
  return taskid_;
}
inline void CAcceptTaskRequest::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CAcceptTaskRequest.TaskID)
}

// optional uint32 NpcEntityID = 2;
inline void CAcceptTaskRequest::clear_npcentityid() {
  npcentityid_ = 0u;
}
inline ::google::protobuf::uint32 CAcceptTaskRequest::npcentityid() const {
  // @@protoc_insertion_point(field_get:CAcceptTaskRequest.NpcEntityID)
  return npcentityid_;
}
inline void CAcceptTaskRequest::set_npcentityid(::google::protobuf::uint32 value) {
  
  npcentityid_ = value;
  // @@protoc_insertion_point(field_set:CAcceptTaskRequest.NpcEntityID)
}

inline const CAcceptTaskRequest* CAcceptTaskRequest::internal_default_instance() {
  return &CAcceptTaskRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CAcceptTaskResponse

// optional uint32 Result = 1;
inline void CAcceptTaskResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CAcceptTaskResponse::result() const {
  // @@protoc_insertion_point(field_get:CAcceptTaskResponse.Result)
  return result_;
}
inline void CAcceptTaskResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CAcceptTaskResponse.Result)
}

// optional uint32 TaskID = 2;
inline void CAcceptTaskResponse::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CAcceptTaskResponse::taskid() const {
  // @@protoc_insertion_point(field_get:CAcceptTaskResponse.TaskID)
  return taskid_;
}
inline void CAcceptTaskResponse::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CAcceptTaskResponse.TaskID)
}

inline const CAcceptTaskResponse* CAcceptTaskResponse::internal_default_instance() {
  return &CAcceptTaskResponse_default_instance_.get();
}
// -------------------------------------------------------------------

// CSubmitTaskRequest

// optional uint32 TaskID = 1;
inline void CSubmitTaskRequest::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CSubmitTaskRequest::taskid() const {
  // @@protoc_insertion_point(field_get:CSubmitTaskRequest.TaskID)
  return taskid_;
}
inline void CSubmitTaskRequest::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CSubmitTaskRequest.TaskID)
}

// optional uint32 ItemIndex = 2;
inline void CSubmitTaskRequest::clear_itemindex() {
  itemindex_ = 0u;
}
inline ::google::protobuf::uint32 CSubmitTaskRequest::itemindex() const {
  // @@protoc_insertion_point(field_get:CSubmitTaskRequest.ItemIndex)
  return itemindex_;
}
inline void CSubmitTaskRequest::set_itemindex(::google::protobuf::uint32 value) {
  
  itemindex_ = value;
  // @@protoc_insertion_point(field_set:CSubmitTaskRequest.ItemIndex)
}

inline const CSubmitTaskRequest* CSubmitTaskRequest::internal_default_instance() {
  return &CSubmitTaskRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CSubmitTaskResponse

// optional uint32 Result = 1;
inline void CSubmitTaskResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CSubmitTaskResponse::result() const {
  // @@protoc_insertion_point(field_get:CSubmitTaskResponse.Result)
  return result_;
}
inline void CSubmitTaskResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CSubmitTaskResponse.Result)
}

// optional uint32 TaskID = 2;
inline void CSubmitTaskResponse::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CSubmitTaskResponse::taskid() const {
  // @@protoc_insertion_point(field_get:CSubmitTaskResponse.TaskID)
  return taskid_;
}
inline void CSubmitTaskResponse::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CSubmitTaskResponse.TaskID)
}

inline const CSubmitTaskResponse* CSubmitTaskResponse::internal_default_instance() {
  return &CSubmitTaskResponse_default_instance_.get();
}
// -------------------------------------------------------------------

// CAbortTaskRequest

// optional uint32 TaskID = 1;
inline void CAbortTaskRequest::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CAbortTaskRequest::taskid() const {
  // @@protoc_insertion_point(field_get:CAbortTaskRequest.TaskID)
  return taskid_;
}
inline void CAbortTaskRequest::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CAbortTaskRequest.TaskID)
}

inline const CAbortTaskRequest* CAbortTaskRequest::internal_default_instance() {
  return &CAbortTaskRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CAbortTaskResponse

// optional uint32 Result = 1;
inline void CAbortTaskResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CAbortTaskResponse::result() const {
  // @@protoc_insertion_point(field_get:CAbortTaskResponse.Result)
  return result_;
}
inline void CAbortTaskResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CAbortTaskResponse.Result)
}

// optional uint32 TaskID = 2;
inline void CAbortTaskResponse::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CAbortTaskResponse::taskid() const {
  // @@protoc_insertion_point(field_get:CAbortTaskResponse.TaskID)
  return taskid_;
}
inline void CAbortTaskResponse::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CAbortTaskResponse.TaskID)
}

inline const CAbortTaskResponse* CAbortTaskResponse::internal_default_instance() {
  return &CAbortTaskResponse_default_instance_.get();
}
// -------------------------------------------------------------------

// CUpdateTaskProcessNotify

// optional uint32 TaskID = 1;
inline void CUpdateTaskProcessNotify::clear_taskid() {
  taskid_ = 0u;
}
inline ::google::protobuf::uint32 CUpdateTaskProcessNotify::taskid() const {
  // @@protoc_insertion_point(field_get:CUpdateTaskProcessNotify.TaskID)
  return taskid_;
}
inline void CUpdateTaskProcessNotify::set_taskid(::google::protobuf::uint32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:CUpdateTaskProcessNotify.TaskID)
}

// optional uint32 Param1 = 2;
inline void CUpdateTaskProcessNotify::clear_param1() {
  param1_ = 0u;
}
inline ::google::protobuf::uint32 CUpdateTaskProcessNotify::param1() const {
  // @@protoc_insertion_point(field_get:CUpdateTaskProcessNotify.Param1)
  return param1_;
}
inline void CUpdateTaskProcessNotify::set_param1(::google::protobuf::uint32 value) {
  
  param1_ = value;
  // @@protoc_insertion_point(field_set:CUpdateTaskProcessNotify.Param1)
}

inline const CUpdateTaskProcessNotify* CUpdateTaskProcessNotify::internal_default_instance() {
  return &CUpdateTaskProcessNotify_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::TASK_MODULE_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::TASK_MODULE_MSG_ID>() {
  return ::TASK_MODULE_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_taskmodule_2ehxx__INCLUDED
