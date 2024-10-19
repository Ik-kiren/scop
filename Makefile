all :
	g++ main.cpp Shader.cpp Vector3.cpp stb.cpp Vector4.cpp Matrix4.cpp parser.cpp Mesh.cpp ScopMaths.cpp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL

home :
	g++ main.cpp Shader.cpp Vector3.cpp stb.cpp Vector4.cpp Matrix4.cpp parser.cpp Mesh.cpp ScopMaths.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL