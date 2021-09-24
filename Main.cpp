#include <iostream>
#include <string>

using namespace std;

class Weapon {
private:
	int _id;
	string _name;
	string _sound;
	int _price;

public:
	Weapon(int id, string name, string sound, int price)
	{
		_id = id;
		_name = name;
		_sound = sound;
		_price = price;
	}

	int GetId()
	{
		return _id;
	}

	void setName(string name)
	{
		_name = name;
	}

	string getName()
	{
		return _name;
	}

	void setSound(string sound)
	{
		_sound = sound;
	}

	string getSound()
	{
		return _sound;
	}

	void setPrice(int price)
	{
		_price = price;
	}

	int getPrice()
	{
		return _price;
	}
};

const int INITIAL_PLAYER_BALANCE = 2500;
const int MAX_AMOUNT_OF_WEAPONS_FOR_STORE = 9;
const int MAX_AMOUNT_OF_WEAPONS_FOR_PLAYER = 5;

enum BuyWeaponStatus {BUY_WEAPON_STATUS_SUCCESS, BUY_WEAPON_STATUS_ERROR_INVALID_ID, BUY_WEAPON_STATUS_ERROR_NOT_ENOUGH_BALANCE, BUY_WEAPON_STATUS_ERROR_ALREADY_OWNED,	BUY_WEAPON_STATUS_FULL_PLAYER_INVENTORY};

class Player
{
private:
	// array of weapons player ownes
	Weapon* weapons[MAX_AMOUNT_OF_WEAPONS_FOR_PLAYER];
	// actual number of weapons acquired by player
	int numberOfWeapons;
	// player's current balance
	int balance;
	// currently selected weapon
	Weapon* selectedWeapon;

public:
	string playerWeapons[MAX_AMOUNT_OF_WEAPONS_FOR_PLAYER] = { }; // Created an array for player weapons
	Player()
	{
		numberOfWeapons = 0;
		balance = INITIAL_PLAYER_BALANCE;
		selectedWeapon = nullptr;
	}

	int GetNumberOfWeapons()
	{
		return numberOfWeapons;
	}

	void AddWeapon(Weapon* weapon)
	{
		weapons[numberOfWeapons++] = weapon;
		cout << "You have selected " << weapon->getName() << endl;
	}

	Weapon* GetWeapon(int index)
	{
		return weapons[index];
	}

	Weapon* GetOwenedWeapons()
	{
		return *weapons;
	}

	bool IsOwned(int weaponId)
	{
		for (int i = 0; i < numberOfWeapons; ++i)
		{
			if (weapons[i]->GetId() == weaponId)
			{
				return true;
			}
		}

		return false;
	}

	void SetBalance(int amount)
	{
		balance = amount;
	}

	int GetBalance()
	{
		return balance;
	}

	Weapon* GetSelectedWeapon()
	{
		return selectedWeapon;
	}

	Weapon* SelectWeapon(int id)
	{
		for (int i = 0; i < numberOfWeapons; ++i)
		{
			if (weapons[i]->GetId() == id)
			{
				selectedWeapon = weapons[id];

				return selectedWeapon;
			}
		}

		return nullptr;
	}
};

class Store // Logic
{	
private:
	Weapon* weapons[MAX_AMOUNT_OF_WEAPONS_FOR_STORE];

public:
	Store()
	{

		weapons[0] = new Weapon(1, "Fire Sword", "Buurn!", 500);
		weapons[1] = new Weapon(2, "Water Sword", "splash!", 500);
		weapons[2] = new Weapon(3, "Steel Sword", "swoosh!", 200);
		weapons[3] = new Weapon(4, "Ancient Wand", "shpling!", 2000);
		weapons[4] = new Weapon(5, "Battle Axe", "swing!", 1000);
		weapons[5] = new Weapon(6, "Iron Shield", "clank!", 500);
		weapons[6] = new Weapon(7, "Healing Potion", "HEAL HEAL HEAL", 100);
		weapons[7] = new Weapon(8, "Potion Sword", "Sting!", 400);
		weapons[8] = new Weapon(9, "Hamburger", "MMMM YUMMY", 50);
	}

