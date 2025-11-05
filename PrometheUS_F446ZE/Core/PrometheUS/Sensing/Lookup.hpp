#pragma once

static float lookup1d(const float p_X, const float p_XTable[], const float p_YTable[], const uint16_t p_TableLength)
{
  float slope;
  uint16_t indexRight;
  uint16_t indexLeft;
  uint16_t binarySearchIndex;
  float result = 0.0f;

  uint16_t maxIndex = p_TableLength - 1;

  if (p_X <= p_XTable[0])
  {
    result = p_YTable[0];
  }
  else if (p_X < p_XTable[maxIndex])
  {
    binarySearchIndex = maxIndex >> 1;
    indexLeft = 0;
    indexRight = maxIndex;

    while (indexRight > (indexLeft + 1))
    {
      if (p_X < p_XTable[binarySearchIndex])
      {
        indexRight = binarySearchIndex;
      }
      else
      {
        indexLeft = binarySearchIndex;
      }

      binarySearchIndex = (indexRight + indexLeft) >> 1;
    }

    slope = (p_YTable[indexRight] - p_YTable[indexLeft]) / (p_XTable[indexRight] - p_XTable[indexLeft]);
    result = (p_X - p_XTable[indexLeft]) * slope + p_YTable[indexLeft];
  }
  else
  {
    result = p_YTable[maxIndex];
  }

  return result;
}
