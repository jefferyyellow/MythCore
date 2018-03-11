// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: chatmodule.hxx

#ifndef PROTOBUF_chatmodule_2ehxx__INCLUDED
#define PROTOBUF_chatmodule_2ehxx__INCLUDED

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
void protobuf_AddDesc_chatmodule_2ehxx();
void protobuf_InitDefaults_chatmodule_2ehxx();
void protobuf_AssignDesc_chatmodule_2ehxx();
void protobuf_ShutdownFile_chatmodule_2ehxx();

class CChatNotify;
class CChatRequest;
class CChatResponse;

enum CHAT_MODULE_MSG_ID {
  ID_CHAT_MODULE_ERROR = 0,
  ID_C2S_REQUEST_CHAT = 4608,
  ID_S2C_RESPONSE_CHAT = 4609,
  ID_S2C_NOTIFY_CHAT = 4610,
  CHAT_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  CHAT_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool CHAT_MODULE_MSG_ID_IsValid(int value);
const CHAT_MODULE_MSG_ID CHAT_MODULE_MSG_ID_MIN = ID_CHAT_MODULE_ERROR;
const CHAT_MODULE_MSG_ID CHAT_MODULE_MSG_ID_MAX = ID_S2C_NOTIFY_CHAT;
const int CHAT_MODULE_MSG_ID_ARRAYSIZE = CHAT_MODULE_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* CHAT_MODULE_MSG_ID_descriptor();
inline const ::std::string& CHAT_MODULE_MSG_ID_Name(CHAT_MODULE_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    CHAT_MODULE_MSG_ID_descriptor(), value);
}
inline bool CHAT_MODULE_MSG_ID_Parse(
    const ::std::string& name, CHAT_MODULE_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CHAT_MODULE_MSG_ID>(
    CHAT_MODULE_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CChatRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CChatRequest) */ {
 public:
  CChatRequest();
  virtual ~CChatRequest();

  CChatRequest(const CChatRequest& from);

  inline CChatRequest& operator=(const CChatRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CChatRequest& default_instance();

  static const CChatRequest* internal_default_instance();

  void Swap(CChatRequest* other);

  // implements Message ----------------------------------------------

  inline CChatRequest* New() const { return New(NULL); }

  CChatRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CChatRequest& from);
  void MergeFrom(const CChatRequest& from);
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
  void InternalSwap(CChatRequest* other);
  void UnsafeMergeFrom(const CChatRequest& from);
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

  // optional uint32 Channel = 1;
  void clear_channel();
  static const int kChannelFieldNumber = 1;
  ::google::protobuf::uint32 channel() const;
  void set_channel(::google::protobuf::uint32 value);

  // optional string Content = 2;
  void clear_content();
  static const int kContentFieldNumber = 2;
  const ::std::string& content() const;
  void set_content(const ::std::string& value);
  void set_content(const char* value);
  void set_content(const char* value, size_t size);
  ::std::string* mutable_content();
  ::std::string* release_content();
  void set_allocated_content(::std::string* content);

  // @@protoc_insertion_point(class_scope:CChatRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr content_;
  ::google::protobuf::uint32 channel_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_chatmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_chatmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_chatmodule_2ehxx();
  friend void protobuf_ShutdownFile_chatmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CChatRequest> CChatRequest_default_instance_;

// -------------------------------------------------------------------

class CChatResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CChatResponse) */ {
 public:
  CChatResponse();
  virtual ~CChatResponse();

  CChatResponse(const CChatResponse& from);

  inline CChatResponse& operator=(const CChatResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CChatResponse& default_instance();

  static const CChatResponse* internal_default_instance();

  void Swap(CChatResponse* other);

  // implements Message ----------------------------------------------

  inline CChatResponse* New() const { return New(NULL); }

  CChatResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CChatResponse& from);
  void MergeFrom(const CChatResponse& from);
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
  void InternalSwap(CChatResponse* other);
  void UnsafeMergeFrom(const CChatResponse& from);
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

  // @@protoc_insertion_point(class_scope:CChatResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 result_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_chatmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_chatmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_chatmodule_2ehxx();
  friend void protobuf_ShutdownFile_chatmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CChatResponse> CChatResponse_default_instance_;

// -------------------------------------------------------------------

class CChatNotify : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CChatNotify) */ {
 public:
  CChatNotify();
  virtual ~CChatNotify();

  CChatNotify(const CChatNotify& from);

  inline CChatNotify& operator=(const CChatNotify& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CChatNotify& default_instance();

  static const CChatNotify* internal_default_instance();

  void Swap(CChatNotify* other);

  // implements Message ----------------------------------------------

  inline CChatNotify* New() const { return New(NULL); }

  CChatNotify* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CChatNotify& from);
  void MergeFrom(const CChatNotify& from);
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
  void InternalSwap(CChatNotify* other);
  void UnsafeMergeFrom(const CChatNotify& from);
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

  // optional uint32 PlayerID = 1;
  void clear_playerid();
  static const int kPlayerIDFieldNumber = 1;
  ::google::protobuf::uint32 playerid() const;
  void set_playerid(::google::protobuf::uint32 value);

  // optional string PlayerName = 2;
  void clear_playername();
  static const int kPlayerNameFieldNumber = 2;
  const ::std::string& playername() const;
  void set_playername(const ::std::string& value);
  void set_playername(const char* value);
  void set_playername(const char* value, size_t size);
  ::std::string* mutable_playername();
  ::std::string* release_playername();
  void set_allocated_playername(::std::string* playername);

  // optional uint32 Channel = 3;
  void clear_channel();
  static const int kChannelFieldNumber = 3;
  ::google::protobuf::uint32 channel() const;
  void set_channel(::google::protobuf::uint32 value);

  // optional string Content = 4;
  void clear_content();
  static const int kContentFieldNumber = 4;
  const ::std::string& content() const;
  void set_content(const ::std::string& value);
  void set_content(const char* value);
  void set_content(const char* value, size_t size);
  ::std::string* mutable_content();
  ::std::string* release_content();
  void set_allocated_content(::std::string* content);

  // @@protoc_insertion_point(class_scope:CChatNotify)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr playername_;
  ::google::protobuf::internal::ArenaStringPtr content_;
  ::google::protobuf::uint32 playerid_;
  ::google::protobuf::uint32 channel_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_chatmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_chatmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_chatmodule_2ehxx();
  friend void protobuf_ShutdownFile_chatmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CChatNotify> CChatNotify_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CChatRequest

// optional uint32 Channel = 1;
inline void CChatRequest::clear_channel() {
  channel_ = 0u;
}
inline ::google::protobuf::uint32 CChatRequest::channel() const {
  // @@protoc_insertion_point(field_get:CChatRequest.Channel)
  return channel_;
}
inline void CChatRequest::set_channel(::google::protobuf::uint32 value) {
  
  channel_ = value;
  // @@protoc_insertion_point(field_set:CChatRequest.Channel)
}

// optional string Content = 2;
inline void CChatRequest::clear_content() {
  content_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CChatRequest::content() const {
  // @@protoc_insertion_point(field_get:CChatRequest.Content)
  return content_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CChatRequest::set_content(const ::std::string& value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CChatRequest.Content)
}
inline void CChatRequest::set_content(const char* value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CChatRequest.Content)
}
inline void CChatRequest::set_content(const char* value, size_t size) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CChatRequest.Content)
}
inline ::std::string* CChatRequest::mutable_content() {
  
  // @@protoc_insertion_point(field_mutable:CChatRequest.Content)
  return content_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CChatRequest::release_content() {
  // @@protoc_insertion_point(field_release:CChatRequest.Content)
  
  return content_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CChatRequest::set_allocated_content(::std::string* content) {
  if (content != NULL) {
    
  } else {
    
  }
  content_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), content);
  // @@protoc_insertion_point(field_set_allocated:CChatRequest.Content)
}

