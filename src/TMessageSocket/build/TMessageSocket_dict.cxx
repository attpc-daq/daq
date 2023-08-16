// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TMessageSocket_dict
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
#include "TMessageSocket.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_TMessageSocket(void *p);
   static void deleteArray_TMessageSocket(void *p);
   static void destruct_TMessageSocket(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TMessageSocket*)
   {
      ::TMessageSocket *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TMessageSocket >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TMessageSocket", ::TMessageSocket::Class_Version(), "TMessageSocket.h", 19,
                  typeid(::TMessageSocket), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TMessageSocket::Dictionary, isa_proxy, 4,
                  sizeof(::TMessageSocket) );
      instance.SetDelete(&delete_TMessageSocket);
      instance.SetDeleteArray(&deleteArray_TMessageSocket);
      instance.SetDestructor(&destruct_TMessageSocket);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TMessageSocket*)
   {
      return GenerateInitInstanceLocal((::TMessageSocket*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TMessageSocket*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TMessageSocket::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TMessageSocket::Class_Name()
{
   return "TMessageSocket";
}

//______________________________________________________________________________
const char *TMessageSocket::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMessageSocket*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TMessageSocket::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TMessageSocket*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TMessageSocket::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMessageSocket*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TMessageSocket::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TMessageSocket*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TMessageSocket::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMessageSocket.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TMessageSocket::Class(),this);
   } else {
      R__b.WriteClassBuffer(TMessageSocket::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TMessageSocket(void *p) {
      delete ((::TMessageSocket*)p);
   }
   static void deleteArray_TMessageSocket(void *p) {
      delete [] ((::TMessageSocket*)p);
   }
   static void destruct_TMessageSocket(void *p) {
      typedef ::TMessageSocket current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TMessageSocket

namespace {
  void TriggerDictionaryInitialization_libTMessageSocket_Impl() {
    static const char* headers[] = {
"TMessageSocket.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/TMessageSocket",
"/storage/ATTPC/DAQ/src/TMessageSocket",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/TMessageSocket/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libTMessageSocket dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TMessageSocket.h")))  TMessageSocket;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libTMessageSocket dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "TMessageSocket.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TMessageSocket", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libTMessageSocket",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libTMessageSocket_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libTMessageSocket_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libTMessageSocket() {
  TriggerDictionaryInitialization_libTMessageSocket_Impl();
}
