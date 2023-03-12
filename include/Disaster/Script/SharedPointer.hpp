#ifndef PX_DISASTER_SCRIPT_SHARED_PTR_PROXY_HPP
#define PX_DISASTER_SCRIPT_SHARED_PTR_PROXY_HPP
#include <string>
#include <memory>

#include <angelscript.h>

namespace px::disaster::script {
  namespace priv {
    template<class T>
    struct SharedPointerWrapper {
      static void Construct(void *memory) {
        new (memory) std::shared_ptr<T>();
      }
      static void Destruct(void *memory) {
        reinterpret_cast<std::__shared_ptr<T>*>(memory)->~__shared_ptr();
      }
    };
  }

  template<class Type>
  void RegisterSharedPointer(const std::string &ptrTypeName, const std::string &typeName, asIScriptEngine *engine) {
    int r;

    r = engine->RegisterObjectType(
      ptrTypeName.c_str(), 
      sizeof(std::shared_ptr<Type>), 
      asOBJ_VALUE | asOBJ_APP_CLASS_CDA
    ); assert(r >= 0);

    r = engine->RegisterObjectBehaviour(
      ptrTypeName.c_str(), 
      asBEHAVE_CONSTRUCT, 
      "void f()", 
      asFUNCTION(priv::SharedPointerWrapper<Type>::Construct), 
      asCALL_CDECL_OBJLAST
    ); assert(r >= 0);

    r = engine->RegisterObjectBehaviour(
      ptrTypeName.c_str(), 
      asBEHAVE_DESTRUCT, 
      "void f()", 
      asFUNCTION(priv::SharedPointerWrapper<Type>::Destruct), 
      asCALL_CDECL_OBJLAST
    ); assert(r >= 0);

    r = engine->RegisterObjectMethod(
      ptrTypeName.c_str(),
      (ptrTypeName + "& opAssign(const " + ptrTypeName + " &in other)").c_str(),
      asMETHODPR(std::shared_ptr<Type>, operator =, (const std::shared_ptr<Type> &), std::shared_ptr<Type> &),
      asCALL_THISCALL
    ); assert(r >= 0);

    r = engine->RegisterObjectMethod(
      ptrTypeName.c_str(),
      (typeName + "& get()").c_str(),
      asMETHOD(std::shared_ptr<Type>, get),
      asCALL_THISCALL
    ); assert(r >= 0);
  }
}

#endif // PX_DISASTER_SCRIPT_SHARED_PTR_PROXY_HPP