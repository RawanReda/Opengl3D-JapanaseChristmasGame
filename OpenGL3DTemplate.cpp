//  Includes
#include <stdlib.h>
#include <cmath>
#include <random>
#include <time.h>

#include <string.h>
#include <algorithm>
#include <iostream> 
using namespace std;
#include <stdio.h>
#include <string.h>
#include <glut.h>
#include <iomanip>
#include <dos.h>
#include <windows.h>
#include <playsoundapi.h>
#include <mmsystem.h>
#include <tchar.h>
//
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
bool  win = false;
float player_posx = 0;
float player_posy = 0;
float player_posz = 0;
float rotate_tree = 0;

int rotateLantern = 0;
int rotatePlayer_angle = 0;
int changeColor = 0;
void Timer(int value);
void collision();
//
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

float topx = 0.0f;
float topy = 1.7f;
float topz = 2.0f;
class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = topx, float eyeY = topy, float eyeZ = topz, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}
	void setView(float x, float y, float z) {
		eye.x = x;
		eye.y = y;
		eye.z = z;
	}
	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		//	cout << eye.x << endl;
		eye = eye + right * d;
		//	cout << "  " << eye.x << " " << eye.y << " " << eye.z << endl;
			//center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		//center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		//center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		//	cout << " " << eye.x << " " << eye.y << " " << eye.z << endl;
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};
Camera camera;

int count_presents = 0;

bool p1 = false;
bool p2 = false;
bool p3 = false;
bool p4 = false;
void collision() {
	if (count_presents == 4 && !win) {
		PlaySound(TEXT("sound/victory.wav"), NULL, SND_ASYNC | SND_FILENAME);

		win = true;
	}
	if (!p1 && player_posx + 0.35 <= -0.7 && player_posz + 0.45 >= 0.7) {
		PlaySound(TEXT("sound/score-point.wav"), NULL, SND_ASYNC | SND_FILENAME);

		cout << "enters" << endl;
		count_presents++;
		p1 = true;

	}

	if (!p2 && (player_posx + 0.35 >= 0.65 && player_posx + 0.35 <= 0.95) && (player_posz + 0.45 <= 0.95 && player_posz + 0.45 >= 0.65)) {
		PlaySound(TEXT("sound/score-point.wav"), NULL, SND_ASYNC | SND_FILENAME);

		cout << "enters p2" << endl;
		count_presents++;
		p2 = true;
	}
	if (!p3 &&(player_posx + 0.35 <= -0.65 && player_posx + 0.35 >= -1) && (player_posz + 0.45 <= 0.15 && player_posz + 0.45 >= -0.15)) {
		PlaySound(TEXT("sound/score-point.wav"), NULL, SND_ASYNC | SND_FILENAME);

		cout << "enters p3" << endl;
		count_presents++;
		p3 = true;
	}
	 if (!p4 && (player_posz + 0.35 <= -0.65 && player_posz + 0.35 >= -1) && (player_posx + 0.45 <= 0.15 && player_posx + 0.45 >= -0.15)) {
		PlaySound(TEXT("sound/score-point.wav"), NULL, SND_ASYNC | SND_FILENAME);

		cout << "enters p3" << endl;
		count_presents++;
		p4 = true;
	}

	glutPostRedisplay();
}
bool move_Lantern = false;
bool move_Presents = false;
bool move_fences = false;
bool move_tree = false;
bool move_snowman = false;
bool move_up = true;
float timeup = 0;
bool timeup_check = false;
float presents_up = 0;
float fences_up = 0;
float snowman_translate = 0;
bool move_right = true;

