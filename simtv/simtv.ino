/*
 * simtv - an Arduino TV Simulator
 *
 * Copyright (C) 2018 Richard "Shred" Körber
 *   https://github.com/shred/simtv
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stream.h"

// Pin assignment for a Velleman KA01 RGB shield.
// Change here if you use a different shield.
const int RED   =  3;   // Pin 3 = red
const int GREEN =  5;   // Pin 5 = green
const int BLUE  =  6;   // Pin 6 = blue

const int LED   = 13;   // Arduino's LED

const unsigned int TARGET_FPS = 30;  // Desired target frame rate

const unsigned int DATA_SIZE = sizeof(DATA) / 2;

int cr, cg, cb;
int ix;
unsigned long startTimeMs;
unsigned long currentFrame;
byte gamma[sizeof(GAMMA)];

/* SETUP: Reset the device */
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  // Copy gamma table to local memory
  for (int ix = 0; ix < sizeof(GAMMA); ix++) {
    gamma[ix] = pgm_read_word_near(GAMMA + ix);
  }
}

/* Wait the given number of target frames */
void waitFrames(unsigned long frames) {
  currentFrame += frames;
  unsigned long playPositionMs = currentFrame * 1000 / TARGET_FPS;
  long ms = playPositionMs + startTimeMs - millis();
  if (ms > 0) {
    delay(ms);
  }
}

/* Set the RGB colors */
void setColor(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

/* Change the color in the given number of steps */
void changeColor(unsigned int r, unsigned int g, unsigned int b, unsigned int step) {
  if (step == 0) {
    setColor(r, g, b);
    waitFrames(TARGET_FPS / FPS);

  } else if (cr == r && cg == g && cb == b) {
    waitFrames((step + 1) * TARGET_FPS / FPS);

  } else {
    unsigned int limit = (step + 1) * TARGET_FPS / FPS;

    for (unsigned int cnt = 1; cnt <= limit; cnt++) {
      unsigned int remain = limit - cnt;
      unsigned int rn = ((r * cnt) + (cr * remain)) / limit;
      unsigned int gn = ((g * cnt) + (cg * remain)) / limit;
      unsigned int bn = ((b * cnt) + (cb * remain)) / limit;
      setColor(rn, gn, bn);
      waitFrames(1);
    }
  }

  cr = r;
  cg = g;
  cb = b;
}

/* MAIN LOOP */
void loop() {
  startTimeMs = millis();
  currentFrame = 0;

  // Play back the stream
  for (ix = 0; ix < DATA_SIZE; ix++) {
    unsigned int val = pgm_read_word_near(DATA + ix);
    unsigned int step = (val >> 12) & 0x0F;

    int r = gamma[(val >> 8) & 0x0F];
    int g = gamma[(val >> 4) & 0x0F];
    int b = gamma[ val       & 0x0F];

    changeColor(r, g, b, step);
  }

  // Reset output lines
  setColor(0, 0, 0);
}
