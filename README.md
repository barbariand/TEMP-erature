# PA1484 – Software Development: Project TEMP-erature
## Introduction
Temp-erature is our  take on a weather app for the ESP32-T4-S3 with lilygo screen configuration.
It is a simplistic GUI weather application for use on the small device and uses simplistic touch navigation with simple configuration schemes to allow users to see an SMHI forecast and the latest months without trouble.
To build and upload the programet to the device, it relies on PlatformIO.
## Getting started
To get started you need to install platformio either in vscode or as a CLI application.
This project also supports using nix for a working development shell and installing all prerequisites automatically and also just for easier development commands in the cli, see justfile for more documentation.

### Prerequisites
- PlatformIO
- vscode(optional)
- nix(optional)
- just(optional)
### Installation
The Wifi config then you need to create the file `./src/network/wifi_cred.hpp` according to `./src/network/wifi_cred_example.hpp`
## Building and running
To build and upload the program first plug the device in to the computer and in VScode press upload in the PlatformIO menu, or in the command line in this directory run `pio run -t upload `
If the device is correctly plugged in it should automatically reboot into the correct firmware and start the program.
For debugging purposes you can also monitor and see the logs using the monitor button in the PlatformIO menu or run `pio run --target upload && pio device monitor`
### Usage
The project has 4 screens, that you can swipe between.
The first screen only contains versioning and group info.
The second contains a 7 day forecast for the selected city.
The third screen contains a scrollable graph for the chosen weather parameter and city.
The fourth screen contains a configuration for selecting the weather parameter and city.
## Features
- [x] US1.1C: As a user, I want to see a starting screen to display the current program version and group number on the first screen.
- [x] US1.2C: As a user, I want to see the weather forecast for the next 7 days for the selected city on the second screen in terms of temperature and weather conditions with symbols (e.g., clear sky, rain, snow, thunder) per day at 12:00.
- [x] US2.1: As a user, I want to be able to navigate between different screens (like forecast screen) by sliding a finger over the touch screen.
- [x] US3.1: As a user, I want to have a screen to view historical weather data.
- [x] US3.2D: As a user, on the third screen I want to view the latest months (SMHI API period: latest-months) of historical hourly data for selected weather parameter in the selected city, using a slider to interact with the historical graph by scrolling where a depleted slider corresponds to the oldest datapoint and a full slider corresponds to the latest datapoint.
- [x] US4.1: As a user, on the fourth screen, I want to access a single settings screen to configure both the city and weather parameter options.
- [x] US4.2B: As a user, I want to select from four weather parameters, namely temperature (1), humidity (6), wind speed (4), and Air pressure (9), using a dropdown list, to customize the historical graph
- [x] US4.3B: As a user, I want to select from five different cities, namely Karlskrona(65090), Stockholm(97400), Göteborg(72420), Malmö(53300), and Kiruna(180940), using a dropdown, to view their weather data for the historical data and starting screen forecast.
