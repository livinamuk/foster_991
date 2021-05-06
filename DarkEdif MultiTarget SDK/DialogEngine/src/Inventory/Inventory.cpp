
#include "Inventory.h"
#include <algorithm>

std::vector<InventoryItemData> Inventory::s_InventoryDatabase;

PlayerInventoryItem* Inventory::p_currentInventory;
PlayerInventoryItem Inventory::s_playerInventory_wearable[INVENTORY_SIZE_LIMIT] = {};
PlayerInventoryItem Inventory::s_playerInventory_equipable[INVENTORY_SIZE_LIMIT] = {};
PlayerInventoryItem Inventory::s_playerInventory_material[INVENTORY_SIZE_LIMIT] = {};
PlayerInventoryItem Inventory::s_playerInventory_consumable[INVENTORY_SIZE_LIMIT] = {};
PlayerInventoryItem Inventory::s_playerInventory_general[INVENTORY_SIZE_LIMIT] = {};
PlayerInventoryItem Inventory::s_playerInventory_quest[INVENTORY_SIZE_LIMIT] = {};

Inventory::EquippedItems Inventory::s_equippedItems;

int Inventory::m_max_wearable_slots;
int Inventory::m_max_equipable_slots;
int Inventory::m_max_material_slots;
int Inventory::m_max_consumable_slots;
int Inventory::m_max_general_slots;
int Inventory::m_max_quest_slots;

float Inventory::s_maxWeight;

std::vector<Container> Inventory::s_containers;
InventoryBagType Inventory::s_currentInventoryBagType;
std::vector<Companion> Inventory::s_compainions;
std::string Inventory::s_currentCompanionName;
std::string Inventory::s_currentContainerName;
Container* Inventory::p_displayedContainer;


void Inventory::SetCurrentInventoryBagToGeneral() {
	s_currentInventoryBagType = InventoryBagType::GENERAL;
	p_currentInventory = s_playerInventory_general;
}
void Inventory::SetCurrentInventoryBagToWearable() {
	s_currentInventoryBagType = InventoryBagType::WEARABLE;
	p_currentInventory = s_playerInventory_wearable;
}
void Inventory::SetCurrentInventoryBagToEquipable() {
	s_currentInventoryBagType = InventoryBagType::EQUIPABLE;
	p_currentInventory = s_playerInventory_equipable;
}
void Inventory::SetCurrentInventoryBagToMaterial() {
	s_currentInventoryBagType = InventoryBagType::MATERIAL;
	p_currentInventory = s_playerInventory_material;
}
void Inventory::SetCurrentInventoryBagToQuest() {
	s_currentInventoryBagType = InventoryBagType::QUEST;
	p_currentInventory = s_playerInventory_quest;
}
void Inventory::SetCurrentInventoryBagToConsumeable() {
	s_currentInventoryBagType = InventoryBagType::CONSUMABLE;
	p_currentInventory = s_playerInventory_consumable;
}
int Inventory::GetCurrentInventoryBagSize() {
	if (s_currentInventoryBagType == InventoryBagType::GENERAL)
		return m_max_general_slots;
	else if (s_currentInventoryBagType == InventoryBagType::WEARABLE)
		return m_max_wearable_slots;
	else if (s_currentInventoryBagType == InventoryBagType::EQUIPABLE)
		return m_max_equipable_slots;
	else if (s_currentInventoryBagType == InventoryBagType::MATERIAL)
		return m_max_material_slots;
	else if (s_currentInventoryBagType == InventoryBagType::QUEST)
		return m_max_quest_slots;
	else if (s_currentInventoryBagType == InventoryBagType::CONSUMABLE)
		return m_max_consumable_slots;
	else
		return 0;
}

std::string Inventory::GetCurrentContainerName()
{
	if (!p_displayedContainer)
		return "NO CONTAINER SELECTED";
	else
		return p_displayedContainer->name;
}

std::string Inventory::GetCurrentContainerIconName()
{
	if (!p_displayedContainer)
		return "NO CONTAINER SELECTED";
	else
		return p_displayedContainer->iconName;
}

void Inventory::ShowContainer()
{
	for (Container& container : s_containers)
		if (container.name == s_currentContainerName) {
			p_displayedContainer = &container;
			//s_currentCompanionName = container.name;
		}
}

void Inventory::ShowCompanion()
{
	for (Container& container : s_containers)
		if (container.name == s_currentCompanionName) {
			//s_currentContainerName = s_currentCompanionName;
			p_displayedContainer = &container;
			return;
		}
}

void Inventory::SetCurrentContainerByName(std::string name)
{
	for (Container& container : s_containers)
		if (container.name == name) {
			p_displayedContainer = &container;
			s_currentContainerName = name;
			return;
		}
}

void Inventory::SetCurrentCompanionByName(std::string name)
{
	for (Container& container : s_containers)
		if (container.name == name) {
			p_displayedContainer = &container;
			s_currentCompanionName = name;
			return;
		}
}

struct by_name {
	bool operator()(InventoryItemData const& a, InventoryItemData const& b) const {
		return a.m_name < b.m_name;
	}
};

