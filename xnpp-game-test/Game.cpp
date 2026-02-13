// LocalProject.cpp : Defines the entry point for the application.
//

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Media.hpp>
#include <Xna/Audio.hpp>
#include <Xna/Input.hpp>

using namespace std;
using namespace Xna;
using namespace Xna::CSharp;

class Game1 : public Game {
	using base = Game;
public:
	Game1() : Game() {
		graphics = Xna::GraphicsDeviceManager(*this);
		Content().RootDirectory("Content");
	}

	void Initialize() override {
		base::Initialize();
	}

	void LoadContent() override {
		spriteBatch = SpriteBatch(GraphicsDevice());

		base::LoadContent();
	}

	void Update(Xna::GameTime& gameTime) override {

		base::Update(gameTime);
	}

	void Draw(Xna::GameTime& gameTime) override {
		graphics.GraphicsDevice()->Clear(Xna::Color::CornflowerBlue());

		base::Draw(gameTime);
	}

private:
	GraphicsDeviceManager graphics = nullptr;
	SpriteBatch spriteBatch = nullptr;

};


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	auto game1 = Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
