#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string tetromino[7];
int s_FieldWidth = 12;
int s_FieldHeight = 18;
static const u32 s_MapWidth = 24;
static char* s_Field = nullptr;

// Game Logic
float Timer = 0.f;
int nCurrentPiece = 0;
int nCurrentRotation = 0;
int nCurrentX = s_FieldWidth / 2;
int nCurrentY = s_FieldHeight;
int nSpeed = 20;
int nSpeedCount = 0;
bool bForceDown = false;
bool bRotateHold = true;
bool bMoveHold = true;
int nPieceCount = 0;
int nScore = 0;
std::vector<int> vLines;
bool bGameOver = false;

static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDWWWWWWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDWWWDDDDDDDDDDDDWWW"
"WDDDDDWWWDDDDDDDDDDDDDWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWWDDDDDDDDDDDDDDWWWWW"
"WWWWWWDDDDDDDDDDDWWWWWWW"
"WWWWWWWDDDDDDDDDWWWWWWWW"
"WWWWWWWWWWDDDDWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
	:Layer("Snadbox2D"), m_CameraController(16.f / 9.f)
{
}


int Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
		//12 13 14 15

		// 12 13 14 15
		// 8 9 10 11
		// 4 5 6 7
		// 0 1 2 3
	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
		//15 11  7  3

		// 0 4 8 12
		// 1 5 9 13
		// 2 6 10 15
		// 3 7 11 16

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
		// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY - py) * s_FieldWidth + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < s_FieldWidth)
			{
				if (nPosY - py >= 0 && nPosY - py < s_FieldHeight)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pi] != '.' && s_Field[fi] != 'E')
						return false; // fail on first hit
				}
			}
		}
	return true;
}

void Sandbox2D::OnAttach()
{
	STS_PROFILE_FUNCTION();

	tetromino[0] = "..I...I...I...I.";
	tetromino[1] = "..S..SS...S.....";
	tetromino[2] = ".....OO..OO.....";
	tetromino[3] = "..T..TT..T......";
	tetromino[4] = ".Z...ZZ...Z.....";
	tetromino[5] = ".L...L...LL.....";
	tetromino[6] = "..J...J..JJ.....";

	s_Field = new char[s_FieldWidth * s_FieldHeight]; // Create play field buffer
	for (int x = 0; x < s_FieldWidth; x++) // Board Boundary
		for (int y = 0; y < s_FieldHeight; y++)
			s_Field[y * s_FieldWidth + x] = (x == 0 || x == s_FieldWidth - 1 || y == 0) ? 'W' : 'E';

	m_CheckerboardTexture = Steins::Texture2D::Create("assets/textures/Checkerboard2.png");
	m_SpriteSheet = Steins::Texture2D::Create("game/textures/RPGpack_sheet_2X.png");
	m_TetrisSheet = Steins::Texture2D::Create("assets/textures/Tetris_Sheet.png");

	m_TextureStairs = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0,11 }, { 128,128 });
	m_TextureTree = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 }, { 1,2 });

	m_Wall = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 3,3 }, { 32,32 });
	m_Empty = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0,0 }, { 128,128 });
	m_Test = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 1,3 }, { 32,32 });
	m_IBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 0,3 }, { 32,32 });
	m_SBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 3,4 }, { 32,32 });
	m_OBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 2,4 }, { 32,32 });
	m_TBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 2,3 }, { 32,32 });
	m_ZBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 0,4 }, { 32,32 });
	m_LBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 1,4 }, { 32,32 });
	m_JBlock = Steins::SubTexture2D::CreateFromCoords(m_TetrisSheet, { 1,3 }, { 32,32 });

	//STS_TRACE("{0}", Steins::Texture2D::)

	m_MapWidth = s_MapWidth;
	m_MapHeight = (u32)(strlen(s_MapTiles) / s_MapWidth);

	s_TextureMap['D'] = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6,11 }, { 128,128 });
	s_TextureMap['W'] = Steins::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128,128 });
	s_TetrisTileMap['W'] = m_Wall;
	s_TetrisTileMap['E'] = m_Empty;
	s_TetrisTileMap['X'] = m_Test;
	s_TetrisTileMap['I'] = m_IBlock;
	s_TetrisTileMap['S'] = m_SBlock;
	s_TetrisTileMap['O'] = m_OBlock;
	s_TetrisTileMap['T'] = m_TBlock;
	s_TetrisTileMap['Z'] = m_ZBlock;
	s_TetrisTileMap['L'] = m_LBlock;
	s_TetrisTileMap['J'] = m_JBlock;

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f);
}
void Sandbox2D::OnDetach()
{
	STS_PROFILE_FUNCTION();

}


