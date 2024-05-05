#include <iostream>
#include <windows.h>
#include <chrono>
#include <fstream>
#include <thread>
#include "Components.h"

using namespace std;

class Controller
{
private:
	ContainerWithUnmarkedGoods* containersWithUnmarkedGoods[4];
	ContainerWithMarkedGoods* containerWithMarkedGoods;

	Sensor* I0, * I1, * I2, * I3, * I4, * I5, * I6, * I7, * I8, * I9, * I10, * I11;
	DriveUnit* Q0, * Q1, * Q2, * Q3, * Q13, * Q14, * Q15;

public:
	Controller(int smallCapacity, int bigCapacity)
	{
		containersWithUnmarkedGoods[0] = new ContainerWithUnmarkedGoods("Conatiner1", smallCapacity);
		containersWithUnmarkedGoods[1] = new ContainerWithUnmarkedGoods("Conatiner2", smallCapacity);
		containersWithUnmarkedGoods[2] = new ContainerWithUnmarkedGoods("Conatiner3", smallCapacity);
		containersWithUnmarkedGoods[3] = new ContainerWithUnmarkedGoods("Conatiner4", smallCapacity);

		containerWithMarkedGoods = new ContainerWithMarkedGoods("MainContainer", bigCapacity);

		containersWithUnmarkedGoods[0]->printInfo();
		containerWithMarkedGoods->printInfo();

		I10 = new Sensor("I10", 1, 8);
		I11 = new Sensor("I11", 1, 10);
		Q13 = new DriveUnit("Q13", 1, 12);
		Q14 = new DriveUnit("Q14", Q13->x + 5, Q13->y);
		Q15 = new DriveUnit("Q15", 1, 14);

		Q0 = new DriveUnit("Q0", I10->x + 14, I10->y);
		Q1 = new DriveUnit("Q1", Q0->x, Q0->y + 2);
		Q2 = new DriveUnit("Q2", Q1->x, Q1->y + 2);
		Q3 = new DriveUnit("Q3", Q2->x, Q2->y + 2);

		I1 = new Sensor("I1", Q0->x + 14, I10->y, "Q4");
		I2 = new Sensor("I2", I1->x, I1->y + 2, "Q5");
		I3 = new Sensor("I3", I1->x, I2->y + 2, "Q6");
		I4 = new Sensor("I4", I1->x, I3->y + 2, "Q7");

		I6 = new Sensor("I6", I1->x + 14, I10->y, "Q9");
		I7 = new Sensor("I7", I6->x, I6->y + 2, "Q10");
		I8 = new Sensor("I8", I7->x, I7->y + 2, "Q11");
		I9 = new Sensor("I9", I8->x, I8->y + 2, "Q12");

		I0 = new Sensor("I0", I6->x + 14, I10->y, "Q8");
		I5 = new Sensor("I5", I0->x, I0->y + 2);

		printOperatorConsole();

		/*
		thread inputThread(&Controller::finishTheProgram, this);
		thread outputThread(&Controller::runTheProgram, this);
		
		inputThread.join();
		outputThread.join();
		*/
		runTheProgram();
	}

	void printOperatorConsole()
	{
		string roof(70, '-');

		moveOnConsole(1, 7);
		cout << roof;
		moveOnConsole(1, 15);
		cout << roof;

		for (int x = 0; x < 71; x += 14)
		{
			for (int y = 8; y < 15; y++)
			{
				moveOnConsole(x, y);
				cout << "|";
			}
		}

		I10->printInfo();

		I11->printInfo();

		Q13->printInfo();
		Q14->printInfo();

		Q15->printInfo();

		Q0->printInfo();
		Q1->printInfo();
		Q2->printInfo();
		Q3->printInfo();

		I1->printInfo();
		I2->printInfo();
		I3->printInfo();
		I4->printInfo();

		I6->printInfo();
		I7->printInfo();
		I8->printInfo();
		I9->printInfo();

		I0->printInfo();
		I5->printInfo();
	}

	void printMessage(string message, int delay)
	{
		string emptyLine(79, ' ');

		moveOnConsole(0, 3);
		cout << message << emptyLine.substr(message.size());

		this_thread::sleep_for(chrono::milliseconds(delay));
	}

