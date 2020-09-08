// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rankmodule.hxx

#ifndef PROTOBUF_rankmodule_2ehxx__INCLUDED
#define PROTOBUF_rankmodule_2ehxx__INCLUDED

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
void protobuf_AddDesc_rankmodule_2ehxx();
void protobuf_InitDefaults_rankmodule_2ehxx();
void protobuf_AssignDesc_rankmodule_2ehxx();
void protobuf_ShutdownFile_rankmodule_2ehxx();

class CGetRankInfoRequest;
class CGetRankInfoResponse;
class PBRankRoleInfo;

enum RANK_MODULE_MSG_ID {
  ID_RANK_MODULE_ERROR = 0,
  ID_C2S_REQUEST_GET_RANK_INFO = 9216,
  ID_S2C_RESPONSE_GET_RANK_INFO = 9217,
  RANK_MODULE_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RANK_MODULE_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RANK_MODULE_MSG_ID_IsValid(int value);
const RANK_MODULE_MSG_ID RANK_MODULE_MSG_ID_MIN = ID_RANK_MODULE_ERROR;
const RANK_MODULE_MSG_ID RANK_MODULE_MSG_ID_MAX = ID_S2C_RESPONSE_GET_RANK_INFO;
const int RANK_MODULE_MSG_ID_ARRAYSIZE = RANK_MODULE_MSG_ID_MAX + 1;

const ::google::protobuf::EnumDescriptor* RANK_MODULE_MSG_ID_descriptor();
inline const ::std::string& RANK_MODULE_MSG_ID_Name(RANK_MODULE_MSG_ID value) {
  return ::google::protobuf::internal::NameOfEnum(
    RANK_MODULE_MSG_ID_descriptor(), value);
}
inline bool RANK_MODULE_MSG_ID_Parse(
    const ::std::string& name, RANK_MODULE_MSG_ID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RANK_MODULE_MSG_ID>(
    RANK_MODULE_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class CGetRankInfoRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CGetRankInfoRequest) */ {
 public:
  CGetRankInfoRequest();
  virtual ~CGetRankInfoRequest();

  CGetRankInfoRequest(const CGetRankInfoRequest& from);

  inline CGetRankInfoRequest& operator=(const CGetRankInfoRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CGetRankInfoRequest& default_instance();

  static const CGetRankInfoRequest* internal_default_instance();

  void Swap(CGetRankInfoRequest* other);

  // implements Message ----------------------------------------------

  inline CGetRankInfoRequest* New() const { return New(NULL); }

  CGetRankInfoRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CGetRankInfoRequest& from);
  void MergeFrom(const CGetRankInfoRequest& from);
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
  void InternalSwap(CGetRankInfoRequest* other);
  void UnsafeMergeFrom(const CGetRankInfoRequest& from);
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

  // optional uint32 RankType = 1;
  void clear_ranktype();
  static const int kRankTypeFieldNumber = 1;
  ::google::protobuf::uint32 ranktype() const;
  void set_ranktype(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CGetRankInfoRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 ranktype_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_rankmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_rankmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_rankmodule_2ehxx();
  friend void protobuf_ShutdownFile_rankmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CGetRankInfoRequest> CGetRankInfoRequest_default_instance_;

// -------------------------------------------------------------------

class PBRankRoleInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PBRankRoleInfo) */ {
 public:
  PBRankRoleInfo();
  virtual ~PBRankRoleInfo();

  PBRankRoleInfo(const PBRankRoleInfo& from);

  inline PBRankRoleInfo& operator=(const PBRankRoleInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PBRankRoleInfo& default_instance();

  static const PBRankRoleInfo* internal_default_instance();

  void Swap(PBRankRoleInfo* other);

  // implements Message ----------------------------------------------

  inline PBRankRoleInfo* New() const { return New(NULL); }

  PBRankRoleInfo* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBRankRoleInfo& from);
  void MergeFrom(const PBRankRoleInfo& from);
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
  void InternalSwap(PBRankRoleInfo* other);
  void UnsafeMergeFrom(const PBRankRoleInfo& from);
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

  // optional uint32 RankValue = 2;
  void clear_rankvalue();
  static const int kRankValueFieldNumber = 2;
  ::google::protobuf::uint32 rankvalue() const;
  void set_rankvalue(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:PBRankRoleInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 roleid_;
  ::google::protobuf::uint32 rankvalue_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_rankmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_rankmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_rankmodule_2ehxx();
  friend void protobuf_ShutdownFile_rankmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PBRankRoleInfo> PBRankRoleInfo_default_instance_;

// -------------------------------------------------------------------

class CGetRankInfoResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CGetRankInfoResponse) */ {
 public:
  CGetRankInfoResponse();
  virtual ~CGetRankInfoResponse();

  CGetRankInfoResponse(const CGetRankInfoResponse& from);

  inline CGetRankInfoResponse& operator=(const CGetRankInfoResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CGetRankInfoResponse& default_instance();

  static const CGetRankInfoResponse* internal_default_instance();

  void Swap(CGetRankInfoResponse* other);

  // implements Message ----------------------------------------------

  inline CGetRankInfoResponse* New() const { return New(NULL); }

  CGetRankInfoResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CGetRankInfoResponse& from);
  void MergeFrom(const CGetRankInfoResponse& from);
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
  void InternalSwap(CGetRankInfoResponse* other);
  void UnsafeMergeFrom(const CGetRankInfoResponse& from);
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

  // optional uint32 RankType = 1;
  void clear_ranktype();
  static const int kRankTypeFieldNumber = 1;
  ::google::protobuf::uint32 ranktype() const;
  void set_ranktype(::google::protobuf::uint32 value);

  // repeated .PBRankRoleInfo RoleInfo = 2;
  int roleinfo_size() const;
  void clear_roleinfo();
  static const int kRoleInfoFieldNumber = 2;
  const ::PBRankRoleInfo& roleinfo(int index) const;
  ::PBRankRoleInfo* mutable_roleinfo(int index);
  ::PBRankRoleInfo* add_roleinfo();
  ::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo >*
      mutable_roleinfo();
  const ::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo >&
      roleinfo() const;

  // optional uint32 SelfPlace = 3;
  void clear_selfplace();
  static const int kSelfPlaceFieldNumber = 3;
  ::google::protobuf::uint32 selfplace() const;
  void set_selfplace(::google::protobuf::uint32 value);

  // optional uint32 SelfRankValue = 4;
  void clear_selfrankvalue();
  static const int kSelfRankValueFieldNumber = 4;
  ::google::protobuf::uint32 selfrankvalue() const;
  void set_selfrankvalue(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:CGetRankInfoResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo > roleinfo_;
  ::google::protobuf::uint32 ranktype_;
  ::google::protobuf::uint32 selfplace_;
  ::google::protobuf::uint32 selfrankvalue_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_rankmodule_2ehxx_impl();
  friend void  protobuf_AddDesc_rankmodule_2ehxx_impl();
  friend void protobuf_AssignDesc_rankmodule_2ehxx();
  friend void protobuf_ShutdownFile_rankmodule_2ehxx();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CGetRankInfoResponse> CGetRankInfoResponse_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CGetRankInfoRequest

// optional uint32 RankType = 1;
inline void CGetRankInfoRequest::clear_ranktype() {
  ranktype_ = 0u;
}
inline ::google::protobuf::uint32 CGetRankInfoRequest::ranktype() const {
  // @@protoc_insertion_point(field_get:CGetRankInfoRequest.RankType)
  return ranktype_;
}
inline void CGetRankInfoRequest::set_ranktype(::google::protobuf::uint32 value) {
  
  ranktype_ = value;
  // @@protoc_insertion_point(field_set:CGetRankInfoRequest.RankType)
}

inline const CGetRankInfoRequest* CGetRankInfoRequest::internal_default_instance() {
  return &CGetRankInfoRequest_default_instance_.get();
}
// -------------------------------------------------------------------

// PBRankRoleInfo

// optional uint32 RoleID = 1;
inline void PBRankRoleInfo::clear_roleid() {
  roleid_ = 0u;
}
inline ::google::protobuf::uint32 PBRankRoleInfo::roleid() const {
  // @@protoc_insertion_point(field_get:PBRankRoleInfo.RoleID)
  return roleid_;
}
inline void PBRankRoleInfo::set_roleid(::google::protobuf::uint32 value) {
  
  roleid_ = value;
  // @@protoc_insertion_point(field_set:PBRankRoleInfo.RoleID)
}

// optional uint32 RankValue = 2;
inline void PBRankRoleInfo::clear_rankvalue() {
  rankvalue_ = 0u;
}
inline ::google::protobuf::uint32 PBRankRoleInfo::rankvalue() const {
  // @@protoc_insertion_point(field_get:PBRankRoleInfo.RankValue)
  return rankvalue_;
}
inline void PBRankRoleInfo::set_rankvalue(::google::protobuf::uint32 value) {
  
  rankvalue_ = value;
  // @@protoc_insertion_point(field_set:PBRankRoleInfo.RankValue)
}

inline const PBRankRoleInfo* PBRankRoleInfo::internal_default_instance() {
  return &PBRankRoleInfo_default_instance_.get();
}
// -------------------------------------------------------------------

// CGetRankInfoResponse

// optional uint32 RankType = 1;
inline void CGetRankInfoResponse::clear_ranktype() {
  ranktype_ = 0u;
}
inline ::google::protobuf::uint32 CGetRankInfoResponse::ranktype() const {
  // @@protoc_insertion_point(field_get:CGetRankInfoResponse.RankType)
  return ranktype_;
}
inline void CGetRankInfoResponse::set_ranktype(::google::protobuf::uint32 value) {
  
  ranktype_ = value;
  // @@protoc_insertion_point(field_set:CGetRankInfoResponse.RankType)
}

// repeated .PBRankRoleInfo RoleInfo = 2;
inline int CGetRankInfoResponse::roleinfo_size() const {
  return roleinfo_.size();
}
inline void CGetRankInfoResponse::clear_roleinfo() {
  roleinfo_.Clear();
}
inline const ::PBRankRoleInfo& CGetRankInfoResponse::roleinfo(int index) const {
  // @@protoc_insertion_point(field_get:CGetRankInfoResponse.RoleInfo)
  return roleinfo_.Get(index);
}
inline ::PBRankRoleInfo* CGetRankInfoResponse::mutable_roleinfo(int index) {
  // @@protoc_insertion_point(field_mutable:CGetRankInfoResponse.RoleInfo)
  return roleinfo_.Mutable(index);
}
inline ::PBRankRoleInfo* CGetRankInfoResponse::add_roleinfo() {
  // @@protoc_insertion_point(field_add:CGetRankInfoResponse.RoleInfo)
  return roleinfo_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo >*
CGetRankInfoResponse::mutable_roleinfo() {
  // @@protoc_insertion_point(field_mutable_list:CGetRankInfoResponse.RoleInfo)
  return &roleinfo_;
}
inline const ::google::protobuf::RepeatedPtrField< ::PBRankRoleInfo >&
CGetRankInfoResponse::roleinfo() const {
  // @@protoc_insertion_point(field_list:CGetRankInfoResponse.RoleInfo)
  return roleinfo_;
}

// optional uint32 SelfPlace = 3;
inline void CGetRankInfoResponse::clear_selfplace() {
  selfplace_ = 0u;
}
inline ::google::protobuf::uint32 CGetRankInfoResponse::selfplace() const {
  // @@protoc_insertion_point(field_get:CGetRankInfoResponse.SelfPlace)
  return selfplace_;
}
inline void CGetRankInfoResponse::set_selfplace(::google::protobuf::uint32 value) {
  
  selfplace_ = value;
  // @@protoc_insertion_point(field_set:CGetRankInfoResponse.SelfPlace)
}

// optional uint32 SelfRankValue = 4;
inline void CGetRankInfoResponse::clear_selfrankvalue() {
  selfrankvalue_ = 0u;
}
inline ::google::protobuf::uint32 CGetRankInfoResponse::selfrankvalue() const {
  // @@protoc_insertion_point(field_get:CGetRankInfoResponse.SelfRankValue)
  return selfrankvalue_;
}
inline void CGetRankInfoResponse::set_selfrankvalue(::google::protobuf::uint32 value) {
  
  selfrankvalue_ = value;
  // @@protoc_insertion_point(field_set:CGetRankInfoResponse.SelfRankValue)
}

inline const CGetRankInfoResponse* CGetRankInfoResponse::internal_default_instance() {
  return &CGetRankInfoResponse_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::RANK_MODULE_MSG_ID> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::RANK_MODULE_MSG_ID>() {
  return ::RANK_MODULE_MSG_ID_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_rankmodule_2ehxx__INCLUDED
