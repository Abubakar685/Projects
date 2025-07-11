#include <iostream>
#include <string>
using namespace std;

class TreeNode {
	public:
    int id;
    string status;
    string type;
    TreeNode* left; 
    TreeNode* right; 

   TreeNode(int key, string Status, string Type) {
    id = key;
    status = Status;
    type = Type;
    left = nullptr;
    right = nullptr;
	}
};

class BST {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, int id, const string& status, const string& type) {
        if (!node) return new TreeNode(id, status, type);

        if (id < node->id)
            node->left = insert(node->left, id, status, type);
        else if (id > node->id)
            node->right = insert(node->right, id, status, type);
        return node;
    }

    TreeNode* search(TreeNode* node, int id) {
        if (!node || node->id == id)
            return node;

        if (id < node->id)
            return search(node->left, id);
        else
            return search(node->right, id);
    }

    void inorder(TreeNode* node) {
        if (!node) {
			return;}
        inorder(node->left);
        cout << "Room " << node->id << " (" << node->type << ") - " << node->status << "\n";
        inorder(node->right);
    }

public:
    BST() {
    root = nullptr;
	}

    void insert(int id, const string& status, const string& type) {
        root = insert(root, id, status, type);
    }

    TreeNode* search(int id) {
        return search(root, id);
    }

    void display() {
        inorder(root);
    }
};

struct Room {
    int id;
    string type;
    string status; // Can be "Occupied", "Booked", "Ready", or "Unavailable"
    bool availability[30] = {true};  // Simplified initialization to true for all 30 days
    Room* next;
};

struct Floor {
    int floorNumber;
    Room* roomHead;
    Floor* next;
};

struct Request {
    int id;
    string name;
    string roomType;
    int nights;
    Request* next;
    int priority;
};

class HotelManagementSystem {
private:
    Floor* hotelHead;
    Request* queueFront;
    Request* queueRear;
    int queueSize;

    struct BookingHistory {
        int id;
        string name;
        int roomId;
        string roomType;
        int nights;
        int floorNumber; // Added floor number to booking history
        BookingHistory* next;
    };

    BookingHistory* historyTop;
    BST roomBST;

    Floor* createFloor(int floorNumber) {
        Floor* newFloor = new Floor;
        newFloor->floorNumber = floorNumber;
        newFloor->roomHead = nullptr;
        newFloor->next = nullptr;

        Room* prevRoom = nullptr;
        system("cls");
        cout << "\n\n----- Floor " << newFloor->floorNumber << " -----\n\n";

        int roomsPerFloor;
        while (true) {
            cout << "Enter number of rooms for floor " << floorNumber << ": ";
            if (cin >> roomsPerFloor && roomsPerFloor > 0) {
                break;
            }
            cout << "Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        for (int i = 1; i <= roomsPerFloor; i++) {
            Room* newRoom = new Room;
            newRoom->id = i;
            newRoom->type = "";
            newRoom->status = "Ready"; // Initially all rooms are Ready
            for (int j = 0; j < 30; ++j) {
                newRoom->availability[j] = true;  // Set each day as available
            }
            newRoom->next = nullptr;

            bool validRoomType = false;
            while (!validRoomType) {
                cout << "Enter room type for room " << i << " (Single/Double/Suite): ";
                cin >> newRoom->type;

                // Convert room type to lowercase for comparison
                for (char& c : newRoom->type) {
                    c = tolower(c);
                }

                // Check if the entered room type is valid
                if (newRoom->type == "single" || newRoom->type == "double" || newRoom->type == "suite") {
                    validRoomType = true; // Valid input, break the loop
                } else {
                    cout << "Invalid room type entered. Please enter 'Single', 'Double', or 'Suite'." << endl;
                }
            }

            // Ask for initial room status
            bool validStatus = false;
            while (!validStatus) {
                cout << "Enter initial room status (Ready/Unavailable): ";
                string initialStatus;
                cin >> initialStatus;

                // Convert status to proper case
                for (char& c : initialStatus) {
                    c = tolower(c);
                }
                initialStatus[0] = toupper(initialStatus[0]);

                if (initialStatus == "Ready" || initialStatus == "Unavailable") {
                    newRoom->status = initialStatus;
                    validStatus = true;
                } else {
                    cout << "Invalid status. Please enter either 'Ready' or 'Unavailable'." << endl;
                }
            }
            	system("cls");

            // Insert room into BST
            roomBST.insert(newRoom->id, newRoom->status, newRoom->type);

            if (!newFloor->roomHead) {
                newFloor->roomHead = newRoom;
            } else {
                prevRoom->next = newRoom;
            }

            prevRoom = newRoom;
        }

        return newFloor;
    }

public:
    HotelManagementSystem(int floors) : hotelHead(nullptr), queueFront(nullptr), queueRear(nullptr), historyTop(nullptr), queueSize(0) {
        Floor* prevFloor = nullptr;
        for (int i = 1; i <= floors; ++i) {
            Floor* newFloor = createFloor(i);
            if (!hotelHead) hotelHead = newFloor;
            else prevFloor->next = newFloor;
            prevFloor = newFloor;
        }
    }

