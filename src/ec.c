#include <openssl/objects.h>

#include "ec.h"

#ifndef OPENSSL_NO_ECDH

EC_KEY *lsec_find_ec_key(lua_State *L, const char *str)
{
  int nid;
  lua_pushstring(L, "SSL:EC:CURVES");
  lua_rawget(L, LUA_REGISTRYINDEX);
  lua_pushstring(L, str);
  lua_rawget(L, -2);

  if (!lua_isnumber(L, -1))
    return NULL;

  nid = (int)lua_tonumber(L, -1);
  return EC_KEY_new_by_curve_name(nid);
}

void lsec_load_curves(lua_State *L)
{
  size_t i;
  size_t size;
  const char *name;
  EC_builtin_curve *curves = NULL;

  lua_pushstring(L, "SSL:EC:CURVES");
  lua_newtable(L);

  size = EC_get_builtin_curves(NULL, 0);
  if (size > 0) {
    curves = (EC_builtin_curve*)malloc(sizeof(EC_builtin_curve) * size);
    EC_get_builtin_curves(curves, size);
    for (i = 0; i < size; i++) {
      name = OBJ_nid2sn(curves[i].nid);
      if (name != NULL) {
        lua_pushstring(L, name);
        lua_pushnumber(L, curves[i].nid);
        lua_rawset(L, -3);
      }
    }
    free(curves);
  }

  lua_rawset(L,  LUA_REGISTRYINDEX);
}

#else

void lsec_load_curves(lua_State *L)
{
  // do nothing
}

#endif