void Inventory::NewInventoryItem(std::string name, InventoryType type, float weight, std::string description, int price, std::vector<Modifier> modifers = std::vector<Modifier>())
{
	InventoryItemData itemData;
	itemData.m_name = name;
	itemData.m_type = type;
	itemData.m_weight = weight;
	itemData.m_description = description;
	itemData.m_price = price;
	itemData.m_modifiers = modifers;
	s_InventoryDatabase.push_back(itemData);
	std::sort(s_InventoryDatabase.begin(), s_InventoryDatabase.end(), by_name());
}

void Inventory::InitDefaults()
{
	s_maxWeight = 5;
	m_max_wearable_slots = 8;
	m_max_equipable_slots = 8;
	m_max_material_slots = 8;
	m_max_consumable_slots = 8;
	m_max_general_slots = 8;
	m_max_quest_slots = 8;

	SetCurrentInventoryBagToGeneral();

	// TESTING DATAAAAAAAAAAAAA
	NewInventoryItem("Hat", InventoryType::WEARABLE_HEAD, 0.1f, "bla bla bla", 1);
	NewInventoryItem("Hammer", InventoryType::MATERIAL, 0.1f, "bla bla bla", 1);
	NewInventoryItem("Jeans", InventoryType::WEARABLE_LOWER_BODY, 0.1f, "bla bla bla", 1);
	NewInventoryItem("Acid Gloves", InventoryType::WEARABLE_HANDS, 0.1f, "bla bla bla", 1);
	NewInventoryItem("Knife", InventoryType::EQUIPABLE, 0.1f, "bla bla bla", 1);
	NewInventoryItem("Jacket", InventoryType::WEARABLE_UPPER_BODY, 0.4f, "bla bla bla", 2);
	NewInventoryItem("Boots", InventoryType::WEARABLE_FEET, 0.4f, "bla bla bla", 2);
	NewInventoryItem("Utility Belt", InventoryType::WEARABLE_BELT, 0.4f, "bla bla bla", 2);
	NewInventoryItem("Canned Beans", InventoryType::CONSUMABLE, 0.4f, "bla bla bla", 2);
	NewInventoryItem("Shotgun", InventoryType::EQUIPABLE, 0.4f, "bla bla bla", 2);
	NewInventoryItem("Medkit", InventoryType::CONSUMABLE, 0.4f, "bla bla bla", 2);

	Modifier buff1, buff2;
	buff1.m_type = ModifierType::SPEED;
	buff1.m_value = 20;
	buff2.m_type = ModifierType::LOCK_PICKING;
	buff2.m_value = 10;
	std::vector<Modifier> modifers;
	modifers.push_back(buff1);
	modifers.push_back(buff2);

	NewInventoryItem("Map", InventoryType::GENERAL, 0.05f, "bla bla bla", 33, modifers);
	NewInventoryItem("Health Potion", InventoryType::CONSUMABLE, 0.05f, "bla bla bla", 33, modifers);

	NewContainer("Test Container A", "Box", 9, std::vector<PlayerInventoryItem> {
		PlayerInventoryItem("Knife", 1),
			PlayerInventoryItem("Jacket", 1),
			PlayerInventoryItem("Hammer", 2),
			PlayerInventoryItem("Utility Belt", 1),
			PlayerInventoryItem("Boots", 1),
			PlayerInventoryItem("Jeans", 1)});

	NewContainer("Test Container B", "Box", 4, std::vector<PlayerInventoryItem> {
		PlayerInventoryItem("Shotgun", 1),
			PlayerInventoryItem("Illegal Documents", 3)});

	NewCompanion("Devil", CompanionType::DOG);
}

void Inventory::NewCompanion(std::string name, CompanionType type)
{
	Companion companion;
	companion.name = name;
	companion.type = type;

	// Now create a container for this companion to use as it's inventory space
	if (type == CompanionType::DONKEY)
		NewContainer(name, "Donkey", 9, std::vector<PlayerInventoryItem> {});

	else if (type == CompanionType::DOG)
		NewContainer(name, "Dog", 6, std::vector<PlayerInventoryItem> {});

	else if (type == CompanionType::CAT)
		NewContainer(name, "Cat", 3, std::vector<PlayerInventoryItem> {});

	s_compainions.push_back(companion);
}

void Inventory::NewContainer(std::string name, std::string iconName, int size, std::vector<PlayerInventoryItem> contents)
{
	Container container;
	container.name = name;
	container.iconName = iconName;
	container.contentsVector.resize(size);

	for (int i = 0; i < contents.size(); i++) {
		container.contentsVector[i].m_name = contents[i].m_name;
		container.contentsVector[i].m_quantity = contents[i].m_quantity;

		if (i >= container.contentsVector.size())
			break;
	}
	s_containers.push_back(container);
}

