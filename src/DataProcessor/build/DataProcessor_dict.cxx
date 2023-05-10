// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME DataProcessor_dict
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
#include "DataProcessor.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DataProcessor(void *p = nullptr);
   static void *newArray_DataProcessor(Long_t size, void *p);
   static void delete_DataProcessor(void *p);
   static void deleteArray_DataProcessor(void *p);
   static void destruct_DataProcessor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DataProcessor*)
   {
      ::DataProcessor *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DataProcessor >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DataProcessor", ::DataProcessor::Class_Version(), "DataProcessor.h", 24,
                  typeid(::DataProcessor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DataProcessor::Dictionary, isa_proxy, 4,
                  sizeof(::DataProcessor) );
      instance.SetNew(&new_DataProcessor);
      instance.SetNewArray(&newArray_DataProcessor);
      instance.SetDelete(&delete_DataProcessor);
      instance.SetDeleteArray(&deleteArray_DataProcessor);
      instance.SetDestructor(&destruct_DataProcessor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DataProcessor*)
   {
      return GenerateInitInstanceLocal((::DataProcessor*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DataProcessor*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DataProcessor::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DataProcessor::Class_Name()
{
   return "DataProcessor";
}

//______________________________________________________________________________
const char *DataProcessor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DataProcessor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DataProcessor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DataProcessor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DataProcessor::Streamer(TBuffer &R__b)
{
   // Stream an object of class DataProcessor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DataProcessor::Class(),this);
   } else {
      R__b.WriteClassBuffer(DataProcessor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DataProcessor(void *p) {
      return  p ? new(p) ::DataProcessor : new ::DataProcessor;
   }
   static void *newArray_DataProcessor(Long_t nElements, void *p) {
      return p ? new(p) ::DataProcessor[nElements] : new ::DataProcessor[nElements];
   }
   // Wrapper around operator delete
   static void delete_DataProcessor(void *p) {
      delete ((::DataProcessor*)p);
   }
   static void deleteArray_DataProcessor(void *p) {
      delete [] ((::DataProcessor*)p);
   }
   static void destruct_DataProcessor(void *p) {
      typedef ::DataProcessor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DataProcessor

namespace {
  void TriggerDictionaryInitialization_libDataProcessor_Impl() {
    static const char* headers[] = {
"DataProcessor.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ_dev/src/DataProcessor",
"/storage/ATTPC/DAQ_dev/src/DataProcessor",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ_dev/src/DataProcessor/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDataProcessor dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$DataProcessor.h")))  DataProcessor;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libDataProcessor dictionary payload"

#ifndef USE_WEBSOCKET
  #define USE_WEBSOCKET 1
#endif
#ifndef NO_SSL_DL
  #define NO_SSL_DL 1
#endif
#ifndef HTTP_WITHOUT_FASTCGI
  #define HTTP_WITHOUT_FASTCGI 1
#endif
#ifndef VECCORE_ENABLE_VC
  #define VECCORE_ENABLE_VC 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "DataProcessor.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DataProcessor", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDataProcessor",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDataProcessor_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDataProcessor_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDataProcessor() {
  TriggerDictionaryInitialization_libDataProcessor_Impl();
}
