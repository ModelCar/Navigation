#include <iostream>
#include <exception>
#include <vector>

//#include <wiringPi.h>           //WiringPi headers
//#include <lcd.h>                //LCD headers from WiringPi

#include "navigator.cpp"

//#define LCD_RS  25               //Register select pin
//#define LCD_E   24               //Enable Pin
//#define LCD_D4  23               //Data pin 4
//#define LCD_D5  22              //Data pin 5
//#define LCD_D6  21              //Data pin 6
//#define LCD_D7  14               //Data pin 7

void showOnLcd(int lcd, std::string row1, std::string row2)
{
//    lcdClear(lcd);
//    lcdPosition(lcd,0,0);           //Position cursor on the first line in the first column
//    lcdPuts(lcd, "Maneuver:");
//    lcdPosition(lcd,11,0);
//    lcdPuts(lcd, row1.c_str());  //Print the text on the LCD at the current cursor postion
//    lcdPosition(lcd, 0, 1);
//    lcdPuts(lcd, "Bearing:");
//    lcdPosition(lcd, 11, 1);
//    lcdPuts(lcd, row2.c_str().substr(0, 5));
}

int main(int argc, const char *argv[]) try
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " map.osm.pbf\n";
        return EXIT_FAILURE;
    }

//    int lcd;                //Handle for LCD
//    wiringPiSetup();		//Initialise WiringPi
//
//    //Initialise LCD(int rows, int cols, int bits, int rs, int enable, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7)
//    lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4,LCD_D5,LCD_D6,LCD_D7, 0, 0, 0, 0);

    coordinates destination;
    destination.latitude = 48.267505;
    destination.longitude = 11.672671;

    auto navigator = new Navigator();
    navigator->setDestination(destination);
    for(;;) {
        auto step = navigator->getNextStep();
        printf("");
    }

}
catch (const std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
