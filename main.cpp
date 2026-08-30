#include "../database/database.cpp"
#include "../database/shard.cpp"
#include "../storage/bucket.cpp"
#include "../storage/dashtable.cpp"
#include "../storage/hasher.cpp"
#include "../storage/segment.cpp"
#include "dashtable.hpp"
#include <iostream>
#include <iomanip>      // for setw
#include <thread>       // for thread
#include <chrono>       // for delays (optional aesthetic)

using namespace std;

// Utility function for section headers
void printHeader(const string& title) {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << setw(24 + title.length() / 2) << right << title
         << setw(24 - title.length() / 2) << right << "\n";
    cout << "╚════════════════════════════════════════════════╝\n";
}

// Handles score updates
void updateScore(Database* db, const string& player, int score) {
    try {
        // cout << "[DEBUG] updateScore called for player: " << player << ", score: " << score << endl;

        string existingScoreStr;
        try {
            existingScoreStr = db->Get(player);
            // cout << "[DEBUG] Existing score string: " << existingScoreStr << endl;
        } catch (const std::runtime_error& e) {
            // cout << "[DEBUG] Player not found: " << e.what() << endl;
            existingScoreStr = "";
        } catch (const std::exception& e) {
            // cout << "[DEBUG] Other exception: " << e.what() << endl;
            existingScoreStr = "";
        }

        if (!existingScoreStr.empty()) {
            int existingScore = stoi(existingScoreStr);
            if (score > existingScore) {
                db->Put(player, to_string(score));
                cout << "\n✅  Score updated successfully!\n";
                cout << "    " << setw(15) << left << player << " ➜ " << score << "\n";
            } else {
                cout << "\n⚠️  Score not updated (new score is not higher).\n";
                cout << "    " << setw(15) << left << player << " ➜ " << score << "\n";
            }
        } else {
            db->Put(player, to_string(score));
            cout << "\n🆕  New player added!\n";
            cout << "    " << setw(15) << left << player << " ➜ " << score << "\n";
        }
    } catch (const std::exception& e) {
        cout << "\n[ERROR] Uncaught exception: " << e.what() << endl;
    } catch (...) {
        cout << "\n[ERROR] Unknown exception occurred." << endl;
    }
}

// Leaderboard menu
void menu(Database* db) {
    int choice;
    while (true) {
        printHeader("🏆  GAME LEADERBOARD  🏆");
        cout << "\n   Please choose an option:\n";
        cout << "   1️⃣  Submit Player Score\n";
        cout << "   2️⃣  View Player Score\n";
        cout << "   3️⃣  Exit\n";
        cout << "\n   🔢 Enter choice (1-3): ";

        cin >> choice;

        if (cin.fail()) {
            cout << "\n❌ Invalid input. Exiting...\n";
            break;
        }

        switch (choice) {
            case 1: {
                printHeader("🎮 SUBMIT SCORE");
                string name;
                int score;
                cout << "   👤 Enter player name  : ";
                cin >> name;
                cout << "   🏅 Enter player score : ";
                cin >> score;

                thread t(updateScore, db, name, score);
                t.join();
                break;
            }
            case 2: {
                printHeader("🔍 VIEW PLAYER SCORE");
                string name;
                cout << "   👤 Enter player name: ";
                cin >> name;
                string score = db->Get(name);
                if (!score.empty()) {
                    cout << "\n   🎯 " << setw(15) << left << name << " ➜ Score: " << score << "\n";
                } else {
                    cout << "\n   ❌ Player not found in the leaderboard.\n";
                }
                break;
            }
            case 3: {
                printHeader("👋 GOODBYE");
                // cout << "\n   Thank you for using the leaderboard!\n\n";
                return;
            }
            default:
                cout << "\n❗ Invalid choice. Please enter 1, 2, or 3.\n";
        }

        cout << "\n───────────────────────────────────────────────\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));  // short pause for UX
    }
}

int main() {
    // cout << "[DEBUG] Starting program...\n";
    Database* db = new Database(8);  // You can tweak the number of shards
    // cout << "[DEBUG] Database created.\n";
    menu(db);
    // cout << "[DEBUG] Returned from menu(). Cleaning up...\n";
    delete db;
    return 0;
}
