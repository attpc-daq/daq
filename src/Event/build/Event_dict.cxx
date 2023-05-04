// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Event_dict
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
#include "Pad.h"
#include "Event.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_Pad(void *p = nullptr);
   static void *newArray_Pad(Long_t size, void *p);
   static void delete_Pad(void *p);
   static void deleteArray_Pad(void *p);
   static void destruct_Pad(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Pad*)
   {
      ::Pad *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Pad >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Pad", ::Pad::Class_Version(), "Pad.h", 8,
                  typeid(::Pad), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Pad::Dictionary, isa_proxy, 4,
                  sizeof(::Pad) );
      instance.SetNew(&new_Pad);
      instance.SetNewArray(&newArray_Pad);
      instance.SetDelete(&delete_Pad);
      instance.SetDeleteArray(&deleteArray_Pad);
      instance.SetDestructor(&destruct_Pad);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Pad*)
   {
      return GenerateInitInstanceLocal((::Pad*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Pad*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Event(void *p = nullptr);
   static void *newArray_Event(Long_t size, void *p);
   static void delete_Event(void *p);
   static void deleteArray_Event(void *p);
   static void destruct_Event(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Event*)
   {
      ::Event *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Event >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Event", ::Event::Class_Version(), "Event.h", 10,
                  typeid(::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Event::Dictionary, isa_proxy, 4,
                  sizeof(::Event) );
      instance.SetNew(&new_Event);
      instance.SetNewArray(&newArray_Event);
      instance.SetDelete(&delete_Event);
      instance.SetDeleteArray(&deleteArray_Event);
      instance.SetDestructor(&destruct_Event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Event*)
   {
      return GenerateInitInstanceLocal((::Event*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Event*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Pad::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Pad::Class_Name()
{
   return "Pad";
}

//______________________________________________________________________________
const char *Pad::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Pad*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Pad::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Pad*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Pad::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Pad*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Pad::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Pad*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Event::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Event::Class_Name()
{
   return "Event";
}

//______________________________________________________________________________
const char *Event::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Event::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Event::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Event::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Pad::Streamer(TBuffer &R__b)
{
   // Stream an object of class Pad.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Pad::Class(),this);
   } else {
      R__b.WriteClassBuffer(Pad::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Pad(void *p) {
      return  p ? new(p) ::Pad : new ::Pad;
   }
   static void *newArray_Pad(Long_t nElements, void *p) {
      return p ? new(p) ::Pad[nElements] : new ::Pad[nElements];
   }
   // Wrapper around operator delete
   static void delete_Pad(void *p) {
      delete ((::Pad*)p);
   }
   static void deleteArray_Pad(void *p) {
      delete [] ((::Pad*)p);
   }
   static void destruct_Pad(void *p) {
      typedef ::Pad current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Pad

//______________________________________________________________________________
void Event::Streamer(TBuffer &R__b)
{
   // Stream an object of class Event.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Event::Class(),this);
   } else {
      R__b.WriteClassBuffer(Event::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Event(void *p) {
      return  p ? new(p) ::Event : new ::Event;
   }
   static void *newArray_Event(Long_t nElements, void *p) {
      return p ? new(p) ::Event[nElements] : new ::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_Event(void *p) {
      delete ((::Event*)p);
   }
   static void deleteArray_Event(void *p) {
      delete [] ((::Event*)p);
   }
   static void destruct_Event(void *p) {
      typedef ::Event current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Event

namespace ROOT {
   static TClass *vectorlEPadgR_Dictionary();
   static void vectorlEPadgR_TClassManip(TClass*);
   static void *new_vectorlEPadgR(void *p = nullptr);
   static void *newArray_vectorlEPadgR(Long_t size, void *p);
   static void delete_vectorlEPadgR(void *p);
   static void deleteArray_vectorlEPadgR(void *p);
   static void destruct_vectorlEPadgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Pad>*)
   {
      vector<Pad> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Pad>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Pad>", -2, "vector", 389,
                  typeid(vector<Pad>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPadgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Pad>) );
      instance.SetNew(&new_vectorlEPadgR);
      instance.SetNewArray(&newArray_vectorlEPadgR);
      instance.SetDelete(&delete_vectorlEPadgR);
      instance.SetDeleteArray(&deleteArray_vectorlEPadgR);
      instance.SetDestructor(&destruct_vectorlEPadgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Pad> >()));

      ::ROOT::AddClassAlternate("vector<Pad>","std::vector<Pad, std::allocator<Pad> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Pad>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPadgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Pad>*)nullptr)->GetClass();
      vectorlEPadgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPadgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPadgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Pad> : new vector<Pad>;
   }
   static void *newArray_vectorlEPadgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Pad>[nElements] : new vector<Pad>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPadgR(void *p) {
      delete ((vector<Pad>*)p);
   }
   static void deleteArray_vectorlEPadgR(void *p) {
      delete [] ((vector<Pad>*)p);
   }
   static void destruct_vectorlEPadgR(void *p) {
      typedef vector<Pad> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Pad>

namespace {
  void TriggerDictionaryInitialization_libEvent_Impl() {
    static const char* headers[] = {
"Pad.h",
"Event.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/Event",
"/storage/ATTPC/DAQ/src/Event",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/Event/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libEvent dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Pad.h")))  Pad;
class __attribute__((annotate("$clingAutoload$Event.h")))  Event;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libEvent dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Pad.h"
#include "Event.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Event", payloadCode, "@",
"Pad", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libEvent",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libEvent_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libEvent_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libEvent() {
  TriggerDictionaryInitialization_libEvent_Impl();
}
