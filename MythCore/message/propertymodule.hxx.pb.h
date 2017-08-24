// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: propertymodule.hxx

#ifndef PROTOBUF_propertymodule_2ehxx__INCLUDED
#define PROTOBUF_propertymodule_2ehxx__INCLUDED

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
void protobuf_AddDesc_propertymodule_2ehxx();
void protobuf_InitDefaults_propertymodule_2ehxx();
void protobuf_AssignDesc_propertymodule_2ehxx();
void protobuf_ShutdownFile_propertymodule_2ehxx();

class CMessageGMCommandRequest;
class CMessageGMCommandResponse;
class CMessagePlayerLevelUpNotify;

enum PROPERTY_MODULE_MSG_ID {
  ID_PROPERTY_MODULE_ERROR = 0,
  ID_S2C_NOTIYF_PLAYER_LEVEL_UP = 1024,
  ID_C2S_REQUEST_GM_COMMAND = 1025,
  ID_S2C_RESPONSE_GM_COMMAND = 1026,
  PROPERTY_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  PROPERTY_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool PROPERTY_MODULE_MSG_ID_IsValid(int value);
const PROPERTY_MODULE_MSG_ID PROPERTY_MODULE_MSG_ID_MIN = ID_PROPERTY_MODULE_ERROR;
const PROPERTY_MODULE_MSG_ID PROPERTY_MODULE_MSG_ID_MAX = ID_S2C_RESPONSE_GM_COMMAND;
const int PROPERTY_MODULE_MSG_ID_ARRAYSIZE = PROPERTY_MODULE_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* PROPERTY_MODULE_MSG_ID_descriptor();
inline const ::std::string& PROPERTY_MODULE_MSG_ID_Name(PROPERTY_MODULE_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    PROPERTY_MODULE_MSG_ID_descriptor(), value);
}
inline bool PROPERTY_MODULE_MSG_ID_Parse(
    const ::std::string& name, PROPERTY_MODULE_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<PROPERTY_MODULE_MSG_ID>(
    PROPERTY_MODULE_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CMessagePlayerLevelUpNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessagePlayerLevelUpNotify) */ {
 public:
  CMessagePlayerLevelUpNotify();
  virtual ~CMessagePlayerLevelUpNotify();

  CMessagePlayerLevelUpNotify(const CMessagePlayerLevelUpNotify& from);

  inline CMessagePlayerLevelUpNotify& operator=(const CMessagePlayerLevelUpNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessagePlayerLevelUpNotify& default_instance();

  static const CMessagePlayerLevelUpNotify* internal_default_instance();

  void Swap(CMessagePlayerLevelUpNotify* other);

  // implements Message ----------------------------------------------

  inline CMessagePlayerLevelUpNotify* New() const { return New(NULL); }

  CMessagePlayerLevelUpNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessagePlayerLevelUpNotify& from);
  void MergeFrom(const CMessagePlayerLevelUpNotify& from);
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
  void InternalSwap(CMessagePlayerLevelUpNotify* other);
  void UnsafeMergeFrom(const CMessagePlayerLevelUpNotify& from);
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

  // optional uint32 Level = 1;
  void clear_level();
  static const int kLevelFieldNumber = 1;
  ::google::protobuf::uint32 level() const;
  void set_level(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CMessagePlayerLevelUpNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 level_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_propertymodule_2ehxx_impl();
  friend void  protobuf_AddDesc_propertymodule_2ehxx_impl();
  friend void protobuf_AssignDesc_propertymodule_2ehxx();
  friend void protobuf_ShutdownFile_propertymodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessagePlayerLevelUpNotify> CMessagePlayerLevelUpNotify_default_instance_;

// -------------------------------------------------------------------

class CMessageGMCommandRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageGMCommandRequest) */ {
 public:
  CMessageGMCommandRequest();
  virtual ~CMessageGMCommandRequest();

  CMessageGMCommandRequest(const CMessageGMCommandRequest& from);

  inline CMessageGMCommandRequest& operator=(const CMessageGMCommandRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageGMCommandRequest& default_instance();

  static const CMessageGMCommandRequest* internal_default_instance();

  void Swap(CMessageGMCommandRequest* other);

  // implements Message ----------------------------------------------

  inline CMessageGMCommandRequest* New() const { return New(NULL); }

  CMessageGMCommandRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageGMCommandRequest& from);
  void MergeFrom(const CMessageGMCommandRequest& from);
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
  void InternalSwap(CMessageGMCommandRequest* other);
  void UnsafeMergeFrom(const CMessageGMCommandRequest& from);
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

  // optional string CommandString = 1;
  void clear_commandstring();
  static const int kCommandStringFieldNumber = 1;
  const ::std::string& commandstring() const;
  void set_commandstring(const ::std::string& value);
  void set_commandstring(const char* value);
  void set_commandstring(const char* value, size_t size);
  ::std::string* mutable_commandstring();
  ::std::string* release_commandstring();
  void set_allocated_commandstring(::std::string* commandstring);

  // @@protoc_insertion_point(class_scope:CMessageGMCommandRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr commandstring_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_propertymodule_2ehxx_impl();
  friend void  protobuf_AddDesc_propertymodule_2ehxx_impl();
  friend void protobuf_AssignDesc_propertymodule_2ehxx();
  friend void protobuf_ShutdownFile_propertymodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageGMCommandRequest> CMessageGMCommandRequest_default_instance_;

// -------------------------------------------------------------------

class CMessageGMCommandResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CMessageGMCommandResponse) */ {
 public:
  CMessageGMCommandResponse();
  virtual ~CMessageGMCommandResponse();

  CMessageGMCommandResponse(const CMessageGMCommandResponse& from);

  inline CMessageGMCommandResponse& operator=(const CMessageGMCommandResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CMessageGMCommandResponse& default_instance();

  static const CMessageGMCommandResponse* internal_default_instance();

  void Swap(CMessageGMCommandResponse* other);

  // implements Message ----------------------------------------------

  inline CMessageGMCommandResponse* New() const { return New(NULL); }

  CMessageGMCommandResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CMessageGMCommandResponse& from);
  void MergeFrom(const CMessageGMCommandResponse& from);
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
  void InternalSwap(CMessageGMCommandResponse* other);
  void UnsafeMergeFrom(const CMessageGMCommandResponse& from);
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

  // @@protoc_insertion_point(class_scope:CMessageGMCommandResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_propertymodule_2ehxx_impl();
  friend void  protobuf_AddDesc_propertymodule_2ehxx_impl();
  friend void protobuf_AssignDesc_propertymodule_2ehxx();
  friend void protobuf_ShutdownFile_propertymodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CMessageGMCommandResponse> CMessageGMCommandResponse_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CMessagePlayerLevelUpNotify

// optional uint32 Level = 1;
inline void CMessagePlayerLevelUpNotify::clear_level() {
  level_ = 0u;
}
inline ::google::protobuf::uint32 CMessagePlayerLevelUpNotify::level() const {
  // @@protoc_insertion_point(field_get:CMessagePlayerLevelUpNotify.Level)
  return level_;
}
inline void CMessagePlayerLevelUpNotify::set_level(::google::protobuf::uint32 value) {
  
  level_ = value;
  // @@protoc_insertion_point(field_set:CMessagePlayerLevelUpNotify.Level)
}

inline const CMessagePlayerLevelUpNotify* CMessagePlayerLevelUpNotify::internal_default_instance() {
  return &CMessagePlayerLevelUpNotify_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageGMCommandRequest

// optional string CommandString = 1;
inline void CMessageGMCommandRequest::clear_commandstring() {
  commandstring_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CMessageGMCommandRequest::commandstring() const {
  // @@protoc_insertion_point(field_get:CMessageGMCommandRequest.CommandString)
  return commandstring_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CMessageGMCommandRequest::set_commandstring(const ::std::string& value) {
  
  commandstring_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CMessageGMCommandRequest.CommandString)
}
inline void CMessageGMCommandRequest::set_commandstring(const char* value) {
  
  commandstring_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CMessageGMCommandRequest.CommandString)
}
inline void CMessageGMCommandRequest::set_commandstring(const char* value, size_t size) {
  
  commandstring_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CMessageGMCommandRequest.CommandString)
}
inline ::std::string* CMessageGMCommandRequest::mutable_commandstring() {
  
  // @@protoc_insertion_point(field_mutable:CMessageGMCommandRequest.CommandString)
  return commandstring_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CMessageGMCommandRequest::release_commandstring() {
  // @@protoc_insertion_point(field_release:CMessageGMCommandRequest.CommandString)
  
  return commandstring_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CMessageGMCommandRequest::set_allocated_commandstring(::std::string* commandstring) {
  if (commandstring != NULL) {
    
  } else {
    
  }
  commandstring_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), commandstring);
  // @@protoc_insertion_point(field_set_allocated:CMessageGMCommandRequest.CommandString)
}

inline const CMessageGMCommandRequest* CMessageGMCommandRequest::internal_default_instance() {
  return &CMessageGMCommandRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CMessageGMCommandResponse

// optional uint32 Result = 1;
inline void CMessageGMCommandResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CMessageGMCommandResponse::result() const {
  // @@protoc_insertion_point(field_get:CMessageGMCommandResponse.Result)
  return result_;
}
inline void CMessageGMCommandResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CMessageGMCommandResponse.Result)
}

inline const CMessageGMCommandResponse* CMessageGMCommandResponse::internal_default_instance() {
  return &CMessageGMCommandResponse_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::PROPERTY_MODULE_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PROPERTY_MODULE_MSG_ID>() {
  return ::PROPERTY_MODULE_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_propertymodule_2ehxx__INCLUDED