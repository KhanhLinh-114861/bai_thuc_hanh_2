#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;
int readInt(const string& prompt, int lo, int hi) {
    int x;
    while (true) {
        cout << prompt;
        cin >> x;
        if (cin.fail()) {
            if (cin.eof()) exit(0);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [Loi] Vui long nhap SO nguyen!\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (x < lo || x > hi) {
            cout << "  [Loi] Gia tri phai trong khoang " << lo << " - " << hi << "!\n";
            continue;
        }
        return x;
    }
}
 
char readSize() {
    string s;
    while (true) {
        cout << "Kich thuoc (S/M/L): ";
        cin >> s;
        if (cin.eof()) exit(0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (s.size() == 1) {
            char c = toupper(s[0]);
            if (c == 'S' || c == 'M' || c == 'L') return c;
        }
        cout << "  [Loi] Chi chap nhan S, M hoac L!\n";
    }
}

string normalizeType(string s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = tolower(s[i]);
    if (s == "car") return "Car";
    if (s == "motorcycle") return "Motorcycle";
    if (s == "bicycle") return "Bicycle";
    return "";
}
 
string readVehicleType() {
    string s, t;
    while (true) {
        cout << "Loai xe (Car/Motorcycle/Bicycle): ";
        cin >> s;
        if (cin.eof()) exit(0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        t = normalizeType(s);
        if (t != "") return t;
        cout << "  [Loi] Loai xe khong hop le!\n";
    }
}
 
class Vehicle {
private:
    int vehicleID;
    string vehicleType;
    int entryTime;
    int parkingDuration;
    double parkingFee;
 
public:
    Vehicle();
    Vehicle(int id, const string& type, int entry, int duration);
 
    int getVehicleID() const;
    string getVehicleType() const;
    int getEntryTime() const;
    int getParkingDuration() const;
    double getParkingFee() const;
 
    void setVehicleID(int id);
    void setVehicleType(const string& type);
    void setEntryTime(int entry);
    void setParkingDuration(int duration);
 
    double calculateFee();      
    void display() const;
};
 
Vehicle::Vehicle()
    : vehicleID(0), vehicleType(""), entryTime(0), parkingDuration(0), parkingFee(0.0) {}
 
Vehicle::Vehicle(int id, const string& type, int entry, int duration)
    : vehicleID(id), vehicleType(type), entryTime(entry), parkingDuration(duration), parkingFee(0.0) {
    calculateFee();
}
 
int Vehicle::getVehicleID() const { return vehicleID; }
string Vehicle::getVehicleType() const { return vehicleType; }
int Vehicle::getEntryTime() const { return entryTime; }
int Vehicle::getParkingDuration() const { return parkingDuration; }
double Vehicle::getParkingFee() const { return parkingFee; }
 
void Vehicle::setVehicleID(int id) { vehicleID = id; }
void Vehicle::setVehicleType(const string& type) { vehicleType = type; calculateFee(); }
void Vehicle::setEntryTime(int entry) { entryTime = entry; calculateFee(); }
void Vehicle::setParkingDuration(int duration) { parkingDuration = duration; calculateFee(); }
 
double Vehicle::calculateFee() {
    double rate = (vehicleType == "Car") ? 6.0 : (vehicleType == "Motorcycle") ? 4.0 : 2.0;
    bool peak = (entryTime >= 6 && entryTime <= 9) || (entryTime >= 15 && entryTime <= 18);
    double total = 0;
    for (int h = 0; h < parkingDuration; ++h) {
        if (h < 2) total += rate * (peak ? 1.30 : 0.95);   // 2 gio dau
        else       total += rate * 0.90;                   // tu gio thu 3
    }
    parkingFee = total;
    return parkingFee;
}
 
void Vehicle::display() const {
    cout << "  ID: " << vehicleID << " | " << vehicleType
         << " | " << entryTime << "h | " << parkingDuration << "h"
         << " | Phi: $" << fixed << setprecision(2) << parkingFee << "\n";
}

class ParkingLot {
private:
    int parkID;
    char size;
    static const int MAX_VEHICLES = 3;
    Vehicle vehicles[MAX_VEHICLES];
    int currentVehicleCount;
 
public:
    ParkingLot();
    ParkingLot(int id, char sz);
 
    int getParkID() const;
    char getSize() const;
    int getCapacity() const;
    int getVehicleCount() const;
    const Vehicle& getVehicleAt(int index) const;
 
    bool isFull() const;
    bool addVehicle(const Vehicle& v);
    bool removeVehicle(int vID);
    int findVehicleIndex(int vID) const;
    void displayLotInfo() const;
};
 
ParkingLot::ParkingLot() : parkID(0), size('S'), currentVehicleCount(0) {}
ParkingLot::ParkingLot(int id, char sz) : parkID(id), size(sz), currentVehicleCount(0) {}
 
int ParkingLot::getParkID() const { return parkID; }
char ParkingLot::getSize() const { return size; }
int ParkingLot::getCapacity() const {
    return (size == 'S') ? 1 : (size == 'M') ? 2 : 3;
}
int ParkingLot::getVehicleCount() const { return currentVehicleCount; }
const Vehicle& ParkingLot::getVehicleAt(int index) const { return vehicles[index]; }
 
bool ParkingLot::isFull() const {
    return currentVehicleCount >= getCapacity();
}
 
bool ParkingLot::addVehicle(const Vehicle& v) {
    if (isFull()) return false;
    vehicles[currentVehicleCount] = v;
    ++currentVehicleCount;
    return true;
}
 
int ParkingLot::findVehicleIndex(int vID) const {
    for (int i = 0; i < currentVehicleCount; ++i)
        if (vehicles[i].getVehicleID() == vID) return i;
    return -1;
}
 
bool ParkingLot::removeVehicle(int vID) {
    int idx = findVehicleIndex(vID);
    if (idx == -1) return false;
    // Don cac phan tu phia sau sang trai
    for (int i = idx; i < currentVehicleCount - 1; ++i)
        vehicles[i] = vehicles[i + 1];
    --currentVehicleCount;
    return true;
}
 
void ParkingLot::displayLotInfo() const {
    cout << "Bai " << parkID << " [" << size << "] "
         << currentVehicleCount << "/" << getCapacity() << " xe\n";
    for (int i = 0; i < currentVehicleCount; ++i)
        vehicles[i].display();
}
 
class SystemInterface {
private:
    static const int MAX_LOTS = 10;
    ParkingLot parkingLots[MAX_LOTS];
    int currentLotCount;
 
    int findLotIndex(int pID) const;
 
public:
    SystemInterface();
    void runMenu();
    bool isParkIDExists(int pID) const;
    bool isVehicleIDExists(int vID) const;
    int generateRandomID();
};
 
SystemInterface::SystemInterface() : currentLotCount(0) {}
 
int SystemInterface::findLotIndex(int pID) const {
    for (int i = 0; i < currentLotCount; ++i)
        if (parkingLots[i].getParkID() == pID) return i;
    return -1;
}
 
bool SystemInterface::isParkIDExists(int pID) const {
    return findLotIndex(pID) != -1;
}
 
bool SystemInterface::isVehicleIDExists(int vID) const {
    for (int i = 0; i < currentLotCount; ++i)
        if (parkingLots[i].findVehicleIndex(vID) != -1) return true;
    return false;
}
 
int SystemInterface::generateRandomID() {
    return 1000 + rand() % 9000;   // ID ngau nhien 1000 - 9999
}
 
void SystemInterface::runMenu() {
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE =====\n";
        cout << "1. Tao bai do xe moi\n";
        cout << "2. Them xe vao bai do\n";
        cout << "3. Xoa mot bai do xe\n";
        cout << "4. Lay xe ra khoi bai do\n";
        cout << "5. Hien thi tat ca bai do xe\n";
        cout << "6. Hien thi xe theo bai cu the\n";
        cout << "7. Liet ke xe theo loai phuong tien\n";
        cout << "8. Tom tat phi do xe trung binh\n";
        cout << "9. Thoat\n";
        int choice = readInt("Chon: ", 1, 9);
 
        switch (choice) {
            case 1: {
                if (currentLotCount >= MAX_LOTS) {
                    cout << "He thong da dat toi da " << MAX_LOTS << " bai do!\n";
                    break;
                }
                int pID = readInt("Ma bai (so nguyen duong): ", 1, 999999);
                char sz = readSize();
                if (isParkIDExists(pID)) {
                    cout << "  [Canh bao] Ma bai " << pID << " da ton tai!\n";
                    do { pID = generateRandomID(); } while (isParkIDExists(pID));
                    cout << "  => Tu dong gan ma ngau nhien: " << pID << "\n";
                }
                parkingLots[currentLotCount] = ParkingLot(pID, sz);
                ++currentLotCount;
                cout << "Da tao bai " << pID << "\n";
                break;
            }
            case 2: {
                if (currentLotCount == 0) {
                    cout << "Chua co bai do nao, hay tao bai truoc!\n";
                    break;
                }
                int vID = readInt("Ma xe (so nguyen duong): ", 1, 999999);
                string type = readVehicleType();
                int entry = readInt("Gio vao (0-23): ", 0, 23);
                int dur = readInt("Thoi luong do (1-23 gio): ", 1, 23);
                if (isVehicleIDExists(vID)) {
                    cout << "  [Canh bao] Ma xe " << vID << " da ton tai!\n";
                    do { vID = generateRandomID(); } while (isVehicleIDExists(vID));
                    cout << "  => Tu dong gan ma ngau nhien: " << vID << "\n";
                }
                Vehicle v(vID, type, entry, dur);
                bool placed = false;
                for (int i = 0; i < currentLotCount && !placed; ++i) {
                    if (!parkingLots[i].isFull()) {
                        parkingLots[i].addVehicle(v);
                        placed = true;
                        cout << "Da them xe " << vID << " vao bai "
                             << parkingLots[i].getParkID()
                             << " | Phi: $" << fixed << setprecision(2) << v.getParkingFee() << "\n";
                    }
                }
                if (!placed) cout << "  [Canh bao] Tat ca cac bai do da day!\n";
                break;
            }
            case 3: {
                int pID = readInt("Ma bai can xoa: ", 1, 999999);
                int idx = findLotIndex(pID);
                if (idx == -1) {
                    cout << "Khong tim thay bai " << pID << "\n";
                    break;
                }
     
                for (int i = idx; i < currentLotCount - 1; ++i)
                    parkingLots[i] = parkingLots[i + 1];
                --currentLotCount;
                cout << "Da xoa bai " << pID << "\n";
                break;
            }
            case 4: {
                int vID = readInt("Ma xe can lay ra: ", 1, 999999);
                bool removed = false;
                for (int i = 0; i < currentLotCount && !removed; ++i)
                    removed = parkingLots[i].removeVehicle(vID);
                cout << (removed ? "Da lay xe ra\n" : "Khong tim thay xe\n");
                break;
            }
            case 5: {
                if (currentLotCount == 0) cout << "Chua co bai do nao.\n";
                for (int i = 0; i < currentLotCount; ++i)
                    parkingLots[i].displayLotInfo();
                break;
            }
            case 6: {
                int pID = readInt("Ma bai: ", 1, 999999);
                int idx = findLotIndex(pID);
                if (idx == -1) cout << "Khong tim thay bai " << pID << "\n";
                else parkingLots[idx].displayLotInfo();
                break;
            }
            case 7: {
                string type = readVehicleType();
                bool found = false;
                for (int i = 0; i < currentLotCount; ++i) {
                    for (int j = 0; j < parkingLots[i].getVehicleCount(); ++j) {
                        const Vehicle& v = parkingLots[i].getVehicleAt(j);
                        if (v.getVehicleType() == type) {
                            if (!found) cout << "Cac xe loai " << type << ":\n";
                            cout << "Bai " << parkingLots[i].getParkID() << ":";
                            v.display();
                            found = true;
                        }
                    }
                }
                if (!found) cout << "Khong co xe loai " << type << "\n";
                break;
            }
            case 8: {
                string types[3] = {"Car", "Motorcycle", "Bicycle"};

                cout << "--- Toan he thong ---\n";
                for (int t = 0; t < 3; ++t) {
                    int cnt = 0; double sum = 0;
                    for (int i = 0; i < currentLotCount; ++i)
                        for (int j = 0; j < parkingLots[i].getVehicleCount(); ++j) {
                            const Vehicle& v = parkingLots[i].getVehicleAt(j);
                            if (v.getVehicleType() == types[t]) { ++cnt; sum += v.getParkingFee(); }
                        }
                    cout << types[t] << ": ";
                    if (cnt == 0) cout << "khong co xe\n";
                    else cout << cnt << " xe | TB: $" << fixed << setprecision(2) << sum / cnt << "\n";
                }

                for (int i = 0; i < currentLotCount; ++i) {
                    cout << "--- Bai " << parkingLots[i].getParkID() << " ---\n";
                    for (int t = 0; t < 3; ++t) {
                        int cnt = 0; double sum = 0;
                        for (int j = 0; j < parkingLots[i].getVehicleCount(); ++j) {
                            const Vehicle& v = parkingLots[i].getVehicleAt(j);
                            if (v.getVehicleType() == types[t]) { ++cnt; sum += v.getParkingFee(); }
                        }
                        cout << types[t] << ": ";
                        if (cnt == 0) cout << "khong co xe\n";
                        else cout << cnt << " xe | TB: $" << fixed << setprecision(2) << sum / cnt << "\n";
                    }
                }
                break;
            }
            case 9:
                cout << "Tam biet!\n";
                return;
        }
    }
}
 
int main() {
    srand((unsigned)time(NULL));
    SystemInterface sys;
    sys.runMenu();
    return 0;
}
 
