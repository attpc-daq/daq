// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME EventMaker_dict
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
#include "EventMaker.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_EventMaker(void *p = nullptr);
   static void *newArray_EventMaker(Long_t size, void *p);
   static void delete_EventMaker(void *p);
   static void deleteArray_EventMaker(void *p);
   static void destruct_EventMaker(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EventMaker*)
   {
      ::EventMaker *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EventMaker >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("EventMaker", ::EventMaker::Class_Version(), "EventMaker.h", 15,
                  typeid(::EventMaker), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EventMaker::Dictionary, isa_proxy, 4,
                  sizeof(::EventMaker) );
      instance.SetNew(&new_EventMaker);
      instance.SetNewArray(&newArray_EventMaker);
      instance.SetDelete(&delete_EventMaker);
      instance.SetDeleteArray(&deleteArray_EventMaker);
      instance.SetDestructor(&destruct_EventMaker);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EventMaker*)
   {
      return GenerateInitInstanceLocal((::EventMaker*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EventMaker*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr EventMaker::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EventMaker::Class_Name()
{
   return "EventMaker";
}

//______________________________________________________________________________
const char *EventMaker::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventMaker*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EventMaker::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventMaker*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EventMaker::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventMaker*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EventMaker::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventMaker*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void EventMaker::Streamer(TBuffer &R__b)
{
   // Stream an object of class EventMaker.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EventMaker::Class(),this);
   } else {
      R__b.WriteClassBuffer(EventMaker::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EventMaker(void *p) {
      return  p ? new(p) ::EventMaker : new ::EventMaker;
   }
   static void *newArray_EventMaker(Long_t nElements, void *p) {
      return p ? new(p) ::EventMaker[nElements] : new ::EventMaker[nElements];
   }
   // Wrapper around operator delete
   static void delete_EventMaker(void *p) {
      delete ((::EventMaker*)p);
   }
   static void deleteArray_EventMaker(void *p) {
      delete [] ((::EventMaker*)p);
   }
   static void destruct_EventMaker(void *p) {
      typedef ::EventMaker current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EventMaker

namespace {
  void TriggerDictionaryInitialization_libEventMaker_Impl() {
    static const char* headers[] = {
"EventMaker.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/EventMaker",
"/storage/ATTPC/DAQ/src/EventMaker",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/EventMaker/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libEventMaker dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$EventMaker.h")))  EventMaker;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libEventMaker dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "EventMaker.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"EventMaker", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libEventMaker",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libEventMaker_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libEventMaker_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libEventMaker() {
  TriggerDictionaryInitialization_libEventMaker_Impl();
}
