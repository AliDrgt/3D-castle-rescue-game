#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// angle of rotation for the camera direction
float angle=0.0;
float rotateAngle=30.0;
float lx=0.0f,ly=0.0f,lz=-1.0f;
float x=0.0f,y=2.0f,z=50.0f;
float dragonSteps = 0;
bool dragonMovingLeft = false;
GLUquadricObj *cylinder = gluNewQuadric();
float knightx = 0, knightz = 10;
bool isKnightJumping = false;
float knightY = 0.0f;
float dragonz = -20.0f, dragony = 5.0f;
float princessX = 10, princessY = 40, princessZ = -100;



bool isDay = true;
bool isNight = false;
GLfloat castle_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat castle_shininess = 0.0f;
GLfloat knight_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat knight_shininess = 100.0f;


//ALI FATIH DURGUT 20190702068

struct BoundingBox {
    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;
};

BoundingBox knightBoundingBox;
BoundingBox dragonBoundingBox;
BoundingBox princessBoundingBox;

void updateBoundingBox() {
    // Assuming the knight is a cube with size 2.
    //Knight
    float size1 = 2.0f;
    knightBoundingBox.minX = knightx - size1 / 2.0f;
    knightBoundingBox.maxX = knightx + size1 / 2.0f;
    knightBoundingBox.minY = knightY - size1 / 2.0f;
    knightBoundingBox.maxY = knightY + size1 / 2.0f;
    knightBoundingBox.minZ = knightz - size1 / 2.0f;
    knightBoundingBox.maxZ = knightz + size1 / 2.0f;

    //Dragon
    float size2 = 10.0f;
    dragonBoundingBox.minX = dragonSteps - size2 / 2.0f;
    dragonBoundingBox.maxX = dragonSteps + size2 / 2.0f;
    dragonBoundingBox.minY = dragony - size2 / 2.0f;
    dragonBoundingBox.maxY = dragony + size2 / 2.0f;
    dragonBoundingBox.minZ = dragonz - size2 / 2.0f;
    dragonBoundingBox.maxZ = dragonz + size2 / 2.0f;


    float size3 = 5.0f;
    princessBoundingBox.minX = princessX - size2 / 2.0f;
    princessBoundingBox.maxX = princessX + size2 / 2.0f;
    princessBoundingBox.minY = princessY - size2 / 2.0f;
    princessBoundingBox.maxY = princessY + size2 / 2.0f;
    princessBoundingBox.minZ = princessZ- size2 / 2.0f;
    princessBoundingBox.maxZ = princessZ + size2 / 2.0f;
}

bool doesIntersect(BoundingBox box1, BoundingBox box2) {
    return (box1.minX <= box2.maxX && box1.maxX >= box2.minX) &&
           (box1.minY <= box2.maxY && box1.maxY >= box2.minY) &&
           (box1.minZ <= box2.maxZ && box1.maxZ >= box2.minZ);

}

void processNormalKeys(unsigned char key, int xx, int yy) {
	float fraction = 0.5f;
	switch (key) {
	    //camera
		case 'g' :
			angle -= 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case 'j' :
			angle += 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case 'y' :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case 'h' :
			x -= lx * fraction;
			z -= lz * fraction;
			break;

	//knight move
        case 's' :
            knightz+=0.8f;
			break;
		case 'w' :
            knightz-=0.8f;
			break;
		case 'd' :
            knightx+=0.8f;
			break;
		case 'a' :
            knightx-=0.8f;
			break;
        case 'e' :
            if (!isKnightJumping){
                isKnightJumping = true;
            }
            break;

    //Day night cycle
        case '1':
            isDay = true;
            isNight = false;
            break;
        case '0':
            isDay = false;
            isNight = true;
            break;

        case 'q':
            exit(1);
            break;

	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45,ratio,0.1,1000);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}



void drawTreeConic(float x, float y,float z){
    glPushMatrix();
    glTranslatef(x,y,z);
    glColor3f(0.8f,0.4f,0.2f);           //silindir kok
    glRotatef(270.0f,1.0f, 0.0f, 0.0f);
    gluCylinder(cylinder, 2, 2, 5, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.500, 0.0);
    glTranslatef(x,y+2,z);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f); //ust koni
    glutSolidCone(6,12,10,10);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();
}



