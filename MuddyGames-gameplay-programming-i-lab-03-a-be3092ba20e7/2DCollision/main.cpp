#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <Tinyc2Debug.hpp>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a NPC's sprites to display
	sf::Texture npc_texture;
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	Tinyc2Debug tinyD(window);
	// Load a mouse texture to display
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	//Text Setup
	sf::Font controlsFont;
	if (!controlsFont.loadFromFile("assets\\ariblk.ttf"))
	{
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	sf::Text controls;
	controls.setFont(controlsFont);
	controls.setCharacterSize(18);
	controls.setFillColor(sf::Color::White);
	controls.setOutlineThickness(1);
	controls.setOutlineColor(sf::Color::Red);
	controls.setString("WASD to move Circle \nUp/Down/Left/Right for ray \nMouse for AABB");
	controls.setPosition(530, 500);
	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players Default Animated Sprite
	AnimatedSprite player_animated_sprite(player_texture);
	player_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	//setup bounding rectangle
	sf::RectangleShape boundingBox;
	boundingBox.setFillColor(sf::Color::Transparent);
	boundingBox.setOutlineThickness(3);
	boundingBox.setOutlineColor(sf::Color::Green);
	

	//AABB TO RAY LIST
	c2Ray ray;
	ray.p = c2V(5, 5);
	ray.d = c2Norm(c2V(2, 1));
	ray.t = 200;

	// AABB TO POLY
	c2Poly poly;
	poly.count = 4;
	poly.verts[0] = c2V(180, 30);
	poly.verts[1] = c2V(310, 70);
	poly.verts[2] = c2V(280, 90);
	poly.verts[3] = c2V(190, 100);
	c2MakePoly(&poly);


	// AABB TO CAPSULE
	c2Capsule capsule;
	capsule.a = c2V(350, 50);
	capsule.b = c2V(400, 50);
	capsule.r = 20;

	// Create circle
	c2Circle circle;
	circle.p = c2V(200, 500);
	circle.r = 30;

	// Create circle
	c2Circle circleStationary;
	circleStationary.p = c2V(480, 70);
	circleStationary.r = 30;

	//Movable Ray
	c2Ray rayMovable;
	rayMovable.p = c2V(500, 500);
	rayMovable.d = c2Norm(c2V(1, 3));
	rayMovable.t = 50;

	// Setup the NPC
	GameObject &npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject &player = Player(player_animated_sprite);

	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width, 
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);


	// Initialize Input
	Input input;

	// Collision result
	int result = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.1f, 0.2f);
	
	// Start the game loop
	sf::Clock clock;
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		
		// Move The NPC
		sf::Vector2f move_to(npc.getAnimatedSprite().getPosition().x + direction.x, npc.getAnimatedSprite().getPosition().y + direction.y);

		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0 + npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0 + npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - npc.getAnimatedSprite().getGlobalBounds().height;
		}
		
		npc.getAnimatedSprite().setPosition(move_to);

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Update Player AABB
		aabb_player.min = c2V(
			player.getAnimatedSprite().getPosition().x, 
			player.getAnimatedSprite().getPosition().y
		);
		aabb_player.max = c2V(
			player.getAnimatedSprite().getPosition().x +
			player.getAnimatedSprite().getGlobalBounds().width, 
			player.getAnimatedSprite().getPosition().y + 
			player.getAnimatedSprite().getGlobalBounds().height
		);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			circle.p.y -= 5000 * clock.restart().asSeconds();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			circle.p.y += 5000 * clock.restart().asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			circle.p.x -= 5000 * clock.restart().asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			circle.p.x += 5000 * clock.restart().asSeconds();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			rayMovable.p.y -= 5000 * clock.restart().asSeconds();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			rayMovable.p.y += 5000 * clock.restart().asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rayMovable.p.x -= 5000 * clock.restart().asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rayMovable.p.x += 5000 * clock.restart().asSeconds();
		}
		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Update the Player
		npc.update();

		// Check for collisions
		result = c2AABBtoAABB(aabb_player, aabb_npc);
		std::cout << ((result != 0) ? ("Collision") : "") << endl;
		if (result){
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
			boundingBox.setOutlineColor(sf::Color::Red);
		}
		else {
			
			
		}
		boundingBox.setSize(sf::Vector2f(player.getAnimatedSprite().getGlobalBounds().height, player.getAnimatedSprite().getGlobalBounds().width));
		boundingBox.setPosition(player.getAnimatedSprite().getPosition().x-1,
			player.getAnimatedSprite().getPosition().y-1);
		std::cout << boundingBox.getPosition().x;


		// Raycast
		c2Raycast cast;
		int hit = c2RaytoAABB(ray, aabb_player, &cast);

		//for aabb to poly
		c2Manifold manifoldPoly;
		c2AABBtoPolyManifold(aabb_player, &poly, NULL, &manifoldPoly);
		//for aabb to capsule
		c2Manifold manifoldCapsule;
		c2AABBtoCapsuleManifold(aabb_player, capsule, &manifoldCapsule);

		// Raycast
		//CIRCLE TO RAY
		c2Raycast castCircleToRay;
		int hitCircleToRay = c2RaytoCircle(ray, circle, &cast);

		//CIRCLE TO POLYGON
		c2Manifold manifoldCircleToPoly;
		c2CircletoPolyManifold(circle, &poly, NULL, &manifoldCircleToPoly);

		//Circle To Circle
		// Check for collision
		c2Manifold manifoldCircleToCircle;
		c2CircletoCircleManifold(circle, circleStationary, &manifoldCircleToCircle);
		//Circle To Capsule
		c2Manifold manifoldCircleToCapsule;
		c2CircletoCapsuleManifold(circle, capsule, &manifoldCircleToCapsule);

		//Cicle To AABB
		c2Manifold manifoldCircleToAABB;
		c2CircletoAABBManifold(circleStationary, aabb_player, &manifoldCircleToAABB);

		//Ray To Poly
		rayMovable.d = c2Norm(rayMovable.d);
		c2Raycast castRayToPoly;
		int hitRayToPoly = c2RaytoPoly(rayMovable, &poly, NULL, &castRayToPoly);

		//Ray to circle
		// Raycast
		c2Raycast castRayToCircle;

		int hitRayToCircle = c2RaytoCircle(rayMovable, circleStationary, &castRayToCircle);


		//Ray To Capsule
		// Raycast
		c2Raycast castRayToCapsule;
		int hitRayToCapsule = c2RaytoCapsule(rayMovable, capsule, &castRayToCapsule);

		//Ray to AABB
		// Raycast
		c2Raycast castRayToAABB;
		int hitRayToAABB = c2RaytoAABB(rayMovable, aabb_player, &castRayToAABB);


		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		
		// Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());
		//aabb to poly
		if (manifoldPoly.count > 0) {
			tinyD.draw(manifoldPoly);
			tinyD.draw(poly, sf::Color::Red);
			player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			boundingBox.setOutlineColor(sf::Color::Red);
		}
		else
		{
			tinyD.draw(poly, sf::Color::Green);
		}
		//aabb to ray
		if (hit)
		{
			tinyD.draw(cast, ray);
			tinyD.draw(ray, sf::Color::Red);
			player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			boundingBox.setOutlineColor(sf::Color::Red);
		}
		else 
		{
			tinyD.draw(ray, sf::Color::Green);
			
		}

		if (manifoldCapsule.count > 0) {
			tinyD.draw(manifoldCapsule);
			tinyD.draw(capsule, sf::Color::Red);
			player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			boundingBox.setOutlineColor(sf::Color::Red);
		}
		else
		{
			tinyD.draw(capsule, sf::Color::Green);
		}
		if (hitCircleToRay) {
			tinyD.draw(castCircleToRay, ray);
			tinyD.draw(ray, sf::Color::Red);
			tinyD.draw(circle, sf::Color::Red);
		}
		else
		{
			//tinyD.draw(ray, sf::Color::Green);
			tinyD.draw(circle, sf::Color::Green);
		}
		if (manifoldCircleToPoly.count > 0) {
			tinyD.draw(manifoldCircleToPoly);
			tinyD.draw(poly, sf::Color::Red);
			tinyD.draw(circle, sf::Color::Red);
		}
	
		if (manifoldCircleToCircle.count > 0) {
			tinyD.draw(manifoldCircleToCircle);
			tinyD.draw(circleStationary, sf::Color::Red);
			tinyD.draw(circle, sf::Color::Red);
		}
		else
		{
			tinyD.draw(circleStationary, sf::Color::Green);
		}
		if (manifoldCircleToCapsule.count > 0) {
			tinyD.draw(manifoldCircleToCapsule);
			tinyD.draw(capsule, sf::Color::Red);
			tinyD.draw(circle, sf::Color::Red);
		}
		
		if (manifoldCircleToAABB.count > 0) {
			tinyD.draw(manifoldCircleToAABB);
			tinyD.draw(aabb_player, sf::Color::Red);
			player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			boundingBox.setOutlineColor(sf::Color::Red);
			tinyD.draw(circleStationary, sf::Color::Red);
		}
		
		if (hitRayToPoly)
		{
			tinyD.draw(castRayToPoly, rayMovable);
			tinyD.draw(rayMovable, sf::Color::Red);
			tinyD.draw(poly, sf::Color::Red);

		}
		else
		{
			tinyD.draw(rayMovable, sf::Color::Green);

		}

		if (hitRayToCircle)
		{
			tinyD.draw(castRayToPoly, rayMovable);
			tinyD.draw(rayMovable, sf::Color::Red);
			tinyD.draw(circleStationary, sf::Color::Red);

		}
		

		if (hitRayToCapsule)
		{
			tinyD.draw(castRayToPoly, rayMovable);
			tinyD.draw(rayMovable, sf::Color::Red);
			tinyD.draw(capsule , sf::Color::Red);

		}
	

		if (hitRayToAABB)
		{
			tinyD.draw(castRayToAABB, rayMovable);
			tinyD.draw(rayMovable, sf::Color::Red);
			player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			boundingBox.setOutlineColor(sf::Color::Red);
		}
		
		c2CircletoAABBManifold(circle, aabb_player, &manifoldCircleToAABB);
		hitRayToCircle = c2RaytoCircle(rayMovable, circle, &castRayToCircle);

		if (hitRayToCircle)
		{
			tinyD.draw(castRayToPoly, rayMovable);
			tinyD.draw(rayMovable, sf::Color::Red);
			tinyD.draw(circle, sf::Color::Red);

		}

		if (manifoldCircleToAABB.count > 0) {
			tinyD.draw(manifoldCircleToAABB);
			tinyD.draw(aabb_player, sf::Color::Red);
			player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			boundingBox.setOutlineColor(sf::Color::Red);
			tinyD.draw(circle, sf::Color::Red);
		}


		// Update the window
		window.draw(player.getAnimatedSprite());
		window.draw(boundingBox);
		//tinyc2Debug.draw(aabb, sf::Color::Green);
		window.draw(controls);
		window.display();
		boundingBox.setOutlineColor(sf::Color::Green);
		player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		clock.restart();
	}

	return EXIT_SUCCESS;
};