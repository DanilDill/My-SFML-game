#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������

void getPlayerCoordinateForView(float x, float y) { //������� ��� ���������� ��������� ������


	view.setCenter(x + 100, y); //������ �� �������, ��������� ��� ���������� ������. +100 - �������� ������ �� ���� ������. �����������������


}
void changeview() {


	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.000f); //������������, ����������
							//view.zoom(1.0006f); //���� ����� ����������� ��������������
	}

	if (Keyboard::isKeyPressed(Keyboard::R)) {
		//view.setRotation(90);//����� �� ������ ������� ������
		view.rotate(1);//���������� ������������ ������ (������������� �������� - � �������� �������)
	}


	
		view.setSize(700, 500);//������������� ������ ������ (��� ��������)
	

	if (Keyboard::isKeyPressed(Keyboard::P)) {
		view.setSize(540, 380);//�������� ������ ������
	}


	

}
