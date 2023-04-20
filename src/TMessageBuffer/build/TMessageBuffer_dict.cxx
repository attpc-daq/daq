// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TMessageBuffer_dict
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
#include "TMessageBuffer.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_TMessageBuffer(void *p);
   static void deleteArray_TMessageBuffer(void *p);
   static void destruct_TMessageBuffer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMessageBuffer*)
   {
      ::TMessageBuffer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMessageBuffer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TMessageBuffer", ::TMessageBuffer::Class_Version(), "TMessageBuffer.h", 15,
                  typeid(::TMessageBuffer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMessageBuffer::Dictionary, isa_proxy, 4,
                  sizeof(::TMessageBuffer) );
      instance.SetDelete(&delete_TMessageBuffer);
      instance.SetDeleteArray(&deleteArray_TMessageBuffer);
      instance.SetDestructor(&destruct_TMessageBuffer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMessageBuffer*)
   {
      return GenerateInitInstanceLocal((::TMessageBuffer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TMessageBuffer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TMessageBuffer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TMessageBuffer::Class_Name()
{
   return "TMessageBuffer";
}

//______________________________________________________________________________
const char *TMessageBuffer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMessageBuffer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TMessageBuffer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMessageBuffer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMessageBuffer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMessageBuffer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMessageBuffer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMessageBuffer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TMessageBuffer::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMessageBuffer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMessageBuffer::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMessageBuffer::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TMessageBuffer(void *p) {
      delete ((::TMessageBuffer*)p);
   }
   static void deleteArray_TMessageBuffer(void *p) {
      delete [] ((::TMessageBuffer*)p);
   }
   static void destruct_TMessageBuffer(void *p) {
      typedef ::TMessageBuffer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMessageBuffer

namespace {
  void TriggerDictionaryInitialization_libTMessageBuffer_Impl() {
    static const char* headers[] = {
"TMessageBuffer.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/daq-dev-/src/TMessageBuffer",
"/storage/ATTPC/daq-dev-/src/TMessageBuffer",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/daq-dev-/src/TMessageBuffer/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libTMessageBuffer dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TMessageBuffer.h")))  TMessageBuffer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libTMessageBuffer dictionary payload"

#ifndef VECCORE_ENABLE_VC
  #define VECCORE_ENABLE_VC 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TMessageBuffer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TMessageBuffer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libTMessageBuffer",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libTMessageBuffer_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libTMessageBuffer_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libTMessageBuffer() {
  TriggerDictionaryInitialization_libTMessageBuffer_Impl();
}
