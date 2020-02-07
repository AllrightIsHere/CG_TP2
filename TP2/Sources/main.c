#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Headers/glm.h"
#include "../Headers/glmint.h"
#include "../Headers/coordenadas.h"
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_mixer.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct camera {
    vetor posicao, direcao, up;
    float fi, teta, raio;
} Camera;

GLuint textura; //textura da skybox atual
GLuint texturaDia; //textura skybox de dia
GLuint texturaNoite; //textura skybox de noite

GLMmodel* cidade = NULL; //Obj da Cidade
GLMmodel* helicoptero = NULL; //corpo do helicóptero
GLMmodel* heliceCentral = NULL; //hélice central
GLMmodel* heliceCauda = NULL; //hélice da cauda

float ang = 0; //angulo de rotação das hélices
float anguloDoHelicoptero = 0;  //para rotacionar o helicoptero
float velocidade = 6; //velocidade do objeto principal

int volume_musica = 100; //volume dó som
Mix_Music *musicAtual = NULL; // variável da música

int tempo = 0; //tempo para noite e dia da skybox

int habilitaIluminacao = 0; //habilita a iluminação
float corIluminacao[4] = {0.5, 0.5, 0.5, 1}; //cor da iluminação

int habilitaNeblina = 0;

Camera cam; //câmera

vetor posObj = (vetor){0, 0, 0, 0};
vetor frente = (vetor) {0, 0, 1, 0};
vetor direita = (vetor) {1, 0, 0, 0};

int selecionaCamera = 1;
int camAux = 0;

void iniciar_musica(char *music){
	if(!Mix_PlayingMusic()){
		Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,1024);
		musicAtual=Mix_LoadMUS(music);
		Mix_VolumeMusic(volume_musica);
		Mix_PlayMusic(musicAtual,-1);
	}
}

void atualizaCamera(){
    cam.direcao.x = cos(cam.teta)*sin(cam.fi)*cam.raio + cam.posicao.x;
    cam.direcao.y = cos(cam.fi)*cam.raio + cam.posicao.y;
    cam.direcao.z = -sin(cam.teta)*sin(cam.fi)*cam.raio + cam.posicao.z;

    cam.up.x = 0;
    cam.up.y = 1;
    cam.up.z = 0;
}

void camera(){
    switch(selecionaCamera){
        case 1:{ 
            vetor aux = normalizado(frente);
            cam.posicao.x = posObj.x - aux.x*cam.raio;
            cam.posicao.y = posObj.y - aux.y*cam.raio;
            cam.posicao.z = posObj.z - aux.z*cam.raio;

            cam.fi = acos(frente.y/normaDoVetor(frente));
            cam.teta = atan(-frente.z/ frente.x);
            break;}
        case 2:
            if(camAux == 0) {
                vetor aux = normalizado(frente);
                cam.posicao.x = posObj.x + aux.x*cam.raio;
                cam.posicao.y = posObj.y + aux.y*cam.raio;
                cam.posicao.z = posObj.z + aux.z*cam.raio;

                cam.teta = acos(frente.y/normaDoVetor(frente)) - M_PI;
                cam.fi = atan(frente.z/ frente.x) - M_PI;
              }
            else if(camAux == 1){
                vetor aux = normalizado(direita);
                cam.posicao.x = posObj.x - aux.x*cam.raio;
                cam.posicao.y = posObj.y - aux.y*cam.raio;
                cam.posicao.z = posObj.z - aux.z*cam.raio;

                cam.fi = acos(direita.y/normaDoVetor(direita));
                cam.teta = atan(direita.z/ direita.x);
            }
            else if(camAux == 2){
                vetor aux = normalizado(direita);
                cam.posicao.x = posObj.x + aux.x*cam.raio;
                cam.posicao.y = posObj.y + aux.y*cam.raio;
                cam.posicao.z = posObj.z + aux.z*cam.raio;

                cam.teta = acos(direita.y/normaDoVetor(direita)) - M_PI/2.0;
                cam.fi = atan(direita.z/ direita.x) - M_PI/2.0;
            }
            else if(camAux == 3){
                cam.posicao.x = posObj.x; 
                cam.posicao.y = posObj.y + 10; //10 é a altura acima do helicóptero
                cam.posicao.z = posObj.z;

                cam.fi = M_PI/2.0 + 0.4; 
                cam.teta = anguloDoHelicoptero*M_PI/180.0; 
            }

            else if(camAux == 4){
                cam.posicao.x = posObj.x + 15.48*cos(anguloDoHelicoptero*M_PI/180.0); //15.48 é a distancia entre o objeto e a posição da camera
                cam.posicao.y = posObj.y + 0.7;
                cam.posicao.z = posObj.z + 15.48*-sin(anguloDoHelicoptero*M_PI/180.0);//15.48 é a distancia entre o objeto e a posição da camera

                cam.fi = M_PI/2.0 + 0.4; 
                cam.teta = anguloDoHelicoptero*M_PI/180.0; 
            }
            break;
    }
    atualizaCamera();
    gluLookAt(cam.posicao.x, cam.posicao.y, cam.posicao.z,
                cam.direcao.x, cam.direcao.y, cam.direcao.z,
                cam.up.x, cam.up.y, cam.up.z);
}

