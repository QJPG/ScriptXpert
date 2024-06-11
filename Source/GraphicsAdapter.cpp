#include "GraphicsAdapter.h"

void GraphicsAdapter::_bindLuaGlobals()
{
	lua_pushinteger(getLuaState(), 0);
	lua_setglobal(getLuaState(), "_GPOSITION_");

	lua_pushinteger(getLuaState(), 1);
	lua_setglobal(getLuaState(), "_GUV_");

	lua_pushinteger(getLuaState(), 2);
	lua_setglobal(getLuaState(), "_GNORMAL_");

	lua_pushinteger(getLuaState(), 3);
	lua_setglobal(getLuaState(), "_GINDEX_");

	lua_pushinteger(getLuaState(), 4);
	lua_setglobal(getLuaState(), "_GCOLOR_");
}

void GraphicsAdapter::_bindLuaFunctions()
{
	lua_register(getLuaState(), "g_compile_shader", L_COMPILESHADER);
	lua_register(getLuaState(), "g_init_obj", L_INITOBJECT);
	lua_register(getLuaState(), "g_init_vgroup", L_INITVDISPATCH);
	lua_register(getLuaState(), "g_init_vdispatch", L_INITVGROUP);
	lua_register(getLuaState(), "g_vdispatch_set", L_VDISPATCHSET);
	lua_register(getLuaState(), "g_vgroup_add", L_VGROUPADD);
	lua_register(getLuaState(), "g_vgroup_erase", L_VGROUPERASE);
	lua_register(getLuaState(), "g_vgroup_set_shader", L_VGROUPSETSHADER);
}

int GraphicsAdapter::L_COMPILESHADER(lua_State* L)
{
	const char* vs_source = luaL_checkstring(L, 1);
	const char* fs_source = luaL_checkstring(L, 2);

	GA_Shader* ptr_GA_shader = new GA_Shader{};

	GLuint vs;
	GLuint fs;
	GLint ok;
	GLchar msg[512];

	//Init VS and Compile

	vs = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &ok);

	if (!ok) {
		glGetShaderInfoLog(vs, 512, NULL, msg);
		std::cerr << "\nGL::Vertex shader comp.\n" << msg << std::endl;
	}

	//Init FS and Compile

	fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);

	if (!ok) {
		glGetShaderInfoLog(fs, 512, NULL, msg);
		std::cerr << "\nGL::Fragment shader comp.\n" << msg << std::endl;
	}

	//Link shaders

	ptr_GA_shader->program = glCreateProgram();
	
	glAttachShader(ptr_GA_shader->program, vs);
	glAttachShader(ptr_GA_shader->program, fs);
	
	glLinkProgram(ptr_GA_shader->program);

	glGetProgramiv(ptr_GA_shader->program, GL_LINK_STATUS, &ok);
	
	if (!ok) {
		glGetProgramInfoLog(ptr_GA_shader->program, 512, NULL, msg);
		std::cerr << "GL::Link Shaders Error.\n" << msg << std::endl;
	}
	
	glDeleteShader(vs);
	glDeleteShader(fs);

	ptr_GA_shader->attribPOSITION = glGetAttribLocation(ptr_GA_shader->program, "POSITION");
	ptr_GA_shader->attribUV = glGetAttribLocation(ptr_GA_shader->program, "UV");
	ptr_GA_shader->attribNORMAL = glGetAttribLocation(ptr_GA_shader->program, "NORMAL");
	ptr_GA_shader->attribCOLOR = glGetAttribLocation(ptr_GA_shader->program, "COLOR");
	ptr_GA_shader->attribINDEX = glGetAttribLocation(ptr_GA_shader->program, "INDEX");

	lua_pushlightuserdata(L, ptr_GA_shader);

	return 1;
}

int GraphicsAdapter::L_INITOBJECT(lua_State* L)
{
	/*
	POSITIONS 0
	UVS 1
	NORMALS 2
	INDICES 3
	COLORS 4
	isStatic?
	shaderPtr
	*/

	VertexDispatcher* ptr_VD = static_cast<VertexDispatcher*>(lua_touserdata(L, 1));

	GA_Object* ptr_GA_object = new GA_Object{};

	for (std::pair<const char*, VertexGroup*> pair : ptr_VD->vgroups) {
		VertexGroup* v = pair.second;

		/*Gen Buffers*/
		glGenVertexArrays(1, &v->vao);
		glGenBuffers(5, v->vbo);

		/*Bind Buffers with data*/
		glBindVertexArray(v->vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[0]);
		glBufferData(
			GL_ARRAY_BUFFER,
			v->positions.size() * sizeof(float),
			v->positions.data(),
			GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[1]);
		glBufferData(
			GL_ARRAY_BUFFER,
			v->uvs.size() * sizeof(float),
			v->uvs.data(),
			GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[2]);
		glBufferData(
			GL_ARRAY_BUFFER,
			v->normals.size() * sizeof(float),
			v->normals.data(),
			GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v->vbo[3]);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			v->indices.size() * sizeof(unsigned int),
			v->indices.data(),
			GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[4]);
		glBufferData(
			GL_ARRAY_BUFFER,
			v->colors.size() * sizeof(float),
			v->colors.data(),
			GL_STATIC_DRAW);

		/*Link Shader Attibuitions for buffers*/
		glUseProgram(v->ptr_GA_shader->program);

		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[0]);
		glEnableVertexAttribArray(v->ptr_GA_shader->attribPOSITION);
		glVertexAttribPointer(
			v->ptr_GA_shader->attribPOSITION,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);

		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[1]);
		glEnableVertexAttribArray(v->ptr_GA_shader->attribUV);
		glVertexAttribPointer(
			v->ptr_GA_shader->attribUV,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);

		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[2]);
		glEnableVertexAttribArray(v->ptr_GA_shader->attribNORMAL);
		glVertexAttribPointer(
			v->ptr_GA_shader->attribNORMAL,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v->vbo[3]);
		glEnableVertexAttribArray(v->ptr_GA_shader->attribINDEX);
		glVertexAttribPointer(
			v->ptr_GA_shader->attribINDEX,
			1,
			GL_UNSIGNED_INT,
			GL_FALSE,
			0,
			0
		);

		glBindBuffer(GL_ARRAY_BUFFER, v->vbo[4]);
		glEnableVertexAttribArray(v->ptr_GA_shader->attribCOLOR);
		glVertexAttribPointer(
			v->ptr_GA_shader->attribCOLOR,
			4,
			GL_FLOAT,
			GL_TRUE,
			0,
			0
		);

		glUseProgram(0);
		glBindVertexArray(0);

	}

	ptr_GA_object->ptr_vd = ptr_VD;

	lua_pushlightuserdata(L, ptr_GA_object);

	return 1;
}