void Inventory::MoveContainerItem(std::string itemName, int newLocation)
{
	// Bail if no container selected
	if (!p_displayedContainer)
		return;
	// check new location is empty
	if (p_displayedContainer->contentsVector[newLocation].m_name == EMPTY_SLOT)
	{
		// find what the location of the original spot
		int oldLocation = -1;
		for (int i = 0; i < p_displayedContainer->contentsVector.size(); i++)
			if (p_displayedContainer->contentsVector[i].m_name == itemName)
				oldLocation = i;
		// If item was not found...
		if (oldLocation != -1)
		{
			// update new spot with those details to new slot
			p_displayedContainer->contentsVector[newLocation].m_name = p_displayedContainer->contentsVector[oldLocation].m_name;
			p_displayedContainer->contentsVector[newLocation].m_quantity = p_displayedContainer->contentsVector[oldLocation].m_quantity;

			// clear old spot
			p_displayedContainer->contentsVector[oldLocation].m_name = EMPTY_SLOT;
			p_displayedContainer->contentsVector[oldLocation].m_quantity = 0;
		}
	}
}

void Inventory::MoveInventoryItem(std::string itemName, int newLocation)
{
	// check new location is empty
	if (p_currentInventory[newLocation].m_name == EMPTY_SLOT)
	{
		// find what the location of the original spot
		int oldLocation = -1;
		for (int i = 0; i < INVENTORY_SIZE_LIMIT; i++)
			if (p_currentInventory[i].m_name == itemName)
				oldLocation = i;
		// If item was not found...
		if (oldLocation != -1)
		{
			// update new spot with those details to new slot
			p_currentInventory[newLocation].m_name = p_currentInventory[oldLocation].m_name;
			p_currentInventory[newLocation].m_quantity = p_currentInventory[oldLocation].m_quantity;

			// clear old spot
			p_currentInventory[oldLocation].m_name = EMPTY_SLOT;
			p_currentInventory[oldLocation].m_quantity = 0;
		}
	}
}

void Inventory::DepositItemsIntoContainer(std::string containerName, std::string itemName, int itemQuantity, int containerSlot)
{
	// find the matching container
	for (Container& container : s_containers) {
		if (container.name == containerName)
		{
			// find out if the conftainer already contains the same item
			for (int i = 0; i < container.contentsVector.size(); i++) {
				if (container.contentsVector[i].m_name == itemName)
				{
					// if so, add to the stack
					container.contentsVector[i].m_quantity += itemQuantity;
					TakeItem(itemName, itemQuantity);
					return;
				}
			}
			// otherwise, if the specificied slot is empty then add it there
			if (container.contentsVector[containerSlot].m_name == EMPTY_SLOT)
			{
				container.contentsVector[containerSlot].m_name = itemName;
				container.contentsVector[containerSlot].m_quantity = itemQuantity;
				TakeItem(itemName, itemQuantity);
				return;
			}
			// otherwise, add it to the first avaliable empty slot
			for (int i = 0; i < container.contentsVector.size(); i++) {
				if (container.contentsVector[i].m_name == EMPTY_SLOT) {
					container.contentsVector[i].m_name = itemName;
					container.contentsVector[i].m_quantity = itemQuantity;
					TakeItem(itemName, itemQuantity);
					return;
				}
			}
			return;
		}
	}
}
\
ReturnValue Inventory::WithdrawItemsFromContainer(std::string containerName, std::string itemName, int itemQuantity, int inventorySlot)
{
	// Bail if player does not enough carrying capacity
	float weight = 0;
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName)
			weight = inventoryItemData.m_weight * itemQuantity;
	if (GetCurrentTotalWeight() + weight > s_maxWeight)
		return ReturnValue::OVERWEIGHT;

	// Find the item type and it's inventory bag type
	InventoryType itemType = GetItemTypeByInventoryName(itemName);
	InventoryBagType targetInventoryBag = GetInventoryBagTypeByItemName(itemName);
	PlayerInventoryItem* playerInventory = GetCorrespondingInventoryArrayPointerByItemName(itemName);

	ReturnValue wasItemPlacementSuccessful = ReturnValue::UNDEFINED;

	// Find the matching container
	for (Container& container : s_containers) {
		if (container.name == containerName)
		{
			// If the corresponding inventory bag is open
			// and the specifed slot is empty
			// and the player does NOT already have the item,
			// then put it in the slot they specified
			if (targetInventoryBag == s_currentInventoryBagType && !HasItem(itemName) && playerInventory[inventorySlot].m_name == EMPTY_SLOT) {
				playerInventory[inventorySlot].m_name = itemName;
				playerInventory[inventorySlot].m_quantity = itemQuantity;
				wasItemPlacementSuccessful = ReturnValue::SUCCESS;
			}
			// Otherwise switch to the target bag and either stack the item or put in the first avaliable empty slot
			else {
				if (targetInventoryBag == InventoryBagType::GENERAL)
					SetCurrentInventoryBagToGeneral();
				else if (targetInventoryBag == InventoryBagType::CONSUMABLE)
					SetCurrentInventoryBagToConsumeable();
				else if (targetInventoryBag == InventoryBagType::EQUIPABLE)
					SetCurrentInventoryBagToEquipable();
				else if (targetInventoryBag == InventoryBagType::MATERIAL)
					SetCurrentInventoryBagToMaterial();
				else if (targetInventoryBag == InventoryBagType::QUEST)
					SetCurrentInventoryBagToQuest();
				else if (targetInventoryBag == InventoryBagType::WEARABLE)
					SetCurrentInventoryBagToWearable();

				wasItemPlacementSuccessful = GiveItem(itemName, itemQuantity);
			}
			// So now if the item was placed successfully into the players inventory....
			if (wasItemPlacementSuccessful == ReturnValue::SUCCESS) {
				// ... find the item in the container and remove the specified quantity
				for (int i = 0; i < container.contentsVector.size(); i++) {
					if (container.contentsVector[i].m_name == itemName) {

						container.contentsVector[i].m_quantity -= itemQuantity;

						// and if the quantity has reached 0 then remove the item
						if (container.contentsVector[i].m_quantity <= 0) {
							container.contentsVector[i].m_name = EMPTY_SLOT;
							container.contentsVector[i].m_quantity = 0;
						}

						return ReturnValue::SUCCESS;
					}
				}
				return ReturnValue::COULD_NOT_FIND_ITEM_IN_CONTAINER;
			}
			return wasItemPlacementSuccessful;
		}
	}
	return ReturnValue::COULD_NOT_FIND_CONTAINER_NAME;
}