inline const CChatRequest* CChatRequest::internal_default_instance() {
  return &CChatRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// CChatResponse

// optional uint32 Result = 1;
inline void CChatResponse::clear_result() {
  result_ = 0u;
}
inline ::google::protobuf::uint32 CChatResponse::result() const {
  // @@protoc_insertion_point(field_get:CChatResponse.Result)
  return result_;
}
inline void CChatResponse::set_result(::google::protobuf::uint32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:CChatResponse.Result)
}

inline const CChatResponse* CChatResponse::internal_default_instance() {
  return &CChatResponse_default_instance_.get();
}
// -------------------------------------------------------------------

// CChatNotify

// optional uint32 PlayerID = 1;
inline void CChatNotify::clear_playerid() {
  playerid_ = 0u;
}
inline ::google::protobuf::uint32 CChatNotify::playerid() const {
  // @@protoc_insertion_point(field_get:CChatNotify.PlayerID)
  return playerid_;
}
inline void CChatNotify::set_playerid(::google::protobuf::uint32 value) {
  
  playerid_ = value;
  // @@protoc_insertion_point(field_set:CChatNotify.PlayerID)
}

// optional string PlayerName = 2;
inline void CChatNotify::clear_playername() {
  playername_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CChatNotify::playername() const {
  // @@protoc_insertion_point(field_get:CChatNotify.PlayerName)
  return playername_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CChatNotify::set_playername(const ::std::string& value) {
  
  playername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CChatNotify.PlayerName)
}
inline void CChatNotify::set_playername(const char* value) {
  
  playername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CChatNotify.PlayerName)
}
inline void CChatNotify::set_playername(const char* value, size_t size) {
  
  playername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CChatNotify.PlayerName)
}
inline ::std::string* CChatNotify::mutable_playername() {
  
  // @@protoc_insertion_point(field_mutable:CChatNotify.PlayerName)
  return playername_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CChatNotify::release_playername() {
  // @@protoc_insertion_point(field_release:CChatNotify.PlayerName)
  
  return playername_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CChatNotify::set_allocated_playername(::std::string* playername) {
  if (playername != NULL) {
    
  } else {
    
  }
  playername_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), playername);
  // @@protoc_insertion_point(field_set_allocated:CChatNotify.PlayerName)
}