void Timer(int value) {
	timeup += 200;
	
	if (!timeup_check && !win && timeup >= 200 * 200) {
		PlaySound(TEXT("sound/defeat.wav"), NULL, SND_ASYNC | SND_FILENAME);
		timeup_check = true;
	}

	if (changeColor == 0) {
		changeColor = 1;
	}
	else {
		changeColor = 0;
	}
	if (move_Presents) {
		presents_up += 0.01;
	}
	if (move_Lantern) {
		rotateLantern += 20;
	}
	if (move_tree) {
		rotate_tree += 20;
	}
	if (move_snowman) {
		float d = 0.03;

		if (move_right) {
			snowman_translate += d;
			if (snowman_translate >= 0.5)
				move_right = false;
		}
		if (!move_right) {
			snowman_translate -= d;
			if (snowman_translate <= -0.5)
				move_right = true;
		}

	}
	if (move_fences) {
		float d = 0.03;

		if (move_up) {
			fences_up += d;
			if (fences_up >= 0.5)
				move_up = false;
		}
		if (!move_up) {
			fences_up -= d;
			if (fences_up <= 0)
				move_up = true;
		}

	}

	glutPostRedisplay();
	glutTimerFunc(200, Timer, 0);
}
void drawWall(double thickness) {
	glPushMatrix();
	glScaled(2.0, thickness, 2.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawFence(double thickness) {
	glColor3f(1, 0.0, 0.0);
	glPushMatrix();
	glTranslated(0.15, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.25, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.35, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.45, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.55, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.65, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.75, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.85, 0.5 * thickness, 0.31);
	glScaled(0.07, thickness, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


void drawPlayer() {

	glPushMatrix();
	glColor3f(0.9f, 0.8f, 0);
	glTranslated(player_posx + 0.3, player_posy + 0.1, player_posz + 0.47); // back leg
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.02f, 0.02f, 0.1f, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9f, 0.8f, 0);
	glTranslated(player_posx + 0.3, player_posy + 0.1, player_posz + 0.43); // front leg
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.02f, 0.02f, 0.1f, 10, 10);
	glPopMatrix();

	glPushMatrix();

	glColor3f(0.9f, 0.8f, 0);
	glTranslated(player_posx + 0.4, player_posy + 0.1, player_posz + 0.43); //  front leg
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.015f, 0.015f, 0.1f, 10, 10);
	glPopMatrix();

	glPushMatrix();

	glColor3f(0.9f, 0.8f, 0);
	glTranslated(player_posx + 0.4, player_posy + 0.1, player_posz + 0.47); // back leg
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.015f, 0.015f, 0.1f, 10, 10);
	glPopMatrix();

	//head 
	glPushMatrix();
	glColor3f(0.9f, 0.8f, 0);
	glTranslated(player_posx + 0.5, player_posy + 0.2, player_posz + 0.45); // head
	glutSolidSphere(0.08, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(player_posx + 0.56, player_posy + 0.24, player_posz + 0.47);
	glScaled(0.01, 0.01, 0.01);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(player_posx + 0.56, player_posy + 0.24, player_posz + 0.43);
	glScaled(0.01, 0.01, 0.01);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(player_posx + 0.565, player_posy + 0.18, player_posz + 0.44);
	glScaled(0.01, 0.01, 0.01);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(player_posx + 0.58, player_posy + 0.2, player_posz + 0.45);
	glScaled(0.01, 0.01, 0.01);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();
	glPushMatrix();

	glColor3f(1, 0, 0);
	glTranslated(player_posx + 0.565, player_posy + 0.18, player_posz + 0.46);
	glScaled(0.01, 0.01, 0.01);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();


	glPushMatrix();
	glTranslated(player_posx + 0.5, player_posy + 0.27, player_posz + 0.45);
	glScaled(0.04, 0.025, 0.04);
	glColor3f(0.8f, 0.5f, 0.2f);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.6, 1.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(player_posx + 0.5, player_posy + 0.29, player_posz + 0.45);
	glScaled(0.03, 0.06, 0.03);
	glColor3f(0.8f, 0.5f, 0.2f);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.6, 1.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(player_posx + 0.5, player_posy + 0.31, player_posz + 0.45);
	glScaled(0.02, 0.01, 0.02);
	glColor3f(0.8f, 0.5f, 0.2f);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.6, 1.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8f, 0.5f, 0.2f);
	glTranslated(player_posx + 0.5, player_posy + 0.31, player_posz + 0.45);
	glScaled(0.05, 0.03, 0.05);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();
	//glPushMatrix();
	//glColor3f(0, 0, 0);
	//glTranslated(player_posx + 0.5, player_posy + 0.2, player_posz + 0.45); // head
	//glutSolidSphere(0.08, 10, 10);
	//glPopMatrix();

	//body of the cat
	glPushMatrix();

	glColor3f(0.9f, 0.8f, 0);
	glTranslated(player_posx + 0.35, player_posy + 0.15, player_posz + 0.45);
	glScaled(0.2, 0.1, 0.1); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();
}
void drawFence2(float x) {

	//glPushMatrix();
	//glTranslated(0.025, 0.1,x -0.025);
	//glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	//glutSolidCube(1);
	//glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.30, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.45, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.60, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.75, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.90, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.98, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix(); // CROSS FENCE 
	glTranslated(0.5, 0.1, x - 0.025);
	glRotated(-90, 0.0, 0.0, 1.0);
	glScaled(0.05, 1, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	/// the other half :)
	glPushMatrix();
	glTranslated(-1, 0, 0);
	glPushMatrix();
	glTranslated(0.025, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.30, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.45, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.60, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.75, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.90, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.98, 0.1, x - 0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix(); // CROSS FENCE 
	glTranslated(0.5, 0.1, x - 0.025);
	glRotated(-90, 0.0, 0.0, 1.0);
	glScaled(0.05, 1, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
	/// <param name="x"></param>


}

void drawFence1(float a) {

	//glColor3f(1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.1);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.25);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.40);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.55);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.70);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.85);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix(); // CROSS FENCE 
	glTranslated(-0.025 + a, 0.1, 0.5);
	glRotated(90, 1.0, 0.0, 0.0);
	glScaled(0.05, 1, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();


	// the other half :)
	glTranslated(0, 0, -1);
	glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.1);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.25);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.40);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.55);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.70);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025 + a, 0.1, 0.85);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix(); // CROSS FENCE 
	glTranslated(-0.025 + a, 0.1, 0.5);
	glRotated(90, 1.0, 0.0, 0.0);
	glScaled(0.05, 1, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
}
void drawChristmasTree(float x, float y, float z) {
	glPushMatrix();

	glTranslated(x, 0, z);
	glRotated(-90, 1, 0, 0);
	glColor3f(0, 1, 0);
	glutSolidCone(0.3, 0.5, 10, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslated(x, 0.3, z);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.25, 0.5, 10, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslated(x, 0.6, z);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.2, 0.5, 10, 8);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslated(x - 0.1, 0.5, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 2);
	glTranslated(x, 0.35, z + 0.2); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x + 0.1, 0.5, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();




	//decoration
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x - 0.1, 0.7, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 2);
	glTranslated(x, 0.8, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(x + 0.1, 0.8, z); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();



	//decoration
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x - 0.1, 0.2, z + 0.15);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();




	//decoration
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(x + 0.1, 0.2, z + 0.15);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslated(x - 0.1, 0.5, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 2);
	glTranslated(x, 0.35, z + 0.2); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x + 0.1, 0.5, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();




	//decoration
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x - 0.1, 0.7, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 2);
	glTranslated(x, 0.8, z + 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(x + 0.1, 0.8, z); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();



	//decoration
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x - 0.1, 0.2, z + 0.15);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();




	//decoration
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(x + 0.1, 0.2, z + 0.15);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	//////////////////////////// back decoration

		//decoration
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslated(x - 0.1, 0.5, z - 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 2);
	glTranslated(x, 0.35, z - 0.2); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x + 0.1, 0.5, z - 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();




	//decoration
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x - 0.1, 0.7, z - 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 2);
	glTranslated(x, 0.8, z - 0.1); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(x + 0.1, 0.8, z); // sphere at (1,1,0)
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();



	//decoration
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x - 0.1, 0.2, z - 0.15);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();


}
void drawSnowman(float x, float y, float z) {
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x, y + 0.2, z + 0.15);
	glScaled(0.015, 0.015, 0.015);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x, y + 0.1, z + 0.15);
	glScaled(0.015, 0.015, 0.015);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x, y + 0.3, z + 0.1);
	glScaled(0.015, 0.015, 0.015);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	//beak
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(x, y + 0.45, z + 0.1);
	glScaled(0.05, 0.05, 0.25);
	glutSolidCone(0.3, 0.5, 10, 8);
	glPopMatrix();

	//eyes
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(x - 0.035, y + 0.48, z + 0.08);
	glScaled(0.015, 0.015, 0.015);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(x + 0.035, y + 0.48, z + 0.08);
	glScaled(0.015, 0.015, 0.015);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	/// <summary>
	/// //
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(x, y + 0.1, z);
	glScaled(0.2, 0.15, 0.15);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(x, y + 0.1, z);
	glScaled(0.2, 0.15, 0.15);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(x, y + 0.3, z);
	glScaled(0.2, 0.15, 0.15);
	glutSolidSphere(0.7, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(x, y + 0.45, z);
	glScaled(0.15, 0.15, 0.15);
	glutSolidSphere(0.7, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.38, z);
	glScaled(0.07, 0.04, 0.07);
	glColor3f(0, 0, 0);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.6, 1.2, 10, 10);
	glPopMatrix();

}
void drawJapeneseLantern(float x, float y, float z) {
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(x, y, z);
	glScaled(0.15, 0.6, 0.15);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.3, 0.5, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.18, z);
	glScaled(0.1, 0.3, 0.1);
	glColor3f(0, 0, 0);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.07, 0.6, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y - 0.18, z);
	glScaled(0.1, 0.3, 0.1);
	glColor3f(0, 0, 0);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.07, 0.6, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.2, z - 0.05);
	glColor3f(0, 0, 0);
	glScaled(0.01, 0.1, 0.01); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();


	//decoration
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(x, y + 0.25, z - 0.05);
	glutSolidSphere(0.0, 10, 10);
	glPopMatrix();


}
void drawPresent(float x, float y, float z) {

	//front and back cover 
	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x - 0.1, 0, z + 0.101);
	glVertex3f(x, 0, z + 0.101);
	glVertex3f(x - 0.1, 0.2, z + 0.101);
	glVertex3f(x, 0.2, z + 0.101);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x + 0.1, 0, z + 0.101);
	glVertex3f(x, 0, z + 0.101);
	glVertex3f(x + 0.1, 0.2, z + 0.101);
	glVertex3f(x, 0.2, z + 0.101);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x - 0.1, 0, z - 0.101);
	glVertex3f(x, 0, z - 0.101);
	glVertex3f(x - 0.1, 0.2, z - 0.101);
	glVertex3f(x, 0.2, z - 0.101);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x + 0.1, 0, z - 0.101);
	glVertex3f(x, 0, z - 0.101);
	glVertex3f(x + 0.1, 0.2, z - 0.101);
	glVertex3f(x, 0.2, z - 0.101);
	glEnd();
	glPopMatrix();

	//side views 

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x + 0.101, 0, z + 0.1);
	glVertex3f(x + 0.101, 0, z);
	glVertex3f(x + 0.101, 0.2, z + 0.1);
	glVertex3f(x + 0.101, 0.2, z);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x + 0.101, 0, z - 0.1);
	glVertex3f(x + 0.101, 0, z);
	glVertex3f(x + 0.101, 0.2, z - 0.1);
	glVertex3f(x + 0.101, 0.2, z);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x - 0.101, 0, z + 0.1);
	glVertex3f(x - 0.101, 0, z);
	glVertex3f(x - 0.101, 0.2, z + 0.1);
	glVertex3f(x - 0.101, 0.2, z);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(x - 0.101, 0, z - 0.1);
	glVertex3f(x - 0.101, 0, z);
	glVertex3f(x - 0.101, 0.2, z - 0.1);
	glVertex3f(x - 0.101, 0.2, z);
	glEnd();
	glPopMatrix();


	/////////////////////////////////


	glPushMatrix();
	glColor3f(0, 1, 2);
	glTranslated(x, 0.1, z);
	glScaled(0.2, 0.2, 0.2);
	glutSolidCube(1);
	glPopMatrix();
	//////////////////////////////////
	/// present top
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(x, 0.2, z);
	glScaled(0.201, 0.03, 0.201);
	glutSolidCube(1);
	glPopMatrix();

	//decoration
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslated(x, 0.21, z); // sphere at (1,1,0)
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

}