void Sandbox2D::OnUpdate(Steins::Timestep dt)
{
	STS_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	Steins::Renderer2D::ResetStats();

	{
		STS_PROFILE_SCOPE("Renderer Prep");
		Steins::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Steins::RenderCommand::Clear();
	}

#if 0
	{
		static float rotation = 0.0f;
		rotation += dt * 50.0f;

		STS_PROFILE_SCOPE("Renderer Draw");

		//Render Order is Important For Alpha blending
		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Steins::Renderer2D::DrawQuad({ -1.0f,0.0f, -.05f }, { .8f,.8f }, m_SquareColor);
		Steins::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f, -.1f }, { .8f,.8f }, glm::radians(-4.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.5f,-0.5f, -.1f }, { .5f,0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Steins::Renderer2D::DrawQuad({ 0.0f, 0.0f, -.2f }, { 20.0f,20.0f }, m_CheckerboardTexture, 10.0f);
		Steins::Renderer2D::DrawRotatedQuad({ -2.0f,-0.0f, -.1f }, { 1.0f,1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
		Steins::Renderer2D::DrawQuad({ -3.0f,-0.0f, -.1f }, { 1.0f,1.0f }, s_TextureMap['W']);
		Steins::Renderer2D::EndScene();

		Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Steins::Renderer2D::DrawQuad({ x, y , -.1f }, { 0.45f, 0.45f }, color);
			}
		}
		Steins::Renderer2D::EndScene();

	}
#endif

	if (Steins::Input::IsMouseButtonPressed(Steins::Mouse::ButtonLeft))
	{
		auto [x, y] = Steins::Input::GetMousePosition();
		auto width = Steins::Application::Get().GetWindow().GetWidth();
		auto height = Steins::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(dt);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	////////////////////////////////////////////////////////////// Tetris ///////////////////////////////////////////////////

	// Timing =======================
	Timer += dt;
	//STS_CORE_INFO("{0}", Timer);
	if (Timer > 0.05f)
	{
		Timer = 0;
		nSpeedCount++;
	}
	bForceDown = (nSpeedCount == nSpeed);

	// Game Logic ===================

	//// Handle player movement
	if (Steins::Input::IsKeyPressed(Steins::Key::Right))
	{
		nCurrentX += (bMoveHold && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		bMoveHold = false;

	}
	else if (Steins::Input::IsKeyPressed(Steins::Key::Left))
	{
		nCurrentX -= (bMoveHold && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		bMoveHold = false;
	}
	else if (Steins::Input::IsKeyPressed(Steins::Key::Down))
	{
		nCurrentY -= DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY - 1) ? 1 : 0;
		bMoveHold = false;
	}
	else
	{
		bMoveHold = true;

	}

	if (Steins::Input::IsKeyPressed(Steins::Key::Z))
	{
		nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
		bRotateHold = false;
	}
	else
	{
		bRotateHold = true;
	}

	// Force the piece down the playfield if it's time
	if (bForceDown)
	{
		// Update difficulty every 50 pieces
		nSpeedCount = 0;
		nPieceCount++;
		if (nPieceCount % 50 == 0)
			if (nSpeed >= 10) nSpeed--;
		// Test if piece can be moved down
		if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY - 1))
			nCurrentY--; // It can, so do it!
		else
		{
			// It can't! Lock the piece in place
			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
					if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != '.')
						s_Field[(nCurrentY - py) * s_FieldWidth + (nCurrentX + px)] = tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)];

			// Check for lines
			for (int py = 0; py < 4; py++)
				if (nCurrentY - py > 0)
				{
					bool bLine = true;
					for (int px = 1; px < s_FieldWidth - 1; px++)
						bLine &= (s_Field[(nCurrentY - py) * s_FieldWidth + px]) != 'E';

					if (bLine)
					{
						// Remove Line, set to =
						for (int px = 1; px < s_FieldWidth - 1; px++)
							s_Field[(nCurrentY - py) * s_FieldWidth + px] = 'E';
						vLines.push_back(nCurrentY - py);
					}
				}

			nScore += 25;
			if (!vLines.empty())	nScore += (1 << vLines.size()) * 100;

			// Pick New Piece
			nCurrentX = s_FieldWidth / 2;
			nCurrentY = s_FieldHeight;
			nCurrentRotation = 0;
			nCurrentPiece = rand() % 7;

			// If piece does not fit straight away, game over!
			bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
		}
	}

	//// Display ======================

	//// Draw Field
	//for (int x = 0; x < nFieldWidth; x++)
	//	for (int y = 0; y < nFieldHeight; y++)
	//		screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];

	//// Draw Current Piece
	//for (int px = 0; px < 4; px++)
	//	for (int py = 0; py < 4; py++)
	//		if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
	//			screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

	//// Animate Line Completion
	//if (!vLines.empty())
	//{
	//	// Display Frame (cheekily to draw lines)
	//	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	//	this_thread::sleep_for(400ms); // Delay a bit

	for (auto& v : vLines)
		for (int px = 1; px < s_FieldWidth - 1; px++)
		{
			for (int py = v; py < v+4; py++)
				s_Field[py * s_FieldWidth + px] = s_Field[(py + 1) * s_FieldWidth + px];
			//s_Field[px] = 'E';
		}

	vLines.clear();
	//}


	Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (u32 y = 0; y < s_FieldHeight; y++)
	{
		for (u32 x = 0; x < s_FieldWidth; x++)
		{
			char tileType = s_Field[x + y * s_FieldWidth];
			Steins::Ref<Steins::SubTexture2D> texture;
			if (s_TetrisTileMap.find(tileType) != s_TetrisTileMap.end())
			{
				texture = s_TetrisTileMap[tileType];
			}
			else
			{
				texture = m_TextureTree;
			}

			Steins::Renderer2D::DrawQuad({ x - s_FieldWidth / 2.0f,  y - s_FieldHeight / 2.0f, -0.5f }, { 1.0f,1.0f }, texture);

		}
	}

	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			Steins::Ref<Steins::SubTexture2D> texture;
			if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != '.')
			{
				texture = s_TetrisTileMap[tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)]];
			}
			else
			{
				texture = s_TetrisTileMap['E'];
			}
			Steins::Renderer2D::DrawQuad({ nCurrentX + px - s_FieldWidth / 2.0f, nCurrentY - py - s_FieldHeight / 2.0f, 0.0f }, { 1.0f,1.0f }, texture);
		}
	}

	Steins::Renderer2D::EndScene();

#if 0
	Steins::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (u32 y = 0; y < m_MapHeight; y++)
	{
		for (u32 x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Steins::Ref<Steins::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
			{
				texture = s_TextureMap[tileType];
			}
			else
			{
				texture = m_TextureBarrel;
			}
			Steins::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,  y - m_MapHeight / 2.0f, 0.0f }, { 1.0f,1.0f }, texture);

			}
		}
	//Steins::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, m_TextureStairs);
	//Steins::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, m_TextureBarrel);
	//Steins::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f,2.0f }, m_TextureTree);
	Steins::Renderer2D::EndScene();
#endif
	}

void Sandbox2D::OnImGuiRender()
{
	STS_PROFILE_FUNCTION();

	{
		ImGui::Begin("Settings");

		auto stats = Steins::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		auto textureID = m_CheckerboardTexture->GetSRV();
		ImGui::Image((void*)m_CheckerboardTexture->GetRendererID(), ImVec2{ 256.0f, 256.0f });
		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Steins::Event& e)
{
	m_CameraController.OnEvent(e);
}
