#include "screen.h"
#include <SPI.h>

#define GRAY_LEVELS 64 // must be a power of two
#define TIMER_INTERVAL_US 30

using namespace std;

void Screen_::setRenderBuffer(const uint8_t *renderBuffer, bool grays)
{
  if(grays) {
    memcpy(renderBuffer_, renderBuffer, ROWS*COLS);
  } else {
    for (int i = 0; i < ROWS * COLS; i++)
    {
      this->renderBuffer_[i] = renderBuffer[i] * 255;
    }
  }
}

uint8_t *Screen_::getRotatedRenderBuffer()
{
  for (int i = 0; i < ROWS * COLS; i++)
  {
    this->rotatedRenderBuffer_[i] = this->renderBuffer_[i];
  }

  this->rotate();

  return this->rotatedRenderBuffer_;
}

uint8_t *Screen_::getRenderBuffer()
{
  return this->renderBuffer_;
}

uint8_t Screen_::getBufferIndex(int index)
{
  return this->renderBuffer_[index];
}

void Screen_::loadFromStorage()
{
#ifdef ENABLE_STORAGE
  storage.begin("led-wall", false);
  if (currentMode == NONE)
  {
    this->clear();
    storage.getBytes("data", this->renderBuffer_, ROWS * COLS);
    this->render();
  }
  else
  {
    storage.getBytes("data", this->cache, ROWS * COLS);
  }
  this->setBrightness(storage.getUInt("brightness", 255));
  storage.end();
#endif
}

void Screen_::rotate()
{
  for (int row = 0; row < ROWS / 2; row++)
  {
    for (int col = row; col < COLS - row - 1; col++)
    {
      for (int r = 0; r < this->currentRotation; r++)
      {
        swap(this->rotatedRenderBuffer_[row * ROWS + col], this->rotatedRenderBuffer_[col * ROWS + (ROWS - 1 - row)]);
        swap(this->rotatedRenderBuffer_[row * ROWS + col], this->rotatedRenderBuffer_[(ROWS - 1 - row) * ROWS + (ROWS - 1 - col)]);
        swap(this->rotatedRenderBuffer_[row * ROWS + col], this->rotatedRenderBuffer_[(ROWS - 1 - col) * ROWS + row]);
      }
    }
  }
}

void Screen_::clear(bool rerender)
{
  memset(this->renderBuffer_, 0, ROWS * COLS);
  if (rerender)
  {
    this->render();
  }
}

void Screen_::persist()
{
#ifdef ENABLE_STORAGE
  storage.begin("led-wall", false);
  storage.putBytes("data", this->renderBuffer_, ROWS * COLS);
  storage.putUInt("brightness", this->brightness);
  storage.end();
#endif
}

void Screen_::setPixelAtIndex(uint8_t index, uint8_t value)
{
  if (index >= 0 && index < COLS * ROWS)
  {
    this->renderBuffer_[index] = value * 255;
  }
}

void Screen_::setPixel(uint8_t x, uint8_t y, uint8_t value)
{
  if (x >= 0 && y >= 0 && x < 16 && y < 16)
  {
    this->renderBuffer_[y * 16 + x] = value * 255;
  }
}

void IRAM_ATTR Screen_::onScreenTimer(){
  Screen._render();
}

void Screen_::setup()
{
  // TODO find proper unused pins for MISO and SS
  SPI.begin(PIN_CLOCK, 34, PIN_DATA, 25); //SCLK, MISO, MOSI, SS
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

  hw_timer_t *Screen_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(Screen_timer, &onScreenTimer, true);
  timerAlarmWrite(Screen_timer, TIMER_INTERVAL_US, true);
  timerAlarmEnable(Screen_timer);
}

void Screen_::render()
{
  // no-op now, see _render() which is called automatically in a timer routine
}

