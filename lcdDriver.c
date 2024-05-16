/*
    author: Matthew Boeding, Matthew Heywood
    version: v1.1

    TFT-LCD (ILI9341) Driver code for the 8051. Can be repurposed for other drivers controllers :)
*/
#include "lcdDriver.h"
#include "bmp_image.h"
#include "font.h"

__xdata uint8_t* const lcd_cmd = (uint8_t __xdata*) LCD_ADDRESS;
__xdata uint8_t* const lcd_data = (uint8_t __xdata*) (LCD_ADDRESS + 1);
/* cursor_y and cursor_x globals*/
uint16_t cursor_x;
uint16_t cursor_y;
/* textsize and rotation*/
uint8_t textsize;
uint8_t rotation;
/* text color and background */
uint16_t foregroundColor;
uint16_t backgroundColor;
/* lcd width and height */
uint16_t _width;
uint16_t _height;

__code uint8_t intro1[9] = {"Welcome\n",0};
__code uint8_t intro2[11] = {"ECEN-433\n",0};
__code uint8_t intro3[11] = {"Rotation 0",0};
__code uint8_t intro4[11] = {"Rotation 1",0};
__code uint8_t intro5[11] = {"Rotation 2",0};
__code uint8_t intro6[11] = {"Rotation 3",0};

void delay (int16_t d)
{
    int16_t i, j = 0; /* declare for backwards compatibility with older C standards */
    for (i=0;i<d;i++) /* this is For(); loop delay used to define delay value in Embedded C*/
    {
        for (j=0;j<80;j++)
        {
        }
    }
}


inline void lcdWrite8Reg(uint8_t d) {
    IOM = 1;
    *lcd_cmd = d;
    IOM = 0;
}
inline void lcdWrite8Data(uint8_t d) {
    IOM = 1;
    *lcd_data = d;
    IOM = 0;
}

inline void lcdWrite8DataBlocking(uint8_t d)
{
    *lcd_data = d;
}
inline void lcdWriteRegData8(uint8_t a, uint8_t d) {
    IOM = 1;
    *lcd_cmd = a;
    *lcd_data = d;
    IOM = 0;
}

uint16_t getCursorX(void)
{
    return cursor_x;
}

uint16_t getCursorY(void)
{
    return cursor_y;
}

uint8_t getTextSize(void)
{
    return textsize;
}

void setCursorX(uint16_t x)
{
    cursor_x = x;
}

void setCursorY(uint16_t y)
{
    cursor_y = y;
}

void setCursor(uint16_t x, uint16_t y){
    cursor_x = x;
    cursor_y = y;
}

void lcdWriteRegData16(uint16_t a, uint16_t d){
    uint8_t hi, lo;
    hi = (a) >> 8;
    lo = (a);
    lcdWrite8Reg(hi);
    lcdWrite8Reg(lo);
    hi = (d) >> 8;
    lo = (d);
    lcdWrite8Data(hi);
    lcdWrite8Data(lo);
}

void setForegroundColor(uint16_t color) {
    foregroundColor = color;
}

void setBackgroundColor(uint16_t color) {
    backgroundColor = color;
}

void setTextSize(uint8_t s){
    if (s > 8) return;
    textsize = (s>0) ? s : 1 ;
}

void setRotation(uint8_t flag){
    rotation = flag;
    switch(flag) {
        case 0:
            flag = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
            _width = TFTWIDTH;
            _height = TFTHEIGHT;
            break;
        case 1:
            flag = (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
            _width = TFTHEIGHT;
            _height = TFTWIDTH;
            break;
        case 2:
            flag = (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
            _width = TFTWIDTH;
            _height = TFTHEIGHT;
            break;
        case 3:
            flag = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
            _width = TFTHEIGHT;
            _height = TFTWIDTH;
            break;
        default:
            flag = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
            _width = TFTWIDTH;
            _height = TFTHEIGHT;
            break;
    }
    lcdWriteRegData8(ILI9341_MEMCONTROL, flag);
}


void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
    lcdWrite8Reg(ILI9341_COLADDRSET);
    lcdWrite8Data(x1 >> 8);
    lcdWrite8Data(x1);
    lcdWrite8Data(x2 >> 8);
    lcdWrite8Data(x2);

    lcdWrite8Reg(ILI9341_PAGEADDRSET);
    lcdWrite8Data(y1 >> 8);
    lcdWrite8Data(y1);
    lcdWrite8Data(y2 >> 8);
    lcdWrite8Data(y2);
}

void tftLcdInit(void){
    _width = TFTWIDTH;
    _height = TFTHEIGHT;

    lcdWrite8Reg(ILI9341_MADCTL_RGB);
    lcdWrite8Data(0x00);
    lcdWrite8Data(0x00);
    lcdWrite8Data(0x00);

    delay(200);

    lcdWrite8Reg(ILI9341_SOFTRESET);
    delay(50);
    lcdWrite8Reg(ILI9341_DISPLAYOFF);
    delay(10);

    lcdWriteRegData8(ILI9341_POWERCONTROL1, 0x23);
    lcdWriteRegData8(ILI9341_POWERCONTROL2, 0x11);
    lcdWrite8Reg(ILI9341_VCOMCONTROL1);
    lcdWrite8Data(0x3d);
    lcdWrite8Data(0x30);
    lcdWriteRegData8(ILI9341_VCOMCONTROL2, 0xaa);
    lcdWriteRegData8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    lcdWrite8Reg(ILI9341_PIXELFORMAT);
    lcdWrite8Data(0x55);lcdWrite8Data(0x00);
    lcdWriteRegData16(ILI9341_FRAMECONTROL, 0x001B);

    lcdWriteRegData8(ILI9341_ENTRYMODE, 0x07);

    lcdWrite8Reg(ILI9341_SLEEPOUT);
    delay(150);
    lcdWrite8Reg(ILI9341_DISPLAYON);
    delay(500);
    setAddress(0,0,_width-1,_height-1);
    /************* Start Initial Sequence ILI9341 controller **********/
}

void drawPixel(uint16_t x3,uint16_t y3,uint16_t color1)
{
    setAddress(x3,y3,x3+1,y3+1);
    lcdWrite8Reg(ILI9341_MEMORYWRITE);
    lcdWrite8Data(color1>>8);
    lcdWrite8Data(color1);
}



void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;


    drawPixel(x0  , y0+r, color);
    drawPixel(x0  , y0-r, color);
    drawPixel(x0+r, y0  , color);
    drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void testCircles(uint8_t radius, uint16_t color) {

    int  x, y, r2 = radius * 2, w = _width  + radius, h = _height + radius;

    for(x=0; x<w; x+=r2) {
        for(y=0; y<h; y+=r2) {
            drawCircle(x, y, radius, color);
        }
    }


}
void fillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color){
    if ((x >= _width) || (y >= _height))
    {
        return;
    }

    if ((x+w-1) >= _width)
    {
        w = _width-x;
    }

    if ((y+h-1) >= _height)
    {
        h = _height-y;
    }

    setAddress(x, y, x+w-1, y+h-1);


    lcdWrite8Reg(ILI9341_MEMORYWRITE);
    for(y=h; y>0; y--)
    {
        for(x=w; x>0; x--)
        {
            lcdWrite8Data(color>>8);
            lcdWrite8Data(color);
        }
    }
}

