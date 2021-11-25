#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream> 
using namespace std;
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

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

class Camera {
public:
	Vector3f eye, center, up;
	float moveXpos;
	Camera(float eyeX = 1.5f, float eyeY = 1.0f, float eyeZ = 1.7f, float centerX = 1.5f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		//center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
	//	center = center + up.unit() * d;
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
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.0, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawFence(double thickness) {

	glPushMatrix();
	glTranslated(0.15, 0, 0.31);
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
void drawFence2(float x) {
	//glPushMatrix();
	//glTranslated(0.0, 0.15, -0.27);
	//glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
	////Front
	////float x = 0.05f;
	//glColor3f(1.0f, 0.0f, 0.0f);     // Red
	//glVertex3f(0.0f, x, 0.0f);
	//glColor3f(0.0f, 1.0f, 0.0f);     // Green
	//glVertex3f(-x, -x, x);
	//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	//glVertex3f(x, -x, x);

	//// Right
	//glColor3f(1.0f, 0.0f, 0.0f);     // Red
	//glVertex3f(0.0f, x, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	//glVertex3f(x, -x, x);
	//glColor3f(0.0f, 1.0f, 0.0f);     // Green
	//glVertex3f(x, -x, -x);

	//// Back
	//glColor3f(1.0f, 0.0f, 0.0f);     // Red
	//glVertex3f(0.0f, x, 0.0f);
	//glColor3f(0.0f, 1.0f, 0.0f);     // Green
	//glVertex3f(x, -x, -x);
	//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	//glVertex3f(-x, -x, -x);

	//// Left
	//glColor3f(1.0f, 0.0f, 0.0f);       // Red
	//glVertex3f(0.0f, x, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f);       // Blue
	//glVertex3f(-x, -x, -x);
	//glColor3f(0.0f, 1.0f, 0.0f);       // Green
	//glVertex3f(-x, -x, x);

	//glEnd();   // Done drawing the pyramid
	//glPopMatrix();


	glPushMatrix();
	glTranslated(0.025, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.30, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.45, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.60, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.75, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.90, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.98, 0.1, -0.025);
	glScaled(0.05, 0.2, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix(); // CROSS FENCE 
	glTranslated(0.5, 0.1, -0.025);
	glRotated(-90, 0.0, 0.0, 1.0);
	glScaled(0.05, 1, 0.05); // 0.75 is - 
	glutSolidCube(1);

	glPopMatrix();

}

void drawFence1(float a) {

	glColor3f(1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslated(-0.025+a, 0.1, 0.1);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025+a, 0.1, 0.25);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025+a, 0.1, 0.40);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025+a, 0.1, 0.55);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025+a, 0.1, 0.70);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.025+a, 0.1, 0.85);
	glScaled(0.05, 0.2, 0.05); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();

	//glPushMatrix();
	//glTranslated(-0.025, 0.1, 1);
	//glScaled(0.05, 0.2, 0.05); // 0.75 is -
	//glutSolidCube(1);
	//glPopMatrix();

	glPushMatrix(); // CROSS FENCE 
	glTranslated(-0.025+a, 0.1, 0.5);
	glRotated(90, 1.0, 0.0, 0.0);
	glScaled(0.05, 1, 0.05); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();

}

void drawPlayer() {
	glPushMatrix();
	glTranslated(0.3, 0.1, 0.47); // cylinder at (0,0,1)
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.02f, 0.02f, 0.1f, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.3, 0.1, 0.43); // cylinder at (0,0,1)
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.02f, 0.02f, 0.1f, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.4, 0.1, 0.43); // cylinder at (0,0,1)
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.015f, 0.015f, 0.1f, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.4, 0.1, 0.47); // cylinder at (0,0,1)
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.015f, 0.015f, 0.1f, 10, 10);
	glPopMatrix();

	//head 
	glPushMatrix();
	glTranslated(0.5, 0.2, 0.45); // sphere at (1,1,0)
	glutSolidSphere(0.08, 10, 10);
	glPopMatrix();

	//body of the cat
	glPushMatrix();
	glTranslated(0.35, 0.15, 0.45);
	glScaled(0.2, 0.1, 0.1); // 0.75 is -
	glutSolidCube(1);
	glPopMatrix();
}
void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslated(0.0, 0.0, 0.0);
	glScaled(0.07, 0.2, 0.1); // 0.75 is - 
	glutSolidCube(1);
	glPopMatrix();
	//glPushMatrix();
	//glTranslated(0.4, 0.4, 0.6);
	//glRotated(45, 0, 0, 1);
	//glScaled(0.08, 0.08, 0.08);
	//drawJack();
	//glPopMatrix();

	//glPushMatrix();
	//glTranslated(0.6, 0.1, 0.6);
	//glRotated(45, 0, 0, 1);
	//glScaled(0.08, 0.08, 0.08);
	//drawJack();
	//glPopMatrix();

	//glPushMatrix();
	//glTranslated(0.6, 0.38, 0.5);
	//glRotated(30, 0, 1, 0);
	//glutSolidTeapot(0.08);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslated(0.25, 0.42, 0.35);
	//glutSolidSphere(0.1, 15, 15);
	//glPopMatrix();
	

	
    glTranslated(1, 0.0, 0.0);
	glPushMatrix();
	drawPlayer();
	glPopMatrix();

	//drawing floor:
	glPushMatrix();
	glColor3f(1.0, 0.0, 1.0);
	drawWall(0.02);
	glPopMatrix();

	//drawing fences:
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	drawFence2(0.05);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	drawFence1(0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	drawFence1(1.05);
	glPopMatrix();

	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
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

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
float a = 1.0;
void Special(int key, int x, int y) {
	a += 0.1;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");

	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
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
