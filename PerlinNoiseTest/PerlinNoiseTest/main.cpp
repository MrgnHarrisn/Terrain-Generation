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
		float nx = random(clip(heights[i - 1].x - epsilon, 0.7, 0.8), clip(heights[i - 1].x + epsilon, 0.7, 0.8));
		float ny = random(clip(heights[i - 1].y - epsilon, 0.7, 0.8), clip(heights[i - 1].y + epsilon, 0.7, 0.8));
		Vector2f nh(nx, ny);
		heights.push_back(nh);
	}

	return interpolate(heights, 600);

}

int main()
{

	int WIDTH = 800, HEIGHT = 800;

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Perlin Noise");
	window.setFramerateLimit(2);

	Image image;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}

		image.create(WIDTH, HEIGHT, sf::Color::White);

		std::vector<float> heights = generate_heights(WIDTH, 0.004);
		for (int i = 0; i < WIDTH; i++) {
			int dirt_height = static_cast<int>(heights[i]);

			// Clip dirt_height to stay within the valid range of the image
			dirt_height = std::max(0, std::min(dirt_height, HEIGHT - 1));

			// Set the dirt color pixel at (i, dirt_height)
			image.setPixel(i, dirt_height, Color(150, 75, 0));

			// Set other pixels above dirt_height
			for (int j = 799; j > dirt_height; j--) {
				image.setPixel(i, j, sf::Color(100, 100, 100));
			}
			for (int j = 799; j > heights[i]; j--)
			{
				image.setPixel(i, j, sf::Color(100, 100, 100));
			}
		}

		sf::Texture texture;
		texture.loadFromImage(image);
		sf::Sprite sprite(texture);

		window.clear();
		window.draw(sprite);
		window.display();

	}

}