    void enqueueRequest() {
    	system("cls");
        Request* newRequest = new Request;
        while (true) {
            cout << "Enter request ID: ";
            if (cin >> newRequest->id && newRequest->id > 0) {
                break;
            }
            cout << "Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore();
        cout << "Enter customer name: ";
        getline(cin, newRequest->name);

        bool validRoomType = false;
        while (!validRoomType) {
            cout << "Enter room type (Single/Double/Suite): ";
            cin >> newRequest->roomType;

            // Convert room type to lowercase for comparison
            for (char& c : newRequest->roomType) {
                c = tolower(c);
            }

            // Check if the entered room type is valid
            if (newRequest->roomType == "single" || newRequest->roomType == "double" || newRequest->roomType == "suite") {
                validRoomType = true; // Valid input, break the loop
            } else {
                cout << "Invalid room type entered. Please enter 'Single', 'Double', or 'Suite'." << endl;
            }
        }

        while (true) {
            cout << "Enter number of nights: ";
            if (cin >> newRequest->nights && newRequest->nights > 0) {
                break;
            }
            cout << "Invalid input. Please enter a positive number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        while (true) {
            cout << "Enter priority (0 for regular, 1 for high priority): ";
            if (cin >> newRequest->priority && (newRequest->priority == 0 || newRequest->priority == 1)) {
                break;
            }
            cout << "Invalid input. Please enter 0 or 1.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        newRequest->next = nullptr;

        if (!queueFront || newRequest->priority == 1) {
            newRequest->next = queueFront;
            queueFront = newRequest;
            if (!queueRear) queueRear = newRequest;
        } else {
            queueRear->next = newRequest;
            queueRear = newRequest;
        }
        queueSize++;
    }

    void processRequests() {
        system("cls");
		cout << "Processing requests...\n";
        if (!queueFront) {
            cout << "No booking requests available to process.\n";
            return;
        }
        while (queueFront) {
            Request* current = queueFront;
            queueFront = queueFront->next;
            queueSize--;

            bool roomTypeExists = false;
            bool roomFound = false;
            bool roomAvailable = false;

            cout << "\nProcessing Request ID: " << current->id << "\n";

            // First check if room type exists
            for (Floor* floor = hotelHead; floor; floor = floor->next) {
                for (Room* room = floor->roomHead; room; room = room->next) {
                    if (room->type == current->roomType) {
                        roomTypeExists = true;
                        if (room->status == "Ready") {
                            roomFound = true;
                            // Check availability for requested nights
                            bool available = true;
                            for (int i = 0; i < current->nights; ++i) {
                                if (!room->availability[i]) {
                                    available = false;
                                    break;
                                }
                            }
                            if (available) {
                                roomAvailable = true;
                                break;
                            }
                        }
                    }
                }
                if (roomAvailable) break;
            }

            Room* allocatedRoom = allocateRoom(current->roomType, current->nights);
            if (allocatedRoom) {
                // Set status based on immediate occupancy or future booking
                char checkIn;
                while (true) {
                    cout << "Is the customer checking in now? (y/n): ";
                    if (cin >> checkIn && (tolower(checkIn) == 'y' || tolower(checkIn) == 'n')) {
                        break;
                    }
                    cout << "Invalid input. Please enter y or n.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }

                if (tolower(checkIn) == 'y') {
                    allocatedRoom->status = "Occupied";
                } else {
                    allocatedRoom->status = "Booked";
                    int checkInDate;
                    while (true) {
                        cout << "Enter check-in date (1-30): ";
                        if (cin >> checkInDate && checkInDate >= 1 && checkInDate <= 30) {
                            break;
                        }
                        cout << "Invalid date. Please enter a date between 1 and 30.\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }

                    // Mark the room as unavailable for the specified dates
                    for (int i = checkInDate - 1; i < checkInDate - 1 + current->nights && i < 30; i++) {
                        allocatedRoom->availability[i] = false;
                    }
                }

                // Find floor number for the allocated room
                Floor* currentFloor = hotelHead;
                int floorNum = 1;
                while (currentFloor) {
                    Room* room = currentFloor->roomHead;
                    while (room) {
                        if (room == allocatedRoom) {
                            cout << "Booking successful for Request ID " << current->id << " - " << current->name << " in Floor " << floorNum << ", Room ID " << allocatedRoom->id << "\n";
                            
                            // Add booking to history with floor number
                            BookingHistory* newHistory = new BookingHistory{
                                current->id, 
                                current->name, 
                                allocatedRoom->id, 
                                allocatedRoom->type, 
                                current->nights,
                                floorNum, // Store the floor number
                                historyTop
                            };
                            historyTop = newHistory;
                            break;
                        }
                        room = room->next;
                    }
                    if (room) break;
                    currentFloor = currentFloor->next;
                    floorNum++;
                }
            } else {
                cout << "No room available for Request ID " << current->id << " - " << current->name << "\n";
                if (!roomTypeExists) {
                    cout << "Reason: The requested room type '" << current->roomType << "' does not exist in the hotel.\n";
                } else if (!roomFound) {
                    cout << "Reason: All rooms of type '" << current->roomType << "' are currently occupied or unavailable.\n";
                } else {
                    cout << "Reason: No rooms of type '" << current->roomType << "' are available for the requested " << current->nights << " nights.\n";
                }
            }

            // Delete the request after processing
            delete current;
        }
    }

    Room* allocateRoom(const string& roomType, int nights) {
        for (Floor* floor = hotelHead; floor; floor = floor->next) {
            for (Room* room = floor->roomHead; room; room = room->next) {
                if (room->type == roomType && (room->status == "Ready")) {
                    bool available = true;
                    for (int i = 0; i < nights; ++i) {
                        if (!room->availability[i]) {
                            available = false;
                            break;
                        }
                    }
                    if (available) {
                        // Mark the room as booked for the requested nights
                        for (int i = 0; i < nights; ++i) {
                            room->availability[i] = false;
                        }
                        return room;
                    }
                }
            }
        }
        return nullptr;
    }

    void cancelBooking() {
    	system("cls");
        if (!historyTop) {
            cout << "No bookings to cancel.\n";
            return;
        }
        BookingHistory* topBooking = historyTop;
        historyTop = historyTop->next;

        for (Floor* floor = hotelHead; floor; floor = floor->next) {
            for (Room* room = floor->roomHead; room; room = room->next) {
                if (room->id == topBooking->roomId) {
                    room->status = "Ready";
                    // Restore availability for all 30 days
                    for (int i = 0; i < topBooking->nights; i++) {
                        room->availability[i] = true;
                    }
                    cout << "Cancelled booking for Request ID " << topBooking->id << " - " << topBooking->name << " in Room ID " << topBooking->roomId << "\n";
                    delete topBooking;
                    return;
                }
            }
        }
    }

    void displayHistory() {
    	system("cls");
        if (!historyTop) {
            cout << "No booking history available.\n";
            return;
        }

        for (BookingHistory* current = historyTop; current; current = current->next) {
            cout << "Booking Found: Request ID " << current->id 
                 << " - " << current->name 
                 << " - Floor " << current->floorNumber 
                 << ", Room " << current->roomId 
                 << " (" << current->roomType << "), " 
                 << current->nights << " nights.\n";
        }
    }

    void displayRoomStatus() {
    	system("cls");
        for (Floor* floor = hotelHead; floor; floor = floor->next) {
            cout << "\nFloor " << floor->floorNumber << ":\n";
            for (Room* room = floor->roomHead; room; room = room->next) {
                cout << "Room " << room->id << " (" << room->type << ") - " << room->status << "\n";
            }
        }
    }

    ~HotelManagementSystem() {
        while (queueFront) {
            Request* temp = queueFront;
            queueFront = queueFront->next;
            delete temp;
        }
        while (historyTop) {
            BookingHistory* temp = historyTop;
            historyTop = historyTop->next;
            delete temp;
        }
        while (hotelHead) {
            Floor* tempFloor = hotelHead;
            hotelHead = hotelHead->next;
            while (tempFloor->roomHead) {
                Room* tempRoom = tempFloor->roomHead;
                tempFloor->roomHead = tempFloor->roomHead->next;
                delete tempRoom;
            }
            delete tempFloor;
        }
    }

    void searchBooking() {
    	system("cls");
        if (!historyTop) {
            cout << "No booking history available.\n";
            return;
        }

        int choice;
        while (true) {
            cout << "\nSearch Booking by:\n1. ID\n2. Name\nEnter choice: ";
            if (cin >> choice && (choice == 1 || choice == 2)) {
                break;
            }
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore();

        if (choice == 1) {
            int searchId;
            while (true) {
                cout << "Enter Booking ID: ";
                if (cin >> searchId && searchId > 0) {
                    break;
                }
                cout << "Invalid input. Please enter a positive number.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            BookingHistory* current = historyTop;
            bool found = false;
            while (current) {
                if (current->id == searchId) {
                    cout << "Booking Found: Request ID " << current->id 
                         << " - " << current->name 
                         << " - Floor " << current->floorNumber 
                         << ", Room " << current->roomId 
                         << " (" << current->roomType << "), " 
                         << current->nights << " nights.\n";
                    found = true;
                }
                current = current->next;
            }
            if (!found) {
                cout << "No booking found with ID " << searchId << "\n";
            }
        } else if (choice == 2) {
            string searchName;
            cout << "Enter Customer Name: ";
            getline(cin, searchName);

            BookingHistory* current = historyTop;
            bool found = false;
            while (current) {
                if (current->name == searchName) {
                    cout << "Booking Found: Request ID " << current->id 
                         << " - Floor " << current->floorNumber 
                         << ", Room " << current->roomId 
                         << " (" << current->roomType << "), " 
                         << current->nights << " nights.\n";
                    found = true;
                }
                current = current->next;
            }
            if (!found) {
                cout << "No booking found for customer " << searchName << "\n";
            }
        } else {
            cout << "Invalid choice.\n";
        }
    }
};

void displayMenu() {

    cout << "\n==================== HOTEL MANAGEMENT MENU ====================\n";
    cout << "1. Enqueue Booking Request\n";
    cout << "2. Process Requests\n";
    cout << "3. Cancel Last Booking\n";
    cout << "4. Display Booking History\n";
    cout << "5. Display Room Status\n";
    cout << "6. Search Booking\n";
    cout << "7. Exit\n";
    cout << "================================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    int floors;
    while (true) {
        cout << "Enter number of floors: ";
        if (cin >> floors && floors > 0) {
        	system("cls");
            break;
        }
        cout << "Invalid input. Please enter a positive number.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    HotelManagementSystem hotel(floors);
    int choice;

    do {
        displayMenu();
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            displayMenu();
        }
        cin.ignore();

        switch (choice) {
            case 1:
                hotel.enqueueRequest();
                break;
            case 2:
                hotel.processRequests();
                break;
            case 3:
                hotel.cancelBooking();
                break;
            case 4:
                hotel.displayHistory();
                break;
            case 5:
                hotel.displayRoomStatus();
                break;
            case 6:
                hotel.searchBooking();
                break;
            case 7:
                cout << "Exiting Hotel Management System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