	~Store()
	{
		for (int i = 0; i < MAX_AMOUNT_OF_WEAPONS_FOR_STORE; ++i)
			delete weapons[i];
	}

	Weapon* GetWeapon(int id)
	{
		if (id < 1 || id > MAX_AMOUNT_OF_WEAPONS_FOR_STORE)
		{
			return nullptr;
		}

		return weapons[id - 1];
	}

	BuyWeaponStatus BuyWeapon(Player& player, int weaponId)
	{
		// todo: validate and return one of the ByuWepoanStatus error statuses:
		//       1. validate weapons id is in range [1,9] - if note, return...
		//       2. wether the weapon with this id already owned bythe player - if yes, return...
		//       3. wether enough balance to buy it - if not return...
		//       4. if player already has 5 weapons - return... FULL_INVENTORY.. 

		int index = weaponId - 1;

		if (index < 0 || index >= MAX_AMOUNT_OF_WEAPONS_FOR_STORE)
		{
			return BUY_WEAPON_STATUS_ERROR_INVALID_ID;
		}

		if (player.IsOwned(weaponId))
		{
			return BUY_WEAPON_STATUS_ERROR_ALREADY_OWNED;
		}

		if (player.GetBalance() < weapons[index]->getPrice())
		{
			return BUY_WEAPON_STATUS_ERROR_NOT_ENOUGH_BALANCE;
		}

		if (player.GetNumberOfWeapons() == MAX_AMOUNT_OF_WEAPONS_FOR_PLAYER || player.GetBalance() == 0)
		{
			return BUY_WEAPON_STATUS_FULL_PLAYER_INVENTORY;
		}


		player.AddWeapon(weapons[index]);
		player.SetBalance(player.GetBalance() - weapons[index]->getPrice());

		return BUY_WEAPON_STATUS_SUCCESS;
	}
};


class Menu //User interface Class
{
private:
	int RunMainMenu()
	{
		int choice;

		cout << "Hello there, Welcome to the Mystical Weapon Store. Please choose what you would like to do during your stay: " << endl;
		cout << "[1]" << " Buy Weapon" << endl;
		cout << "[2]" << " Check Balance" << endl;
		cout << "[3]" << " Exit Store" << endl;

		cin >> choice;

		return choice;
	}

	void RunBuyWeaponMenu(Store& store, Player& player)
	{
		int weaponToBuyId;

		if (player.GetNumberOfWeapons() > 0)
		{
			cout << "Your selected weapons are: " << endl;
			for (int i = 0; i < player.GetNumberOfWeapons(); i++)
			{
				cout << player.GetWeapon(i)->GetId() << "\t" << player.GetWeapon(i)->getName() << "\t(" << player.GetWeapon(i)->getSound() << ")" << endl;
			}
			cout << endl;
		}

		cout << "Please choose your weapons from the following list: " << endl;
		cout << "Slot #\t Weapon Name\t\t Price" << endl;

		for (int id = 1; id <= MAX_AMOUNT_OF_WEAPONS_FOR_STORE; id++)
		{
			Weapon* weapon = store.GetWeapon(id);

			cout << id << "\t" << weapon->getName() << "\t\t$" << weapon->getPrice() << endl;
		}

		cout << "Your total amount to spend is: $" << player.GetBalance() << " coins" << endl;
		cout << endl;
		cout << "To exit to the main menu, please type 0" << endl;

		cin >> weaponToBuyId;

		if (weaponToBuyId == 0)
		{
			return;
		}

		BuyWeaponStatus buyStatus = store.BuyWeapon(player, weaponToBuyId);

		switch (buyStatus)
		{
		case BUY_WEAPON_STATUS_SUCCESS:
			break;

		case BUY_WEAPON_STATUS_ERROR_INVALID_ID:
			cout << "Please enter a valid weapon ID number (1-9): ";
			break;

		case BUY_WEAPON_STATUS_ERROR_ALREADY_OWNED:
			cout << "You already own this weapon." << endl << "Please choose another weapon: " << endl;
			break;

		case BUY_WEAPON_STATUS_ERROR_NOT_ENOUGH_BALANCE:
			cout << "You cannot afford this weapon!" << endl << "Please choose another weapon: " << endl;
			break;

		case BUY_WEAPON_STATUS_FULL_PLAYER_INVENTORY:
			cout << "You have now filled up your inventory and are ready to head out!" << endl << "Happy Hunting! We'll see you soon!" << endl;
			break;
		}

		/*
		for (int i = 0; i < player.GetNumberOfWeapons(); i++)
		{
			if (player.GetNumberOfWeapons >)
			cout << player.GetWeapon(i)
		}
		*/
	}
public:
	void WeaponManagement(Store& store, Player& player)
	{
		while (true)
		{

			int option = RunMainMenu();

			switch (option)
			{
			case 1:
				if (player.GetNumberOfWeapons() == MAX_AMOUNT_OF_WEAPONS_FOR_PLAYER
					|| player.GetBalance() == 0)
				{
					cout << "You have eceeded number of weapons or your balance is $0." << endl;
				}
				else
				{
					RunBuyWeaponMenu(store, player);
				}
				break;

			case 2:
				cout << "Player balanace is: " << player.GetBalance() << endl;
				break;

			case 3:
				return;

			default:
				cout << "No Such Option Exists, please select an option between 1-3: " << endl;
			}
		}
	}

