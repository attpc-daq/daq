// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RawEventProcessor_dict
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
#include "RawEventProcessor.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_RawEventProcessor(void *p = nullptr);
   static void *newArray_RawEventProcessor(Long_t size, void *p);
   static void delete_RawEventProcessor(void *p);
   static void deleteArray_RawEventProcessor(void *p);
   static void destruct_RawEventProcessor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RawEventProcessor*)
   {
      ::RawEventProcessor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RawEventProcessor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("RawEventProcessor", ::RawEventProcessor::Class_Version(), "RawEventProcessor.h", 25,
                  typeid(::RawEventProcessor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RawEventProcessor::Dictionary, isa_proxy, 4,
                  sizeof(::RawEventProcessor) );
      instance.SetNew(&new_RawEventProcessor);
      instance.SetNewArray(&newArray_RawEventProcessor);
      instance.SetDelete(&delete_RawEventProcessor);
      instance.SetDeleteArray(&deleteArray_RawEventProcessor);
      instance.SetDestructor(&destruct_RawEventProcessor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RawEventProcessor*)
   {
      return GenerateInitInstanceLocal((::RawEventProcessor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RawEventProcessor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RawEventProcessor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RawEventProcessor::Class_Name()
{
   return "RawEventProcessor";
}

//______________________________________________________________________________
const char *RawEventProcessor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawEventProcessor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RawEventProcessor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawEventProcessor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RawEventProcessor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawEventProcessor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RawEventProcessor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawEventProcessor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RawEventProcessor::Streamer(TBuffer &R__b)
{
   // Stream an object of class RawEventProcessor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RawEventProcessor::Class(),this);
   } else {
      R__b.WriteClassBuffer(RawEventProcessor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RawEventProcessor(void *p) {
      return  p ? new(p) ::RawEventProcessor : new ::RawEventProcessor;
   }
   static void *newArray_RawEventProcessor(Long_t nElements, void *p) {
      return p ? new(p) ::RawEventProcessor[nElements] : new ::RawEventProcessor[nElements];
   }
   // Wrapper around operator delete
   static void delete_RawEventProcessor(void *p) {
      delete ((::RawEventProcessor*)p);
   }
   static void deleteArray_RawEventProcessor(void *p) {
      delete [] ((::RawEventProcessor*)p);
   }
   static void destruct_RawEventProcessor(void *p) {
      typedef ::RawEventProcessor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RawEventProcessor

namespace {
  void TriggerDictionaryInitialization_libRawEventProcessor_Impl() {
    static const char* headers[] = {
"RawEventProcessor.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/daq-dev-/src/RawEventProcessor",
"/storage/ATTPC/daq-dev-/src/RawEventProcessor",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/daq-dev-/src/RawEventProcessor/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libRawEventProcessor dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawEventProcessor.h")))  RawEventProcessor;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawEventProcessor dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawEventProcessor.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"RawEventProcessor", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawEventProcessor",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawEventProcessor_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawEventProcessor_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawEventProcessor() {
  TriggerDictionaryInitialization_libRawEventProcessor_Impl();
}
