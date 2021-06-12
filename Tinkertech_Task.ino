#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>

//Adafruit_SSD1306 constructor which accepts no reset pin.
Adafruit_SSD1306 display(-1);

//String which should be received from the SoftwareSerial but now it is hardcoded.
char InputString[] = "We translate C source code to Java source code. The translation is completely automatic, supports the entire C language and creates functionally equivalent Java code - ready to be executed.";

//To store the words while spliting the Input string.
char *words = NULL;

//The splited string is stored in the String array.
String str[30];

void setup(){
    //Interator 
    int i = 0;

    //used to store the starting index of the string in each page.
    int start = 0;

    //used to store the ending index of the string in each page.
    int end = 0;;

    //Spliting the InputString with empty spaces.
    words = strtok(InputString, " ");
    while(words != NULL)
    {
        str[i++] = words;
        words = strtok(NULL, " "); 
    }
    
    //Turning ON the Internal charge Pump circuitry and selcting the i2c address line. 
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

    //Clearing the display
    display.clearDisplay();

    //Selecting the text size as 1 which is equivalent to 7:10
    display.setTextSize(1);

    //White color text is ideal for black background
    display.setTextColor(WHITE);

    //Iterator
    int n = i;
    while(start<n)
    {
    //Call the fucntion with string array, start index, end index and interator.
    start = display_message(str,start,&end,n);
    
    //Clearing the screen for every new page;
    display.clearDisplay();

    //Ending the creation of new page to avoid adding the last line to new page.
    if(end == n-1)
        break;
    }
    }

    
//Function to print the string in SSD1306 Display
int display_message(String *str,int start, int*end,int n)
{
    //Total number of alphabets that can be printed in a single line. 21 is a trial and error value of the total characters that can be printed on a single line. 
    int space = 21;
    //Used to set the cursor on the consecutive lines after printing the curret line. It is hardcoded based on trial and error.
    int line = 0;
    
    //Starting to diplay from (0,0)
    display.setCursor(0,line);

    //Pritning the strign with Wrapping
    for(int i =start;i<n;i++)
    {
        String s=str[i] + " ";
        
        //if length of the current word is less than the available space, print the word on the current line
        if(s.length() <= space)
        {
            display.print(s);
            display.display();
            space-=s.length();
            
        }
        else{
            //Horizonatally available space re-declaration
            space = 21;

            //8 is the number of pixels to display right below the current line. It is used instead of 1 because using 1 would print the text slightly  below the text and the image would be distorted.
            line = line + 8;

            //line 8 indicates the starting of the 2nd line. The value of i is the starting index of the next page.(scrolling down)
            if(line == 8)
            {
                //Setting the start index for next page
                start = i;
            }
            display.setCursor(0,line);
            i--;
            
            //line 32 indicates the end of 4th line which is the end of the current page.
            if(line == 32)
            {
                //Delay between two successive pages
                delay(500);
                return start;
            }
            }
         //End index of the last page.
        *end = i;
    }
    return start;
}
void loop()
{

}
