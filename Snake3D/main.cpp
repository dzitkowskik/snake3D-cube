// 02561-04-04
#include "Angel.h"
#include "ObjLoader.h"
#include "Uniform.h"
#include "Game.h"
#include <iostream>
#include <sstream>

using namespace std;

float aspect = 1;

GLuint ModelView, Projection;
GLuint program;
vector<int> meshIndices;

vec3 eyePoint(2., 0., 0.);
vec3 lightPos(13, 13, 13);
vec2 mousePos;
vec2 oldCoords(0, 0);



struct Light 
{
	vec3 position;
	vec3 color;
	float lightType;
	float attenuation;
	float ambientCoefficient;
};
std::vector<Light> lights;

// FUNCTIONS
vec3 toSpherical(vec3 cartesian);
vec3 toCartesian(vec3 spherical);



void loadModel()
{
	const char* modelName = "cube.obj";
	cout << "Loading model " << modelName << endl;
	std::vector<vec3> outPositions;

	std::vector<vec3> outNormal;
	std::vector<vec2> outUv;

	loadObject(modelName, outPositions, meshIndices, outNormal, outUv, 0.5f);

	// Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec3) * outPositions.size() * 2,
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec3) * outPositions.size(), &outPositions[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec3) * outPositions.size(),
		     sizeof(vec3) * outPositions.size(), &outNormal[0] );

	// set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
	if (vNormal != GL_INVALID_INDEX ){
		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
				   BUFFER_OFFSET(sizeof(vec3) * outPositions.size()) );
	}
}

template<typename T>
void setLightUniform(const char* uniformName, size_t index, const T& value)
{
	std::ostringstream ss;
	ss << "Lights[" << index << "]." << uniformName;
	std::string name = ss.str();
	_Uniform<T> uniformSetter;

	uniformSetter.Register(program, name.c_str());
	uniformSetter.SetData(value);
}

void loadLights()
{
	lights.clear();

	Light first;
	first.position = -lightPos;
	first.color = vec3(.75, .75, .75);
	first.ambientCoefficient = 0.08f;
	first.attenuation = 0.001f;
	first.lightType = 1.0;
	lights.push_back(first);

	Light second;
	second.position = lightPos;
	second.color = vec3(.75, .75, .75);
	second.ambientCoefficient = 0.08f;
	second.attenuation = 0.001f;
	second.lightType = 1.0;
	lights.push_back(second);


	for(size_t i = 0; i < lights.size(); ++i)
	{
		setLightUniform("position",i,lights[i].position);
		setLightUniform("color",i,lights[i].color);
		setLightUniform("lightType",i,lights[i].lightType);
		setLightUniform("attenuation",i,lights[i].attenuation);
		setLightUniform("ambientCoefficient",i,lights[i].ambientCoefficient);
	}

	glUniform1i( glGetUniformLocation(program, "NumLights"), lights.size() );
}

void reloadShader(){
	// Load shaders and use the resulting shader program
    program = InitShader( "phong-shader.vert", "phong-shader.frag", "fragColor" );
    glUseProgram( program );
	loadModel();
	loadLights();
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
}

void init() {
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace( GL_BACK/*GL_FRONT  or GL_BACK or even GL_FRONT_AND_BACK */);
    reloadShader();
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

void drawLights()
{
	for(int i=0; i<lights.size(); i++)
	{
		Light l = lights[i];
		vec4 eye = vec4(eyePoint);
		vec4 at( 0., 0., 0.,1);
		vec4 up( 0., 1., 0.,0);
		mat4 modelView = LookAt(eye, at, up);
		modelView *= Angel::Scale(0.25, 0.25, 0.25);

		modelView *= Angel::Translate(l.position);
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);

		vec4 material_color(1, 1, 0, 1.0);
		vec4 material_specular_color(.0, .0, .0, .0);
		float  material_shininess = 0.0;

		glUniform4fv( glGetUniformLocation(program, "MaterialColor"), 1, material_color);
		glUniform4fv( glGetUniformLocation(program, "MaterialSpecularColor"), 1, material_specular_color);
		glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess);

		glDrawElements( GL_TRIANGLES, meshIndices.size(), GL_UNSIGNED_INT, &meshIndices[0]);
	}
}