	int SelectOwnedWeapons(Player& player)
	{
		int selection;

		cout << "Please select a weapon from your inventory: " << endl;

		for (int i = 0; i < player.GetNumberOfWeapons(); i++)
		{
			cout << i + 1 << "\t" << player.GetWeapon(i)->getName() << "\t" << player.GetWeapon(i)->getSound() << endl;
		}

		cin >> selection;
		return selection;
	}

	// Something wront here
	void Simulation(Player &player)
	{

		int option;

		while (true)
		{
			option = SelectOwnedWeapons(player);

		}
	}
};

int main()
{
	Store store;
	Player player;
	Menu menu;

	menu.WeaponManagement(store, player);
	menu.Simulation(player);
	
	/*

	const int weaponAmount = 9;
	
	Weapon* weapons[weaponAmount];
	weapons[0] = new Weapon("Fire Sword", "Buurn!", 500);
	weapons[1] = new Weapon("Water Sword", "splash!", 500);
	weapons[2] = new Weapon("Steel Sword", "swoosh!", 200);
	weapons[3] = new Weapon("Ancient Wand", "shpling!", 2000);
	weapons[4] = new Weapon("Battle Axe", "swing!", 1000);
	weapons[5] = new Weapon("Iron Shield", "clank!", 500);
	weapons[6] = new Weapon("Healing Potion", "HEAL HEAL HEAL", 100);
	weapons[7] = new Weapon("Potion Sword", "Sting!", 400);
	weapons[8] = new Weapon("Hamburger", "MMMM YUMMY", 50);

	int playerCoins = 2500;
	
	Weapon *playerWeapons[5];

	Store store;

	cout << "Welcome to your Weapons store." << endl << "Please select an option from the folowing menu:" << endl;
	cout << "[1]" << " Buy Weapon" << endl;
	cout << "[2]" << " Check Balance" << endl;
	cout << "[3]" << " Exit Store" << endl;

	int selected;
	cin >> selected;
	store.setSelecetedOption(selected);

	switch (store.getSelectedOption())
	{
	case 1:
		cout << "Please choose your weapons from the following list: " << endl;
		cout << "Slot #\t Weapon Name\t\t Price" << endl;
		for (int i = 0; i < 9; i++)
		{
			cout << i + 1 << "\t" << weapons[i]->getWeaponName() << "\t\t$" << weapons[i]->getPrice() << endl;
		}

		cout << "Your total amount to spend is: $" << playerCoins << " coins" << endl;

		for (int i = 0; i < 5 && playerCoins > 0; i++)
		{
			int weaponNumber = 0;
			cout << "Choose a weapon for your slot: " << i + 1 << endl;
			cin >> weaponNumber;

			if (weaponNumber > 0)
			{
				while (weapons[weaponNumber - 1]->getIsOwned() || weapons[weaponNumber - 1]->getPrice() > playerCoins)
				{
					if (weapons[weaponNumber - 1]->getIsOwned())
					{
						cout << "You have already selected this weapon. Please choose anpther one." << endl;
						cin >> weaponNumber;
					}
					if (weapons[weaponNumber - 1]->getPrice() > playerCoins)
					{
						cout << "You cannot afford this weapon. Please choose anpther one." << endl;
						cin >> weaponNumber;
					}
				}
				playerWeapons[i] = weapons[weaponNumber - 1];
				playerWeapons[i]->setIsOwned(true);
				playerCoins -= weapons[weaponNumber - 1]->getPrice();
			}
			cout << "coins balance = " << playerCoins << endl;
		}

		cout << "Your weapons are: " << endl;
		for (int i = 0; i < 5; i++)
		{
			cout << " Weapon Name: " << playerWeapons[i]->getWeaponName() << "; Weapon Sound: " << playerWeapons[i]->getSound() << endl;

		}

	}
	
	*/
	// redundant method - must be removed
	//bool RunWeaponManagement(Store& store, Player& player)
	//{
	//	// todo:
	//	// 1. print the menu
	//	// 2. wait for user's choice
	//	// 3. validate user choice
	//	// 4. if "Buy wrapon" - call store.BuyWeapon(id) cna check teh return status
	//	// 5. if "Check balance" - return store.GetBalance();
	//	// 6. if "Exit" - return

	//	cout << "Hello there, Welcome to the Mystical Weapon Store. Please choose what you would like to do during your stay: " << endl;
	//	cout << "[1]" << " Buy Weapon" << endl;
	//	cout << "[2]" << " Check Balance" << endl;
	//	cout << "[3]" << " Exit Store" << endl;
	//	cin >> playerMenuChoice;


	//	if (playerMenuChoice < 1 || playerMenuChoice > 3)
	//	{
	//		cout << "No Such Option Exists, please select an option between 1-3: " << endl;
	//		return true;
	//	}

	//	if (playerMenuChoice == 1)
	//	{
	//		cout << "Please choose your weapons from the following list: " << endl;
	//		cout << "Slot #\t Weapon Name\t\t Price" << endl;
	//		for (int i = 0; i < 9; i++)
	//		{
	//			cout << i + 1 << "\t" << weapons->getName() << "\t\t$" << weapons->getPrice() << endl;
	//		}

	//		cout << "Your total amount to spend is: $" << INITIAL_PLAYER_BALANCE << " coins" << endl;


	//		int id = 0;
	//		BuyWeaponStatus status = store.BuyWeapon(id);

	//		switch (status)
	//		{
	//		case BUY_WEAPON_STATUS_SUCCESS:
	//			break;
	//		case BUY_WEAPON_STATUS_ERROR_INVALID_ID:
	//			cout << "Please enter a valid weapon ID number (1-9): ";
	//			return true;
	//		case BUY_WEAPON_STATUS_ERROR_ALREADY_OWNED:
	//			cout << "You already own this weapon." << endl << "Please choose another weapon: " << endl;
	//			cin >> playerWeaponChoice;
	//		case BUY_WEAPON_STATUS_ERROR_NOT_ENOUGH_BALANCE:
	//			cout << "You cannot afford this weapon!" << endl << "Please choose another weapon: " << endl;
	//			cin >> playerWeaponChoice;
	//		case BUY_WEAPON_STATUS_FULL_PLAYER_INVENTORY:
	//			cout << "You have now filled up your inventory and are ready to head out!" << endl << "Happy Hunting! We'll see you soon!" << endl;
	//			break;
	//		}
	//	}

	//	if (playerMenuChoice == 2)
	//	{
	//		cout << "Youre current balance  is: " << playerbalance.GetPlayerBalance() << endl;
	//	}

	//	if (playerMenuChoice == 3)
	//	{
	//		cout << "Happy Hunting! See you next time! " << endl;
	//		break;
	//	}
	//}

}
