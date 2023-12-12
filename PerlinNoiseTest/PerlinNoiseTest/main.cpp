#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>

using namespace sf;

float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

float random(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

float dot_product_2(Vector2f a, Vector2f b)
{
	return (a.x * b.x + a.y * b.y);
}

float dot_product_3(Vector2f a, Vector2f b, Vector2f c)
{
	return a.x * b.x * c.x + a.y * b.y * c.y;
}

/* Kill me now */
std::vector<float> interpolate(std::vector<Vector2f> heights, int max_height)
{
	std::vector<float> new_heights;
	for (int i = 0; i < heights.size(); i++) {
		if (i > 0 && i < heights.size() - 1) {
			float sum = dot_product_3(heights[i], heights[i - 1], heights[i + 1]);
			new_heights.push_back(static_cast<int>(sum * max_height));
		}
		else {
			if (i == 0) {
				float sum = dot_product_2(heights[i], heights[i + 1]);
				new_heights.push_back(static_cast<int>(sum * max_height));
			}
			else {
				float sum = dot_product_2(heights[i], heights[i - 1]);
				new_heights.push_back(static_cast<int>(sum * max_height));
			}
		}
	}
	return new_heights;
}

// DEFAULT HEIGHT OF MAP IS 256
// WIDTH CAN BE WHATEVER
std::vector<float> generate_heights(int map_width, float epsilon)
{

	Vector2f first(random(0.0, 1.0), random(0.0, 1.0));

	std::vector<Vector2f> heights;
	heights.push_back(first);
	for (int i = 1; i < map_width; i++) {
		float nx = random(clip(heights[i - 1].x - epsilon, 0.0, 1.0), clip(heights[i - 1].x + epsilon, 0.0, 1.0));
		float ny = random(clip(heights[i - 1].y - epsilon, 0.0, 1.0), clip(heights[i - 1].y + epsilon, 0.0, 1.0));
		Vector2f nh(nx, ny);
		heights.push_back(nh);
	}

	return interpolate(heights, 400);

}

int main()
{

	int WIDTH = 1920, HEIGHT = 800;

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Perlin Noise");

	Image image;
	image.create(WIDTH, HEIGHT, sf::Color::White);

	std::vector<float> heights = generate_heights(WIDTH, 0.004);
	for (int i = 0; i < WIDTH; i++) {
		image.setPixel(i, heights[i], sf::Color::Black);
	}

	sf::Texture texture;
	texture.loadFromImage(image);

	sf::Sprite sprite(texture);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();

	}

}