// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME EventConverter_dict
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
#include "EventConverter.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_EventConverter(void *p = nullptr);
   static void *newArray_EventConverter(Long_t size, void *p);
   static void delete_EventConverter(void *p);
   static void deleteArray_EventConverter(void *p);
   static void destruct_EventConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EventConverter*)
   {
      ::EventConverter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EventConverter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("EventConverter", ::EventConverter::Class_Version(), "EventConverter.h", 14,
                  typeid(::EventConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EventConverter::Dictionary, isa_proxy, 4,
                  sizeof(::EventConverter) );
      instance.SetNew(&new_EventConverter);
      instance.SetNewArray(&newArray_EventConverter);
      instance.SetDelete(&delete_EventConverter);
      instance.SetDeleteArray(&deleteArray_EventConverter);
      instance.SetDestructor(&destruct_EventConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EventConverter*)
   {
      return GenerateInitInstanceLocal((::EventConverter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EventConverter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr EventConverter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EventConverter::Class_Name()
{
   return "EventConverter";
}

//______________________________________________________________________________
const char *EventConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventConverter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EventConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventConverter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EventConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventConverter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EventConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventConverter*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void EventConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class EventConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EventConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(EventConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EventConverter(void *p) {
      return  p ? new(p) ::EventConverter : new ::EventConverter;
   }
   static void *newArray_EventConverter(Long_t nElements, void *p) {
      return p ? new(p) ::EventConverter[nElements] : new ::EventConverter[nElements];
   }
   // Wrapper around operator delete
   static void delete_EventConverter(void *p) {
      delete ((::EventConverter*)p);
   }
   static void deleteArray_EventConverter(void *p) {
      delete [] ((::EventConverter*)p);
   }
   static void destruct_EventConverter(void *p) {
      typedef ::EventConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EventConverter

namespace {
  void TriggerDictionaryInitialization_libEventConverter_Impl() {
    static const char* headers[] = {
"EventConverter.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ_dev/src/EventConverter",
"/storage/ATTPC/DAQ_dev/src/EventConverter",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ_dev/src/EventConverter/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libEventConverter dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$EventConverter.h")))  EventConverter;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libEventConverter dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "EventConverter.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"EventConverter", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libEventConverter",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libEventConverter_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libEventConverter_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libEventConverter() {
  TriggerDictionaryInitialization_libEventConverter_Impl();
}
