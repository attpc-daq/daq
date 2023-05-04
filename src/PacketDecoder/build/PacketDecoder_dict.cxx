// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PacketDecoder_dict
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
#include "PacketDecoder.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_PacketDecoder(void *p = nullptr);
   static void *newArray_PacketDecoder(Long_t size, void *p);
   static void delete_PacketDecoder(void *p);
   static void deleteArray_PacketDecoder(void *p);
   static void destruct_PacketDecoder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PacketDecoder*)
   {
      ::PacketDecoder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PacketDecoder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PacketDecoder", ::PacketDecoder::Class_Version(), "PacketDecoder.h", 15,
                  typeid(::PacketDecoder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PacketDecoder::Dictionary, isa_proxy, 4,
                  sizeof(::PacketDecoder) );
      instance.SetNew(&new_PacketDecoder);
      instance.SetNewArray(&newArray_PacketDecoder);
      instance.SetDelete(&delete_PacketDecoder);
      instance.SetDeleteArray(&deleteArray_PacketDecoder);
      instance.SetDestructor(&destruct_PacketDecoder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PacketDecoder*)
   {
      return GenerateInitInstanceLocal((::PacketDecoder*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PacketDecoder*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PacketDecoder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PacketDecoder::Class_Name()
{
   return "PacketDecoder";
}

//______________________________________________________________________________
const char *PacketDecoder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PacketDecoder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PacketDecoder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PacketDecoder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PacketDecoder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PacketDecoder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PacketDecoder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PacketDecoder*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PacketDecoder::Streamer(TBuffer &R__b)
{
   // Stream an object of class PacketDecoder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PacketDecoder::Class(),this);
   } else {
      R__b.WriteClassBuffer(PacketDecoder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PacketDecoder(void *p) {
      return  p ? new(p) ::PacketDecoder : new ::PacketDecoder;
   }
   static void *newArray_PacketDecoder(Long_t nElements, void *p) {
      return p ? new(p) ::PacketDecoder[nElements] : new ::PacketDecoder[nElements];
   }
   // Wrapper around operator delete
   static void delete_PacketDecoder(void *p) {
      delete ((::PacketDecoder*)p);
   }
   static void deleteArray_PacketDecoder(void *p) {
      delete [] ((::PacketDecoder*)p);
   }
   static void destruct_PacketDecoder(void *p) {
      typedef ::PacketDecoder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PacketDecoder

namespace {
  void TriggerDictionaryInitialization_libPacketDecoder_Impl() {
    static const char* headers[] = {
"PacketDecoder.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/PacketDecoder",
"/storage/ATTPC/DAQ/src/PacketDecoder",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/PacketDecoder/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libPacketDecoder dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$PacketDecoder.h")))  PacketDecoder;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libPacketDecoder dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "PacketDecoder.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"PacketDecoder", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libPacketDecoder",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libPacketDecoder_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libPacketDecoder_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libPacketDecoder() {
  TriggerDictionaryInitialization_libPacketDecoder_Impl();
}
