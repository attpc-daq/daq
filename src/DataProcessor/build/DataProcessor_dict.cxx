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
         instance("DataProcessor", ::DataProcessor::Class_Version(), "DataProcessor.h", 28,
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

namespace ROOT {
   static TClass *vectorlEmaplEstringcOintgRsPgR_Dictionary();
   static void vectorlEmaplEstringcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEmaplEstringcOintgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEmaplEstringcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEmaplEstringcOintgRsPgR(void *p);
   static void deleteArray_vectorlEmaplEstringcOintgRsPgR(void *p);
   static void destruct_vectorlEmaplEstringcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<map<string,int> >*)
   {
      vector<map<string,int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<map<string,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<map<string,int> >", -2, "vector", 389,
                  typeid(vector<map<string,int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmaplEstringcOintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<map<string,int> >) );
      instance.SetNew(&new_vectorlEmaplEstringcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEmaplEstringcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEmaplEstringcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmaplEstringcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEmaplEstringcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<map<string,int> > >()));

      ::ROOT::AddClassAlternate("vector<map<string,int> >","std::vector<std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > >, std::allocator<std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<map<string,int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmaplEstringcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<map<string,int> >*)nullptr)->GetClass();
      vectorlEmaplEstringcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmaplEstringcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmaplEstringcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<map<string,int> > : new vector<map<string,int> >;
   }
   static void *newArray_vectorlEmaplEstringcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<map<string,int> >[nElements] : new vector<map<string,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmaplEstringcOintgRsPgR(void *p) {
      delete ((vector<map<string,int> >*)p);
   }
   static void deleteArray_vectorlEmaplEstringcOintgRsPgR(void *p) {
      delete [] ((vector<map<string,int> >*)p);
   }
   static void destruct_vectorlEmaplEstringcOintgRsPgR(void *p) {
      typedef vector<map<string,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<map<string,int> >

namespace ROOT {
   static TClass *vectorlERawEventmUgR_Dictionary();
   static void vectorlERawEventmUgR_TClassManip(TClass*);
   static void *new_vectorlERawEventmUgR(void *p = nullptr);
   static void *newArray_vectorlERawEventmUgR(Long_t size, void *p);
   static void delete_vectorlERawEventmUgR(void *p);
   static void deleteArray_vectorlERawEventmUgR(void *p);
   static void destruct_vectorlERawEventmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<RawEvent*>*)
   {
      vector<RawEvent*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<RawEvent*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<RawEvent*>", -2, "vector", 389,
                  typeid(vector<RawEvent*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlERawEventmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<RawEvent*>) );
      instance.SetNew(&new_vectorlERawEventmUgR);
      instance.SetNewArray(&newArray_vectorlERawEventmUgR);
      instance.SetDelete(&delete_vectorlERawEventmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlERawEventmUgR);
      instance.SetDestructor(&destruct_vectorlERawEventmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<RawEvent*> >()));

      ::ROOT::AddClassAlternate("vector<RawEvent*>","std::vector<RawEvent*, std::allocator<RawEvent*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<RawEvent*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlERawEventmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<RawEvent*>*)nullptr)->GetClass();
      vectorlERawEventmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlERawEventmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlERawEventmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RawEvent*> : new vector<RawEvent*>;
   }
   static void *newArray_vectorlERawEventmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RawEvent*>[nElements] : new vector<RawEvent*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlERawEventmUgR(void *p) {
      delete ((vector<RawEvent*>*)p);
   }
   static void deleteArray_vectorlERawEventmUgR(void *p) {
      delete [] ((vector<RawEvent*>*)p);
   }
   static void destruct_vectorlERawEventmUgR(void *p) {
      typedef vector<RawEvent*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<RawEvent*>

namespace ROOT {
   static TClass *maplEstringcOintgR_Dictionary();
   static void maplEstringcOintgR_TClassManip(TClass*);
   static void *new_maplEstringcOintgR(void *p = nullptr);
   static void *newArray_maplEstringcOintgR(Long_t size, void *p);
   static void delete_maplEstringcOintgR(void *p);
   static void deleteArray_maplEstringcOintgR(void *p);
   static void destruct_maplEstringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,int>*)
   {
      map<string,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,int>", -2, "map", 100,
                  typeid(map<string,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,int>) );
      instance.SetNew(&new_maplEstringcOintgR);
      instance.SetNewArray(&newArray_maplEstringcOintgR);
      instance.SetDelete(&delete_maplEstringcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOintgR);
      instance.SetDestructor(&destruct_maplEstringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,int> >()));

      ::ROOT::AddClassAlternate("map<string,int>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,int>*)nullptr)->GetClass();
      maplEstringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,int> : new map<string,int>;
   }
   static void *newArray_maplEstringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,int>[nElements] : new map<string,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOintgR(void *p) {
      delete ((map<string,int>*)p);
   }
   static void deleteArray_maplEstringcOintgR(void *p) {
      delete [] ((map<string,int>*)p);
   }
   static void destruct_maplEstringcOintgR(void *p) {
      typedef map<string,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,int>

namespace {
  void TriggerDictionaryInitialization_libDataProcessor_Impl() {
    static const char* headers[] = {
"DataProcessor.h",
nullptr
    };
    static const char* includePaths[] = {
"/storage/ATTPC/DAQ/src/DataProcessor",
"/storage/ATTPC/DAQ/src/DataProcessor",
"/opt/root/include",
"/opt/root/include/",
"/storage/ATTPC/DAQ/src/DataProcessor/build/",
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
