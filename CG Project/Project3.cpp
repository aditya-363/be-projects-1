#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "irrKlang.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define aisgl_max(x,y) ((y>x)?y:x)
#define aisgl_min(x,y) ((x<y)?x:y)
using namespace irrklang;

const int ALEXA_LISTEN = 1, ALEXA_THINK = 2, ALEXA_SLEEP = 0;

int prevx = 0, prevy = 0;
long int frameNumber = 0;
float anglex = 0.0, angley = 0.0, angle = 0.0, fov = 20.0, valx = 0.0;
float cameraParams[] = { -0.5, 0.5, 2.0, 0.25, 0.15, 0.0, 0.0, 1.0, 0.0 };
GLfloat light_position0[] = { 0.5, 2.0, 2.0, 0.0 };
GLfloat mat_emission1[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat alexaGray[] = { 0.3, 0.3, 0.3 },
alexaLightGray[] = { 0.8, 0.8, 0.8 },
sofaBeige[] = { 1.0, 0.8509, 0.7019 },
alexaBlue[] = { 0.0,1.0,1.0 },
alexaDarkBlue[] = { 0.0,0.0,0.5 },
alexaBlack[] = { 0.0,0.0,0.0 },
tableRosewood[] = { 0.50, 0.314, 0.21 },
zblue[] = { 0.0,0.0,1.0 },
ygreen[] = { 0.0,1.0,0.0 },
xred[] = { 1.0,0.0,0.0 },
floorBeige[] = { 0.9, 0.742, 0.656 },
floorColor[] = { 0.8, 0.8, 0.8 },
sidewallColor[] = { 0.941, 0.980, 0.517 },
backwallColor[] = { 0.517, 0.858, 0.223 },
frontwallColor[] = { 0.5, 0.3, 1.0 },
tvColorWhite[] = { 1.0, 1.0, 1.0, 1.0 },
tvColorGray[] = { 0.5, 0.5, 0.5, 0.0 },
ketchupBottle[] = { 1.0, 0.01, 0.01 };
GLfloat temp = 0.0;
GLuint image1,image2,image3,imageA,imageA1;
ILuint texid;
ILboolean success;
char m1[] = "Weather, News, Facts, Jokes,",
m2[] = "Reminders, Home automation,",
m3[] = " Music, Media, Calls & More",
m4[] = "Powerful Sound That",
m40[] = "Can Fill Up Any Room",
m5[] = "Integration with Restaurants,",
m50[] = "Cabs, Amazon and More",
m6[] = "Just Ask",
m7[] = "Ask Alexa to Do It",
m80[] = "PIZZA HUT",
m8[] = "Far Field Microphones",
m9[] = "Listening for your Commands",
m10[] = "NETFLIX";

const aiScene* scene1 = NULL;
const aiScene* scene2 = NULL;
const aiScene* scene3 = NULL;
const aiScene* scene4 = NULL;
const aiScene* scene5 = NULL;
const aiScene* scene6 = NULL;
const aiScene* scene7 = NULL;
const aiScene* scene8 = NULL;
const aiScene* scene9 = NULL;
GLuint scene1_list = 0, scene2_list = 0, scene3_list = 0, scene4_list = 0, scene5_list = 0, scene6_list = 0, scene7_list = 0, scene8_list = 0, scene9_list = 0;
aiVector3D scene1_min, scene1_max, scene1_center;
aiVector3D scene2_min, scene2_max, scene2_center;
aiVector3D scene3_min, scene3_max, scene3_center;
aiVector3D scene4_min, scene4_max, scene4_center;
aiVector3D scene5_min, scene5_max, scene5_center;
aiVector3D scene6_min, scene6_max, scene6_center;
aiVector3D scene7_min, scene7_max, scene7_center;
aiVector3D scene8_min, scene8_max, scene8_center;
aiVector3D scene9_min, scene9_max, scene9_center;
aiVector3D scene_min, scene_max, scene_center;

GLuint loadImage(const char* theFileName)
{
	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	success = ilLoadImage(theFileName);
	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			//iluFlipImage();
		}
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		if (!success)
		{
			error = ilGetError();
			std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			exit(-1);
		}
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}
	else
	{
		error = ilGetError();
		std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		exit(-1);
	}
	ilDeleteImages(1, &imageID);
	std::cout << "Texture creation successful." << std::endl;
	return textureID;
}
void init() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);

	glEnable(GL_LIGHTING);
	GLfloat mat_specular0[] = { 1.0,1.0,1.0, 1.0 };
	GLfloat mat_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient0[] = { 0.1,0.1,0.1,0.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_MULTISAMPLE);
	if (getenv("MODEL_IS_BROKEN"))
		glFrontFace(GL_CW);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	image1 = loadImage("texi.jpg");
	image2 = loadImage("texf1.jpg");
	image3 = loadImage("texe.jpg");
	imageA = loadImage("alexa.jpg");
	imageA1 = loadImage("alexa2.jpg");
	glBindTexture(GL_TEXTURE_2D, image1);
}
void alexaLight(int state) {
	if (state == ALEXA_THINK) {
		GLfloat mat_emission[] = { 0.0, 0.5, 0.5, 1.0 };
		GLfloat mat_specular0[] = { 0.0,0.0,0.0, 1.0 };
		GLfloat mat_ambient[] = { 1.0,1.0,1.0,1.0 };
		GLfloat mat_shininess0[] = { 100.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular0);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		if (frameNumber % 120 <= 50) {
			glColor3f(alexaBlue[0], alexaBlue[1] - (0.02*(frameNumber % 120)), alexaBlue[2] - (0.01*(frameNumber % 120)));
		}
		else if (frameNumber % 120 <= 70) {
			glColor3fv(alexaDarkBlue);
		}
		else if (frameNumber % 120 <= 110) {
			glColor3f(alexaDarkBlue[0], alexaDarkBlue[1] + (0.02*((frameNumber % 120) - 60)), alexaDarkBlue[2] + (0.02*((frameNumber % 120) - 60)));
		}
		else {
			glColor3fv(alexaBlue);
		}
	}
	else if (state == ALEXA_LISTEN) {
		GLfloat mat_emission[] = { 0.0, 0.5, 0.5, 1.0 };
		GLfloat mat_specular0[] = { 0.0,0.0,0.0, 1.0 };
		GLfloat mat_ambient[] = { 1.0,1.0,1.0,1.0 };
		GLfloat mat_shininess0[] = { 100.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular0);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		glColor3fv(alexaBlue);
	}
	else if (state == ALEXA_SLEEP) {
		GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat mat_specular0[] = { 0.0,0.0,0.0, 1.0 };
		GLfloat mat_ambient[] = { 0.0,0.0,0.0,1.0 };
		GLfloat mat_shininess0[] = { 100.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular0);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		glColor3fv(alexaGray);
	}
	else { return; }
}
void reshape(int width, int height)
{
	const double aspectRatio = (float)width / height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspectRatio, 0.1, 1000.0);  
	glViewport(0, 0, width, height);
}
void get_bounding_box_for_node(const  aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo, const aiScene* sc) {
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const  aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo, sc);
	}
	*trafo = prev;
}
void get_bounding_box(aiVector3D* min, aiVector3D* max, const aiScene* sc)
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(sc->mRootNode, min, max, &trafo, sc);
}
void color4_to_float4(const  aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}
void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
void text_disp(float x, float y, float z, char *string, void *font)
{
	int len, i;
	glRasterPos3f(x, y, z);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}