PlayerInventoryItem* Inventory::GetCorrespondingInventoryArrayPointerByItemName(std::string name)
{
	InventoryBagType bagType = GetInventoryBagTypeByItemName(name);
	if (bagType == InventoryBagType::EQUIPABLE)
		return s_playerInventory_equipable;
	else if (bagType == InventoryBagType::CONSUMABLE)
		return s_playerInventory_consumable;
	else if (bagType == InventoryBagType::MATERIAL)
		return s_playerInventory_material;
	else if (bagType == InventoryBagType::QUEST)
		return s_playerInventory_quest;
	else if (bagType == InventoryBagType::WEARABLE)
		return s_playerInventory_wearable;
	else
		return s_playerInventory_general;
}

InventoryBagType Inventory::GetInventoryBagTypeByItemName(std::string name)
{
	InventoryType type = GetItemTypeByInventoryName(name);
	if (type == InventoryType::EQUIPABLE)
		return InventoryBagType::EQUIPABLE;
	else if (type == InventoryType::CONSUMABLE)
		return InventoryBagType::CONSUMABLE;
	else if (type == InventoryType::MATERIAL)
		return InventoryBagType::MATERIAL;
	else if (type == InventoryType::QUEST)
		return InventoryBagType::QUEST;
	else if (type == InventoryType::WEARABLE_BELT
		|| type == InventoryType::WEARABLE_FEET
		|| type == InventoryType::WEARABLE_HANDS
		|| type == InventoryType::WEARABLE_HEAD
		|| type == InventoryType::WEARABLE_LOWER_BODY
		|| type == InventoryType::WEARABLE_UPPER_BODY
		|| type == InventoryType::WEARABLE_BELT)
		return InventoryBagType::WEARABLE;
	else
		return InventoryBagType::GENERAL;
}


void Inventory::SaveInventoryDatabase()
{
	std::string filename = "InventoryDatabase.txt";

	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value inventoryArray(rapidjson::kArrayType);
	document.SetObject();

	for (InventoryItemData& item : s_InventoryDatabase)
	{
		rapidjson::Value object(rapidjson::kObjectType);

		SaveString(&object, "Name", item.m_name, allocator);
		SaveString(&object, "Type", InventoryTypeToString(item.m_type), allocator);
		SaveFloat(&object, "Weight", item.m_weight, allocator);
		SaveString(&object, "Description", item.m_description, allocator);

		// Modifiers
		rapidjson::Value modifiersObject;
		modifiersObject.SetObject();
		for (Modifier& modifier : item.m_modifiers) {
			SaveString(&modifiersObject, "Type", ModiferTypeToString(modifier.m_type), allocator);
			SaveInt(&modifiersObject, "Value", modifier.m_value, allocator);
		}
		object.AddMember("Effect", modifiersObject, allocator);

		inventoryArray.PushBack(object, allocator);
	}

	document.AddMember("InventoryDatabase", inventoryArray, allocator);

	// Convert JSON document to string
	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	// Save it
	std::string data = strbuf.GetString();
	std::ofstream out("res/" + filename);
	out << data;
	out.close();
}

void Inventory::SaveString(rapidjson::Value* object, std::string elementName, std::string string, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetString(string.c_str(), static_cast<rapidjson::SizeType>(string.length()), allocator);
	object->AddMember(name, value, allocator);
}

void Inventory::SaveFloat(rapidjson::Value* object, std::string elementName, float number, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetFloat(number);
	object->AddMember(name, value, allocator);
}

void Inventory::SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetInt(number);
	object->AddMember(name, value, allocator);
}

std::string Inventory::InventoryTypeToString(InventoryType query)
{
	if (query == InventoryType::WEARABLE_HEAD) return "WEARABLE_HEAD";
	else if (query == InventoryType::WEARABLE_UPPER_BODY) return "WEARABLE_UPPER_BODY";
	else if (query == InventoryType::WEARABLE_LOWER_BODY) return "WEARABLE_LOWER_BODY";
	else if (query == InventoryType::WEARABLE_HANDS) return "WEARABLE_HANDS";
	else if (query == InventoryType::WEARABLE_FEET) return "WEARABLE_FEET";
	else if (query == InventoryType::WEARABLE_BELT) return "WEARABLE_BELT";
	else if (query == InventoryType::EQUIPABLE) return "EQUIPABLE";
	else if (query == InventoryType::MATERIAL) return "MATERIAL";
	else if (query == InventoryType::CONSUMABLE) return "CONSUMABLE";
	else if (query == InventoryType::GENERAL) return "GENERAL";
	else if (query == InventoryType::QUEST) return "QUEST";
	else return "UNDEFINED";
}