void Screen_::_render()
{
  // TODO restore rotation ability (but efficiently)
  const auto buf = this->renderBuffer_;

  static byte bits[ROWS*COLS/8] = { 0 };
  memset(bits, 0, ROWS*COLS/8);

  static byte counter = 0;

  for (int idx = 0; idx < ROWS*COLS; idx++) {
    bits[idx >> 3] |= (buf[positions[idx]] > counter ? 0x80 : 0) >> (idx & 7);
  }

  counter += (256/GRAY_LEVELS);

  digitalWrite(PIN_LATCH, LOW);

  SPI.writeBytes(bits, sizeof(bits));

  digitalWrite(PIN_LATCH, HIGH);
}

void Screen_::cacheCurrent()
{
  for (int i = 0; i < COLS * ROWS; i++)
  {
    this->cache[i] = this->renderBuffer_[i];
  }
}

void Screen_::restoreCache()
{
  this->setRenderBuffer(this->cache);
}

void Screen_::drawLine(int x1, int y1, int x2, int y2, int ledStatus)
{
  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int error = (dx > dy ? dx : -dy) / 2;

  while (x1 != x2 || y1 != y2)
  {
    this->setPixel(x1, y1, ledStatus);

    int error2 = error;
    if (error2 > -dx)
    {
      error -= dy;
      x1 += sx;
      setPixel(x1, y1, ledStatus);
    }

    else if (error2 < dy)
    {
      error += dx;
      y1 += sy;
      setPixel(x1, y1, ledStatus);
    }
  };
};

void Screen_::drawRectangle(int x, int y, int width, int height, bool fill, int ledStatus)
{
  if (!fill)
  {
    this->drawLine(x, y, x + width, y, ledStatus);
    this->drawLine(x, y + 1, x, y + height - 1, ledStatus);
    this->drawLine(x + width, y + 1, x + width, y + height - 1, ledStatus);
    this->drawLine(x, y + height - 1, x + width, y + height - 1, ledStatus);
  }
  else
  {
    for (int i = x; i < x + width; i++)
    {
      this->drawLine(i, y, i, y + height - 1, ledStatus);
    }
  }
};

void Screen_::drawCharacter(int x, int y, std::vector<int> bits, int bitCount)
{
  for (int i = 0; i < bits.size(); i += bitCount)
  {
    for (int j = 0; j < bitCount; j++)
    {
      setPixel(x + j, (y + (i / bitCount)), bits[i + j]);
    }
  }
}

std::vector<int> Screen_::readBytes(std::vector<int> bytes)
{
  vector<int> bits;
  int k = 0;

  for (int i = 0; i < bytes.size(); i++)
  {
    for (int j = 8 - 1; j >= 0; j--)
    {
      int b = (bytes[i] >> j) & 1;
      bits.push_back(b);
      k++;
    }
  }

  return bits;
};

void Screen_::drawNumbers(int x, int y, std::vector<int> numbers)
{
  for (int i = 0; i < numbers.size(); i++)
  {
    this->drawCharacter(x + (i * 5), y, this->readBytes(smallNumbers[numbers.at(i)]), 4);
  }
}

unsigned int Screen_::getCurrentBrightness() const
{
  return this->brightness;
}

void Screen_::setBrightness(unsigned int brightness)
{
  this->brightness = brightness;
  analogWrite(PIN_ENABLE, 255 - brightness);
}

void Screen_::drawBigNumbers(int x, int y, std::vector<int> numbers)
{
  for (int i = 0; i < numbers.size(); i++)
  {
    this->drawCharacter(x + (i * 8), y, this->readBytes(bigNumbers[numbers.at(i)]), 8);
  }
}

void Screen_::drawWeather(int x, int y, int weather)
{
  this->drawCharacter(x, y, this->readBytes(weatherIcons[weather]), 16);
}

Screen_ &Screen_::getInstance()
{
  static Screen_ instance;
  return instance;
}

Screen_ &Screen = Screen.getInstance();
