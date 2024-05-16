#include "games_programs.h"
#include "menu.h"
#include "io.h"
#include "input.h"
#include "lcdDriver.h"
#include "timer.h"

#define HALF_PADDLE 15

void games_pong(void) {
    // Game state
    uint8_t input;

    __xdata uint8_t paddleLeft = 120;
    __xdata uint8_t paddleLeftOld = 120;
    __xdata int8_t paddleLeftVelocity = 0;

    __xdata uint8_t paddleRight = 120;
    __xdata uint8_t paddleRightOld = 120;
    __xdata int8_t paddleRightVelocity = 0;

    __xdata uint16_t ballX = 160;
    __xdata uint8_t ballY = 120;
    __xdata uint16_t ballXOld = 160;
    __xdata uint8_t ballYOld = 120;
    __xdata int8_t ballVelocityX = 2;
    __xdata int8_t ballVelocityY = 1;

    __xdata uint8_t scoreLeft = 0;
    __xdata uint8_t scoreRight = 0;

    // Initialize the game
    setBackgroundColor(BLACK);
    fillScreen();
    setForegroundColor(GRAY);
    setTextSize(2);
    setCursor(130, 0);
    printByteHexadecimal(scoreLeft);
    printChar(':');
    printByteHexadecimal(scoreRight);

    // Initialize ball and paddles
    fillRect(0, paddleLeft - HALF_PADDLE, 4, 2 * HALF_PADDLE, WHITE);
    fillRect(TFTHEIGHT - 4, paddleRight - HALF_PADDLE, 4, 2 * HALF_PADDLE, WHITE);
    fillRect(ballX - 2, ballY - 2, 4, 4, WHITE);

    // Game loop
    while (1) {
        // Wait for frame time
        input = 0;
        while (input != 3) {
            input += hasFrameElapsed();
        }

        // Get input
        input = inputGetNES();
        if (input & NES_UP) {
            paddleLeftVelocity = -2;
        } else if (input & NES_DOWN) {
            paddleLeftVelocity = 2;
        } else {
            paddleLeftVelocity = 0;
        }

        if (input & NES_START) break;

        // Control *AI*
        if (ballY < paddleRight) {
            paddleRightVelocity = -1;
        } else if (ballY > paddleRight) {
            paddleRightVelocity = 1;
        } else {
            paddleRightVelocity = 0;
        }

        // Update game state
        paddleLeftOld = paddleLeft;
        paddleLeft += paddleLeftVelocity;
        if (paddleLeft < HALF_PADDLE) {
            paddleLeft = HALF_PADDLE;
        } else if (paddleLeft > TFTWIDTH - HALF_PADDLE) {
            paddleLeft = TFTWIDTH - HALF_PADDLE;
        }
        paddleRightOld = paddleRight;
        paddleRight += paddleRightVelocity;
        if (paddleRight < HALF_PADDLE) {
            paddleRight = HALF_PADDLE;
        } else if (paddleRight > TFTWIDTH - HALF_PADDLE) {
            paddleRight = TFTWIDTH - HALF_PADDLE;
        }

        ballXOld = ballX;
        ballYOld = ballY;
        ballX += ballVelocityX;
        ballY += ballVelocityY;

        // Handle potential collisions
        if (ballY < 3 || ballY > TFTWIDTH - 3) {
            ballVelocityY = -ballVelocityY;
        }
        if (ballX < 5) {
            if (ballY > paddleLeft - HALF_PADDLE && ballY < paddleLeft + HALF_PADDLE) {
                ballVelocityX = 2;
                ballX = 6;
            } else {
                scoreRight++;
                ballX = 160;
                ballY = 120;
                ballVelocityX = 2;
                ballVelocityY = 1;
                setCursor(130, 0);
                printByteHexadecimal(scoreLeft);
                printChar(':');
                printByteHexadecimal(scoreRight);
            }
        } else if (ballX > TFTHEIGHT - 5) {
            if (ballY > paddleRight - HALF_PADDLE && ballY < paddleRight + HALF_PADDLE) {
                ballVelocityX = -2;
                ballX = TFTHEIGHT - 6;
            } else {
                scoreLeft++;
                ballX = 160;
                ballY = 120;
                ballVelocityX = -2;
                ballVelocityY = -1;
                setCursor(130, 0);
                printByteHexadecimal(scoreLeft);
                printChar(':');
                printByteHexadecimal(scoreRight);
            }
        }

        // Update the screen
        if (paddleLeft != paddleLeftOld) {
            if (paddleLeftVelocity > 0) {
                fillRect(0, paddleLeftOld - HALF_PADDLE, 4, paddleLeft - paddleLeftOld, BLACK);
                fillRect(0, paddleLeftOld + HALF_PADDLE, 4, paddleLeft - paddleLeftOld, WHITE);
            } else if (paddleLeftVelocity < 0) {
                fillRect(0, paddleLeft + HALF_PADDLE, 4, paddleLeftOld - paddleLeft, BLACK);
                fillRect(0, paddleLeft - HALF_PADDLE, 4, paddleLeftOld - paddleLeft, WHITE);
            }
        }
        if (paddleRight != paddleRightOld) {
            if (paddleRightVelocity > 0) {
                fillRect(TFTHEIGHT - 4, paddleRightOld - HALF_PADDLE, 4, paddleRight - paddleRightOld, BLACK);
                fillRect(TFTHEIGHT - 4, paddleRightOld + HALF_PADDLE, 4, paddleRight - paddleRightOld, WHITE);
            } else if (paddleRightVelocity < 0) {
                fillRect(TFTHEIGHT - 4, paddleRight + HALF_PADDLE, 4, paddleRightOld - paddleRight, BLACK);
                fillRect(TFTHEIGHT - 4, paddleRight - HALF_PADDLE, 4, paddleRightOld - paddleRight, WHITE);
            }
        }
        if (ballX != ballXOld || ballY != ballYOld) {
            fillRect(ballXOld - 2, ballYOld - 2, 4, 4, BLACK);
            fillRect(ballX - 2, ballY - 2, 4, 4, WHITE);
        }

        // Win condition
        if (scoreLeft == 9 || scoreRight == 9) {
            break;
        }
    }
    return;
}

void games_snake(void) {
    return;
}