std::string Inventory::ModiferTypeToString(ModifierType query)
{
	if (query == ModifierType::SPEED) return "SPEED";
	else if (query == ModifierType::LOCK_PICKING) return "LOCK_PICKING";
	else return "NONE";
}



void Inventory::TakeItem(std::string name, int quantity)
{
	// First find the item type and select the appropriate array and slot limit
	InventoryType type = GetItemTypeByInventoryName(name);

	// First set it to consuermable inventory, then check if this is wrong.... This prevents "potentially uninitilazed local variable" error
	PlayerInventoryItem* playerInventory = s_playerInventory_consumable;
	int sizeLimit = m_max_consumable_slots;

	if (type == InventoryType::EQUIPABLE) {
		playerInventory = s_playerInventory_equipable;
		sizeLimit = m_max_equipable_slots;
	}
	else if (type == InventoryType::GENERAL) {
		playerInventory = s_playerInventory_general;
		sizeLimit = m_max_general_slots;
	}
	else if (type == InventoryType::MATERIAL) {
		playerInventory = s_playerInventory_material;
		sizeLimit = m_max_material_slots;
	}
	else if (type == InventoryType::QUEST) {
		playerInventory = s_playerInventory_quest;
		sizeLimit = m_max_quest_slots;
	}
	else if (type != InventoryType::UNDEFINED) {
		playerInventory = s_playerInventory_wearable;
		sizeLimit = m_max_wearable_slots;
	}

	// iterate over player inventory
	for (size_t i = 0; i < sizeLimit; i++)
	{
		// find the matching name
		if (playerInventory[i].m_name == name)
		{
			// if there enough of the item then remove that specified quantity
			if (playerInventory[i].m_quantity <= quantity)
				playerInventory[i].m_quantity -= quantity;
			// If that was the last one of the item then remove it from the players inventory entirely
			if (playerInventory[i].m_quantity >= 0) {
				playerInventory[i].m_name = EMPTY_SLOT;
				playerInventory[i].m_quantity = 0;
			}
			return;
		}
	}
}

ReturnValue Inventory::GiveItem(std::string name, int quantity)
{
	// Do you have enough weight capacity?
	float weight = 0;
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == name)
			weight = inventoryItemData.m_weight * quantity;
	if (GetCurrentTotalWeight() + weight > s_maxWeight)
		return ReturnValue::OVERWEIGHT;

	// First find the item type and select the appropriate array
	InventoryType item_type = GetItemTypeByInventoryName(name);
	if (item_type == InventoryType::UNDEFINED)
		return ReturnValue::UNDEFINED_ITEM;

	// First set it to consuermable inventory, then check if this is wrong.... This prevents "potentially uninitilazed local variable" error
	PlayerInventoryItem* playerInventory = s_playerInventory_consumable;
	int sizeLimit = m_max_consumable_slots;

	if (item_type == InventoryType::EQUIPABLE) {
		playerInventory = s_playerInventory_equipable;
		sizeLimit = m_max_equipable_slots;
	}
	else if (item_type == InventoryType::GENERAL) {
		playerInventory = s_playerInventory_general;
		sizeLimit = m_max_general_slots;
	}
	else if (item_type == InventoryType::MATERIAL) {
		playerInventory = s_playerInventory_material;
		sizeLimit = m_max_material_slots;
	}
	else if (item_type == InventoryType::QUEST) {
		playerInventory = s_playerInventory_quest;
		sizeLimit = m_max_quest_slots;
	}
	else if (item_type == InventoryType::WEARABLE_BELT ||
		item_type == InventoryType::WEARABLE_FEET ||
		item_type == InventoryType::WEARABLE_HANDS ||
		item_type == InventoryType::WEARABLE_HEAD ||
		item_type == InventoryType::WEARABLE_LOWER_BODY ||
		item_type == InventoryType::WEARABLE_UPPER_BODY ||
		item_type == InventoryType::WEARABLE_BELT) {
		playerInventory = s_playerInventory_wearable;
		sizeLimit = m_max_wearable_slots;
	}

	// First see if you have the item already. Cause then you can just add to the quantity.
	for (size_t i = 0; i < sizeLimit; i++) {
		if (playerInventory[i].m_name == name) {
			playerInventory[i].m_quantity += quantity;
			return ReturnValue::SUCCESS;
		}
	}
	// Otherwise put it in first empty slot
	for (size_t i = 0; i < sizeLimit; i++) {
		if (playerInventory[i].m_name == EMPTY_SLOT) {
			playerInventory[i].m_name = name;
			playerInventory[i].m_quantity = quantity;
			return ReturnValue::SUCCESS;
		}
	}
	return ReturnValue::NO_FREE_SLOTS;
	//std::cout << "SIZE LIMIT: " << sizeLimit << "\n";
	//std::cout << "Failed adding to inventory: " << name << " of type " << InventoryTypeToString(item_type) <<  "\n";
}

