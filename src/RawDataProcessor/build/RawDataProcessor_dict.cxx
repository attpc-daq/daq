// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RawDataProcessor_dict
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
#include "RawDataProcessor.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_RawDataProcessor(void *p = nullptr);
   static void *newArray_RawDataProcessor(Long_t size, void *p);
   static void delete_RawDataProcessor(void *p);
   static void deleteArray_RawDataProcessor(void *p);
   static void destruct_RawDataProcessor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RawDataProcessor*)
   {
      ::RawDataProcessor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RawDataProcessor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("RawDataProcessor", ::RawDataProcessor::Class_Version(), "RawDataProcessor.h", 25,
                  typeid(::RawDataProcessor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RawDataProcessor::Dictionary, isa_proxy, 4,
                  sizeof(::RawDataProcessor) );
      instance.SetNew(&new_RawDataProcessor);
      instance.SetNewArray(&newArray_RawDataProcessor);
      instance.SetDelete(&delete_RawDataProcessor);
      instance.SetDeleteArray(&deleteArray_RawDataProcessor);
      instance.SetDestructor(&destruct_RawDataProcessor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RawDataProcessor*)
   {
      return GenerateInitInstanceLocal((::RawDataProcessor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RawDataProcessor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RawDataProcessor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RawDataProcessor::Class_Name()
{
   return "RawDataProcessor";
}

//______________________________________________________________________________
const char *RawDataProcessor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawDataProcessor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RawDataProcessor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawDataProcessor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RawDataProcessor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawDataProcessor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RawDataProcessor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawDataProcessor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RawDataProcessor::Streamer(TBuffer &R__b)
{
   // Stream an object of class RawDataProcessor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RawDataProcessor::Class(),this);
   } else {
      R__b.WriteClassBuffer(RawDataProcessor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RawDataProcessor(void *p) {
      return  p ? new(p) ::RawDataProcessor : new ::RawDataProcessor;
   }
   static void *newArray_RawDataProcessor(Long_t nElements, void *p) {
      return p ? new(p) ::RawDataProcessor[nElements] : new ::RawDataProcessor[nElements];
   }
   // Wrapper around operator delete
   static void delete_RawDataProcessor(void *p) {
      delete ((::RawDataProcessor*)p);
   }
   static void deleteArray_RawDataProcessor(void *p) {
      delete [] ((::RawDataProcessor*)p);
   }
   static void destruct_RawDataProcessor(void *p) {
      typedef ::RawDataProcessor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RawDataProcessor

namespace {
  void TriggerDictionaryInitialization_libRawDataProcessor_Impl() {
    static const char* headers[] = {
"RawDataProcessor.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/daq-dev-/src/RawDataProcessor",
"/storage/ATTPC/daq-dev-/src/RawDataProcessor",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/daq-dev-/src/RawDataProcessor/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libRawDataProcessor dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawDataProcessor.h")))  RawDataProcessor;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawDataProcessor dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawDataProcessor.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"RawDataProcessor", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawDataProcessor",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawDataProcessor_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawDataProcessor_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawDataProcessor() {
  TriggerDictionaryInitialization_libRawDataProcessor_Impl();
}