void drawTreeSpher(float x, float y,float z){
    glPushMatrix();
    glTranslatef(x,y,z);
    glColor3f(0.8f,0.4f,0.2f);           //silindir kok
    glRotatef(270.0f,1.0f, 0.0f, 0.0f);
    gluCylinder(cylinder, 2, 2, 5, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0f, 0.85f, 0.0f);
    glTranslatef(x,y+10,z);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f); //ust daire
    glutSolidSphere(6.0f,10,10);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();


}

void drawWall(){
    glColor3f(0.54f,0.54f,0.54f);
    glBegin(GL_QUADS);
    glVertex2f(-500.0f, 0.0f);
    glVertex2f(-20.0f, 0.0f);
    glVertex2f(-20.0f, 9.0f);
    glVertex2f(-500.0f, 9.0f);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(20.0f, 0.0f);
    glVertex2f(500.0f, 0.0f);
    glVertex2f(500.0f, 9.0f);
    glVertex2f(20.0f, 9.0f);
    glEnd();


}

void drawCastle(){
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -100.0f);
    glScalef(5.0f, 5.0f, 1.0f);  //kale
    glColor3f(0.54f,0.54f,0.54f);
    glutSolidCube(15);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -100.0f);
    glScalef(5.0f, 5.0f, 1.0f);         //kapi
    glColor3f(0.9f,0.9f,0.9f);
    gluCylinder(cylinder, 5, 5, 20, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(35.0f, 0.0f, -90.0f);
    glScalef(5.0f, 5.0f, 1.0f);
    glColor3f(0.1f, 0.1f, 0.1f);// sag kule
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(cylinder, 1, 1, 10, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-35.0f, 0.0f, -90.0f);
    glScalef(5.0f, 5.0f, 1.0f);
    glColor3f(0.1f, 0.1f, 0.1f);// sol kule
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(cylinder, 1, 1, 10, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0f, 35.0f, -100.0f);
    glScalef(1.0f, 2.0f, 0.0f);
    glColor3f(0.1f, 0.1f, 0.1f);// bayrak diregi
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(cylinder, 1, 1, 10, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0f, 50.0f, -100.0f);//bayrak
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
    glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(-5.0f, 0.0f, 0.0f);
    glScalef(5.0f,3.0f, 1.0f);
    glutSolidCube(2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
    glPopMatrix();

}


void drawDragon(float x, float y, float z){
    //kafa
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(x,y,z);
	glutSolidSphere(5.0f,20,20);
	glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();

    // gozler
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(x+2.75f, y+1.5f, z+4.0f);
	glutSolidSphere(1.0f,10,10);
	glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();

    glPushMatrix();
	glTranslatef(x-2.75f, y+1.5f, z+4.0f);
	glutSolidSphere(1.0f,10,10);
	glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
	glPopMatrix();

	//boynuz
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(x-2.75f, y+3.5f, z);
    glRotatef(270.0f, 1.0f, 1.0f, 0.0f);
    glutSolidCone(2,4,10,10);
    glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(x+2.75f, y+3.5f, z);
    glRotatef(270.0f, 1.0f, -1.0f, 0.0f);
    glutSolidCone(2,4,10,10);
    glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();




}

void drawKnight(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y + knightY, z);
    glColor3f(0.83f, 0.83f, 0.83f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f); // Body
    gluCylinder(cylinder, 2, 2, 5, 10, 2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, y + 5 + knightY, z);
    glColor3f(0.0f, 0.15f, 0.15f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f); // Head
    glutSolidSphere(2.0f, 10, 10);
    glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x - 2, y + 2 + knightY, z);
    glRotatef(230.0f, 1.0f, 0.0f, 0.0f); // Sword
    glColor3f(0.0, 0.500, 1.0);
    glutSolidCone(1, 4, 10, 10);
    glMaterialfv(GL_FRONT, GL_SPECULAR, knight_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, knight_shininess);
    glPopMatrix();

}

