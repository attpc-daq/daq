// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME EventQA_dict
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
#include "EventQA.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_EventQA(void *p = nullptr);
   static void *newArray_EventQA(Long_t size, void *p);
   static void delete_EventQA(void *p);
   static void deleteArray_EventQA(void *p);
   static void destruct_EventQA(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EventQA*)
   {
      ::EventQA *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EventQA >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("EventQA", ::EventQA::Class_Version(), "EventQA.h", 19,
                  typeid(::EventQA), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EventQA::Dictionary, isa_proxy, 4,
                  sizeof(::EventQA) );
      instance.SetNew(&new_EventQA);
      instance.SetNewArray(&newArray_EventQA);
      instance.SetDelete(&delete_EventQA);
      instance.SetDeleteArray(&deleteArray_EventQA);
      instance.SetDestructor(&destruct_EventQA);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EventQA*)
   {
      return GenerateInitInstanceLocal((::EventQA*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EventQA*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr EventQA::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EventQA::Class_Name()
{
   return "EventQA";
}

//______________________________________________________________________________
const char *EventQA::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventQA*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EventQA::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventQA*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EventQA::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventQA*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EventQA::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventQA*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void EventQA::Streamer(TBuffer &R__b)
{
   // Stream an object of class EventQA.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EventQA::Class(),this);
   } else {
      R__b.WriteClassBuffer(EventQA::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EventQA(void *p) {
      return  p ? new(p) ::EventQA : new ::EventQA;
   }
   static void *newArray_EventQA(Long_t nElements, void *p) {
      return p ? new(p) ::EventQA[nElements] : new ::EventQA[nElements];
   }
   // Wrapper around operator delete
   static void delete_EventQA(void *p) {
      delete ((::EventQA*)p);
   }
   static void deleteArray_EventQA(void *p) {
      delete [] ((::EventQA*)p);
   }
   static void destruct_EventQA(void *p) {
      typedef ::EventQA current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EventQA

namespace {
  void TriggerDictionaryInitialization_libEventQA_Impl() {
    static const char* headers[] = {
"EventQA.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/EventQA",
"/storage/ATTPC/DAQ/src/EventQA",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/EventQA/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libEventQA dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$EventQA.h")))  EventQA;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libEventQA dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "EventQA.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"EventQA", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libEventQA",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libEventQA_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libEventQA_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libEventQA() {
  TriggerDictionaryInitialization_libEventQA_Impl();
}
