#include <iostream>

#define GREEN_COLOR "\x1b[32m"
#define RED_COLOR "\x1b[91m"
#define BLACK_COLOR "\x1b[30m"
#define RESET_COLOR "\x1b[0m"

using namespace std;

void moveOnConsole(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class DriveUnit
{
private:
	string name;
	char condition = '0';

public:
	int x, y;

	DriveUnit(string nameDriveUnit, int x, int y)
	{
		name = nameDriveUnit;
		this->x = x;
		this->y = y;
	}

	void changeCondition()
	{
		condition = (char)((condition + 1) % 2 + 48);
		printInfo();
	}

	void printInfo()
	{
		moveOnConsole(x, y);
		if (condition == '1')
		{
			cout << GREEN_COLOR;
		}

		cout << name << RESET_COLOR;
	}
};

class Sensor
{
private:
	string name;
	char condition = '0';

public:
	int x, y;
	DriveUnit* driveUnit;

	Sensor(string nameSensor, int x, int y)
	{
		name = nameSensor;
		this->x = x;
		this->y = y;
	}

	Sensor(string nameSensor, int x, int y, string nameDriveUnit) : Sensor(nameSensor, x, y)
	{
		driveUnit = new DriveUnit(nameDriveUnit, x + 4, y);
	}

	void changeCondition()
	{
		condition = (char)((condition + 1) % 2 + 48);
		printInfo();
	}

	void printInfo()
	{
		moveOnConsole(x, y);
		if (condition == '1')
		{
			cout << GREEN_COLOR;
		}

		cout << name << RESET_COLOR;
		
		if (driveUnit != nullptr)
		{
			driveUnit->printInfo();
		}
	}
};

class Container
{
private:
	string name;

public:
	int quantityOfGoods;
	int capacity;

	Container(string name, int capacity, int quantity)
	{
		this->name = name;
		this->capacity = capacity;
		quantityOfGoods = quantity;
	}

	virtual void printInfo() = 0;
};

class ContainerWithUnmarkedGoods : public Container
{
public:
	ContainerWithUnmarkedGoods(string name, int capacity) 
		: Container(name, capacity, capacity) {};

	void changeQuantity()
	{
		quantityOfGoods--;
		printInfo();
	}

	void printInfo()
	{
		moveOnConsole(0, 0);
		cout << "Осталось немаркированных товаров: " << quantityOfGoods * 4;
	}
};

class ContainerWithMarkedGoods : public Container
{
public:
	ContainerWithMarkedGoods(string name, int capacity) 
		: Container(name, capacity, 0) {};

	void addGoods()
	{
		quantityOfGoods += 4;
		printInfo();
	}

	void printInfo()
	{
		moveOnConsole(0, 1);
		cout << "Заполненность контейнера: " << quantityOfGoods << "/" << capacity;
	}
};