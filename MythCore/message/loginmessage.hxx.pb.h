// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: loginmessage.hxx

#ifndef PROTOBUF_loginmessage_2ehxx__INCLUDED
#define PROTOBUF_loginmessage_2ehxx__INCLUDED

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
void protobuf_AddDesc_loginmessage_2ehxx();
void protobuf_InitDefaults_loginmessage_2ehxx();
void protobuf_AssignDesc_loginmessage_2ehxx();
void protobuf_ShutdownFile_loginmessage_2ehxx();

class CMessageCreateRoleRequest;
class CMessageCreateRoleResponse;
class CMessageEnterSceneRequest;
class CMessageEnterSceneResponse;
class CMessageLoginRequest;
class CMessageLoginResponse;

enum LOGIN_MSG_ID {
  ID_ERROR = 0,
  ID_C2S_REQUEST_LOGIN = 1,
  ID_S2C_RESPONSE_LOGIN = 2,
  ID_C2S_REQUEST_CREATE_ROLE = 3,
  ID_S2C_RESPONSE_CREATE_ROLE = 4,
  ID_C2S_REQUEST_ENTER_SCENE = 5,
  ID_S2C_RESPONSE_ENTER_SCENE = 6,
  LOGIN_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  LOGIN_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool LOGIN_MSG_ID_IsValid(int value);
const LOGIN_MSG_ID LOGIN_MSG_ID_MIN = ID_ERROR;
const LOGIN_MSG_ID LOGIN_MSG_ID_MAX = ID_S2C_RESPONSE_ENTER_SCENE;
const int LOGIN_MSG_ID_ARRAYSIZE = LOGIN_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* LOGIN_MSG_ID_descriptor();
inline const ::std::string& LOGIN_MSG_ID_Name(LOGIN_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    LOGIN_MSG_ID_descriptor(), value);
}
inline bool LOGIN_MSG_ID_Parse(
    const ::std::string& name, LOGIN_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<LOGIN_MSG_ID>(
    LOGIN_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CMessageLoginRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageLoginRequest) */ {
 public:
  CMessageLoginRequest();
  virtual ~CMessageLoginRequest();

  CMessageLoginRequest(const CMessageLoginRequest& from);

  inline CMessageLoginRequest& operator=(const CMessageLoginRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageLoginRequest& default_instance();

  static const CMessageLoginRequest* internal_default_instance();

  void Swap(CMessageLoginRequest* other);

  // implements Message ----------------------------------------------

  inline CMessageLoginRequest* New() const { return New(NULL); }

  CMessageLoginRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageLoginRequest& from);
  void MergeFrom(const CMessageLoginRequest& from);
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
  void InternalSwap(CMessageLoginRequest* other);
  void UnsafeMergeFrom(const CMessageLoginRequest& from);
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

  // optional string Name = 1;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // optional uint32 ChannelID = 2;
  void clear_channelid();
  static const int kChannelIDFieldNumber = 2;
  ::google::protobuf::uint32 channelid() const;
  void set_channelid(::google::protobuf::uint32 value);

  // optional uint32 WorldID = 3;
  void clear_worldid();
  static const int kWorldIDFieldNumber = 3;
  ::google::protobuf::uint32 worldid() const;
  void set_worldid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessageLoginRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::uint32 channelid_;
  ::google::protobuf::uint32 worldid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_loginmessage_2ehxx_impl();
  friend void  protobuf_AddDesc_loginmessage_2ehxx_impl();
  friend void protobuf_AssignDesc_loginmessage_2ehxx();
  friend void protobuf_ShutdownFile_loginmessage_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageLoginRequest> CMessageLoginRequest_default_instance_;

// -------------------------------------------------------------------

class CMessageLoginResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageLoginResponse) */ {
 public:
  CMessageLoginResponse();
  virtual ~CMessageLoginResponse();

  CMessageLoginResponse(const CMessageLoginResponse& from);

  inline CMessageLoginResponse& operator=(const CMessageLoginResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageLoginResponse& default_instance();

  static const CMessageLoginResponse* internal_default_instance();

  void Swap(CMessageLoginResponse* other);

  // implements Message ----------------------------------------------

  inline CMessageLoginResponse* New() const { return New(NULL); }

  CMessageLoginResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageLoginResponse& from);
  void MergeFrom(const CMessageLoginResponse& from);
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
  void InternalSwap(CMessageLoginResponse* other);
  void UnsafeMergeFrom(const CMessageLoginResponse& from);
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

  // optional uint32 AccountID = 1;
  void clear_accountid();
  static const int kAccountIDFieldNumber = 1;
  ::google::protobuf::uint32 accountid() const;
  void set_accountid(::google::protobuf::uint32 value);

  // optional uint32 RoleID = 2;
  void clear_roleid();
  static const int kRoleIDFieldNumber = 2;
  ::google::protobuf::uint32 roleid() const;
  void set_roleid(::google::protobuf::uint32 value);

  // optional uint32 ChannelID = 3;
  void clear_channelid();
  static const int kChannelIDFieldNumber = 3;
  ::google::protobuf::uint32 channelid() const;
  void set_channelid(::google::protobuf::uint32 value);

  // optional uint32 WorldID = 4;
  void clear_worldid();
  static const int kWorldIDFieldNumber = 4;
  ::google::protobuf::uint32 worldid() const;
  void set_worldid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessageLoginResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 accountid_;
  ::google::protobuf::uint32 roleid_;
  ::google::protobuf::uint32 channelid_;
  ::google::protobuf::uint32 worldid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_loginmessage_2ehxx_impl();
  friend void  protobuf_AddDesc_loginmessage_2ehxx_impl();
  friend void protobuf_AssignDesc_loginmessage_2ehxx();
  friend void protobuf_ShutdownFile_loginmessage_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageLoginResponse> CMessageLoginResponse_default_instance_;

// -------------------------------------------------------------------

class CMessageCreateRoleRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageCreateRoleRequest) */ {
 public:
  CMessageCreateRoleRequest();
  virtual ~CMessageCreateRoleRequest();

  CMessageCreateRoleRequest(const CMessageCreateRoleRequest& from);

  inline CMessageCreateRoleRequest& operator=(const CMessageCreateRoleRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageCreateRoleRequest& default_instance();

  static const CMessageCreateRoleRequest* internal_default_instance();

  void Swap(CMessageCreateRoleRequest* other);

  // implements Message ----------------------------------------------

  inline CMessageCreateRoleRequest* New() const { return New(NULL); }

  CMessageCreateRoleRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageCreateRoleRequest& from);
  void MergeFrom(const CMessageCreateRoleRequest& from);
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
  void InternalSwap(CMessageCreateRoleRequest* other);
  void UnsafeMergeFrom(const CMessageCreateRoleRequest& from);
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

  // optional uint32 AccountID = 1;
  void clear_accountid();
  static const int kAccountIDFieldNumber = 1;
  ::google::protobuf::uint32 accountid() const;
  void set_accountid(::google::protobuf::uint32 value);

  // optional uint32 ChannelID = 2;
  void clear_channelid();
  static const int kChannelIDFieldNumber = 2;
  ::google::protobuf::uint32 channelid() const;
  void set_channelid(::google::protobuf::uint32 value);

  // optional uint32 WorldID = 3;
  void clear_worldid();
  static const int kWorldIDFieldNumber = 3;
  ::google::protobuf::uint32 worldid() const;
  void set_worldid(::google::protobuf::uint32 value);

  // optional string RoleName = 4;
  void clear_rolename();
  static const int kRoleNameFieldNumber = 4;
  const ::std::string& rolename() const;
  void set_rolename(const ::std::string& value);
  void set_rolename(const char* value);
  void set_rolename(const char* value, size_t size);
  ::std::string* mutable_rolename();
  ::std::string* release_rolename();
  void set_allocated_rolename(::std::string* rolename);

  // @@protoc_insertion_point(class_scope:CMessageCreateRoleRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr rolename_;
  ::google::protobuf::uint32 accountid_;
  ::google::protobuf::uint32 channelid_;
  ::google::protobuf::uint32 worldid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_loginmessage_2ehxx_impl();
  friend void  protobuf_AddDesc_loginmessage_2ehxx_impl();
  friend void protobuf_AssignDesc_loginmessage_2ehxx();
  friend void protobuf_ShutdownFile_loginmessage_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageCreateRoleRequest> CMessageCreateRoleRequest_default_instance_;

// -------------------------------------------------------------------

class CMessageCreateRoleResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageCreateRoleResponse) */ {
 public:
  CMessageCreateRoleResponse();
  virtual ~CMessageCreateRoleResponse();

  CMessageCreateRoleResponse(const CMessageCreateRoleResponse& from);

  inline CMessageCreateRoleResponse& operator=(const CMessageCreateRoleResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageCreateRoleResponse& default_instance();

  static const CMessageCreateRoleResponse* internal_default_instance();

  void Swap(CMessageCreateRoleResponse* other);

  // implements Message ----------------------------------------------

  inline CMessageCreateRoleResponse* New() const { return New(NULL); }

  CMessageCreateRoleResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageCreateRoleResponse& from);
  void MergeFrom(const CMessageCreateRoleResponse& from);
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
  void InternalSwap(CMessageCreateRoleResponse* other);
  void UnsafeMergeFrom(const CMessageCreateRoleResponse& from);
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

  // optional uint32 RoleID = 2;
  void clear_roleid();
  static const int kRoleIDFieldNumber = 2;
  ::google::protobuf::uint32 roleid() const;
  void set_roleid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessageCreateRoleResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  ::google::protobuf::uint32 roleid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_loginmessage_2ehxx_impl();
  friend void  protobuf_AddDesc_loginmessage_2ehxx_impl();
  friend void protobuf_AssignDesc_loginmessage_2ehxx();
  friend void protobuf_ShutdownFile_loginmessage_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageCreateRoleResponse> CMessageCreateRoleResponse_default_instance_;

// -------------------------------------------------------------------

class CMessageEnterSceneRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageEnterSceneRequest) */ {
 public:
  CMessageEnterSceneRequest();
  virtual ~CMessageEnterSceneRequest();

  CMessageEnterSceneRequest(const CMessageEnterSceneRequest& from);

  inline CMessageEnterSceneRequest& operator=(const CMessageEnterSceneRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageEnterSceneRequest& default_instance();

  static const CMessageEnterSceneRequest* internal_default_instance();

  void Swap(CMessageEnterSceneRequest* other);

  // implements Message ----------------------------------------------

  inline CMessageEnterSceneRequest* New() const { return New(NULL); }

  CMessageEnterSceneRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageEnterSceneRequest& from);
  void MergeFrom(const CMessageEnterSceneRequest& from);
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
  void InternalSwap(CMessageEnterSceneRequest* other);
  void UnsafeMergeFrom(const CMessageEnterSceneRequest& from);
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

  // optional uint32 RoleID = 1;
  void clear_roleid();
  static const int kRoleIDFieldNumber = 1;
  ::google::protobuf::uint32 roleid() const;
  void set_roleid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessageEnterSceneRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 roleid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_loginmessage_2ehxx_impl();
  friend void  protobuf_AddDesc_loginmessage_2ehxx_impl();
  friend void protobuf_AssignDesc_loginmessage_2ehxx();
  friend void protobuf_ShutdownFile_loginmessage_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageEnterSceneRequest> CMessageEnterSceneRequest_default_instance_;

// -------------------------------------------------------------------

class CMessageEnterSceneResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageEnterSceneResponse) */ {
 public:
  CMessageEnterSceneResponse();
  virtual ~CMessageEnterSceneResponse();

  CMessageEnterSceneResponse(const CMessageEnterSceneResponse& from);

  inline CMessageEnterSceneResponse& operator=(const CMessageEnterSceneResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageEnterSceneResponse& default_instance();

  static const CMessageEnterSceneResponse* internal_default_instance();

  void Swap(CMessageEnterSceneResponse* other);

  // implements Message ----------------------------------------------

  inline CMessageEnterSceneResponse* New() const { return New(NULL); }

  CMessageEnterSceneResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageEnterSceneResponse& from);
  void MergeFrom(const CMessageEnterSceneResponse& from);
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
  void InternalSwap(CMessageEnterSceneResponse* other);
  void UnsafeMergeFrom(const CMessageEnterSceneResponse& from);
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

  // optional uint32 Result = 2;
  void clear_result();
  static const int kResultFieldNumber = 2;
  ::google::protobuf::uint32 result() const;
  void set_result(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessageEnterSceneResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_loginmessage_2ehxx_impl();
  friend void  protobuf_AddDesc_loginmessage_2ehxx_impl();
  friend void protobuf_AssignDesc_loginmessage_2ehxx();
  friend void protobuf_ShutdownFile_loginmessage_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageEnterSceneResponse> CMessageEnterSceneResponse_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CMessageLoginRequest

// optional string Name = 1;
inline void CMessageLoginRequest::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CMessageLoginRequest::name() const {
  // @@protoc_insertion_point(field_get:CMessageLoginRequest.Name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CMessageLoginRequest::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CMessageLoginRequest.Name)
}
inline void CMessageLoginRequest::set_name(const char* value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CMessageLoginRequest.Name)
}
inline void CMessageLoginRequest::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CMessageLoginRequest.Name)
}
inline ::std::string* CMessageLoginRequest::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:CMessageLoginRequest.Name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CMessageLoginRequest::release_name() {
  // @@protoc_insertion_point(field_release:CMessageLoginRequest.Name)
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CMessageLoginRequest::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:CMessageLoginRequest.Name)
}

