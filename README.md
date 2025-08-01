# 🎧 Qt Internet Radio Player

A simple internet radio player built with **Qt 6 (C++)**. It supports ICY metadata for real-time song titles, animated GIF feedback for playback status, stream retry mechanism, and a track history list.

## 🚀 Features

- 🎵 Displays current song using ICY metadata
- 🔁 Automatic retry on stream errors
- 🕺 Playback status with animated GIFs
- 📜 Keeps a history of last 20 tracks
- 🎚️ Volume control with slider
- 🔗 Supports manual and preset radio URLs

---

## 🛠️ Technologies Used

- Qt 6 (C++)
- QMediaPlayer, QAudioOutput
- QMovie (for GIFs)
- QStringListModel
- QTimer, QPropertyAnimation

---

## 📂 File Structure

- mainwindow.h / .cpp         # Main UI logic
- icystreamfetcher.h / .cpp   # Handles ICY metadata
- ui_mainwindow.h             # Generated by Qt Designer
- resources.qrc               # GIFs and icons
- CMakeLists.txt              # Build configuration



## 🧪 How to Build

```bash
git clone https://github.com/BathanCaln/qt-radio-player.git
cd qt-radio-player
mkdir build && cd build
cmake ..
make
./RadioPlayer

