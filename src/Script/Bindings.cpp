#include <Disaster/Script/Bindings.hpp>
#include <cassert>

#include <Disaster/Program.hpp>
#include <Disaster/AppConsole.hpp>
#include <Disaster/Gameplay/Chunk.hpp>
#include <Disaster/Gameplay/World.hpp>
#include <Disaster/Gameplay/Game.hpp> 
#include <Disaster/Graphics/Sprite.hpp>
#include <Disaster/Graphics/Texture.hpp>
#include <Disaster/Script/SharedPointer.hpp>
#include <Disaster/Utils/ResourceManager.hpp>
#include <angelscript.h>

#define AS_CHECK(...) r = __VA_ARGS__; assert(r >= 0)
#define WHOLE(...) __VA_ARGS__

namespace px::disaster::script {
  using namespace gameplay;

  /// @brief Proxy template constructor of any class for AngelScript
  /// @tparam T class type
  /// @tparam ...TArgs class constuctor arguments
  template<class T, class ...TArgs>
  void Constructor(void *memory, TArgs... args) {
    new(memory) T(args...);
  }
  
  /// @brief Proxy template destructor of any class for AngelScript
  /// @tparam T class type
  template<class T>
  void Destructor(void *memory) {
    reinterpret_cast<T*>(memory)->~T();
  }

  // System section
  /// @brief Registration of Color type in AngelScript. The constructor and members are registered.
  /// @param engine AngelScript engine
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
  
