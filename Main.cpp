
# include <Siv3D.hpp>

struct Firework : IEffect
{
	struct Particle
	{
		Vec2 pos, v0;
	};

	Array<Particle> m_particles;

	Firework(const Vec2& pos, int count)
		: m_particles(count)
	{
		for (auto& particle : m_particles)
		{
			const Vec2 v = Circular(Random(10.0, 80.0), Random(TwoPi));
			particle.pos = pos + v;
			particle.v0 = v * 2.0;
		}
	}

	bool update(double t) override
	{
		for (const auto& particle : m_particles)
		{
			const Vec2 pos = particle.pos + particle.v0 * t + 0.5* t*t * Vec2(0, 320);

			Circle(pos, 1.5).draw(HSV(pos.y / 4.0, 0.6, 1.0).toColorF(1.0 - t));
		}

		return t < 1.0;
	}
};

void Main()
{
	Window::SetTitle(L"10'00");

	Stopwatch stopwatch;

	Effect effect;

	const Font font(50);

	const Vec2 pos = font(L"00'00").regionCenter(Window::Center()).pos;

	while (System::Update())
	{
		if ((!stopwatch.isActive() || stopwatch.isPaused()) && Input::MouseL.clicked)
		{
			Graphics::SetVSyncEnabled(false);
			stopwatch.start();
		}
		else if (stopwatch.isActive() && Input::MouseL.clicked)
		{
			Graphics::SetVSyncEnabled(false);
			stopwatch.pause();
		}
		else if (Input::MouseR.clicked)
		{
			stopwatch.reset();
		}
		else if (stopwatch.isPaused() && stopwatch.ms() / 10 == 10'00)
		{
			Graphics::SetVSyncEnabled(true);
			effect.add<Firework>(Circular(200, Random(TwoPi)) + Window::Center(), 30);
		}

		font(L"{:0>2}'{:0>2}"_fmt, stopwatch.s(), stopwatch.ms() % 1000 / 10).draw(pos);

		if (effect.hasEffects())
		{
			Graphics2D::SetBlendState(BlendState::Additive);

			effect.update();

			Graphics2D::SetBlendState(BlendState::Default);
		}
		else
		{
			System::Sleep(4ms);
		}
	}
}
