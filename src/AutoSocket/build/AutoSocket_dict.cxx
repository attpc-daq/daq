// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME AutoSocket_dict
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
#include "AutoSocket.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_AutoSocket(void *p);
   static void deleteArray_AutoSocket(void *p);
   static void destruct_AutoSocket(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AutoSocket*)
   {
      ::AutoSocket *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AutoSocket >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AutoSocket", ::AutoSocket::Class_Version(), "AutoSocket.h", 18,
                  typeid(::AutoSocket), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AutoSocket::Dictionary, isa_proxy, 4,
                  sizeof(::AutoSocket) );
      instance.SetDelete(&delete_AutoSocket);
      instance.SetDeleteArray(&deleteArray_AutoSocket);
      instance.SetDestructor(&destruct_AutoSocket);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AutoSocket*)
   {
      return GenerateInitInstanceLocal((::AutoSocket*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AutoSocket*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AutoSocket::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AutoSocket::Class_Name()
{
   return "AutoSocket";
}

//______________________________________________________________________________
const char *AutoSocket::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AutoSocket*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AutoSocket::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AutoSocket*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AutoSocket::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AutoSocket*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AutoSocket::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AutoSocket*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AutoSocket::Streamer(TBuffer &R__b)
{
   // Stream an object of class AutoSocket.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AutoSocket::Class(),this);
   } else {
      R__b.WriteClassBuffer(AutoSocket::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AutoSocket(void *p) {
      delete ((::AutoSocket*)p);
   }
   static void deleteArray_AutoSocket(void *p) {
      delete [] ((::AutoSocket*)p);
   }
   static void destruct_AutoSocket(void *p) {
      typedef ::AutoSocket current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AutoSocket

namespace {
  void TriggerDictionaryInitialization_libAutoSocket_Impl() {
    static const char* headers[] = {
"AutoSocket.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/AutoSocket",
"/storage/ATTPC/DAQ/src/AutoSocket",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/AutoSocket/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAutoSocket dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$AutoSocket.h")))  AutoSocket;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAutoSocket dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "AutoSocket.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AutoSocket", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAutoSocket",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAutoSocket_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAutoSocket_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAutoSocket() {
  TriggerDictionaryInitialization_libAutoSocket_Impl();
}