int Inventory::GetItemQuantity(std::string name)
{
	// Check every inventory to find the item and report the quantity
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_consumable)
		if (inventoryItem.m_name == name)
			return inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_equipable)
		if (inventoryItem.m_name == name)
			return inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_general)
		if (inventoryItem.m_name == name)
			return inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_material)
		if (inventoryItem.m_name == name)
			return inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_quest)
		if (inventoryItem.m_name == name)
			return inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_wearable)
		if (inventoryItem.m_name == name)
			return inventoryItem.m_quantity;
	return -1;
}

bool Inventory::HasItem(std::string name)
{
	if (GetItemQuantity(name) > 0)
		return true;
	else
		return false;
}

/*
std::string Inventory::GetGameItemNameByIndex(int index)
{
	if (index < 0 || index >= (int)s_maxInventorySlots)
		return "out_of_range";

	return s_playerInventory[index].m_name;
}*/


void Inventory::PrintCurrentContainer()
{
	if (!p_displayedContainer)
	{
		std::cout << "NO CONTAINER SELECTED!\n\n";
		return;
	}

	std::cout << "CONTAINER " << p_displayedContainer->name << "\n";

	for (int i = 0; i < p_displayedContainer->contentsVector.size(); i++)
		std::cout << " [" << i << "] " << p_displayedContainer->contentsVector[i].m_name << " (" << p_displayedContainer->contentsVector[i].m_quantity << ")\n";
	std::cout << "\n";
}

void Inventory::PrintContainerContents()
{
	for (Container container : s_containers)
	{
		std::cout << "CONTAINER " << container.name << "\n";

		for (int i = 0; i < container.contentsVector.size(); i++)
			std::cout << " [" << i << "] " << container.contentsVector[i].m_name << " (" << container.contentsVector[i].m_quantity << ")\n";

		std::cout << "\n";
	}
}

void Inventory::PrintContainerContents(std::string containerName)
{
	for (Container container : s_containers)
	{
		if (container.name == containerName) {
			std::cout << "CONTAINER " << container.name << "\n";
			for (int i = 0; i < container.contentsVector.size(); i++)
				std::cout << " [" << i << "] " << container.contentsVector[i].m_name << " (" << container.contentsVector[i].m_quantity << ")\n";
			std::cout << "\n";
		}
	}
}

void Inventory::PrintContainerContents(int i)
{
	std::cout << "CONTAINER " << s_containers[i].name << "\n";
	for (int j = 0; j < s_containers[i].contentsVector.size(); j++)
		std::cout << " [" << j << "] " << s_containers[i].contentsVector[j].m_name << " (" << s_containers[i].contentsVector[j].m_quantity << ")\n";

	std::cout << "\n";

}

void Inventory::PrintInventoryDatabase()
{
	std::cout << "ITEM DATABASE \n";
	for (int i = 0; i < s_InventoryDatabase.size(); i++)
		std::cout << " " << s_InventoryDatabase[i].m_name << "\n";
	std::cout << "\n";
}

void Inventory::PrintPlayerInventoryList()
{
	//std::cout << "PLAYER INVENTORY \n";
	std::cout << " max weight:     " << s_maxWeight << "kg\n";
	std::cout << " current weight: " << GetCurrentTotalWeight() << "kg\n\n";

	std::cout << " GENERAL INVENTORY\n";
	for (int i = 0; i < m_max_general_slots; i++)
		if (s_playerInventory_general[i].m_name != EMPTY_SLOT)
			std::cout << "  [" << i << "] " << s_playerInventory_general[i].m_name << " (" << s_playerInventory_general[i].m_quantity << ")\n";
		else
			std::cout << "  [" << i << "] " << s_playerInventory_general[i].m_name << "\n";
	std::cout << "\n";

	std::cout << " CONSUMERABLE INVENTORY\n";
	for (int i = 0; i < m_max_consumable_slots; i++)
		if (s_playerInventory_consumable[i].m_name != EMPTY_SLOT)
			std::cout << "  [" << i << "] " << s_playerInventory_consumable[i].m_name << " (" << s_playerInventory_consumable[i].m_quantity << ")\n";
		else
			std::cout << "  [" << i << "] " << s_playerInventory_consumable[i].m_name << "\n";
	std::cout << "\n";

	std::cout << " EQUIPABLE INVENTORY\n";
	for (int i = 0; i < m_max_equipable_slots; i++)
		if (s_playerInventory_equipable[i].m_name != EMPTY_SLOT)
			std::cout << "  [" << i << "] " << s_playerInventory_equipable[i].m_name << " (" << s_playerInventory_equipable[i].m_quantity << ")\n";
		else
			std::cout << "  [" << i << "] " << s_playerInventory_equipable[i].m_name << "\n";
	std::cout << "\n";

	std::cout << " MATERIAL INVENTORY\n";
	for (int i = 0; i < m_max_material_slots; i++)
		if (s_playerInventory_material[i].m_name != EMPTY_SLOT)
			std::cout << "  [" << i << "] " << s_playerInventory_material[i].m_name << " (" << s_playerInventory_material[i].m_quantity << ")\n";
		else
			std::cout << "  [" << i << "] " << s_playerInventory_material[i].m_name << "\n";
	std::cout << "\n";

	std::cout << " QUEST INVENTORY\n";
	for (int i = 0; i < m_max_quest_slots; i++)
		if (s_playerInventory_quest[i].m_name != EMPTY_SLOT)
			std::cout << "  [" << i << "] " << s_playerInventory_quest[i].m_name << " (" << s_playerInventory_quest[i].m_quantity << ")\n";
		else
			std::cout << "  [" << i << "] " << s_playerInventory_quest[i].m_name << "\n";
	std::cout << "\n";

	std::cout << " WEARABLE INVENTORY\n";
	for (int i = 0; i < m_max_wearable_slots; i++)
		if (s_playerInventory_wearable[i].m_name != EMPTY_SLOT)
			std::cout << "  [" << i << "] " << s_playerInventory_wearable[i].m_name << " (" << s_playerInventory_wearable[i].m_quantity << ")\n";
		else
			std::cout << "  [" << i << "] " << s_playerInventory_wearable[i].m_name << "\n";
	std::cout << "\n";
}

