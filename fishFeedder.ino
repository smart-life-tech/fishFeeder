#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>
#include <TimeAlarms.h>

#define RELAY_PIN 2 // Replace with the actual pin connected to the relay
#define SERVO_PIN 3 // Replace with the actual pin connected to the servo

RTC_DS3231 rtc;

Servo dispenserServo;

void setup()
{
    Serial.begin(9600);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    dispenserServo.attach(SERVO_PIN);

    // Set up alarms
    Alarm.timerRepeat(dailyAlarm(6, 0, 0), feed1);  // 6am for feed 1
    Alarm.timerRepeat(dailyAlarm(12, 0, 0), feed2); // 12pm for feed 2
    // Add more alarms for additional feed times if needed

    // Initialize other pins
    pinMode(RELAY_PIN, OUTPUT);
}

void loop()
{
    Alarm.delay(1000); // Allow Alarms to trigger
}

time_t dailyAlarm(int hour, int minute, int second)
{
    DateTime now = rtc.now();
    DateTime alarmTime = DateTime(now.year(), now.month(), now.day(), hour, minute, second);
    if (now.unixtime() > alarmTime.unixtime())
    {
        alarmTime = alarmTime + TimeSpan(1, 0, 0, 0); // If the alarm time has already passed today, set it for tomorrow
    }
    return now.unixtime() + (alarmTime.unixtime() - now.unixtime());
}

void feed1()
{
    feed(1);
}

void feed2()
{
    feed(2);
}

// Add more functions for additional feed times if needed

void feed(int feedNumber)
{
    int feedDuration = 0; // Set the default feed duration
    // Read the feed duration from the user input or any other method
    // You can use Serial communication, buttons, or any other input method

    digitalWrite(RELAY_PIN, HIGH); // Engage throwing motor

    dispenserServo.write(0); // Start the dispenser servo

    delay(feedDuration * 1000); // Wait for the specified feed duration

    dispenserServo.write(90); // Stop the dispenser servo

    delay(3000); // Wait for 3 seconds before turning off the throwing motor

    digitalWrite(RELAY_PIN, LOW); // Turn off the throwing motor
}
