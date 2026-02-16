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
		oldKState = curKState;
		curKState = Xna::Keyboard::GetState();

		if (curKState.IsKeyDown(Xna::Keys::Right) && oldKState.IsKeyUp(Xna::Keys::Right)) {
			backColor = Xna::Color::Red();
		}

		if (curKState.IsKeyDown(Xna::Keys::A) && oldKState.IsKeyUp(Xna::Keys::A)) {
			backColor = Xna::Color::Green();
		}

		if (curKState.IsKeyDown(Xna::Keys::Left) && oldKState.IsKeyUp(Xna::Keys::Left)) {
			backColor = Xna::Color::Blue();
		}

		if (curKState.IsKeyDown(Xna::Keys::Space) && oldKState.IsKeyUp(Xna::Keys::Space)) {
			backColor = Xna::Color::Azure();
		}

		if (curKState.IsKeyDown(Xna::Keys::PageDown) && oldKState.IsKeyUp(Xna::Keys::PageDown)) {
			backColor = Xna::Color::Bisque();
		}

		if (curKState.IsKeyDown(Xna::Keys::D1) && oldKState.IsKeyUp(Xna::Keys::D1)) {
			backColor = Xna::Color::Cyan();
		}

		if (curKState.IsKeyDown(Xna::Keys::NumPad0) && oldKState.IsKeyUp(Xna::Keys::NumPad0)) {
			backColor = Xna::Color::DarkOrange();
		}

		auto mouseState = Xna::Mouse::GetState();

		if (mouseState.LeftButton == Xna::ButtonState::Pressed) {
			backColor = Xna::Color::Black();
		}

		if (mouseState.RightButton == Xna::ButtonState::Pressed) {
			backColor = Xna::Color::Orange();
		}

		if (mouseState.ScroolWheelValue > 50) {
			backColor = Xna::Color::CornflowerBlue();
		}

		base::Update(gameTime);
	}

	void Draw(Xna::GameTime& gameTime) override {
		graphics.GraphicsDevice()->Clear(backColor);

		base::Draw(gameTime);
	}

private:
	Xna::GraphicsDeviceManager graphics = nullptr;
	Xna::SpriteBatch spriteBatch = nullptr;
	Xna::KeyboardState oldKState{};
	Xna::KeyboardState curKState{};
	Xna::Color backColor = Xna::Color::CornflowerBlue();
	bool one = false;
};


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	auto game1 = Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
