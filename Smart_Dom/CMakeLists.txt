cmake_minimum_required(VERSION 3.13.1)
set(BOARD "nucleo_f103rb")

find_package(Zephyr)
project(Smart_Dom)

# source files
target_sources(app PRIVATE code/main.c code/light_sensor.c code/blinds.c)
