#ifndef __LUA_HELPERS_H
#define __LUA_HELPERS_H

/* Some preprocessor magic to help us out on our journey */ 

#define get_boolean_field(field) \
	lua_pushstring(L, #field); \
	lua_gettable(L, -2);	     \
	assert(lua_isboolean(L, -1));		\
	field = lua_toboolean(L, -1);	\
	lua_pop(L, 1);

#define get_int_field(field) \
	lua_pushstring(L, #field); \
	lua_gettable(L, -2);	     \
	assert(lua_isnumber(L, -1));	\
	field = lua_tonumber(L, -1);	\
	lua_pop(L, 1);

#define get_str_field(field) \
	lua_pushstring(L, #field); \
	lua_gettable(L, -2);	     \
	assert(lua_isstring(L, -1));	\
	field = lua_tostring(L, -1);	\
	lua_pop(L, 1);

#define get_str_member(var, memberid, failcode)		\
	lua_pushnumber(L, memberid); \
	lua_gettable(L, -2);	     \
	if (!lua_isstring(L, -1)) { failcode } 	\
	var = lua_tostring(L, -1);	\
	lua_pop(L, 1);

#define get_int_member(var, memberid, failcode)		\
	lua_pushnumber(L, memberid); \
	lua_gettable(L, -2);	     \
	if (!lua_isnumber(L, -1)) { failcode } 	\
	var = lua_tonumber(L, -1);	\
	lua_pop(L, 1);


#endif
