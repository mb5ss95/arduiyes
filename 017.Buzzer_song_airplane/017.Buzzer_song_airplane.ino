int Buzzer = 9;
int length = 25;
char notes[] = "edcdeeedddeggedcdeeeddedc";

int beats[] = {1,2,1,4,1,2,1,5,4,1,1,6,4,3,2 };
//int beats[] = {3,1,2,2,2,2,4,2,2,4,2,2,4,3,1,2,2,2,2,4,2,2,3,1,8};
int tempo = 100;

void playTone(int tone, int duration)
{
  for(long i = 0; i<duration*1000L; i+=tone*2)
  {
    digitalWrite(Buzzer, 1);
    delayMicroseconds(tone);
    digitalWrite(Buzzer, 0);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration)
{
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'c'};
  int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};

  for(int i=0; i<8; i++)
  {
    if(names[i] == note)
    {
      playTone(tones[i], duration);
    }
  }
}

void setup()
{
  pinMode(Buzzer, OUTPUT);
}

void loop()
{
  for(int i = 0; i<length; i++)
  {
    if(notes[i] == ' ')
    {
      delay(beats[i]*tempo);
    }
    else
    {
      playNote(notes[i], beats[i]*tempo);
    }
  delay(tempo/2);
  }
  
}
