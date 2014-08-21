#pragma once

#include "drape_global.hpp"
#include "pointers.hpp"
#include "texture.hpp"

#include "../base/buffer_vector.hpp"

#include "../geometry/point2d.hpp"
#include "../geometry/rect2d.hpp"

#include "../std/map.hpp"

namespace dp
{

class StipplePenKey : public Texture::Key
{
public:
  virtual Texture::ResourceType GetType() const { return Texture::StipplePen; }

  buffer_vector<uint8_t, 8> m_pattern;
};

class StipplePenHandle
{
public:
  StipplePenHandle(uint64_t value) : m_keyValue(value) {} // don't use this ctor. Only for tests
  StipplePenHandle(buffer_vector<uint8_t, 8> const & pattern);
  StipplePenHandle(StipplePenKey const & info);

  bool operator == (StipplePenHandle const & other) const { return m_keyValue == other.m_keyValue; }
  bool operator < (StipplePenHandle const & other) const { return m_keyValue < other.m_keyValue; }

private:
  void Init(buffer_vector<uint8_t, 8> const & pattern);

private:
  friend string DebugPrint(StipplePenHandle const & );
  uint64_t m_keyValue;
};

class StipplePenRasterizator
{
public:
  StipplePenRasterizator() : m_pixelLength(0) {}
  StipplePenRasterizator(StipplePenKey const & key);

  uint32_t GetSize() const;
  uint32_t GetBufferSize() const;

  void Rasterize(void * buffer);

private:
  StipplePenKey m_key;
  uint32_t m_pixelLength;
};

class StipplePenResourceInfo : public Texture::ResourceInfo
{
public:
  StipplePenResourceInfo(m2::RectF const & texRect, uint32_t pixelLength)
    : Texture::ResourceInfo(texRect)
    , m_pixelLength(pixelLength)
  {
  }

  virtual Texture::ResourceType GetType() const { return Texture::StipplePen; }
  uint32_t GetPixelLength() const { return m_pixelLength; }

private:
  uint32_t m_pixelLength;
};

class StipplePenPacker
{
public:
  StipplePenPacker(m2::PointU const & canvasSize);

  m2::RectU PackResource(uint32_t width);
  m2::RectF MapTextureCoords(m2::RectU const & pixelRect) const;

private:
  m2::PointU m_canvasSize;
  buffer_vector<uint32_t, 4> m_columns;
  uint32_t m_currentColumn;
};

class StipplePenIndex
{
public:
  StipplePenIndex(m2::PointU const & canvasSize) : m_packer(canvasSize) {}
  StipplePenResourceInfo const * MapResource(StipplePenKey const & key);
  void UploadResources(RefPointer<Texture> texture);

private:
  typedef MasterPointer<StipplePenResourceInfo> TResourcePtr;
  typedef map<StipplePenHandle, TResourcePtr> TResourceMapping;
  typedef pair<m2::RectU, StipplePenRasterizator> TPendingNode;
  typedef buffer_vector<TPendingNode, 32> TPendingNodes;

  TResourceMapping m_resourceMapping;
  TPendingNodes m_pendingNodes;
  StipplePenPacker m_packer;
};

string DebugPrint(StipplePenHandle const & key);

}