void drawCidade(void) //desenha a cidade
{
    glmDraw(cidade, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void drawHelicoptero(void){ // desenha a parte central do helicóptero
	glmDraw(helicoptero, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void drawHeliceCentral(void){ //desenha a hélice central
	glmDraw(heliceCentral, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void drawHeliceCauda(void){ //desenha a hélice da cauda
	glmDraw(heliceCauda, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void drawNeblina(){
	float corNeblina[4] = {0.7, 0.7, 0.7, 1}; //cor da neblina
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.01); //densidade da neblina
    glFogfv(GL_FOG_COLOR, corNeblina);
}

void inicializa() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    cam.posicao.x = 0;
    cam.posicao.y = 0;
    cam.posicao.z = 0;
    cam.posicao.w = 0;

    cam.teta = 0;
    cam.fi = 0;
    cam.raio = 100;

    glClearColor(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texturaDia = SOIL_load_OGL_texture(
        "../Skybox/skybox.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y);

    texturaNoite = SOIL_load_OGL_texture(
        "../Skybox/skyboxNoite.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y);

    cidade = glmReadOBJ("../Objects/cidade.obj");
    if (!cidade) {
      printf("Erro ao carregar aquivo .obj: cidade.obj\n");
      exit(0);
    }
    glmUnitize(cidade);
    glmFacetNormals(cidade);
    glmVertexNormals(cidade, 90.0, 1);

    helicoptero = glmReadOBJ("../TP_OBJ_PR/Helicopter.obj");
    if (!helicoptero) {
      printf("Erro ao carregar aquivo .obj: Helicopter.obj\n");
      exit(0);
    }
    glmUnitize(helicoptero);
    glmFacetNormals(helicoptero);
    glmVertexNormals(helicoptero, 90.0, 1);

    heliceCentral = glmReadOBJ("../TP_OBJ_PR/HeliceCentral.obj");
    if (!heliceCentral) {
      printf("Erro ao carregar aquivo .obj: HeliceCentral.obj\n");
      exit(0);
    }
    glmUnitize(heliceCentral);
    glmFacetNormals(heliceCentral);
    glmVertexNormals(heliceCentral, 90.0, 1);

    heliceCauda = glmReadOBJ("../TP_OBJ_PR/HeliceSecundaria.obj");
    if (!heliceCauda) {
      printf("Erro ao carregar aquivo .obj: HeliceSecundaria.obj\n");
      exit(0);
    }
    glmUnitize(heliceCauda);
    glmFacetNormals(heliceCauda);
    glmVertexNormals(heliceCauda, 90.0, 1);

    iniciar_musica("../Sound/som.mp3");
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)(w)/h, 0.1, 200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void desenhaSphere(GLuint texture, Camera aux){ // desenha a skybox
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
		GLUquadric* quadObj = gluNewQuadric();
		gluQuadricDrawStyle(quadObj, GLU_FILL);
		gluQuadricNormals(quadObj, GLU_SMOOTH);
		gluQuadricTexture(quadObj, GL_TRUE);
		glPushMatrix();
			glTranslatef(posObj.x, posObj.y, posObj.z);
			glScalef(aux.raio, aux.raio, aux.raio);
			gluSphere(quadObj, 1, 100, 100);
		glPopMatrix();
		gluDeleteQuadric(quadObj);
	glDisable(GL_TEXTURE_2D);
}


int momentoAnterior;
float fps = 0;

void iluminacao(){
	if (habilitaIluminacao) {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT_MODEL_AMBIENT);
    } else {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT_MODEL_AMBIENT);
    }

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, corIluminacao);
}


void desenhaCena() {

    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;

    // apaga a tela para desenhar de novo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera();
    glColor3d(0, 0, 0);

    glPushMatrix();
      glScalef(5000, 5000, 5000);
      drawCidade();
    glPopMatrix();

    glPushMatrix();        
        glTranslatef(posObj.x, posObj.y, posObj.z);
        glRotated(anguloDoHelicoptero, 0, 1, 0); //angulo do helicoptero
        glScalef(20, 20, 20);
        drawHelicoptero();
        glPushMatrix();
        	glTranslatef(0.4 , 0.18, 0);
            glRotated(ang,0, 1, 0);
        	drawHeliceCentral();
        glPopMatrix();
        glPushMatrix();
        	glTranslatef(-0.9, 0.2, 0);
            glRotated(ang,0, 0, 1);
        	drawHeliceCauda();
        glPopMatrix();
    glPopMatrix();
    
    // desenha skybox
    desenhaSphere(textura, cam);

    if(habilitaNeblina == 1){
    	//desenha neblina
    	drawNeblina();
	}else{
		glDisable(GL_FOG);
	}

    //configura a iluminação
    iluminacao();
	
    // calcula quantos quadros por segundo está chamando a desenha
    // usamos o MAX(delta, 1) pra evitar divisão por 0
    fps = 1000.0f / MAX(delta, 1.0f);

    momentoAnterior = momentoAtual;

    // troca os buffers e manda desenhar
    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case '1': // 1
            selecionaCamera = 1;
            break;
        case '2': // 2
            selecionaCamera = 2;
            break;
        case 'c':
        case 'C': // 2
            if(selecionaCamera == 2){
                camAux = (camAux + 1) % 5;
                printf("Passou a usar a camera %d\n", camAux);
            }
            break;
        case 'd':
        case 'D': //aumenta o angulo no sentido horário
            anguloDoHelicoptero-=5;
            if(anguloDoHelicoptero>=360){anguloDoHelicoptero=0;}
            if(anguloDoHelicoptero<=-360){anguloDoHelicoptero=0;}
            break;
        case 'a':
        case 'A': //aumenta o angulo no sentido anti-horário
            anguloDoHelicoptero+=5;
            if(anguloDoHelicoptero>=360){anguloDoHelicoptero=0;}
            if(anguloDoHelicoptero<=-360){anguloDoHelicoptero=0;}
            break;

        case 'w':
        case 'W': //aumenta a velocidade
        		if(velocidade <= 12)
        			velocidade++;
            break;

        case 's':
        case 'S'://diminui a velocidade
        		if(velocidade > 0)
        			velocidade--;
            break;
        case 'l':
        case 'L':
        		habilitaIluminacao = (habilitaIluminacao + 1) % 2;
        		break;
        case 'n':
        case 'N':
        		habilitaNeblina = (habilitaNeblina + 1) % 2;
        		break;
    }
    glutPostRedisplay();
}

void atualiza() {
    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;

    vetor velocidadeObj = (vetor){ velocidade * (float)delta / 1000.0 * cos(anguloDoHelicoptero*M_PI/180.0), 0, velocidade * (float)delta / 1000.0 * -sin(anguloDoHelicoptero*M_PI/180.0), 0 };
    posObj = somaPontoComVetor(posObj, velocidadeObj);

    ang +=10 + velocidade; 
    if(ang >= 360){ang = 0;}

    tempo = (tempo + 1) % 2000 ;

    if(tempo <= 999){
    	textura = texturaDia;        
    }
    else{
    	textura = texturaNoite;	
    }

    glutPostRedisplay();
}

char tituloDaJanela[200];
void atualizaFPS(int periodo) {
    sprintf(tituloDaJanela,
        "TP2 (%.2f fps)",
        fps);
    glutSetWindowTitle(tituloDaJanela);
    glutTimerFunc(periodo, atualizaFPS, periodo);
}


int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("TP2");

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutIdleFunc(atualiza);
    glutTimerFunc(0, atualizaFPS, 1000);
    glutKeyboardFunc(teclado);

    glewInit();
    inicializa();

    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GLM_SMOOTH);


    glutMainLoop();
    return 0;
}

