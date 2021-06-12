# TinkerTechLabs - Software Developer Intern Task
I have been trying to get the input string from Software Serial and dipslay the received text on a 128x64 based on SSD 1306 driver, with word wrapping and if the text is longer than what fits on the screen, the display should automatically scroll down with interval of 500ms.\
The Manufactorers library Adafruit_SSD1306.h includes inbuilt functions only for scrolling horizontally and diagonally.
From the SSD1306 documentation, it is concluded that display's based on SSD1306 driver doesnot have option to scroll downwards or upwards. So, there is no way to scroll the text vertically downwards on the SSD1306 display.\
With some further research, an article from [hackaday](https://hackaday.io/project/10533-nano-tty/log/34525-smooth-scrolling-on-a-ssd1306) saved my day. 
The SSD1306 display driver supports vertical scrolling by setting the start line for the scrolled text. The code here uses I2C protocol for communication between display and processor.\
This is the third version or method for display the text with vertical scrolling and the demo video is included in the google drive.\
Note: The Input String should be received from Software Serial.

## PROGRAM:
### Header Files
```
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
```
The necessary Header files and constructors are declared on the global scope. Adafruit_SSD1306 constructor which accepts no reset pin. String which should be received from the SoftwareSerial is stored on the Character array (InputString) but now the value is hardcoded. The character pointer words is used to store the words while spliting the Input string. The splited string is stored in the String array(str).
### Spliting the String by the empty spaces between words
```
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
```
The strtok() function splits the string in the character array InputString and stores each word in the string array str which uses the character pointer words. Empty is also inlcuded when adding the string to the array. It is inlcuded in void setup().
### Display Setting and Function call
```
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
```
Firstly, the internal charge pump circuitry is turned ON and the necessary display settings are made. The funciton display_message() is repeatedly called for number of words in the str array. The display is cleared to print the next page(scorlled text).At the end of the string, the break statement is used to stop the program in the last word to avoid printing the last word on the next page. This function call concludes the setup() function.
### Display_message Function
```
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
```
This logic gives the user illusion of text scrolling by creating replacing the existing line of text with sucessive text that should have been scrolled. The idea is the when the text fitting in the first page is displayed, the next page is displayed with the second line of previous page as starting line for the current page and printing the sucessive text on the 4th line which should have been scrolled. The process is repeated for the n number of words in the string.
## Limitation
- The proper method for getting the input from Software Serial is to be achieved. 
- For the user, the display would appear to be flickering and requires a little long time delay to read the text.
## Conclusion
So far, this method is effective for displaying text while comparing to the [version 1 and version 2](https://github.com/RalphBacon/SSD1306-TFT-Screen-I2C-SPI) by Ralph Bacon. The version 1 contains the input string in the header file and imports the string while executing. This constrats our need of dynamically getting the data. The version 2 can be applicable only to small strings. So, these methods are less effective to display the input string with vertical scroll.
