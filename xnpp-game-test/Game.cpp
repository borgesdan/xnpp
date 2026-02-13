// LocalProject.cpp : Defines the entry point for the application.
//

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Media.hpp>
#include <Xna/Audio.hpp>
#include <Xna/Input.hpp>


class Game1 : public Xna::Game{
	using base = Xna::Game;
public:
	Game1() : Xna::Game() {
		graphics = Xna::GraphicsDeviceManager(*this);
		Content().RootDirectory("Content");
	}

	void Initialize() override {		
		base::Initialize();
	}

	void LoadContent() override {
		auto device = GraphicsDevice();
		spriteBatch = Xna::SpriteBatch(nullptr);

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
	Xna::GraphicsDeviceManager graphics = nullptr;
	Xna::SpriteBatch spriteBatch = nullptr;
	bool one = false;
};


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	auto game1 = Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