void drawGameOverScreen() {
	//decoration
	glPushMatrix();
	glColor3f(0, 0, 0);
	glScaled(30, 30, 30);
	glTranslated(0, 0, 0); // sphere at (1,1,0)
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

}

void drawBackground() {
	//decoration
	glPushMatrix();
	glColor3f(0.1f, 0.3f, 4);
	glScaled(30, 30, 30);
	glTranslated(0, 0, 0); // sphere at (1,1,0)
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawBackground();
	if (count_presents == 4 || !timeup_check) {
		drawBackground();
		glPushMatrix();

		glColor3f(1, 0, 0);
		drawWall(0.02);
		glPopMatrix();

		//glPushMatrix();
		//glColor3f(0, 0, 1);
		//glTranslated(0, 0, 0.5);
		//glScaled(0.2, 0.1, 0.1); // z
		//glutSolidCube(1);
		//glPopMatrix();

		//glPushMatrix();
		//glColor3f(0, 1, 0);
		//glTranslated(0.5, 0, 0);
		//glScaled(0.2, 0.1, 0.1); // x
		//glutSolidCube(1);
		//glPopMatrix();

		//glPushMatrix();
		//glColor3f(1, 0, 0);
		//glScaled(0.2, 0.1, 0.1); // center
		//glutSolidCube(1);
		//glPopMatrix();

		//////////////////////////////////////////////////////////////////////////////


		glPushMatrix();
		glTranslated(player_posx + 0.35, player_posy + 0.15, player_posz + 0.45);
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glTranslated(-(player_posx + 0.35), -(player_posy + 0.15), -(player_posz + 0.45));
		glTranslated(player_posx + 0.35, player_posy + 0.15, player_posz + 0.45);
		glRotatef(rotatePlayer_angle, 0.0f, 1.0f, 0.0f);
		glTranslated(-(player_posx + 0.35), -(player_posy + 0.15), -(player_posz + 0.45));
		drawPlayer();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.7, 0, -0.7);
		glRotatef(rotate_tree, 0.0f, 1.0f, 0.0f);
		glTranslated(-(-0.7), 0, -(-0.7));
		drawChristmasTree(-0.7, 0, -0.7);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.7, 0, -0.7);
		glRotatef(rotate_tree, 0.0f, 1.0f, 0.0f);
		glTranslated(-(0.7), 0, -(-0.7));
		drawChristmasTree(0.7, 0, -0.7);
		glPopMatrix();


		/// <summary>
		///  Presents animation
		/// </summary>
		glPushMatrix();


		if (move_Presents) {
			//	glScaled(1, 1, 1.1);
			glTranslated(0, presents_up, 0);
		}


		// draw present 
		if (p1 == false) {
			glPushMatrix();
			//glColor3f(0, 1, 2);
			drawPresent(-0.8, 0, 0.8);
			glPopMatrix();
		}
		if (p2 == false) {
			glPushMatrix();
			//glColor3f(0, 1, 2);
			drawPresent(0.8, 0, 0.8);
			glPopMatrix();
		}

		if (p3 == false) {
			glPushMatrix();
			//glColor3f(0, 1, 2);
			drawPresent(-0.8, 0, 0);
			glPopMatrix();
		}
		if (p4 == false) {
			glPushMatrix();
			//glColor3f(0, 1, 2);
			drawPresent(0, 0, -0.8);
			glPopMatrix();
		}

		glPopMatrix();
		////////////////////////////////////////
		glPushMatrix();
		glRotated(rotateLantern, 0, 1, 0);
		drawJapeneseLantern(-0.3, 0.5, -0.9);
		drawJapeneseLantern(-0.8, 0.4, 0.3);
		drawJapeneseLantern(0.9, 0.6, 0.4);
		glPopMatrix();
		////////////////////////////////////////


		glPushMatrix();
		//glTranslated(0.8, 0, 0.4);
		//glRotatef(-90, 0.0f, 1.0f, 0.0f);
		//glTranslated(-(0.8), 0, -(0.4));
		//gltranslated(0.5f, 0, 0);
		glTranslated(snowman_translate, 0, 0);
		drawSnowman(0, 0, 0);
		glPopMatrix();

		//drawing fences:
		glPushMatrix();
		glTranslated(0, fences_up, 0);

		glColor3f(1, changeColor, 0);
		glPushMatrix();
		drawFence2(-1);
		glPopMatrix();
		glPushMatrix();
		drawFence1(-1);
		glPopMatrix();
		glPushMatrix();
		drawFence1(1.05);
		glPopMatrix();

		glPopMatrix();


	}
	else {
		drawGameOverScreen();


	}


	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'k': //top
		topx = 0;
		topy = 2.5f;
		topz = 0.5f;
		camera.setView(topx, topy, topz);
		break;
	case 'm': //side
		topx = 2.0f;
		topy = 1.7f;
		topz = 0.0f;
		camera.setView(topx, topy, topz);
		break;
	case 'n': // front
		topx = 0.0f;
		topy = 1.7f;
		topz = 2.0f;
		camera.setView(topx, topy, topz);
		break;
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case 'h':
		if (player_posz > -1 && player_posz < 1)
			player_posz += 0.01;
		break;
	case 'p':
		move_Presents = !move_Presents;
		if (move_Presents) {
			PlaySound(TEXT("sound/magic.wav"), NULL, SND_ASYNC | SND_FILENAME);

		}
		if (move_Presents == false)
			presents_up = 0;
		break;
	case 'f':
		move_fences = !move_fences;
		if (move_fences) {
			PlaySound(TEXT("sound/magic.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		if (move_fences == false)
			fences_up = 0;
		break;
	case 'l':
		move_Lantern = !move_Lantern;
		if (move_Lantern) {
			PlaySound(TEXT("sound/magic.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		if (move_Lantern == false)
			rotateLantern = 0;
		break;
	case 't':
		move_tree = !move_tree;
		if (move_tree) {
			PlaySound(TEXT("sound/magic.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		if (move_tree == false)
			rotate_tree = 0;
		break;
	case 'o':
		move_snowman = !move_snowman;
		if (move_snowman) {
			PlaySound(TEXT("sound/magic.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		if (move_snowman == false)
			snowman_translate = 0;
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}
	//	cout << player_posx << "<-x    z-> " << player_posz << endl;
	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;
	float d = 0.01;
	cout << rotatePlayer_angle << " " << endl;
	switch (key) {
	case GLUT_KEY_LEFT:
		rotatePlayer_angle = -90;
		if (player_posx > -1.2)
			player_posx -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		rotatePlayer_angle = 90;

		if (player_posx < 0.55)
			player_posx += 0.01;

		break;

	case GLUT_KEY_UP:
		rotatePlayer_angle = 180;
		if (player_posz > -1.35)
			player_posz -= 0.01;
		break;


		break;
	case GLUT_KEY_DOWN:
		rotatePlayer_angle = 0;
		if (player_posz < 0.5)
			player_posz += 0.01;
		break;

	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Cat with a hat");

	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
	glutIdleFunc(collision);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
