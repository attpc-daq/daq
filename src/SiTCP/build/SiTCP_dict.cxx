// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SiTCP_dict
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
#include "SiTCP.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_SiTCP(void *p = nullptr);
   static void *newArray_SiTCP(Long_t size, void *p);
   static void delete_SiTCP(void *p);
   static void deleteArray_SiTCP(void *p);
   static void destruct_SiTCP(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SiTCP*)
   {
      ::SiTCP *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SiTCP >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("SiTCP", ::SiTCP::Class_Version(), "SiTCP.h", 21,
                  typeid(::SiTCP), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SiTCP::Dictionary, isa_proxy, 4,
                  sizeof(::SiTCP) );
      instance.SetNew(&new_SiTCP);
      instance.SetNewArray(&newArray_SiTCP);
      instance.SetDelete(&delete_SiTCP);
      instance.SetDeleteArray(&deleteArray_SiTCP);
      instance.SetDestructor(&destruct_SiTCP);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SiTCP*)
   {
      return GenerateInitInstanceLocal((::SiTCP*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SiTCP*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr SiTCP::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SiTCP::Class_Name()
{
   return "SiTCP";
}

//______________________________________________________________________________
const char *SiTCP::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SiTCP*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SiTCP::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SiTCP*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SiTCP::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SiTCP*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SiTCP::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SiTCP*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void SiTCP::Streamer(TBuffer &R__b)
{
   // Stream an object of class SiTCP.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SiTCP::Class(),this);
   } else {
      R__b.WriteClassBuffer(SiTCP::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SiTCP(void *p) {
      return  p ? new(p) ::SiTCP : new ::SiTCP;
   }
   static void *newArray_SiTCP(Long_t nElements, void *p) {
      return p ? new(p) ::SiTCP[nElements] : new ::SiTCP[nElements];
   }
   // Wrapper around operator delete
   static void delete_SiTCP(void *p) {
      delete ((::SiTCP*)p);
   }
   static void deleteArray_SiTCP(void *p) {
      delete [] ((::SiTCP*)p);
   }
   static void destruct_SiTCP(void *p) {
      typedef ::SiTCP current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SiTCP

namespace {
  void TriggerDictionaryInitialization_libSiTCP_Impl() {
    static const char* headers[] = {
"SiTCP.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/SiTCP",
"/storage/ATTPC/DAQ/src/SiTCP",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/SiTCP/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libSiTCP dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$SiTCP.h")))  SiTCP;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libSiTCP dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "SiTCP.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"SiTCP", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSiTCP",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSiTCP_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSiTCP_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSiTCP() {
  TriggerDictionaryInitialization_libSiTCP_Impl();
}
