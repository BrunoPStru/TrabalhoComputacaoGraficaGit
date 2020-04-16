#define _CRT_SECURE_NO_DEPRECATE
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void displayCube();
void keys(int key, int x, int y);
void mouseWheel(int wheel, int direction, int x, int y);

double rotatex = 0;
double rotatey = 0;
double zoom = 0.5;
float color[1000][1000];
float values[10000][10000];
int pos[10000][10000];
int faces = 0;

void displayCube() {
	// Limpa a tela e o Z-Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reinicia transformações
	glLoadIdentity();

	// Rotaciona quando o usuário muda rotate_x e rotate_y
	glRotatef(rotatex, 1.0, 0.0, 0.0);
	glRotatef(rotatey, 0.0, 1.0, 0.0);

	glScalef(zoom, zoom, zoom);

	int p = 0, j = 0;
	for (int i = 0; i < faces; i++) {
		glBegin(GL_POLYGON);
		//glColor3f(color[i][j], color[i][i + 1], color[i][+2]);

		for (int j = 0; j < 3; j++) {
			glColor3f(color[i][0], color[i][1], color[i][2]);
			glVertex3f((values[pos[i][j] - 1][0])/10, (values[pos[i][j] - 1][1])/10, (values[pos[i][j] - 1][2])/10);
			//glColor3f((float) (i + j) / 10, (float)(i + j) / 10, (float)(i + j) / 10);
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

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	// Retorna para o SO
	srand((unsigned int)time(NULL));
	FILE* fp;
	fp = fopen("cubo.obj", "r");
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

		for (int i = 0; i <= faces; i++) {
			color[i][0] = rand() / ((double)RAND_MAX);
			color[i][1] = rand() / ((double)RAND_MAX);
			color[i][2] = rand() / ((double)RAND_MAX);
		}
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
	}
	if (fp != NULL) 
		fclose(fp);
	_getch();
	return 0;
}