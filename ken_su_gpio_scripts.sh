#!/bin/bash
# Short script to toggle a GPIO pin at the highest frequency
# possible using Bash - by Derek Molloy

echo in > /sys/class/gpio/gpio67/direction
echo rising > /sys/class/gpio/gpio67/edge


PERIOD=10000000
DUTY_CYCLE=2500000
echo out > /sys/class/gpio/gpio22/direction
config-pin P8_19 pwm
echo $PERIOD > /sys/class/pwm/pwm-7:0/period
echo $DUTY_CYCLE > /sys/class/pwm/pwm-7:0/duty_cycle
echo 1 > /sys/class/pwm/pwm-7:0/enable

echo -e "\n P8_8 Direction"
cat /sys/class/gpio/gpio67/direction

echo -e "\n P8_19 Direction"
cat /sys/class/gpio/gpio22/direction

echo -e "\n PWM Enable"
cat /sys/class/pwm/pwm-7:0/enable

echo -e "\n Period"
cat /sys/class/pwm/pwm-7:0/period

echo -e "\n Duty Cylce"
cat /sys/class/pwm/pwm-7:0/duty_cycle