  /// @brief Registration of Vector2 in AngelScript. 
  /// Registers constructor, members and overloaded operators. 
  /// You need to specify T - type of members (Vector<float> = Vector2f), 
  /// the name by which the class will be registered, name of vector with similar type of members, names of member types 
  /// for AngelScript and flags for additional AngelScript flags (such as asOBJ_APP_CLASS_ALLFLOATS for FloatRect)
  /// @tparam T rect type
  /// @param name registered name
  /// @param tname name of member type
  /// @param vname name of vector with similar type of members (for FloatRect is Vector2f)
  /// @param engine AngelScript engine
  /// @param flags Additional flags
  template<class T>
  void RegisterVector(std::string name, std::string tname, asIScriptEngine *engine, asDWORD flags) {
    int r;
    AS_CHECK(engine->RegisterObjectType(name.c_str(), sizeof(Vector2<T>), asOBJ_VALUE | asOBJ_POD | flags | asGetTypeTraits<Vector2<T>>()));

    AS_CHECK(engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Constructor<Vector2<T>>), asCALL_CDECL_OBJFIRST));
    AS_CHECK(engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, 
            ("void f(" + tname + " y, " + tname + " x)").c_str(), 
            asFUNCTION(WHOLE(Constructor<Vector2<T>, T, T>)), asCALL_CDECL_OBJFIRST));
    
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            (tname + " get_length() const").c_str(),
            asMETHOD(Vector2<T>, Length), asCALL_THISCALL));

    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            (name + " opNeg() const").c_str(), asMETHODPR(Vector2<T>, operator-, () const, Vector2<T>), asCALL_THISCALL));
    
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            (name + " opAdd(" + name + ") const").c_str(), asMETHOD(Vector2<T>, operator+), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            (name + " opSub(" + name + ") const").c_str(), asMETHODPR(Vector2<T>, operator-, (const Vector2<T>&) const, Vector2<T>), asCALL_THISCALL));

    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            (name + " opMul(" + tname + ") const").c_str(), asMETHOD(Vector2<T>, operator*), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod(name.c_str(), 
            (name + " opDiv(" + tname + ") const").c_str(), asMETHOD(Vector2<T>, operator/), asCALL_THISCALL));
    
    AS_CHECK(engine->RegisterObjectProperty(name.c_str(), (tname + " x").c_str(), asOFFSET(Vector2<T>, x)));
    AS_CHECK(engine->RegisterObjectProperty(name.c_str(), (tname + " y").c_str(), asOFFSET(Vector2<T>, y)));
  }
  
  /// @brief Registration of Rect in AngelScript. 
  /// Registers constructor, members and methods. 
  /// You need to specify T - type of members (Rect<float> = FloatRect), 
  /// the name by which the class will be registered, name of , names of member types 
  /// for AngelScript and flags for additional AngelScript flags (such as asOBJ_APP_CLASS_ALLFLOATS for Vector2f)
  /// @tparam T vector type
  /// @param name registered name
  /// @param tname name of member type
  /// @param engine AngelScript engine
  /// @param flags Additional flags
  template<class T>
  void RegisterRect(std::string name, std::string vname, std::string tname, asIScriptEngine *engine, asDWORD flags) {
    int r;
    AS_CHECK(engine->RegisterObjectType(name.c_str(), sizeof(Rect<T>), asOBJ_VALUE | asOBJ_POD | flags | asGetTypeTraits<Rect<T>>()));

    AS_CHECK(engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Constructor<Rect<T>>), asCALL_CDECL_OBJFIRST));
    AS_CHECK(engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, 
            ("void f(" + tname + " left, " + tname + " top, " + tname + " width, " + tname + " height)").c_str(), 
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
  
  /// @brief Registers all classes from the Disaster/System in AngelScript
  /// @param engine AngelScript engine
  void RegisterSystem(asIScriptEngine *engine) {
    RegisterColor(engine);

    RegisterVector<int>("Vector2i", "int", engine, asOBJ_APP_CLASS_ALLINTS);
    RegisterVector<uint>("Vector2u", "uint", engine, asOBJ_APP_CLASS_ALLINTS);
    RegisterVector<float>("Vector2f", "float", engine, asOBJ_APP_CLASS_ALLFLOATS);
    RegisterVector<double>("Vector2d", "double", engine, asOBJ_APP_CLASS_ALLFLOATS);

    RegisterRect<int>("IntRect", "Vector2i", "int", engine, asOBJ_APP_CLASS_ALLINTS);
    RegisterRect<uint>("UintRect", "Vector2u", "uint", engine, asOBJ_APP_CLASS_ALLINTS);
    RegisterRect<float>("FloatRect", "Vector2f", "float", engine, asOBJ_APP_CLASS_ALLFLOATS);
  }

  // Graphics section
  void RegisterSprite(asIScriptEngine *engine) {

  }

  void RegisterGraphics(asIScriptEngine *engine) {

  }

  // Gameplay section
  /// @brief Register the Chunk class as CChunk (the prefix means the class can only be created from C++ code, 
  /// i.e. no constructors or AngelScript doesn't handle this object)
  /// @param engine AngelScript engine
  void RegisterChunk(asIScriptEngine *engine) {
    int r;

    AS_CHECK(engine->RegisterObjectType("CChunk", 0, asOBJ_REF | asOBJ_NOCOUNT));

    AS_CHECK(engine->RegisterObjectMethod("CChunk", "TileID GetTile(int x, int y) const", asMETHOD(Chunk, GetTile), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod("CChunk", "void SetTile(int x, int y, TileID id)", asMETHOD(Chunk, SetTile), asCALL_THISCALL));

    AS_CHECK(engine->RegisterObjectMethod("CChunk", "Vector2i get_position() const", asMETHOD(Chunk, GetPosition), asCALL_THISCALL));

    RegisterSharedPointer<Chunk>("ChunkPtr", "CChunk", engine);
  }
  
  /// @brief Registers the World class as CWorld (the prefix means the class can only be created from C++ code, 
  /// i.e. no constructors or AngelScript doesn't handle this object)
  /// @param engine AngelScript engine
  void RegisterWorld(asIScriptEngine *engine) {
    int r;
    AS_CHECK(engine->RegisterObjectType("CWorld", 0, asOBJ_REF | asOBJ_NOHANDLE));

    AS_CHECK(engine->RegisterObjectMethod("CWorld", "ChunkPtr GetChunk(Vector2i position)", asMETHOD(World, GetChunk), asCALL_THISCALL));
    AS_CHECK(engine->RegisterObjectMethod("CWorld", "bool RequestChunk(Vector2i position)", asMETHOD(World, RequestChunk), asCALL_THISCALL));
  }
  
  /// @brief Registers the Game class as CGame (the prefix means the class can only be created from C++ code, 
  /// i.e. no constructors or AngelScript doesn't handle this object)
  /// @param engine AngelScript engine
  void RegisterGame(asIScriptEngine *engine) {
    int r;
    AS_CHECK(engine->RegisterObjectType("CGame", 0, asOBJ_REF | asOBJ_NOHANDLE));

    AS_CHECK(engine->RegisterObjectMethod("CGame", "CWorld &get_world() property", asMETHOD(Game, GetWorld), asCALL_THISCALL));
  }
  
  /// @brief Registers all classes from the Disaster/Gameplay in AngelScript
  /// @param engine AngelScript engine
  void RegisterGameplay(asIScriptEngine *engine) {
    RegisterChunk(engine);
    RegisterWorld(engine);
    RegisterGame(engine);
  }

  // Utils section
  void RegisterResourceManager(asIScriptEngine *engine) {
    // int r;
    // AS_CHECK(engine->RegisterObjectType("CResourceManager", 0, asOBJ_REF | asOBJ_NOHANDLE));

    // AS_CHECK(engine->RegisterObjectMethod("CResourceManager", "Texture &"))
  }
  void RegisterUtils(asIScriptEngine *engine) {

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