void fillScreen(void){
    long len = 76800;
    int blocks = 1200;
    uint8_t  i, hi =backgroundColor >> 8;
    uint8_t lo = backgroundColor;
    setAddress(0,0,_width-1,_height-1);

    lcdWrite8Reg(ILI9341_MEMORYWRITE);
    lcdWrite8Data(hi);
    lcdWrite8Data(lo);
    len--;
    IOM = 1;
    while(blocks--) {
        i = 16;
        do {
            *lcd_data = hi;*lcd_data = lo;
            *lcd_data = hi;*lcd_data = lo;
            *lcd_data = hi;*lcd_data = lo;
            *lcd_data = hi;*lcd_data = lo;
        } while(--i);
    }
    for(i = (char)len & 63; i--; ) {
        *lcd_data = hi;*lcd_data = lo;
    }
    IOM = 0;

}
void drawChar(uint8_t c){
    int16_t x = cursor_x;
    int16_t y = cursor_y;
    int8_t size = textsize;

    int8_t i, j;
    uint8_t line;
    for (i=0; i<6; i++)
    {

        if (i == 5)
        {
            line = 0x0;
        }
        else
    {
            line = pgm_read_byte(font+(c*5)+i);
        }
        for (j = 0; j<8; j++)
        {
            if (line & 0x1)
            {
                if (size == 1)
                {
                    drawPixel(x+i, y+j, foregroundColor);
                }
                else {
                    fillRect(x+(i*size), y+(j*size), size, size, foregroundColor);
                }
            } else if (backgroundColor != foregroundColor)
            {
                if (size == 1)
                {
                    drawPixel(x+i, y+j, backgroundColor);
                }
                else
            {
                    fillRect(x+i*size, y+j*size, size, size, backgroundColor);
                }
            }

            line >>= 1;
        }
    }
}

void printChar(char c)
{
    if (c == '\n')
    {
        cursor_y += textsize*8;
        cursor_x  = 0;
    }
    else if (c == '\r')
    {

    }
    else
{
        drawChar((uint8_t) c);
        cursor_x += textsize*6;
    }
}
void print(char *str)
{
    while(*str != 0)    /* Send each char of string till the NULL */
    {
        printChar(*str);    /* Call transmit data function */
        str++;
    }
}

void println(char *str)
{
    print(str);
    printChar('\n');
}

uint16_t reverse(uint8_t d) {
    uint16_t rev = 0;
    uint16_t val = 0;
    while(d >= 1){

        val = d%10;
        d = d/10;
        rev = rev * 10 + val;
    }
    return rev;
}

void printByteDecimal(uint8_t d) {
    uint8_t val;
    uint16_t id;
    id = reverse(d);
    while (id >= 1){

        val = id % 10;
        id = id/10;
        printChar(val + '0');
    }
}

void printNibbleHexadecimal(uint8_t d) {
    printChar((d & 0x0F) + (((d & 0x0F) < 10) ? 0x30 : 0x37));
}

void printByteHexadecimal(uint8_t d) {
    printNibbleHexadecimal(d >> 4);
    printNibbleHexadecimal(d);
}

void writeSomeLines(void){
    backgroundColor = RED;
    fillScreen();
    delay(20);
    backgroundColor = GREEN;
    fillScreen();
    delay(20);
    setRotation(0);
    backgroundColor = BLACK;
    fillScreen();
    testCircles(20, RED);
    delay(100);
    fillScreen();
    setTextSize(5);
    setForegroundColor(CYAN);
    print(intro1);
    setTextSize(3);
    print(intro2);
    print(intro3);
    delay(150);
    fillScreen();
    setRotation(1);
    setCursor(0,0);
    print(intro4);
    delay(150);
    fillScreen();
    setRotation(2);
    setCursor(0,0);
    print(intro5);
    delay(150);
    fillScreen();
    setRotation(3);
    setCursor(0,0);
    delay(150);
    print(intro6);
    delay(150);
    fillScreen();
}

