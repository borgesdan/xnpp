// LocalProject.cpp : Defines the entry point for the application.
//

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Media.hpp>
#include <Xna/Audio.hpp>
#include <Xna/Input.hpp>
#include <Xna/CSharp/TimeSpan.hpp>


using namespace Xna;

class Game1 : public Xna::Game {
	using base = Xna::Game;
public:
	Game1() : Xna::Game() {
		graphics = Xna::GraphicsDeviceManager(this);
		Content().RootDirectory("Content");
	}

	void Initialize() override {
		base::Initialize();
	}

	void LoadContent() override {
		spriteBatch = Xna::SpriteBatch(GraphicsDevice());
		base::LoadContent();
	}

	void Update(Xna::GameTime& gameTime) override {
		base::Update(gameTime);
	}

	void Draw(Xna::GameTime& gameTime) override {
		graphics.GraphicsDevice()->Clear(Colors::CornflowerBlue);
		base::Draw(gameTime);
	}

private:
	Xna::GraphicsDeviceManager graphics = nullptr;
	Xna::SpriteBatch spriteBatch = nullptr;
};


int main()
{
	auto game1 = Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
