#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_ibID = 0;


	m_programID = 0;
}


CMyApp::~CMyApp(void)
{
}




void CMyApp::InitCan()
{
	Vertex vert[canSmooth];

	for (int i = 0; i < canSmooth / 2; i++) {
		float alpha = 2 * M_PI / (canSmooth / 2) * i;
		vert[i] = { glm::vec3(cos(alpha) / 2,-1.5f,sin(alpha) / 2), glm::vec3(((double)rand() / (RAND_MAX)),((double)rand() / (RAND_MAX)),((double)rand() / (RAND_MAX))) };
	}
	for (int i = 0; i < canSmooth / 2; i++) {
		float alpha = 2 * M_PI / (canSmooth / 2) * i;
		vert[i + (canSmooth / 2)] = { glm::vec3(cos(alpha) / 2,1.5f,sin(alpha) / 2), glm::vec3(((double)rand() / (RAND_MAX)),((double)rand() / (RAND_MAX)),((double)rand() / (RAND_MAX))) };
	}



	GLushort indices[(canSmooth * 3) + (((canSmooth)-4) * 3)];
	for (int i = 0; i < (canSmooth / 2); ++i) {
		indices[(i * 6)] = (i + (canSmooth / 2));
		indices[(i * 6) + 1] = ((i + 1) % (canSmooth / 2));
		indices[(i * 6) + 2] = i;
		indices[(i * 6) + 3] = ((i + 1) % (canSmooth / 2));
		indices[(i * 6) + 4] = (i + (canSmooth / 2));
		indices[(i * 6) + 5] = (((i + 1) % (canSmooth / 2)) + (canSmooth / 2));
	}

	for (int i = 0; i < (canSmooth / 2 - 2); ++i) {
		indices[(canSmooth * 3) + (i * 3)] = (canSmooth / 2);
		indices[(canSmooth * 3) + (i * 3) + 1] = (i + (canSmooth / 2)) + 2;
		indices[(canSmooth * 3) + (i * 3) + 2] = (i + (canSmooth / 2)) + 1;
	}

	for (int i = 0; i < ((canSmooth / 2) - 2); ++i) {
		indices[(canSmooth * 3) + (i * 3) + ((canSmooth / 2 - 2) * 3)] = i + 2;
		indices[(canSmooth * 3) + (i * 3) + ((canSmooth / 2 - 2) * 3) + 1] = 0;
		indices[(canSmooth * 3) + (i * 3) + ((canSmooth / 2 - 2) * 3) + 2] = i + 1;
	}


	/*indices[0] = 5;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 1;
	indices[4] = 5;
	indices[5] = 6;

	indices[6] = 6;
	indices[7] = 2;
	indices[8] = 1;
	indices[9] = 2;
	indices[10] = 6;
	indices[11] = 7;

	indices[12] = 7;
	indices[13] = 3;
	indices[14] = 2;
	indices[15] = 3;
	indices[16] = 7;
	indices[17] = 8;

	indices[18] = 8;
	indices[19] = 5;
	indices[20] = 3;
	indices[21] = 4;
	indices[22] = 8;
	indices[23] = 9;

	indices[24] = 9;
	indices[25] = 0;
	indices[26] = 4;
	indices[27] = 0;
	indices[28] = 9;
	indices[29] = 5;*/


	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_can_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_can_vaoID);

	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_can_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_can_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
		sizeof(vert),		// ennyi b�jt nagys�gban
		vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
		GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat


	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	);

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); // ez lesz majd a sz�n
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_can_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_can_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	InitCan();

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
		0,				// a VAO-beli azonos�t� index
		"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation(m_programID, 1, "vs_in_col");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy(aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] S�der Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	//
	// egy�b inicializ�l�s
	//

	// vet�t�si m�trix l�trehoz�sa
	m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram(m_programID);
}

void CMyApp::Update()
{

	m_matView = glm::lookAt(glm::vec3(0, 40, 30),		// honnan n�zz�k a sz�nteret 50/30
		glm::vec3(0, 0, 0),		// a sz�nt�r melyik pontj�t n�zz�k
		glm::vec3(0, 1, 0));		// felfel� mutat� ir�ny a vil�gban
}

void CMyApp::DrawCan(glm::mat4 trafo)
{
	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_can_vaoID);

	glm::mat4 mvp = m_matProj * m_matView * trafo;

	// majd k�ldj�k �t a megfelel� m�trixot!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre t�lts�nk �t adatot
		1,			// egy darab m�trixot
		GL_FALSE,	// NEM transzpon�lva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

	// kirajzol�s
	glDrawElements(GL_TRIANGLES,	// primit�v t�pus
		(canSmooth * 3) + (((canSmooth)-4) * 3),
		GL_UNSIGNED_SHORT,			// indexek tipusa
		0);							// indexek cime
}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram(m_programID);


	float t2 = SDL_GetTicks();
	float pos = 10 * (sin(t2 / 10000.0 * 2 * M_PI));
	
	for (int i = 0; i < num; ++i) {
		float alpha1 = 2 * M_PI / num * i;
		//Belso formacio
		for (int i = 0; i < 4; ++i) {
			float alpha2 = 2 * M_PI / 4 * i;
			DrawCan(
				glm::rotate<float>(alpha1, glm::vec3(0, 1, 0)) *
				glm::translate(glm::vec3(15, 0, 0)) *
				glm::rotate<float>(alpha1, glm::vec3(0, -1, 0)) *
				glm::translate(glm::vec3(pos, 0, 0.05 * pos * pos)) *
				glm::rotate<float>(alpha2, glm::vec3(0, 1, 0)) *
				glm::translate(glm::vec3(5 * sin(45 * (M_PI / 180)) / sin(90 * (M_PI / 180)), 0, 0))
			);
		}
		for (int i = 0; i < 2; ++i) {
			float alpha2 = 2 * M_PI / 2 * i;
			DrawCan(
				glm::rotate<float>(alpha1, glm::vec3(0, 1, 0)) *
				glm::translate(glm::vec3(15, 0, 0)) *
				glm::rotate<float>(alpha1, glm::vec3(0, -1, 0)) *
				glm::translate(glm::vec3(pos, 0, 0.05 * pos * pos)) *
				glm::rotate<float>(alpha2, glm::vec3(0, 0, 1)) *
				glm::translate(glm::vec3(0, 5 * sin(45 * (M_PI / 180)) / sin(90 * (M_PI / 180)), 0))

			);
		}
	}

	

	
	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram(0);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym) {
	case SDLK_KP_PLUS:
		num++;
		break;
	case SDLK_KP_MINUS:
		num = num <= 0 ? 0 : num - 1;
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(45.0f,		// 90 fokos nyilasszog
		_w / (float)_h,	// ablakmereteknek megfelelo nezeti arany
		0.01f,			// kozeli vagosik
		100.0f);		// tavoli vagosik
}