#pragma once
#include "ProgramAdapter.h"

#include <glad/glad.h>

class GraphicsAdapter :
	public ProgramAdapter
{
private:
	struct GA_Shader {
		GLuint program;

		GLuint attribPOSITION;
		GLuint attribNORMAL;
		GLuint attribUV;
		GLuint attribINDEX;
		GLuint attribCOLOR;
	};

	struct VertexGroup {
		GLuint vao;
		GLuint vbo[5];

		std::vector<float> positions;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
		std::vector<float> colors;

		GA_Shader* ptr_GA_shader;
	};

	struct VertexDispatcher {
		std::map<const char*, VertexGroup*> vgroups;
	};

	struct GA_Object {
		VertexDispatcher* ptr_vd;
		float* model; //Matrix Model to: gl_Position = (proj * view * model * vec4(POSITION, 1.0)).xyz;
	};

	void _bindLuaGlobals() override;
	void _bindLuaFunctions() override;

	static int L_COMPILESHADER(lua_State* L);
	static int L_INITOBJECT(lua_State* L);
	static int L_INITVGROUP(lua_State* L);
	static int L_VGROUPSETSHADER(lua_State* L);
	static int L_VGROUPADD(lua_State* L);
	static int L_VGROUPERASE(lua_State* L);
	static int L_INITVDISPATCH(lua_State* L);
	static int L_VDISPATCHSET(lua_State* L);

public:
	~GraphicsAdapter();
};