void drawPrincess(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 0.0f, 1.0f);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f); // Body
    gluCylinder(cylinder, 2, 2, 5, 10, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, y + 5, z);
    glColor3f(2.0f, 0.5f, 1.0f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f); // Head
    glutSolidSphere(2.0f, 10, 10);
    glPopMatrix();

    GLfloat castle_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat castle_shininess = 0.0f;

    glMaterialfv(GL_FRONT, GL_SPECULAR, castle_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, castle_shininess);
}



void renderScene(void) {
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z,x + lx,y + ly,z + lz,0.0f,1.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw ground
	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);
    glVertex3f(-500.0f, 0.0f, -500.0f);
    glVertex3f(-500.0f, 0.0f,  500.0f);
    glVertex3f( 500.0f, 0.0f,  500.0f);
    glVertex3f( 500.0f, 0.0f, -500.0f);
	glEnd();
    drawWall();

    drawTreeConic(50.0f, 0.0f, -100.0f);
    drawTreeSpher(70.0f, 0.0f, -80.0f);
    drawTreeConic(100.0f, 0.0f, -100.0f);
    drawTreeSpher(90.0f, 0.0f, -60.0f);

    drawTreeConic(-50.0f, 0.0f, -100.0f);
    drawTreeSpher(-70.0f, 0.0f, -80.0f);
    drawTreeConic(-100.0f, 0.0f, -100.0f);
    drawTreeSpher(-90.0f, 0.0f, -60.0f);


    drawTreeConic(-50.0f, 0.0f, -50.0f);
    drawTreeSpher(-70.0f, 0.0f, -40.0f);
    drawTreeConic(-100.0f, 0.0f, -50.0f);
    drawTreeSpher(-90.0f, 0.0f, -30.0f);

    drawTreeConic(50.0f, 0.0f, -50.0f);
    drawTreeSpher(70.0f, 0.0f, -40.0f);
    drawTreeConic(100.0f, 0.0f, -50.0f);
    drawTreeSpher(90.0f, 0.0f, -30.0f);
    updateBoundingBox();
    drawKnight(knightx, 0.0f, knightz);
    drawPrincess(princessX,princessY,princessZ);

    drawDragon(dragonSteps ,dragony, -20.0f);
    updateBoundingBox();
    drawCastle();
    if (dragonMovingLeft) {
        if (dragonSteps > -30) {
            dragonSteps-= 0.3f;
        } else {
            dragonMovingLeft = false;
        }
    } else {
        if (dragonSteps < 30) {
            dragonSteps+= 0.3f;
        } else {
            dragonMovingLeft = true;
        }
    }

    if (isDay){
        // Enable sunlight
        glEnable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    } else if (isNight) {
        // Enable emissive light
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }

    if (isKnightJumping) {
        knightY += 5.8f;
        if (knightY > 30.0f) { // max height of jump
            isKnightJumping = false;
        }
    } else if (knightY > 0.0f) {
        knightY -= 5.8f;
        if (knightY < 0.0f) {
            knightY = 0.0f;
        }
    }
    //Hit dragon to kill it
    if (doesIntersect(knightBoundingBox, dragonBoundingBox)) {
        dragony = 200.0f;
    }

    //Jump to princess to save her
    if (doesIntersect(knightBoundingBox, princessBoundingBox)) {
        princessY = 0.0f;
        princessX = 0.0f;
        princessZ = -60;
    }

    glutPostRedisplay();
    rotateAngle += 0.5f;
	glutSwapBuffers();
}




int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(700,700);
	glutCreateWindow("my game xd");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

    // Set up sunlight properties
    GLfloat sunlight_position[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat sunlight_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat sunlight_specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, sunlight_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunlight_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunlight_specular);

// Set up emissive light properties
    GLfloat emissive_position[] = {0.0, 10.0, -90.0, 1.0};
    GLfloat emissive_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat emissive_diffuse[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat emissive_emission[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat emissive_specular[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, emissive_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, emissive_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, emissive_diffuse);
    glLightfv(GL_LIGHT1, GL_EMISSION, emissive_emission);
    glLightfv(GL_LIGHT1, GL_SPECULAR, emissive_specular);


    glClearColor(0.0,0.0,1.0,0.0);
	glutMainLoop();
	return 1;
}