void display( void ) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4  projection = Perspective( 45.0, aspect, 0.5, 30.0 );


    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );

    //  Generate tha model-view matrixn
	const vec3 viewer_pos( 0.0, 0.0, 2.0 );

	vector<Cube> cubesToDraw = Game();
	for (int i=0; i < cubesToDraw.size(); i++)
	{
		Cube c = cubesToDraw[i];
		vec4 eye = vec4(eyePoint);
		vec4 at( 0., 0., 0.,1);
		vec4 up( 0., 1., 0.,0);
		mat4 modelView = LookAt(eye, at, up);
		modelView *= Angel::Translate(c.position);
		modelView *= Angel::Scale(0.9, 0.9, 0.9);
		glUniformMatrix4fv( ModelView, 1, GL_TRUE, modelView );

		vec4 material_specular_color( .5, 0.2, .5, 1.0 );
		float  material_shininess = .5;

		glUniform4fv( glGetUniformLocation(program, "MaterialColor"), 1, c.color );
		glUniform4fv( glGetUniformLocation(program, "MaterialSpecularColor"), 1, material_specular_color );
		glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );

		glDrawElements( GL_TRIANGLES, meshIndices.size(), GL_UNSIGNED_INT, &meshIndices[0]);
	}

	drawLights();

	glutSwapBuffers();
	Angel::CheckError();
}

vec3 toCartesian(vec3 spherical)
{
	float radius = spherical.x;
	float theta = spherical.y;
	float phi = spherical.z;
	float X = cos(theta) * cos(phi) * radius;
	float Y = sin(theta) * cos(phi) * radius;
	float Z = sin(phi) * radius;
	return vec3(X, Y, Z);
}

vec3 toSpherical(vec3 cartesian)
{
	float X = cartesian.x;
	float Y = cartesian.y;
	float Z = cartesian.z;
	float radius = sqrt(X * X + Y * Y + Z * Z);
	float theta = atan2(Y, X);
	float phi = acos(Z / radius);
	return vec3(radius, theta, phi);
}

void mouse( int button, int state, int x, int y ) 
{
	vec3 sperical = toSpherical(eyePoint);
    if(state == GLUT_DOWN)
	{
		switch(button) 
		{
			case GLUT_LEFT_BUTTON:  
				mousePos.x = x;
				mousePos.y = y;
				break;
			case GLUT_RIGHT_BUTTON:
				break;
		}
    }
}

void mouseMotion(int x, int y)
{
	vec3 sperical = toSpherical(eyePoint);
	float dx = x - mousePos.x;
	float dy = y - mousePos.y;
	sperical[1] = dy * 0.008f;
	sperical[2] = dx * 0.008f;
	eyePoint = toCartesian(sperical);
	glutPostRedisplay();
}

void MouseWheel(int wheel, int direction, int x, int y)
{
    wheel=0;
    if (direction==-1)
    {
        eyePoint *= 1.1;
 
    }
    else if (direction==+1)
    {
        eyePoint /= 1.1;
    }
	glutPostRedisplay();
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
		case 'R': 
		case 'r':
			cout << "Reload shaders" << endl;
			reloadShader();
			break;
		case 'f':
			SnakeGame::getInstance().eatFood();
			glutPostRedisplay();
			break;
	}
}

void specialInput(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			SnakeGame::getInstance().move(direction::up);
			break;	
		case GLUT_KEY_DOWN:
			SnakeGame::getInstance().move(direction::down);
			break;
		case GLUT_KEY_LEFT:
			SnakeGame::getInstance().move(direction::left);
			break;
		case GLUT_KEY_RIGHT:
			SnakeGame::getInstance().move(direction::right);
			break;
	}
}

void reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
	aspect = float(width)/height;
}

void gameRound(int n)
{
	SnakeGame::getInstance().round();
	glutTimerFunc(n, gameRound, n);
}

int main(int argc, char* argv[]) {
	cout << "Left mouse to change rotation" << endl;
	cout << "Right mouse to change model" << endl;
	cout << "Press 'r' to reload shaders" << endl;
    glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    
	glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_3_2_CORE_PROFILE);
	glutCreateWindow("02561-04-04");
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
	glutSpecialFunc(specialInput);

	// RUN GAME
	SnakeGame::getInstance().initBoard(5);
	gameRound(1000);

	Angel::CheckError();
    glutMainLoop();
}