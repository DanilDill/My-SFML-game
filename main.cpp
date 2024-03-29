#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h" //���������� ��� � ������
#include "view.h"//���������� ��� � ����� ������
#include <sstream>
#include "quests.h"
#include <math.h>

using namespace sf;
////////////////////////////////////////////////////����� ������////////////////////////
class Player { // ����� ������
public:

	float x,y, w, h, dx, dy, speed; //���������� ������ � � �, ������ ������, ��������� (�� � � �� �), ���� ��������
	int dir,PlayerScore,health;
	bool life, isMove, isSelect;//����������� (direction) �������� ������
	String File; //���� � �����������
	Image image;//���� �����������
	Texture texture;//���� ��������
	Sprite sprite;//���� ������
	
	Player(String F, float X, float Y, float W, float H) { //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		dx = 0; dy = 0; speed = 0; dir = 0;
		PlayerScore = 0; life = true;
		health = 100; isMove = false; isSelect = false;
		File = F;//��� �����+����������
		w = W; h = H;//������ � ������
		
		image.loadFromFile("images/" + File);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������. � ����� ������ "hero.png" � ��������� ������ ���������� image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(41, 33, 59));//������� �������� �����-����� ����, ��� ���� ��� ���������� �� ��������.
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h)); //������ ������� ���� ������������� ��� ������ ������ ����, � �� ���� ����� �����. IntRect - ���������� �����

		sprite.setOrigin(w / 2, h / 2);
	}
	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	
	{
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed; dy = 0; break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed; dy = 0; break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dx = 0; dy = speed; break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dx = 0; dy = -speed; break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}

		x += dx*time;//�� �������� �� �������� �����. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		y += dy*time;//���������� �� ������

		speed = 0;//�������� ��������, ����� �������� �����������.
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();
	}
	void interactionWithMap() {
		for (int i = y/32;i<(y+h)/32;i++)
			for (int j = x / 32; j < (x + w) / 32; j++) {
				if (TileMap[i][j] == '0') {
					if (dy > 0)
					{
						y = i * 32 - h;
					}
					if (dy < 0)
					{
						y = i * 32 + 32;
					}
					if (dx > 0)
					{
						x = j * 32 - w;
					}
					if (dx < 0) {
						x = j * 32 + 32;
					}


				}
				if (TileMap[i][j] == 's') {
					PlayerScore++;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'd') {
					health = health - 20;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'h') {
					health = health + 20;
					if (health > 100) health =100;
					TileMap[i][j] = ' ';
			
			
				}
				if (health <= 0)  life = false;
			}
				
				
			
	}
		float GetPlayerCoordinateX() {
			
		return x;//���������� ���������� �
	}
	float GetPlayerCoordinateY() {
		return y;// ���������� ���������� �
	}
};
int main()
{
	getMapRandomGenerate();//��������� ��������� ��������
	float tempX = 0;//��������� ����� �.������� �� ����� ������� ���� ���� ����
	float tempY = 0;//����� Y
 int distance = 0;//��� ���������� �� ������� �� ���� �������


	Font font;
	font.loadFromFile("CyrilicOld.ttf");


	Text text("",font,15);
	Text text_g_o("", font, 30);

	text_g_o.setFillColor(Color::Red);
	text.setFillColor(Color::Black);

	text_g_o.setStyle(Text::Bold | Text::Underlined);
	
	///////////////////////////////// �������� ��������� ����� ////////////////////////////////
	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/map.png");//��������� ���� ��� �����
	Texture map;//�������� �����
	map.loadFromImage(map_image);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������
	////////////////////////////////////////////////////////

	////////////////////////////////////// �������� ������///////////////////////////////////////////////////
	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //���������� �����, ������� �������� ��������
	s_quest.setScale(0.6f, 0.6f);//���� ��������� ��������, => ������ ���� ������
	///////////////////////////////////////////////////

	RenderWindow window(sf::VideoMode(1366, 768), "my First game");

	Player p("hero.png", 250, 250, 96.0, 96.0);//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
	bool showMissionText = true;
	//���������� ������ �� �������?
	float dX = 0;// ������������� ������� �� �
	float dY = 0;//�� �




	float CurrentFrame = 0;//������ ������� ����
	float currentFrame = 0;
	Clock clock;
	//Clock gameTimeClock;//���������� �������� �������, ����� ����� ������� ����� ���� (��� ��������� �������)
	//int gameTime = 0;//�������������� ���������� ����

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� ������
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		//if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//���� ��� �����,���� � ����� (��� ��������� �������)

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed)//���� ������ ������� ����
				if ( sf::Mouse::Left)//� ������ �����
					if (p.sprite.getGlobalBounds().contains(pos.x, pos.y))//� ��� ���� ���������� ������� �������� � ������
					{
						
						p.sprite.setColor(Color::Green);
						p.isSelect = true;
						tempX = pos.x;
						tempY = pos.y;
 

					}
			if (p.isSelect) {
				if (event.type == Event::MouseButtonPressed)
					if ( sf::Mouse::Right) {
						p.isMove = true;
						p.isSelect = false;
						p.sprite.setColor(Color::White);
						tempX = pos.x;
						tempY = pos.y;
                	

					}
			}
			

			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape) {
					window.close();
				}

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Tab) {
					switch (showMissionText) {
					case true:{
						std::ostringstream playerHealthString;//������ �������� ������
						playerHealthString << p.health; //������� � ������ �������� 
						std::ostringstream task;//������ ������ ������
						task << getTextMision(getCurrentMission(p.health));
						text.setString("��������: " + playerHealthString.str() + "\n" + task.str());
						showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
						break;//������� , ����� �� ��������� ������� "false" (������� ����)
						}
					case false:{
						text.setString("");
						showMissionText = true;
						break;

							  }
					}
					
				}

			
			
		}