void apply_material(const  aiMaterial *mtl)
{
	float c[4];
	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	ai_real shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.1f, 0.1f, 0.1f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}
void recursive_render(const  aiScene *sc, const  aiNode* nd, const aiScene* sce)
{
	unsigned int i, n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	for (; n < nd->mNumMeshes; ++n) {
		const  aiMesh* mesh = sce->mMeshes[nd->mMeshes[n]];
		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
		if (mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		}
		else {
			glEnable(GL_LIGHTING);
		}
		for (t = 0; t < mesh->mNumFaces; ++t) {
			const  aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;
			switch (face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}
			glBegin(face_mode);
			for (i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if (mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}
			glEnd();
		}
	}
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n], sce);
	}
	glPopMatrix();
}
void processScenes() {
	if (scene1) {
		get_bounding_box(&scene1_min, &scene1_max, scene1);
		scene1_center.x = (scene1_min.x + scene1_max.x) / 2.0f;
		scene1_center.y = (scene1_min.y + scene1_max.y) / 2.0f;
		scene1_center.z = (scene1_min.z + scene1_max.z) / 2.0f;
		if (scene1_list == 0) {
			scene1_list = glGenLists(1);
			glNewList(scene1_list, GL_COMPILE);
			recursive_render(scene1, scene1->mRootNode, scene1);
			glEndList();
		}
		scene_center = scene1_center;
		scene_max = scene1_max;
		scene_min = scene1_min;
	}
	else {
		std::cout << "Import Scene 1 Failed!";
		getchar();
		exit(-1);
	}
	if (scene2) {
		get_bounding_box(&scene2_min, &scene2_max, scene2);
		scene2_center.x = (scene2_min.x + scene2_max.x) / 2.0f;
		scene2_center.y = (scene2_min.y + scene2_max.y) / 2.0f;
		scene2_center.z = (scene2_min.z + scene2_max.z) / 2.0f;
		if (scene2_list == 0) {
			scene2_list = glGenLists(1);
			glNewList(scene2_list, GL_COMPILE);
			recursive_render(scene2, scene2->mRootNode, scene2);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 2 Failed!";
		getchar();
		exit(-1);
	}
	if (scene3) {
		get_bounding_box(&scene3_min, &scene3_max, scene3);
		scene3_center.x = (scene3_min.x + scene3_max.x) / 2.0f;
		scene3_center.y = (scene3_min.y + scene3_max.y) / 2.0f;
		scene3_center.z = (scene3_min.z + scene3_max.z) / 2.0f;
		if (scene3_list == 0) {
			scene3_list = glGenLists(1);
			glNewList(scene3_list, GL_COMPILE);
			recursive_render(scene3, scene3->mRootNode, scene3);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 3 Failed!";
		getchar();
		exit(-1);
	}
	if (scene4) {
		get_bounding_box(&scene4_min, &scene4_max, scene4);
		scene4_center.x = (scene4_min.x + scene4_max.x) / 2.0f;
		scene4_center.y = (scene4_min.y + scene4_max.y) / 2.0f;
		scene4_center.z = (scene4_min.z + scene4_max.z) / 2.0f;
		if (scene4_list == 0) {
			scene4_list = glGenLists(1);
			glNewList(scene4_list, GL_COMPILE);
			recursive_render(scene4, scene4->mRootNode, scene4);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 4 Failed!";
		getchar();
		exit(-1);
	}
	if (scene5) {
		get_bounding_box(&scene5_min, &scene5_max, scene5);
		scene5_center.x = (scene5_min.x + scene5_max.x) / 2.0f;
		scene5_center.y = (scene5_min.y + scene5_max.y) / 2.0f;
		scene5_center.z = (scene5_min.z + scene5_max.z) / 2.0f;
		if (scene5_list == 0) {
			scene5_list = glGenLists(1);
			glNewList(scene5_list, GL_COMPILE);
			recursive_render(scene5, scene5->mRootNode, scene5);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 5 Failed!";
		getchar();
		exit(-1);
	}
	if (scene6) {
		get_bounding_box(&scene6_min, &scene6_max, scene6);
		scene6_center.x = (scene6_min.x + scene6_max.x) / 2.0f;
		scene6_center.y = (scene6_min.y + scene6_max.y) / 2.0f;
		scene6_center.z = (scene6_min.z + scene6_max.z) / 2.0f;
		if (scene6_list == 0) {
			scene6_list = glGenLists(1);
			glNewList(scene6_list, GL_COMPILE);
			recursive_render(scene6, scene6->mRootNode, scene6);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 6 Failed!";
		getchar();
		exit(-1);
	}
	if (scene7) {
		get_bounding_box(&scene7_min, &scene7_max, scene7);
		scene7_center.x = (scene7_min.x + scene7_max.x) / 2.0f;
		scene7_center.y = (scene7_min.y + scene7_max.y) / 2.0f;
		scene7_center.z = (scene7_min.z + scene7_max.z) / 2.0f;
		if (scene7_list == 0) {
			scene7_list = glGenLists(1);
			glNewList(scene7_list, GL_COMPILE);
			recursive_render(scene7, scene7->mRootNode, scene7);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 7 Failed!";
		getchar();
		exit(-1);
	}
	if (scene8) {
		get_bounding_box(&scene8_min, &scene8_max, scene8);
		scene8_center.x = (scene8_min.x + scene8_max.x) / 2.0f;
		scene8_center.y = (scene8_min.y + scene8_max.y) / 2.0f;
		scene8_center.z = (scene8_min.z + scene8_max.z) / 2.0f;
		if (scene8_list == 0) {
			scene8_list = glGenLists(1);
			glNewList(scene8_list, GL_COMPILE);
			recursive_render(scene8, scene8->mRootNode, scene8);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 8 Failed!";
		getchar();
		exit(-1);
	}
	if (scene9) {
		get_bounding_box(&scene9_min, &scene9_max, scene9);
		scene9_center.x = (scene9_min.x + scene9_max.x) / 2.0f;
		scene9_center.y = (scene9_min.y + scene9_max.y) / 2.0f;
		scene9_center.z = (scene9_min.z + scene9_max.z) / 2.0f;
		if (scene9_list == 0) {
			scene9_list = glGenLists(1);
			glNewList(scene9_list, GL_COMPILE);
			recursive_render(scene9, scene9->mRootNode, scene9);
			glEndList();
		}
	}
	else {
		std::cout << "Import Scene 9 Failed!";
		getchar();
		exit(-1);
	}
}
void timer(int extra) {
	if (frameNumber == 610)	glEnable(GL_LIGHT0);
	if (frameNumber > 970 && frameNumber < 1060) {
		cameraParams[2] -= 0.01;
		cameraParams[0] += 0.008;
		cameraParams[1] -= 0.001;
	}
	if (frameNumber == 5045) {
		scene_center = scene5_center;
		scene_max = scene5_max;
		scene_min = scene5_min;
		cameraParams[0] = -0.8;
		cameraParams[1] = 0.1;
		cameraParams[2] = 0.2;
		cameraParams[3] = 0.0;
		cameraParams[4] = 0.0;
		cameraParams[5] = 0.0;
	}
	if (frameNumber == 7140) {
		scene_center = scene2_center;
		scene_max = scene2_max;
		scene_min = scene2_min;
		cameraParams[0] = 0.0;
		cameraParams[1] = -2.8;
		cameraParams[2] = 8.0;
		cameraParams[3] = -1.8;
		cameraParams[4] = -3.4;
		cameraParams[5] = 1.4;
		temp = 0.0;
	}
	if (frameNumber == 9844) {
		scene_center = scene7_center;
		scene_max = scene7_max;
		scene_min = scene7_min;
		cameraParams[0] = -0.5;
		cameraParams[1] = 0.0;
		cameraParams[2] = 25.0;
		cameraParams[3] = 0.0;
		cameraParams[4] = 0.5;
		cameraParams[5] = 0.0;
	}
	if (frameNumber == 10590) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, imageA);
		cameraParams[0] = 0.0;
		cameraParams[1] = 0.0;
		cameraParams[2] = 0.0;
		cameraParams[3] = 0.0;
		cameraParams[4] = 0.0;
		cameraParams[5] = -1.0;
		angley = 180.0;
	}
	if (frameNumber == 11100) {
		glBindTexture(GL_TEXTURE_2D, imageA1);
	}
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); //60fps
}
void display(void)
{
	float tmp;
	if (frameNumber >= 5045 && frameNumber < 7140)	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	if (frameNumber == 7140)	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraParams[0], cameraParams[1], cameraParams[2], cameraParams[3], cameraParams[4], cameraParams[5], cameraParams[6], cameraParams[7], cameraParams[8]);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	if (frameNumber >= 0 && frameNumber < 5045) {
		tmp = scene_max.x - scene_min.x;
		tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
		tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
		tmp = 1.f / tmp;
		if (frameNumber >= 0 && frameNumber < 610)	glDisable(GL_LIGHT0);
		glPushMatrix();
		glScalef(tmp, tmp, tmp);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
		glCallList(scene1_list);
		glPopMatrix();
		glEnable(GL_COLOR_MATERIAL);
		if (frameNumber >= 610)	glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(-1.4, -0.1, -0.6);
		glColor3fv(floorColor);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, image1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2f(0.0, 0.0);
		glTexCoord2i(0, 1);
		glVertex2f(0.0, 2.0);
		glTexCoord2i(1, 1);
		glVertex2f(2.0, 2.0);
		glTexCoord2i(1, 0);
		glVertex2f(2.0, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPushMatrix();
		glColor3fv(backwallColor);
		glTranslatef(-0.4, 0.0, -0.5);
		glScalef(2.0, 2.0, 0.00001);
		glutSolidCube(1.0);
		glPopMatrix();
		glPushMatrix();
		glColor3fv(sidewallColor);
		glTranslatef(0.55, -0.66, 1.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, image2);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2f(0.0, 0.0);
		glTexCoord2i(0, 1);
		glVertex2f(0.0, 2.0);
		glTexCoord2i(1, 1);
		glVertex2f(2.0, 2.0);
		glTexCoord2i(1, 0);
		glVertex2f(2.0, 0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		if (frameNumber >= 610)	glEnable(GL_LIGHTING);
		glPushMatrix();
		GLUquadric *g, *h, *f;
		g = gluNewQuadric();
		h = gluNewQuadric();
		f = gluNewQuadric();
		glScalef(0.025, 0.025, 0.025);
		glTranslatef(16.5, 0.55, -6.0);
		glColor3fv(alexaGray);
		glRotatef(-90, 1.0, 0.0, 0.0);
		gluCylinder(g, 0.6, 0.6, 2.5, 16, 16);
		glPopMatrix();
		glPushMatrix();
		glScalef(0.025, 0.025, 0.025);
		if (frameNumber >= 0 && frameNumber < 325)	alexaLight(ALEXA_SLEEP);
		else if (frameNumber >= 325 && frameNumber < 528)	alexaLight(ALEXA_LISTEN);
		else if (frameNumber >= 528 && frameNumber < 715)	alexaLight(ALEXA_THINK);
		else if (frameNumber >= 715 && frameNumber < 974)	alexaLight(ALEXA_SLEEP);
		else if (frameNumber >= 974 && frameNumber < 1140)	alexaLight(ALEXA_LISTEN);
		else if (frameNumber >= 1140 && frameNumber < 2620)	alexaLight(ALEXA_THINK);
		else if (frameNumber >= 2620 && frameNumber < 3005)	alexaLight(ALEXA_SLEEP);
		else if (frameNumber >= 3005 && frameNumber < 3104)	alexaLight(ALEXA_LISTEN);
		else if (frameNumber >= 3104 && frameNumber < 3725)	alexaLight(ALEXA_SLEEP);
		else if (frameNumber >= 3725 && frameNumber < 3910)	alexaLight(ALEXA_LISTEN);
		else if (frameNumber >= 3910 && frameNumber < 4198)	alexaLight(ALEXA_THINK);
		else if (frameNumber >= 4198 && frameNumber < 4385)	alexaLight(ALEXA_LISTEN);
		else if (frameNumber >= 4385 && frameNumber < 4940)	alexaLight(ALEXA_THINK);
		else if (frameNumber >= 4940 && frameNumber < 5045)	alexaLight(ALEXA_SLEEP);
		glTranslatef(16.5, 3.05, -6.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		gluCylinder(h, 0.6, 0.55, 0.15, 16, 16);	
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);
		glPopMatrix();
		glPushMatrix();
		glScalef(0.025, 0.025, 0.025);
		glColor3fv(alexaGray);
		glTranslatef(16.5, 3.15, -6.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		gluDisk(f, 0.0, 0.55, 16, 16);
		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);
		if (frameNumber >= 1272 && frameNumber < 2660) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
			text_disp(0.27, 0.21, -0.2, m1, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.27, 0.18, -0.2, m2, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.27, 0.15, -0.2, m3, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.30, 0.12, -0.2, m7, GLUT_BITMAP_TIMES_ROMAN_24);
			glEnable(GL_LIGHTING);
		}
		if (frameNumber >= 3020 && frameNumber < 3300) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
			text_disp(0.31, 0.15, -0.2, m4, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.31, 0.12, -0.2, m40, GLUT_BITMAP_TIMES_ROMAN_24);
			glEnable(GL_LIGHTING);
		}
		if (frameNumber >= 3725 && frameNumber < 3990) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
			text_disp(0.285, 0.15, -0.2, m8, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.27, 0.12, -0.2, m9, GLUT_BITMAP_TIMES_ROMAN_24);
			glEnable(GL_LIGHTING);
		}
		if (frameNumber >= 4182 && frameNumber < 4700) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
			text_disp(0.29, 0.18, -0.2, m5, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.30, 0.15, -0.2, m50, GLUT_BITMAP_TIMES_ROMAN_24);
			text_disp(0.36, 0.12, -0.2, m6, GLUT_BITMAP_TIMES_ROMAN_24);
			glEnable(GL_LIGHTING);
		}
	}
	else if (frameNumber >= 5045 && frameNumber < 7140) {
		//Car Scene
		if (frameNumber < 6800) {
			tmp = scene_max.x - scene_min.x;
			tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
			tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
			tmp = 5.f / tmp;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
			glTranslatef(5.0, 0.7, 0.83);
			glCallList(scene5_list);
			glPopMatrix();
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
			glTranslatef(15.0, 0.7, 0.83);
			glCallList(scene5_list);
			glPopMatrix();
			glPushMatrix();
			glScalef(tmp / 10, tmp / 10, tmp / 10);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glRotatef(-90.0, 0.0, 0.0, 1.0);
			glTranslatef(-scene4_center.x, -scene4_center.y, -scene4_center.z);
			glTranslatef(0.0, temp, 0.0);
			glCallList(scene4_list);
			glPopMatrix();
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
			text_disp(0.01, 0.12, 0.0, m80, GLUT_BITMAP_TIMES_ROMAN_24);
			glEnable(GL_LIGHTING);
			if (frameNumber >= 5060 && frameNumber < 5300) {
				cameraParams[0] += 0.00332;
				cameraParams[2] -= 0.00250;
			}
			if (frameNumber > 5300 && frameNumber < 5400) { cameraParams[2] -= 0.004; }
			if (frameNumber > 5558 && frameNumber < 6186) {
				temp += 0.2;
				cameraParams[3] += 0.01;
				if (cameraParams[2] < 0.0)	cameraParams[2] += 0.001;
			}
			if (frameNumber >= 6186 && frameNumber < 6800) {
				temp += 0.15;
				cameraParams[3] += 0.01;
			}
		}
		if (frameNumber == 6800)	temp = -5.0;
		if (frameNumber >= 6800 && frameNumber < 7140) {
			tmp = scene_max.x - scene_min.x;
			tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
			tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
			tmp = 5.f / tmp;
			cameraParams[0] = 0.3;
			cameraParams[1] = 0.0;
			cameraParams[2] = -0.4;
			cameraParams[3] = 0.0;
			cameraParams[4] = 0.05;
			cameraParams[5] = 0.0;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
			glTranslatef(1.0, 0.7, 0.83);
			glCallList(scene5_list);
			glPopMatrix();
			glPushMatrix();
			glScalef(tmp / 10, tmp / 10, tmp / 10);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glRotatef(-90.0, 0.0, 0.0, 1.0);
			glTranslatef(-scene4_center.x, -scene4_center.y, -scene4_center.z);
			glTranslatef(0.0, temp, 0.0);
			glCallList(scene4_list);
			glPopMatrix();
			if (temp < 0)	temp += 0.0227;
		}
	}
	else if (frameNumber >= 7140 && frameNumber < 9844) {
		//Kitchen Scene
		if (frameNumber >= 7140 && frameNumber < 7410) {
			tmp = scene_max.x - scene_min.x;
			tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
			tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
			tmp = 10.0f / tmp;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
			glCallList(scene2_list);
			glPopMatrix();
			tmp = scene6_max.x - scene6_min.x;
			tmp = aisgl_max(scene6_max.y - scene6_min.y, tmp);
			tmp = aisgl_max(scene6_max.z - scene6_min.z, tmp);
			tmp = 3.5f / tmp;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			if (frameNumber < 7400)	glRotatef(180.0, 0.0, 1.0, 0.0);
			else glRotatef(90.0, 0.0, 1.0, 0.0);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene6_center.x, -scene6_center.y, -scene6_center.z);
			glTranslatef(2.0, -6.5, -6.0);
			glCallList(scene6_list);
			glPopMatrix();
			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();
			GLUquadric *g, *h, *f;
			g = gluNewQuadric();
			h = gluNewQuadric();
			f = gluNewQuadric();
			glTranslatef(-1.8, -3.74, 1.4);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(alexaLightGray);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(g, 0.45, 0.45, 0.3, 16, 16);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.8, -3.678, 1.4);
			glScalef(0.25, 0.25, 0.25);
			alexaLight(ALEXA_SLEEP);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(h, 0.45, 0.4, 0.1, 16, 16);	
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.8, -3.66, 1.4);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(alexaLightGray);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluDisk(f, 0.0, 0.4, 16, 16);
			glPopMatrix();
			glDisable(GL_COLOR_MATERIAL);
		}
		if (frameNumber > 7310 && frameNumber < 7450 && cameraParams[2]>5.0)	cameraParams[2] -= 0.1;
		if (frameNumber >= 7410 && frameNumber < 7620) {
			tmp = scene_max.x - scene_min.x;
			tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
			tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
			tmp = 10.0f / tmp;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
			glCallList(scene2_list);
			glPopMatrix();
			tmp = scene8_max.x - scene8_min.x;
			tmp = aisgl_max(scene8_max.y - scene8_min.y, tmp);
			tmp = aisgl_max(scene8_max.z - scene8_min.z, tmp);
			tmp = 1.0f / tmp;
			glPushMatrix();
			glTranslatef(-scene8_center.x, -scene8_center.y, -scene8_center.z);
			glTranslatef(-1.65, -3.35, 3.3);
			glRotatef(-180.0, 1.0, 0.0, 0.0);
			glRotatef(100.0, 0.0, 1.0, 0.0);
			glRotatef(20.0, 0.0, 0.0, 1.0);
			glScalef(tmp, tmp, tmp);
			glCallList(scene8_list);
			glPopMatrix();
			tmp = scene8_max.x - scene8_min.x;
			tmp = aisgl_max(scene8_max.y - scene8_min.y, tmp);
			tmp = aisgl_max(scene8_max.z - scene8_min.z, tmp);
			tmp = 1.0f / tmp;
			glPushMatrix();
			glTranslatef(-scene8_center.x, -scene8_center.y, -scene8_center.z);
			glTranslatef(-1.665, -3.35, 3.8);
			glRotatef(-180.0, 1.0, 0.0, 0.0);
			glRotatef(100.0, 0.0, 1.0, 0.0);
			glRotatef(20.0, 0.0, 0.0, 1.0);
			if (frameNumber % 50 < 10)	glRotatef(-2.0, 0.0, 1.0, 0.0);
			glScalef(tmp, tmp, tmp);
			glCallList(scene8_list);
			glPopMatrix();
			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();
			glTranslatef(-1.2, -3.2, 0.8);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(ketchupBottle);
			GLUquadric *ket, *ketbott;
			ket = gluNewQuadric();
			ketbott = gluNewQuadric();
			gluCylinder(ket, 0.25, 0.25, 1.0, 8, 8);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.2, -2.95, 0.8);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(ketchupBottle);
			gluDisk(ketbott, 0.0, 0.25, 16, 16);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.2, -3.2, 0.8);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(ketchupBottle);
			glutSolidCone(0.1, 0.2, 8, 8);
			glPopMatrix();
			glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();
			GLUquadric *g, *h, *f;
			g = gluNewQuadric();
			h = gluNewQuadric();
			f = gluNewQuadric();
			glTranslatef(-1.8, -3.74, 1.4);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(alexaLightGray);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(g, 0.45, 0.45, 0.3, 16, 16);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.8, -3.678, 1.4);
			glScalef(0.25, 0.25, 0.25);
			alexaLight(ALEXA_SLEEP);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(h, 0.45, 0.4, 0.1, 16, 16);	
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.8, -3.66, 1.4);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(alexaLightGray);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluDisk(f, 0.0, 0.4, 16, 16);
			glPopMatrix();
			glDisable(GL_COLOR_MATERIAL);
		}
		if (frameNumber >= 7620 && frameNumber < 9844) {
			tmp = scene_max.x - scene_min.x;
			tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
			tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
			tmp = 10.0f / tmp;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
			glCallList(scene2_list);
			glPopMatrix();
			tmp = scene9_max.x - scene9_min.x;
			tmp = aisgl_max(scene9_max.y - scene9_min.y, tmp);
			tmp = aisgl_max(scene9_max.z - scene9_min.z, tmp);
			tmp = 3.5f / tmp;
			glPushMatrix();
			glScalef(tmp, tmp, tmp);
			glRotatef(-45.0, 0.0, 1.0, 0.0);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glTranslatef(-scene9_center.x, -scene9_center.y, -scene9_center.z);
			glTranslatef(-2.0, -0.5, -6.0);
			glCallList(scene9_list);
			glPopMatrix();
			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();
			glTranslatef(-0.8, -3.7, 1.5);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(ketchupBottle);
			GLUquadric *ket, *ketbott;
			ket = gluNewQuadric();
			ketbott = gluNewQuadric();
			gluCylinder(ket, 0.25, 0.25, 1.0, 8, 8);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-0.8, -3.45, 1.5);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(ketchupBottle);
			gluDisk(ketbott, 0.0, 0.25, 16, 16);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-0.8, -3.44, 1.5);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(ketchupBottle);
			glutSolidCone(0.1, 0.2, 8, 8);
			glPopMatrix();
			glDisable(GL_COLOR_MATERIAL);
			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();
			GLUquadric *g, *h, *f;
			g = gluNewQuadric();
			h = gluNewQuadric();
			f = gluNewQuadric();
			glTranslatef(-1.8, -3.74, 1.4);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(alexaLightGray);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(g, 0.45, 0.45, 0.3, 16, 16);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.8, -3.678, 1.4);
			glScalef(0.25, 0.25, 0.25);
			if (frameNumber >= 7785 && frameNumber < 7998)	alexaLight(ALEXA_LISTEN);
			else if (frameNumber >= 7998 && frameNumber < 8675)	alexaLight(ALEXA_THINK);
			else if (frameNumber >= 8675 && frameNumber < 8890)	alexaLight(ALEXA_LISTEN);
			else if (frameNumber >= 8890 && frameNumber < 9250)	alexaLight(ALEXA_THINK);
			else if (frameNumber >= 9360 && frameNumber < 9600)	alexaLight(ALEXA_LISTEN);
			else alexaLight(ALEXA_SLEEP);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(h, 0.45, 0.4, 0.1, 16, 16);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-1.8, -3.66, 1.4);
			glScalef(0.25, 0.25, 0.25);
			glColor3fv(alexaLightGray);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluDisk(f, 0.0, 0.4, 16, 16);
			glPopMatrix();
			glDisable(GL_COLOR_MATERIAL);
		}
	}
	else if (frameNumber >= 9844 && frameNumber < 10590) {
		//TV Scene
		tmp = scene_max.x - scene_min.x;
		tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
		tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
		tmp = 10.0f / tmp;
		glPushMatrix();
		glScalef(tmp, tmp, tmp);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glRotatef(-90.0, 0.0, 0.0, 1.0);
		glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
		glCallList(scene7_list);
		glPopMatrix();
		glPushMatrix();
		tmp = scene1_max.x - scene1_min.x;
		tmp = aisgl_max(scene1_max.y - scene1_min.y, tmp);
		tmp = aisgl_max(scene1_max.z - scene1_min.z, tmp);
		tmp = 10.0f / tmp;
		glPushMatrix();
		glTranslatef(0.0, -3.0, 10.0);
		glScalef(tmp, tmp, tmp);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glRotatef(180.0, 0.0, 0.0, 1.0);
		glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
		glCallList(scene1_list);
		glPopMatrix();
		glEnable(GL_COLOR_MATERIAL);
		if (frameNumber >= 10300) {}
		else 		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(0.0, -4.25, 0.0);
		glColor3fv(floorColor);
		glScalef(30.0, 0.00001, 30.0);
		glutSolidCube(1.0);
		glPopMatrix();
		glPushMatrix();
		glColor3fv(frontwallColor);
		glTranslatef(0.0, 0.0, -1.25);
		glScalef(30.0, 30.0, 0.00001);
		glutSolidCube(1.0);
		glPopMatrix();
		glPushMatrix();
		glColor3fv(sidewallColor);
		glTranslatef(-8.0, 0.0, 0.0);
		glScalef(0.00001, 30.0, 30.0);
		glutSolidCube(1.0);
		glPopMatrix();
		if (frameNumber >= 10300) {}
		else 		glEnable(GL_LIGHTING);
		glPushMatrix();
		if (frameNumber >= 9920 && frameNumber < 10124)			alexaLight(ALEXA_LISTEN);
		else if (frameNumber >= 10124 && frameNumber < 10332)	alexaLight(ALEXA_THINK);
		else													alexaLight(ALEXA_SLEEP);
		glTranslatef(3.75, -1.315, 0.51);
		glScalef(0.75, 0.05, 0.05);
		glutSolidCube(1.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0, 2.3, -0.75);
		glScalef(6.3, 3.54375, 0.00001);
		GLfloat mat_emission[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat mat_specular0[] = { 0.0,0.0,0.0, 1.0 };
		GLfloat mat_ambient[] = { 1.0,1.0,1.0,1.0 };
		GLfloat mat_shininess0[] = { 100.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular0);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		if (frameNumber == 10300) {
			GLfloat mat_specular1[] = { 0.0,0.0,0.0, 1.0 };
			GLfloat mat_diffuse1[] = { 0.5, 0.5, 0.5, 1.0 };
			GLfloat mat_ambient1[] = { 0.1,0.1,0.1,0.0 };
			glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular1);
			glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient1);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse1);
		}
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		if (frameNumber > 10332 && frameNumber < 10590) {
			glColor4fv(tvColorWhite);
		}
		else		glColor4fv(tvColorGray);
		glutSolidCube(1.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
		if (frameNumber > 10332 && frameNumber < 10590) {
			glColor3f(1.0, 0.0, 0.0);
			glDisable(GL_LIGHTING);
			glPushMatrix();
			text_disp(-0.5, 2.0, 0.1, m10, GLUT_BITMAP_TIMES_ROMAN_24);
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}
		glDisable(GL_COLOR_MATERIAL);
	}
	else if (frameNumber >= 10590 && frameNumber <= 11100) {
		//Final Scene
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-640.0, 640.0, -360.0, 360.0, 0.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 0.0, 0.2);
		glRotatef(180.0, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2f(-640.0, -360.0);
		glTexCoord2i(0, 1); glVertex2f(-640.0, 360.0);
		glTexCoord2i(1, 1); glVertex2f(640.0, 360.0);
		glTexCoord2i(1, 0); glVertex2f(640.0, -360.0);
		glEnd();
		glPopMatrix();
	}
	else {
		std::cout << "Ad ended.\n";
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-640.0, 640.0, -360.0, 360.0, 0.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 0.0, 0.2);
		glRotatef(180.0, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2f(-640.0, -360.0);
		glTexCoord2i(0, 1); glVertex2f(-640.0, 360.0);
		glTexCoord2i(1, 1); glVertex2f(640.0, 360.0);
		glTexCoord2i(1, 0); glVertex2f(640.0, -360.0);
		glEnd();
		glPopMatrix();
	}
	glutSwapBuffers();
	frameNumber += 1;
}
int main(int argc, char **argv)
{
	aiLogStream stream;
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("Alexa - Amazon's Smart Assistant");
	glutFullScreen();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
	aiAttachLogStream(&stream);

	scene1 = aiImportFile("sofa.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene2 = aiImportFile("kitchen.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene3 = aiImportFile("pizza.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene6 = aiImportFile("personkitchen.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene7 = aiImportFile("room1.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene8 = aiImportFile("personhand.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene9 = aiImportFile("person.blend", aiProcessPreset_TargetRealtime_MaxQuality);
	scene4 = aiImportFile("car.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	scene5 = aiImportFile("roadbuildings.blend", aiProcessPreset_TargetRealtime_MaxQuality);

	processScenes();
	init();
	glutTimerFunc(0, timer, 0);
	
	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
		std::cout << "\nError\n";
	engine->play2D("alexa_background.ogg", false);
	glutMainLoop();
	
	aiReleaseImport(scene1);
	aiReleaseImport(scene2);
	aiReleaseImport(scene3);
	aiReleaseImport(scene4);
	aiReleaseImport(scene5);
	aiReleaseImport(scene6);
	aiReleaseImport(scene7);
	aiReleaseImport(scene8);
	aiReleaseImport(scene9);
	aiDetachAllLogStreams();
	return 0;
}
