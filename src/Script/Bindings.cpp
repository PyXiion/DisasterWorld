#include <Disaster/Script/Bindings.hpp>
#include <cassert>

#include <Disaster/Program.hpp>
#include <Disaster/AppConsole.hpp>
#include <Disaster/Gameplay/Chunk.hpp>
#include <Disaster/Gameplay/World.hpp>
#include <Disaster/Gameplay/Game.hpp> 
#include <angelscript.h>

#define AS_CHECK(...) r = __VA_ARGS__; assert(r >= 0)
#define WHOLE(...) __VA_ARGS__

namespace px::disaster::script {
  using namespace gameplay;

  template<class T, class ...TArgs>
  void Constructor(void *memory, TArgs... args) {
    new(memory) T(args...);
  }
  template<class T>
  void Destructor(void *memory) {
    reinterpret_cast<T*>(memory)->~T();
  }

  // System section
  void RegisterColor(asIScriptEngine *engine) {
    int r;
    AS_CHECK(engine->RegisterObjectType("Color", sizeof(Color), asOBJ_VALUE | asOBJ_POD));

    AS_CHECK(engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Constructor<Color>), asCALL_CDECL_OBJFIRST));
    AS_CHECK(engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, 
            "void f(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a = 255)", 
            asFUNCTION(WHOLE(Constructor<Color, ColorComponent, ColorComponent, ColorComponent, ColorComponent>)), asCALL_CDECL_OBJFIRST));

    AS_CHECK(engine->RegisterObjectProperty("Color", "ColorComponent r", asOFFSET(Color, r)));
    AS_CHECK(engine->RegisterObjectProperty("Color", "ColorComponent g", asOFFSET(Color, g)));
    AS_CHECK(engine->RegisterObjectProperty("Color", "ColorComponent b", asOFFSET(Color, b)));
    AS_CHECK(engine->RegisterObjectProperty("Color", "ColorComponent a", asOFFSET(Color, a)));
  }
  template<class T>
  void RegisterVector(std::string name, std::string tname, asIScriptEngine *engine) {
  }
  template<class T>
  void RegisterRect(std::string name, std::string vname, std::string tname, asIScriptEngine *engine) {
    int r;
    AS_CHECK(engine->RegisterObjectType(name.c_str(), sizeof(Rect<T>), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Rect<T>>()));

    AS_CHECK(engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Constructor<Rect<T>>), asCALL_CDECL_OBJFIRST));
    AS_CHECK(engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, 
            ("void f(" + tname + "left, " + tname + "top, " + tname + "width, " + tname + "height)").c_str(), 
            asFUNCTION(WHOLE(Constructor<Rect<T>, T, T, T, T>)), asCALL_CDECL_OBJFIRST));
    
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            ("bool Contains(" + vname + " point) const").c_str(),
            asMETHODPR(Rect<T>, Contains, (Vector2<T>) const, bool), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            ("bool Contains(" + tname + " x, " + tname + " y) const").c_str(),
            asMETHODPR(Rect<T>, Contains, (T, T) const, bool), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            ("bool Intersects(" + name + " otherRect) const").c_str(),
            asMETHOD(Rect<T>, Intersects), asCALL_THISCALL));
    
    AS_CHECK(engine->RegisterObjectProperty(name.c_str(), (tname + " left").c_str(), asOFFSET(Rect<T>, left)));
    AS_CHECK(engine->RegisterObjectProperty(name.c_str(), (tname + " top").c_str(), asOFFSET(Rect<T>, top)));
    AS_CHECK(engine->RegisterObjectProperty(name.c_str(), (tname + " width").c_str(), asOFFSET(Rect<T>, width)));
    AS_CHECK(engine->RegisterObjectProperty(name.c_str(), (tname + " height").c_str(), asOFFSET(Rect<T>, height)));
  }
  void RegisterSystem(asIScriptEngine *engine) {
    RegisterColor(engine);

    RegisterVector<int>("Vector2i", "int", engine);
    RegisterVector<uint>("Vector2u", "uint", engine);
    RegisterVector<float>("Vector2f", "float", engine);
    RegisterVector<double>("Vector2d", "double", engine);

    RegisterRect<int>("IntRect", "Vector2i", "int", engine);
    RegisterRect<uint>("UintRect", "Vector2u", "uint", engine);
    RegisterRect<float>("FloatRect", "Vector2f", "float", engine);
  }

  // Gameplay section
  void RegisterChunk(asIScriptEngine *engine) {
    int r;

    AS_CHECK(engine->RegisterObjectType("CChunk", 0, asOBJ_REF | asOBJ_NOCOUNT));

    AS_CHECK(engine->RegisterObjectMethod("CChunk", "TileID GetTile(int x, int y) const", asMETHOD(Chunk, GetTile), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod("CChunk", "void SetTile(int x, int y, TileID id)", asMETHOD(Chunk, SetTile), asCALL_THISCALL));

    AS_CHECK(engine->RegisterObjectMethod("CChunk", "int get_x() const", asMETHOD(Chunk, GetX), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod("CChunk", "int get_y() const", asMETHOD(Chunk, GetY), asCALL_THISCALL));
  }
  void RegisterWorld(asIScriptEngine *engine) {
    int r;

    AS_CHECK(engine->RegisterObjectType("CWorld", 0, asOBJ_REF | asOBJ_NOHANDLE));

    AS_CHECK(engine->RegisterObjectMethod("CWorld", "CChunk @GetChunk(float x, float y)", asMETHOD(World, GetChunk), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod("CWorld", "bool RequestChunk(int x, int y)", asMETHOD(World, RequestChunk), asCALL_THISCALL));
  }
  void RegisterGame(asIScriptEngine *engine) {
    int r;

    AS_CHECK(engine->RegisterObjectType("CGame", 0, asOBJ_REF | asOBJ_NOHANDLE));

    AS_CHECK(engine->RegisterObjectMethod("CGame", "CWorld &get_world() property", asMETHOD(Game, GetWorld), asCALL_THISCALL));
  }
  void RegisterGameplay(asIScriptEngine *engine) {
    RegisterChunk(engine);
    RegisterWorld(engine);
    RegisterGame(engine);
  }

  void ConfigureEngine(ScriptEngine &scriptEngine) {
    asIScriptEngine *engine = scriptEngine.Get();
    int r;

    // Typedefs
    AS_CHECK(engine->RegisterTypedef("TileID", "uint16"));
    AS_CHECK(engine->RegisterTypedef("ColorComponent", "uint8"));

    RegisterSystem(engine);
    RegisterGameplay(engine);

    // Global properties
    AS_CHECK(engine->RegisterGlobalProperty("CGame game", &Program::GetInstance().GetGame()));
  }
}