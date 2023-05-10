// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME ParameterGenerator_dict
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
#include "ParameterGenerator.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_ParameterGenerator(void *p = nullptr);
   static void *newArray_ParameterGenerator(Long_t size, void *p);
   static void delete_ParameterGenerator(void *p);
   static void deleteArray_ParameterGenerator(void *p);
   static void destruct_ParameterGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ParameterGenerator*)
   {
      ::ParameterGenerator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ParameterGenerator >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ParameterGenerator", ::ParameterGenerator::Class_Version(), "ParameterGenerator.h", 25,
                  typeid(::ParameterGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ParameterGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::ParameterGenerator) );
      instance.SetNew(&new_ParameterGenerator);
      instance.SetNewArray(&newArray_ParameterGenerator);
      instance.SetDelete(&delete_ParameterGenerator);
      instance.SetDeleteArray(&deleteArray_ParameterGenerator);
      instance.SetDestructor(&destruct_ParameterGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ParameterGenerator*)
   {
      return GenerateInitInstanceLocal((::ParameterGenerator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ParameterGenerator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ParameterGenerator::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ParameterGenerator::Class_Name()
{
   return "ParameterGenerator";
}

//______________________________________________________________________________
const char *ParameterGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ParameterGenerator*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ParameterGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ParameterGenerator*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ParameterGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ParameterGenerator*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ParameterGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ParameterGenerator*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ParameterGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ParameterGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ParameterGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ParameterGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ParameterGenerator(void *p) {
      return  p ? new(p) ::ParameterGenerator : new ::ParameterGenerator;
   }
   static void *newArray_ParameterGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::ParameterGenerator[nElements] : new ::ParameterGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_ParameterGenerator(void *p) {
      delete ((::ParameterGenerator*)p);
   }
   static void deleteArray_ParameterGenerator(void *p) {
      delete [] ((::ParameterGenerator*)p);
   }
   static void destruct_ParameterGenerator(void *p) {
      typedef ::ParameterGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ParameterGenerator

namespace {
  void TriggerDictionaryInitialization_libParameterGenerator_Impl() {
    static const char* headers[] = {
"ParameterGenerator.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ_dev/src/ParameterGenerator",
"/storage/ATTPC/DAQ_dev/src/ParameterGenerator",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ_dev/src/ParameterGenerator/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libParameterGenerator dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$ParameterGenerator.h")))  ParameterGenerator;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libParameterGenerator dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "ParameterGenerator.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ParameterGenerator", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libParameterGenerator",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libParameterGenerator_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libParameterGenerator_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libParameterGenerator() {
  TriggerDictionaryInitialization_libParameterGenerator_Impl();
}
