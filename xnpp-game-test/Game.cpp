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
	Game1() : Xna::Game(Xna::GameRunMode::Bgfx) {
		graphics = Xna::GraphicsDeviceManager(*this);
		Content().RootDirectory("Content");
	}

	void Initialize() override {		
		base::Initialize();
	}

	void LoadContent() override {
		base::LoadContent();

		spriteBatch = Xna::SpriteBatch(GraphicsDevice());
		texture = Content().Load<Xna::Texture2D>("Sprites/Gem");
	}

	void Update(Xna::GameTime& gameTime) override {
		oldKState = curKState;
		curKState = Xna::Keyboard::GetState();		

		if (curKState.IsKeyDown(Xna::Keys::Right) && oldKState.IsKeyUp(Xna::Keys::Right)) {
			backColor = Xna::Color::Red();
			auto soundEffectInstance1 = soundEffect.CreateInstance();			
			soundEffectInstance1.Play();			
		}		

		if (curKState.IsKeyDown(Xna::Keys::A) && oldKState.IsKeyUp(Xna::Keys::A)) {
			backColor = Xna::Color::Green();
			Xna::MediaPlayer::Play(song);			
		}

		if (curKState.IsKeyDown(Xna::Keys::Left) && oldKState.IsKeyUp(Xna::Keys::Left)) {
			backColor = Xna::Color::Blue();

			auto soundEffectInstance2 = soundEffect.CreateInstance();
			soundEffectInstance2.Play();

			//Xna::MediaPlayer::Stop();
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

		oldPState = curPState;
		curPState = Xna::GamePad::GetState(Xna::PlayerIndex::One);

		if (curPState.IsConnected() && curPState.IsButtonDown(Xna::Buttons::X) && oldPState.IsButtonUp(Xna::Buttons::X)) {
			backColor = Xna::Color::Ivory();
			Xna::GamePad::SetVibration(Xna::PlayerIndex::One, 0.2f, 0.2f);
		}

		if (curPState.IsConnected() && curPState.IsButtonDown(Xna::Buttons::B) && oldPState.IsButtonUp(Xna::Buttons::B)) {
			backColor = Xna::Color::Ivory();
			Xna::GamePad::SetVibration(Xna::PlayerIndex::One, 1, 1);
		}

		if (curPState.IsConnected() && curPState.IsButtonDown(Xna::Buttons::Y) && oldPState.IsButtonUp(Xna::Buttons::Y)) {
			backColor = Xna::Color::Ivory();
			Xna::GamePad::SetVibration(Xna::PlayerIndex::One, 0.5f, 0.5f);
		}

		if (curPState.IsConnected() && curPState.IsButtonDown(Xna::Buttons::A) && oldPState.IsButtonUp(Xna::Buttons::A)) {
			backColor = Xna::Color::Ivory();
			Xna::GamePad::SetVibration(Xna::PlayerIndex::One, 0, 0);
		}

		base::Update(gameTime);
	}

	void Draw(Xna::GameTime& gameTime) override {
		//graphics.GraphicsDevice()->Clear(backColor);

		spriteBatch.Begin();
		spriteBatch.Draw(texture, Xna::Vector2(100, 100), Xna::Color::White());
		spriteBatch.End();

		base::Draw(gameTime);
	}

private:
	Xna::GraphicsDeviceManager graphics = nullptr;
	Xna::SpriteBatch spriteBatch = nullptr;
	Xna::KeyboardState oldKState{};
	Xna::KeyboardState curKState{};
	Xna::GamePadState oldPState{};
	Xna::GamePadState curPState{};
	Xna::Color backColor = Xna::Color::CornflowerBlue();
	Xna::SoundEffect soundEffect = nullptr;
	Xna::Song song = nullptr;
	std::shared_ptr<Xna::SoundEffectInstance> soundEffectInstance = nullptr;
	Xna::Texture2D texture = nullptr;
	bool one = false;
};


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	auto game1 = Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
