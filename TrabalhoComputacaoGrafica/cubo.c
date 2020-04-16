#define _CRT_SECURE_NO_DEPRECATE
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void generateObject();
void displayCube();
void keys(int key, int x, int y);
void mouseWheel(int wheel, int direction, int x, int y);

double rotatex = -15;
double rotatey = 15;
double translatex = 0;
double translatey = 0;
double translatez = 0;
double zoom = 0.5;
float color[1000][1000];
float values[1000][1000];
int pos[1000][1000];
int faces = 0;

void generateObject() {
	FILE* fp;
	//Abre o arquivo .obj
	fp = fopen("cubo.obj", "r");

	//Leitura do .obj
	if (fp == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else {
		char info[50];
		int i = 0, j = 0, k = 0;
		while ((fgets(info, sizeof(info), fp)) != NULL) {
			char* p;
			p = strtok(info, " ");
			if (strcmp(p, "v") == 0)
			{
				p = strtok(NULL, " ");
				for (int ii = 0; p; ii++) {
					//Armazena em algum lugar
					values[i][ii] = atof(p);
					p = strtok(NULL, " ");
				}
				i++;
			}
			else if (strcmp(p, "f") == 0) {
				p = strtok(NULL, " ");
				for (int jj = 0; p; jj++) {
					pos[j][jj] = atoi(p);
					p = strtok(NULL, " ");
				}
				j++;
			}
			faces = j;
		}

		//Gera randomicamente as cores do objeto
		for (int i = 0; i <= faces; i++) {
			color[i][0] = rand() / ((double)RAND_MAX);
			color[i][1] = rand() / ((double)RAND_MAX);
			color[i][2] = rand() / ((double)RAND_MAX);
		}

		if (fp != NULL)
			fclose(fp);
	}
}

void displayCube() {
	// Limpa a tela e o Z-Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reinicia transformações
	glLoadIdentity();

	// Rotaciona quando o usuário muda rotate_x e rotate_y
	glRotatef(rotatex, 1.0, 0.0, 0.0);
	glRotatef(rotatey, 0.0, 1.0, 0.0);
	glTranslatef(translatex, 0.0, 0.0);
	glTranslatef(0.0, translatey, 0.0);
	glTranslatef(0.0, 0.0, translatez);

	glScalef(zoom, zoom, zoom);

	int p = 0, j = 0;
	for (int i = 0; i < faces; i++) {
		glBegin(GL_POLYGON);

		for (int j = 0; j < 3; j++) {
			glColor3f(color[i][0], color[i][1], color[i][2]);
			glVertex3f((values[pos[i][j] - 1][0])/10, (values[pos[i][j] - 1][1])/10, (values[pos[i][j] - 1][2])/10);
		}
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}

void keys(int key, int x, int y) {
	// Seta direita - aumenta rotação em 5 graus
	if (key == GLUT_KEY_RIGHT)
		rotatey += 5;

	// Seta para esquerda - diminui a rotação por 5 graus
	else if (key == GLUT_KEY_LEFT)
		rotatey -= 5;

	else if (key == GLUT_KEY_UP)
		rotatex += 5;

	else if (key == GLUT_KEY_DOWN)
		rotatex -= 5;

	else if (key == GLUT_KEY_F1)
		translatex -= 0.2;

	else if (key == GLUT_KEY_F2)
		translatex += 0.2;

	else if (key == GLUT_KEY_F3)
		translatey -= 0.2;

	else if (key == GLUT_KEY_F4)
		translatey += 0.2;

	else if (key == GLUT_KEY_F5)
		translatez -= 0.2;

	else if (key == GLUT_KEY_F6)
		translatez += 0.2;

	// Requisitar atualização do display
	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y) {
	//Scrool do mouse aumenta ou diminui o zoom no objeto
	wheel = 0;
	if (direction == -1)
	{
		zoom -= 0.5;
	}
	else if (direction == +1)
	{
		zoom += 0.5;
	}

	// Requisitar atualização do display
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	// Retorna para o SO
	srand((unsigned int)time(NULL));
	
	//Função gera objeto
	generateObject();

	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Objeto");

	// Habilita o teste de profundidade do Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Funções
	glutDisplayFunc(displayCube);
	glutSpecialFunc(keys);
	glutMouseWheelFunc(mouseWheel);

	// Passa o controle dos eventos para o GLUT
	glutMainLoop();

	return 0;
}