if (p.isMove) {
				distance = sqrt((tempX - p.x)*(tempX - p.x) + (tempY - p.y)*(tempY - p.y));//������� ��������� (���������� �� ����� � �� ����� �). ��������� ������� ����� �������

			if (distance >30) {//���� �������� ������� �������� �� ����� �������� ������� �������
			
					p.x += 0.1*time*(tempX - p.x) / distance;//���� �� ���� � ������� ������� �������
					p.y += 0.1*time*(tempY - p.y) / distance;//���� �� ������ ��� ��
		}
				else { p.isMove = false; }//������� ��� ��� ������ �� ���� � ������� ������� ��������� � �������
		//		std::cout << distance; 
 			}
                  //float dX = pos.x - p.x;//������ , ����������� ������, ������� ���������� ������ � ������
                //  float dY = pos.y - p.y;//�� ��, ���������� y
                //  float rotation = (atan2(dY, dX)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
                //  std::cout << rotation << "\n";//������� �� ������� � ���������
               //   p.sprite.setRotation(rotation);//������������ ������ �� ��� �������	

	

		///////////////////////////////////////////���������� ���������� � ���������////////////////////////////////////////////////////////////////////////
	     if(p.life){
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			p.dir = 1; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
			//getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//�������� ���������� ������ � ������� ���������� �������s
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
			getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//�������� ���������� ������ � ������� ���������� �������
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
			getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//�������� ���������� ������ � ������� ���������� �������

		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			p.dir = 2; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
			getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//�������� ���������� ������ � ������� ���������� �������
		}
		p.update(time);
	//	viewmap(time);//������� ���������� �����, �������� �� ����� sfml
	   changeview();//������������� � ������� ����
		window.setView(view);//"��������" ������ � ���� sfml
			window.clear();}
		 /////////////////////////////////////////��������� �����///////////////////////////////////////////////////////////////////////////////////////////
			sf::Vector2i localPosition = Mouse::getPosition(window);//������� � ������ ���������� ���� ������������ ���� (�,�)
			if (localPosition.x < 3) { view.move(-0.2*time, 0); }//���� ������ �������� � ����� ���� ������,�� ������� ������ �����
			if (localPosition.x > window.getSize().x - 3) { view.move(0.2*time, 0); }//������ ����-������
			if (localPosition.y > window.getSize().y - 3) { view.move(0, 0.2*time); }//������ ���� - ����
			if (localPosition.y < 3) { view.move(0, -0.2*time); }//������� ���� - �����
		 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////������ �����/////////////////////
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
				{
					if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ ������, �� ������ 1� ���������
					if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� ������ s, �� ������ 2� ���������
					if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������
					if (TileMap[i][j] == 'd') s_map.setTextureRect(IntRect(96, 0, 32, 32));//��������� ��������� ��������
					if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));//��������� �������
					s_map.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

					window.draw(s_map);//������ ���������� �� �����
				}
		
			
			
	      
			//////////////////////////������ ���������//////////////////////////
			if (p.health <= 0) {
				text_g_o.setPosition(view.getCenter().x, view.getCenter().y);
				text_g_o.setString("GAME OVER");
				text.setCharacterSize(40);
			
				

			}
			//////////////////////////////////////////////////////////////////////////////////////
			if (!showMissionText) {
				text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//������� ����� ����� ���������� �����
				s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//������� ���� ��� �����			
				window.draw(s_quest); window.draw(text); //������ ������ ������ (��� ��� ������ ������). � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���
			}
			window.draw(text_g_o);
			window.draw(p.sprite);
            
			window.display();
		}

		return 0;
	}