// optional uint32 Channel = 3;
inline void CChatNotify::clear_channel() {
  channel_ = 0u;
}
inline ::google::protobuf::uint32 CChatNotify::channel() const {
  // @@protoc_insertion_point(field_get:CChatNotify.Channel)
  return channel_;
}
inline void CChatNotify::set_channel(::google::protobuf::uint32 value) {
  
  channel_ = value;
  // @@protoc_insertion_point(field_set:CChatNotify.Channel)
}

// optional string Content = 4;
inline void CChatNotify::clear_content() {
  content_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CChatNotify::content() const {
  // @@protoc_insertion_point(field_get:CChatNotify.Content)
  return content_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CChatNotify::set_content(const ::std::string& value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CChatNotify.Content)
}
inline void CChatNotify::set_content(const char* value) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CChatNotify.Content)
}
inline void CChatNotify::set_content(const char* value, size_t size) {
  
  content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CChatNotify.Content)
}
inline ::std::string* CChatNotify::mutable_content() {
  
  // @@protoc_insertion_point(field_mutable:CChatNotify.Content)
  return content_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CChatNotify::release_content() {
  // @@protoc_insertion_point(field_release:CChatNotify.Content)
  
  return content_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CChatNotify::set_allocated_content(::std::string* content) {
  if (content != NULL) {
    
  } else {
    
  }
  content_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), content);
  // @@protoc_insertion_point(field_set_allocated:CChatNotify.Content)
}

inline const CChatNotify* CChatNotify::internal_default_instance() {
  return &CChatNotify_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::CHAT_MODULE_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::CHAT_MODULE_MSG_ID>() {
  return ::CHAT_MODULE_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_chatmodule_2ehxx__INCLUDED
