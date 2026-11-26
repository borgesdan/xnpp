// LocalProject.cpp : Defines the entry point for the application.
//

#include <Xna/Framework.hpp>
#include <crtdbg.h>

using namespace Xna;

class Game1 : public Xna::Game {
	using base = Xna::Game;
public:
	Game1() : Xna::Game() {
		graphics = Xna::GraphicsDeviceManager(this);
		Content()->RootDirectory("Content");
	}

	void OnStartRun() override {
		graphics.Initialize();		

		base::OnStartRun();
	}

	void Initialize() override {
		graphics.PreferredBackBufferWidth(1280);
		graphics.PreferredBackBufferHeight(720);
		graphics.ApplyChanges(); //TODO: Implementar método Reset do bgfx

		base::Initialize();
	}

	void LoadContent() override {
		spriteBatch = Xna::SpriteBatch(GraphicsDevice());

		blocks = Content()->Load<Texture2D>("blocks");

		base::LoadContent();
	}

	void Update(Xna::GameTime& gameTime) override {
		base::Update(gameTime);
	}

	void Draw(Xna::GameTime& gameTime) override {
		graphics.GraphicsDevice()->Clear(Colors::CornflowerBlue);

		spriteBatch.Begin();
		spriteBatch.Draw(blocks, Vector2::Zero(), Color::White());
		spriteBatch.End();

		base::Draw(gameTime);
	}

private:
	Xna::SpriteBatch spriteBatch = nullptr;
	Xna::GraphicsDeviceManager graphics = nullptr;

	Texture2D blocks{nullptr};
};


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);

	auto game1 = Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
