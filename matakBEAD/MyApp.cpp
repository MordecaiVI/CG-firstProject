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
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_can_vaoID);

	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_can_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_can_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		sizeof(vert),		// ennyi bájt nagyságban
		vert,	// errõl a rendszermemóriabeli címrõl olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat


	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// index puffer létrehozása
	glGenBuffers(1, &m_can_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_can_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	InitCan();

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
		0,				// a VAO-beli azonosító index
		"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation(m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
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

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	//
	// egyéb inicializálás
	//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);

	// shader-beli transzformációs mátrixok címének lekérdezése
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

	m_matView = glm::lookAt(glm::vec3(0, 40, 30),		// honnan nézzük a színteret 50/30
		glm::vec3(0, 0, 0),		// a színtér melyik pontját nézzük
		glm::vec3(0, 1, 0));		// felfelé mutató irány a világban
}

void CMyApp::DrawCan(glm::mat4 trafo)
{
	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_can_vaoID);

	glm::mat4 mvp = m_matProj * m_matView * trafo;

	// majd küldjük át a megfelelõ mátrixot!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre töltsünk át adatot
		1,			// egy darab mátrixot
		GL_FALSE,	// NEM transzponálva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

	// kirajzolás
	glDrawElements(GL_TRIANGLES,	// primitív típus
		(canSmooth * 3) + (((canSmooth)-4) * 3),
		GL_UNSIGNED_SHORT,			// indexek tipusa
		0);							// indexek cime
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
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

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(45.0f,		// 90 fokos nyilasszog
		_w / (float)_h,	// ablakmereteknek megfelelo nezeti arany
		0.01f,			// kozeli vagosik
		100.0f);		// tavoli vagosik
}