void Inventory::PrintEquippedItems()
{
	std::cout << "EQUIPED ITEMS \n";
	std::cout << " Head:       " << s_equippedItems.m_head << "\n";
	std::cout << " Upperbody:  " << s_equippedItems.m_upperBody << "\n";
	std::cout << " Lowerbody:  " << s_equippedItems.m_lowerBody << "\n";
	std::cout << " Belt:       " << s_equippedItems.m_belt << "\n";
	std::cout << " Hands:      " << s_equippedItems.m_hands << "\n";
	std::cout << " Feet:       " << s_equippedItems.m_feet << "\n";
	std::cout << " Equip slot: " << s_equippedItems.m_equipped << "\n\n";
}

void Inventory::EquipToHead(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::WEARABLE_HEAD)
			s_equippedItems.m_head = itemName;
}
void Inventory::EquipToFeet(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::WEARABLE_FEET)
			s_equippedItems.m_feet = itemName;
}
void Inventory::EquipToBelt(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::WEARABLE_BELT) {
			s_equippedItems.m_belt = itemName;
		}
}
void Inventory::EquipToHands(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::WEARABLE_HANDS)
			s_equippedItems.m_hands = itemName;
}
void Inventory::EquipToLowerBody(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::WEARABLE_LOWER_BODY)
			s_equippedItems.m_lowerBody = itemName;
}
void Inventory::EquipToUpperBody(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::WEARABLE_UPPER_BODY)
			s_equippedItems.m_upperBody = itemName;
}
void Inventory::EquipToEquipSlot(std::string itemName)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == itemName && inventoryItemData.m_type == InventoryType::EQUIPABLE)
			s_equippedItems.m_equipped = itemName;
}

float Inventory::GetCurrentTotalWeight()
{
	float weight = 0;

	for (PlayerInventoryItem& inventoryItem : s_playerInventory_consumable)
		weight += GetWeightByItemName(inventoryItem.m_name) * inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_equipable)
		weight += GetWeightByItemName(inventoryItem.m_name) * inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_general)
		weight += GetWeightByItemName(inventoryItem.m_name) * inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_material)
		weight += GetWeightByItemName(inventoryItem.m_name) * inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_quest)
		weight += GetWeightByItemName(inventoryItem.m_name) * inventoryItem.m_quantity;
	for (PlayerInventoryItem& inventoryItem : s_playerInventory_wearable)
		weight += GetWeightByItemName(inventoryItem.m_name) * inventoryItem.m_quantity;

	return weight;
}

float Inventory::GetWeightByItemName(std::string name)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == name)
			return inventoryItemData.m_weight;
	return 0;
}

void Inventory::ClearAllPlayerInventory()
{
	for (size_t i = 0; i < INVENTORY_SIZE_LIMIT; i++) {
		s_playerInventory_wearable[i].m_name = EMPTY_SLOT;
		s_playerInventory_wearable[i].m_quantity = 0;
		s_playerInventory_equipable[i].m_name = EMPTY_SLOT;
		s_playerInventory_equipable[i].m_quantity = 0;
		s_playerInventory_material[i].m_name = EMPTY_SLOT;
		s_playerInventory_material[i].m_quantity = 0;
		s_playerInventory_consumable[i].m_name = EMPTY_SLOT;
		s_playerInventory_consumable[i].m_quantity = 0;
		s_playerInventory_general[i].m_name = EMPTY_SLOT;
		s_playerInventory_general[i].m_quantity = 0;
		s_playerInventory_quest[i].m_name = EMPTY_SLOT;
		s_playerInventory_quest[i].m_quantity = 0;
	}
}

InventoryType Inventory::GetItemTypeByInventoryName(std::string name)
{
	for (InventoryItemData& inventoryItemData : s_InventoryDatabase)
		if (inventoryItemData.m_name == name)
			return inventoryItemData.m_type;

	return InventoryType::UNDEFINED;
}