// optional uint32 ChannelID = 2;
inline void CMessageLoginRequest::clear_channelid() {
  channelid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageLoginRequest::channelid() const {
  // @@protoc_insertion_point(field_get:CMessageLoginRequest.ChannelID)
  return channelid_;
}
inline void CMessageLoginRequest::set_channelid(::google::protobuf::uint32 value) {
  
  channelid_ = value;
  // @@protoc_insertion_point(field_set:CMessageLoginRequest.ChannelID)
}

// optional uint32 WorldID = 3;
inline void CMessageLoginRequest::clear_worldid() {
  worldid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageLoginRequest::worldid() const {
  // @@protoc_insertion_point(field_get:CMessageLoginRequest.WorldID)
  return worldid_;
}
inline void CMessageLoginRequest::set_worldid(::google::protobuf::uint32 value) {
  
  worldid_ = value;
  // @@protoc_insertion_point(field_set:CMessageLoginRequest.WorldID)
}

inline const CMessageLoginRequest* CMessageLoginRequest::internal_default_instance() {
  return &CMessageLoginRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageLoginResponse

// optional uint32 AccountID = 1;
inline void CMessageLoginResponse::clear_accountid() {
  accountid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageLoginResponse::accountid() const {
  // @@protoc_insertion_point(field_get:CMessageLoginResponse.AccountID)
  return accountid_;
}
inline void CMessageLoginResponse::set_accountid(::google::protobuf::uint32 value) {
  
  accountid_ = value;
  // @@protoc_insertion_point(field_set:CMessageLoginResponse.AccountID)
}

// optional uint32 RoleID = 2;
inline void CMessageLoginResponse::clear_roleid() {
  roleid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageLoginResponse::roleid() const {
  // @@protoc_insertion_point(field_get:CMessageLoginResponse.RoleID)
  return roleid_;
}
inline void CMessageLoginResponse::set_roleid(::google::protobuf::uint32 value) {
  
  roleid_ = value;
  // @@protoc_insertion_point(field_set:CMessageLoginResponse.RoleID)
}

// optional uint32 ChannelID = 3;
inline void CMessageLoginResponse::clear_channelid() {
  channelid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageLoginResponse::channelid() const {
  // @@protoc_insertion_point(field_get:CMessageLoginResponse.ChannelID)
  return channelid_;
}
inline void CMessageLoginResponse::set_channelid(::google::protobuf::uint32 value) {
  
  channelid_ = value;
  // @@protoc_insertion_point(field_set:CMessageLoginResponse.ChannelID)
}

// optional uint32 WorldID = 4;
inline void CMessageLoginResponse::clear_worldid() {
  worldid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageLoginResponse::worldid() const {
  // @@protoc_insertion_point(field_get:CMessageLoginResponse.WorldID)
  return worldid_;
}
inline void CMessageLoginResponse::set_worldid(::google::protobuf::uint32 value) {
  
  worldid_ = value;
  // @@protoc_insertion_point(field_set:CMessageLoginResponse.WorldID)
}

inline const CMessageLoginResponse* CMessageLoginResponse::internal_default_instance() {
  return &CMessageLoginResponse_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageCreateRoleRequest

// optional uint32 AccountID = 1;
inline void CMessageCreateRoleRequest::clear_accountid() {
  accountid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageCreateRoleRequest::accountid() const {
  // @@protoc_insertion_point(field_get:CMessageCreateRoleRequest.AccountID)
  return accountid_;
}
inline void CMessageCreateRoleRequest::set_accountid(::google::protobuf::uint32 value) {
  
  accountid_ = value;
  // @@protoc_insertion_point(field_set:CMessageCreateRoleRequest.AccountID)
}

// optional uint32 ChannelID = 2;
inline void CMessageCreateRoleRequest::clear_channelid() {
  channelid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageCreateRoleRequest::channelid() const {
  // @@protoc_insertion_point(field_get:CMessageCreateRoleRequest.ChannelID)
  return channelid_;
}
inline void CMessageCreateRoleRequest::set_channelid(::google::protobuf::uint32 value) {
  
  channelid_ = value;
  // @@protoc_insertion_point(field_set:CMessageCreateRoleRequest.ChannelID)
}

// optional uint32 WorldID = 3;
inline void CMessageCreateRoleRequest::clear_worldid() {
  worldid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageCreateRoleRequest::worldid() const {
  // @@protoc_insertion_point(field_get:CMessageCreateRoleRequest.WorldID)
  return worldid_;
}
inline void CMessageCreateRoleRequest::set_worldid(::google::protobuf::uint32 value) {
  
  worldid_ = value;
  // @@protoc_insertion_point(field_set:CMessageCreateRoleRequest.WorldID)
}

// optional string RoleName = 4;
inline void CMessageCreateRoleRequest::clear_rolename() {
  rolename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CMessageCreateRoleRequest::rolename() const {
  // @@protoc_insertion_point(field_get:CMessageCreateRoleRequest.RoleName)
  return rolename_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CMessageCreateRoleRequest::set_rolename(const ::std::string& value) {
  
  rolename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CMessageCreateRoleRequest.RoleName)
}
inline void CMessageCreateRoleRequest::set_rolename(const char* value) {
  
  rolename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CMessageCreateRoleRequest.RoleName)
}
inline void CMessageCreateRoleRequest::set_rolename(const char* value, size_t size) {
  
  rolename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CMessageCreateRoleRequest.RoleName)
}
inline ::std::string* CMessageCreateRoleRequest::mutable_rolename() {
  
  // @@protoc_insertion_point(field_mutable:CMessageCreateRoleRequest.RoleName)
  return rolename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CMessageCreateRoleRequest::release_rolename() {
  // @@protoc_insertion_point(field_release:CMessageCreateRoleRequest.RoleName)
  
  return rolename_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CMessageCreateRoleRequest::set_allocated_rolename(::std::string* rolename) {
  if (rolename != NULL) {
    
  } else {
    
  }
  rolename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), rolename);
  // @@protoc_insertion_point(field_set_allocated:CMessageCreateRoleRequest.RoleName)
}

inline const CMessageCreateRoleRequest* CMessageCreateRoleRequest::internal_default_instance() {
  return &CMessageCreateRoleRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageCreateRoleResponse

// optional uint32 Result = 1;
inline void CMessageCreateRoleResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CMessageCreateRoleResponse::result() const {
  // @@protoc_insertion_point(field_get:CMessageCreateRoleResponse.Result)
  return result_;
}
inline void CMessageCreateRoleResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CMessageCreateRoleResponse.Result)
}

// optional uint32 RoleID = 2;
inline void CMessageCreateRoleResponse::clear_roleid() {
  roleid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageCreateRoleResponse::roleid() const {
  // @@protoc_insertion_point(field_get:CMessageCreateRoleResponse.RoleID)
  return roleid_;
}
inline void CMessageCreateRoleResponse::set_roleid(::google::protobuf::uint32 value) {
  
  roleid_ = value;
  // @@protoc_insertion_point(field_set:CMessageCreateRoleResponse.RoleID)
}

inline const CMessageCreateRoleResponse* CMessageCreateRoleResponse::internal_default_instance() {
  return &CMessageCreateRoleResponse_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageEnterSceneRequest

// optional uint32 RoleID = 1;
inline void CMessageEnterSceneRequest::clear_roleid() {
  roleid_ = 0u;
}
inline ::google::protobuf::uint32 CMessageEnterSceneRequest::roleid() const {
  // @@protoc_insertion_point(field_get:CMessageEnterSceneRequest.RoleID)
  return roleid_;
}
inline void CMessageEnterSceneRequest::set_roleid(::google::protobuf::uint32 value) {
  
  roleid_ = value;
  // @@protoc_insertion_point(field_set:CMessageEnterSceneRequest.RoleID)
}

inline const CMessageEnterSceneRequest* CMessageEnterSceneRequest::internal_default_instance() {
  return &CMessageEnterSceneRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageEnterSceneResponse

// optional uint32 Result = 2;
inline void CMessageEnterSceneResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CMessageEnterSceneResponse::result() const {
  // @@protoc_insertion_point(field_get:CMessageEnterSceneResponse.Result)
  return result_;
}
inline void CMessageEnterSceneResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CMessageEnterSceneResponse.Result)
}

inline const CMessageEnterSceneResponse* CMessageEnterSceneResponse::internal_default_instance() {
  return &CMessageEnterSceneResponse_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::LOGIN_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::LOGIN_MSG_ID>() {
  return ::LOGIN_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_loginmessage_2ehxx__INCLUDED
