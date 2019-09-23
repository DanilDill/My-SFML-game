#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h" //подключили код с картой
#include "view.h"//подключили код с видом камеры
#include <sstream>
#include "quests.h"
#include <math.h>

using namespace sf;
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player { // класс Игрока
public:

	float x,y, w, h, dx, dy, speed; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	int dir,PlayerScore,health;
	bool life, isMove, isSelect;//направление (direction) движения игрока
	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт
	
	Player(String F, float X, float Y, float W, float H) { //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		dx = 0; dy = 0; speed = 0; dir = 0;
		PlayerScore = 0; life = true;
		health = 100; isMove = false; isSelect = false;
		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		
		image.loadFromFile("images/" + File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(41, 33, 59));//убираем ненужный темно-синий цвет, эта тень мне показалась не красивой.
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, w, h)); //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов

		sprite.setOrigin(w / 2, h / 2);
	}
	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0; break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0; break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed; break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed; break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx*time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy*time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
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
			
		return x;//Возвращаем координату Х
	}
	float GetPlayerCoordinateY() {
		return y;// Возвращаем координату У
	}
};
int main()
{
	getMapRandomGenerate();//рандомная генерация обьектов
	float tempX = 0;//временная коорд Х.Снимаем ее после нажатия прав клав мыши
	float tempY = 0;//коорд Y
 int distance = 0;//это расстояние от объекта до тыка курсора


	Font font;
	font.loadFromFile("CyrilicOld.ttf");


	Text text("",font,15);
	Text text_g_o("", font, 30);

	text_g_o.setFillColor(Color::Red);
	text.setFillColor(Color::Black);

	text_g_o.setStyle(Text::Bold | Text::Underlined);
	
	///////////////////////////////// загрузка элементов карты ////////////////////////////////
	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом
	////////////////////////////////////////////////////////

	////////////////////////////////////// загрузка миссий///////////////////////////////////////////////////
	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше
	///////////////////////////////////////////////////

	RenderWindow window(sf::VideoMode(1366, 768), "my First game");

	Player p("hero.png", 250, 250, 96.0, 96.0);//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	bool showMissionText = true;
	//произведен щелчок по спрайту?
	float dX = 0;// корректировка нажатия по х
	float dY = 0;//по у




	float CurrentFrame = 0;//хранит текущий кадр
	float currentFrame = 0;
	Clock clock;
	//Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры (для установки таймера)
	//int gameTime = 0;//инициализируем переменную игры

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсор
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		//if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//пока жив игрок,идет и время (для установки таймера)

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
				if ( sf::Mouse::Left)//а именно левая
					if (p.sprite.getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
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
						std::ostringstream playerHealthString;//строка здоровья игрока
						playerHealthString << p.health; //заносим в строку здоровье 
						std::ostringstream task;//строка текста миссии
						task << getTextMision(getCurrentMission(p.health));
						text.setString("Здоровье: " + playerHealthString.str() + "\n" + task.str());
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
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
				distance = sqrt((tempX - p.x)*(tempX - p.x) + (tempY - p.y)*(tempY - p.y));//считаем дистанцию (расстояние от точки А до точки Б). используя формулу длины вектора

			if (distance >30) {//этим условием убираем дергание во время конечной позиции спрайта
			
					p.x += 0.1*time*(tempX - p.x) / distance;//идем по иксу с помощью вектора нормали
					p.y += 0.1*time*(tempY - p.y) / distance;//идем по игреку так же
		}
				else { p.isMove = false; }//говорим что уже никуда не идем и выводим веселое сообщение в консоль
		//		std::cout << distance; 
 			}
                  //float dX = pos.x - p.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор
                //  float dY = pos.y - p.y;//он же, координата y
                //  float rotation = (atan2(dY, dX)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
                //  std::cout << rotation << "\n";//смотрим на градусы в консольке
               //   p.sprite.setRotation(rotation);//поворачиваем спрайт на эти градусы	

	

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
	     if(p.life){
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			p.dir = 1; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
			//getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//передаем координаты игрока в функцию управления камеройs
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
			getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//передаем координаты игрока в функцию управления камерой
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
			getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//передаем координаты игрока в функцию управления камерой

		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			p.dir = 2; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
			getPlayerCoordinateForView(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());//передаем координаты игрока в функцию управления камерой
		}
		p.update(time);
	//	viewmap(time);//функция скроллинга карты, передаем ей время sfml
	   changeview();//прикалываемся с камерой вида
		window.setView(view);//"оживляем" камеру в окне sfml
			window.clear();}
		 /////////////////////////////////////////СКРОЛЛИНГ КАРТЫ///////////////////////////////////////////////////////////////////////////////////////////
			sf::Vector2i localPosition = Mouse::getPosition(window);//заносим в вектор координаты мыши относительно окна (х,у)
			if (localPosition.x < 3) { view.move(-0.2*time, 0); }//если пришли курсором в левый край экрана,то двигаем камеру влево
			if (localPosition.x > window.getSize().x - 3) { view.move(0.2*time, 0); }//правый край-вправо
			if (localPosition.y > window.getSize().y - 3) { view.move(0, 0.2*time); }//нижний край - вниз
			if (localPosition.y < 3) { view.move(0, -0.2*time); }//верхний край - вверх
		 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////Рисуем карту/////////////////////
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
				{
					if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
					if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
					if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
					if (TileMap[i][j] == 'd') s_map.setTextureRect(IntRect(96, 0, 32, 32));//добавляем отнималку здоровья
					if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));//добавляем аптечку
					s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

					window.draw(s_map);//рисуем квадратики на экран
				}
		
			
			
	      
			//////////////////////////Смерть Персонажа//////////////////////////
			if (p.health <= 0) {
				text_g_o.setPosition(view.getCenter().x, view.getCenter().y);
				text_g_o.setString("GAME OVER");
				text.setCharacterSize(40);
			
				

			}
			//////////////////////////////////////////////////////////////////////////////////////
			if (!showMissionText) {
				text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
				s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока			
				window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
			}
			window.draw(text_g_o);
			window.draw(p.sprite);
            
			window.display();
		}

		return 0;
	}