	/*
	void finishTheProgram()
	{
		moveOnConsole(0, 30);
		int temp;
		cin >> temp;
	}
	*/
	void runTheProgram()
	{
		I10->changeCondition();
		printMessage("Старт программы...", 2000);
		I10->changeCondition();
		
		Q0->changeCondition();
		Q1->changeCondition();
		Q2->changeCondition();
		Q3->changeCondition();
		printMessage("Включение конвейерных лент...Активация приводов Q0, Q1, Q2, Q3...", 2000);

		while (true)
		{
			printMessage("Проверка наличия заготовок в контейнерах...Активация датчиков I1, I2, I3, I4...", 2000);

			I1->changeCondition();
			this_thread::sleep_for(chrono::milliseconds(500));
			I1->changeCondition();

			I2->changeCondition();
			this_thread::sleep_for(chrono::milliseconds(500));
			I2->changeCondition();

			I3->changeCondition();
			this_thread::sleep_for(chrono::milliseconds(500));
			I3->changeCondition();

			I4->changeCondition();
			this_thread::sleep_for(chrono::milliseconds(500));
			I4->changeCondition();

			if (containersWithUnmarkedGoods[0]->quantityOfGoods == 0)
			{
				Q0->changeCondition();
				Q1->changeCondition();
				Q2->changeCondition();
				Q3->changeCondition();
				Q13->changeCondition();
				printMessage("Заготовки закончились...Выходим из программы.", 3000);
				Q13->changeCondition();

				moveOnConsole(0, 16);
				break;
			}

			for (ContainerWithUnmarkedGoods* item : containersWithUnmarkedGoods)
			{
				item->changeQuantity();
				cout << " ";
			}

			I1->driveUnit->changeCondition();
			I2->driveUnit->changeCondition();
			I3->driveUnit->changeCondition();
			I4->driveUnit->changeCondition();
			printMessage("Открываем контейнеры с заготовками...Активация приводов Q4, Q5, Q6, Q7...", 2500);
	
			I1->driveUnit->changeCondition();
			I2->driveUnit->changeCondition();
			I3->driveUnit->changeCondition();
			I4->driveUnit->changeCondition();
			printMessage("Товары поступают на ленту...", 2000);

			I6->changeCondition();
			I7->changeCondition();
			I8->changeCondition();
			I9->changeCondition();
			printMessage("Активация датчиков приближения I6, I7, I8, I9...", 2500);
			I6->changeCondition();
			I7->changeCondition();
			I8->changeCondition();
			I9->changeCondition();

			I6->driveUnit->changeCondition();
			I7->driveUnit->changeCondition();
			I8->driveUnit->changeCondition();
			I9->driveUnit->changeCondition();
			printMessage("Активация приводов печати Q9, Q10, Q11, Q12", 2500);
			I6->driveUnit->changeCondition();
			I7->driveUnit->changeCondition();
			I8->driveUnit->changeCondition();
			I9->driveUnit->changeCondition();

			printMessage("Товары промаркированы...", 2000);

			I0->changeCondition();
			printMessage("Активация датчика I0...", 2000);
			I0->changeCondition();
			
			I0->driveUnit->changeCondition();
			printMessage("Открывается крышка контейнера с приводом Q8...", 2500);
			I0->driveUnit->changeCondition();

			printMessage("Партия маркированных товаров попала в контейнер...", 2500);
			containerWithMarkedGoods->addGoods();

			I5->changeCondition();
			printMessage("Активация датчика I5...Проверка заполненности контейнера", 2500);
			I5->changeCondition();

			if (containerWithMarkedGoods->quantityOfGoods >= containerWithMarkedGoods->capacity)
			{
				Q0->changeCondition();
				Q1->changeCondition();
				Q2->changeCondition();
				Q3->changeCondition();
				Q14->changeCondition();

				/*
				if (containerWithMarkedGoods->quantityOfGoods >= containerWithMarkedGoods->capacity)
				{
					printMessage("Контейнер переполнен...Завершение работы...Осталось " + 
						to_string(containerWithMarkedGoods->quantityOfGoods - containerWithMarkedGoods->capacity) +
						" немаркированных заготовок", 3000);
				}
				*/
				printMessage("Контейнер полон...Завершение работы...", 3000);

				Q14->changeCondition();

				moveOnConsole(0, 16);
				break;
			}

			printMessage("Ожидание новой партии товаров...", 2000);
		}
	}
};

class UserInterface
{
public:
	void startProgram(string message = "")
	{
		if (message == "exit")
		{
			return;
		}

		string welcomeMessage = "Добро пожаловать в программу автоматизированной системы управления!\n"
			"Введите одну из команд:\n\n";
		cout << welcomeMessage;
	
		string menu = "start - Запуск программы\n"
					  "info - Вывод инструкции\n"
					  "exit - Выйти из программы";
		cout << menu << endl << "\n";
	
		string controlMessage;
		cin >> controlMessage;
	
		if (controlMessage == "start")
		{
			system("cls");
			inputData();
		}
	
		else if (controlMessage == "info")
		{
			system("cls");
			string message;

			do
			{
				ifstream file("instruction.txt");
				string s;
				while (getline(file, s))
				{
					cout << s << "\n";
				}
				file.close();

				cout << "back - Вернуться обратно в меню\n";

				cin >> message;

				if (message != "back")
				{
					if (cin.fail())
					{
						cin.clear();
					}

					system("cls");
					cout << RED_COLOR << "Введена несуществующая команда!\n" << RESET_COLOR;
				}
			} while (message != "back");

			system("cls");
			startProgram();
		}
	
		else if (controlMessage == "exit")
		{
			return;
		}

		else
		{
			if (cin.fail())
			{
				cin.clear();
			}

			system("cls");
			cout << RED_COLOR << "Введена несуществующая команда!\n\n" << RESET_COLOR;
			startProgram();
		}
	}

	void inputData()
	{
		cout << "Введите вместимость 1-го контейнера, в котором будут храниться\n"
				"немаркированные заготовки(Общее количество заготовок будет умножено на 4): ";
		int smallCapacity;
		cin >> smallCapacity;
	
		cout << "Введите вместимость контейнера, в котором будут храниться маркированные заготовки: ";
		int bigCapacity;
		cin >> bigCapacity;
	
		cout << "\nВведите команду:\n"
				"start - Запустить программу\n"
				"exit - Выйти в меню\n\n";
	
		string controlMessage;
		cin >> controlMessage;
	
		system("cls");
		if (controlMessage == "exit")
		{
			startProgram();
		}

		else if (controlMessage == "start")
		{
			Controller controller(smallCapacity, bigCapacity);
		}

		else
		{
			if (cin.fail())
			{
				cin.clear();
			}

			system("cls");
			cout << RED_COLOR << "Введена несуществующая команда!\n\n" << RESET_COLOR;
			inputData();
		}
	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	UserInterface userInterface;
	userInterface.startProgram();

	return 0;
}