int GraphicsAdapter::L_INITVGROUP(lua_State* L)
{
	VertexGroup* ptr_VG = new VertexGroup{};

	lua_pushlightuserdata(L, ptr_VG);

	return 1;
}

int GraphicsAdapter::L_VGROUPSETSHADER(lua_State* L)
{
	VertexGroup* ptr_VG = static_cast<VertexGroup*>(lua_touserdata(L, 1));
	ptr_VG->ptr_GA_shader = static_cast<GA_Shader*>(lua_touserdata(L, 2));

	return 0;
}

int GraphicsAdapter::L_VGROUPADD(lua_State* L)
{
	VertexGroup* ptr_VG = static_cast<VertexGroup*>(lua_touserdata(L, -1));

	float x, y, z, w;
	unsigned int i;

	switch (luaL_checkinteger(L, 2)) {
		case 0:
			x = luaL_checknumber(L, 3);
			y = luaL_checknumber(L, 4);
			z = luaL_checknumber(L, 5);

			ptr_VG->positions.push_back(x);
			ptr_VG->positions.push_back(y);
			ptr_VG->positions.push_back(z);

			break;

		case 1:
			x = luaL_checknumber(L, 3);
			y = luaL_checknumber(L, 4);

			ptr_VG->uvs.push_back(x);
			ptr_VG->uvs.push_back(y);

			break;

		case 2:
			x = luaL_checknumber(L, 3);
			y = luaL_checknumber(L, 4);
			z = luaL_checknumber(L, 5);

			ptr_VG->normals.push_back(x);
			ptr_VG->normals.push_back(y);
			ptr_VG->normals.push_back(z);

			break;

		case 3:
			i = luaL_checknumber(L, 3);

			ptr_VG->indices.push_back(i);

			break;

		case 4:
			x = luaL_checknumber(L, 3);
			y = luaL_checknumber(L, 4);
			z = luaL_checknumber(L, 5);
			w = luaL_checknumber(L, 6);

			ptr_VG->colors.push_back(x);
			ptr_VG->colors.push_back(y);
			ptr_VG->colors.push_back(z);
			ptr_VG->colors.push_back(w);

			break;
	}

	return 0;
}

int GraphicsAdapter::L_VGROUPERASE(lua_State* L)
{
	VertexGroup* ptr_VG = static_cast<VertexGroup*>(lua_touserdata(L, 1));
	
	int index = luaL_checkinteger(L, 3);

	switch (luaL_checkinteger(L, 2)) {
	case 0:
		ptr_VG->positions.erase(ptr_VG->positions.begin() + index - 3, ptr_VG->positions.begin() + index);

		break;

	case 1:
		ptr_VG->uvs.erase(ptr_VG->uvs.begin() + index - 2, ptr_VG->uvs.begin() + index);

		break;

	case 2:
		ptr_VG->normals.erase(ptr_VG->normals.begin() + index - 3, ptr_VG->normals.begin() + index);

		break;

	case 3:
		ptr_VG->indices.erase(ptr_VG->indices.begin() + index);

		break;

	case 4:
		ptr_VG->colors.erase(ptr_VG->colors.begin() + index - 4, ptr_VG->colors.begin() + index);

		break;
	}

	return 0;
}

int GraphicsAdapter::L_INITVDISPATCH(lua_State* L)
{
	VertexDispatcher* ptr_VD = new VertexDispatcher;

	lua_pushlightuserdata(L, ptr_VD);

	return 1;
}

int GraphicsAdapter::L_VDISPATCHSET(lua_State* L)
{
	VertexDispatcher* ptr_VD = static_cast<VertexDispatcher*>(lua_touserdata(L, 1));
	ptr_VD->vgroups[luaL_checkstring(L, 2)] = static_cast<VertexGroup*>(lua_touserdata(L, 3));

	return 0;
}

GraphicsAdapter::~GraphicsAdapter()
{
}
