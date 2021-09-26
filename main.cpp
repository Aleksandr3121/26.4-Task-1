#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

class Track {
public:
    Track(const string& n, unsigned dif_time = 0) : name(n), duration_time(dif_time) {
        if (duration_time >= 3600)duration_time = 3599;
        time_t t = time(nullptr);
        creation_time = *localtime(&t);
    }

    void play() {
        if (!play_) {
            cout << name << '\n'
                 << "Creation time: " << put_time(&creation_time, "%Y/%m/%d %H:%M:%S") << '\n';
            cout << setfill('0');
            cout << "Duration: " << setw(2) << duration_time / 60 << ':' << (duration_time % 60) << endl;
            play_ = true;
            pause_ = false;
        }
    }

    void pause() {
        if (play_ && !pause_) {
            cout << "Track " << name << " on pause" << endl;
            pause_ = true;
            play_ = false;
        }
    }

    void stop() {
        if (play_ || pause_) {
            cout << "Track " << name << " is stopped" << endl;
            play_ = false;
            pause_ = false;
        }
    }

    const string& getName() const {
        return name;
    }

private:
    string name;
    unsigned duration_time;
    tm creation_time;
    bool play_ = false;
    bool pause_ = false;
};

bool operator<(const Track& lhs, const Track& rhs) {
    return lhs.getName() < rhs.getName();
}

bool operator==(const Track& lhs, const Track& rhs) {
    return lhs.getName() == rhs.getName();
}

bool operator!=(const Track& lhs, const Track& rhs) {
    return !(lhs == rhs);
}

class AudioPlayer {
public:

    AudioPlayer() : cur_index(-1) {
        load("input.txt");
    }

    void play(const string& name) {
        auto it = lower_bound(recordings.begin(), recordings.end(), Track(name));
        if (it == recordings.end() || it->getName() != name) {
            cout << "There is no such track" << endl;
        } else {
            if ((it - recordings.begin()) != cur_index && cur_index != -1) {
                recordings[cur_index].stop();
            }
            it->play();
            cur_index = it - recordings.begin();
        }
    }

    void pause() {
        if (cur_index != -1) {
            recordings[cur_index].pause();
        }
    }

    void stop() {
        if (cur_index != -1) {
            recordings[cur_index].stop();
        }
    }

    void next() {
        if (cur_index != -1) {
            recordings[cur_index].stop();
        }
        cur_index = rand() % recordings.size();
        recordings[cur_index].play();
    }

private:
    void load(const string& name_file) {
        ifstream file(name_file);
        if (!file.is_open()) {
            throw invalid_argument("");
        }
        string input_str;
        while (getline(file, input_str)) {
            size_t pos_space = input_str.find(' ');
            recordings.emplace_back(Track(input_str.substr(0, pos_space),
                                          stoul(input_str.substr(pos_space + 1))));
        }
        sort(recordings.begin(), recordings.end());
        file.close();
    }

    int cur_index;
    vector<Track> recordings;

};

int main() {
    AudioPlayer audioPlayer;
    string input_str;
    while (cin >> input_str && input_str != "exit") {
        if (input_str == "play") {
            cout << "Enter name of track: ";
            cin >> input_str;
            audioPlayer.play(input_str);
        } else if (input_str == "pause") {
            audioPlayer.pause();
        } else if (input_str == "next") {
            audioPlayer.next();
        } else if (input_str == "stop") {
            audioPlayer.stop();
        } else cerr << "Unknown command" << endl;
    }
    return 0;
}
