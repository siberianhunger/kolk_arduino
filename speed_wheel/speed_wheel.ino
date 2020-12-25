
#include <UTFT.h>
#include <URTouch.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ConsPin       2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

UTFT    myGLCD(ITDB24E_8,A5,A4,A3,A2);
// Инициализация тачскрина 
URTouch  myTouch( A1, 10, A0, 8, 9);

volatile boolean actionState = LOW;
const float pi = 3.14;
int speed = 0, count = 0;
int rez_m = 0; // Частота прерывания
int tah = 0;
int PrevPressTime = 0;
int countEvents=0; // количетсво зарегестрированных событий
int offset; //переменная для отступа

int P = 14;
int R = 15;
int D = 16;

int XPrev = 0, XPrevT = 0;
int YPrev = 0, YPrevT = 0;

void interprev() {

    if(digitalRead(ConsPin)==HIGH) { //при нажатии кнопки
        //Если от предыдущего нажатия прошло больше 100 миллисекунд
        if (millis() - PrevPressTime >= 100) {
            //Запоминается время первого срабатывания
            PrevPressTime = millis();
            count++;
        }
    }
}


void setup()
{
    // инициируем дисплей (в качестве параметра данной функции можно указать ориентацию дисплея: PORTRAIT или LANDSCAPE), по умолчанию LANDSCAPE - горизонтальная ориентация
    myGLCD.InitLCD();
    myGLCD.clrScr();
    // Устанавливаем прерывание
    attachInterrupt(digitalPinToInterrupt(ConsPin), interprev, FALLING);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}


void set_transmission(int XM, int Y)
{
    if (digitalRead(P) == HIGH && digitalRead(R) == LOW && digitalRead(D) == LOW)
    {
        // активен режим P
        myGLCD.setBackColor(0, 0, 0);
        myGLCD.setFont(BigFont);
        myGLCD.print("P", XM, Y-offset*1.5);
        myGLCD.setFont(SmallFont);
        Y=Y+myGLCD.getFontYsize();
        myGLCD.print("R", XM, Y-offset);
        Y=Y+myGLCD.getFontYsize();
        myGLCD.print("D", XM, Y-offset);
    }
    if ((digitalRead(R) == HIGH && digitalRead(P) == LOW && digitalRead(D) == LOW))
    {
        // активен режим R
        myGLCD.setFont(BigFont);
        myGLCD.print("R", XM, Y-offset*1.5);
        myGLCD.setFont(SmallFont);
        Y=Y+myGLCD.getFontYsize();
        myGLCD.print("P", XM, Y-offset);
        Y=Y+myGLCD.getFontYsize();
        myGLCD.print("D", XM, Y-offset);
    }
    if (digitalRead(D) == HIGH && digitalRead(R) == LOW && digitalRead(P) == LOW)
    {
        // активен режим D
        myGLCD.setFont(BigFont);
        myGLCD.print("D", XM, Y-offset*1.5);
        myGLCD.setFont(SmallFont);
        Y=Y+myGLCD.getFontYsize();
        myGLCD.print("R", XM, Y-offset);
        Y=Y+myGLCD.getFontYsize();
        myGLCD.print("P", XM, Y-offset);
    }

}

void print_time_in_OLED()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(32,0);
    display.setTextColor(SSD1306_WHITE);
    display.println(F("Session time:"));
    display.setTextSize(2);
    display.cp437(true);
    display.setCursor(32,10);
    display.print(String(millis()/3600000) + ":");
    display.print(String((millis()/60000) % 60) + ":");
    display.println(String((millis() / 1000) % 60));
    display.display();
}


/* Вечный цикл */
void loop()
{
    int X=160, Y=120; // цетр дисплея, относительно которого будет строится панель

    //вывод приветственого слова в верху дисплея
    myGLCD.setFont(BigFont);
    myGLCD.print("SPEED<---------->TACHO", CENTER, 0);

    offset=40; //переменная для отступа
    int XM=X+myGLCD.getFontXsize()*(Y+offset);

    set_transmission(XM, Y);

    X=160;
    Y=120;
    if (speed > 240)  speed = 0;
    if (tah > 240) tah = 0;
    graphical(X,Y);
    print_time_in_OLED();
    rez_m = count/(((millis() / 1000) / 60) + 1); // Делим колв-во всех совершенных нажатий на кол-во пройденных минут

}

void arrow_tah(int XO2, int YO2, int R)
{
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawLine(XO2, YO2, XPrevT, YPrevT);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawLine(XO2, YO2, XO2 + (R * cos((tah + 150) * PI / 180)), YO2 - ( -R * sin((tah + 150) * PI / 180)));

    XPrevT = XO2 + (R * cos((tah + 150) * PI / 180));
    YPrevT = YO2 - (- R * sin((tah + 150) * PI / 180));
    tah++;

}

void arrow_speed(int XO1, int YO1, int R)
{
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawLine(XO1, YO1, XPrev, YPrev);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawLine(XO1, YO1, XO1 + (R * cos((speed + 150) * PI / 180)), YO1 - ( -R * sin((speed + 150) * PI / 180)));

    XPrev = XO1 + (R * cos((speed + 150) * PI / 180));
    YPrev = YO1 - (- R * sin((speed + 150) * PI / 180));

    speed++;
}

void graphical(int X, int Y)
{
    // Установка цвета
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(0, 0, 0);


    myGLCD.drawLine(X, Y+50, X, Y-70);

    int R=60;

    int XO1=80;
    int YO1=120;
    myGLCD.drawCircle(XO1, YO1, R);


    int XO2=240;
    int YO2=120;
    myGLCD.drawCircle(XO2, YO2, R);

    // подпись km/h и R.P.M
    myGLCD.setFont(BigFont);
    int offset=R+5; // отступ от самого контура окружности
    myGLCD.print("km/h", XO1-R*0.6, YO1+offset);
    myGLCD.print("R.P.M", XO2-R*0.6, YO2+offset);

    // установка размера текста
    myGLCD.setFont(SmallFont);

    offset=6;
    // подпись шкалы у спидометра
    for (int i = 0; i < 10; i++){
        myGLCD.printNumI(i * 25, XO1 + (R * cos((i * 25 + 150) * PI / 180)), YO1 - ( -R * sin((i * 25 + 150) * PI / 180)));

    }

    // подпись шкалы у тахометра;
    for (int i = 0; i < 9; i++){
        myGLCD.printNumI(i * 25, XO1 + (R * cos((i * 25 + 150) * PI / 180)), YO1 - ( -R * sin((i * 25 + 150) * PI / 180)));
        myGLCD.printNumI(i, XO2 + (R * cos((30*i + 150) * PI / 180)), YO2 - ( -R * sin((30*i + 150) * PI / 180)));

    }

    offset=5;
    myGLCD.setFont(BigFont);
    myGLCD.print(String(rez_m), XO2-offset, YO2-offset);
    arrow_speed(XO1, YO1, R);
    arrow_tah(XO2, YO2, R);


}
