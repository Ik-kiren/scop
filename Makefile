CC = g++
NAME = Scop
SRCS = srcs/main.cpp srcs/Shader.cpp srcs/Vector3.cpp srcs/stb.cpp srcs/Vector4.cpp srcs/Matrix4.cpp srcs/Mesh.cpp srcs/ScopMaths.cpp srcs/Camera.cpp srcs/Object.cpp
FLAGS = -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL
OBJS = $(SRCS:cpp=o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(NAME)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re: fclean all

home :
	g++ main.cpp Shader.cpp Vector3.cpp stb.cpp Vector4.cpp Matrix4.cpp Mesh.cpp ScopMaths.cpp Camera.cpp Object.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL

.PHONY: all clean fclean re home