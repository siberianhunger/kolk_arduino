#include <avr/interrupt.h>
#include <math.h>
#include <TM1637.h>
const int interval = 10;
const int allowed_error = 50;

const int true_trigger = 880;
const int false_trigger = 440;

unsigned long interval_array[interval];
unsigned long previous_time = 0;
unsigned long last_event_time = 0;
unsigned int duration = 1;
unsigned int last_trig_state = -1;
unsigned int parity = 0;
byte count = 0;

bool transmission_started = false;

int CLK = 12;
int DIO = 13;
TM1637 tm(CLK,DIO);

void setup() {
    tm.init();
    tm.set(2);
    Serial.begin(9600);
    attachInterrupt(1, decoder, RISING);
}

void decoder()
{
    unsigned long time_millis = millis();
    interval_array[count] = time_millis - previous_time;
    previous_time = time_millis;
    count++;
    if(count == interval)
    {
        double average_receive_time = 0;
        for(int i = 0; i < interval; i++) average_receive_time += interval_array[i];
        average_receive_time = average_receive_time / interval;
        average_receive_time = 1000/average_receive_time;
        count = 0;
        int trig_state = 2;
        if ((average_receive_time > (true_trigger - allowed_error)) && (average_receive_time < (true_trigger + allowed_error))) trig_state = 1;
        if ((average_receive_time > (false_trigger - allowed_error)) && (average_receive_time < (false_trigger + allowed_error))) trig_state = 0;
        if (trig_state < 2)
        {
            if (last_trig_state == trig_state)
            {
                if (last_trig_state == 0) duration += 2;
                else if (last_trig_state == 1) duration++;
            }
            else
            {
                if (transmission_started)
                {
                    if (last_trig_state == 1) parity += 1;
                    Serial.print(last_trig_state);
                    while (duration > 19)
                    {
                        duration -= 16;
                        if (last_trig_state == 1 && duration > 21 && duration < 27)
                        {
                            Serial.println();
                            Serial.println("Передача закончилась");
                            last_event_time = millis() - last_event_time;
                            if (parity % 2 == last_trig_state){

                                Serial.println("Бит чётности верный");
                                tm.display(0,13);
                                tm.display(1,0);
                                tm.point(0);
                                tm.display(2,10);
                                tm.display(3,14);
                            }

                            else{
                                Serial.println("Бит чётности неверный");
                                tm.display(0,0);
                                tm.display(1,0);
                                tm.point(0);
                                tm.display(2,0);
                                tm.display(3,0);
                            }
                            parity = 0;
                            Serial.println();
                            transmission_started = false;
                            break;
                        }
                        else
                        {
                            if (last_trig_state == 1) parity += 1;
                            Serial.print(last_trig_state);
                        }
                    }
                }
                else if (last_trig_state == 1 && duration > 21 && duration < 26)
                {
                    Serial.println(F("Время с момента прошлого события: "));
                    Serial.println(String(last_event_time/1000) + " sec");
                    Serial.println("НОВОЕ СОБЫТИЕ! Начало передачи:");
                    transmission_started = true;

                }
                duration = 1;
            }
            last_trig_state = trig_state;
        }
    }
}

void loop() {

}
