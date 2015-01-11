#include "Angel.h"
#include "ObjLoader.h"
#include "Uniform.h"
#include "TextureLoader.h"
#include "Game.h"
#include "LightManager.h"
#include "CubeMesh.h"
#include "Camera.h"
#include "Helpers.h"
#include "TextureManager.h"
#include <iostream>
#include <sstream>

using namespace std;

CubeMesh smallCube;
CubeMesh skybox;
CubeMesh sun;
Camera camera;
LightManager lightManager;

GLuint cubemapTexture;
vec2 mousePos;
vec2 oldCoords(0, 0);
float alpha=0.02;
float beta=0.02;
float h=15;
int size = 6;
int speed = 200;
bool pause;

void gameRound(int n);

void init() {
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
	Shader phongShader("phong-shader.vert", "phong-shader.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");
	
	skybox = CubeMesh(skyboxShader, "cube.obj");
	smallCube = CubeMesh(phongShader, "cube.obj");
	lightManager = LightManager(phongShader);

	camera = Camera(vec3(15,0,0), vec3(0,0,1), 1.0f);

	TextureManager tm;
	cubemapTexture = tm.initCubemapTexture();

	glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

void display( void ) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vector<Cube> cubesToDraw = Game();

	smallCube.activate();
	lightManager.loadLights();
	for (int i=0; i < cubesToDraw.size(); i++)
	{
		Cube c = cubesToDraw[i];
		smallCube.draw(c, camera);
	}

	skybox.activate();
	Cube sky(vec3(), vec4(), 50.0);
	skybox.drawCubeMap(sky, camera, cubemapTexture);

	glutSwapBuffers();
	Angel::CheckError();
}

void mouse( int button, int state, int x, int y ) 
{
	vec3 sperical = toSpherical(camera.eyePoint);
    if(state == GLUT_DOWN)
	{
		pause = true;
		switch(button) 
		{
			case GLUT_LEFT_BUTTON:  
				mousePos.x = x;
				mousePos.y = y;
				break;
			case GLUT_RIGHT_BUTTON:
				break;
		}
    } else if (state == GLUT_UP) {
		pause = false;
	}
}

void mouseMotion(int x, int y)
{
	vec3 sperical = toSpherical(camera.eyePoint);
	float dx = x - mousePos.x;
	float dy = y - mousePos.y;
	sperical[1] = dy * 0.008f;
	sperical[2] = dx * 0.008f;
	camera.eyePoint = toCartesian(sperical);
	glutPostRedisplay();
}

void MouseWheel(int wheel, int direction, int x, int y)
{
    wheel=0;
    if (direction==-1)
    {
        camera.eyePoint *= 1.1;
		h *= 1.1;
    }
    else if (direction==+1)
    {
        camera.eyePoint /= 1.1;
		h /= 1.1;
    }
	glutPostRedisplay();
}

void restart()
{
	camera.eyePoint = vec3(15., 0., 0.);
	camera.up_eye = vec3( 0., 0., 1.);
	SnakeGame::getInstance().initBoard(size);
	pause = false;
}

void printInstruction()
{
	cout << "INSTRUCTION" << endl;
	cout << "Press q to quit" << endl;
	cout << "Press p to pause" << endl;
	cout << "Press + to speed up" << endl;
	cout << "Press - to slow down" << endl;
	cout << "Press [ to lower board size" << endl;
	cout << "Press ] to enlarge board size" << endl;
	cout << "Press r to restart the game" << endl;
	cout << "HAVE FUN!" << endl;
}

void keyboard( unsigned char key, int x, int y ) {
	cout << "Pressed key " << key << endl;
    switch( key ) 
	{
		case 033: // Escape Key
		case 'q': 
		case 'Q':
			exit( EXIT_SUCCESS );
			break;
		case '+':
			speed -= 20;
			if(speed < 100) speed = 100;
			cout << "speed: " << speed << endl;
			break;
		case '-':
			speed += 20;
			if(speed > 600) speed = 600;
			cout << "speed: " << speed << endl;
			break;
		case '4':
			size = 4;
			restart();
			break;
		case '[':
			size -= 1;
			if(size < 4) size = 4;
			restart();
			cout << "size: " << size << endl;
			break;
		case ']':
			size += 1;
			if(size > 13) size = 13;
			restart();
			cout << "size: " << size << endl;
			break;
		case 'C':
		case 'c':
			pause = true;
			gameRound(speed);
		case 'R': // Restart game
		case 'r':
			restart();
			break;
		case 'P': // Pause game
		case 'p':
			pause = !pause;
			break;
	}
}

void specialInput(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			SnakeGame::getInstance().move(Direction::left);
			break;
		case GLUT_KEY_RIGHT:
			SnakeGame::getInstance().move(Direction::right);
			break;
	}
}

void reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
	camera.aspect = float(width)/height;
}

void gameRound(int n)
{
	if(!pause)
	{
		if(SnakeGame::getInstance().round())
			return;
		glutPostRedisplay();
	}
	glutTimerFunc(speed, gameRound, speed);
}

void idle()
{
	if (pause) return;

	Snake snake_temp = SnakeGame::getInstance().getSnake();
	Cube head = snake_temp.cubes.front();
	snake_temp.cubes.pop_front();
	Cube head2 =  snake_temp.cubes.front();
	vec3 dir = head.position - head2.position; 
	Side s = snake_temp.side;

	if (s==pos_x)
	{
		camera.eyePoint += (head.position-camera.eyePoint-vec3(-h,0,0))*alpha;
		camera.up_eye+=(dir-camera.up_eye)*beta;
	}
	else if (s==pos_z)
	{
		camera.eyePoint+=(head.position-camera.eyePoint-vec3(0,0,-h))*alpha;
		camera.up_eye+=(dir-camera.up_eye)*beta;
	}
	else if (s==neg_z)
	{
		camera.eyePoint+=(head.position-camera.eyePoint-vec3(0,0,+h))*alpha;
		camera.up_eye+=(dir-camera.up_eye)*beta;
	}
	else if (s==pos_y)
	{
		camera.eyePoint+=(head.position-camera.eyePoint-vec3(0,-h,0))*alpha;
		camera.up_eye+=(dir-camera.up_eye)*beta;
	}
	else if (s==neg_y)
	{
		camera.eyePoint+=(head.position-camera.eyePoint-vec3(0,+h,0))*alpha;
		camera.up_eye+=(dir-camera.up_eye)*beta;
	}
	else if (s==neg_x)
	{
		camera.eyePoint+=(head.position-camera.eyePoint-vec3(h,0,0))*alpha;
		camera.up_eye+=(dir-camera.up_eye)*beta;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	printInstruction();
    glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    
	glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_3_2_CORE_PROFILE);
	glutCreateWindow("Snake 3D");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutReshapeWindow(512, 512);

	Angel::InitOpenGL();
	
    init();

    glutDisplayFunc(display);
	glutMouseWheelFunc(MouseWheel);
	glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

	//intelligent camera
	glutIdleFunc(idle);
	glutSpecialFunc(specialInput);

	// RUN GAME
	SnakeGame::getInstance().initBoard(size);
	gameRound(speed);

	Angel::CheckError();
    glutMainLoop();
}