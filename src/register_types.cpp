#include "register_types.hpp"

#include "Mob.hpp"
#include "Player.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>

#include "Hud.hpp"
#include "Main.hpp"

using namespace godot;

void initialize_gdextension_types(const ModuleInitializationLevel p_level)
{
  if( p_level != MODULE_INITIALIZATION_LEVEL_SCENE ) return;

  GDREGISTER_CLASS( Player );
  GDREGISTER_CLASS( Mob );
  GDREGISTER_CLASS( Main );
  GDREGISTER_CLASS( HUD );
}

void uninitialize_gdextension_types(const ModuleInitializationLevel p_level)
{
  if( p_level != MODULE_INITIALIZATION_LEVEL_SCENE ) return;
}

extern "C"
{
// Initialization
GDExtensionBool GDE_EXPORT GodotBeginnings(const GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                           GDExtensionClassLibraryPtr p_library,
                                           GDExtensionInitialization *r_initialization)
{
  GDExtensionBinding::InitObject init_obj( p_get_proc_address, p_library, r_initialization );
  init_obj.register_initializer( initialize_gdextension_types );
  init_obj.register_terminator( uninitialize_gdextension_types );
  init_obj.set_minimum_library_initialization_level( MODULE_INITIALIZATION_LEVEL_SCENE );

  return init_obj.init();
}
}
