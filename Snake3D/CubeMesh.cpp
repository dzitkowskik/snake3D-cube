#include "CubeMesh.h"
#include "ObjLoader.h"
#include "Camera.h"

void CubeMesh::loadModel()
{
	std::vector<vec3> outPositions;
	std::vector<vec3> outNormal;
	std::vector<vec2> outUv;

	loadObject(this->name.c_str(), outPositions, this->indices, outNormal, outUv, 0.5f);

	// Create a vertex array object
    glGenVertexArrays( 1, &vertexArrayObject );
    glBindVertexArray( vertexArrayObject );

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
    GLuint vPosition = glGetAttribLocation( this->shader.shaderProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( this->shader.shaderProgram, "vNormal" ); 
	if (vNormal != GL_INVALID_INDEX){
		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
				   BUFFER_OFFSET(sizeof(vec3) * outPositions.size()) );
	}
}

void CubeMesh::activate()
{
	glUseProgram(this->shader.shaderProgram);
}

void CubeMesh::deactivate()
{
	glUseProgram(0);
}

void CubeMesh::draw(Cube cube, Camera camera, float material_shininess)
{
	mat4  projection = Perspective( 45.0, camera.aspect, 0.1, 300.0 );
    glUniformMatrix4fv( this->shader.projectionUniform, 1, GL_TRUE, projection );

	vec4 eye = vec4(camera.eyePoint);
	vec4 at( 0., 0., 0., 1.);
	mat4 modelView = LookAt(eye, at, vec4(camera.up_eye, 1.));

	modelView *= Angel::Translate(cube.position);
	modelView *= Angel::Scale(cube.scale, cube.scale, cube.scale);
	glUniformMatrix4fv( this->shader.modelViewUniform, 1, GL_TRUE, modelView );

	vec4 material_specular_color( .5, 0.2, .5, 1.0 );

	glUniform4fv( glGetUniformLocation(this->shader.shaderProgram, "MaterialColor"), 1, cube.color );
	glUniform4fv( glGetUniformLocation(this->shader.shaderProgram, "MaterialSpecularColor"), 1, material_specular_color );
	glUniform1f( glGetUniformLocation(this->shader.shaderProgram, "Shininess"), material_shininess );

	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, &this->indices[0]);
}

void CubeMesh::drawCubeMap(Cube cube, Camera camera, GLuint cubemapTexture)
{
	mat4  projection = Perspective( 45.0, camera.aspect, 0.1, 300.0 );
    glUniformMatrix4fv( this->shader.projectionUniform, 1, GL_TRUE, projection );

	vec4 eye = vec4(camera.eyePoint);
	vec4 at( 0., 0., 0., 1.);
	mat4 modelView = LookAt(eye, at, vec4(camera.up_eye, 1.));

	modelView *= Angel::Translate(cube.position);
	modelView *= Angel::Scale(cube.scale, cube.scale, cube.scale);
	glUniformMatrix4fv( this->shader.modelViewUniform, 1, GL_TRUE, modelView );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(this->shader.cubemapUniform, 0);

	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, &this->indices[0]);
}