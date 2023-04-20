// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RawEvent_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "Channel.h"
#include "RawEvent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_Channel(void *p = nullptr);
   static void *newArray_Channel(Long_t size, void *p);
   static void delete_Channel(void *p);
   static void deleteArray_Channel(void *p);
   static void destruct_Channel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Channel*)
   {
      ::Channel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Channel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Channel", ::Channel::Class_Version(), "Channel.h", 5,
                  typeid(::Channel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Channel::Dictionary, isa_proxy, 4,
                  sizeof(::Channel) );
      instance.SetNew(&new_Channel);
      instance.SetNewArray(&newArray_Channel);
      instance.SetDelete(&delete_Channel);
      instance.SetDeleteArray(&deleteArray_Channel);
      instance.SetDestructor(&destruct_Channel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Channel*)
   {
      return GenerateInitInstanceLocal((::Channel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Channel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RawEvent(void *p = nullptr);
   static void *newArray_RawEvent(Long_t size, void *p);
   static void delete_RawEvent(void *p);
   static void deleteArray_RawEvent(void *p);
   static void destruct_RawEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RawEvent*)
   {
      ::RawEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RawEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("RawEvent", ::RawEvent::Class_Version(), "RawEvent.h", 9,
                  typeid(::RawEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RawEvent::Dictionary, isa_proxy, 4,
                  sizeof(::RawEvent) );
      instance.SetNew(&new_RawEvent);
      instance.SetNewArray(&newArray_RawEvent);
      instance.SetDelete(&delete_RawEvent);
      instance.SetDeleteArray(&deleteArray_RawEvent);
      instance.SetDestructor(&destruct_RawEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RawEvent*)
   {
      return GenerateInitInstanceLocal((::RawEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RawEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Channel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Channel::Class_Name()
{
   return "Channel";
}

//______________________________________________________________________________
const char *Channel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Channel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Channel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Channel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Channel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Channel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Channel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Channel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RawEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RawEvent::Class_Name()
{
   return "RawEvent";
}

//______________________________________________________________________________
const char *RawEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RawEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RawEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RawEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Channel::Streamer(TBuffer &R__b)
{
   // Stream an object of class Channel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Channel::Class(),this);
   } else {
      R__b.WriteClassBuffer(Channel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Channel(void *p) {
      return  p ? new(p) ::Channel : new ::Channel;
   }
   static void *newArray_Channel(Long_t nElements, void *p) {
      return p ? new(p) ::Channel[nElements] : new ::Channel[nElements];
   }
   // Wrapper around operator delete
   static void delete_Channel(void *p) {
      delete ((::Channel*)p);
   }
   static void deleteArray_Channel(void *p) {
      delete [] ((::Channel*)p);
   }
   static void destruct_Channel(void *p) {
      typedef ::Channel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Channel

//______________________________________________________________________________
void RawEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class RawEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RawEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(RawEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RawEvent(void *p) {
      return  p ? new(p) ::RawEvent : new ::RawEvent;
   }
   static void *newArray_RawEvent(Long_t nElements, void *p) {
      return p ? new(p) ::RawEvent[nElements] : new ::RawEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_RawEvent(void *p) {
      delete ((::RawEvent*)p);
   }
   static void deleteArray_RawEvent(void *p) {
      delete [] ((::RawEvent*)p);
   }
   static void destruct_RawEvent(void *p) {
      typedef ::RawEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RawEvent

namespace ROOT {
   static TClass *vectorlEChannelgR_Dictionary();
   static void vectorlEChannelgR_TClassManip(TClass*);
   static void *new_vectorlEChannelgR(void *p = nullptr);
   static void *newArray_vectorlEChannelgR(Long_t size, void *p);
   static void delete_vectorlEChannelgR(void *p);
   static void deleteArray_vectorlEChannelgR(void *p);
   static void destruct_vectorlEChannelgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Channel>*)
   {
      vector<Channel> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Channel>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Channel>", -2, "vector", 389,
                  typeid(vector<Channel>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEChannelgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Channel>) );
      instance.SetNew(&new_vectorlEChannelgR);
      instance.SetNewArray(&newArray_vectorlEChannelgR);
      instance.SetDelete(&delete_vectorlEChannelgR);
      instance.SetDeleteArray(&deleteArray_vectorlEChannelgR);
      instance.SetDestructor(&destruct_vectorlEChannelgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Channel> >()));

      ::ROOT::AddClassAlternate("vector<Channel>","std::vector<Channel, std::allocator<Channel> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Channel>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEChannelgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Channel>*)nullptr)->GetClass();
      vectorlEChannelgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEChannelgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEChannelgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Channel> : new vector<Channel>;
   }
   static void *newArray_vectorlEChannelgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Channel>[nElements] : new vector<Channel>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEChannelgR(void *p) {
      delete ((vector<Channel>*)p);
   }
   static void deleteArray_vectorlEChannelgR(void *p) {
      delete [] ((vector<Channel>*)p);
   }
   static void destruct_vectorlEChannelgR(void *p) {
      typedef vector<Channel> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Channel>

namespace {
  void TriggerDictionaryInitialization_libRawEvent_Impl() {
    static const char* headers[] = {
"Channel.h",
"RawEvent.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/daq-dev-fastQueue/src/RawEvent",
"/storage/ATTPC/daq-dev-fastQueue/src/RawEvent",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libRawEvent dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Channel.h")))  Channel;
class __attribute__((annotate("$clingAutoload$RawEvent.h")))  RawEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawEvent dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Channel.h"
#include "RawEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Channel", payloadCode, "@",
"RawEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawEvent",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawEvent_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawEvent_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawEvent() {
  TriggerDictionaryInitialization_libRawEvent_Impl();
}