void Inventory::SetGeneralInventoryBagSize(int size) {
	m_max_general_slots = size;
}
void Inventory::SetWearableInventoryBagSize(int size) {
	m_max_wearable_slots = size;
}
void Inventory::SetEquipableInventoryBagSize(int size) {
	m_max_equipable_slots = size;
}
void Inventory::SetMaterialInventoryBagSize(int size) {
	m_max_material_slots = size;
}
void Inventory::SetConsumableInventoryBagSize(int size) {
	m_max_consumable_slots = size;
}
void Inventory::SetQuestInventoryBagSize(int size) {
	m_max_quest_slots = size;
}
int Inventory::GetGeneralInventoryBagSize() {
	return m_max_general_slots;
}
int Inventory::GetWearableInventoryBagSize() {
	return m_max_wearable_slots;
}
int Inventory::GetEquipableInventoryBagSize() {
	return m_max_equipable_slots;
}
int Inventory::GetMaterialInventoryBagSize() {
	return m_max_material_slots;
}
int Inventory::GetConsumableInventoryBagSize() {
	return m_max_consumable_slots;
}
int Inventory::GetQuestInventoryBagSize() {
	return m_max_quest_slots;
}
bool Inventory::IsGeneralInventoryBagOpen() {
	return s_currentInventoryBagType == InventoryBagType::GENERAL;
}
bool Inventory::IsWearableInventoryBagOpen() {
	return s_currentInventoryBagType == InventoryBagType::WEARABLE;
}
bool Inventory::IsEquipableInventoryBagOpen() {
	return s_currentInventoryBagType == InventoryBagType::EQUIPABLE;
}
bool Inventory::IsMaterialInventoryBagOpen() {
	return s_currentInventoryBagType == InventoryBagType::MATERIAL;
}
bool Inventory::IsConsumableInventoryBagOpen() {
	return s_currentInventoryBagType == InventoryBagType::CONSUMABLE;
}
bool Inventory::IsQuestInventoryBagOpen() {
	return s_currentInventoryBagType == InventoryBagType::QUEST;
}
int Inventory::GetCurrentContainerSize() {
	if (p_displayedContainer)
		return p_displayedContainer->contentsVector.size();
	else
		return -1;
}
int Inventory::GetCurrentInventoryBagItemQuantityByindex(int index) {
	if (p_currentInventory && index > 0)
		return p_currentInventory[index].m_quantity;
	else
		return -1;
}
std::string Inventory::GetInventoryBagItemNameByindex(int index) {
	if (p_currentInventory && index > 0)
		return p_currentInventory[index].m_name;
	else
		return "cannot access inventory";
}
int Inventory::GetCurrentContainerItemQuantityByindex(int index) {
	if (p_displayedContainer && index > 0 && index < p_displayedContainer->contentsVector.size())
		return p_displayedContainer->contentsVector[index].m_quantity;
	else
		return -1;
}
std::string Inventory::GetCurrentContainerItemNameByindex(int index) {
	if (p_displayedContainer && index > 0 && index < p_displayedContainer->contentsVector.size())
		return p_displayedContainer->contentsVector[index].m_name;
	else
		return "Current container pointer is null";
}

int Inventory::GetContainerSizeByName(std::string containerName)
{
	for (Container& container : s_containers)
		if (container.name == containerName)
			return container.contentsVector.size();
	return -1;
}

std::string Inventory::GetItemDescriptionByName(std::string name)
{
	for (InventoryItemData& item : s_InventoryDatabase)
		if (item.m_name == name)
			return item.m_description;
	return "Item name not found";
}

int Inventory::GetItemNumberOfModifierEffectsByName(std::string name)
{
	for (InventoryItemData& item : s_InventoryDatabase)
		if (item.m_name == name)
			return item.m_modifiers.size();
	return 0;
}

std::string Inventory::GetItemModiferEffectNameByIndex(std::string name, int index)
{
	for (InventoryItemData& item : s_InventoryDatabase)
		if (item.m_name == name)
			return Util::GetModifierTypeNameAsString(item.m_modifiers[index].m_type);
	return "ITEM NAME NOT FOUND";
}


int Inventory::GetItemModiferEffectValueByIndex(std::string name, int index)
{
	for (InventoryItemData& item : s_InventoryDatabase)
		if (item.m_name == name)
			return item.m_modifiers[index].m_value;
	return -1;
}

std::string Inventory::GetItemTypeAsStringByName(std::string itemName)
{
	for (InventoryItemData& item : s_InventoryDatabase)
		if (item.m_name == itemName)
			return Util::GetItemTypeAsString(item.m_type);
	return "Could not find item";
}

std::string Inventory::GetCurrentContainerItemNameByIndex(int index)
{
	return Inventory::p_displayedContainer->contentsVector[index].m_name;
}

int Inventory::GetCurrentContainerItemQuantityByIndex(int index)
{
	return Inventory::p_displayedContainer->contentsVector[index].m_quantity;
}

std::string Inventory::GetCurrentBagItemNameByIndex(int index)
{
	return p_currentInventory[index].m_name;
}
int Inventory::GetCurrentBagIteQuantityByIndex(int index)
{
	return p_currentInventory[index].m_quantity;
}